/*---------------------------------------------
   DDEPOP1.C -- DDE Server for Population Data
                (c) Charles Petzold, 1996
  ---------------------------------------------*/

#include <windows.h>
#include <dde.h>
#include <string.h>
#include "ddepop.h"

typedef struct
     {
     unsigned int fAdvise:1 ;
     unsigned int fDeferUpd:1 ;
     unsigned int fAckReq:1 ;
     unsigned int dummy:13 ;
     long         lPopPrev ;
     }
     POPADVISE ;

#define ID_TIMER    1
#define DDE_TIMEOUT 3000

LRESULT CALLBACK WndProc         (HWND, UINT, WPARAM, LPARAM) ;
LRESULT CALLBACK ServerProc      (HWND, UINT, WPARAM, LPARAM) ;
BOOL    CALLBACK TimerEnumProc   (HWND, LONG) ;
BOOL    CALLBACK CloseEnumProc   (HWND, LONG) ;
BOOL             PostDataMessage (HWND, HWND, int, BOOL, BOOL, BOOL) ;

char      szAppName[]     = "DdePop1" ;
char      szServerClass[] = "DdePop1.Server" ;
HINSTANCE hInst ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
     {
     HWND        hwnd ;
     MSG         msg ;
     WNDCLASSEX  wndclass ;

     hInst = hInstance ;

               // Register window class

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

               // Register window class for DDE Server

     wndclass.cbSize        = sizeof (wndclass) ;
     wndclass.style         = 0 ;
     wndclass.lpfnWndProc   = ServerProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 2 * sizeof (DWORD) ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = NULL ;
     wndclass.hCursor       = NULL ;
     wndclass.hbrBackground = NULL ;
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = szServerClass ;
     wndclass.hIconSm       = NULL ;

     RegisterClassEx (&wndclass) ;

     hwnd = CreateWindow (szAppName, "DDE Population Server",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          NULL, NULL, hInstance, NULL) ;

     InitPops () ;                      // initialize 'pop' structure

     SetTimer (hwnd, ID_TIMER, 5000, NULL) ;

     ShowWindow (hwnd, SW_SHOWMINNOACTIVE) ;
     UpdateWindow (hwnd) ;

     while (GetMessage (&msg, NULL, 0, 0))
          {
          TranslateMessage (&msg) ;
          DispatchMessage (&msg) ;
          }

     KillTimer (hwnd, ID_TIMER) ;

     return msg.wParam ;
     }

LRESULT CALLBACK WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
     {
     static char szTopic[] = "US_Population" ;
     ATOM        aApp, aTop ;
     HWND        hwndClient, hwndServer ;

     switch (iMsg)
          {
          case WM_DDE_INITIATE :

                    // wParam          -- sending window handle
                    // LOWORD (lParam) -- application atom
                    // HIWORD (lParam) -- topic atom

               hwndClient = (HWND) wParam ;

               aApp = GlobalAddAtom (szAppName) ;
               aTop = GlobalAddAtom (szTopic) ;

                    // Check for matching atoms, create window, and acknowledge

               if ((LOWORD (lParam) == NULL || LOWORD (lParam) == aApp) &&
                   (HIWORD (lParam) == NULL || HIWORD (lParam) == aTop))
                    {
                    hwndServer = CreateWindow (szServerClass, NULL,
                                               WS_CHILD, 0, 0, 0, 0,
                                               hwnd, NULL, hInst, NULL) ;

                    SetWindowLong (hwndServer, 0, (LONG) hwndClient) ;
                    SendMessage ((HWND) wParam, WM_DDE_ACK,
                                 (WPARAM) hwndServer,
                                 MAKELPARAM (aApp, aTop)) ;
                    }

                    // Otherwise, delete the atoms just created

               else
                    {
                    GlobalDeleteAtom (aApp) ;
                    GlobalDeleteAtom (aTop) ;
                    }

               return 0 ;

          case WM_TIMER :
          case WM_TIMECHANGE :

                    // Calculate new current populations

               CalcPops () ;

                    // Notify all child windows

               EnumChildWindows (hwnd, &TimerEnumProc, 0L) ;
               return 0 ;

          case WM_QUERYOPEN :
               return 0 ;

          case WM_CLOSE :

                    // Notify all child windows

               EnumChildWindows (hwnd, &CloseEnumProc, 0L) ;

               break ;                  // for default processing

          case WM_DESTROY :
               PostQuitMessage (0) ;
               return 0 ;
          }
     return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
     }

