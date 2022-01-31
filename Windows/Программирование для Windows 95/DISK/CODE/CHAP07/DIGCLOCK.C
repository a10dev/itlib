/*-----------------------------------------
   DIGCLOCK.C -- Digital Clock Program
                 (c) Charles Petzold, 1996
  -----------------------------------------*/

#include <windows.h>
#include <time.h>

#define ID_TIMER    1

#define YEAR  (datetime->tm_year % 100)
#define MONTH (datetime->tm_mon  + 1)
#define MDAY  (datetime->tm_mday)
#define WDAY  (datetime->tm_wday)
#define HOUR  (datetime->tm_hour)
#define MIN   (datetime->tm_min)
#define SEC   (datetime->tm_sec)

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;
void SizeTheWindow (int *, int *, int *, int *) ;

char  sDate[2], sTime[2], sAMPM[2][5] ;
int   iDate, iTime ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
     {
     static char szAppName[] = "DigClock" ;
     HWND        hwnd ;
     MSG         msg ;
     int         xStart, yStart, xClient, yClient ;
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

     SizeTheWindow (&xStart, &yStart, &xClient, &yClient) ;

     hwnd = CreateWindow (szAppName, szAppName,
                          WS_POPUP | WS_DLGFRAME | WS_SYSMENU,
                          xStart,  yStart,
                          xClient, yClient,
                          NULL, NULL, hInstance, NULL) ;

     if (!SetTimer (hwnd, ID_TIMER, 1000, NULL))
          {
          MessageBox (hwnd, "Too many clocks or timers!", szAppName,
                      MB_ICONEXCLAMATION | MB_OK) ;
          return FALSE ;
          }

     ShowWindow (hwnd, SW_SHOWNOACTIVATE) ; 
     UpdateWindow (hwnd) ;

     while (GetMessage (&msg, NULL, 0, 0))
          {
          TranslateMessage (&msg) ;
          DispatchMessage (&msg) ;
          }
     return msg.wParam ;
     }

void SizeTheWindow (int *pxStart,  int *pyStart,
                    int *pxClient, int *pyClient)
     {
     HDC        hdc ;
     TEXTMETRIC tm ;

     hdc = CreateIC ("DISPLAY", NULL, NULL, NULL) ;
     GetTextMetrics (hdc, &tm) ;
     DeleteDC (hdc) ;

     *pxClient = 2 * GetSystemMetrics (SM_CXDLGFRAME) + 16*tm.tmAveCharWidth ;
     *pxStart  =     GetSystemMetrics (SM_CXSCREEN)   - *pxClient ;
     *pyClient = 2 * GetSystemMetrics (SM_CYDLGFRAME) + 2*tm.tmHeight ;
     *pyStart  =     0 ;
     }

void SetInternational (void)
     {
     static char cName [] = "intl" ;

     iDate = GetProfileInt (cName, "iDate", 0) ;
     iTime = GetProfileInt (cName, "iTime", 0) ;

     GetProfileString (cName, "sDate",  "/", sDate,    2) ;
     GetProfileString (cName, "sTime",  ":", sTime,    2) ;
     GetProfileString (cName, "s1159", "AM", sAMPM[0], 5) ;
     GetProfileString (cName, "s2359", "PM", sAMPM[1], 5) ;
     }

void WndPaint (HWND hwnd, HDC hdc)
     {
     static char szWday[] = "Sun\0Mon\0Tue\0Wed\0Thu\0Fri\0Sat" ;
     char        cBuffer[40] ;
     int         iLength ;
     RECT        rect ;
     struct tm  *datetime ;
     time_t      lTime ;

     time (&lTime) ;
     datetime = localtime (&lTime) ;
     
     iLength = wsprintf (cBuffer, "  %s  %d%s%02d%s%02d  \r\n",
               (PSTR) szWday + 4 * WDAY,
               iDate == 1 ? MDAY  : iDate == 2 ? YEAR  : MONTH, (PSTR) sDate,
               iDate == 1 ? MONTH : iDate == 2 ? MONTH : MDAY,  (PSTR) sDate,
               iDate == 1 ? YEAR  : iDate == 2 ? MDAY  : YEAR) ;

     if (iTime == 1)
          iLength += wsprintf (cBuffer + iLength, "  %02d%s%02d%s%02d  ",
                               HOUR, (PSTR) sTime, MIN, (PSTR) sTime, SEC) ;
     else
          iLength += wsprintf (cBuffer + iLength, "  %d%s%02d%s%02d %s  ",
                               (HOUR % 12) ? (HOUR % 12) : 12,
                               (PSTR) sTime, MIN, (PSTR) sTime, SEC,
                               (PSTR) sAMPM [HOUR / 12]) ;

     GetClientRect (hwnd, &rect) ;
     DrawText (hdc, cBuffer, -1, &rect, DT_CENTER | DT_NOCLIP) ;
     }

LRESULT CALLBACK WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
     {
     HDC         hdc ;
     PAINTSTRUCT ps ;

     switch (iMsg)
          {
          case WM_CREATE :
               SetInternational () ;
               return 0 ;

          case WM_TIMER :
               InvalidateRect (hwnd, NULL, FALSE) ;
               return 0 ;

          case WM_PAINT :
               hdc = BeginPaint (hwnd, &ps) ;
               WndPaint (hwnd, hdc) ;
               EndPaint (hwnd, &ps) ;
               return 0 ;

          case WM_WININICHANGE :
               SetInternational () ;
               InvalidateRect (hwnd, NULL, TRUE) ;
               return 0 ;

          case WM_DESTROY :
               KillTimer (hwnd, ID_TIMER) ;
               PostQuitMessage (0) ;
               return 0 ;
          }
     return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
     }
