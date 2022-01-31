/*----------------------------------------
   ENVIRON.C -- Environment List Box
                (c) Charles Petzold, 1996
  ----------------------------------------*/

#include <windows.h>
#include <stdlib.h>
#include <string.h>

#define  MAXENV  4096

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
     {
     static char szAppName[] = "Environ" ;
     HWND        hwnd ;
     MSG         msg ;
     WNDCLASSEX  wndclass ;

	 wndclass.cbSize        = sizeof (wndclass) ;
     wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1) ;
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = szAppName ;
	 wndclass.hIconSm       = LoadIcon (NULL, IDI_APPLICATION) ;

     RegisterClassEx (&wndclass) ;

     hwnd = CreateWindow (szAppName, "Environment List Box",
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
     static char szBuffer[MAXENV + 1] ;
     static HWND hwndList, hwndText ;
     HDC         hdc ;
     int         i ;
     TEXTMETRIC  tm ;

     switch (iMsg)
          {
          case WM_CREATE :
               hdc = GetDC (hwnd) ;
               GetTextMetrics (hdc, &tm) ;
               ReleaseDC (hwnd, hdc) ;

               hwndList = CreateWindow ("listbox", NULL,
                              WS_CHILD | WS_VISIBLE | LBS_STANDARD,
                              tm.tmAveCharWidth, tm.tmHeight * 3,
                              tm.tmAveCharWidth * 16 +
                                   GetSystemMetrics (SM_CXVSCROLL),
                              tm.tmHeight * 5,
                              hwnd, (HMENU) 1,
                              (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE),
                              NULL) ;

               hwndText = CreateWindow ("static", NULL,
                              WS_CHILD | WS_VISIBLE | SS_LEFT,
                              tm.tmAveCharWidth,          tm.tmHeight,
                              tm.tmAveCharWidth * MAXENV, tm.tmHeight,
                              hwnd, (HMENU) 2,
                              (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE),
                              NULL) ;

               for (i = 0 ; environ[i] ; i++)
                    {
                    if (strlen (environ [i]) > MAXENV)
                         continue ;
                    *strchr (strcpy (szBuffer, environ [i]), '=') = '\0' ;
                    SendMessage (hwndList, LB_ADDSTRING, 0, (LPARAM) szBuffer) ;
                    }
               return 0 ;

          case WM_SETFOCUS :
               SetFocus (hwndList) ;
               return 0 ;

          case WM_COMMAND :
               if (LOWORD (wParam) == 1 && HIWORD (wParam) == LBN_SELCHANGE)
                    {
                    i = SendMessage (hwndList, LB_GETCURSEL, 0, 0) ;
                    i = SendMessage (hwndList, LB_GETTEXT, i,
                                               (LPARAM) szBuffer) ;

                    strcpy (szBuffer + i + 1, getenv (szBuffer)) ;
                    *(szBuffer + i) = '=' ;

                    SetWindowText (hwndText, szBuffer) ;
                    }
               return 0 ;

          case WM_DESTROY :
               PostQuitMessage (0) ;
               return 0 ;
          }
     return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
     }
