/*------------------------------------------------------------------
   DEVCAPS2.C -- Displays Device Capability Information (Version 2)
                 (c) Charles Petzold, 1996
  ------------------------------------------------------------------*/

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include "devcaps2.h"

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;
void DoBasicInfo    (HDC, HDC, int, int) ;
void DoOtherInfo    (HDC, HDC, int, int) ;
void DoBitCodedCaps (HDC, HDC, int, int, int) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
     {
     static char szAppName[] = "DevCaps2" ;
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

     hwnd = CreateWindow (szAppName, NULL,
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

LRESULT CALLBACK WndProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
     {
     static char   szDevice[32], szWindowText[64] ;
     static int    n, cxChar, cyChar, 
		           nCurrentDevice = IDM_SCREEN,
                   nCurrentInfo   = IDM_BASIC ;
     static DWORD  dwNeeded, dwReturned ;
     static LPPRINTER_INFO_5 pinfo5 ;

     DWORD         i ;
     HDC           hdc, hdcInfo ;
     HMENU         hMenu ;
     PAINTSTRUCT   ps ;
     TEXTMETRIC    tm ;
     HANDLE        hPrint ;

     switch (msg)
          {
          case WM_CREATE :
               hdc = GetDC (hwnd) ;
               SelectObject (hdc, GetStockObject (SYSTEM_FIXED_FONT)) ;
               GetTextMetrics (hdc, &tm) ;
               cxChar = tm.tmAveCharWidth ;
               cyChar = tm.tmHeight + tm.tmExternalLeading ;
               ReleaseDC (hwnd, hdc) ;

               lParam = 0 ;
                                                  // fall through
          case WM_WININICHANGE :
               if (lParam != 0 && lstrcmp ((PSTR) lParam, "devices") != 0)
                    return 0 ;

               hMenu = GetSubMenu (GetMenu (hwnd), 0) ;

               while (GetMenuItemCount (hMenu) > 1)
                    DeleteMenu (hMenu, 1, MF_BYPOSITION) ;

               // Get a list of all local and remote printers
               // 
               // First, find out how large an array we need; this
               //   call will fail, leaving the required size in dwNeeded
               EnumPrinters (PRINTER_ENUM_LOCAL, 
                             NULL, 5, (LPBYTE) "", 0, &dwNeeded, &dwReturned) ;

               // Next, allocate space for PRINTER_INFO_5 array
               if (pinfo5)
                    HeapFree (GetProcessHeap (), 0, pinfo5) ;
               pinfo5 = (LPPRINTER_INFO_5) HeapAlloc (GetProcessHeap (), 
                                              HEAP_NO_SERIALIZE, dwNeeded) ;

               // Last, fill allocated PRINTER_INFO_5 array
               if (!pinfo5 || !EnumPrinters (PRINTER_ENUM_LOCAL, 
                                    NULL, 5, (LPBYTE) pinfo5, dwNeeded, 
                                    &dwNeeded, &dwReturned))
                    {
                    MessageBox (hwnd, "Could not enumerate printers!",
                                                  NULL, MB_ICONSTOP) ;
                    DestroyWindow (hwnd) ;
                    return 0 ;
                    }

               for (i = 0, n = IDM_SCREEN + 1 ; i < dwReturned ; i++, n++)
                    {
                    AppendMenu (hMenu, n % 16 ? 0 : MF_MENUBARBREAK, n, 
                                                  pinfo5->pPrinterName) ;
                    pinfo5++ ;
                    }

               AppendMenu (hMenu, MF_SEPARATOR, 0, NULL) ;
               AppendMenu (hMenu, 0, IDM_DEVMODE, "Properties") ;

               wParam = IDM_SCREEN ;
                                                  // fall through
          case WM_COMMAND :
               hMenu = GetMenu (hwnd) ;

               if (wParam < IDM_DEVMODE)          // IDM_SCREEN & Printers
                    {
                    CheckMenuItem (hMenu, nCurrentDevice, MF_UNCHECKED) ;
                    nCurrentDevice = wParam ;
                    CheckMenuItem (hMenu, nCurrentDevice, MF_CHECKED) ;
                    }
               else if (wParam == IDM_DEVMODE)    // "Properties" selection
                    {
                    GetMenuString (hMenu, nCurrentDevice, szDevice,
                                   sizeof (szDevice), MF_BYCOMMAND) ;

                    if (OpenPrinter (szDevice, &hPrint, NULL))
                         {
                         PrinterProperties (hwnd, hPrint) ;
                         ClosePrinter (hPrint) ;
                         }
                    }
               else                               // info menu items
                    {
                    CheckMenuItem (hMenu, nCurrentInfo, MF_UNCHECKED) ;
                    nCurrentInfo = wParam ;
                    CheckMenuItem (hMenu, nCurrentInfo, MF_CHECKED) ;
                    }
               InvalidateRect (hwnd, NULL, TRUE) ;
               return 0 ;

          case WM_INITMENUPOPUP :
               if (lParam == 0)
                    EnableMenuItem (GetMenu (hwnd), IDM_DEVMODE,
                         nCurrentDevice == IDM_SCREEN ?
                              MF_GRAYED : MF_ENABLED) ;
               return 0 ;

          case WM_PAINT :
               strcpy (szWindowText, "Device Capabilities: ") ;
          
               if (nCurrentDevice == IDM_SCREEN)
                    {
                    strcpy (szDevice, "DISPLAY") ;
                    hdcInfo = CreateIC (szDevice, NULL, NULL, NULL) ;
                    }
               else
                    {
                    hMenu = GetMenu (hwnd) ;
                    GetMenuString (hMenu, nCurrentDevice, szDevice,
                                   sizeof (szDevice), MF_BYCOMMAND) ;
                    hdcInfo = CreateIC (NULL, szDevice, NULL, NULL) ;
                    }

               strcat (szWindowText, szDevice) ;
               SetWindowText (hwnd, szWindowText) ;

               hdc = BeginPaint (hwnd, &ps) ;
               SelectObject (hdc, GetStockObject (SYSTEM_FIXED_FONT)) ;

               if (hdcInfo)
                    {
                    switch (nCurrentInfo)
                         {
                         case IDM_BASIC :
                              DoBasicInfo (hdc, hdcInfo, cxChar, cyChar) ;
                              break ;

                         case IDM_OTHER :
                              DoOtherInfo (hdc, hdcInfo, cxChar, cyChar) ;
                              break ;

                         case IDM_CURVE :
                         case IDM_LINE :
                         case IDM_POLY :
                         case IDM_TEXT :
                              DoBitCodedCaps (hdc, hdcInfo, cxChar, cyChar,
                                   nCurrentInfo - IDM_CURVE) ;
                              break ;
                         }
                    DeleteDC (hdcInfo) ;
                    }

               EndPaint (hwnd, &ps) ;
               return 0 ;

          case WM_DESTROY :
               if (pinfo5)
                    HeapFree (GetProcessHeap (), 0, pinfo5) ;

               PostQuitMessage (0) ;
               return 0 ;
          }
     return DefWindowProc (hwnd, msg, wParam, lParam) ;
     }

void DoBasicInfo (HDC hdc, HDC hdcInfo, int cxChar, int cyChar)
     {
     static struct
          {
          int   nIndex ;
          char *szDesc ;
          }
          info[] =
          {
          HORZSIZE,      "HORZSIZE     Width in millimeters:",
          VERTSIZE,      "VERTSIZE     Height in millimeters:",
          HORZRES,       "HORZRES      Width in pixels:",
          VERTRES,       "VERTRES      Height in raster lines:",
          BITSPIXEL,     "BITSPIXEL    Color bits per pixel:",
          PLANES,        "PLANES       Number of color planes:",
          NUMBRUSHES,    "NUMBRUSHES   Number of device brushes:",
          NUMPENS,       "NUMPENS      Number of device pens:",
          NUMMARKERS,    "NUMMARKERS   Number of device markers:",
          NUMFONTS,      "NUMFONTS     Number of device fonts:",
          NUMCOLORS,     "NUMCOLORS    Number of device colors:",
          PDEVICESIZE,   "PDEVICESIZE  Size of device structure:",
          ASPECTX,       "ASPECTX      Relative width of pixel:",
          ASPECTY,       "ASPECTY      Relative height of pixel:",
          ASPECTXY,      "ASPECTXY     Relative diagonal of pixel:",
          LOGPIXELSX,    "LOGPIXELSX   Horizontal dots per inch:",
          LOGPIXELSY,    "LOGPIXELSY   Vertical dots per inch:",
          SIZEPALETTE,   "SIZEPALETTE  Number of palette entries:",
          NUMRESERVED,   "NUMRESERVED  Reserved palette entries:",
          COLORRES,      "COLORRES     Actual color resolution:"
          } ;
     char   szBuffer[80] ;
     int    i ;

     for (i = 0 ; i < sizeof (info) / sizeof (info[0]) ; i++)
          TextOut (hdc, cxChar, (i + 1) * cyChar, szBuffer,
               sprintf (szBuffer, "%-40s%8d", info[i].szDesc,
                    GetDeviceCaps (hdcInfo, info[i].nIndex))) ;
     }

void DoOtherInfo (HDC hdc, HDC hdcInfo, int cxChar, int cyChar)
     {
     static BITS clip[] =
          {
          CP_RECTANGLE,  "CP_RECTANGLE",     "Can Clip To Rectangle:"
          } ; 

     static BITS raster[] =
          {
          RC_BITBLT,       "RC_BITBLT",       "Capable of simple BitBlt:",
          RC_BANDING,      "RC_BANDING",      "Requires banding support:",
          RC_SCALING,      "RC_SCALING",      "Requires scaling support:",
          RC_BITMAP64,     "RC_BITMAP64",     "Supports bitmaps >64K:",
          RC_GDI20_OUTPUT, "RC_GDI20_OUTPUT", "Has 2.0 output calls:",
          RC_DI_BITMAP,    "RC_DI_BITMAP",    "Supports DIB to memory:",
          RC_PALETTE,      "RC_PALETTE",      "Supports a palette:",
          RC_DIBTODEV,     "RC_DIBTODEV",     "Supports bitmap conversion:",
          RC_BIGFONT,      "RC_BIGFONT",      "Supports fonts >64K:",
          RC_STRETCHBLT,   "RC_STRETCHBLT",   "Supports StretchBlt:",
          RC_FLOODFILL,    "RC_FLOODFILL",    "Supports FloodFill:",
          RC_STRETCHDIB,   "RC_STRETCHDIB",   "Supports StretchDIBits:"
          } ;

     static char *szTech[] = { "DT_PLOTTER (Vector plotter)",
                               "DT_RASDISPLAY (Raster display)",
                               "DT_RASPRINTER (Raster printer)",
                               "DT_RASCAMERA (Raster camera)",
                               "DT_CHARSTREAM (Character-stream, PLP)",
                               "DT_METAFILE (Metafile, VDM)",
                               "DT_DISPFILE (Display-file)" } ;
     char        szBuffer[80] ;
     int         i ;

     TextOut (hdc, cxChar, cyChar, szBuffer,
          sprintf (szBuffer, "%-24s%04XH",
               "DRIVERVERSION:", GetDeviceCaps (hdcInfo, DRIVERVERSION))) ;

     TextOut (hdc, cxChar, 2 * cyChar, szBuffer,
          sprintf (szBuffer, "%-24s%-40s", "TECHNOLOGY:", 
		           szTech[GetDeviceCaps (hdcInfo, TECHNOLOGY)])) ;

     TextOut (hdc, cxChar, 4 * cyChar, szBuffer,
          sprintf (szBuffer, "CLIPCAPS (Clipping capabilities)")) ;

     for (i = 0 ; i < sizeof (clip) / sizeof (clip[0]) ; i++)
          TextOut (hdc, 9 * cxChar, (i + 6) * cyChar, szBuffer,
               sprintf (szBuffer, "%-16s%-28s %3s",
                    clip[i].szMask, clip[i].szDesc,
                    GetDeviceCaps (hdcInfo, CLIPCAPS) & clip[i].nMask ?
                         "Yes" : "No")) ;

     TextOut (hdc, cxChar, 8 * cyChar, szBuffer,
          sprintf (szBuffer, "RASTERCAPS (Raster capabilities)")) ;

     for (i = 0 ; i < sizeof (raster) / sizeof (raster[0]) ; i++)
          TextOut (hdc, 9 * cxChar, (i + 10) * cyChar, szBuffer,
               sprintf (szBuffer, "%-16s%-28s %3s",
                    raster[i].szMask, raster[i].szDesc,
                    GetDeviceCaps (hdcInfo, RASTERCAPS) & raster[i].nMask ?
                         "Yes" : "No")) ;
     }

void DoBitCodedCaps (HDC hdc, HDC hdcInfo, int cxChar, int cyChar, int nType)
     {
     static BITS curves[] =
          {
          CC_CIRCLES,    "CC_CIRCLES",    "circles:",
          CC_PIE,        "CC_PIE",        "pie wedges:",
          CC_CHORD,      "CC_CHORD",      "chord arcs:",
          CC_ELLIPSES,   "CC_ELLIPSES",   "ellipses:",
          CC_WIDE,       "CC_WIDE",       "wide borders:",
          CC_STYLED,     "CC_STYLED",     "styled borders:",
          CC_WIDESTYLED, "CC_WIDESTYLED", "wide and styled borders:",
          CC_INTERIORS,  "CC_INTERIORS",  "interiors:"
          } ; 

     static BITS lines[] =
          {
          LC_POLYLINE,   "LC_POLYLINE",   "polyline:",
          LC_MARKER,     "LC_MARKER",     "markers:",
          LC_POLYMARKER, "LC_POLYMARKER", "polymarkers",
          LC_WIDE,       "LC_WIDE",       "wide lines:",
          LC_STYLED,     "LC_STYLED",     "styled lines:",
          LC_WIDESTYLED, "LC_WIDESTYLED", "wide and styled lines:",
          LC_INTERIORS,  "LC_INTERIORS",  "interiors:"
          } ;

     static BITS poly[] =
          {
          PC_POLYGON,    "PC_POLYGON",    "alternate fill polygon:",
          PC_RECTANGLE,  "PC_RECTANGLE",  "rectangle:",
          PC_WINDPOLYGON,"PC_WINDPOLYGON","winding number fill polygon:",
          PC_SCANLINE,   "PC_SCANLINE",   "scanlines:",
          PC_WIDE,       "PC_WIDE",       "wide borders:",
          PC_STYLED,     "PC_STYLED",     "styled borders:",
          PC_WIDESTYLED, "PC_WIDESTYLED", "wide and styled borders:",
          PC_INTERIORS,  "PC_INTERIORS",  "interiors:"
          } ;

     static BITS text[] =
          {
          TC_OP_CHARACTER, "TC_OP_CHARACTER", "character output precision:",
          TC_OP_STROKE,    "TC_OP_STROKE",    "stroke output precision:",
          TC_CP_STROKE,    "TC_CP_STROKE",    "stroke clip precision:",
          TC_CR_90,        "TC_CP_90",        "90 degree character rotation:",
          TC_CR_ANY,       "TC_CR_ANY",       "any character rotation:",
          TC_SF_X_YINDEP,  "TC_SF_X_YINDEP",  "scaling independent of X and Y:",
          TC_SA_DOUBLE,    "TC_SA_DOUBLE",    "doubled character for scaling:",
          TC_SA_INTEGER,   "TC_SA_INTEGER",   "integer multiples for scaling:",
          TC_SA_CONTIN,    "TC_SA_CONTIN",    "any multiples for exact scaling:",
          TC_EA_DOUBLE,    "TC_EA_DOUBLE",    "double weight characters:",
          TC_IA_ABLE,      "TC_IA_ABLE",      "italicizing:",
          TC_UA_ABLE,      "TC_UA_ABLE",      "underlining:",
          TC_SO_ABLE,      "TC_SO_ABLE",      "strikeouts:",
          TC_RA_ABLE,      "TC_RA_ABLE",      "raster fonts:",
          TC_VA_ABLE,      "TC_VA_ABLE",      "vector fonts:"
          } ;

     static struct
          {
          int   nIndex ;
          char  *szTitle ;
          BITS  (*pbits)[] ;
          short nSize ;
          }
          bitinfo[] =
          {
          CURVECAPS,  "CURVCAPS (Curve Capabilities)",
                      (BITS (*)[]) curves, sizeof (curves) / sizeof (curves[0]),
          LINECAPS,   "LINECAPS (Line Capabilities)",
                      (BITS (*)[]) lines, sizeof (lines) / sizeof (lines[0]),
          POLYGONALCAPS, "POLYGONALCAPS (Polygonal Capabilities)",
                      (BITS (*)[]) poly, sizeof (poly) / sizeof (poly[0]),
          TEXTCAPS,   "TEXTCAPS (Text Capabilities)",
                      (BITS (*)[]) text, sizeof (text) / sizeof (text[0])
          } ;

     static char szBuffer[80] ;
     BITS        (*pbits)[] = bitinfo[nType].pbits ;
     int         nDevCaps = GetDeviceCaps (hdcInfo, bitinfo[nType].nIndex) ;
     int         i ;

     TextOut (hdc, cxChar, cyChar, bitinfo[nType].szTitle,
                    strlen (bitinfo[nType].szTitle)) ;

     for (i = 0 ; i < bitinfo[nType].nSize ; i++)
          TextOut (hdc, cxChar, (i + 3) * cyChar, szBuffer,
               sprintf (szBuffer, "%-16s %s %-32s %3s",
                    (*pbits)[i].szMask, "Can do", (*pbits)[i].szDesc,
                    nDevCaps & (*pbits)[i].nMask ? "Yes" : "No")) ;
     }
