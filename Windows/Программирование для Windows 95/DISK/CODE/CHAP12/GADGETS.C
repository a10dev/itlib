/*-----------------------------------------
   GADGETS.C -- Gadgets for a frame window.
                 (c) Paul Yao, 1996
  -----------------------------------------*/
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include "comcthlp.h"
#include "gadgets.h"

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;
LRESULT CALLBACK ClientWndProc (HWND, UINT, WPARAM, LPARAM) ;

char szAppName[]  = "Gadgets" ;
BOOL bIgnoreSize = FALSE ;
HINSTANCE hInst ;
HWND hwndClient = NULL ;
HWND hwndToolBar = NULL ;
HWND hwndStatusBar = NULL ;
HWND hwndNotify = NULL ;

extern DWORD dwToolBarStyles ;
extern BOOL bStrings ;
extern BOOL bLargeIcons ;
extern BOOL bComboBox ;
extern DWORD dwStatusBarStyles ;
extern int cyToolBar ;

//-------------------------------------------------------------------
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                    PSTR lpszCmdLine, int cmdShow)       
     {
     HWND        hwnd ;
     MSG         msg ;
     WNDCLASSEX  wc ;

     hInst = hInstance ;

	 wc.cbSize        = sizeof (wc) ;
     wc.lpszClassName = szAppName ;
     wc.hInstance     = hInstance ;
     wc.lpfnWndProc   = WndProc ;
     wc.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wc.hIcon         = LoadIcon (hInst, MAKEINTRESOURCE (IDI_APP)) ;
     wc.lpszMenuName  = "MAIN" ;
     wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
     wc.style         = 0 ;
     wc.cbClsExtra    = 0 ;
     wc.cbWndExtra    = 0 ;
	 wc.hIconSm       = LoadIcon (hInst, MAKEINTRESOURCE (IDI_APP)) ;

     RegisterClassEx (&wc) ;

     wc.lpszClassName = "ClientWndProc" ;
     wc.hInstance     = hInstance ;
     wc.lpfnWndProc   = ClientWndProc ;
     wc.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wc.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
     wc.lpszMenuName  = NULL ;
     wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1) ;
     wc.style         = 0 ;
     wc.cbClsExtra    = 0 ;
     wc.cbWndExtra    = 0 ;
	 wc.hIconSm       = LoadIcon (NULL, IDI_APPLICATION) ;

     RegisterClassEx (&wc) ;

     hwnd = CreateWindowEx(0L,
                           szAppName, szAppName,
                           WS_OVERLAPPEDWINDOW,  
                           CW_USEDEFAULT, CW_USEDEFAULT,
                           CW_USEDEFAULT, CW_USEDEFAULT,
                           NULL, NULL, hInstance, NULL) ;
     ShowWindow (hwnd, cmdShow) ;
     UpdateWindow (hwnd) ;

     InitCommonControls () ;

     while (GetMessage (&msg, NULL, 0, 0))
          {
          TranslateMessage (&msg) ; 
          DispatchMessage (&msg) ;  
          }
     return msg.wParam ;
     }

//-------------------------------------------------------------------
void MenuCheckMark (HMENU hmenu, int id, BOOL bCheck)
     {
     int iState ;
     iState = (bCheck) ? MF_CHECKED : MF_UNCHECKED ;
     CheckMenuItem (hmenu, id, iState) ;
     }

