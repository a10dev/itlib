/*----------------------------------------
   COLORS1.C -- Colors Using Scroll Bars
                (c) Charles Petzold, 1996
  ----------------------------------------*/

#include <windows.h>
#include <stdlib.h>

LRESULT CALLBACK WndProc    (HWND, UINT, WPARAM, LPARAM) ;
LRESULT CALLBACK ScrollProc (HWND, UINT, WPARAM, LPARAM) ;

WNDPROC fnOldScr[3] ;
HWND    hwndScrol[3], hwndLabel[3], hwndValue[3], hwndRect ;
int     color[3], iFocus ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
     {
     static char  szAppName[] = "Colors1" ;
     static char *szColorLabel[] = { "Red", "Green", "Blue" } ;
     HWND         hwnd ;
     int          i ;
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
     wndclass.hbrBackground = CreateSolidBrush (0L) ;
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = szAppName ;
	 wndclass.hIconSm       = LoadIcon (NULL, IDI_APPLICATION) ;

     RegisterClassEx (&wndclass) ;

     hwnd = CreateWindow (szAppName, "Color Scroll",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          NULL, NULL, hInstance, NULL) ;

     hwndRect = CreateWindow ("static", NULL,
                              WS_CHILD | WS_VISIBLE | SS_WHITERECT,
                              0, 0, 0, 0,
                              hwnd, (HMENU) 9, hInstance, NULL) ;

     for (i = 0 ; i < 3 ; i++)
          {
          hwndScrol[i] = CreateWindow ("scrollbar", NULL,
                              WS_CHILD | WS_VISIBLE | WS_TABSTOP | SBS_VERT,
                              0, 0, 0, 0,
                              hwnd, (HMENU) i, hInstance, NULL) ;

          hwndLabel[i] = CreateWindow ("static", szColorLabel[i],
                              WS_CHILD | WS_VISIBLE | SS_CENTER,
                              0, 0, 0, 0,
                              hwnd, (HMENU) (i + 3), hInstance, NULL) ;

          hwndValue[i] = CreateWindow ("static", "0",
                              WS_CHILD | WS_VISIBLE | SS_CENTER,
                              0, 0, 0, 0,
                              hwnd, (HMENU) (i + 6), hInstance, NULL) ;

          fnOldScr[i] = (WNDPROC) SetWindowLong (hwndScrol[i], GWL_WNDPROC,
                                                 (LONG) ScrollProc) ;

          SetScrollRange (hwndScrol[i], SB_CTL, 0, 255, FALSE) ;
          SetScrollPos   (hwndScrol[i], SB_CTL, 0, FALSE) ;
          }

     ShowWindow (hwnd, iCmdShow) ;
     UpdateWindow (hwnd) ;

     while (GetMessage (&msg, NULL, 0, 0))
          {
          TranslateMessage (&msg) ;
          DispatchMessage  (&msg) ;
          }
     return msg.wParam ;
     }

