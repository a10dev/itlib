/*---------------------------------------
   TOOLTIP.C -- Tooltip helper functions
                (c) Paul Yao, 1996
  ---------------------------------------*/
#include <windows.h>
#include <commctrl.h>
#include "comcthlp.h"
#include "notifdef.h"
#include "gadgets.h"

extern BOOL bComboBox;
extern char szTbStrings[];
extern HINSTANCE hInst;
extern HWND hwndEdit;
extern HWND hwndCombo;
extern HWND hwndEdit;
static HWND hwndTT;

// Map toolbar button command to string index. 
int CommandToString[] = 
     { IDM_FILE_NEW, IDM_FILE_OPEN, IDM_FILE_SAVE, IDM_FILE_PRINT,
       IDM_FILE_PREVIEW, IDM_EDIT_CUT, IDM_EDIT_COPY, IDM_EDIT_PASTE,
       IDM_EDIT_UNDO, IDM_EDIT_PROP, IDM_TB_HELP, IDM_TB_DELETE, -1   
     } ;

//-------------------------------------------------------------------
BOOL InitToolTip (HWND hwndToolBar, HWND hwndComboBox)
     {
     BOOL bSuccess ;
     TOOLINFO ti ;

     // Fetch handle to tooltip control
     hwndTT = ToolBar_GetToolTips (hwndToolBar) ;
     if (hwndTT == NULL) 
          return FALSE ;

     // Add tooltip for main combo box
     ZeroMemory (&ti, sizeof (TOOLINFO)) ;
	 ti.cbSize = sizeof (TOOLINFO) ;
     ti.uFlags = TTF_IDISHWND | TTF_CENTERTIP | TTF_SUBCLASS ;
     ti.hwnd   = hwndToolBar ;
     ti.uId    = (UINT) (HWND) hwndComboBox ;
     ti.lpszText = LPSTR_TEXTCALLBACK ;
     bSuccess = ToolTip_AddTool (hwndTT, &ti) ;
     if (!bSuccess)
          return FALSE ;

     // Add tooltip for combo box's edit control
     hwndEdit = GetWindow (hwndComboBox, GW_CHILD) ;
     ti.uId    = (UINT) (HWND) hwndEdit ;
     bSuccess = ToolTip_AddTool (hwndTT, &ti) ;

     return bSuccess ;
     }

//-------------------------------------------------------------------
void CopyToolTipText (LPTOOLTIPTEXT lpttt)
     {
     int i ;
     int iButton = lpttt->hdr.idFrom ;
     int cb ;
     int cMax ;
     LPSTR pString ;
     LPSTR pDest = lpttt->lpszText ;

     // Check for combo box window handles
     if (lpttt->uFlags & TTF_IDISHWND)
          {
          if ((iButton == (int) hwndCombo) ||
              (iButton == (int) hwndEdit))
               {
               lstrcpy (pDest, "1-2-3 ComboBox") ;
               return ;
               }
          }

     // Map command ID to string index
     for (i = 0 ; CommandToString[i] != -1 ; i++)
          {
          if (CommandToString[i] == iButton)
               {
               iButton = i ;
               break ;
               }
          }

     // To be safe, count number of strings in text
     pString = szTbStrings ;
     cMax = 0 ;
     while (*pString != '\0')
          {
          cMax++ ;
          cb = lstrlen (pString) ;
          pString += (cb + 1) ;
          }

     // Check for valid parameter
     if (iButton > cMax)
          {
          pString = "Invalid Button Index" ;
          }
     else
          {
          // Cycle through to requested string
          pString = szTbStrings ;
          for (i = 0 ; i < iButton ; i++)
               {
               cb = lstrlen (pString) ;
               pString += (cb + 1) ;
               }
          }

     lstrcpy (pDest, pString) ;
     }
