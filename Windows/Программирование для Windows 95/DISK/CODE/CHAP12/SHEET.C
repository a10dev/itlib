/*-----------------------------------------
   SHEET.C -- Property sheet page functions.
                 (c) Paul Yao, 1996
  -----------------------------------------*/
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include "comcthlp.h"
#include "property.h"

extern DWORD dwChildStyle ;
extern DWORD dwChildExStyle ;
extern DWORD dwSheetStyles ;
extern char szAppName[] ;
extern HINSTANCE hInst ;
extern HICON hiconApp ;

//-------------------------------------------------------------------
int CALLBACK PropSheetProc (HWND hwndDlg, UINT uMsg, LPARAM lParam)
     {
     switch (uMsg)
          {
          case PSCB_INITIALIZED :
               // Process PSCB_INITIALIZED
               break ;

          case PSCB_PRECREATE :
               // Process PSCB_PRECREATE
               break ;

          default :
               // Unknown message
               break ;
          }

     return 0 ;
     }

//-------------------------------------------------------------------
BOOL CreatePropertySheet (HWND hwndParent)
     {
     PROPSHEETHEADER pshead ;
     PROPSHEETPAGE   pspage[2] ;

     // Initialize property sheet HEADER data
     ZeroMemory (&pshead, sizeof (PROPSHEETHEADER)) ;
     pshead.dwSize  = sizeof (PROPSHEETHEADER) ;
     pshead.dwFlags = dwSheetStyles     |
                      PSH_PROPSHEETPAGE |
                      PSH_USECALLBACK   |
                      PSH_USEHICON ;
     pshead.hwndParent  = hwndParent ;
     pshead.hInstance   = hInst ;
     pshead.hIcon       = hiconApp ;
     pshead.pszCaption  = "A Child Window" ;
     pshead.nPages      = 2 ;
     pshead.nStartPage  = 0 ;
     pshead.ppsp        = pspage ;
     pshead.pfnCallback = PropSheetProc ;

     // Zero out property PAGE data
     ZeroMemory (&pspage, 2 * sizeof (PROPSHEETPAGE)) ;

     // PAGE 1 -- window style page
     pspage[0].dwSize      = sizeof (PROPSHEETPAGE) ;
     pspage[0].dwFlags     = PSP_USECALLBACK | PSP_USEICONID ;
     pspage[0].hInstance   = hInst ;
     pspage[0].pszTemplate = MAKEINTRESOURCE (IDD_STYLES) ;
     pspage[0].pszIcon     = MAKEINTRESOURCE (IDI_PAGE1) ;
     pspage[0].pfnDlgProc  = StyleDlgProc ;
     pspage[0].lParam      = (LPARAM) &dwChildStyle ;
     pspage[0].pfnCallback = StylePageProc ;

     // PAGE 2 -- extended window style page
     pspage[1].dwSize      = sizeof (PROPSHEETPAGE) ;
     pspage[1].dwFlags     = PSP_USECALLBACK | PSP_USEICONID | 
                             PSP_HASHELP ;
     pspage[1].hInstance   = hInst ;
     pspage[1].pszTemplate = MAKEINTRESOURCE (IDD_EXSTYLES) ;
     pspage[1].pszIcon     = MAKEINTRESOURCE (IDI_PAGE2) ;
     pspage[1].pfnDlgProc  = ExStyleDlgProc ;
     pspage[1].lParam      = (LPARAM) &dwChildExStyle ;
     pspage[1].pfnCallback = ExStylePageProc ;

     // --------- Create & display property sheet --------- 

     return PropertySheet (&pshead) ;
     }
