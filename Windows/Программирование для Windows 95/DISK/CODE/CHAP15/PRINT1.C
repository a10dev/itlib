/*---------------------------------------
   PRINT1.C -- Bare Bones Printing
               (c) Charles Petzold, 1996
  ---------------------------------------*/

#include <windows.h>

HDC  GetPrinterDC (void) ;              // in PRINT.C
void PageGDICalls (HDC, int, int) ;

HINSTANCE hInst ;
char      szAppName[] = "Print1" ;
char      szCaption[] = "Print Program 1" ;

BOOL PrintMyPage (HWND hwnd)
     {
     static DOCINFO di     = { sizeof (DOCINFO), "Print1: Printing", NULL } ;
     BOOL           bError = FALSE ;
     HDC            hdcPrn ;
     int            xPage, yPage ;

     if (NULL == (hdcPrn = GetPrinterDC ()))
          return TRUE ;

     xPage = GetDeviceCaps (hdcPrn, HORZRES) ;
     yPage = GetDeviceCaps (hdcPrn, VERTRES) ;

     if (StartDoc (hdcPrn, &di) > 0)
          {
          if (StartPage (hdcPrn) > 0)
               {
               PageGDICalls (hdcPrn, xPage, yPage) ;

               if (EndPage (hdcPrn) > 0)
                    EndDoc (hdcPrn) ;
               else
                    bError = TRUE ;
               }
          }
     else
          bError = TRUE ;

     DeleteDC (hdcPrn) ;
     return bError ;
     }
