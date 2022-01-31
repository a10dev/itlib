/*--------------------------------------------------------
   MDIDEMO.C -- Multiple Document Interface Demonstration
                (c) Charles Petzold, 1996
  --------------------------------------------------------*/

#include <windows.h>
#include <stdlib.h>
#include "mdidemo.h"

LRESULT CALLBACK FrameWndProc  (HWND, UINT, WPARAM, LPARAM) ;
BOOL    CALLBACK CloseEnumProc (HWND, LPARAM) ;
LRESULT CALLBACK HelloWndProc  (HWND, UINT, WPARAM, LPARAM) ;
LRESULT CALLBACK RectWndProc   (HWND, UINT, WPARAM, LPARAM) ;

          // structure for storing data unique to each Hello child window

typedef struct tagHELLODATA
     {
     UINT     iColor ;
     COLORREF clrText ;
     }
     HELLODATA, *LPHELLODATA ;

          // structure for storing data unique to each Rect child window

typedef struct tagRECTDATA
     {
     short cxClient ;
     short cyClient ;
     }
     RECTDATA, *LPRECTDATA ;

          // global variables

char      szFrameClass[] = "MdiFrame" ;
char      szHelloClass[] = "MdiHelloChild" ;
char      szRectClass[]  = "MdiRectChild" ;
HINSTANCE hInst ;
HMENU     hMenuInit, hMenuHello, hMenuRect ;
HMENU     hMenuInitWindow, hMenuHelloWindow, hMenuRectWindow ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
     {
     HACCEL      hAccel ;
     HWND        hwndFrame, hwndClient ;
     MSG         msg ;
     WNDCLASSEX  wndclass ;

     hInst = hInstance ;

     if (!hPrevInstance) 
          {
                    // Register the frame window class

          wndclass.cbSize        = sizeof (wndclass) ;
          wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
          wndclass.lpfnWndProc   = FrameWndProc ;
          wndclass.cbClsExtra    = 0 ;
          wndclass.cbWndExtra    = 0 ;
          wndclass.hInstance     = hInstance ;
          wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
          wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
          wndclass.hbrBackground = (HBRUSH) (COLOR_APPWORKSPACE + 1) ;
          wndclass.lpszMenuName  = NULL ;
          wndclass.lpszClassName = szFrameClass ;
          wndclass.hIconSm       = LoadIcon (NULL, IDI_APPLICATION) ;

          RegisterClassEx (&wndclass) ;

                    // Register the Hello child window class

          wndclass.cbSize        = sizeof (wndclass) ;
          wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
          wndclass.lpfnWndProc   = HelloWndProc ;
          wndclass.cbClsExtra    = 0 ;
          wndclass.cbWndExtra    = sizeof (HANDLE) ;
          wndclass.hInstance     = hInstance ;
          wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
          wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
          wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
          wndclass.lpszMenuName  = NULL ;
          wndclass.lpszClassName = szHelloClass ;
          wndclass.hIconSm       = LoadIcon (NULL, IDI_APPLICATION) ;

          RegisterClassEx (&wndclass) ;

                    // Register the Rect child window class

          wndclass.cbSize        = sizeof (wndclass) ;
          wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
          wndclass.lpfnWndProc   = RectWndProc ;
          wndclass.cbClsExtra    = 0 ;
          wndclass.cbWndExtra    = sizeof (HANDLE) ;
          wndclass.hInstance     = hInstance ;
          wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
          wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
          wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
          wndclass.lpszMenuName  = NULL ;
          wndclass.lpszClassName = szRectClass ;
          wndclass.hIconSm       = LoadIcon (NULL, IDI_APPLICATION) ;

          RegisterClassEx (&wndclass) ;
          }
               // Obtain handles to three possible menus & submenus

     hMenuInit  = LoadMenu (hInst, "MdiMenuInit") ;
     hMenuHello = LoadMenu (hInst, "MdiMenuHello") ;
     hMenuRect  = LoadMenu (hInst, "MdiMenuRect") ;

     hMenuInitWindow  = GetSubMenu (hMenuInit,   INIT_MENU_POS) ;
     hMenuHelloWindow = GetSubMenu (hMenuHello, HELLO_MENU_POS) ;
     hMenuRectWindow  = GetSubMenu (hMenuRect,   RECT_MENU_POS) ;

               // Load accelerator table

     hAccel = LoadAccelerators (hInst, "MdiAccel") ;

               // Create the frame window

     hwndFrame = CreateWindow (szFrameClass, "MDI Demonstration",
                               WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
                               CW_USEDEFAULT, CW_USEDEFAULT,
                               CW_USEDEFAULT, CW_USEDEFAULT,
                               NULL, hMenuInit, hInstance, NULL) ;

     hwndClient = GetWindow (hwndFrame, GW_CHILD) ;

     ShowWindow (hwndFrame, iCmdShow) ;
     UpdateWindow (hwndFrame) ;

               // Enter the modified message loop

     while (GetMessage (&msg, NULL, 0, 0))
          {
          if (!TranslateMDISysAccel (hwndClient, &msg) &&
              !TranslateAccelerator (hwndFrame, hAccel, &msg))
               {
               TranslateMessage (&msg) ;
               DispatchMessage (&msg) ;
               }
          }
               // Clean up by deleting unattached menus

     DestroyMenu (hMenuHello) ;
     DestroyMenu (hMenuRect) ;

     return msg.wParam ;
     }

