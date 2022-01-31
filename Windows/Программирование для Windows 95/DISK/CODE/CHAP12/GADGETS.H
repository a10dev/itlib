// Resource definitions.
#define IDM_FILE_NEW                    100    // -- Menu Commands --
#define IDM_FILE_OPEN                   101
#define IDM_FILE_SAVE                   102
#define IDM_FILE_SAVEAS                 103
#define IDM_FILE_PRINT                  104
#define IDM_FILE_PREVIEW                105
#define IDM_FILE_EXIT                   106
#define IDM_EDIT_UNDO                   200
#define IDM_EDIT_CUT                    201
#define IDM_EDIT_COPY                   202
#define IDM_EDIT_PASTE                  203
#define IDM_EDIT_PROP                   204
#define IDM_TB_HELP                     250
#define IDM_TB_DELETE                   251
#define IDM_IGNORESIZE                  300
#define IDM_STRINGS                     301
#define IDM_LARGEICONS                  302
#define IDM_SMALLICONS                  303
#define IDM_NODIVIDER                   400
#define IDM_WRAPABLE                    401
#define IDM_TOP                         402
#define IDM_BOTTOM                      403
#define IDM_NOMOVEY                     404
#define IDM_NOPARENTALIGN               405
#define IDM_NORESIZE                    406
#define IDM_ADJUSTABLE                  407
#define IDM_ALTDRAG                     408
#define IDM_TOOLTIPS                    409
#define IDM_TB_CHECK                    500
#define IDM_TB_ENABLE                   501
#define IDM_TB_HIDE                     502
#define IDM_TB_INDETERMINATE            503
#define IDM_TB_PRESS                    504
#define IDM_TB_BUTTONCOUNT              505
#define IDM_TB_GETROWS                  506
#define IDM_TB_CUSTOMIZE                507
#define IDM_STAT_IGNORESIZE             600
#define IDM_STAT_SIZEGRIP               700
#define IDM_STAT_TOP                    701
#define IDM_STAT_BOTTOM                 702
#define IDM_STAT_NOMOVEY                703
#define IDM_STAT_NOPARENTALIGN          704
#define IDM_STAT_NORESIZE               705
#define IDM_ST_GETBORDERS               800
#define IDM_ST_GETPARTS                 801
#define IDM_ST_SETTEXT                  802
#define IDM_ST_SIMPLE                   803
#define IDM_NOTIFICATIONS_CLEAR         900
#define IDM_VIEW_TOOLBAR                1000
#define IDM_VIEW_STATUS                 1001
#define IDM_VIEW_NOTIFICATIONS          1002
#define IDM_COMBOBOX                    4000
#define IDI_APP                         101    // -- Icons --
#define IDS_MAIN_MENU                   71     // -- Strings --
#define IDS_MAIN_MENU1                  72
#define IDS_MAIN_MENU2                  73
#define IDS_MAIN_MENU3                  74
#define IDS_MAIN_MENU4                  75
#define IDS_MAIN_MENU5                  76
#define IDS_TOOLBAR_MENU                80
#define IDS_TOOLBAR_MENU1               81
#define IDS_TOOLBAR_MENU2               82
#define IDS_TOOLBAR_MENU3               83
#define IDS_STATUSBAR_MENU              90
#define IDS_STATUSBAR_MENU1             91
#define IDS_STATUSBAR_MENU2             92
#define IDS_STATUSBAR_MENU3             93

#define IDC_TB_COMBOBOX                 2000  // -- Toolbar combo box

// Toolbar functions.
HWND InitToolBar (HWND hwndParent) ;
HWND RebuildToolBar (HWND hwndParent, WORD wFlag) ;
void ToolBarMessage (HWND hwndTB, WORD wMsg) ;
LRESULT ToolBarNotify (HWND hwnd, WPARAM wParam, LPARAM lParam) ;

// Tooltip functions.
BOOL InitToolTip (HWND hwndToolBar, HWND hwndComboBox) ;
BOOL RelayToolTipMessage (LPMSG pMsg) ;
void CopyToolTipText (LPTOOLTIPTEXT lpttt) ;

// Status bar functions.
HWND InitStatusBar (HWND hwndParent) ;
HWND RebuildStatusBar (HWND hwndParent, WORD wFlag) ;
void StatusBarMessage (HWND hwndSB, WORD wMsg) ;
LRESULT Statusbar_MenuSelect (HWND, WPARAM, LPARAM) ;

// Notification window functions.
HWND ViewNotificationsToggle (HWND hwnd) ;
void DisplayNotificationDetails (WPARAM wParam, LPARAM lParam) ;
void ClearNotificationList () ;
