/*--------------------------------------
   PROPERTY.C -- Property sheet example
                 (c) Paul Yao, 1996
  --------------------------------------*/
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include "comcthlp.h"
#include "property.h"

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

char szAppName[]  = "Property Sheet" ;
HINSTANCE hInst ;
HWND hwndMain = NULL ;
HWND hwndChild = NULL ;
HWND hwndModeless = NULL ;

HICON hiconApp ;  // Application icon
BOOL bWizard ;    // Flag whether PSH_WIZARD is set

// Values modified by property sheet
DWORD dwChildStyle = WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CAPTION ;
DWORD dwChildExStyle = 0L ;

// Value modified by menu item selection
DWORD dwSheetStyles = PSH_PROPTITLE ;

//-------------------------------------------------------------------
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                    PSTR lpszCmdLine, int cmdShow)       
     {
     HWND       hwnd ;
     MSG        msg ;
     WNDCLASSEX wc ;

     hInst = hInstance ;
     hiconApp = LoadIcon (hInst, MAKEINTRESOURCE (IDI_APP)) ;

     ZeroMemory (&wc, sizeof (WNDCLASSEX)) ;
	 wc.cbSize        = sizeof (wc) ;
     wc.lpszClassName = "MAIN" ;
     wc.hInstance     = hInstance ;
     wc.lpfnWndProc   = WndProc ;
     wc.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wc.hIcon         = hiconApp ;
     wc.lpszMenuName  = "MAIN" ;
     wc.hbrBackground = (HBRUSH) (COLOR_APPWORKSPACE + 1) ;
	 wc.hIconSm       = hiconApp ;

     RegisterClassEx (&wc) ;

     wc.lpszClassName = "CHILD" ;
     wc.lpfnWndProc   = DefWindowProc ;
     wc.hCursor       = LoadCursor (NULL, IDC_IBEAM) ; 
     wc.hIcon         = NULL ;
     wc.lpszMenuName  = NULL ;
     wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1) ;
	 wc.hIconSm       = NULL ;

     RegisterClassEx (&wc) ;

     hwndMain = 
     hwnd = CreateWindowEx (WS_EX_OVERLAPPEDWINDOW, "MAIN", 
                            szAppName, WS_OVERLAPPEDWINDOW, 
                            CW_USEDEFAULT, CW_USEDEFAULT, 
                            CW_USEDEFAULT, CW_USEDEFAULT, 
                            NULL, NULL, hInstance, NULL) ;
                          
     ShowWindow (hwnd, cmdShow) ;
     UpdateWindow (hwnd) ;

     InitCommonControls () ;

     while (GetMessage (&msg, NULL, 0, 0))
          {
          if ((hwndModeless) &&
              (!(PropSheet_IsDialogMessage (hwndModeless, &msg))))
              continue ;

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
void static FlipFlag (LPDWORD dwStyle, DWORD flag)
     {
     if (*dwStyle & flag)  // Flag on -- turn off
          {
          *dwStyle &= (~flag) ;
          }
     else                  // Flag off -- turn on
          {
          *dwStyle |= flag ;
          }
     }

//-------------------------------------------------------------------
LRESULT CALLBACK
WndProc (HWND hwnd, UINT mMsg, WPARAM wParam, LPARAM lParam) 
     {
     switch (mMsg)
          {
          case WM_CREATE :
               hwndChild = CreateWindowEx (dwChildExStyle, "CHILD", 
                                           "First Window", dwChildStyle, 
                                           0, 0, 0, 0, hwnd, (HMENU) 1, 
										   hInst, NULL) ;
                            

               CreateWindowEx (dwChildExStyle, "CHILD", "Second Window",
                               WS_CLIPSIBLINGS | dwChildStyle, 
                               10, 10, 200, 50, hwnd,
                               (HMENU) 2, hInst, NULL) ;
               return 0 ;

          case WM_COMMAND :
               {
               switch (LOWORD (wParam))
                    {
                    case IDM_WINDOW_PROPERTIES :
                         {
                         BOOL bRet ;

                         // If modeless, active existing property sheet
                         if (hwndModeless)
                              {
                              SetActiveWindow (hwndModeless) ;
                              break ;
                              }

                         // Are we creating a wizard?
                         bWizard = (dwSheetStyles & PSH_WIZARD) ;

                         // Create actual property sheet
                         bRet = CreatePropertySheet (hwnd) ;
                    
                         // Store handle if modeless
                         if (dwSheetStyles & PSH_MODELESS)
                              {
                              hwndModeless = (HWND) bRet ;
                              break ;
                              }
                         break ;
                         }

                    case IDM_WIZARD :
                         FlipFlag (&dwSheetStyles, PSH_WIZARD) ;
                         break ;

                    case IDM_HASHELP :
                         FlipFlag (&dwSheetStyles, PSH_HASHELP) ;
                         break ;

                    case IDM_MODELESS :
                         FlipFlag (&dwSheetStyles, PSH_MODELESS) ;
                         break ;

                    case IDM_NOAPPLYNOW :
                         FlipFlag (&dwSheetStyles, PSH_NOAPPLYNOW) ;
                         break ;

                    case IDM_PROPTITLE :
                         FlipFlag (&dwSheetStyles, PSH_PROPTITLE) ;
                         break ;

                    case IDM_RTLREADING :
                         FlipFlag (&dwSheetStyles, PSH_RTLREADING) ;
                         break ;
                    }
                    return 0 ;
          }

          case WM_DESTROY :
               PostQuitMessage (0) ;
               return 0 ;

          case WM_INITMENU :
               {
               BOOL bCheck ;
               HMENU hmenu = (HMENU) wParam ;

               bCheck = (dwSheetStyles & PSH_WIZARD) ;
               MenuCheckMark (hmenu, IDM_WIZARD, bCheck) ;

               bCheck = (dwSheetStyles & PSH_HASHELP) ;
               MenuCheckMark (hmenu, IDM_HASHELP, bCheck) ;

               bCheck = (dwSheetStyles & PSH_MODELESS) ;
               MenuCheckMark (hmenu, IDM_MODELESS, bCheck) ;

               bCheck = (dwSheetStyles & PSH_NOAPPLYNOW) ;
               MenuCheckMark (hmenu, IDM_NOAPPLYNOW, bCheck) ;

               bCheck = (dwSheetStyles & PSH_PROPTITLE) ;
               MenuCheckMark (hmenu, IDM_PROPTITLE, bCheck) ;

               bCheck = (dwSheetStyles & PSH_RTLREADING) ;
               MenuCheckMark (hmenu, IDM_RTLREADING, bCheck) ;

               return 0 ;
               }

          case WM_SETFOCUS :
               SetFocus (hwndChild) ;
               return 0 ;

          case WM_SIZE :
               {
               int cxWidth  = LOWORD (lParam) ;
               int cyHeight = HIWORD (lParam) ;
               int x, y, cx, cy ;

               x  = cxWidth  / 4 ;
               cx = cxWidth  / 2 ;
               y  = cyHeight / 4 ;
               cy = cyHeight / 2 ;

               MoveWindow (hwndChild, x, y, cx, cy, TRUE) ;
               return 0 ;
               }

          case PM_CREATEWINDOW :
               {
               RECT rClient ;
               LPARAM l ;

               DestroyWindow (hwndChild) ;
               hwndChild = CreateWindowEx (dwChildExStyle, "CHILD", 
                                           "First Window", dwChildStyle, 
                                           0, 0, 0, 0, hwnd, (HMENU) 1, 
										   hInst, NULL) ;

               // Send ourselves a WM_SIZE to resize child window
               GetClientRect (hwnd, &rClient) ;
               l = MAKELPARAM (rClient.right, rClient.bottom) ;
               SendMessage (hwnd, WM_SIZE, 0, l) ;
               return 0 ;
               }

          default :
               return (DefWindowProc (hwnd, mMsg, wParam, lParam)) ;
          }
     }
