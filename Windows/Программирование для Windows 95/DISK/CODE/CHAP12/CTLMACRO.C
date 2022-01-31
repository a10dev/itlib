/*--------------------------------------------
   CTLMACRO.C -- Control Macro Helper Utility
                 (c) Paul Yao, 1996
  --------------------------------------------*/
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <richedit.h>
#include <richole.h>
#include "treeview.h"
#include "resource.h"
#include "notify.h"

#define COL_WHITE     (RGB (255, 255, 255))

BOOL CALLBACK DlgProc (HWND, UINT, WPARAM, LPARAM) ;

HINSTANCE hInst ;

//-------------------------------------------------------------------
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                    PSTR lpszCmdLine, int cmdShow)       
     {
     hInst = hInstance ;

     // Load common control library
     InitCommonControls () ;

     DialogBox (hInstance, MAKEINTRESOURCE (IDD_DIALOG), NULL, DlgProc) ;

     return 0 ;
     }

//-------------------------------------------------------------------
BOOL CALLBACK
DlgProc (HWND hwnd, UINT mMsg, WPARAM wParam, LPARAM lParam) 
     {
     static BOOL bFirstSelection = TRUE ;

     switch (mMsg)
          {
          case WM_INITDIALOG :
               {
               HICON       hicon ;
               HIMAGELIST  himl ;
               HTREEITEM   hItem ;

               // Add listview root items
               HWND hwndCtrl = GetDlgItem (hwnd, IDC_MESSAGES) ;
               tv_BuildRootFolder (hwndCtrl) ;

               // Select first item
               hItem = TreeView_GetFirstVisible (hwndCtrl) ;
               TreeView_SelectItem (hwndCtrl, hItem) ;

               // Create image list & connect to list view
               himl = 
               ImageList_LoadBitmap (hInst,
                                     MAKEINTRESOURCE (IDR_TVITEMS),
                                     16,
                                     0,
                                     COL_WHITE) ;
            
               TreeView_SetImageList (hwndCtrl, himl, TVSIL_NORMAL) ;
            
               // Disable [Copy] button when no macro is present
               hwndCtrl = GetDlgItem (hwnd, IDC_COPY) ;
               EnableWindow (hwndCtrl, FALSE) ;

               // Install icon
               hicon = LoadIcon (hInst, MAKEINTRESOURCE (IDI_APP)) ;
               SendMessage (hwnd, WM_SETICON, (WPARAM) (BOOL) TRUE, 
                            (LPARAM) (HICON) hicon) ;

               return TRUE ;
               }

          case WM_COMMAND :
               switch (LOWORD (wParam))
                    {
                    // Copy macro data to clipboard
                    case IDC_COPY :
                         {
                         HWND hwndEdit = GetDlgItem (hwnd, IDC_MACROS) ;
                         Edit_SetSel (hwndEdit, 0, -1) ;
                         SendMessage (hwndEdit, WM_COPY, 0, 0L) ;
                         break ;
                         }

                    // Close dialog
                    case IDOK :
                         EndDialog (hwnd, 0) ;
                         break ;

                    // Enable & disable [Copy] button when edit changes
                    case IDC_MACROS :
                         {
                         WORD wNotifyCode = HIWORD (wParam) ;
                         if (wNotifyCode == EN_CHANGE)
                              {
                              HWND hwndEdit = (HWND) lParam ;
                              HWND hwndCopy = GetDlgItem (hwnd, IDC_COPY) ;
                              int  cb = Edit_GetTextLength (hwndEdit) ;
                              EnableWindow (hwndCopy, cb) ;
                              }
                         break ;
                         }

                    }
               return TRUE ;

          case WM_NOTIFY :
               {
               int idCtrl = (int) wParam ;
               LPNMHDR pnmh = (LPNMHDR) lParam ;

               // Expand tree as required
               if (pnmh->code == TVN_ITEMEXPANDING)
                    {
                    LPNM_TREEVIEW pnmtv = (LPNM_TREEVIEW) lParam ;
                    tv_ExpandChild (pnmh->hwndFrom, pnmtv->itemNew.lParam) ;
                    }

               // Respond to selection of items
               if (pnmh->code == TVN_SELCHANGED)
                    {
                    LPNM_TREEVIEW pnmtv = (LPNM_TREEVIEW) lParam ;
                    HWND hwndEdit = GetDlgItem (hwnd, IDC_MACROS) ;

                    // Ignore first selection request
                    if (bFirstSelection)
                         {
                         bFirstSelection = FALSE ;
                         return TRUE ;
                         }

                    // Fetch macro for leaf nodes
                    if (tv_FetchMacro (pnmtv->itemNew.lParam, hwndEdit))
                         {
                         Edit_SetSel (hwndEdit, 0, -1) ;
                         }
                    else
                         {
                         // Expand tree for folder nodes
                         TreeView_Expand (pnmh->hwndFrom, 
                                          pnmtv->itemNew.hItem, 
                                          TVE_EXPAND) ;
                         Edit_SetText (hwndEdit, "") ;
                         }

                    return TRUE ;
                    }

               // Double-click means copy macro to clipboard
               if (pnmh->code == NM_DBLCLK)
                    {
                    HWND hwndEdit   = GetDlgItem (hwnd, IDC_MACROS) ;
                    HWND hwndButton = GetDlgItem (hwnd, IDC_COPY) ;
                    if (Edit_GetTextLength (hwndEdit) > 0)
                         {
                         FORWARD_WM_COMMAND (hwnd, IDC_COPY, 
                                             hwndButton, BN_CLICKED,
                                             SendMessage) ;
                         }
                    }

               return TRUE ;
               }

          default :
               return FALSE ;
          }
     }
