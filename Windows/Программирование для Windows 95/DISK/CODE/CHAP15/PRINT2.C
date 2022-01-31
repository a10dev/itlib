/*------------------------------------------
   PRINT2.C -- Printing with Abort Function
               (c) Charles Petzold, 1996
  ------------------------------------------*/

#include <windows.h>

HDC  GetPrinterDC (void) ;              // in PRINT.C
void PageGDICalls (HDC, int, int) ;

HINSTANCE hInst ;
char      szAppName[] = "Print2" ;
char      szCaption[] = "Print Program 2 (Abort Function)" ;

BOOL CALLBACK AbortProc (HDC hdcPrn, int iCode)
     {
     MSG msg ;

     while (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
          {
          TranslateMessage (&msg) ;
          DispatchMessage (&msg) ;
          }
     return TRUE ;
     }

BOOL PrintMyPage (HWND hwnd)
     {
     static DOCINFO di     = { sizeof (DOCINFO), "Print2: Printing", NULL } ;
     BOOL           bError = FALSE ;
     HDC            hdcPrn ;
     short          xPage, yPage ;

     if (NULL == (hdcPrn = GetPrinterDC ()))
          return TRUE ;

     xPage = GetDeviceCaps (hdcPrn, HORZRES) ;
     yPage = GetDeviceCaps (hdcPrn, VERTRES) ;

     EnableWindow (hwnd, FALSE) ;

     SetAbortProc (hdcPrn, AbortProc) ;

     if (StartDoc (hdcPrn, &di) > 0)
          {
          if (StartPage (hdcPrn) > 0)
               {
               PageGDICalls (hdcPrn, xPage, yPage) ;

               if (EndPage (hdcPrn) > 0)
                    bError = TRUE ;
               }
          }
     else
          bError = TRUE ;

     if (!bError)
          EndDoc (hdcPrn) ;

     EnableWindow (hwnd, TRUE) ;
     DeleteDC (hdcPrn) ;
     return bError ;
     }
