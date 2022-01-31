/*----------------------------------
   PAGE1.C -- Property sheet page 1
              (c) Paul Yao, 1996
  ----------------------------------*/
#include <windows.h>
#include <prsht.h>
#include "property.h"
#include "notify.h"
#include "helper.h"

static LPDWORD pTheStyles ;
extern BOOL bWizard ;
extern HWND hwndMain ;
extern HWND hwndModeless ;

DWORD FetchStyles (HWND hwndDlg) ;

//-------------------------------------------------------------------
UINT CALLBACK 
StylePageProc (HWND  hwnd, UINT uMsg, LPPROPSHEETPAGE ppsp)
     {
     switch (uMsg)
          {
          case PSPCB_CREATE :
               // Store pointer to style data
               pTheStyles = (LPDWORD) ppsp->lParam ;
               return TRUE ;

          case PSPCB_RELEASE :
               return 0;
          }

     return 0 ;
     }

//-------------------------------------------------------------------
BOOL CALLBACK 
StyleDlgProc (HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam)
     {
     switch (msg)
          {
          case WM_INITDIALOG :
               {
               BOOL bCheck ;
               DWORD dwOrigStyle = *pTheStyles ;

               bCheck = (dwOrigStyle & WS_VISIBLE) ;
               SetButtonCheck (hwndDlg, IDC_VISIBLE, bCheck) ;

               bCheck = (dwOrigStyle & WS_DISABLED) ;
               SetButtonCheck (hwndDlg, IDC_DISABLED, bCheck) ;

               bCheck = (dwOrigStyle & WS_MINIMIZE) ;
               SetButtonCheck (hwndDlg, IDC_MINIMIZE, bCheck) ;

               bCheck = (dwOrigStyle & WS_MAXIMIZE) ;
               SetButtonCheck (hwndDlg, IDC_MAXIMIZE, bCheck) ;

               bCheck = (dwOrigStyle & WS_CLIPCHILDREN) ;
               SetButtonCheck (hwndDlg, IDC_CLIPCHILDREN, bCheck) ;

               bCheck = (dwOrigStyle & WS_CLIPSIBLINGS) ;
               SetButtonCheck (hwndDlg, IDC_CLIPSIBLINGS, bCheck) ;

               bCheck = (dwOrigStyle & WS_BORDER) ;
               SetButtonCheck (hwndDlg, IDC_BORDER, bCheck) ;

               bCheck = (dwOrigStyle & WS_CAPTION) ;
               SetButtonCheck (hwndDlg, IDC_CAPTION, bCheck) ;

               bCheck = (dwOrigStyle & WS_DLGFRAME) ;
               SetButtonCheck (hwndDlg, IDC_DLGFRAME, bCheck) ;

               bCheck = (dwOrigStyle & WS_HSCROLL) ;
               SetButtonCheck (hwndDlg, IDC_HSCROLL, bCheck) ;

               bCheck = (dwOrigStyle & WS_MAXIMIZEBOX) ;
               SetButtonCheck (hwndDlg, IDC_MAXIMIZEBOX, bCheck) ;

               bCheck = (dwOrigStyle & WS_MINIMIZEBOX) ;
               SetButtonCheck (hwndDlg, IDC_MINIMIZEBOX, bCheck) ;

               bCheck = (dwOrigStyle & WS_SYSMENU) ;
               SetButtonCheck (hwndDlg, IDC_SYSMENU, bCheck) ;

               bCheck = (dwOrigStyle & WS_THICKFRAME) ;
               SetButtonCheck (hwndDlg, IDC_THICKFRAME, bCheck) ;

               bCheck = (dwOrigStyle & WS_VSCROLL) ;
               SetButtonCheck (hwndDlg, IDC_VSCROLL, bCheck) ;

               return TRUE ;
          }

          case WM_COMMAND :
               {
               WORD wNotifyCode = HIWORD (wParam) ;
               WORD wID = LOWORD (wParam) ;
               HWND hwndSheet ;

               switch (wID)
                    {
                    case IDC_VISIBLE :
                    case IDC_DISABLED :
                    case IDC_MINIMIZE :
                    case IDC_MAXIMIZE :
                    case IDC_CLIPCHILDREN :
                    case IDC_CLIPSIBLINGS :
                    case IDC_BORDER :
                    case IDC_CAPTION :
                    case IDC_DLGFRAME :
                    case IDC_HSCROLL :
                    case IDC_MAXIMIZEBOX :
                    case IDC_MINIMIZEBOX :
                    case IDC_SYSMENU :
                    case IDC_THICKFRAME :
                    case IDC_VSCROLL :
                         hwndSheet = GetParent (hwndDlg) ;
                         PropSheet_Changed (hwndSheet, hwndDlg) ;
                         break ;
                    }
               return TRUE ;
               }

          case WM_HELP :
               // Catch F1 key strike
               MessageBox (hwndDlg, "WM_HELP Message Received",
                           "StyleDlgProc", MB_OK) ;
               return TRUE ;

          case WM_NOTIFY :
               {
               LPNMHDR pnmh = (LPNMHDR) lParam ;

               // Handle OK and Apply buttons
               if (pnmh->code == PSN_APPLY)
                    {
                    HWND hwndPS ;
                    HWND hwndActive ;

                    // Overwrite current style value
                    *pTheStyles = FetchStyles (hwndDlg) ;

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

               // Enable Next button on wizard page
               if (bWizard && pnmh->code == PSN_SETACTIVE)
                    {
                    HWND hwndSheet = GetParent (hwndDlg) ;
                    PropSheet_SetWizButtons (hwndSheet, PSWIZB_NEXT) ;
                    }
               return TRUE ;
               }

          default :
               return FALSE ;
          }
     }

//-------------------------------------------------------------------
DWORD FetchStyles (HWND hwndDlg)
     {
     DWORD dwStyle = WS_CHILD ;

     if (QueryButtonCheck (hwndDlg, IDC_VISIBLE))
          {
          dwStyle |= WS_VISIBLE ;
          }

     if (QueryButtonCheck (hwndDlg, IDC_DISABLED))
          {
          dwStyle |= WS_DISABLED ;
          }

     if (QueryButtonCheck (hwndDlg, IDC_MINIMIZE))
          {
          dwStyle |= WS_MINIMIZE ;
          }

     if (QueryButtonCheck (hwndDlg, IDC_MAXIMIZE))
          {
          dwStyle |= WS_MAXIMIZE ;
          }

     if (QueryButtonCheck (hwndDlg, IDC_CLIPCHILDREN))
          {
          dwStyle |= WS_CLIPCHILDREN ;
          }

     if (QueryButtonCheck (hwndDlg, IDC_CLIPSIBLINGS))
          {
          dwStyle |= WS_CLIPSIBLINGS ;
          }

     if (QueryButtonCheck (hwndDlg, IDC_BORDER))
          {
          dwStyle |= WS_BORDER ;
          }

     if (QueryButtonCheck (hwndDlg, IDC_CAPTION))
          {
          dwStyle |= WS_CAPTION ;
          }

     if (QueryButtonCheck (hwndDlg, IDC_DLGFRAME))
          {
          dwStyle |= WS_DLGFRAME ;
          }

     if (QueryButtonCheck (hwndDlg, IDC_HSCROLL))
          {
          dwStyle |= WS_HSCROLL ;
          }

     if (QueryButtonCheck (hwndDlg, IDC_MAXIMIZEBOX))
          {
          dwStyle |= WS_MAXIMIZEBOX ;
          }

     if (QueryButtonCheck (hwndDlg, IDC_MINIMIZEBOX))
          {
          dwStyle |= WS_MINIMIZEBOX ;
          }

     if (QueryButtonCheck (hwndDlg, IDC_SYSMENU))
          {
          dwStyle |= WS_SYSMENU ;
          }

     if (QueryButtonCheck (hwndDlg, IDC_THICKFRAME))
          {
          dwStyle |= WS_THICKFRAME ;
          }

     if (QueryButtonCheck (hwndDlg, IDC_VSCROLL))
          {
          dwStyle |= WS_VSCROLL ;
          }

     return dwStyle ;
     }