LRESULT CALLBACK ServerProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
     {
     ATOM          aItem ;
     char          szItem[10] ;
     DDEACK        DdeAck ;
     DDEADVISE    *pDdeAdvise ;
     DWORD         dwTime ;
     GLOBALHANDLE  hPopAdvise, hDdeAdvise, hCommands, hDdePoke ;
     int           i ;
     UINT          uiLow, uiHi ;
     HWND          hwndClient ;
     MSG           msg ;
     POPADVISE    *pPopAdvise ;
     WORD          cfFormat, wStatus ;

     switch (iMsg)
          {
          case WM_CREATE :

                    // Allocate memory for POPADVISE structures

               hPopAdvise = GlobalAlloc (GHND, NUM_STATES * sizeof (POPADVISE));

               if (hPopAdvise == NULL)
                    DestroyWindow (hwnd) ;
               else
                    SetWindowLong (hwnd, 4, (LONG) hPopAdvise) ;

               return 0 ;

          case WM_DDE_REQUEST :

                    // wParam          -- sending window handle
                    // LOWORD (lParam) -- data format
                    // HIWORD (lParam) -- item atom

               hwndClient = (HWND) wParam ;
               cfFormat   = LOWORD (lParam) ;
               aItem      = HIWORD (lParam) ;

                    // Check for matching format and data item

               if (cfFormat == CF_TEXT)
                    {
                    GlobalGetAtomName (aItem, szItem, sizeof (szItem)) ;

                    for (i = 0 ; i < NUM_STATES ; i++)
                         if (strcmp (szItem, pop[i].szState) == 0)
                              break ;

                    if (i < NUM_STATES)
                         {
                         GlobalDeleteAtom (aItem) ;
                         PostDataMessage (hwnd, hwndClient, i,
                                          FALSE, FALSE, TRUE) ;
                         return 0 ;
                         }
                    }

                    // Negative acknowledge if no match

               DdeAck.bAppReturnCode = 0 ;
               DdeAck.reserved       = 0 ;
               DdeAck.fBusy          = FALSE ;
               DdeAck.fAck           = FALSE ;

               wStatus = *((WORD *) &DdeAck) ;

               if (!PostMessage (hwndClient, WM_DDE_ACK, (WPARAM) hwnd,
                                 PackDDElParam (WM_DDE_ACK, wStatus, aItem)))
                    {
                    GlobalDeleteAtom (aItem) ;
                    }

               return 0 ;

          case WM_DDE_ADVISE :

                    // wParam -- sending window handle
                    // lParam -- DDEADVISE memory handle & item atom

               UnpackDDElParam (WM_DDE_ADVISE, lParam, &uiLow, &uiHi) ;
               FreeDDElParam (WM_DDE_ADVISE, lParam) ;

               hwndClient = (HWND) wParam ;
               hDdeAdvise = (GLOBALHANDLE) uiLow ;
               aItem      = (ATOM) uiHi ;

               pDdeAdvise = (DDEADVISE *) GlobalLock (hDdeAdvise) ;

                    // Check for matching format and data item

               if (pDdeAdvise->cfFormat == CF_TEXT)
                    {
                    GlobalGetAtomName (aItem, szItem, sizeof (szItem)) ;

                    for (i = 0 ; i < NUM_STATES ; i++)
                         if (strcmp (szItem, pop[i].szState) == 0)
                              break ;

                         // Fill in the POPADVISE structure and acknowledge

                    if (i < NUM_STATES)
                         {
                         hPopAdvise = (GLOBALHANDLE) GetWindowLong (hwnd, 4) ;
                         pPopAdvise = (POPADVISE *)
                                              GlobalLock (hPopAdvise) ;

                         pPopAdvise[i].fAdvise   = TRUE ;
                         pPopAdvise[i].fDeferUpd = pDdeAdvise->fDeferUpd ;
                         pPopAdvise[i].fAckReq   = pDdeAdvise->fAckReq ;
                         pPopAdvise[i].lPopPrev  = pop[i].lPop ;

                         GlobalUnlock (hDdeAdvise) ;
                         GlobalFree (hDdeAdvise) ;

                         DdeAck.bAppReturnCode = 0 ;
                         DdeAck.reserved       = 0 ;
                         DdeAck.fBusy          = FALSE ;
                         DdeAck.fAck           = TRUE ;

                         wStatus = *((WORD *) &DdeAck) ;

                         if (!PostMessage (hwndClient, WM_DDE_ACK,
                                           (WPARAM) hwnd,
                                           PackDDElParam (WM_DDE_ACK,
                                                          wStatus, aItem)))
                              {
                              GlobalDeleteAtom (aItem) ;
                              }
                         else
                              {
                              PostDataMessage (hwnd, hwndClient, i,
                                               pPopAdvise[i].fDeferUpd,
                                               pPopAdvise[i].fAckReq,
                                               FALSE) ;
                              }

                         GlobalUnlock (hPopAdvise) ;
                         return 0 ;
                         }
                    }

                         // Otherwise, post a negative WM_DDE_ACK

               GlobalUnlock (hDdeAdvise) ;

               DdeAck.bAppReturnCode = 0 ;
               DdeAck.reserved       = 0 ;
               DdeAck.fBusy          = FALSE ;
               DdeAck.fAck           = FALSE ;

               wStatus = *((WORD *) &DdeAck) ;

               if (!PostMessage (hwndClient, WM_DDE_ACK, (WPARAM) hwnd,
                                 PackDDElParam (WM_DDE_ACK, wStatus, aItem)))
                    {
                    GlobalFree (hDdeAdvise) ;
                    GlobalDeleteAtom (aItem) ;
                    }

               return 0 ;

          case WM_DDE_UNADVISE :

                    // wParam          -- sending window handle
                    // LOWORD (lParam) -- data format
                    // HIWORD (lParam) -- item atom

               hwndClient = (HWND) wParam ;
               cfFormat   = LOWORD (lParam) ;
               aItem      = HIWORD (lParam) ;

               DdeAck.bAppReturnCode = 0 ;
               DdeAck.reserved       = 0 ;
               DdeAck.fBusy          = FALSE ;
               DdeAck.fAck           = TRUE ;

               hPopAdvise  = (GLOBALHANDLE) GetWindowLong (hwnd, 4) ;
               pPopAdvise = (POPADVISE *) GlobalLock (hPopAdvise) ;

                    // Check for matching format and data item

               if (cfFormat == CF_TEXT || cfFormat == NULL)
                    {
                    if (aItem == (ATOM) NULL)
                         for (i = 0 ; i < NUM_STATES ; i++)
                              pPopAdvise[i].fAdvise = FALSE ;
                    else
                         {
                         GlobalGetAtomName (aItem, szItem, sizeof (szItem)) ;

                         for (i = 0 ; i < NUM_STATES ; i++)
                              if (strcmp (szItem, pop[i].szState) == 0)
                                   break ;

                         if (i < NUM_STATES)
                              pPopAdvise[i].fAdvise = FALSE ;
                         else
                              DdeAck.fAck = FALSE ;
                         }
                    }
               else
                    DdeAck.fAck = FALSE ;

                    // Acknowledge either positively or negatively

               wStatus = *((WORD *) &DdeAck) ;

               if (!PostMessage (hwndClient, WM_DDE_ACK, (WPARAM) hwnd,
                                 PackDDElParam (WM_DDE_ACK, wStatus, aItem)))
                    {
                    if (aItem != (ATOM) NULL)
                         GlobalDeleteAtom (aItem) ;
                    }

               GlobalUnlock (hPopAdvise) ;
               return 0 ;

          case WM_DDE_EXECUTE :

                    // Post negative acknowledge

               hwndClient = (HWND) wParam ;
               hCommands  = (GLOBALHANDLE) lParam ;

               DdeAck.bAppReturnCode = 0 ;
               DdeAck.reserved       = 0 ;
               DdeAck.fBusy          = FALSE ;
               DdeAck.fAck           = FALSE ;

               wStatus = *((WORD *) &DdeAck) ;

               if (!PostMessage (hwndClient, WM_DDE_ACK, (WPARAM) hwnd,
                                 PackDDElParam (WM_DDE_ACK,
                                                wStatus, (UINT) hCommands)))
                    {
                    GlobalFree (hCommands) ;
                    }
               return 0 ;

          case WM_DDE_POKE :

                    // Post negative acknowledge

               UnpackDDElParam (WM_DDE_POKE, lParam, &uiLow, &uiHi) ;
               FreeDDElParam (WM_DDE_POKE, lParam) ;
               hwndClient = (HWND) wParam ;
               hDdePoke   = (GLOBALHANDLE) uiLow ;
               aItem      = (ATOM) uiHi ;

               DdeAck.bAppReturnCode = 0 ;
               DdeAck.reserved       = 0 ;
               DdeAck.fBusy          = FALSE ;
               DdeAck.fAck           = FALSE ;

               wStatus = *((WORD *) &DdeAck) ;

               if (!PostMessage (hwndClient, WM_DDE_ACK, (WPARAM) hwnd,
                                 PackDDElParam (WM_DDE_ACK, wStatus, aItem)))
                    {
                    GlobalFree (hDdePoke) ;
                    GlobalDeleteAtom (aItem) ;
                    }

               return 0 ;

          case WM_DDE_TERMINATE :

                    // Respond with another WM_DDE_TERMINATE iMsg

               hwndClient = (HWND) wParam ;
               PostMessage (hwndClient, WM_DDE_TERMINATE, (WPARAM) hwnd, 0L) ;
               DestroyWindow (hwnd) ;
               return 0 ;

          case WM_TIMER :

                    // Post WM_DDE_DATA iMsgs for changed populations

               hwndClient = (HWND) GetWindowLong (hwnd, 0) ;
               hPopAdvise = (GLOBALHANDLE) GetWindowLong (hwnd, 4) ;
               pPopAdvise = (POPADVISE *) GlobalLock (hPopAdvise) ;

               for (i = 0 ; i < NUM_STATES ; i++)
                    if (pPopAdvise[i].fAdvise)
                         if (pPopAdvise[i].lPopPrev != pop[i].lPop)
                              {
                              if (!PostDataMessage (hwnd, hwndClient, i,
                                                    pPopAdvise[i].fDeferUpd,
                                                    pPopAdvise[i].fAckReq,
                                                    FALSE))
                                   break ;

                              pPopAdvise[i].lPopPrev = pop[i].lPop ;
                              }

               GlobalUnlock (hPopAdvise) ;
               return 0 ;

          case WM_CLOSE :

                    // Post a WM_DDE_TERMINATE iMsg to the client

               hwndClient = (HWND) GetWindowLong (hwnd, 0) ;
               PostMessage (hwndClient, WM_DDE_TERMINATE, (WPARAM) hwnd, 0L) ;

               dwTime = GetCurrentTime () ;

               while (GetCurrentTime () - dwTime < DDE_TIMEOUT)
                    if (PeekMessage (&msg, hwnd, WM_DDE_TERMINATE,
                                     WM_DDE_TERMINATE, PM_REMOVE))
                         break ;

               DestroyWindow (hwnd) ;
               return 0 ;

          case WM_DESTROY :
               hPopAdvise = (GLOBALHANDLE) GetWindowLong (hwnd, 4) ;
               GlobalFree (hPopAdvise) ;
               return 0 ;
          }
     return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
     }