//-------------------------------------------------------------------
LRESULT CALLBACK
WndProc (HWND hwnd, UINT mMsg, WPARAM wParam, LPARAM lParam) 
     {
     switch (mMsg)
          {
          case WM_CREATE :
               {
               // Create toolbar (source resides in toolbar.c).
               hwndToolBar = InitToolBar (hwnd) ;

               // Create status bar (source resides in statbar.c).
               hwndStatusBar = InitStatusBar (hwnd) ;

               // Create client window (contains notify list).
               hwndClient = CreateWindowEx (WS_EX_CLIENTEDGE,
                                 "ClientWndProc", NULL,
                                 WS_CHILD | WS_VISIBLE, 0, 0, 0, 0,
                                 hwnd, (HMENU) 4, hInst, NULL) ;

               return 0 ;
               }

          case WM_COMMAND :
               {
               // Toolbar button commands.
               if (LOWORD (wParam) < 300)
                    {
                    char ach[80] ;
                    wsprintf (ach, "Got Command (%d)", wParam) ;
                    MessageBox (hwnd, ach, szAppName, MB_OK) ;
                    break ;
                    }

               // Menu item commands
               switch (LOWORD (wParam))
                    {
                    // Toolbar settings
                    case IDM_STRINGS :
                    case IDM_LARGEICONS :
                    case IDM_SMALLICONS :
                    case IDM_NODIVIDER :
                    case IDM_WRAPABLE :
                    case IDM_TOP :
                    case IDM_BOTTOM :
                    case IDM_NOMOVEY :
                    case IDM_NOPARENTALIGN :
                    case IDM_NORESIZE :
                    case IDM_ADJUSTABLE :
                    case IDM_ALTDRAG :
                    case IDM_TOOLTIPS :
                    case IDM_COMBOBOX :
                         DestroyWindow (hwndToolBar) ;
                         hwndToolBar = RebuildToolBar (hwnd, 
                                                       LOWORD (wParam)) ;
                         break ;

                    // Toolbar messages
                    case IDM_TB_CHECK :
                    case IDM_TB_ENABLE :
                    case IDM_TB_HIDE :
                    case IDM_TB_INDETERMINATE :
                    case IDM_TB_PRESS :
                    case IDM_TB_BUTTONCOUNT :
                    case IDM_TB_GETROWS :
                    case IDM_TB_CUSTOMIZE :
                         ToolBarMessage (hwndToolBar, LOWORD (wParam)) ;
                         break ;

                    // Status bar settings
                    case IDM_STAT_SIZEGRIP :
                    case IDM_STAT_TOP :
                    case IDM_STAT_BOTTOM :
                    case IDM_STAT_NOMOVEY :
                    case IDM_STAT_NOPARENTALIGN :
                    case IDM_STAT_NORESIZE :
                         DestroyWindow (hwndStatusBar) ;
                         hwndStatusBar = RebuildStatusBar (hwnd,
                                                           LOWORD (wParam)) ;
                         break ;
                      
                    // Status bar messages
                    case IDM_ST_GETBORDERS :
                    case IDM_ST_GETPARTS :
                    case IDM_ST_SETTEXT :
                    case IDM_ST_SIMPLE :
                         StatusBarMessage (hwndStatusBar, LOWORD (wParam)) ;
                         break ;

                    // Toggle display of toolbar
                    case IDM_VIEW_TOOLBAR :
                         {
                         RECT r ;

                         if (hwndToolBar && IsWindowVisible (hwndToolBar))
                              {
                              ShowWindow (hwndToolBar, SW_HIDE) ;
                              }
                         else
                              {
                              ShowWindow (hwndToolBar, SW_SHOW) ;
                              }

                         // Resize other windows.
                         GetClientRect (hwnd, &r) ;
                         PostMessage (hwnd, WM_SIZE, 0, 
                                      MAKELPARAM (r.right, r.bottom)) ;

                         break;
                         }

                    // Toggle display of status bar
                    case IDM_VIEW_STATUS :
                         {
                         RECT r;

                         if (hwndStatusBar && IsWindowVisible (hwndStatusBar))
                              {
                              ShowWindow (hwndStatusBar, SW_HIDE) ;
                              }
                         else
                              {
                              ShowWindow (hwndStatusBar, SW_SHOW) ;
                              }

                         // Resize other windows.
                         GetClientRect (hwnd, &r) ;
                         PostMessage (hwnd, WM_SIZE, 0, 
                                      MAKELPARAM (r.right, r.bottom)) ;

                         break;
                         }

                    // Toggle display of notifications window.
                    case IDM_VIEW_NOTIFICATIONS :
                         hwndNotify = ViewNotificationsToggle (hwndClient) ;
                         break ;

                    // Toggle ignore WM_SIZE to show auto-size/auto-move
                    case IDM_IGNORESIZE :
                    case IDM_STAT_IGNORESIZE :
                         {
                         RECT r ;

                         bIgnoreSize = !bIgnoreSize ;
                         if (bIgnoreSize)
                              {
                              ShowWindow (hwndClient, SW_HIDE) ;
                              }
                         else
                              {
                              ShowWindow (hwndClient, SW_SHOW) ;
                              GetClientRect (hwnd, &r) ;
                              PostMessage (hwnd, WM_SIZE, 0, 
                                           MAKELPARAM (r.right, r.bottom)) ;
                              }
                         break ;
                         }

                    // Clear contents of notification window
                    case IDM_NOTIFICATIONS_CLEAR :
                         ClearNotificationList () ;
                         break;
                    }

               return 0;
               }

          case WM_INITMENU :
               {
               BOOL bCheck ;
               HMENU hmenu = (HMENU) wParam ;

               MenuCheckMark (hmenu, IDM_IGNORESIZE, bIgnoreSize) ;
               MenuCheckMark (hmenu, IDM_STAT_IGNORESIZE, bIgnoreSize) ;

               // Toolbar menu items.
               MenuCheckMark (hmenu, IDM_STRINGS, bStrings) ;
               MenuCheckMark (hmenu, IDM_LARGEICONS, bLargeIcons) ;
               MenuCheckMark (hmenu, IDM_SMALLICONS, !bLargeIcons) ;
               MenuCheckMark (hmenu, IDM_COMBOBOX, bComboBox) ;

               bCheck = (dwToolBarStyles & CCS_NODIVIDER) ;
               MenuCheckMark (hmenu, IDM_NODIVIDER, bCheck) ;

               bCheck = (dwToolBarStyles & TBSTYLE_WRAPABLE) ;
               MenuCheckMark(hmenu, IDM_WRAPABLE, bCheck) ;

               bCheck = ((dwToolBarStyles & 3) == CCS_TOP) ;
               MenuCheckMark(hmenu, IDM_TOP, bCheck) ;

               bCheck = ((dwToolBarStyles & 3) == CCS_BOTTOM) ;
               MenuCheckMark (hmenu, IDM_BOTTOM, bCheck) ;

               bCheck = ((dwToolBarStyles & 3) == CCS_NOMOVEY) ;
               MenuCheckMark (hmenu, IDM_NOMOVEY, bCheck) ;

               bCheck = (dwToolBarStyles & CCS_NOPARENTALIGN) ;
               MenuCheckMark (hmenu, IDM_NOPARENTALIGN, bCheck) ;

               bCheck = (dwToolBarStyles & CCS_NORESIZE) ;
               MenuCheckMark (hmenu, IDM_NORESIZE, bCheck) ;

               bCheck = (dwToolBarStyles & CCS_ADJUSTABLE) ;
               MenuCheckMark (hmenu, IDM_ADJUSTABLE, bCheck) ;

               bCheck = (dwToolBarStyles & TBSTYLE_ALTDRAG) ;
               MenuCheckMark (hmenu, IDM_ALTDRAG, bCheck) ;

               bCheck = (dwToolBarStyles & TBSTYLE_TOOLTIPS) ;
               MenuCheckMark (hmenu, IDM_TOOLTIPS, bCheck) ;

               // Status bar menu items.
               bCheck = (dwStatusBarStyles & SBARS_SIZEGRIP) ;
               MenuCheckMark (hmenu, IDM_STAT_SIZEGRIP, bCheck) ;

               bCheck = ((dwStatusBarStyles & 3) == CCS_TOP) ;
               MenuCheckMark (hmenu, IDM_STAT_TOP, bCheck) ;

               bCheck = ((dwStatusBarStyles & 3) == CCS_BOTTOM) ;
               MenuCheckMark (hmenu, IDM_STAT_BOTTOM, bCheck) ;

               bCheck = ((dwStatusBarStyles & 3) == CCS_NOMOVEY) ;
               MenuCheckMark (hmenu, IDM_STAT_NOMOVEY, bCheck) ;

               bCheck = (dwStatusBarStyles & CCS_NOPARENTALIGN) ;
               MenuCheckMark (hmenu, IDM_STAT_NOPARENTALIGN, bCheck) ;

               bCheck = (dwStatusBarStyles & CCS_NORESIZE) ;
               MenuCheckMark (hmenu, IDM_STAT_NORESIZE, bCheck) ;

               // View menu items.
               bCheck = IsWindowVisible (hwndToolBar) ;
               MenuCheckMark (hmenu, IDM_VIEW_TOOLBAR, bCheck) ;
            
               bCheck = IsWindowVisible (hwndStatusBar) ;
               MenuCheckMark (hmenu, IDM_VIEW_STATUS, bCheck) ;

               bCheck = (hwndNotify != NULL) ;
               MenuCheckMark (hmenu, IDM_VIEW_NOTIFICATIONS, bCheck) ;
               return 0 ;
               }

          case WM_MENUSELECT :
               return Statusbar_MenuSelect (hwnd, wParam, lParam) ;

          case WM_DESTROY :
               PostQuitMessage (0) ;
               return 0 ;

          case WM_NOTIFY :
               {
               LPNMHDR pnmh = (LPNMHDR) lParam ;
               int idCtrl = (int) wParam ;

               // Display notification details in notify window
               DisplayNotificationDetails (wParam, lParam) ;

               // Toolbar notifications
               if ((pnmh->code >= TBN_LAST) &&
                   (pnmh->code <= TBN_FIRST))
                    {
                    return ToolBarNotify (hwnd, wParam, lParam) ;
                    }
            
               // Fetch tooltip text
               if (pnmh->code == TTN_NEEDTEXT)
                    {
                    LPTOOLTIPTEXT lpttt = (LPTOOLTIPTEXT) lParam ;
                    CopyToolTipText (lpttt) ;
                    }

               return 0 ;
               }

          case WM_SIZE :
               {
               int cx = LOWORD (lParam) ;
               int cy = HIWORD (lParam) ;
               int cyStatus ;
               int cyTB ;
               int x, y ;
               DWORD dwStyle ;
               RECT rWindow ;

               // Ignore size message to allow auto-move and auto-size
               // features to be more clearly seen.
               if (bIgnoreSize)
                    return 0 ;

               // Adjust toolbar size.
               if (IsWindowVisible (hwndToolBar))
                    {
                    dwStyle = GetWindowLong (hwndToolBar, GWL_STYLE) ;
                    if (dwStyle & CCS_NORESIZE)
                         {
                         MoveWindow (hwndToolBar, 
                                     0, 0, cx, cyToolBar, FALSE) ;
                         }
                    else
                         {
                         ToolBar_AutoSize (hwndToolBar) ;
                         }
                    InvalidateRect (hwndToolBar, NULL, TRUE) ;
                    GetWindowRect (hwndToolBar, &rWindow) ;
                    ScreenToClient (hwnd, (LPPOINT) &rWindow.left) ;
                    ScreenToClient (hwnd, (LPPOINT) &rWindow.right) ;
                    cyTB = rWindow.bottom - rWindow.top ;
                    } 
               else
                    {
                    cyTB = 0 ;
                    }

               // Adjust status bar size.
               if (IsWindowVisible (hwndStatusBar))
                    {
                    GetWindowRect (hwndStatusBar, &rWindow) ;
                    cyStatus = rWindow.bottom - rWindow.top ;
                    MoveWindow (hwndStatusBar, 0, cy - cyStatus, 
                                              cx, cyStatus, TRUE) ;
                    }
               else
                    {
                    cyStatus = 0 ;
                    }

               // Adjust client window size.
               x = 0 ;
               y = cyTB ;
               cy = cy - (cyStatus + cyTB) ;
               MoveWindow (hwndClient, x, y, cx, cy, TRUE) ;
               return 0;
               }

          default:
               return (DefWindowProc (hwnd, mMsg, wParam, lParam)) ;
          }
     }

