/*----------------------------------------
   HEXCALC.C -- Hexadecimal Calculator
                (c) Charles Petzold, 1996
  ----------------------------------------*/

#include <windows.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
     {
     static char  szAppName[] = "HexCalc" ;
     HWND         hwnd ;
     MSG          msg ;
     WNDCLASSEX   wndclass ;

     wndclass.cbSize        = sizeof (wndclass) ;
     wndclass.style         = CS_HREDRAW | CS_VREDRAW;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = DLGWINDOWEXTRA ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = LoadIcon (hInstance, szAppName) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1) ;
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = szAppName ;
     wndclass.hIconSm       = LoadIcon (hInstance, szAppName) ;

     RegisterClassEx (&wndclass) ;

     hwnd = CreateDialog (hInstance, szAppName, 0, NULL) ;

     ShowWindow (hwnd, iCmdShow) ;

     while (GetMessage (&msg, NULL, 0, 0))
          {
          TranslateMessage (&msg) ;
          DispatchMessage (&msg) ;
          }
     return msg.wParam ;
     }

void ShowNumber (HWND hwnd, UINT iNumber)
     {
     char szBuffer[20] ;

     SetDlgItemText (hwnd, VK_ESCAPE, strupr (ltoa (iNumber, szBuffer, 16))) ;
     }

DWORD CalcIt (UINT iFirstNum, int iOperation, UINT iNum)
     {
     switch (iOperation)
          {
          case '=' : return iNum ;
          case '+' : return iFirstNum +  iNum ;
          case '-' : return iFirstNum -  iNum ;
          case '*' : return iFirstNum *  iNum ;
          case '&' : return iFirstNum &  iNum ;
          case '|' : return iFirstNum |  iNum ;
          case '^' : return iFirstNum ^  iNum ;
          case '<' : return iFirstNum << iNum ;
          case '>' : return iFirstNum >> iNum ;
          case '/' : return iNum ? iFirstNum / iNum : UINT_MAX ;
          case '%' : return iNum ? iFirstNum % iNum : UINT_MAX ;
          default  : return 0 ;
          }
     }

LRESULT CALLBACK WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
     {
     static BOOL  bNewNumber = TRUE ;
     static int   iOperation = '=' ;
     static UINT  iNumber, iFirstNum ;
     HWND         hButton ;

     switch (iMsg)
          {
          case WM_KEYDOWN :                   // left arrow --> backspace
               if (wParam != VK_LEFT)
                    break ;
               wParam = VK_BACK ;
                                             // fall through
          case WM_CHAR :
               if ((wParam = toupper (wParam)) == VK_RETURN)
                    wParam = '=' ;

               hButton = GetDlgItem (hwnd, wParam) ;

               if (hButton != NULL)
                    {
                    SendMessage (hButton, BM_SETSTATE, 1, 0) ;
                    SendMessage (hButton, BM_SETSTATE, 0, 0) ;
                    }
               else
                    {
                    MessageBeep (0) ;
                    break ;
                    }
                                             // fall through
          case WM_COMMAND :
               SetFocus (hwnd) ;

               if (LOWORD (wParam) == VK_BACK)         // backspace
                    ShowNumber (hwnd, iNumber /= 16) ;

               else if (LOWORD (wParam) == VK_ESCAPE)  // escape
                    ShowNumber (hwnd, iNumber = 0) ;

               else if (isxdigit (LOWORD (wParam)))    // hex digit
                    {
                    if (bNewNumber)
                         {
                         iFirstNum = iNumber ;
                         iNumber = 0 ;
                         }
                    bNewNumber = FALSE ;

                    if (iNumber <= UINT_MAX >> 4)
                         ShowNumber (hwnd, iNumber = 16 * iNumber + wParam -
                              (isdigit (wParam) ? '0' : 'A' - 10)) ;
                    else
                         MessageBeep (0) ;
                    }
               else                                    // operation
                    {
                    if (!bNewNumber)
                         ShowNumber (hwnd, iNumber =
                              CalcIt (iFirstNum, iOperation, iNumber)) ;
                    bNewNumber = TRUE ;
                    iOperation = LOWORD (wParam) ;
                    }
               return 0 ;

          case WM_DESTROY :
               PostQuitMessage (0) ;
               return 0 ;
          }
     return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
     }
