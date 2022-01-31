/*--------------------------------
   HELPER.C -- Helper routines.
               (c) Paul Yao, 1996
  --------------------------------*/
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include "property.h"


//-------------------------------------------------------------------
void SetButtonCheck (HWND hwndDlg, int CtrlID, BOOL bCheck)
     {
     HWND hwndCtrl = GetDlgItem (hwndDlg, CtrlID) ;
     if (bCheck)
          {
          Button_SetCheck (hwndCtrl, BST_CHECKED) ;
          }
     }

//-------------------------------------------------------------------
BOOL QueryButtonCheck (HWND hwndDlg, int CtrlID)
     {
     HWND hwndCtrl = GetDlgItem (hwndDlg, CtrlID) ;
     int  nCheck = Button_GetCheck (hwndCtrl) ;
     return (nCheck == BST_CHECKED) ;
     } 
