/******************************************************************************
Module:  SparseStream.h
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#include "..\CmnHdr.h"     /* See Appendix A. */
#include <WinIoCtl.h>


///////////////////////////////////////////////////////////////////////////////


#pragma once


///////////////////////////////////////////////////////////////////////////////


class CSparseStream {
public:
   static BOOL DoesFileSystemSupportSparseStreams(PCTSTR pszVolume);
   static BOOL DoesFileContainAnySparseStreams(PCTSTR pszPathname);

public:
   CSparseStream(HANDLE hstream = INVALID_HANDLE_VALUE) { 
      Initialize(hstream); 
   }

   virtual ~CSparseStream() { }

   void Initialize(HANDLE hstream = INVALID_HANDLE_VALUE) { 
      m_hstream = hstream; 
   }

public:
   operator HANDLE() const { return(m_hstream); }

public:
   BOOL IsStreamSparse() const;
   BOOL MakeSparse();
   BOOL DecommitPortionOfStream(
      __int64 qwFileOffsetStart, __int64 qwFileOffsetEnd);

   FILE_ALLOCATED_RANGE_BUFFER* QueryAllocatedRanges(PDWORD pdwNumEntries);
   BOOL FreeAllocatedRanges(FILE_ALLOCATED_RANGE_BUFFER* pfarb);

private:
   HANDLE m_hstream;

private:
   static BOOL AreFlagsSet(DWORD fdwFlagBits, DWORD fFlagsToCheck) {
      return((fdwFlagBits & fFlagsToCheck) == fFlagsToCheck);
   }
};


///////////////////////////////////////////////////////////////////////////////


inline BOOL CSparseStream::DoesFileSystemSupportSparseStreams(
   PCTSTR pszVolume) {

   DWORD dwFileSystemFlags = 0;
   BOOL fOk = GetVolumeInformation(pszVolume, NULL, 0, NULL, NULL, 
      &dwFileSystemFlags, NULL, 0);
   fOk = fOk && AreFlagsSet(dwFileSystemFlags, FILE_SUPPORTS_SPARSE_FILES);
   return(fOk);
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CSparseStream::IsStreamSparse() const {

   BY_HANDLE_FILE_INFORMATION bhfi;
   GetFileInformationByHandle(m_hstream, &bhfi);
   return(AreFlagsSet(bhfi.dwFileAttributes, FILE_ATTRIBUTE_SPARSE_FILE));
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CSparseStream::MakeSparse() {

   DWORD dw;
   return(DeviceIoControl(m_hstream, FSCTL_SET_SPARSE, 
      NULL, 0, NULL, 0, &dw, NULL));
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CSparseStream::DecommitPortionOfStream(
   __int64 qwOffsetStart, __int64 qwOffsetEnd) {

   // NOTE: This function does not work if this file is memory-mapped.
   DWORD dw;
   FILE_ZERO_DATA_INFORMATION fzdi;
   fzdi.FileOffset.QuadPart = qwOffsetStart;
   fzdi.BeyondFinalZero.QuadPart = qwOffsetEnd + 1;
   return(DeviceIoControl(m_hstream, FSCTL_SET_ZERO_DATA, (PVOID) &fzdi, 
      sizeof(fzdi), NULL, 0, &dw, NULL));
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CSparseStream::DoesFileContainAnySparseStreams(
   PCTSTR pszPathname) {

   DWORD dw = GetFileAttributes(pszPathname);
   return((dw == 0xfffffff) 
      ? FALSE : AreFlagsSet(dw, FILE_ATTRIBUTE_SPARSE_FILE));
}


///////////////////////////////////////////////////////////////////////////////


inline FILE_ALLOCATED_RANGE_BUFFER* CSparseStream::QueryAllocatedRanges(
   PDWORD pdwNumEntries) {

   FILE_ALLOCATED_RANGE_BUFFER farb;
   farb.FileOffset.QuadPart = 0;
   farb.Length.LowPart = 
      GetFileSize(m_hstream, (PDWORD) &farb.Length.HighPart);

   // There is no way to determine the correct memory block size prior to 
   // attempting to collect this data, so I just picked 100 * sizeof(*pfarb)
   DWORD cb = 100 * sizeof(farb);
   FILE_ALLOCATED_RANGE_BUFFER* pfarb = (FILE_ALLOCATED_RANGE_BUFFER*) 
      HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, cb);

   DeviceIoControl(m_hstream, FSCTL_QUERY_ALLOCATED_RANGES,
      &farb, sizeof(farb), pfarb, cb, &cb, NULL);
   *pdwNumEntries = cb / sizeof(*pfarb);
   return(pfarb);
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CSparseStream::FreeAllocatedRanges(
   FILE_ALLOCATED_RANGE_BUFFER* pfarb) {

   // Free the queue entry's allocated memory
   return(HeapFree(GetProcessHeap(), 0, pfarb));
}


///////////////////////////////// End Of File /////////////////////////////////
