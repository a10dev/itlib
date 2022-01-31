/*---------------------------------------------
   HEAD.C -- Displays beginning (head) of file
             (c) Charles Petzold, 1996
  ---------------------------------------------*/

#include <windows.h>
#include <string.h>
#include <direct.h>

#define  MAXPATH     256
#define  MAXREAD    8192

LRESULT CALLBACK WndProc  (HWND, UINT, WPARAM, LPARAM) ;
LRESULT CALLBACK ListProc (HWND, UINT, WPARAM, LPARAM) ;

WNDPROC fnOldList ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
     {
     static char szAppName[] = "Head" ;
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
     wndclass.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1) ;
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = szAppName ;
	 wndclass.hIconSm       = LoadIcon (NULL, IDI_APPLICATION) ;

     RegisterClassEx (&wndclass) ;

     hwnd = CreateWindow (szAppName, "File Head",
                          WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
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
     static BOOL     bValidFile ;
     static char     sReadBuffer[MAXREAD], szFile[MAXPATH] ;
     static HWND     hwndList, hwndText ;
     static OFSTRUCT ofs ;
     static RECT     rect ;
     char            szBuffer[MAXPATH + 1] ;
     HDC             hdc ;
     int             iHandle, i ;
     PAINTSTRUCT     ps ;
     TEXTMETRIC      tm ;

     switch (iMsg)
          {
          case WM_CREATE :
               hdc = GetDC (hwnd) ;
               SelectObject (hdc, GetStockObject (SYSTEM_FIXED_FONT)) ;
               GetTextMetrics (hdc, &tm) ;
               ReleaseDC (hwnd, hdc) ;

               rect.left = 20 * tm.tmAveCharWidth ;
               rect.top  =  3 * tm.tmHeight ;

               hwndList = CreateWindow ("listbox", NULL,
                              WS_CHILDWINDOW | WS_VISIBLE | LBS_STANDARD,
                              tm.tmAveCharWidth, tm.tmHeight * 3,
                              tm.tmAveCharWidth * 13 +
                                   GetSystemMetrics (SM_CXVSCROLL),
                              tm.tmHeight * 10,
                              hwnd, (HMENU) 1,
                              (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE),
                              NULL) ;

               hwndText = CreateWindow ("static", getcwd (szBuffer, MAXPATH),
                              WS_CHILDWINDOW | WS_VISIBLE | SS_LEFT,
                              tm.tmAveCharWidth,           tm.tmHeight,
                              tm.tmAveCharWidth * MAXPATH, tm.tmHeight,
                              hwnd, (HMENU) 2,
                              (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE),
                              NULL) ;

               fnOldList = (WNDPROC) SetWindowLong (hwndList, GWL_WNDPROC,
                                                    (LPARAM) ListProc) ;

               SendMessage (hwndList, LB_DIR, 0x37, (LPARAM) "*.*") ;
               return 0 ;

          case WM_SIZE :
               rect.right  = LOWORD (lParam) ;
               rect.bottom = HIWORD (lParam) ;
               return 0 ;

          case WM_SETFOCUS :
               SetFocus (hwndList) ;
               return 0 ;

          case WM_COMMAND :
               if (LOWORD (wParam) == 1 && HIWORD (wParam) == LBN_DBLCLK)
                    {
                    if (LB_ERR == (i = SendMessage (hwndList,
                                                    LB_GETCURSEL, 0, 0L)))
                         break ;

                    SendMessage (hwndList, LB_GETTEXT, i, (LPARAM) szBuffer) ;

                    if (-1 != OpenFile (szBuffer, &ofs, OF_EXIST | OF_READ))
                         {
                         bValidFile = TRUE ;
                         strcpy (szFile, szBuffer) ;
                         getcwd (szBuffer, MAXPATH) ;
                         if (szBuffer [strlen (szBuffer) - 1] != '\\')
                              strcat (szBuffer, "\\") ;
                         SetWindowText (hwndText, strcat (szBuffer, szFile)) ;
                         }
                    else
                         {
                         bValidFile = FALSE ;
                         szBuffer [strlen (szBuffer) - 1] = '\0' ;
                         chdir (szBuffer + 1) ;
                         getcwd (szBuffer, MAXPATH) ;
                         SetWindowText (hwndText, szBuffer) ;
                         SendMessage (hwndList, LB_RESETCONTENT, 0, 0L) ;
                         SendMessage (hwndList, LB_DIR, 0x37, (LONG) "*.*") ;
                         }
                    InvalidateRect (hwnd, NULL, TRUE) ;
                    }
               return 0 ;

          case WM_PAINT :
               hdc = BeginPaint (hwnd, &ps) ;
               SelectObject (hdc, GetStockObject (SYSTEM_FIXED_FONT)) ;
               SetTextColor (hdc, GetSysColor (COLOR_BTNTEXT)) ;
               SetBkColor   (hdc, GetSysColor (COLOR_BTNFACE)) ;

               if (bValidFile && -1 != (iHandle =
                         OpenFile (szFile, &ofs, OF_REOPEN | OF_READ)))
                    {
                    i = _lread (iHandle, sReadBuffer, MAXREAD) ;
                    _lclose (iHandle) ;
                    DrawText (hdc, sReadBuffer, i, &rect, DT_WORDBREAK |
                                   DT_EXPANDTABS | DT_NOCLIP | DT_NOPREFIX) ;
                    }
               else
                    bValidFile = FALSE ;

               EndPaint (hwnd, &ps) ;
               return 0 ;

          case WM_DESTROY :
               PostQuitMessage (0) ;
               return 0 ;
          }
     return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
     }

LRESULT CALLBACK ListProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
     {
     if (iMsg == WM_KEYDOWN && wParam == VK_RETURN)

          SendMessage (GetParent (hwnd), WM_COMMAND, 1,
                       MAKELONG (hwnd, LBN_DBLCLK)) ;

     return CallWindowProc (fnOldList, hwnd, iMsg, wParam, lParam) ;
     }
