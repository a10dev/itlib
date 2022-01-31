/*-----------------------------------------------------
   SYSMETS.C -- System Metrics Display Program (Final)
                (c) Charles Petzold, 1996
  -----------------------------------------------------*/

#include <windows.h>
#include <string.h>
#include "sysmets.h"

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
     {
     static char szAppName[] = "SysMets" ;
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

     hwnd = CreateWindow (szAppName, "System Metrics",
                          WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
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
     static int  cxChar, cxCaps, cyChar, cxClient, cyClient, iMaxWidth,
                 iVscrollPos, iVscrollMax, iHscrollPos, iHscrollMax ;
     char        szBuffer[10] ;
     HDC         hdc ;
     int         i, x, y, iPaintBeg, iPaintEnd, iVscrollInc, iHscrollInc ;
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

               iMaxWidth = 40 * cxChar + 22 * cxCaps ;
               return 0 ;

          case WM_SIZE :
               cxClient = LOWORD (lParam) ;
               cyClient = HIWORD (lParam) ;

               iVscrollMax = max (0, NUMLINES + 2 - cyClient / cyChar) ;
               iVscrollPos = min (iVscrollPos, iVscrollMax) ;

               SetScrollRange (hwnd, SB_VERT, 0, iVscrollMax, FALSE) ;
               SetScrollPos   (hwnd, SB_VERT, iVscrollPos, TRUE) ;

               iHscrollMax = max (0, 2 + (iMaxWidth - cxClient) / cxChar) ;
               iHscrollPos = min (iHscrollPos, iHscrollMax) ;

               SetScrollRange (hwnd, SB_HORZ, 0, iHscrollMax, FALSE) ;
               SetScrollPos   (hwnd, SB_HORZ, iHscrollPos, TRUE) ;
               return 0 ;

          case WM_VSCROLL :
               switch (LOWORD (wParam))
                    {
                    case SB_TOP :
                         iVscrollInc = -iVscrollPos ;
                         break ;

                    case SB_BOTTOM :
                         iVscrollInc = iVscrollMax - iVscrollPos ;
                         break ;

                    case SB_LINEUP :
                         iVscrollInc = -1 ;
                         break ;

                    case SB_LINEDOWN :
                         iVscrollInc = 1 ;
                         break ;

                    case SB_PAGEUP :
                         iVscrollInc = min (-1, -cyClient / cyChar) ;
                         break ;

                    case SB_PAGEDOWN :
                         iVscrollInc = max (1, cyClient / cyChar) ;
                         break ;

                    case SB_THUMBTRACK :
                         iVscrollInc = HIWORD (wParam) - iVscrollPos ;
                         break ;

                    default :
                         iVscrollInc = 0 ;
                    }
               iVscrollInc = max (-iVscrollPos,
                             min (iVscrollInc, iVscrollMax - iVscrollPos)) ;

               if (iVscrollInc != 0)
                    {
                    iVscrollPos += iVscrollInc ;
                    ScrollWindow (hwnd, 0, -cyChar * iVscrollInc, NULL, NULL) ;
                    SetScrollPos (hwnd, SB_VERT, iVscrollPos, TRUE) ;
                    UpdateWindow (hwnd) ;
                    }
               return 0 ;

          case WM_HSCROLL :
               switch (LOWORD (wParam))
                    {
                    case SB_LINEUP :
                         iHscrollInc = -1 ;
                         break ;

                    case SB_LINEDOWN :
                         iHscrollInc = 1 ;
                         break ;

                    case SB_PAGEUP :
                         iHscrollInc = -8 ;
                         break ;

                    case SB_PAGEDOWN :
                         iHscrollInc = 8 ;
                         break ;

                    case SB_THUMBPOSITION :
                         iHscrollInc = HIWORD (wParam) - iHscrollPos ;
                         break ;

                    default :
                         iHscrollInc = 0 ;
                    }
               iHscrollInc = max (-iHscrollPos,
                             min (iHscrollInc, iHscrollMax - iHscrollPos)) ;

               if (iHscrollInc != 0)
                    {
                    iHscrollPos += iHscrollInc ;
                    ScrollWindow (hwnd, -cxChar * iHscrollInc, 0, NULL, NULL) ;
                    SetScrollPos (hwnd, SB_HORZ, iHscrollPos, TRUE) ;
                    }
               return 0 ;

          case WM_KEYDOWN :
               switch (wParam)
                    {
                    case VK_HOME :
                         SendMessage (hwnd, WM_VSCROLL, SB_TOP, 0L) ;
                         break ;

                    case VK_END :
                         SendMessage (hwnd, WM_VSCROLL, SB_BOTTOM, 0L) ;
                         break ;

                    case VK_PRIOR :
                         SendMessage (hwnd, WM_VSCROLL, SB_PAGEUP, 0L) ;
                         break ;

                    case VK_NEXT :
                         SendMessage (hwnd, WM_VSCROLL, SB_PAGEDOWN, 0L) ;
                         break ;

                    case VK_UP :
                         SendMessage (hwnd, WM_VSCROLL, SB_LINEUP, 0L) ;
                         break ;

                    case VK_DOWN :
                         SendMessage (hwnd, WM_VSCROLL, SB_LINEDOWN, 0L) ;
                         break ;

                    case VK_LEFT :
                         SendMessage (hwnd, WM_HSCROLL, SB_PAGEUP, 0L) ;
                         break ;

                    case VK_RIGHT :
                         SendMessage (hwnd, WM_HSCROLL, SB_PAGEDOWN, 0L) ;
                         break ;
                    }
               return 0 ;

          case WM_PAINT :
               hdc = BeginPaint (hwnd, &ps) ;

               iPaintBeg = max (0, iVscrollPos + ps.rcPaint.top / cyChar - 1) ;
               iPaintEnd = min (NUMLINES,
                                iVscrollPos + ps.rcPaint.bottom / cyChar) ;

               for (i = iPaintBeg ; i < iPaintEnd ; i++)
                    {
                    x = cxChar * (1 - iHscrollPos) ;
                    y = cyChar * (1 - iVscrollPos + i) ;

                    TextOut (hdc, x, y,
                             sysmetrics[i].szLabel,
                             strlen (sysmetrics[i].szLabel)) ;

                    TextOut (hdc, x + 22 * cxCaps, y,
                             sysmetrics[i].szDesc,
                             strlen (sysmetrics[i].szDesc)) ;

                    SetTextAlign (hdc, TA_RIGHT | TA_TOP) ;

                    TextOut (hdc, x + 22 * cxCaps + 40 * cxChar, y,
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
