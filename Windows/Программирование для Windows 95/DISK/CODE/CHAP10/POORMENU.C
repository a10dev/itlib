/*-----------------------------------------
   POORMENU.C -- The Poor Person's Menu
                 (c) Charles Petzold, 1996
  -----------------------------------------*/

#include <windows.h>

#define IDM_ABOUT   1
#define IDM_HELP    2
#define IDM_REMOVE  3

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

static char szAppName[] = "PoorMenu" ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
     {
     HMENU      hMenu ;
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

     hwnd = CreateWindow (szAppName, "The Poor-Person's Menu",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          NULL, NULL, hInstance, NULL) ;

     hMenu = GetSystemMenu (hwnd, FALSE) ;

     AppendMenu (hMenu, MF_SEPARATOR, 0,          NULL) ;
     AppendMenu (hMenu, MF_STRING,    IDM_ABOUT,  "About...") ;
     AppendMenu (hMenu, MF_STRING,    IDM_HELP,   "Help...") ;
     AppendMenu (hMenu, MF_STRING,    IDM_REMOVE, "Remove Additions") ;

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
     switch (iMsg)
          {
          case WM_SYSCOMMAND :
               switch (LOWORD (wParam))
                    {
                    case IDM_ABOUT :
                         MessageBox (hwnd, "A Poor-Person's Menu Program.",
                                     szAppName, MB_OK | MB_ICONINFORMATION) ;
                         return 0 ;

                    case IDM_HELP :
                         MessageBox (hwnd, "Help not yet implemented!",
                                     szAppName, MB_OK | MB_ICONEXCLAMATION) ;
                         return 0 ;

                    case IDM_REMOVE :
                         GetSystemMenu (hwnd, TRUE) ;
                         return 0 ;
                    }
               break ;

          case WM_DESTROY :
               PostQuitMessage (0) ;
               return 0 ;
          }
     return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
     }
