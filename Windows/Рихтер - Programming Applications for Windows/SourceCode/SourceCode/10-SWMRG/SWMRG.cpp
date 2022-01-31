/******************************************************************************
Module:  SWMRG.cpp
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#include "..\CmnHdr.h"     /* See Appendix A. */
#include "SWMRG.h"


///////////////////////////////////////////////////////////////////////////////


CSWMRG::CSWMRG() {

   // Initially no readers want access, no writers want access, and 
   // no threads are accessing the resource
   m_nWaitingReaders = m_nWaitingWriters = m_nActive = 0;
   m_hsemReaders = CreateSemaphore(NULL, 0, MAXLONG, NULL);
   m_hsemWriters = CreateSemaphore(NULL, 0, MAXLONG, NULL);
   InitializeCriticalSection(&m_cs);
}


///////////////////////////////////////////////////////////////////////////////


CSWMRG::~CSWMRG() {

#ifdef _DEBUG
   // A SWMRG shouldn't be destroyed if any threads are using the resource
   if (m_nActive != 0)
      DebugBreak();
#endif

   m_nWaitingReaders = m_nWaitingWriters = m_nActive = 0;
   DeleteCriticalSection(&m_cs);
   CloseHandle(m_hsemReaders);
   CloseHandle(m_hsemWriters);
}


///////////////////////////////////////////////////////////////////////////////


VOID CSWMRG::WaitToRead() {

   // Ensure exclusive access to the member variables
   EnterCriticalSection(&m_cs);

   // Are there writers waiting or is a writer writing?
   BOOL fResourceWritePending = (m_nWaitingWriters || (m_nActive < 0));

   if (fResourceWritePending) {

      // This reader must wait, increment the count of waiting readers
      m_nWaitingReaders++;
   } else {

      // This reader can read, increment the count of active readers
      m_nActive++;
   }

   // Allow other threads to attempt reading/writing
   LeaveCriticalSection(&m_cs);

   if (fResourceWritePending) {

      // This thread must wait
      WaitForSingleObject(m_hsemReaders, INFINITE);
   }
}


///////////////////////////////////////////////////////////////////////////////


VOID CSWMRG::WaitToWrite() {

   // Ensure exclusive access to the member variables
   EnterCriticalSection(&m_cs);

   // Are there any threads accessing the resource?
   BOOL fResourceOwned = (m_nActive != 0);

   if (fResourceOwned) {

      // This writer must wait, increment the count of waiting writers
      m_nWaitingWriters++;
   } else {

      // This writer can write, decrement the count of active writers
      m_nActive = -1;
   }

   // Allow other threads to attempt reading/writing
   LeaveCriticalSection(&m_cs);

   if (fResourceOwned) {

      // This thread must wait
      WaitForSingleObject(m_hsemWriters, INFINITE);
   }
}


///////////////////////////////////////////////////////////////////////////////


VOID CSWMRG::Done() {

   // Ensure exclusive access to the member variables
   EnterCriticalSection(&m_cs);

   if (m_nActive > 0) {

      // Readers have control so a reader must be done
      m_nActive--;
   } else {

      // Writers have control so a writer must be done
      m_nActive++;
   }

   HANDLE hsem = NULL;  // Assume no threads are waiting
   LONG lCount = 1;     // Assume only 1 waiter wakes; always true for writers

   if (m_nActive == 0) {

      // No thread has access, who should wake up?
      // NOTE: It is possible that readers could never get access
      //       if there are always writers wanting to write

      if (m_nWaitingWriters > 0) {

         // Writers are waiting and they take priority over readers
         m_nActive = -1;         // A writer will get access
         m_nWaitingWriters--;    // One less writer will be waiting
         hsem = m_hsemWriters;   // Writers wait on this semaphore
         // NOTE: The semaphore will release only 1 writer thread

      } else if (m_nWaitingReaders > 0) {

         // Readers are waiting and no writers are waiting
         m_nActive = m_nWaitingReaders;   // All readers will get access
         m_nWaitingReaders = 0;           // No readers will be waiting
         hsem = m_hsemReaders;            // Readers wait on this semaphore
         lCount = m_nActive;              // Semaphore releases all readers
      } else {

         // There are no threads waiting at all; no semaphore gets released
      }
   }

   // Allow other threads to attempt reading/writing
   LeaveCriticalSection(&m_cs);

   if (hsem != NULL) {

      // Some threads are to be released
      ReleaseSemaphore(hsem, lCount, NULL);
   }
}


//////////////////////////////// End of File //////////////////////////////////
