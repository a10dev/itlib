/*-----------------------------------------
   MENUDEMO.C -- Menu Demonstration
                 (c) Charles Petzold, 1996
  -----------------------------------------*/

#include <windows.h>
#include "menudemo.h"

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

char szAppName[] = "MenuDemo" ;

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
     wndclass.lpszMenuName  = szAppName ;
     wndclass.lpszClassName = szAppName ;
     wndclass.hIconSm       = LoadIcon (NULL, IDI_APPLICATION) ;

     RegisterClassEx (&wndclass) ;

     hwnd = CreateWindow (szAppName, "Menu Demonstration",
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
     static int  iColorID[5] = { WHITE_BRUSH,  LTGRAY_BRUSH, GRAY_BRUSH,
                                 DKGRAY_BRUSH, BLACK_BRUSH } ;
     static int  iSelection = IDM_WHITE ;
     HMENU       hMenu ;

     switch (iMsg)
          {
          case WM_COMMAND :
               hMenu = GetMenu (hwnd) ;

               switch (LOWORD (wParam))
                    {
                    case IDM_NEW :
                    case IDM_OPEN :
                    case IDM_SAVE :
                    case IDM_SAVEAS :
                         MessageBeep (0) ;
                         return 0 ;

                    case IDM_EXIT :
                         SendMessage (hwnd, WM_CLOSE, 0, 0L) ;
                         return 0 ;

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

                    case IDM_START :
                         if (SetTimer (hwnd, 1, 1000, NULL))
                              {
                              EnableMenuItem (hMenu, IDM_START, MF_GRAYED) ;
                              EnableMenuItem (hMenu, IDM_STOP,  MF_ENABLED) ;
                              }
                         return 0 ;

                    case IDM_STOP :
                         KillTimer (hwnd, 1) ;
                         EnableMenuItem (hMenu, IDM_START, MF_ENABLED) ;
                         EnableMenuItem (hMenu, IDM_STOP,  MF_GRAYED) ;
                         return 0 ;

                    case IDM_HELP :
                         MessageBox (hwnd, "Help not yet implemented!",
                                     szAppName, MB_ICONEXCLAMATION | MB_OK) ;
                         return 0 ;

                    case IDM_ABOUT :
                         MessageBox (hwnd, "Menu Demonstration Program.",
                                     szAppName, MB_ICONINFORMATION | MB_OK) ;
                         return 0 ;
                    }
               break ;

          case WM_TIMER :
               MessageBeep (0) ;
               return 0 ;

          case WM_DESTROY :
               PostQuitMessage (0) ;
               return 0 ;
          }
     return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
     }
