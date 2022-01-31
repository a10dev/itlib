/*---------------------------------------
   PRINT4.C -- Printing with Banding
               (c) Charles Petzold, 1996
  ---------------------------------------*/

#include <windows.h>

HDC  GetPrinterDC (void) ;              // in PRINT.C
void PageGDICalls (HDC, int, int) ;

HINSTANCE hInst ;
char      szAppName[] = "Print4" ;
char      szCaption[] = "Print Program 4 (Banding)" ;

BOOL   bUserAbort ;
HWND   hDlgPrint ;

BOOL CALLBACK PrintDlgProc (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
     {
     switch (msg)
          {
          case WM_INITDIALOG :
               SetWindowText (hDlg, szAppName) ;
               EnableMenuItem (GetSystemMenu (hDlg, FALSE), SC_CLOSE,
                                                            MF_GRAYED) ;
               return TRUE ;

          case WM_COMMAND :
               bUserAbort = TRUE ;
               EnableWindow (GetParent (hDlg), TRUE) ;
               DestroyWindow (hDlg) ;
               hDlgPrint = 0 ;
               return TRUE ;
          }
     return FALSE ;
     }

BOOL CALLBACK AbortProc (HDC hdcPrn, int iCode)
     {
     MSG   msg ;

     while (!bUserAbort && PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
          {
          if (!hDlgPrint || !IsDialogMessage (hDlgPrint, &msg))
               {
               TranslateMessage (&msg) ;
               DispatchMessage (&msg) ;
               }
          }
     return !bUserAbort ;
     }

BOOL PrintMyPage (HWND hwnd)
     {
     static DOCINFO di     = { sizeof (DOCINFO), "Print4: Printing", NULL } ;
     BOOL           bError = FALSE ;
     HDC            hdcPrn ;
     RECT           rect ;
     int            xPage, yPage ;

     if (NULL == (hdcPrn = GetPrinterDC ()))
          return TRUE ;

     xPage = GetDeviceCaps (hdcPrn, HORZRES) ;
     yPage = GetDeviceCaps (hdcPrn, VERTRES) ;

     EnableWindow (hwnd, FALSE) ;

     bUserAbort = FALSE ;
     hDlgPrint = CreateDialog (hInst, "PrintDlgBox", hwnd, PrintDlgProc) ;

     SetAbortProc (hdcPrn, AbortProc) ;

     if (StartDoc (hdcPrn, &di) > 0 && StartPage (hdcPrn) > 0 &&
         ExtEscape (hdcPrn, NEXTBAND, 0, NULL, sizeof (rect), (LPSTR) &rect) > 0)
          {
          while (!IsRectEmpty (&rect) && !bUserAbort)
               {
               Rectangle (hdcPrn, rect.left, rect.top, rect.right, 
                                                       rect.bottom) ;
               PageGDICalls (hdcPrn, xPage, yPage) ;

               if (ExtEscape (hdcPrn, NEXTBAND, 0, NULL, sizeof (rect), 
                   (LPSTR) &rect) < 0)
                    {
                    bError = TRUE ;          // If error, set flag and
                    break ;                  //   break out of loop
                    }
               }
          }
     else
          bError = TRUE ;

     if (!bError)
          {
          if (bUserAbort)
               AbortDoc (hdcPrn) ;
          else
               if (EndPage (hdcPrn))
                    EndDoc (hdcPrn) ;
          }

     if (!bUserAbort)
          {
          EnableWindow (hwnd, TRUE) ;     
          DestroyWindow (hDlgPrint) ;
          }

     DeleteDC (hdcPrn) ;

     return bError || bUserAbort ;
     }
