// Menu item identifiers
#define IDM_OVERLAPPED    100
#define IDM_POPUP         101
#define IDM_CHILD         102
#define IDM_WIZARD        200
#define IDM_HASHELP       201
#define IDM_MODELESS      202
#define IDM_MULTILINETABS 203
#define IDM_NOAPPLYNOW    204
#define IDM_PROPTITLE     205
#define IDM_RTLREADING    206

// Dialog template IDs
#define IDD_STYLES                      101
#define IDD_EXSTYLES                    102

// Icon IDs
#define IDI_PAGE1                       103
#define IDI_PAGE2                       104

// Dialog Control IDs
#define IDC_BORDER                      1000
#define IDC_CAPTION                     1001
#define IDC_VISIBLE                     1005
#define IDC_DISABLED                    1006
#define IDC_DLGFRAME                    1007
#define IDC_MINIMIZE                    1008
#define IDC_MAXIMIZE                    1009
#define IDC_HSCROLL                     1010
#define IDC_MAXIMIZEBOX                 1011
#define IDC_MINIMIZEBOX                 1012
#define IDC_SYSMENU                     1013
#define IDC_THICKFRAME                  1014
#define IDC_VSCROLL                     1015
#define IDC_CLIPCHILDREN                1016
#define IDC_CLIPSIBLINGS                1017
#define IDC_CLIENTEDGE                  1018
#define IDC_CONTEXTHELP                 1019
#define IDC_DLGMODALFRAME               1020
#define IDC_EXOVERLAPPED                1021
#define IDC_PALETTE                     1022
#define IDC_STATICEDGE                  1023
#define IDC_TOOLWINDOW                  1024
#define IDC_WINDOWEDGE                  1025
#define IDC_ACCEPTFILES                 1026
#define IDC_APPWINDOW                   1027
#define IDC_TOPMOST                     1028
#define IDC_TRANSPARENT                 1029
#define IDC_CONTROLPARENT               1030
#define IDC_MDICHILD                    1031
#define IDC_NOPARENTNOTIFY              1032
#define IDM_WINDOW_PROPERTIES           40001
#define IDC_STATIC                      -1
#define IDI_APP                         1000

// Private message
#define PM_CREATEWINDOW   WM_APP

// Property Sheet Functions (in SHEET.C)
BOOL CreatePropertySheet (HWND hwndParent) ;

// Property Page Functions (in PAGE1.C and PAGE2.C)
UINT CALLBACK StylePageProc (HWND, UINT, LPPROPSHEETPAGE) ;
BOOL CALLBACK StyleDlgProc (HWND, UINT, WPARAM, LPARAM) ;
UINT CALLBACK ExStylePageProc (HWND, UINT, LPPROPSHEETPAGE) ;
BOOL CALLBACK ExStyleDlgProc (HWND, UINT, WPARAM, LPARAM) ;
