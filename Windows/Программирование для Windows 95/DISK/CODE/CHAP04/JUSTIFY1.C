/*-----------------------------------------
   JUSTIFY1.C -- Justified Type Program
                 (c) Charles Petzold, 1996
  -----------------------------------------*/

#include <windows.h>
#include "ezfont.h"

#define LEFT         0
#define RIGHT        1
#define CENTER       2
#define JUSTIFIED    3

#define ALIGN        JUSTIFIED

#define MyCreateFont EzCreateFont (hdc, "Times New Roman", 150, 0, 0, TRUE)

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
     {
     static     char szAppName[] = "Justify1" ;
     HWND       hwnd ;
     MSG        msg ;
     WNDCLASSEX wndclass ;

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

     hwnd = CreateWindow (szAppName, "Justified Type",
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

void DrawRuler (HDC hdc, RECT *prc)
     {
     static int iRuleSize [16] = { 360, 72, 144, 72, 216, 72, 144, 72,
                                   288, 72, 144, 72, 216, 72, 144, 72 } ;
     int        i, j ;
     POINT      ptClient ;

     SaveDC (hdc) ;
                    // Set Logical Twips mapping mode

     SetMapMode (hdc, MM_ANISOTROPIC) ;
     SetWindowExtEx (hdc, 1440, 1440, NULL) ;
     SetViewportExtEx (hdc, GetDeviceCaps (hdc, LOGPIXELSX),
                            GetDeviceCaps (hdc, LOGPIXELSY), NULL) ;

                    // Move the origin to a half inch from upper left

     SetWindowOrgEx (hdc, -720, -720, NULL) ;

                    // Find the right margin (quarter inch from right)

     ptClient.x = prc->right ;
     ptClient.y = prc->bottom ;
     DPtoLP (hdc, &ptClient, 1) ;
     ptClient.x -= 360 ;

                    // Draw the rulers

     MoveToEx (hdc, 0,          -360, NULL) ;
     LineTo   (hdc, ptClient.x, -360) ;
     MoveToEx (hdc, -360,          0, NULL) ;
     LineTo   (hdc, -360, ptClient.y) ;

     for (i = 0, j = 0 ; i <= ptClient.x ; i += 1440 / 16, j++)
          {
          MoveToEx (hdc, i, -360, NULL) ;
          LineTo   (hdc, i, -360 - iRuleSize [j % 16]) ;
          }

     for (i = 0, j = 0 ; i <= ptClient.y ; i += 1440 / 16, j++)
          {
          MoveToEx (hdc, -360, i, NULL) ;
          LineTo   (hdc, -360 - iRuleSize [j % 16], i) ;
          }

     RestoreDC (hdc, -1) ;
     }

void Justify (HDC hdc, PSTR pText, RECT *prc, int iAlign)
     {
     int  xStart, yStart, iBreakCount ;
     PSTR pBegin, pEnd ;
     SIZE size ;

     yStart = prc->top ;
     do                            // for each text line
          {
          iBreakCount = 0 ;
          while (*pText == ' ')    // skip over leading blanks
               pText++ ;
          pBegin = pText ;

          do                       // until the line is known
               {
               pEnd = pText ;

               while (*pText != '\0' && *pText++ != ' ') ;
               if (*pText == '\0')
                    break ;
                                   // for each space, calculate extents
               iBreakCount++ ;
               SetTextJustification (hdc, 0, 0) ;
               GetTextExtentPoint32 (hdc, pBegin, pText - pBegin - 1, &size) ;
               }
          while ((int) size.cx < (prc->right - prc->left)) ;

          iBreakCount-- ;
          while (*(pEnd - 1) == ' ')   // eliminate trailing blanks
               {
               pEnd-- ;
               iBreakCount-- ;
               }

          if (*pText == '\0' || iBreakCount <= 0)
               pEnd = pText ;

          SetTextJustification (hdc, 0, 0) ;
          GetTextExtentPoint32 (hdc, pBegin, pEnd - pBegin, &size) ;


          switch (iAlign)               // use alignment for xStart
               {
               case LEFT:
                    xStart = prc->left ;
                    break ;

               case RIGHT:
                    xStart = prc->right - size.cx ;
                    break ;

               case CENTER:
                    xStart = (prc->right + prc->left - size.cx) / 2 ;
                    break ;

               case JUSTIFIED:
                    if (*pText != '\0' && iBreakCount > 0)
                         SetTextJustification (hdc,
                              prc->right - prc->left - size.cx,
                              iBreakCount) ;
                    xStart = prc->left ;
                    break ;
               }

          TextOut (hdc, xStart, yStart, pBegin, pEnd - pBegin) ;
          yStart += size.cy ;
          pText = pEnd ;
          }
     while (*pText && yStart < prc->bottom) ;

     }

LRESULT CALLBACK WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
     {
     static char szText[] = "Call me Ishmael. Some years ago -- never mind "
                            "how long precisely -- having little or no money "
                            "in my purse, and nothing particular to interest "
                            "me on shore, I thought I would sail about a "
                            "little and see the watery part of the world. It "
                            "is a way I have of driving off the spleen, and "
                            "regulating the circulation.  Whenever I find "
                            "myself growing grim about the mouth; whenever "
                            "it is a damp, drizzly November in my soul; "
                            "whenever I find myself involuntarily pausing "
                            "before coffin warehouses, and bringing up the "
                            "rear of every funeral I meet; and especially "
                            "whenever my hypos get such an upper hand of me, "
                            "that it requires a strong moral principle to "
                            "prevent me from deliberately stepping into the "
                            "street, and methodically knocking people's hats "
                            "off -- then, I account it high time to get to sea "
                            "as soon as I can. This is my substitute for "
                            "pistol and ball.  With a philosophical flourish "
                            "Cato throws himself upon his sword; I quietly "
                            "take to the ship. There is nothing surprising "
                            "in this. If they but knew it, almost all men in "
                            "their degree, some time or other, cherish very "
                            "nearly the same feelings towards the ocean with "
                            "me." ;

     HDC         hdc ;
     PAINTSTRUCT ps ;
     RECT        rcClient ;

     switch (iMsg)
          {
          case WM_PAINT:
               hdc = BeginPaint (hwnd, &ps) ;

               GetClientRect (hwnd, &rcClient) ;
               DrawRuler (hdc, &rcClient) ;

               rcClient.left  += GetDeviceCaps (hdc, LOGPIXELSX) / 2 ;
               rcClient.top   += GetDeviceCaps (hdc, LOGPIXELSY) / 2 ;
               rcClient.right -= GetDeviceCaps (hdc, LOGPIXELSX) / 4 ;

               SelectObject (hdc, MyCreateFont) ;

               Justify (hdc, szText, &rcClient, ALIGN) ;

               DeleteObject (SelectObject (hdc, GetStockObject (SYSTEM_FONT)));
               EndPaint (hwnd, &ps) ;
               return 0 ;

          case WM_DESTROY:
               PostQuitMessage (0) ;
               return 0 ;
          }
     return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
     }
