/*----------------------------------------
   TOOLBAR.C -- Toolbar helper functions.
                (c) Paul Yao, 1996
  ----------------------------------------*/
#include <windows.h>
#include <commctrl.h>
#include "comcthlp.h"
#include "gadgets.h"
#include "notifdef.h"

HWND hwndCombo ;
HWND hwndEdit ;
HWND hwndToolTip ;
HWND hwndTB ;

int cyToolBar ;

BOOL bComboBox   = FALSE ;
BOOL bStrings    = FALSE ;
BOOL bLargeIcons = FALSE ;

DWORD dwToolBarStyles = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
                        CCS_TOP | CCS_NODIVIDER | TBSTYLE_TOOLTIPS ;

extern HINSTANCE hInst ;

char szTbStrings[] = "New\0Open\0Save\0Print\0Preview\0"
                     "Cut\0Copy\0Paste\0Undo\0Properties\0"
                     "Help\0Delete\0" ;

TBBUTTON tbb[] = 
     {
     0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0, 0, 0,
     0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0, 0, 0,
     0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0, 0, 0,
     0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0, 0, 0,
     0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0, 0, 0,
     0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0, 0, 0,
     0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0, 0, 0,
     0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0, 0, 0,
     0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0, 0, 0,
     0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0, 0, 0,
     0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0, 0, 0,
     0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0, 0, 0,
     0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0, 0, 0,
     0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0, 0, 0,
     0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0, 0, 0,
     0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0, 0, 0,
     0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0, 0, 0,
     0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0, 0, 0,
     0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0, 0, 0,
     0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0, 0, 0,
     0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0, 0, 0,
     STD_FILENEW, IDM_FILE_NEW, TBSTATE_ENABLED, TBSTYLE_BUTTON,
        0, 0, 0, 0,
     STD_FILEOPEN, IDM_FILE_OPEN, TBSTATE_ENABLED, TBSTYLE_BUTTON,
        0, 0, 0, 1,
     STD_FILESAVE, IDM_FILE_SAVE, TBSTATE_ENABLED, TBSTYLE_BUTTON,
        0, 0, 0, 2,
     STD_PRINT, IDM_FILE_PRINT, TBSTATE_ENABLED, TBSTYLE_BUTTON,
        0, 0, 0, 3,
     STD_PRINTPRE, IDM_FILE_PREVIEW, TBSTATE_ENABLED, TBSTYLE_CHECK,
        0, 0, 0, 4,

     0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0, 0, 0,

     STD_CUT, IDM_EDIT_CUT, TBSTATE_ENABLED, TBSTYLE_CHECKGROUP,
        0, 0, 0, 5,
     STD_COPY, IDM_EDIT_COPY, TBSTATE_ENABLED, TBSTYLE_CHECKGROUP,
        0, 0, 0, 6,
     STD_PASTE, IDM_EDIT_PASTE, TBSTATE_ENABLED, TBSTYLE_CHECKGROUP, 
        0, 0, 0, 7,
     STD_UNDO, IDM_EDIT_UNDO, TBSTATE_ENABLED, TBSTYLE_BUTTON,
        0, 0, 0, 8,
     STD_PROPERTIES, IDM_EDIT_PROP, TBSTATE_ENABLED, TBSTYLE_CHECK,
        0, 0, 0, 9,
     STD_HELP, IDM_TB_HELP, TBSTATE_ENABLED, TBSTYLE_BUTTON,
        0, 0, 0, 10,
     STD_DELETE, IDM_TB_DELETE, TBSTATE_ENABLED, TBSTYLE_BUTTON,
        0, 0, 0, 11,
     } ;

int nCust[] = { 21, 22, 23, 24, 25, 27, 28, 29, 30, 31, 32, 33, -1} ;

TBADDBITMAP tbStdLarge[] = 
     {
     HINST_COMMCTRL, IDB_STD_LARGE_COLOR,
     } ;

TBADDBITMAP tbStdSmall[] = 
     {
     HINST_COMMCTRL, IDB_STD_SMALL_COLOR,
     } ;

//-------------------------------------------------------------------
LPSTR GetString (int iString)
     {
     int i, cb ;
     LPSTR pString ;

     // Cycle through to requested string
     pString = szTbStrings ;
     for (i = 0 ; i < iString ; i++)
          {
          cb = lstrlen (pString) ;
          pString += (cb + 1) ;
          }

     return pString ;
     }

