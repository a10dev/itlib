/*---------------------------------------------
   CALLER.C -- Call into private OLE component
               (c) Paul Yao, 1996
  ---------------------------------------------*/
#include <windows.h>
#include "caller.h"
#include "imalloc.h"

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

char szWndClass[] = "CallerWindow" ;
char szAppName[]  = "Malloc Caller" ;

//-------------------------------------------------------------------
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                    PSTR lpszCmdLine, int cmdShow)       
     {
     HWND        hwnd ;
     MSG         msg ;
     WNDCLASSEX  wc ;

	 wc.cbSize        = sizeof (wc) ;
     wc.lpszClassName = szWndClass ;
     wc.hInstance     = hInstance ;
     wc.lpfnWndProc   = WndProc ;
     wc.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wc.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
     wc.lpszMenuName  = "MAIN" ;
     wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1) ;
     wc.style         = 0 ;
     wc.cbClsExtra    = 0 ;
     wc.cbWndExtra    = 0 ;
	 wc.hIconSm       = LoadIcon (NULL, IDI_APPLICATION) ;

     RegisterClassEx (&wc) ;

     hwnd = CreateWindowEx (0L, szWndClass, szAppName,
                            WS_OVERLAPPEDWINDOW,  
                            CW_USEDEFAULT, CW_USEDEFAULT,
                            CW_USEDEFAULT, CW_USEDEFAULT,
                            NULL, NULL, hInstance, NULL) ;

     ShowWindow (hwnd, cmdShow) ;
     UpdateWindow (hwnd) ;

     while (GetMessage (&msg, NULL, 0, 0))
          {
          TranslateMessage (&msg) ; 
          DispatchMessage (&msg) ;  
          }
     return msg.wParam ;
     }