//-------------------------------------------------------------------
LRESULT CALLBACK 
ClientWndProc (HWND hwnd, UINT mMsg, WPARAM wParam, LPARAM lParam)
     {
     static COLORREF crBack ;
     static HBRUSH hbr ;

     switch (mMsg)
          {
          case WM_CREATE :
               hwndNotify = ViewNotificationsToggle (hwnd) ;
               crBack = GetSysColor (COLOR_APPWORKSPACE) ;
               hbr = CreateSolidBrush (crBack) ;
               return 0 ;

          case WM_DESTROY :
               DeleteObject (hbr) ;
               return 0 ;

          case WM_CTLCOLORLISTBOX :
               {
               DefWindowProc (hwnd, mMsg, wParam, lParam) ;
               SetBkColor ((HDC) wParam, crBack) ;
               SetBkMode ((HDC) wParam, TRANSPARENT) ;
               return (LRESULT) (HBRUSH) hbr ;
               }

          case WM_SIZE :
               {
               HWND hwndNotify = GetWindow (hwnd, GW_CHILD) ;
               int cx = LOWORD (lParam) ;
               int cy = HIWORD (lParam) ;

               // Ignore if notification window is absent.
               if (hwndNotify != NULL)
                    {
                    MoveWindow (hwndNotify, 0, 0, cx, cy, TRUE) ;
                    }

               return 0 ;
               }

          default :
               return (DefWindowProc (hwnd, mMsg, wParam, lParam)) ;
          }
     }