LRESULT CALLBACK FrameWndProc (HWND hwnd, UINT iMsg, WPARAM wParam,
                                                     LPARAM lParam)
     {
     static HWND         hwndClient ;
     CLIENTCREATESTRUCT  clientcreate ;
     HWND                hwndChild ;
     MDICREATESTRUCT     mdicreate ;

     switch (iMsg)
          {
          case WM_CREATE :          // Create the client window

               clientcreate.hWindowMenu  = hMenuInitWindow ;
               clientcreate.idFirstChild = IDM_FIRSTCHILD ;

               hwndClient = CreateWindow ("MDICLIENT", NULL,
                              WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
                              0, 0, 0, 0, hwnd, (HMENU) 1, hInst,
                              (LPSTR) &clientcreate) ;
               return 0 ;

          case WM_COMMAND :
               switch (wParam)
                    {
                    case IDM_NEWHELLO :       // Create a Hello child window

                         mdicreate.szClass = szHelloClass ;
                         mdicreate.szTitle = "Hello" ;
                         mdicreate.hOwner  = hInst ;
                         mdicreate.x       = CW_USEDEFAULT ;
                         mdicreate.y       = CW_USEDEFAULT ;
                         mdicreate.cx      = CW_USEDEFAULT ;
                         mdicreate.cy      = CW_USEDEFAULT ;
                         mdicreate.style   = 0 ;
                         mdicreate.lParam  = 0 ;

                         hwndChild = (HWND) SendMessage (hwndClient,
                                        WM_MDICREATE, 0,
                                        (LPARAM) (LPMDICREATESTRUCT) &mdicreate) ;
                         return 0 ;

                    case IDM_NEWRECT :        // Create a Rect child window

                         mdicreate.szClass = szRectClass ;
                         mdicreate.szTitle = "Rectangles" ;
                         mdicreate.hOwner  = hInst ;
                         mdicreate.x       = CW_USEDEFAULT ;
                         mdicreate.y       = CW_USEDEFAULT ;
                         mdicreate.cx      = CW_USEDEFAULT ;
                         mdicreate.cy      = CW_USEDEFAULT ;
                         mdicreate.style   = 0 ;
                         mdicreate.lParam  = 0 ;

                         hwndChild = (HWND) SendMessage (hwndClient,
                                        WM_MDICREATE, 0,
                                        (LPARAM) (LPMDICREATESTRUCT) &mdicreate) ;
                         return 0 ;

                    case IDM_CLOSE :          // Close the active window

                         hwndChild = (HWND) SendMessage (hwndClient,
                                                  WM_MDIGETACTIVE, 0, 0) ;

                         if (SendMessage (hwndChild, WM_QUERYENDSESSION, 0, 0))
                              SendMessage (hwndClient, WM_MDIDESTROY,
                                           (WPARAM) hwndChild, 0) ;
                         return 0 ;

                    case IDM_EXIT :           // Exit the program

                         SendMessage (hwnd, WM_CLOSE, 0, 0) ;
                         return 0 ;

                                   // messages for arranging windows
                    case IDM_TILE :
                         SendMessage (hwndClient, WM_MDITILE, 0, 0) ;
                         return 0 ;

                    case IDM_CASCADE :
                         SendMessage (hwndClient, WM_MDICASCADE, 0, 0) ;
                         return 0 ;

                    case IDM_ARRANGE :
                         SendMessage (hwndClient, WM_MDIICONARRANGE, 0, 0) ;
                         return 0 ;

                    case IDM_CLOSEALL :       // Attempt to close all children

                         EnumChildWindows (hwndClient, &CloseEnumProc, 0) ;
                         return 0 ;

                    default :            // Pass to active child...

                         hwndChild = (HWND) SendMessage (hwndClient,
                                                  WM_MDIGETACTIVE, 0, 0) ;

                        if (IsWindow (hwndChild))
                             SendMessage (hwndChild, WM_COMMAND,
                                          wParam, lParam) ;

                        break ;        // ...and then to DefFrameProc
                    }
               break ;

          case WM_QUERYENDSESSION :
          case WM_CLOSE :                     // Attempt to close all children

               SendMessage (hwnd, WM_COMMAND, IDM_CLOSEALL, 0) ;

               if (NULL != GetWindow (hwndClient, GW_CHILD))
                    return 0 ;

               break ;   // I.e., call DefFrameProc 

          case WM_DESTROY :
               PostQuitMessage (0) ;
               return 0 ;
          }
               // Pass unprocessed messages to DefFrameProc (not DefWindowProc)

     return DefFrameProc (hwnd, hwndClient, iMsg, wParam, lParam) ;
     }