BOOL CALLBACK TimerEnumProc (HWND hwnd, LPARAM lParam)
     {
     SendMessage (hwnd, WM_TIMER, 0, 0L) ;

     return TRUE ;
     }

BOOL CALLBACK CloseEnumProc (HWND hwnd, LPARAM lParam)
     {
     SendMessage (hwnd, WM_CLOSE, 0, 0L) ;

     return TRUE ;
     }

BOOL PostDataMessage (HWND hwndServer, HWND hwndClient, int iState,
                      BOOL fDeferUpd, BOOL fAckReq, BOOL fResponse)
     {
     ATOM         aItem ;
     char         szPopulation[16] ;
     DDEACK       DdeAck ;
     DDEDATA     *pDdeData ;
     DWORD        dwTime ;
     GLOBALHANDLE hDdeData ;
     MSG          msg ;
     WORD         wStatus ;

     aItem = GlobalAddAtom (pop[iState].szState) ;

          // Allocate a DDEDATA structure if not deferred update

     if (fDeferUpd)
          {
          hDdeData = NULL ;
          }
     else
          {
          wsprintf (szPopulation, "%ld\r\n", pop[iState].lPop) ;

          hDdeData = GlobalAlloc (GHND | GMEM_DDESHARE,
                                  sizeof (DDEDATA) + strlen (szPopulation)) ;

          pDdeData = (DDEDATA *) GlobalLock (hDdeData) ;

          pDdeData->fResponse = fResponse ;
          pDdeData->fRelease  = TRUE ;
          pDdeData->fAckReq   = fAckReq ;
          pDdeData->cfFormat  = CF_TEXT ;

          lstrcpy ((PSTR) pDdeData->Value, szPopulation) ;

          GlobalUnlock (hDdeData) ;
          }

          // Post the WM_DDE_DATA iMsg

     if (!PostMessage (hwndClient, WM_DDE_DATA, (WPARAM) hwndServer,
                       PackDDElParam (WM_DDE_DATA, (UINT) hDdeData, aItem)))
          {
          if (hDdeData != NULL)
               GlobalFree (hDdeData) ;

          GlobalDeleteAtom (aItem) ;
          return FALSE ;
          }

          // Wait for the acknowledge iMsg if it's requested

     if (fAckReq)
          {
          DdeAck.fAck = FALSE ;

          dwTime = GetCurrentTime () ;

          while (GetCurrentTime () - dwTime < DDE_TIMEOUT)
               {
               if (PeekMessage (&msg, hwndServer, WM_DDE_ACK, WM_DDE_ACK,
                                PM_REMOVE))
                    {
                    wStatus = LOWORD (msg.lParam) ;
                    DdeAck = *((DDEACK *) &wStatus) ;
                    aItem  = HIWORD (msg.lParam) ;
                    GlobalDeleteAtom (aItem) ;
                    break ;
                    }
               }

          if (DdeAck.fAck == FALSE)
               {
               if (hDdeData != NULL)
                    GlobalFree (hDdeData) ;

               return FALSE ;
               }
          }

     return TRUE ;
     }
