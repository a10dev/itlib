/*-----------------------------------------
   ANACLOCK.C -- Analog Clock Program
                 (c) Charles Petzold, 1996
  -----------------------------------------*/

#include <windows.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define ID_TIMER    1
#define TWOPI       (2 * 3.14159)

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
     {
     static char szAppName[] = "AnaClock" ;
     HWND        hwnd;
     MSG         msg;
     WNDCLASSEX  wndclass ;

	 wndclass.cbSize        = sizeof (wndclass) ;
     wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = NULL ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = szAppName ;
	 wndclass.hIconSm       = NULL ;

     RegisterClassEx (&wndclass) ;

     hwnd = CreateWindow (szAppName, "Analog Clock",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          NULL, NULL, hInstance, NULL) ;

     if (!SetTimer (hwnd, ID_TIMER, 1000, NULL))
          {
          MessageBox (hwnd, "Too many clocks or timers!", szAppName,
                      MB_ICONEXCLAMATION | MB_OK) ;
          return FALSE ;
          }

     ShowWindow (hwnd, iCmdShow) ;
     UpdateWindow (hwnd) ;

     while (GetMessage (&msg, NULL, 0, 0))
          {
          TranslateMessage (&msg) ;
          DispatchMessage (&msg) ;
          }
     return msg.wParam ;
     }

void SetIsotropic (HDC hdc, int cxClient, int cyClient)
     {
     SetMapMode (hdc, MM_ISOTROPIC) ;
     SetWindowExtEx (hdc, 1000, 1000, NULL) ;
     SetViewportExtEx (hdc, cxClient / 2, -cyClient / 2, NULL) ;
     SetViewportOrgEx (hdc, cxClient / 2,  cyClient / 2, NULL) ;
     }

void RotatePoint (POINT pt[], int iNum, int iAngle)
     {
     int   i ;
     POINT ptTemp ;

     for (i = 0 ; i < iNum ; i++)
          {
          ptTemp.x = (int) (pt[i].x * cos (TWOPI * iAngle / 360) +
                            pt[i].y * sin (TWOPI * iAngle / 360)) ;

          ptTemp.y = (int) (pt[i].y * cos (TWOPI * iAngle / 360) -
                            pt[i].x * sin (TWOPI * iAngle / 360)) ;

          pt[i] = ptTemp ;
          }
     }

void DrawClock (HDC hdc)
     {
     int   iAngle ;
     POINT pt[3] ;

     for (iAngle = 0 ; iAngle < 360 ; iAngle += 6)
          {
          pt[0].x =   0 ;
          pt[0].y = 900 ;

          RotatePoint (pt, 1, iAngle) ;

          pt[2].x = pt[2].y = iAngle % 5 ? 33 : 100 ;

          pt[0].x -= pt[2].x / 2 ;
          pt[0].y -= pt[2].y / 2 ;

          pt[1].x  = pt[0].x + pt[2].x ;
          pt[1].y  = pt[0].y + pt[2].y ;

          SelectObject (hdc, GetStockObject (BLACK_BRUSH)) ;

          Ellipse (hdc, pt[0].x, pt[0].y, pt[1].x, pt[1].y) ;
          }
     }

void DrawHands (HDC hdc, struct tm *datetime, BOOL bChange)
     {
     static POINT pt[3][5] = { 0, -150, 100, 0, 0, 600, -100, 0, 0, -150,
                               0, -200,  50, 0, 0, 800,  -50, 0, 0, -200,
                               0,    0,   0, 0, 0,   0,    0, 0, 0,  800 } ;
     int          i, iAngle[3] ;
     POINT        ptTemp[3][5] ;

     iAngle[0] = (datetime->tm_hour * 30) % 360 + datetime->tm_min / 2 ;
     iAngle[1] =  datetime->tm_min  *  6 ;
     iAngle[2] =  datetime->tm_sec  *  6 ;

     memcpy (ptTemp, pt, sizeof (pt)) ;

     for (i = bChange ? 0 : 2 ; i < 3 ; i++)
          {
          RotatePoint (ptTemp[i], 5, iAngle[i]) ;

          Polyline (hdc, ptTemp[i], 5) ;
          }
     }

LRESULT CALLBACK WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
     {
     static int         cxClient, cyClient ;
     static struct tm   dtPrevious ;
     BOOL               bChange ;
     HDC                hdc ;
     PAINTSTRUCT        ps ;
     time_t             lTime ;
     struct tm         *datetime ;

     switch (iMsg)
          {
          case WM_CREATE :
               time (&lTime) ;
               datetime = localtime (&lTime) ;

               dtPrevious = * datetime ;
               return 0 ;

          case WM_SIZE :
               cxClient = LOWORD (lParam) ;
               cyClient = HIWORD (lParam) ;
               return 0 ;

          case WM_TIMER :
               time (&lTime) ;
               datetime = localtime (&lTime) ;

               bChange = datetime->tm_hour != dtPrevious.tm_hour ||
                         datetime->tm_min  != dtPrevious.tm_min ;

               hdc = GetDC (hwnd) ;

               SetIsotropic (hdc, cxClient, cyClient) ;

               SelectObject (hdc, GetStockObject (WHITE_PEN)) ;
               DrawHands (hdc, &dtPrevious, bChange) ;

               SelectObject (hdc, GetStockObject (BLACK_PEN)) ;
               DrawHands (hdc, datetime, TRUE) ;

               ReleaseDC (hwnd, hdc) ;

               dtPrevious = *datetime ;
               return 0 ;

          case WM_PAINT :
               hdc = BeginPaint (hwnd, &ps) ;

               SetIsotropic (hdc, cxClient, cyClient) ;
               DrawClock    (hdc) ;
               DrawHands    (hdc, &dtPrevious, TRUE) ;

               EndPaint (hwnd, &ps) ;
               return 0 ;

          case WM_DESTROY :
               KillTimer (hwnd, ID_TIMER) ;
               PostQuitMessage (0) ;
               return 0 ;
          }
     return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
     }
