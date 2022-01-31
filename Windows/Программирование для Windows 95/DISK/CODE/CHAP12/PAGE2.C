/*----------------------------------
   PAGE2.C -- Property sheet page 2
              (c) Paul Yao, 1996
  ----------------------------------*/
#include <windows.h>
#include <commctrl.h>
#include "property.h"
#include "notify.h"
#include "helper.h"

static LPDWORD pTheExStyles ;
extern BOOL bWizard ;
extern HWND hwndMain ;
extern HWND hwndModeless ;

DWORD FetchExStyles (HWND hwndDlg) ;

//-------------------------------------------------------------------
UINT CALLBACK 
ExStylePageProc (HWND  hwnd, UINT uMsg, LPPROPSHEETPAGE ppsp)
     {
     switch (uMsg)
          {
          case PSPCB_CREATE :
               // Store pointer to extended style data
               pTheExStyles = (LPDWORD) ppsp->lParam ;
               return TRUE ;

          case PSPCB_RELEASE :
               break ;
          }

     return 0 ;
     }

//-------------------------------------------------------------------
BOOL CALLBACK 
ExStyleDlgProc (HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam)
     {
     switch (msg)
          {
          case WM_INITDIALOG :
               {
               BOOL bCheck ;
               DWORD dwOrigStyle = *pTheExStyles ;

               bCheck = (dwOrigStyle & WS_EX_CLIENTEDGE) ;
               SetButtonCheck (hwndDlg, IDC_CLIENTEDGE, bCheck) ;

               bCheck = (dwOrigStyle & WS_EX_CONTEXTHELP) ;
               SetButtonCheck (hwndDlg, IDC_CONTEXTHELP, bCheck) ;

               bCheck = (dwOrigStyle & WS_EX_DLGMODALFRAME) ;
               SetButtonCheck (hwndDlg, IDC_DLGMODALFRAME, bCheck) ;

               bCheck = (dwOrigStyle & WS_EX_OVERLAPPEDWINDOW) ;
               SetButtonCheck (hwndDlg, IDC_EXOVERLAPPED, bCheck) ;

               bCheck = (dwOrigStyle & WS_EX_PALETTEWINDOW) ;
               SetButtonCheck (hwndDlg, IDC_PALETTE, bCheck) ;

               bCheck = (dwOrigStyle & WS_EX_STATICEDGE) ;
               SetButtonCheck (hwndDlg, IDC_STATICEDGE, bCheck) ;

               bCheck = (dwOrigStyle & WS_EX_TOOLWINDOW) ;
               SetButtonCheck (hwndDlg, IDC_TOOLWINDOW, bCheck) ;

               bCheck = (dwOrigStyle & WS_EX_WINDOWEDGE) ;
               SetButtonCheck (hwndDlg, IDC_WINDOWEDGE, bCheck) ;

               bCheck = (dwOrigStyle & WS_EX_ACCEPTFILES) ;
               SetButtonCheck (hwndDlg, IDC_ACCEPTFILES, bCheck) ;

               bCheck = (dwOrigStyle & WS_EX_APPWINDOW) ;
               SetButtonCheck (hwndDlg, IDC_APPWINDOW, bCheck) ;

               bCheck = (dwOrigStyle & WS_EX_TOPMOST) ;
               SetButtonCheck (hwndDlg, IDC_TOPMOST, bCheck) ;

               bCheck = (dwOrigStyle & WS_EX_TRANSPARENT) ;
               SetButtonCheck (hwndDlg, IDC_TRANSPARENT, bCheck) ;

               bCheck = (dwOrigStyle & WS_EX_CONTROLPARENT) ;
               SetButtonCheck (hwndDlg, IDC_CONTROLPARENT, bCheck) ;

               bCheck = (dwOrigStyle & WS_EX_MDICHILD) ;
               SetButtonCheck (hwndDlg, IDC_MDICHILD, bCheck) ;

               bCheck = (dwOrigStyle & WS_EX_NOPARENTNOTIFY) ;
               SetButtonCheck (hwndDlg, IDC_NOPARENTNOTIFY, bCheck) ;

               return TRUE ;
               }

          case WM_COMMAND :
               {
               WORD wNotifyCode = HIWORD (wParam) ;
               WORD wID = LOWORD (wParam) ;
               HWND hwndSheet ;

               switch (wID)
                    {
                    case IDC_CLIENTEDGE :
                    case IDC_CONTEXTHELP :
                    case IDC_DLGMODALFRAME :
                    case IDC_EXOVERLAPPED :
                    case IDC_PALETTE :
                    case IDC_STATICEDGE :
                    case IDC_TOOLWINDOW :
                    case IDC_WINDOWEDGE :
                    case IDC_ACCEPTFILES :
                    case IDC_APPWINDOW :
                    case IDC_TOPMOST :
                    case IDC_TRANSPARENT :
                    case IDC_CONTROLPARENT :
                    case IDC_MDICHILD :
                    case IDC_NOPARENTNOTIFY :
                         hwndSheet = GetParent (hwndDlg) ;
                         PropSheet_Changed (hwndSheet, hwndDlg) ;
                         break ;
                    }
               return TRUE ;
               }

          case WM_HELP :
               // Catch F1 key strike
               MessageBox (hwndDlg, "WM_HELP Message Received",
                           "ExStyleDlgProc", MB_OK) ;
               return TRUE ;

          case WM_NOTIFY :
               {
               LPNMHDR pnmh = (LPNMHDR) lParam ;

			   // Handle Finish button on wizard page
			   if (pnmh->code == PSN_WIZFINISH)
			        {
				    HWND hwndPS ;
					
					hwndPS = GetParent (hwndDlg) ;
					PropSheet_Apply (hwndPS) ;
					return TRUE ;
			        }

               // Handle OK and Apply buttons
               if (pnmh->code == PSN_APPLY || pnmh->code == PSN_RESET)
                    {
                    HWND hwndPS ;
                    HWND hwndActive ;

                    // Overwrite current style value
                    *pTheExStyles = FetchExStyles (hwndDlg) ;

                    // Tell main window to re-create child window
                    hwndPS = GetParent (hwndDlg) ;
                    hwndActive = PropSheet_GetCurrentPageHwnd (hwndPS) ;

                    // Only re-create if we're the active page
                    if (hwndDlg == hwndActive)
                         PostMessage (hwndMain, PM_CREATEWINDOW, 0, 0L) ;
                    }

               // Destroy modeless dialog on OK or Cancel
               if ((IsWindowEnabled (hwndMain)) &&
                   (pnmh->code == PSN_APPLY || pnmh->code == PSN_RESET))
                    {
                    LPPSHNOTIFY psh = (LPPSHNOTIFY) lParam ;
                    HWND hwndPropSheet ;

                    // Ignore Apply button
                    if (pnmh->code == PSN_APPLY && psh->lParam == 0)
                         return TRUE ;

                    // Clicking OK or Cancel, destroy property sheet
                    hwndPropSheet = GetParent (hwndDlg) ;
                    DestroyWindow (hwndPropSheet) ;
                    hwndModeless = NULL ;
               }

               // Enable Back and Finish buttons on wizard page
               if (bWizard && pnmh->code == PSN_SETACTIVE)
                    {
                    HWND hwndSheet = GetParent (hwndDlg) ;
                    PropSheet_SetWizButtons (hwndSheet, PSWIZB_BACK |
                                                        PSWIZB_FINISH) ;
                    }

               // Support for Help button
               if (pnmh->code == PSN_HELP)
                    {
                    MessageBox (hwndDlg, "PSN_HELP Notification Received",
                                "ExStyleDlgProc", MB_OK) ;
                    }

               return TRUE ;
               }

          default :
               return FALSE ;
          }
     }

