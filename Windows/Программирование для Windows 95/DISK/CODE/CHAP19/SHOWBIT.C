/*-----------------------------------------------------------
   SHOWBIT.C -- Shows bitmaps in BITLIB dynamic link library
                (c) Charles Petzold, 1996
  -----------------------------------------------------------*/

#include <windows.h>

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
     {
     static char  szAppName[] = "ShowBit" ;
     HWND         hwnd ;
     MSG          msg ;
     WNDCLASSEX   wndclass ;

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

     hwnd = CreateWindow (szAppName, "Show Bitmaps from BITLIB (Press Key)",
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

void DrawBitmap (HDC hdc, int xStart, int yStart, HBITMAP hBitmap)
     {
     BITMAP bm ;
     HDC    hMemDC ;
     POINT  pt ;

     hMemDC = CreateCompatibleDC (hdc) ;
     SelectObject (hMemDC, hBitmap) ;
     GetObject (hBitmap, sizeof (BITMAP), (PSTR) &bm) ;
     pt.x = bm.bmWidth ;
     pt.y = bm.bmHeight ;

     BitBlt (hdc, xStart, yStart, pt.x, pt.y, hMemDC, 0, 0, SRCCOPY) ;

     DeleteDC (hMemDC) ;
     }

LRESULT CALLBACK WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
     {
     static HINSTANCE  hLibrary ;
     static int        iCurrent = 1 ;
     HBITMAP           hBitmap ;
     HDC               hdc ;
     PAINTSTRUCT       ps ;

     switch (iMsg)
          {
          case WM_CREATE :
               if ((hLibrary = LoadLibrary ("BITLIB.DLL")) == NULL)
                    DestroyWindow (hwnd) ;

               return 0 ;

          case WM_CHAR :
               if (hLibrary)
                    {
                    iCurrent ++ ;
                    InvalidateRect (hwnd, NULL, TRUE) ;
                    }
               return 0 ;

          case WM_PAINT :
               hdc = BeginPaint (hwnd, &ps) ;

               if (hLibrary)
                    {
                    if (NULL == (hBitmap = LoadBitmap (hLibrary,
                                             MAKEINTRESOURCE (iCurrent))))
                         {
                         iCurrent = 1 ;
                         hBitmap = LoadBitmap (hLibrary,
                                             MAKEINTRESOURCE (iCurrent)) ;
                         }

                    if (hBitmap)
                         {
                         DrawBitmap (hdc, 0, 0, hBitmap) ;
                         DeleteObject (hBitmap) ;
                         }
                    }

               EndPaint (hwnd, &ps) ;
               return 0 ;

          case WM_DESTROY :
               if (hLibrary)
                    FreeLibrary (hLibrary) ;

               PostQuitMessage (0) ;
               return 0 ;
          }
     return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
     }