//-------------------------------------------------------------------
LRESULT ToolBarNotify (HWND hwnd, WPARAM wParam, LPARAM lParam)
     {
     LPNMHDR pnmh = (LPNMHDR) lParam ;
     int idCtrl = (int) wParam ;

     // Allow toolbar to be customized
     if ((pnmh->code == TBN_QUERYDELETE) ||
         (pnmh->code == TBN_QUERYINSERT))
          {
          return 1 ; // We always say "yes"
          }

     // Provide details of allowable toolbar buttons
     if (pnmh->code == TBN_GETBUTTONINFO)
          {
          LPTBNOTIFY ptbn = (LPTBNOTIFY) lParam ;
          int iButton = nCust[ptbn->iItem] ;

          if (iButton != -1)
               {
               lstrcpy (ptbn->pszText, GetString (ptbn->iItem)) ;
               memcpy (&ptbn->tbButton, &tbb[iButton], sizeof (TBBUTTON)) ;
               return 1 ;
               }
          }

     return 0 ;
     }

//-------------------------------------------------------------------
HWND InitToolBar (HWND hwndParent)
     {
     int  iNumButtons ;
     LPTBBUTTON ptbb ;

     if (bComboBox)
          {
          ptbb = &tbb[0] ;
          iNumButtons = 31 ;
          }
     else
          {
          ptbb = &tbb[21] ;
          iNumButtons = 10 ;
          }

     UINT uiBitmap = (bLargeIcons) ? IDB_STD_LARGE_COLOR :
                                     IDB_STD_SMALL_COLOR ;
     hwndTB = CreateToolbarEx (hwndParent,
                   dwToolBarStyles,
                   1, 15,
                   HINST_COMMCTRL,
                   uiBitmap,
                   ptbb,
                   iNumButtons,
                   0, 0, 0, 0,
                   sizeof (TBBUTTON)) ;

     // If requested, add to string list
     if (bStrings)
          ToolBar_AddString (hwndTB, 0, szTbStrings) ;

     // Store handle to tooltip control
     hwndToolTip = ToolBar_GetToolTips (hwndTB) ;

     // Insert combo box into toolbar
     if (bComboBox)
          {
          RECT r ;
          int x, y, cx, cy ;

          // Calculate coordinates for combo box
          ToolBar_GetItemRect (hwndTB, 0, &r) ;
          x = r.left ;
          y = r.top ;
          cy = 100 ;
          ToolBar_GetItemRect (hwndTB, 18, &r) ;
          cx = r.right - x + 1 ;

          hwndCombo = CreateWindow ("combobox",
                           NULL,
                           WS_CHILD | WS_VISIBLE |
                           CBS_DROPDOWN,
                           x, y, cx, cy,
                           hwndParent,
                           (HMENU) IDC_TB_COMBOBOX,
                           hInst,
                           0) ;

          // Set toolbar as combo box window parent
          SetParent (hwndCombo, hwndTB) ;

          SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) "One") ;
          SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) "Two") ;
          SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) "Three") ;

          // Calculate toolbar height
          GetWindowRect (hwndCombo, &r) ;
          cyToolBar = r.bottom - r.top + 1 ;
          cyToolBar += y ;
          cyToolBar += (2 * GetSystemMetrics (SM_CYBORDER)) ;
          ToolBar_GetItemRect (hwndTB, 0, &r) ;
          cyToolBar = max (cyToolBar, r.bottom+5) ;

          // If toolbar has tooltips, initialize.
          if (dwToolBarStyles & TBSTYLE_TOOLTIPS)
               InitToolTip (hwndTB, hwndCombo) ;
          }

     return hwndTB ;
     }

//-------------------------------------------------------------------
void static FlipStyleFlag (LPDWORD dwStyle, DWORD flag)
     {
     if (*dwStyle & flag)  // Flag on -- turn off
          {
          *dwStyle &= (~flag) ;
          }
     else                  // Flag off -- turn on
          {
          *dwStyle |= flag ;
          }
     }