//-------------------------------------------------------------------
DWORD FetchExStyles (HWND hwndDlg)
     {
     DWORD dwStyle = 0L ;

     if (QueryButtonCheck (hwndDlg, IDC_CLIENTEDGE))
          {
          dwStyle |= WS_EX_CLIENTEDGE ;
          }

     if (QueryButtonCheck (hwndDlg, IDC_CONTEXTHELP))
          {
          dwStyle |= WS_EX_CONTEXTHELP ;
          }

     if (QueryButtonCheck (hwndDlg, IDC_DLGMODALFRAME))
          {
          dwStyle |= WS_EX_DLGMODALFRAME ;
          }

     if (QueryButtonCheck (hwndDlg, IDC_EXOVERLAPPED))
          {
          dwStyle |= WS_EX_OVERLAPPEDWINDOW ;
          }

     if (QueryButtonCheck (hwndDlg, IDC_PALETTE))
          {
          dwStyle |= WS_EX_PALETTEWINDOW ;
          }

     if (QueryButtonCheck (hwndDlg, IDC_STATICEDGE))
          {
          dwStyle |= WS_EX_STATICEDGE ;
          }

     if (QueryButtonCheck (hwndDlg, IDC_TOOLWINDOW))
          {
          dwStyle |= WS_EX_TOOLWINDOW ;
          }

     if (QueryButtonCheck (hwndDlg, IDC_WINDOWEDGE))
          {
          dwStyle |= WS_EX_WINDOWEDGE ;
          }

     if (QueryButtonCheck (hwndDlg, IDC_ACCEPTFILES))
          {
          dwStyle |= WS_EX_ACCEPTFILES ;
          }

     if (QueryButtonCheck (hwndDlg, IDC_APPWINDOW))
          {
          dwStyle |= WS_EX_APPWINDOW ;
          }

     if (QueryButtonCheck (hwndDlg, IDC_TOPMOST))
          {
          dwStyle |= WS_EX_TOPMOST ;
          }

     if (QueryButtonCheck (hwndDlg, IDC_TRANSPARENT))
          {
          dwStyle |= WS_EX_TRANSPARENT ;
          }

     if (QueryButtonCheck (hwndDlg, IDC_CONTROLPARENT))
          {
          dwStyle |= WS_EX_CONTROLPARENT ;
          }

     if (QueryButtonCheck (hwndDlg, IDC_MDICHILD))
          {
          dwStyle |= WS_EX_MDICHILD ;
          }

     if (QueryButtonCheck (hwndDlg, IDC_NOPARENTNOTIFY))
          {
          dwStyle |= WS_EX_NOPARENTNOTIFY ;
          }


     return dwStyle ;
     }
