/*--------------------------------------------------------
   STRPROG.C -- Program using STRLIB dynamic link library
                (c) Charles Petzold, 1996
  --------------------------------------------------------*/

#include <windows.h>
#include <string.h>
#include "strprog.h"
#include "strlib.h"

#define MAXLEN 32
#define WM_DATACHANGE WM_USER

typedef struct
     {
     HDC  hdc ;
     int  xText ;
     int  yText ;
     int  xStart ;
     int  yStart ;
     int  xIncr ;
     int  yIncr ;
     int  xMax ;
     int  yMax ;
     }
     CBPARAM ;

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

char szAppName[] = "StrProg" ;
char szString[MAXLEN] ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
     {
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
     wndclass.lpszMenuName  = szAppName ;
     wndclass.lpszClassName = szAppName ;
     wndclass.hIconSm       = LoadIcon (NULL, IDI_APPLICATION) ;

     RegisterClassEx (&wndclass) ;

     hwnd = CreateWindow (szAppName, "DLL Demonstration Program",
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

BOOL CALLBACK DlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
     {
     switch (iMsg)
          {
          case WM_INITDIALOG :
               SendDlgItemMessage (hDlg, IDD_STRING, EM_LIMITTEXT,
                                   MAXLEN - 1, 0) ;
               return TRUE ;

          case WM_COMMAND :
               switch (wParam)
                    {
                    case IDOK :
                         GetDlgItemText (hDlg, IDD_STRING, szString, MAXLEN) ;
                         EndDialog (hDlg, TRUE) ;
                         return TRUE ;

                    case IDCANCEL :
                         EndDialog (hDlg, FALSE) ;
                         return TRUE ;
                    }
          }
     return FALSE ;
     }

BOOL CALLBACK EnumCallBack (HWND hwnd, LPARAM lParam)
     {
     char szClassName[16] ;

     GetClassName (hwnd, szClassName, sizeof (szClassName)) ;

     if (0 == strcmp (szClassName, szAppName))
          SendMessage (hwnd, WM_DATACHANGE, 0, 0) ;

     return TRUE ;
     }

BOOL CALLBACK GetStrCallBack (PSTR pString, CBPARAM *pcbp)
     {
     TextOut (pcbp->hdc, pcbp->xText, pcbp->yText,
              pString, strlen (pString)) ;

     if ((pcbp->yText += pcbp->yIncr) > pcbp->yMax)
          {
          pcbp->yText = pcbp->yStart ;
          if ((pcbp->xText += pcbp->xIncr) > pcbp->xMax)
               return FALSE ;
          }
     return TRUE ;
     }

LRESULT CALLBACK WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
     {
     static HINSTANCE  hInst ;
     static int        cxChar, cyChar, cxClient, cyClient ;
     CBPARAM           cbparam ;
     HDC               hdc ;
     PAINTSTRUCT       ps ;
     TEXTMETRIC        tm ;

     switch (iMsg)
          {
          case WM_CREATE :
               hInst = ((LPCREATESTRUCT) lParam)->hInstance ;
               hdc   = GetDC (hwnd) ;
               GetTextMetrics (hdc, &tm) ;
               cxChar = (int) tm.tmAveCharWidth ;
               cyChar = (int) (tm.tmHeight + tm.tmExternalLeading) ;
               ReleaseDC (hwnd, hdc) ;

               return 0 ;

          case WM_COMMAND :
               switch (wParam)
                    {
                    case IDM_ENTER :
                         if (DialogBox (hInst, "EnterDlg", hwnd, &DlgProc))
                              {
                              if (AddString (szString))
                                   EnumWindows (&EnumCallBack, 0) ;
                              else
                                   MessageBeep (0) ;
                              }
                         break ;

                    case IDM_DELETE :
                         if (DialogBox (hInst, "DeleteDlg", hwnd, &DlgProc))
                              {
                              if (DeleteString (szString))
                                   EnumWindows (&EnumCallBack, 0) ;
                              else
                                   MessageBeep (0) ;
                              }
                         break ;
                    }
               return 0 ;

          case WM_SIZE :
               cxClient = (int) LOWORD (lParam) ;
               cyClient = (int) HIWORD (lParam) ;
               return 0 ;

          case WM_DATACHANGE :
               InvalidateRect (hwnd, NULL, TRUE) ;
               return 0 ;

          case WM_PAINT :
               hdc = BeginPaint (hwnd, &ps) ;

               cbparam.hdc   = hdc ;
               cbparam.xText = cbparam.xStart = cxChar ;
               cbparam.yText = cbparam.yStart = cyChar ;
               cbparam.xIncr = cxChar * MAXLEN ;
               cbparam.yIncr = cyChar ;
               cbparam.xMax  = cbparam.xIncr * (1 + cxClient / cbparam.xIncr) ;
               cbparam.yMax  = cyChar * (cyClient / cyChar - 1) ;

               GetStrings ((PSTRCB) GetStrCallBack, (PVOID) &cbparam) ;

               EndPaint (hwnd, &ps) ;
               return 0 ;

          case WM_DESTROY :
               PostQuitMessage (0) ;
               return 0 ;
          }
     return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
     }