BOOL CALLBACK CloseEnumProc (HWND hwnd, LPARAM lParam)
     {
     if (GetWindow (hwnd, GW_OWNER))         // Check for icon title
          return 1 ;

     SendMessage (GetParent (hwnd), WM_MDIRESTORE, (WPARAM) hwnd, 0) ;

     if (!SendMessage (hwnd, WM_QUERYENDSESSION, 0, 0))
          return 1 ;

     SendMessage (GetParent (hwnd), WM_MDIDESTROY, (WPARAM) hwnd, 0) ;
          return 1 ;
     }

LRESULT CALLBACK HelloWndProc (HWND hwnd, UINT iMsg, WPARAM wParam,
                                                        LPARAM lParam)
     {
     static COLORREF clrTextArray[] = { RGB (0,   0, 0), RGB (255, 0,   0),
                                        RGB (0, 255, 0), RGB (  0, 0, 255),
                                        RGB (255, 255, 255) } ;
     static HWND     hwndClient, hwndFrame ;
     HDC             hdc ;
     HMENU           hMenu ;
     LPHELLODATA     lpHelloData ;
     PAINTSTRUCT     ps ;
     RECT            rect ;

     switch (iMsg)
          {
          case WM_CREATE :
                         // Allocate memory for window private data

               lpHelloData = (LPHELLODATA) HeapAlloc (GetProcessHeap (),
                                                      HEAP_ZERO_MEMORY, 
                                                      sizeof (HELLODATA)) ;
               lpHelloData->iColor  = IDM_BLACK ;
               lpHelloData->clrText = RGB (0, 0, 0) ;
               SetWindowLong (hwnd, 0, (long) lpHelloData) ;

                         // Save some window handles

               hwndClient = GetParent (hwnd) ;
               hwndFrame  = GetParent (hwndClient) ;
               return 0 ;

          case WM_COMMAND :
               switch (wParam)
                    {
                    case IDM_BLACK :
                    case IDM_RED :
                    case IDM_GREEN :
                    case IDM_BLUE :
                    case IDM_WHITE :
                                   // Change the text color

                         lpHelloData = (LPHELLODATA) GetWindowLong (hwnd, 0) ;

                         hMenu = GetMenu (hwndFrame) ;

                         CheckMenuItem (hMenu, lpHelloData->iColor,
                                               MF_UNCHECKED) ;
                         lpHelloData->iColor = wParam ;
                         CheckMenuItem (hMenu, lpHelloData->iColor,
                                               MF_CHECKED) ;

                         lpHelloData->clrText =
                               clrTextArray[wParam - IDM_BLACK] ;

                         InvalidateRect (hwnd, NULL, FALSE) ;
                    }
               return 0 ;

          case WM_PAINT :
                         // Paint the window

               hdc = BeginPaint (hwnd, &ps) ;

               lpHelloData = (LPHELLODATA) GetWindowLong (hwnd, 0) ;
               SetTextColor (hdc, lpHelloData->clrText) ;

               GetClientRect (hwnd, &rect) ;

               DrawText (hdc, "Hello, World!", -1, &rect,
                         DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;

               EndPaint (hwnd, &ps) ;
               return 0 ;

          case WM_MDIACTIVATE :

                         // Set the Hello menu if gaining focus

               if (lParam == (LPARAM) hwnd)
                    SendMessage (hwndClient, WM_MDISETMENU,
                               (WPARAM) hMenuHello, (LPARAM) hMenuHelloWindow) ;

                         // Check or uncheck menu item

               lpHelloData = (LPHELLODATA) GetWindowLong (hwnd, 0) ;
               CheckMenuItem (hMenuHello, lpHelloData->iColor,
                        (lParam == (LPARAM) hwnd) ? MF_CHECKED : MF_UNCHECKED) ;

                         // Set the Init menu if losing focus

               if (lParam != (LPARAM) hwnd)
                    SendMessage (hwndClient, WM_MDISETMENU, (WPARAM) hMenuInit,
                                 (LPARAM) hMenuInitWindow) ;

               DrawMenuBar (hwndFrame) ;
               return 0 ;

          case WM_QUERYENDSESSION :
          case WM_CLOSE :
               if (IDOK != MessageBox (hwnd, "OK to close window?", "Hello",
                                       MB_ICONQUESTION | MB_OKCANCEL))
                    return 0 ;

               break ;   // I.e., call DefMDIChildProc

          case WM_DESTROY :
               lpHelloData = (LPHELLODATA) GetWindowLong (hwnd, 0) ;
               HeapFree (GetProcessHeap (), 0, lpHelloData) ;
               return 0 ;
          }
               // Pass unprocessed message to DefMDIChildProc

     return DefMDIChildProc (hwnd, iMsg, wParam, lParam) ;
     }

LRESULT CALLBACK RectWndProc (HWND hwnd, UINT iMsg, WPARAM wParam,
                                                       LPARAM lParam)
     {
     static HWND  hwndClient, hwndFrame ;
     HBRUSH       hBrush ;
     HDC          hdc ;
     LPRECTDATA   lpRectData ;
     PAINTSTRUCT  ps ;
     int          xLeft, xRight, yTop, yBottom ;
     short        nRed, nGreen, nBlue ;

     switch (iMsg)
          {
          case WM_CREATE :
                         // Allocate memory for window private data

               lpRectData = (LPRECTDATA) HeapAlloc (GetProcessHeap (),
                                                    HEAP_ZERO_MEMORY, 
                                                    sizeof (RECTDATA)) ;

               SetWindowLong (hwnd, 0, (long) lpRectData) ;

                         // Start the timer going

               SetTimer (hwnd, 1, 250, NULL) ;

                         // Save some window handles

               hwndClient = GetParent (hwnd) ;
               hwndFrame  = GetParent (hwndClient) ;
               return 0 ;

          case WM_SIZE :            // If not minimized, save the window size

               if (wParam != SIZE_MINIMIZED)
                    {
                    lpRectData = (LPRECTDATA) GetWindowLong (hwnd, 0) ;

                    lpRectData->cxClient = LOWORD (lParam) ;
                    lpRectData->cyClient = HIWORD (lParam) ;
                    }

               break ;        // WM_SIZE must be processed by DefMDIChildProc

          case WM_TIMER :           // Display a random rectangle

               lpRectData = (LPRECTDATA) GetWindowLong (hwnd, 0) ;

               xLeft   = rand () % lpRectData->cxClient ;
               xRight  = rand () % lpRectData->cxClient ;
               yTop    = rand () % lpRectData->cyClient ;
               yBottom = rand () % lpRectData->cyClient ;
               nRed    = rand () & 255 ;
               nGreen  = rand () & 255 ;
               nBlue   = rand () & 255 ;

               hdc = GetDC (hwnd) ;
               hBrush = CreateSolidBrush (RGB (nRed, nGreen, nBlue)) ;
               SelectObject (hdc, hBrush) ;

               Rectangle (hdc, min (xLeft, xRight), min (yTop, yBottom),
                               max (xLeft, xRight), max (yTop, yBottom)) ;

               ReleaseDC (hwnd, hdc) ;
               DeleteObject (hBrush) ;
               return 0 ;

          case WM_PAINT :           // Clear the window

               InvalidateRect (hwnd, NULL, TRUE) ;
               hdc = BeginPaint (hwnd, &ps) ;
               EndPaint (hwnd, &ps) ;
               return 0 ;

          case WM_MDIACTIVATE :     // Set the appropriate menu
               if (lParam == (LPARAM) hwnd)
                    SendMessage (hwndClient, WM_MDISETMENU, (WPARAM) hMenuRect,
                                 (LPARAM) hMenuRectWindow) ;
               else
                    SendMessage (hwndClient, WM_MDISETMENU, (WPARAM) hMenuInit,
                                 (LPARAM) hMenuInitWindow) ;

               DrawMenuBar (hwndFrame) ;
               return 0 ;

          case WM_DESTROY :
               lpRectData = (LPRECTDATA) GetWindowLong (hwnd, 0) ;
               HeapFree (GetProcessHeap (), 0, lpRectData) ;
               KillTimer (hwnd, 1) ;
               return 0 ;
          }
               // Pass unprocessed message to DefMDIChildProc

     return DefMDIChildProc (hwnd, iMsg, wParam, lParam) ;
     }