//-------------------------------------------------------------------
LRESULT CALLBACK 
WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) 
     {
     static int       iCurLine = 0 ;
     static LPMALLOC  pMalloc = NULL ;
     static LPSTR     szLine[10] ;
     static RECT      rHit[10] ;

     switch (iMsg)
          {
          case WM_CREATE :
               // Initialize data pointer array
               ZeroMemory (szLine, sizeof (szLine)) ;
               return 0 ;

          case WM_COMMAND :
               switch (LOWORD (wParam))
                    {
                    case IDM_CREATE :
                         pMalloc = CreateAllocator () ;
                         if (pMalloc == NULL)
                              {
                              MessageBox (hwnd, "Error: No allocator", 
                                          szAppName, MB_OK) ;
                              return 0 ;
                              }

                         InvalidateRect (hwnd, NULL, TRUE) ;
                         return 0 ;

                    case IDM_DESTROY :
                         {
                         int i ;

                         // Mark allocated blocks as invalid
                         for (i = 0 ; i < 10 ; i++)
                              {
                              if ((szLine[i] != NULL) &&
                                  (pMalloc->lpVtbl->DidAlloc (pMalloc, 
                                                              szLine[i])))
                                   {
                                   szLine[i] = NULL ;
                                   }
                              }

                         // Disconnect from & free allocator
                         pMalloc->lpVtbl->Release (pMalloc) ;
                         pMalloc = NULL ;

                         InvalidateRect (hwnd, NULL, TRUE) ;
                         return 0 ;
                         }

                    case IDM_IUNKNOWN :
                         {
                         LPUNKNOWN pUnk ;
                         HRESULT hr =
                              pMalloc->lpVtbl->QueryInterface (pMalloc, 
                                                               IID_IUnknown,
                                                               (void **) &pUnk) ;
                         if (SUCCEEDED (hr))
                              {
                              pUnk->lpVtbl->Release (pUnk) ;
                              MessageBox (hwnd, "IUnknown supported", 
                                          szAppName, MB_OK) ;
                              }
                         else
                              {
                              MessageBox (hwnd, "IUnknown not supported", 
                                          szAppName, MB_OK) ;
                              }
                         return 0 ;
                         }

                    case IDM_IMALLOC :
                         {
                         LPUNKNOWN pUnk ;
                         HRESULT hr =
                              pMalloc->lpVtbl->QueryInterface (pMalloc, 
                                                               IID_IMalloc,
                                                               (void **) &pUnk) ;
                         if (SUCCEEDED (hr))
                              {
                              pUnk->lpVtbl->Release (pUnk) ;
                              MessageBox (hwnd, "IMalloc supported", 
                                          szAppName, MB_OK) ;
                              }
                         else
                              {
                              MessageBox (hwnd, "IMalloc not supported", 
                                          szAppName, MB_OK) ;
                              }
                         return 0 ;
                         }

                    case IDM_IMARSHAL :
                         {
                         LPUNKNOWN pUnk ;
                         HRESULT hr =
                              pMalloc->lpVtbl->QueryInterface (pMalloc, 
                                                               IID_IMarshal,
                                                               (void **) &pUnk) ;
                         if (SUCCEEDED (hr))
                              {
                              pUnk->lpVtbl->Release (pUnk) ;
                              MessageBox (hwnd, "IMarshal supported",
                                          szAppName, MB_OK) ;
                              }
                         else
                              {
                              MessageBox (hwnd, "IMarshal not supported",
                                          szAppName, MB_OK) ;
                              }
                         return 0 ;
                         }

                    case IDM_ALLOCATE_CUSTOM :
                         if (szLine[iCurLine] != NULL)
                              {
                              MessageBox (hwnd, "Error: Free First", 
                                          szAppName, MB_OK) ;
                              return 0 ;
                              }

                         // Allocate from IAllocate interface
                         szLine[iCurLine] = 
                              (char *) pMalloc->lpVtbl->Alloc (pMalloc, 100) ;
                         lstrcpy (szLine[iCurLine], "*IMalloc memory*") ;

                         InvalidateRect (hwnd, NULL, TRUE) ;
                         return 0 ;

                    case IDM_ALLOCATE_DEFAULT :
                         if (szLine[iCurLine] != NULL)
                              {
                              MessageBox (hwnd, "Error: Free First", 
                                          szAppName, MB_OK) ;
                              return 0 ;
                              }

                         // Allocate from default heap
                         szLine[iCurLine] = (char *) malloc (100) ;
                         lstrcpy (szLine[iCurLine], "-Malloc memory-") ;

                         InvalidateRect (hwnd, NULL, TRUE) ;
                         return 0 ;

                    case IDM_FREE :
                         if (szLine[iCurLine] == NULL)
                              {
                              MessageBox (hwnd, "Error: Nothing to free", 
                                          szAppName, MB_OK) ;
                              return 0 ;
                              }

                         if (pMalloc == NULL)
                              {
                              goto FreeMalloc ;
                              }

                         // Free allocated object
                         if (pMalloc->lpVtbl->DidAlloc (pMalloc, 
                                                        szLine[iCurLine]))
                              {
                              pMalloc->lpVtbl->Free (pMalloc, 
                                                     szLine[iCurLine]) ;
                              }
                         else
                              {
               FreeMalloc:
                              free (szLine[iCurLine]) ;
                              }

                         szLine[iCurLine] = NULL ;

                         InvalidateRect (hwnd, NULL, TRUE) ;
                         return 0 ;
                         }

          case WM_DESTROY :
               // Disconnect from & free allocator
               if (pMalloc)
                    {
                    pMalloc->lpVtbl->Release (pMalloc) ;
                    pMalloc = NULL ;
                    }

               PostQuitMessage (0) ;  // Handle application shutdown
               return 0 ;

          case WM_INITMENU :
               {
               HMENU hMenu = (HMENU) wParam ;
               if (pMalloc)
                    {
                    EnableMenuItem (hMenu, IDM_CREATE,          MF_GRAYED) ;
                    EnableMenuItem (hMenu, IDM_DESTROY,         MF_ENABLED) ;
                    EnableMenuItem (hMenu, IDM_ALLOCATE_CUSTOM, MF_ENABLED) ;
                    EnableMenuItem (hMenu, IDM_IUNKNOWN,        MF_ENABLED) ;
                    EnableMenuItem (hMenu, IDM_IMALLOC,         MF_ENABLED) ;
                    EnableMenuItem (hMenu, IDM_IMARSHAL,        MF_ENABLED) ;
                    }
               else
                    {
                    EnableMenuItem (hMenu, IDM_CREATE,          MF_ENABLED) ;
                    EnableMenuItem (hMenu, IDM_DESTROY,         MF_GRAYED) ;
                    EnableMenuItem (hMenu, IDM_ALLOCATE_CUSTOM, MF_GRAYED) ;
                    EnableMenuItem (hMenu, IDM_IUNKNOWN,        MF_GRAYED) ;
                    EnableMenuItem (hMenu, IDM_IMALLOC,         MF_GRAYED) ;
                    EnableMenuItem (hMenu, IDM_IMARSHAL,        MF_GRAYED) ;
                    }
               return 0 ;
               }

          case WM_LBUTTONDOWN :
               {
               int i ;
               int x = LOWORD (lParam) ;
               int y = HIWORD (lParam) ;
               POINT pt = { x, y } ;

               for (i = 0 ; i < 10 ; i++)
                    {
                    if (PtInRect (&rHit[i], pt))
                         {
                         if (iCurLine != i)  // Minimize screen blink
                              {
                              InvalidateRect (hwnd, &rHit[iCurLine], TRUE) ;
                              InvalidateRect (hwnd, &rHit[i], TRUE) ;
                              iCurLine = i ;
                              }
                         break ;
                         }    
                    }
                    return 0 ;
               }

          case WM_PAINT :
               {
               char         szBuff[10] ;
               COLORREF     crText, crBack ;
               HDC          hdc ;
               int          cc ;
               int          i ;
               int          XCount, XText, Y ;
               int          cyLineHeight ;
               PAINTSTRUCT  ps ;
               RECT         rOpaque ;
               TEXTMETRIC   tm ;

               hdc = BeginPaint (hwnd, &ps) ;

               // Fetch line height
               GetTextMetrics (ps.hdc, &tm) ;
               cyLineHeight = tm.tmHeight + tm.tmExternalLeading ;

               // Fetch current text colors
               crText = GetTextColor (ps.hdc) ;
               crBack = GetBkColor (ps.hdc) ;

               XCount = tm.tmAveCharWidth * 3 ;
               XText  = XCount + tm.tmAveCharWidth * 7 ;
               Y      = tm.tmHeight ;

               for (i = 0 ; i < 10 ; i++, Y += cyLineHeight)
                    {
                    // Set colors to highlight current line
                    if (i == iCurLine)
                         {
                         SetTextColor (ps.hdc, crBack) ;
                         SetBkColor (ps.hdc, crText) ;

                         SetRect (&rOpaque, 0, Y, 9999, Y + cyLineHeight) ;
                         ExtTextOut (ps.hdc, 0, 0, ETO_OPAQUE, &rOpaque,
                                     NULL, 0, NULL ) ;
                         }
                    else
                         {
                         SetTextColor (ps.hdc, crText) ;
                         SetBkColor (ps.hdc, crBack) ;
                         }

                    // Display line count
                    cc = wsprintf (szBuff, "Line %d", i) ;
                    TextOut (ps.hdc, XCount, Y, szBuff, cc) ;

                    // Display text if a string has been defined
                    if (szLine[i] != NULL)
                         {
                         cc = lstrlen (szLine[i]) ;
                         TextOut (ps.hdc, XText, Y, szLine[i], cc) ;
                         }

                    // Calculate hit test rectangle
                    SetRect (&rHit[i], 0, Y, 9999, Y + cyLineHeight) ;
                    }

               EndPaint (hwnd, &ps) ;
               return 0 ;
               }
          default :
               return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
          }
     }
