/*------------------------------------------------
   STRLIB.C -- Library module for STRPROG program
               (c) Charles Petzold,  1996
  ------------------------------------------------*/

#include <windows.h>
#include "strlib.h"

#pragma data_seg ("shared")

PSTR pszStrings[MAX_STRINGS] = { NULL } ;
int  iTotal = 0 ;

#pragma data_seg ()

int WINAPI DllMain (HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved)
     {
     int i ;

     switch (fdwReason)
          {
          // Nothing to do when process (or thread) begins

          case DLL_PROCESS_ATTACH :
          case DLL_THREAD_ATTACH :
          case DLL_THREAD_DETACH :
               break ;

          // When process terminates, free any remaining blocks

          case DLL_PROCESS_DETACH :
               for (i = 0 ; i < iTotal ; i++)
                    UnmapViewOfFile (pszStrings[i]) ;
               break ;
          }

     return TRUE ;
     }

EXPORT BOOL CALLBACK AddString (PSTR pStringIn)
     {
     HANDLE hString ;
     PSTR   pString ;
     int    i, iLength, iCompare ;

     if (iTotal == MAX_STRINGS - 1)
          return FALSE ;

     iLength = strlen (pStringIn) ;
     if (iLength == 0)
          return FALSE ;

     hString = CreateFileMapping ((HANDLE) -1, NULL, PAGE_READWRITE,
                                  0, 1 + iLength, NULL) ;
     if (hString == NULL)
          return FALSE ;

     pString = (PSTR) MapViewOfFile (hString, FILE_MAP_WRITE, 0, 0, 0) ;
     strcpy (pString, pStringIn) ;
     AnsiUpper (pString) ;

     for (i = iTotal ; i > 0 ; i--)
          {
          iCompare = strcmpi (pStringIn, pszStrings[i - 1]) ;

          if (iCompare >= 0)
               break ;

          pszStrings[i] = pszStrings[i - 1] ;
          }

     pszStrings[i] = pString ;

     iTotal++ ;
     return TRUE ;
     }

EXPORT BOOL CALLBACK DeleteString (PSTR pStringIn)
     {
     int i, j, iCompare ;

     if (0 == strlen (pStringIn))
          return FALSE ;

     for (i = 0 ; i < iTotal ; i++)
          {
          iCompare = lstrcmpi (pszStrings[i], pStringIn) ;

          if (iCompare == 0)
               break ;
          }

     // If given string not in list, return without taking action

     if (i == iTotal)
          return FALSE ;

     // Else free memory occupied by the string and adjust list downward

     UnmapViewOfFile (pszStrings[i]) ;

     for (j = i ; j < iTotal ; j++)
          pszStrings[j] = pszStrings[j + 1] ;

     pszStrings[iTotal--] = NULL ;    // Destroy unused pointer
     return TRUE ;
     }

EXPORT int CALLBACK GetStrings (PSTRCB pfnGetStrCallBack, PVOID pParam)
     {
     BOOL bReturn ;
     int  i ;

     for (i = 0 ; i < iTotal ; i++)
          {
          bReturn = pfnGetStrCallBack (pszStrings[i], pParam) ;

          if (bReturn == FALSE)
               return i + 1 ;
          }
     return iTotal ;
     }
