/*----------------------------------------
   POPMENU.C -- Popup Menu Demonstration
                (c) Charles Petzold, 1996
  ----------------------------------------*/

#include <windows.h>
#include "popmenu.h"

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

char      szAppName[] = "PopMenu" ;
HINSTANCE hInst ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
     {
     HWND       hwnd ;
     MSG        msg ;
     WNDCLASSEX wndclass ;

     wndclass.cbSize        = sizeof (wndclass) ;
     wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = szAppName ;
     wndclass.hIconSm       = LoadIcon (NULL, IDI_APPLICATION) ;

     RegisterClassEx (&wndclass) ;

     hInst = hInstance ;

     hwnd = CreateWindow (szAppName, "Popup Menu Demonstration",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          NULL, NULL, hInstance, NULL) ;

     ShowWindow (hwnd, iCmdShow) ;
     UpdateWindow (hwnd) ;

     while (GetMessage (&msg, NULL, 0, 0))
          {
          TranslateMessage (&msg) ;
          DispatchMessage (&msg) ;
          }
     return msg.wParam ;
     }

LRESULT CALLBACK WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
     {
     static HMENU hMenu ;
     static int   iColorID[5] = { WHITE_BRUSH,  LTGRAY_BRUSH, GRAY_BRUSH,
                                  DKGRAY_BRUSH, BLACK_BRUSH } ;
     static int   iSelection = IDM_WHITE ;
     POINT        point ;

     switch (iMsg)
          {
          case WM_CREATE :
               hMenu = LoadMenu (hInst, szAppName) ;
               hMenu = GetSubMenu (hMenu, 0) ;
               return 0 ;

          case WM_RBUTTONDOWN :
               point.x = LOWORD (lParam) ;
               point.y = HIWORD (lParam) ;
               ClientToScreen (hwnd, &point) ;

               TrackPopupMenu (hMenu, 0, point.x, point.y, 0, hwnd, NULL) ;
               return 0 ;

          case WM_COMMAND :
               switch (LOWORD (wParam))
                    {
                    case IDM_NEW :
                    case IDM_OPEN :
                    case IDM_SAVE :
                    case IDM_SAVEAS :
                    case IDM_UNDO :
                    case IDM_CUT :
                    case IDM_COPY :
                    case IDM_PASTE :
                    case IDM_DEL :
                         MessageBeep (0) ;
                         return 0 ;

                    case IDM_WHITE :          // Note: Logic below
                    case IDM_LTGRAY :         //   assumes that IDM_WHITE
                    case IDM_GRAY :           //   through IDM_BLACK are
                    case IDM_DKGRAY :         //   consecutive numbers in
                    case IDM_BLACK :          //   the order shown here.

                         CheckMenuItem (hMenu, iSelection, MF_UNCHECKED) ;
                         iSelection = LOWORD (wParam) ;
                         CheckMenuItem (hMenu, iSelection, MF_CHECKED) ;
     
                         SetClassLong (hwnd, GCL_HBRBACKGROUND,
                              (LONG) GetStockObject 
							     (iColorID[LOWORD (wParam) - IDM_WHITE])) ;

                         InvalidateRect (hwnd, NULL, TRUE) ;
                         return 0 ;

                    case IDM_ABOUT :
                         MessageBox (hwnd, "Popup Menu Demonstration Program.",
                                     szAppName, MB_ICONINFORMATION | MB_OK) ;
                         return 0 ;

                    case IDM_EXIT :
                         SendMessage (hwnd, WM_CLOSE, 0, 0) ;
                         return 0 ;

                    case IDM_HELP :
                         MessageBox (hwnd, "Help not yet implemented!",
                                     szAppName, MB_ICONEXCLAMATION | MB_OK) ;
                         return 0 ;
                    }
               break ;

          case WM_DESTROY :
               PostQuitMessage (0) ;
               return 0 ;
          }
     return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
     }
