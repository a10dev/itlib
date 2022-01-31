/*---------------------------------------
   PRINT3.C -- Printing with Dialog Box
               (c) Charles Petzold, 1996
  ---------------------------------------*/

#include <windows.h>

HDC  GetPrinterDC (void) ;              // in PRINT.C
void PageGDICalls (HDC, int, int) ;

HINSTANCE hInst ;
char      szAppName[] = "Print3" ;
char      szCaption[] = "Print Program 3 (Dialog Box)" ;

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
     static DOCINFO di     = { sizeof (DOCINFO), "Print3: Printing", NULL } ;
     BOOL           bError = FALSE ;
     HDC            hdcPrn ;
     int            xPage, yPage ;

     if (NULL == (hdcPrn = GetPrinterDC ()))
          return TRUE ;

     xPage = GetDeviceCaps (hdcPrn, HORZRES) ;
     yPage = GetDeviceCaps (hdcPrn, VERTRES) ;

     EnableWindow (hwnd, FALSE) ;

     bUserAbort = FALSE ;
     hDlgPrint = CreateDialog (hInst, "PrintDlgBox", hwnd, PrintDlgProc) ;

     SetAbortProc (hdcPrn, AbortProc) ;

     if (StartDoc (hdcPrn, &di) > 0)
          {
          if (StartPage (hdcPrn) > 0)
               {
               PageGDICalls (hdcPrn, xPage, yPage) ;

               if (EndPage (hdcPrn) > 0)
                    EndDoc (hdcPrn) ;
               else
                    bError = TRUE ;
               }
          }
     else
          bError = TRUE ;

     if (!bUserAbort)
          {
          EnableWindow (hwnd, TRUE) ;
          DestroyWindow (hDlgPrint) ;
          }

     DeleteDC (hdcPrn) ;

     return bError || bUserAbort ;
     }
