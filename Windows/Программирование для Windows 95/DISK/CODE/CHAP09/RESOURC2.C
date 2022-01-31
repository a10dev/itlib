/*-----------------------------------------------------------
   RESOURC2.C -- Icon and Cursor Demonstration Program No. 2
                 (c) Charles Petzold, 1996
  -----------------------------------------------------------*/

#include <windows.h>

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

char      szAppName[] = "Resourc2" ;
HINSTANCE hInst ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
     {
     HBITMAP    hBitmap ;
     HBRUSH     hBrush ;
     HWND       hwnd ;
     MSG        msg ;
     WNDCLASSEX wndclass ;

     hBitmap = LoadBitmap (hInstance, szAppName) ;
     hBrush = CreatePatternBrush (hBitmap) ;

     wndclass.cbSize        = sizeof (wndclass) ;
     wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = LoadIcon (hInstance, szAppName) ;
     wndclass.hCursor       = LoadCursor (hInstance, szAppName) ;
     wndclass.hbrBackground = hBrush ;
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = szAppName ;
     wndclass.hIconSm       = LoadIcon (hInstance, szAppName) ;

     RegisterClassEx (&wndclass) ;

     hInst = hInstance ;

     hwnd = CreateWindow (szAppName, "Icon and Cursor Demo",
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

     DeleteObject ((HGDIOBJ) hBrush) ;       // clean-up
     DeleteObject ((HGDIOBJ) hBitmap) ;

     return msg.wParam ;
     }

LRESULT CALLBACK WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
     {
     static HICON hIcon ;
     static int   cxIcon, cyIcon, cxClient, cyClient ;
     HDC          hdc ;
     PAINTSTRUCT  ps ;
     int          x, y ;

     switch (iMsg)
          {
          case WM_CREATE :
               hIcon = LoadIcon (hInst, szAppName) ;
               cxIcon = GetSystemMetrics (SM_CXICON) ;
               cyIcon = GetSystemMetrics (SM_CYICON) ;
               return 0 ;

          case WM_SIZE :
               cxClient = LOWORD (lParam) ;
               cyClient = HIWORD (lParam) ;
               return 0 ;

          case WM_PAINT :
               hdc = BeginPaint (hwnd, &ps) ;

               for (y = cyIcon ; y < cyClient ; y += 2 * cyIcon)
                    for (x = cxIcon ; x < cxClient ; x += 2 * cxIcon)
                         DrawIcon (hdc, x, y, hIcon) ;

               EndPaint (hwnd, &ps) ;
               return 0 ;

          case WM_DESTROY :
               PostQuitMessage (0) ;
               return 0 ;
          }
     return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
     }