//-------------------------------------------------------------------
HWND RebuildToolBar (HWND hwndParent, WORD wFlag)
     {
     HWND hwndTB ;
     RECT r ;

     switch (wFlag)
          {
          case IDM_STRINGS :
               bStrings = !bStrings ;
               break ;

          case IDM_LARGEICONS :
               bLargeIcons = TRUE ;
               break ;

          case IDM_SMALLICONS :
               bLargeIcons = FALSE ;
               break ;

          case IDM_NODIVIDER :
               FlipStyleFlag (&dwToolBarStyles, CCS_NODIVIDER) ;
               break ;

          case IDM_WRAPABLE :
               FlipStyleFlag (&dwToolBarStyles, TBSTYLE_WRAPABLE) ;
               break ;

          case IDM_TOP :
               dwToolBarStyles &= 0xFFFFFFFC ;
               dwToolBarStyles |= CCS_TOP ;
               break ;

          case IDM_BOTTOM :
               dwToolBarStyles &= 0xFFFFFFFC ;
               dwToolBarStyles |= CCS_BOTTOM ;
               break ;

          case IDM_NOMOVEY :
               dwToolBarStyles &= 0xFFFFFFFC ;
               dwToolBarStyles |= CCS_NOMOVEY ;
               break ;

          case IDM_NOPARENTALIGN :
               FlipStyleFlag (&dwToolBarStyles, CCS_NOPARENTALIGN) ;
               break ;

          case IDM_NORESIZE :
               FlipStyleFlag (&dwToolBarStyles, CCS_NORESIZE) ;
               break ;

          case IDM_ADJUSTABLE :
               FlipStyleFlag (&dwToolBarStyles, CCS_ADJUSTABLE) ;
               break ;

          case IDM_ALTDRAG :
               FlipStyleFlag (&dwToolBarStyles, TBSTYLE_ALTDRAG) ;
               break ;

          case IDM_TOOLTIPS :
               FlipStyleFlag (&dwToolBarStyles, TBSTYLE_TOOLTIPS) ;
               break ;

          case IDM_COMBOBOX :
               bComboBox = (!bComboBox) ;
          }

     hwndTB = InitToolBar (hwndParent) ;

     // Post parent a WM_SIZE message to resize children
     GetClientRect (hwndParent, &r) ;
     PostMessage (hwndParent, WM_SIZE, 0, 
                  MAKELPARAM (r.right, r.bottom)) ;

     return hwndTB ;
     }


//-------------------------------------------------------------------
void ToolBarMessage (HWND hwndTB, WORD wMsg)
     {
     switch (wMsg)
          {
          case IDM_TB_CHECK :
               {
               int nState = ToolBar_GetState (hwndTB, 1) ;
               BOOL bCheck = (!(nState & TBSTATE_CHECKED)) ;
               ToolBar_CheckButton (hwndTB, 1, bCheck ) ;
               break ;
               }

          case IDM_TB_ENABLE :
               {
               int nState = ToolBar_GetState (hwndTB, 2) ;
               BOOL bEnabled = (!(nState & TBSTATE_ENABLED)) ;
               ToolBar_EnableButton (hwndTB, 2, bEnabled) ;
               break ;
               }

          case IDM_TB_HIDE :
               {
               int nState = ToolBar_GetState (hwndTB, 3) ;
               BOOL bShow = (!(nState & TBSTATE_HIDDEN)) ;
               ToolBar_HideButton (hwndTB, 3, bShow) ;
               break ;
               }

          case IDM_TB_INDETERMINATE :
               {
               int nState = ToolBar_GetState (hwndTB, 4) ;
               BOOL bInd = (!(nState & TBSTATE_INDETERMINATE)) ;
               ToolBar_Indeterminate (hwndTB, 4, bInd) ;
               break ;
               }

          case IDM_TB_PRESS :
               {
               int nState = ToolBar_GetState (hwndTB, 5) ;
               BOOL bPress = (!(nState & TBSTATE_PRESSED)) ;
               ToolBar_PressButton (hwndTB, 5, bPress) ;
               break ;
               }

          case IDM_TB_BUTTONCOUNT :
               {
               int nButtons = ToolBar_ButtonCount (hwndTB) ;
               char ach[80] ;
               wsprintf (ach, "Button Count = %d", nButtons) ;
               MessageBox (GetParent (hwndTB), ach, 
                           "TB_BUTTONCOUNT", MB_OK) ;
               break ;
               }

          case IDM_TB_GETROWS :
               {
               int nRows = ToolBar_GetRows (hwndTB) ;
               char ach[80] ;
               wsprintf (ach, "Row Count = %d", nRows) ;
               MessageBox (GetParent (hwndTB), ach, 
                           "TB_GETROWS", MB_OK) ;
               break ;
               }
          case IDM_TB_CUSTOMIZE :
               // ToolBar_Customize (hwndTB) ;
               SendMessage (hwndTB, TB_CUSTOMIZE, (LPARAM) &tbb[25], 5) ;
               break ;
          }
     }
