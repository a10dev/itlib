BOOL tv_BuildRootFolder (HWND hwndListView) ;
void tv_ExpandChild (HWND hwndLV, LPARAM lParam) ;
BOOL tv_FetchMacro (LPARAM lParam, HWND hwndCtrl) ;

typedef struct _ITEM
     {
     int        nType ;    // Type (TYPE_ITEM or TYPE_FOLDER)
     HTREEITEM  hItem ;    // Handle to TreeView item
     LPSTR      pName ;    // Message name
     LPSTR      pMacro ;   // Macro
     } ITEM, *LPITEM ;

typedef struct _FOLDER
     {
     int        nType ;    // Type (TYPE_ITEM or TYPE_FOLDER)
     HTREEITEM  hItem ;    // Handle to TreeView item
     LPSTR      pName ;    // Name of folder
     LPITEM     piChild ;  // Pointer to macro data array
     } FOLDER, *LPFOLDER ;

#define LASTITEM ((HTREEITEM) 0xffff)
#define TYPE_FOLDER 0x01
#define TYPE_ITEM   0x02
