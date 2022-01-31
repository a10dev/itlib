/*---------------------------------------------------------
   DEVCAPS1.C -- Device Capabilities Display Program No. 1
                 (c) Charles Petzold, 1996
  ---------------------------------------------------------*/

#include <windows.h>
#include <string.h>

#define NUMLINES ((int) (sizeof devcaps / sizeof devcaps [0]))

struct
     {
     int  iIndex ;
     char *szLabel ;
     char *szDesc ;
     }
     devcaps [] =
          {
          HORZSIZE,      "HORZSIZE",     "Width in millimeters:",
          VERTSIZE,      "VERTSIZE",     "Height in millimeters:",
          HORZRES,       "HORZRES",      "Width in pixels:",
          VERTRES,       "VERTRES",      "Height in raster lines:",
          BITSPIXEL,     "BITSPIXEL",    "Color bits per pixel:",
          PLANES,        "PLANES",       "Number of color planes:",
          NUMBRUSHES,    "NUMBRUSHES",   "Number of device brushes:",
          NUMPENS,       "NUMPENS",      "Number of device pens:",
          NUMMARKERS,    "NUMMARKERS",   "Number of device markers:",
          NUMFONTS,      "NUMFONTS",     "Number of device fonts:",
          NUMCOLORS,     "NUMCOLORS",    "Number of device colors:",
          PDEVICESIZE,   "PDEVICESIZE",  "Size of device structure:",
          ASPECTX,       "ASPECTX",      "Relative width of pixel:",
          ASPECTY,       "ASPECTY",      "Relative height of pixel:",
          ASPECTXY,      "ASPECTXY",     "Relative diagonal of pixel:",
          LOGPIXELSX,    "LOGPIXELSX",   "Horizontal dots per inch:",
          LOGPIXELSY,    "LOGPIXELSY",   "Vertical dots per inch:",
          SIZEPALETTE,   "SIZEPALETTE",  "Number of palette entries:",
          NUMRESERVED,   "NUMRESERVED",  "Reserved palette entries:",
          COLORRES,      "COLORRES",     "Actual color resolution:"
          } ;

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
     {
     static char szAppName[] = "DevCaps1" ;
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

     hwnd = CreateWindow (szAppName, "Device Capabilities",
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
     static int  cxChar, cxCaps, cyChar ;
     char        szBuffer[10] ;
     HDC         hdc ;
     int         i ;
     PAINTSTRUCT ps ;
     TEXTMETRIC  tm ;

     switch (iMsg)
          {
          case WM_CREATE:
               hdc = GetDC (hwnd) ;

               GetTextMetrics (hdc, &tm) ;
               cxChar = tm.tmAveCharWidth ;
               cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2 ;
               cyChar = tm.tmHeight + tm.tmExternalLeading ;

               ReleaseDC (hwnd, hdc) ;
               return 0 ;

          case WM_PAINT:
               hdc = BeginPaint (hwnd, &ps) ;

               for (i = 0 ; i < NUMLINES ; i++)
                    {
			        TextOut (hdc, cxChar, cyChar * (1 + i),
                             devcaps[i].szLabel,
                             strlen (devcaps[i].szLabel)) ;

                    TextOut (hdc, cxChar + 22 * cxCaps, cyChar * (1 + i),
                             devcaps[i].szDesc,
                             strlen (devcaps[i].szDesc)) ;

                    SetTextAlign (hdc, TA_RIGHT | TA_TOP) ;

                    TextOut (hdc, cxChar + 22 * cxCaps + 40 * cxChar,
			                 cyChar * (1 + i), szBuffer,
                             wsprintf (szBuffer, "%5d",
                                  GetDeviceCaps (hdc, devcaps[i].iIndex))) ;

                    SetTextAlign (hdc, TA_LEFT | TA_TOP) ;
                    }

               EndPaint (hwnd, &ps) ;
               return 0 ;

          case WM_DESTROY:
               PostQuitMessage (0) ;
               return 0 ;
          }

     return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
     }