LRESULT CALLBACK WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
     {
     static COLORREF crPrim[3] = { RGB (255, 0, 0), RGB (0, 255, 0),
                                   RGB (0, 0, 255) } ;
     static HBRUSH   hBrush[3], hBrushStatic ;
     static int      cyChar ;
     static RECT     rcColor ;
     char            szbuffer[10] ;
     int             i, cxClient, cyClient ;

     switch (iMsg)
          {
          case WM_CREATE :
               for (i = 0 ; i < 3 ; i++)
                    hBrush[i] = CreateSolidBrush (crPrim[i]) ;

               hBrushStatic = CreateSolidBrush (
                                   GetSysColor (COLOR_BTNHIGHLIGHT)) ;

               cyChar = HIWORD (GetDialogBaseUnits ()) ;
               return 0 ;

          case WM_SIZE :
               cxClient = LOWORD (lParam) ;
               cyClient = HIWORD (lParam) ;

               SetRect (&rcColor, cxClient / 2, 0, cxClient, cyClient) ;

               MoveWindow (hwndRect, 0, 0, cxClient / 2, cyClient, TRUE) ;

               for (i = 0 ; i < 3 ; i++)
                    {
                    MoveWindow (hwndScrol[i],
                         (2 * i + 1) * cxClient / 14, 2 * cyChar,
                         cxClient / 14, cyClient - 4 * cyChar, TRUE) ;

                    MoveWindow (hwndLabel[i],
                         (4 * i + 1) * cxClient / 28, cyChar / 2,
                         cxClient / 7, cyChar, TRUE) ;

                    MoveWindow (hwndValue[i],
                         (4 * i + 1) * cxClient / 28, cyClient - 3 * cyChar / 2,
                         cxClient / 7, cyChar, TRUE) ;
                    }
               SetFocus (hwnd) ;
               return 0 ;

          case WM_SETFOCUS :
               SetFocus (hwndScrol[iFocus]) ;
               return 0 ;

          case WM_VSCROLL :
               i = GetWindowLong ((HWND) lParam, GWL_ID) ;

               switch (LOWORD (wParam))
                    {
                    case SB_PAGEDOWN :
                         color[i] += 15 ;
                                                  // fall through
                    case SB_LINEDOWN :
                         color[i] = min (255, color[i] + 1) ;
                         break ;

                    case SB_PAGEUP :
                         color[i] -= 15 ;
                                                  // fall through
                    case SB_LINEUP :
                         color[i] = max (0, color[i] - 1) ;
                         break ;

                    case SB_TOP :
                         color[i] = 0 ;
                         break ;

                    case SB_BOTTOM :
                         color[i] = 255 ;
                         break ;

                    case SB_THUMBPOSITION :
                    case SB_THUMBTRACK :
                         color[i] = HIWORD (wParam) ;
                         break ;

                    default :
                         break ;
                    }
               SetScrollPos  (hwndScrol[i], SB_CTL, color[i], TRUE) ;
               SetWindowText (hwndValue[i], itoa (color[i], szbuffer, 10)) ;

               DeleteObject ((HBRUSH)
                    SetClassLong (hwnd, GCL_HBRBACKGROUND,
                         (LONG) CreateSolidBrush (
                                     RGB (color[0], color[1], color[2])))) ;

               InvalidateRect (hwnd, &rcColor, TRUE) ;
               return 0 ;

          case WM_CTLCOLORSCROLLBAR :
               i = GetWindowLong ((HWND) lParam, GWL_ID) ;

               return (LRESULT) hBrush[i] ;

          case WM_CTLCOLORSTATIC :
               i = GetWindowLong ((HWND) lParam, GWL_ID) ;

               if (i >= 3 && i <= 8)    // static text controls
                    {
                    SetTextColor ((HDC) wParam, crPrim[i % 3]) ;
                    SetBkColor ((HDC) wParam, GetSysColor (COLOR_BTNHIGHLIGHT));

                    return (LRESULT) hBrushStatic ;
                    }
               break ;

          case WM_SYSCOLORCHANGE :
               DeleteObject (hBrushStatic) ;

               hBrushStatic = CreateSolidBrush (
                                   GetSysColor (COLOR_BTNHIGHLIGHT)) ;
               return 0 ;

          case WM_DESTROY :
               DeleteObject ((HBRUSH)
                    SetClassLong (hwnd, GCL_HBRBACKGROUND,
                         (LONG) GetStockObject (WHITE_BRUSH))) ;

               for (i = 0 ; i < 3 ; DeleteObject (hBrush[i++])) ;

               DeleteObject (hBrushStatic) ;

               PostQuitMessage (0) ;
               return 0 ;
          }
     return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
     }

LRESULT CALLBACK ScrollProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
     {
     int i = GetWindowLong (hwnd, GWL_ID) ;

     switch (iMsg)
          {
          case WM_KEYDOWN :
               if (wParam == VK_TAB)
                    SetFocus (hwndScrol[(i +
                         (GetKeyState (VK_SHIFT) < 0 ? 2 : 1)) % 3]) ;
               break ;

          case WM_SETFOCUS :
               iFocus = i ;
               break ;
          }
     return CallWindowProc (fnOldScr[i], hwnd, iMsg, wParam, lParam) ;
     }
