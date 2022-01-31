/*------------------------------------------------
   SCRAMBLE.C -- Scramble (and Unscramble) Screen
                 (c) Charles Petzold, 1996
  ------------------------------------------------*/

#include <windows.h>
#include <stdlib.h>

#define NUM 200

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
     {
     static int iKeep [NUM][4] ;
     HDC        hdc, hdcMem ;
     int        cx, cy ;
     HBITMAP    hBitmap ;
     int        i, j, x1, y1, x2, y2 ;

     if (LockWindowUpdate (GetDesktopWindow ()))
          {
          hdc     = CreateDC ("DISPLAY", NULL, NULL, NULL) ;
          hdcMem  = CreateCompatibleDC (hdc) ;
          cx  = GetSystemMetrics (SM_CXSCREEN) / 10 ;
          cy  = GetSystemMetrics (SM_CYSCREEN) / 10 ;
          hBitmap = CreateCompatibleBitmap (hdc, cx, cy) ;

          SelectObject (hdcMem, hBitmap) ;

          srand ((int) GetCurrentTime ()) ;

          for (i = 0 ; i < 2 ; i++)
               for (j = 0 ; j < NUM ; j++)
                    {
                    if (i == 0)
                         {
                         iKeep [j] [0] = x1 = cx * (rand () % 10) ;
                         iKeep [j] [1] = y1 = cy * (rand () % 10) ;
                         iKeep [j] [2] = x2 = cx * (rand () % 10) ;
                         iKeep [j] [3] = y2 = cy * (rand () % 10) ;
                         }
                    else
                         {
                         x1 = iKeep [NUM - 1 - j] [0] ;
                         y1 = iKeep [NUM - 1 - j] [1] ;
                         x2 = iKeep [NUM - 1 - j] [2] ;
                         y2 = iKeep [NUM - 1 - j] [3] ;
                         }
                    BitBlt (hdcMem, 0, 0, cx, cy, hdc,  x1, y1, SRCCOPY) ;
                    BitBlt (hdc,  x1, y1, cx, cy, hdc,  x2, y2, SRCCOPY) ;
                    BitBlt (hdc,  x2, y2, cx, cy, hdcMem, 0, 0, SRCCOPY) ;

                    Sleep (10) ;
                    }

          DeleteDC (hdcMem) ;
          DeleteDC (hdc) ;
          DeleteObject (hBitmap) ;

          LockWindowUpdate (NULL) ;
          }

     return FALSE ;
     }
