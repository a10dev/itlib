/*----------------------------------------------------
   SYSMETS2.C -- System Metrics Display Program No. 2
                 (c) Charles Petzold, 1996
  ----------------------------------------------------*/

#include <windows.h>
#include <string.h>
#include "sysmets.h"

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
     {
     static char szAppName[] = "SysMets2" ;
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

     hwnd = CreateWindow (szAppName, "Get System Metrics No. 2",
                          WS_OVERLAPPEDWINDOW | WS_VSCROLL,
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
     static int  cxChar, cxCaps, cyChar, cyClient, iVscrollPos ;
     char        szBuffer[10] ;
     HDC         hdc ;
     int         i, y ;
     PAINTSTRUCT ps ;
     TEXTMETRIC  tm ;

     switch (iMsg)
          {
          case WM_CREATE :
               hdc = GetDC (hwnd) ;

               GetTextMetrics (hdc, &tm) ;
               cxChar = tm.tmAveCharWidth ;
               cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2 ;
               cyChar = tm.tmHeight + tm.tmExternalLeading ;

               ReleaseDC (hwnd, hdc) ;

               SetScrollRange (hwnd, SB_VERT, 0, NUMLINES, FALSE) ;
               SetScrollPos   (hwnd, SB_VERT, iVscrollPos, TRUE) ;
               return 0 ;

          case WM_SIZE :
               cyClient = HIWORD (lParam) ;
               return 0 ;

          case WM_VSCROLL :
               switch (LOWORD (wParam))
                    {
                    case SB_LINEUP :
                         iVscrollPos -= 1 ;
                         break ;

                    case SB_LINEDOWN :
                         iVscrollPos += 1 ;
                         break ;

                    case SB_PAGEUP :
                         iVscrollPos -= cyClient / cyChar ;
                         break ;

                    case SB_PAGEDOWN :
                         iVscrollPos += cyClient / cyChar ;
                         break ;

                    case SB_THUMBPOSITION :
                         iVscrollPos = HIWORD (wParam) ;
                         break ;

                    default :
                         break ;
                    }
               iVscrollPos = max (0, min (iVscrollPos, NUMLINES)) ;

               if (iVscrollPos != GetScrollPos (hwnd, SB_VERT))
                    {
                    SetScrollPos (hwnd, SB_VERT, iVscrollPos, TRUE) ;
                    InvalidateRect (hwnd, NULL, TRUE) ;
                    }
               return 0 ;

          case WM_PAINT :
               hdc = BeginPaint (hwnd, &ps) ;

               for (i = 0 ; i < NUMLINES ; i++)
                    {
                    y = cyChar * (1 - iVscrollPos + i) ;

                    TextOut (hdc, cxChar, y,
                             sysmetrics[i].szLabel,
                             strlen (sysmetrics[i].szLabel)) ;

                    TextOut (hdc, cxChar + 22 * cxCaps, y,
                             sysmetrics[i].szDesc,
                             strlen (sysmetrics[i].szDesc)) ;

                    SetTextAlign (hdc, TA_RIGHT | TA_TOP) ;

                    TextOut (hdc, cxChar + 22 * cxCaps + 40 * cxChar, y,
                             szBuffer,
                             wsprintf (szBuffer, "%5d",
			                 GetSystemMetrics (sysmetrics[i].iIndex))) ;

                    SetTextAlign (hdc, TA_LEFT | TA_TOP) ;
                    }

               EndPaint (hwnd, &ps) ;
               return 0 ;

          case WM_DESTROY :
               PostQuitMessage (0) ;
               return 0 ;
          }

     return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
     }
