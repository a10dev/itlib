/*----------------------------------------------
   TREEVIEW.C -- Functions to support tree view
                 (c) Paul Yao, 1996
  ----------------------------------------------*/
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include "treeview.h"
#include "treedata.h"
#include "resource.h"

//-------------------------------------------------------------------
BOOL tv_BuildRootFolder (HWND hwndLV)
     {
     int i ;
     TV_INSERTSTRUCT tvis ;

     // Initialize list view item structure
     ZeroMemory (&tvis, sizeof(TV_INSERTSTRUCT)) ;
     tvis.hParent = NULL ;
     tvis.hInsertAfter = TVI_LAST ; 
     tvis.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_CHILDREN | 
                      TVIF_IMAGE | TVIF_SELECTEDIMAGE ;
     tvis.item.iImage = 0 ;
     tvis.item.iSelectedImage = 0 ;
     tvis.item.cchTextMax = 256 ; 

     for (i = 0 ; i < cRootFolders ; i++)
          {
          tvis.item.pszText = fldRoot[i].pName ; 
          tvis.item.lParam = (LPARAM) &fldRoot[i] ;
          tvis.item.cChildren = 1 ;
          fldRoot[i].hItem = TreeView_InsertItem (hwndLV, &tvis) ;
          }

     return TRUE ;
     }

//-------------------------------------------------------------------
void tv_ExpandChild (HWND hwndLV, LPARAM lParam)
     {
     int i ;
     LPFOLDER pFolder = (LPFOLDER) lParam ;
     LPITEM   pItem = pFolder->piChild ;
     TV_INSERTSTRUCT tvis ;

     // Initialize list view item structure
     ZeroMemory (&tvis, sizeof(TV_INSERTSTRUCT)) ;
     tvis.hParent = pFolder->hItem ;
     tvis.hInsertAfter = TVI_LAST ; 
     tvis.item.mask = TVIF_TEXT | TVIF_PARAM | 
                      TVIF_IMAGE | TVIF_SELECTEDIMAGE ;
     tvis.item.iImage = 1 ;
     tvis.item.iSelectedImage = 1 ;
     tvis.item.cchTextMax = 256 ;

     if (pItem->hItem == NULL)
          {
          if (pItem->nType == TYPE_ITEM)        // Add macro items
               {
               for (i = 0 ; pItem[i].hItem != LASTITEM ; i++)
                    {
                    tvis.item.pszText = pItem[i].pName ; 
                    tvis.item.lParam = (LPARAM) &pItem[i] ;
                    pItem[i].hItem = TreeView_InsertItem (hwndLV, &tvis) ;
                    }
               }
          else // (pItem->nType == TYPE_FOLDER) // Add folder items
               {
               LPFOLDER pFldr = (LPFOLDER) pFolder->piChild ;
               tvis.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_CHILDREN |
                                TVIF_IMAGE | TVIF_SELECTEDIMAGE ;
               tvis.item.iImage = 0 ;
               tvis.item.iSelectedImage = 0 ;
               for (i = 0 ; pFldr[i].hItem != LASTITEM ; i++)
                    {
                    tvis.item.pszText = pFldr[i].pName ; 
                    tvis.item.lParam = (LPARAM) &pFldr[i] ;
                    tvis.item.cChildren = 1 ;
                    pFldr[i].hItem = TreeView_InsertItem (hwndLV, &tvis) ;
                    } /* [for i] */
               } /* [if pItem->nType] */
          } /* [if pItem->hItem] */
     }

//-------------------------------------------------------------------
BOOL tv_FetchMacro (LPARAM lParam, HWND hwndCtrl)
     {
     LPITEM pItem = (LPITEM) lParam ;

     if (pItem->nType == TYPE_FOLDER)
          return FALSE ;

     Edit_SetText (hwndCtrl, pItem->pMacro) ;
     return TRUE ;
     }

