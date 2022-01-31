/*-------------------------------------------------
   CHECKER3.C -- Mouse Hit-Test Demo Program No. 3
                 (c) Charles Petzold, 1996
  -------------------------------------------------*/

#include <windows.h>

#define DIVISIONS 5
#define MoveTo(hdc, x, y) MoveToEx (hdc, x, y, NULL)

LRESULT CALLBACK WndProc   (HWND, UINT, WPARAM, LPARAM) ;
LRESULT CALLBACK ChildWndProc (HWND, UINT, WPARAM, LPARAM) ;

char      szChildClass[] = "Checker3_Child" ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
     {
     static char szAppName[] = "Checker3" ;
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
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = szAppName ;
	 wndclass.hIconSm       = LoadIcon (NULL, IDI_APPLICATION) ;

     RegisterClassEx (&wndclass) ;

     wndclass.lpfnWndProc   = ChildWndProc ;
     wndclass.cbWndExtra    = sizeof (WORD) ;
     wndclass.hIcon         = NULL ;
     wndclass.lpszClassName = szChildClass ;
	 wndclass.hIconSm       = NULL ;

     RegisterClassEx (&wndclass) ;

     hwnd = CreateWindow (szAppName, "Checker3 Mouse Hit-Test Demo",
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
     static HWND hwndChild[DIVISIONS][DIVISIONS] ;
     int         cxBlock, cyBlock, x, y ;

     switch (iMsg)
          {
          case WM_CREATE :
               for (x = 0 ; x < DIVISIONS ; x++)
                    for (y = 0 ; y < DIVISIONS ; y++)
                         {
                         hwndChild[x][y] = CreateWindow (szChildClass, NULL,
                              WS_CHILDWINDOW | WS_VISIBLE,
                              0, 0, 0, 0,
                              hwnd, (HMENU) (y << 8 | x),
                              (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE),
                              NULL) ;
                         }
               return 0 ;

          case WM_SIZE :
               cxBlock = LOWORD (lParam) / DIVISIONS ;
               cyBlock = HIWORD (lParam) / DIVISIONS ;

               for (x = 0 ; x < DIVISIONS ; x++)
                    for (y = 0 ; y < DIVISIONS ; y++)
                         MoveWindow (hwndChild[x][y],
                              x * cxBlock, y * cyBlock,
                              cxBlock, cyBlock, TRUE) ;
               return 0 ;

          case WM_LBUTTONDOWN :
               MessageBeep (0) ;
               return 0 ;

          case WM_DESTROY :
               PostQuitMessage (0) ;
               return 0 ;
          }
     return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
     }

LRESULT CALLBACK ChildWndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
     {
     HDC         hdc ;
     PAINTSTRUCT ps ;
     RECT        rect ;

     switch (iMsg)
          {
          case WM_CREATE :
               SetWindowWord (hwnd, 0, 0) ;       // on/off flag
               return 0 ;

          case WM_LBUTTONDOWN :
               SetWindowWord (hwnd, 0, 1 ^ GetWindowWord (hwnd, 0)) ;
               InvalidateRect (hwnd, NULL, FALSE) ;
               return 0 ;

          case WM_PAINT :
               hdc = BeginPaint (hwnd, &ps) ;

               GetClientRect (hwnd, &rect) ;
               Rectangle (hdc, 0, 0, rect.right, rect.bottom) ;

               if (GetWindowWord (hwnd, 0))
                    {
                    MoveTo (hdc, 0,          0) ;
                    LineTo (hdc, rect.right, rect.bottom) ;
                    MoveTo (hdc, 0,          rect.bottom) ;
                    LineTo (hdc, rect.right, 0) ;
                    }

               EndPaint (hwnd, &ps) ;
               return 0 ;
          }
     return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
     }
