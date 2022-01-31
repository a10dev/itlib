/*------------------------------------------
   ABOUT1.C -- About Box Demo Program No. 1
	       (c) Charles Petzold, 1996
  ------------------------------------------*/

#include <windows.h>
#include "about1.h"

LRESULT CALLBACK WndProc      (HWND, UINT, WPARAM, LPARAM) ;
BOOL    CALLBACK AboutDlgProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
		            PSTR szCmdLine, int iCmdShow)
     {
     static char  szAppName[] = "About1" ;
     MSG          msg ;
     HWND         hwnd ;
     WNDCLASSEX   wndclass ;

     wndclass.cbSize        = sizeof (wndclass) ;
     wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = LoadIcon (hInstance, szAppName) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = szAppName ;
     wndclass.lpszClassName = szAppName ;
     wndclass.hIconSm       = LoadIcon (hInstance, szAppName) ;

     RegisterClassEx (&wndclass) ;

     hwnd = CreateWindow (szAppName, "About Box Demo Program",
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
     static WNDPROC   lpfnAboutDlgProc ;
     static HINSTANCE hInstance ;

     switch (iMsg)
          {
          case WM_CREATE :
               hInstance = ((LPCREATESTRUCT) lParam)->hInstance ;
               return 0 ;

          case WM_COMMAND :
               switch (LOWORD (wParam))
                    {
                    case IDM_ABOUT :
                         DialogBox (hInstance, "AboutBox", hwnd, AboutDlgProc) ;
                         return 0 ;
                    }
               break ;
     
          case WM_DESTROY :
               PostQuitMessage (0) ;
               return 0 ;
          }
     return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
     }

BOOL CALLBACK AboutDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
     {
     switch (iMsg)
          {
          case WM_INITDIALOG :
               return TRUE ;

          case WM_COMMAND :
               switch (LOWORD (wParam))
                    {
		            case IDOK :
		            case IDCANCEL :
                         EndDialog (hDlg, 0) ;
			             return TRUE ;
                    }
               break ;
          }
     return FALSE ;
     }
