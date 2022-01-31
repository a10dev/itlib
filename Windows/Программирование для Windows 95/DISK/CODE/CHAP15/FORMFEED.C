/*---------------------------------------------
   FORMFEED.C -- Advances printer to next page
                 (c) Charles Petzold, 1996
  ---------------------------------------------*/

#include <windows.h>

HDC  GetPrinterDC (void) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPSTR lpszCmdLine, int iCmdShow)
     {
     static DOCINFO di       = { sizeof (DOCINFO), "FormFeed", NULL } ;
     HDC            hdcPrint = GetPrinterDC () ;

     if (hdcPrint != NULL)
          {
          if (StartDoc (hdcPrint, &di) > 0)
               if (StartPage (hdcPrint) > 0 && EndPage (hdcPrint) > 0)
                    EndDoc (hdcPrint) ;

          DeleteDC (hdcPrint) ;
          }
     return FALSE ;
     }

HDC GetPrinterDC (void)
     {
     PRINTER_INFO_5 pinfo5[3] ; 
     DWORD          dwNeeded, dwReturned ;

     if (EnumPrinters (PRINTER_ENUM_DEFAULT, NULL, 5, (LPBYTE) pinfo5,
                       sizeof (pinfo5), &dwNeeded, &dwReturned))
          return CreateDC (NULL, pinfo5[0].pPrinterName, NULL, NULL) ;

     return 0 ;            // EnumPrinters failed, so return null hdc
     }
