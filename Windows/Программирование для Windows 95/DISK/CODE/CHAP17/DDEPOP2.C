/*-----------------------------------------------
   DDEPOP2.C -- DDEML Server for Population Data
                (c) Charles Petzold, 1996
  -----------------------------------------------*/

#include <windows.h>
#include <ddeml.h>
#include <string.h>
#include "ddepop.h"

#define WM_USER_INITIATE (WM_USER + 1)
#define ID_TIMER         1

LRESULT  CALLBACK WndProc     (HWND, UINT, WPARAM, LPARAM) ;
HDDEDATA CALLBACK DdeCallback (UINT, UINT, HCONV, HSZ, HSZ,
                                        HDDEDATA, DWORD, DWORD) ;

char      szAppName[] = "DdePop2" ;
char      szTopic[]   = "US_Population" ;
DWORD     idInst ;
HINSTANCE hInst ;
HWND      hwnd ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
     {
     MSG        msg ;
     WNDCLASSEX wndclass ;

     wndclass.cbSize        = sizeof (wndclass) ;
     wndclass.style         = 0 ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = LoadIcon (hInstance, szAppName) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = szAppName ;
     wndclass.hIconSm       = LoadIcon (hInstance, szAppName) ;

     RegisterClassEx (&wndclass) ;

     hwnd = CreateWindow (szAppName, "DDEML Population Server",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          NULL, NULL, hInstance, NULL) ;

     ShowWindow (hwnd, SW_SHOWMINNOACTIVE) ;
     UpdateWindow (hwnd) ;

               // Initialize for using DDEML

     if (DdeInitialize (&idInst, (PFNCALLBACK) &DdeCallback,
                        CBF_FAIL_EXECUTES | CBF_FAIL_POKES |
                        CBF_SKIP_REGISTRATIONS | CBF_SKIP_UNREGISTRATIONS, 0))
          {
          MessageBox (hwnd, "Could not initialize server!",
                      szAppName, MB_ICONEXCLAMATION | MB_OK) ;

          DestroyWindow (hwnd) ;
          return FALSE ;
          }

                // Set the timer

     SetTimer (hwnd, ID_TIMER, 5000, NULL) ;

               // Start things going

     SendMessage (hwnd, WM_USER_INITIATE, 0, 0L) ;

     while (GetMessage (&msg, NULL, 0, 0))
          {
          TranslateMessage (&msg) ;
          DispatchMessage (&msg) ;
          }

               // Clean up

     DdeUninitialize (idInst) ;
     KillTimer (hwnd, ID_TIMER) ;

     return msg.wParam ;
     }

int GetStateNumber (UINT iFmt, HSZ hszItem)
     {
     char szItem[32] ;
     int  i ;

     if (iFmt != CF_TEXT)
          return -1 ;

     DdeQueryString (idInst, hszItem, szItem, sizeof (szItem), 0) ;

     for (i = 0 ; i < NUM_STATES ; i++)
          if (strcmp (szItem, pop[i].szState) == 0)
               break ;

     if (i >= NUM_STATES)
          return -1 ;

     return i ;
     }

HDDEDATA CALLBACK DdeCallback (UINT iType, UINT iFmt, HCONV hConv,
                               HSZ hsz1, HSZ hsz2, HDDEDATA hData,
                               DWORD dwData1, DWORD dwData2)
     {
     char szBuffer[32] ;
     int  i ;

     switch (iType)
          {
          case XTYP_CONNECT :            // hsz1 = topic
                                         // hsz2 = service

               DdeQueryString (idInst, hsz2, szBuffer, sizeof (szBuffer), 0) ;

               if (0 != strcmp (szBuffer, szAppName))
                    return FALSE ;

               DdeQueryString (idInst, hsz1, szBuffer, sizeof (szBuffer), 0) ;

               if (0 != strcmp (szBuffer, szTopic))
                    return FALSE ;

               return (HDDEDATA) TRUE ;

          case XTYP_ADVSTART :           // hsz1 = topic
                                         // hsz2 = item

                    // Check for matching format and data item

               if (-1 == (i = GetStateNumber (iFmt, hsz2)))
                    return FALSE ;

               pop[i].lPopLast = 0 ;
               PostMessage (hwnd, WM_TIMER, 0, 0L) ;

               return (HDDEDATA) TRUE ;

          case XTYP_REQUEST :
          case XTYP_ADVREQ :             // hsz1 = topic
                                         // hsz2 = item

                    // Check for matching format and data item

               if (-1 == (i = GetStateNumber (iFmt, hsz2)))
                    return NULL ;

               wsprintf (szBuffer, "%ld\r\n", pop[i].lPop) ;

               return DdeCreateDataHandle (idInst, (unsigned char *) szBuffer,
                                           strlen (szBuffer) + 1,
                                           0, hsz2, CF_TEXT, 0) ;

          case XTYP_ADVSTOP :            // hsz1 = topic
                                         // hsz2 = item
  
                    // Check for matching format and data item

               if (-1 == (i = GetStateNumber (iFmt, hsz2)))
                    return FALSE ;

               return (HDDEDATA) TRUE ;
          }

     return NULL ;
     }

LRESULT CALLBACK WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
     {
     static HSZ hszService, hszTopic ;
     HSZ        hszItem ;
     int        i ;

     switch (iMsg)
          {
          case WM_USER_INITIATE :
               InitPops () ;

               hszService = DdeCreateStringHandle (idInst, szAppName, 0) ;
               hszTopic   = DdeCreateStringHandle (idInst, szTopic,   0) ;

               DdeNameService (idInst, hszService, NULL, DNS_REGISTER) ;
               return 0 ;

          case WM_TIMER :
          case WM_TIMECHANGE :

                    // Calculate new current populations

               CalcPops () ;

               for (i = 0 ; i < NUM_STATES ; i++)
                    if (pop[i].lPop != pop[i].lPopLast)
                         {
                         hszItem = DdeCreateStringHandle (idInst,
                                                          pop[i].szState, 0) ;

                         DdePostAdvise (idInst, hszTopic, hszItem) ;

                         DdeFreeStringHandle (idInst, hszItem) ;

                         pop[i].lPopLast = pop[i].lPop ;
                         }

               return 0 ;

          case WM_QUERYOPEN :
               return 0 ;

          case WM_DESTROY :
               DdeNameService (idInst, hszService, NULL, DNS_UNREGISTER) ;
               DdeFreeStringHandle (idInst, hszService) ;
               DdeFreeStringHandle (idInst, hszTopic) ;

               PostQuitMessage (0) ;
               return 0 ;
          }
     return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
     }
