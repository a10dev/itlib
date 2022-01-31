/*-----------------------------------------
   TREEDATA.H -- Control Macro data definitions.
                 (c) Paul Yao, 1995
  -----------------------------------------*/
//-------------------------------------------------------------------
// ********** Animation Control Messages********** 
//-------------------------------------------------------------------
ITEM itemAnimate[] = 
    { TYPE_ITEM, NULL, "ACM_OPEN", "BOOL Animate_Open(hwnd, szName)",
      TYPE_ITEM, NULL, "ACM_PLAY", "BOOL Animate_Play(hwnd, from, to, rep)",
      TYPE_ITEM, NULL, "ACM_STOP", "BOOL Animate_Stop(hwnd)",
      TYPE_ITEM, NULL, "Close File", "BOOL Animate_Close(hwnd)",
      TYPE_ITEM, NULL, "Seek Frame", "BOOL Animate_Seek(hwnd, frame)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

//-------------------------------------------------------------------
// ********** Header Control Messages********** 
//-------------------------------------------------------------------
ITEM itemHeader[] =
    { TYPE_ITEM, NULL, "HDM_DELETEITEM", "BOOL Header_DeleteItem(hwndHD, i)",
      TYPE_ITEM, NULL, "HDM_GETITEM", "BOOL Header_GetItem(hwndHD, i, phdi)",
      TYPE_ITEM, NULL, "HDM_GETITEMCOUNT", "int Header_GetItemCount(hwndHD)",
      TYPE_ITEM, NULL, "HDM_HITTEST", "int Header_HitTest(hwndHD, phdhti)",
      TYPE_ITEM, NULL, "HDM_INSERTITEM", "int Header_InsertItem(hwndHD, i, phdi)",
      TYPE_ITEM, NULL, "HDM_LAYOUT", "BOOL Header_Layout(hwndHD, playout)",
      TYPE_ITEM, NULL, "HDM_SETITEM", "BOOL Header_SetItem(hwndHD, i, phdi)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

//-------------------------------------------------------------------
// ********** Hot Key Control Messages ********** 
//-------------------------------------------------------------------
ITEM itemHotKey[] =
    {
      TYPE_ITEM, NULL, "HKM_SETHOTKEY", "void HotKey_SetHotKey(hwnd, bVKHotKey, bfMods)",
      TYPE_ITEM, NULL, "HKM_GETHOTKEY", "DWORD HotKey_GetHotKey(hwnd)",
      TYPE_ITEM, NULL, "HKM_SETRULES", "void HotKey_SetRules(hwnd, fwCombInv, fwModInv)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

//-------------------------------------------------------------------
// ********** Image List Control Messages ********** 
//-------------------------------------------------------------------
// Create/Destroy/Save Image List
ITEM itemImageList1[] =
    {
      TYPE_ITEM, NULL, "Create", "HIMAGELIST ImageList_Create(int cx, int cy, UINT flags, int cInitial, int cGrow)",
      TYPE_ITEM, NULL, "Destroy", "BOOL ImageList_Destroy(HIMAGELIST himl)",
      TYPE_ITEM, NULL, "Create from Bitmap", "HIMAGELIST ImageList_LoadBitmap(HINSTANCE hi, LPCSTR lpbmp, int cx, int cGrow, COLORREF crMask)",
      TYPE_ITEM, NULL, "Create from Bitmap, Cursor, or Icon", "HIMAGELIST ImageList_LoadImage(HINSTANCE hi, LPCSTR lpbmp, int cx, int cGrow, COLORREF crMask, UINT uType, UINT uFlags)",
      TYPE_ITEM, NULL, "Merge 2 Images & Create New Image List", "HIMAGELIST ImageList_Merge(HIMAGELIST himl1, int i1, HIMAGELIST himl2, int i2, int dx, int dy)",
      TYPE_ITEM, NULL, "Read from a Stream", "HIMAGELIST ImageList_Read(LPSTREAM pstm)",
      TYPE_ITEM, NULL, "Write to a Stream", "BOOL ImageList_Write(HIMAGELIST himl, LPSTREAM pstm)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Modify Image List
ITEM itemImageList2[] =
    {
      TYPE_ITEM, NULL, "Add from Bitmap", "int ImageList_Add(HIMAGELIST himl, HBITMAP hbmImage, HBITMAP hbmMask)",
      TYPE_ITEM, NULL, "Add from Icon", "int ImageList_AddIcon(HIMAGELIST himl, HICON hicon)",
      TYPE_ITEM, NULL, "Add from Bitmap, masking", "int ImageList_AddMasked(HIMAGELIST himl, HBITMAP hbmImage, COLORREF crMask)",
      TYPE_ITEM, NULL, "Remove image", "BOOL ImageList_Remove(HIMAGELIST himl, int i)",
      TYPE_ITEM, NULL, "Remove all images", "BOOL ImageList_RemoveAll(HIMAGELIST himl)",
      TYPE_ITEM, NULL, "Replace image from a bitmap", "BOOL ImageList_Replace(HIMAGELIST himl, int i, HBITMAP hbmImage, HBITMAP hbmMask)",
      TYPE_ITEM, NULL, "Replace image from an icon", "int ImageList_ReplaceIcon(HIMAGELIST himl, int i, HICON hicon)",
      TYPE_ITEM, NULL, "Identify image to use as overlay", "BOOL ImageList_SetOverlayImage(HIMAGELIST himl, int iImage, int iOverlay)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Drag Images
ITEM itemImageList3[] =
    {
      TYPE_ITEM, NULL, "Create temporary dragging image list", "HIMAGELIST ImageList_GetDragImage(POINT FAR* ppt,POINT FAR* pptHotspot)",
      TYPE_ITEM, NULL, "Create cursor for dragging", "BOOL ImageList_SetDragCursorImage(HIMAGELIST himlDrag, int iDrag, int dxHotspot, int dyHotspot)",
      TYPE_ITEM, NULL, "Start drag", "BOOL ImageList_BeginDrag(HIMAGELIST himlTrack, int iTrack, int dxHotspot, int dyHotspot)",
      TYPE_ITEM, NULL, "Move drag", "BOOL ImageList_DragMove(int x, int y)",
      TYPE_ITEM, NULL, "End drag", "void ImageList_EndDrag()",
      TYPE_ITEM, NULL, "Lock window", "BOOL ImageList_DragEnter(HWND hwndLock, int x, int y)",
      TYPE_ITEM, NULL, "Unlock window", "BOOL ImageList_DragLeave(HWND hwndLock)",
      TYPE_ITEM, NULL, "Show/hide drag image", "BOOL ImageList_DragShowNolock(BOOL fShow)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Draw Images
ITEM itemImageList4[] =
    {
      TYPE_ITEM, NULL,"Simple Image Draw", "BOOL ImageList_Draw(HIMAGELIST himl, int i, HDC hdcDst, int x, int y, UINT fStyle)",
      TYPE_ITEM, NULL,"Draw with Color Blend", "BOOL ImageList_DrawEx(HIMAGELIST himl, int i, HDC hdcDst, int x, int y, int dx, int dy, COLORREF rgbBk, COLORREF rgbFg, UINT fStyle)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Create Icon/Cursor from Image List
ITEM itemImageList5[] =
    {
      TYPE_ITEM, NULL, "Create icon or cursor", "HICON ImageList_ExtractIcon(hi, himl, i)",
      TYPE_ITEM, NULL, "Create icon or cursor", "HICON ImageList_GetIcon(HIMAGELIST himl, int i, UINT flags)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Set/Query Attributes
ITEM itemImageList6[] =
    {
      TYPE_ITEM, NULL, "Query number of images", "int ImageList_GetImageCount(HIMAGELIST himl)",
      TYPE_ITEM, NULL, "Query bitmap handles & image size", "BOOL ImageList_GetImageInfo(HIMAGELIST himl, int i, IMAGEINFO FAR* pImageInfo)",
      TYPE_ITEM, NULL, "Query background color", "COLORREF ImageList_GetBkColor(HIMAGELIST himl)",
      TYPE_ITEM, NULL, "Set background color", "COLORREF ImageList_SetBkColor(HIMAGELIST himl, COLORREF clrBk)",
      TYPE_ITEM, NULL, "Query image size", "BOOL ImageList_GetIconSize(HIMAGELIST himl, int FAR *cx, int FAR *cy)",
      TYPE_ITEM, NULL, "Set size of all images & clear list", "BOOL ImageList_SetIconSize(HIMAGELIST himl, int cx, int cy)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

FOLDER fldImageList[] =
    {
      TYPE_FOLDER, NULL, "Create/Destroy/Save Image List", itemImageList1,
      TYPE_FOLDER, NULL, "Modify Image List", itemImageList2,
      TYPE_FOLDER, NULL, "Drag Images", itemImageList3,
      TYPE_FOLDER, NULL, "Draw Images", itemImageList4,
      TYPE_FOLDER, NULL, "Create Icon/Cursor from Image List", itemImageList5,
      TYPE_FOLDER, NULL, "Set / Query Attributes", itemImageList6,
      TYPE_FOLDER, LASTITEM, NULL, NULL,
    };

//-------------------------------------------------------------------
// ********** List View Control Messages ********** 
//-------------------------------------------------------------------
// Modify Item List
ITEM itemListView1[] =
    {
      TYPE_ITEM, NULL, "LVM_INSERTITEM       ", "int ListView_InsertItem(hwnd, pitem)",  
      TYPE_ITEM, NULL, "LVM_DELETEITEM       ", "BOOL ListView_DeleteItem(hwnd, i)",
      TYPE_ITEM, NULL, "LVM_DELETEALLITEMS   ", "BOOL ListView_DeleteAllItems(hwnd)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Control Attributes
ITEM itemListView2[] =
    {
      TYPE_ITEM, NULL, "LVM_GETITEMCOUNT", "int ListView_GetItemCount(hwnd)",
      TYPE_ITEM, NULL, "LVM_SETITEMCOUNT", "VOID ListView_SetItemCount(hwndLV, cItems)",
      TYPE_ITEM, NULL, "LVM_GETSELECTEDCOUNT", "UINT ListView_GetSelectedCount(hwndLV)",
      TYPE_ITEM, NULL, "LVM_GETCALLBACKMASK", "UINT ListView_GetCallbackMask(hwnd)",
      TYPE_ITEM, NULL, "LVM_SETCALLBACKMASK", "BOOL ListView_SetCallbackMask(hwnd, mask)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Drawing Attributes
ITEM itemListView3[] =
    {
      TYPE_ITEM, NULL, "LVM_GETBKCOLOR", "COLORREF ListView_GetBkColor(hwnd)",
      TYPE_ITEM, NULL, "LVM_SETBKCOLOR", "BOOL ListView_SetBkColor(hwnd, clrBk)",
      TYPE_ITEM, NULL, "LVM_GETIMAGELIST", "HIMAGELIST ListView_GetImageList(hwnd, iImageList)",
      TYPE_ITEM, NULL, "LVM_SETIMAGELIST", "HIMAGELIST ListView_SetImageList(hwnd, himl, iImageList)",
      TYPE_ITEM, NULL, "LVM_GETTEXTCOLOR", "COLORREF ListView_GetTextColor(hwnd)", 
      TYPE_ITEM, NULL, "LVM_SETTEXTCOLOR", "BOOL ListView_SetTextColor(hwnd, clrText)",
      TYPE_ITEM, NULL, "LVM_GETTEXTBKCOLOR", "COLORREF ListView_GetTextBkColor(hwnd)", 
      TYPE_ITEM, NULL, "LVM_SETTEXTBKCOLOR", "BOOL ListView_SetTextBkColor(hwnd, clrTextBk)",
      TYPE_ITEM, NULL, "LVM_GETORIGIN", "BOOL ListView_GetOrigin(hwndLV, ppt)",
      TYPE_ITEM, NULL, "LVM_GETVIEWRECT", "BOOL ListView_GetViewRect(hwnd, prc)",
      TYPE_ITEM, NULL, "LVM_GETITEMSPACING", "DWORD ListView_GetItemSpacing(hwndLV, fSmall)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Item Attributes
ITEM itemListView4[] =
    {
      TYPE_ITEM, NULL, "LVM_GETITEM          ", "BOOL ListView_GetItem(hwnd, pitem)",
      TYPE_ITEM, NULL, "LVM_SETITEM          ", "BOOL ListView_SetItem(hwnd, pitem)",
      TYPE_ITEM, NULL, "LVM_GETITEMRECT      ", "BOOL ListView_GetItemRect(hwnd, i, prc, code)",
      TYPE_ITEM, NULL, "LVM_GETITEMPOSITION  ", "BOOL ListView_GetItemPosition(hwndLV, i, ppt)",
      TYPE_ITEM, NULL, "LVM_SETITEMPOSITION  ", "BOOL ListView_SetItemPosition(hwndLV, i, x, y)",
      TYPE_ITEM, NULL, "LVM_SETITEMPOSITION32", "VOID ListView_SetItemPosition32(hwndLV, i, x, y)",
      TYPE_ITEM, NULL, "LVM_GETSTRINGWIDTH   ", "int ListView_GetStringWidth(hwndLV, psz)",
      TYPE_ITEM, NULL, "LVM_GETITEMSTATE     ", "UINT ListView_GetItemState(hwndLV, i, mask)",
      TYPE_ITEM, NULL, "LVM_SETITEMSTATE     ", "BOOL ListView_SetItemState(hwndLV, i, data, mask)",
      TYPE_ITEM, NULL, "LVM_GETITEMTEXT      ", "VOID ListView_GetItemText(hwndLV, i, iSubItem_, pszText_, cchTextMax_)",
      TYPE_ITEM, NULL, "LVM_SETITEMTEXT      ", "VOID ListView_SetItemText(hwndLV, i, iSubItem_, pszText_)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Column Attributes
ITEM itemListView5[] =
    {
      TYPE_ITEM, NULL, "LVM_GETCOLUMN        ", "BOOL ListView_GetColumn(hwnd, iCol, pcol)",
      TYPE_ITEM, NULL, "LVM_SETCOLUMN        ", "BOOL ListView_SetColumn(hwnd, iCol, pcol)",
      TYPE_ITEM, NULL, "LVM_INSERTCOLUMN     ", "int ListView_InsertColumn(hwnd, iCol, pcol)",
      TYPE_ITEM, NULL, "LVM_DELETECOLUMN     ", "BOOL ListView_DeleteColumn(hwnd, iCol)",
      TYPE_ITEM, NULL, "LVM_GETCOLUMNWIDTH   ", "int ListView_GetColumnWidth(hwnd, iCol)",
      TYPE_ITEM, NULL, "LVM_SETCOLUMNWIDTH   ", "BOOL ListView_SetColumnWidth(hwnd, iCol, cx)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Reorganize or Redraw List
ITEM itemListView6[] =
    {
      TYPE_ITEM, NULL, "LVM_REDRAWITEMS      ", "BOOL ListView_RedrawItems(hwndLV, iFirst, iLast)",
      TYPE_ITEM, NULL, "LVM_ARRANGE          ", "BOOL ListView_Arrange(hwndLV, code)",
      TYPE_ITEM, NULL, "LVM_UPDATE           ", "BOOL ListView_Update(hwndLV, i)",
      TYPE_ITEM, NULL, "LVM_SORTITEMS        ", "BOOL ListView_SortItems(hwndLV, _pfnCompare, _lPrm)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Scrolling Attributes
ITEM itemListView7[] =
    {
      TYPE_ITEM, NULL, "LVM_GETTOPINDEX", "int ListView_GetTopIndex(hwndLV)",
      TYPE_ITEM, NULL, "LVM_GETCOUNTPERPAGE", "int ListView_GetCountPerPage(hwndLV)",
      TYPE_ITEM, NULL, "LVM_ENSUREVISIBLE", "BOOL ListView_EnsureVisible(hwndLV, i, fPartialOK)",
      TYPE_ITEM, NULL, "LVM_SCROLL", "BOOL ListView_Scroll(hwndLV, dx, dy)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Search Item List
ITEM itemListView8[] =
    {
      TYPE_ITEM, NULL, "LVM_GETNEXTITEM", "int ListView_GetNextItem(hwnd, i, flags)",
      TYPE_ITEM, NULL, "LVM_FINDITEM", "int ListView_FindItem(hwnd, iStart, plvfi)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Mouse / Keyboard Input
ITEM itemListView9[] =
    {
      TYPE_ITEM, NULL, "LVM_CREATEDRAGIMAGE  ", "HIMAGELIST ListView_CreateDragImage(hwnd, i, lpptUpLeft)",
      TYPE_ITEM, NULL, "LVM_GETISEARCHSTRING ", "BOOL ListView_GetISearchString(hwndLV, lpsz)",
      TYPE_ITEM, NULL, "LVM_HITTEST          ", "int ListView_HitTest(hwndLV, pinfo)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// In-Place Label Editing
ITEM itemListView0[] =
    {
      TYPE_ITEM, NULL, "LVM_EDITLABEL        ", "HWND ListView_EditLabel(hwndLV, i)",
      TYPE_ITEM, NULL, "LVM_GETEDITCONTROL   ", "HWND ListView_GetEditControl(hwndLV)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

FOLDER fldListView[] =
    {
      TYPE_FOLDER, NULL, "Modify Item List", itemListView1,
      TYPE_FOLDER, NULL, "Control Attributes", itemListView2,
      TYPE_FOLDER, NULL, "Drawing Attributes", itemListView3,
      TYPE_FOLDER, NULL, "Item Attributes", itemListView4,
      TYPE_FOLDER, NULL, "Column Attributes", itemListView5,
      TYPE_FOLDER, NULL, "Reorganize or Redraw List", itemListView6,
      TYPE_FOLDER, NULL, "Scrolling Attributes", itemListView7,
      TYPE_FOLDER, NULL, "Search Item List", itemListView8,
      TYPE_FOLDER, NULL, "Mouse / Keyboard Input", itemListView9,
      TYPE_FOLDER, NULL, "In-Place Label Editing", itemListView0,
      TYPE_FOLDER, LASTITEM, NULL, NULL,
    };

//-------------------------------------------------------------------
ITEM itemProgress[] =
    {
      TYPE_ITEM, NULL, "PBM_SETRANGE", "DWORD Progress_SetRange(hwnd, nMinRange, nMaxRange)",
      TYPE_ITEM, NULL, "PBM_SETPOS", "int Progress_SetPos(hwnd, nNewPos)",
      TYPE_ITEM, NULL, "PBM_DELTAPOS", "int Progress_DeltaPos(hwnd, nIncrement)",
      TYPE_ITEM, NULL, "PBM_SETSTEP", "int Progress_SetStep(hwnd, nStepInc)",
      TYPE_ITEM, NULL, "PBM_STEPIT", "int Progress_StepIt(hwnd)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

//-------------------------------------------------------------------
// ********** Property Sheet Control Messages ********** 
//-------------------------------------------------------------------
// Modify Property Sheet
ITEM itemPropSheet1[] =
    {
      TYPE_ITEM, NULL, "PSM_ADDPAGE", "BOOL PropSheet_AddPage(hDlg, hpage)",
      TYPE_ITEM, NULL, "PSM_REMOVEPAGE", "VOID PropSheet_RemovePage(hDlg, index, hpage)",
      TYPE_ITEM, NULL, "PSM_SETTITLE", "VOID PropSheet_SetTitle(hDlg, wStyle, lpszText)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Property Sheet Button State
ITEM itemPropSheet2[] =
    {
      TYPE_ITEM, NULL, "PSM_APPLY", "BOOL PropSheet_Apply(hDlg)",
      TYPE_ITEM, NULL, "PSM_CHANGED", "BOOL PropSheet_Changed(hDlg, hwnd)",
      TYPE_ITEM, NULL, "PSM_UNCHANGED", "VOID PropSheet_UnChanged(hDlg, hwnd)",
      TYPE_ITEM, NULL, "PSM_CANCELTOCLOSE", "VOID PropSheet_CancelToClose(hDlg)",
      TYPE_ITEM, NULL, "PSM_PRESSBUTTON", "BOOL PropSheet_PressButton(hDlg, iButton)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Wizard Button State
ITEM itemPropSheet3[] =
    {
      TYPE_ITEM, NULL, "PSM_SETFINISHTEXT", "VOID PropSheet_SetFinishText(hDlg, lpszText)",
      TYPE_ITEM, NULL, "PSM_SETWIZBUTTONS", "VOID PropSheet_SetWizButtons(hDlg, dwFlags)",
      TYPE_ITEM, NULL, "PSM_PRESSBUTTON", "BOOL PropSheet_PressButton(hDlg, iButton)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// System Restart
ITEM itemPropSheet4[] =
    {
      TYPE_ITEM, NULL, "PSM_RESTARTWINDOWS ", "VOID PropSheet_RestartWindows(hDlg)",
      TYPE_ITEM, NULL, "PSM_REBOOTSYSTEM", "VOID PropSheet_RebootSystem(hDlg)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Query Window Handle
ITEM itemPropSheet5[] =
    {
      TYPE_ITEM, NULL, "PSM_GETTABCONTROL", "HWND PropSheet_GetTabControl(hDlg)",
      TYPE_ITEM, NULL, "PSM_GETCURRENTPAGEHWND", "HWND PropSheet_GetCurrentPageHwnd(hDlg)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Activate Specific Page
ITEM itemPropSheet6[] =
    {
      TYPE_ITEM, NULL, "PSM_SETCURSEL", "BOOL PropSheet_SetCurSel(hDlg, hpage, index)",
      TYPE_ITEM, NULL, "PSM_SETCURSELID", "BOOL PropSheet_SetCurSelByID(hDlg, id)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Forward Message to All Property Pages
ITEM itemPropSheet7[] =
    {
      TYPE_ITEM, NULL, "PSM_ISDIALOGMESSAGE", "BOOL PropSheet_IsDialogMessage(hDlg, pMsg)",
      TYPE_ITEM, NULL, "PSM_QUERYSIBLINGS", "int PropSheet_QuerySiblings(hDlg, wParam, lParam)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

FOLDER fldPropSheet[] =
    {
      TYPE_FOLDER, NULL, "Modify Property Sheet", itemPropSheet1,
      TYPE_FOLDER, NULL, "Property Sheet Button State", itemPropSheet2,
      TYPE_FOLDER, NULL, "Wizard Button State", itemPropSheet3,
      TYPE_FOLDER, NULL, "System Restart", itemPropSheet4,
      TYPE_FOLDER, NULL, "Query Window Handle", itemPropSheet5,
      TYPE_FOLDER, NULL, "Activate Specific Page", itemPropSheet6,
      TYPE_FOLDER, NULL, "Forward Message to All Property Pages", itemPropSheet7,
      TYPE_FOLDER, LASTITEM, NULL, NULL,
    };

//-------------------------------------------------------------------
// ********** Rich Edit Control Messages ********** 
//-------------------------------------------------------------------
// Read/Write Text
ITEM itemRichEdit1[] =
    {
      TYPE_ITEM, NULL, "Get Text", "int RichEdit_GetText(hwndCtl, lpch, cchMax)",
      TYPE_ITEM, NULL, "Get Text Length", " int RichEdit_GetTextLength(hwndCtl)",
      TYPE_ITEM, NULL, "Set Text", " BOOL RichEdit_SetText(hwndCtl, lpsz)",
      TYPE_ITEM, NULL, "EM_GETLINE", "int RichEdit_GetLine(hwndCtl, line, lpch, cchMax)",
      TYPE_ITEM, NULL, "EM_REPLACESEL", "void RichEdit_ReplaceSel(hwndCtl, lpszReplace)",
      TYPE_ITEM, NULL, "EM_GETTEXTRANGE","int RichEdit_GetTextRange (hwnd, lpRange) ",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Search
ITEM itemRichEdit2[] =
    {
      TYPE_ITEM, NULL, "EM_FINDTEXT","int RichEdit_FindText (hwnd, fuFlags, lpFindText) ",
      TYPE_ITEM, NULL, "EM_FINDTEXTEX","int RichEdit_FindTextEx (hwnd, fuFlags, lpFindText) ",
      TYPE_ITEM, NULL, "EM_FINDWORDBREAK ","int RichEdit_FindWordBreak (hwnd, code, ichStart) ",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Text Selection
ITEM itemRichEdit3[] =
    {
      TYPE_ITEM, NULL, "EM_GETSEL", "DWORD RichEdit_GetSel(hwndCtl)",
      TYPE_ITEM, NULL, "EM_EXGETSEL","void RichEdit_ExGetSel (hwnd, lpchr) ",
      TYPE_ITEM, NULL, "EM_SETSEL", "void RichEdit_SetSel(hwndCtl, ichStart, ichEnd)",
      TYPE_ITEM, NULL, "EM_EXSETSEL","int RichEdit_ExSetSel (hwnd, ichCharRange)",
      TYPE_ITEM, NULL, "EM_SELECTIONTYPE ","int RichEdit_SelectionType (hwnd)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Text Attributes
ITEM itemRichEdit4[] =
    {
      TYPE_ITEM, NULL, "EM_GETLIMITTEXT","int RichEdit_GetLimitText (hwnd)",
      TYPE_ITEM, NULL, "EM_LIMITTEXT", "void RichEdit_LimitText(hwndCtl, cchMax)",
      TYPE_ITEM, NULL, "EM_EXLIMITTEXT ","void RichEdit_ExLimitText (hwnd, cchTextMax) ",
      TYPE_ITEM, NULL, "EM_GETLINECOUNT", "int RichEdit_GetLineCount(hwndCtl)",
      TYPE_ITEM, NULL, "EM_GETMODIFY", "BOOL RichEdit_GetModify(hwndCtl)",
      TYPE_ITEM, NULL, "EM_SETMODIFY", "void RichEdit_SetModify(hwndCtl, fModified)",
      TYPE_ITEM, NULL, "EM_GETFIRSTVISIBLELINE", "int RichEdit_GetFirstVisibleLine(hwndCtl)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Scrolling
ITEM itemRichEdit5[] =
    {
      TYPE_ITEM, NULL, "EM_SCROLLCARET", "BOOL RichEdit_ScrollCaret(hwndCtl)",
      TYPE_ITEM, NULL, "EM_LINESCROLL", "void RichEdit_Scroll(hwndCtl, dv, dh)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Text Query
ITEM itemRichEdit6[] =
    {
      TYPE_ITEM, NULL, "EM_LINEFROMCHAR", "int RichEdit_LineFromChar(hwndCtl, ich)",
      TYPE_ITEM, NULL, "EM_EXLINEFROMCHAR","int RichEdit_ExLineFromChar (hwnd, ichCharPos)",
      TYPE_ITEM, NULL, "EM_LINEINDEX", "int RichEdit_LineIndex(hwndCtl, line)",
      TYPE_ITEM, NULL, "EM_LINELENGTH", "int RichEdit_LineLength(hwndCtl, line)",
      TYPE_ITEM, NULL, "EM_CHARFROMPOS ","DWORD RichEdit_CharFromPos (hwnd, x, y)",
      TYPE_ITEM, NULL, "EM_GETSELTEXT","int RichEdit_GetSelText (hwnd, lpBuf) ",
      TYPE_ITEM, NULL, "EM_POSFROMCHAR ","DWORD RichEdit_PosFromChar (hwnd, wCharIndex)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Edit Menu Features
ITEM itemRichEdit7[] =
    {
      TYPE_ITEM, NULL, "EM_CANUNDO", "BOOL RichEdit_CanUndo(hwndCtl)",
      TYPE_ITEM, NULL, "EM_UNDO", "BOOL RichEdit_Undo(hwndCtl)",
      TYPE_ITEM, NULL, "EM_EMPTYUNDOBUFFER", "void RichEdit_EmptyUndoBuffer(hwndCtl)",
      TYPE_ITEM, NULL, "EM_CANPASTE", "BOOL RichEdit_CanPaste (hwnd, uFormat)",
      TYPE_ITEM, NULL, "EM_PASTESPECIAL","void RichEdit_PasteSpecial (hwnd, uFormat)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Window state
ITEM itemRichEdit8[] =
    {
      TYPE_ITEM, NULL, "Enable / Disable Control", "BOOL RichEdit_Enable(hwndCtl, fEnable)",
      TYPE_ITEM, NULL, "EM_SETREADONLY", "BOOL RichEdit_SetReadOnly(hwndCtl, fReadOnly)",
      TYPE_ITEM, NULL, "EM_GETOPTIONS","UINT RichEdit_GetOptions (hwnd)",
      TYPE_ITEM, NULL, "EM_SETOPTIONS","UINT RichEdit_SetOptions (hwnd, fOperation, fOptions)",
      TYPE_ITEM, NULL, "EM_HIDESELECTION ","void RichEdit_HideSelection (hwnd, fHide, fChangeStyle)",
      TYPE_ITEM, NULL, "EM_SETBKGNDCOLOR ","COLORREF RichEdit_SetBkgndColor (hwnd, fUseSysColor, clr)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Set / Get Word Break Proc
ITEM itemRichEdit9[] =
    {
      TYPE_ITEM, NULL, "EM_SETWORDBREAKPROC", "void RichEdit_SetWordBreakProc(hwndCtl, lpfnWordBreak)",
      TYPE_ITEM, NULL, "EM_GETWORDBREAKPROC", "EDITWORDBREAKPROC RichEdit_GetWordBreakProc(hwndCtl)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Printing
ITEM itemRichEdit10[] =
    {
      TYPE_ITEM, NULL, "EM_DISPLAYBAND ","BOOL RichEdit_DisplayBand (hwnd, lprc) ",
      TYPE_ITEM, NULL, "EM_FORMATRANGE ","int RichEdit_FormatRange (hwnd, fRender, lpFmt) ",
      TYPE_ITEM, NULL, "EM_SETTARGETDEVICE ","BOOL RichEdit_SetTargetDevice (hwnd, hdcTarget, cxLineWidth)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Formatting
ITEM itemRichEdit11[] =
    {
      TYPE_ITEM, NULL, "EM_GETRECT", "void RichEdit_GetRect(hwndCtl, lprc)",
      TYPE_ITEM, NULL, "EM_SETRECT", "void RichEdit_SetRect(hwndCtl, lprc)",
      TYPE_ITEM, NULL, "EM_GETCHARFORMAT ","DWORD RichEdit_GetCharFormat (hwnd, fSelection, lpFmt)",
      TYPE_ITEM, NULL, "EM_SETCHARFORMAT ","BOOL RichEdit_SetCharFormat (hwnd, uFlags, lpFmt)",
      TYPE_ITEM, NULL, "EM_GETPARAFORMAT ","DWORD RichEdit_GetParaFormat (hwnd, lpFmt)",
      TYPE_ITEM, NULL, "EM_SETPARAFORMAT ","BOOL RichEdit_SetParaFormat (hwnd, lpFmt)",
      TYPE_ITEM, NULL, "EM_GETWORDBREAKPROCEX","EDITWORDBREAKPROCEX * RichEdit_GetWordBreakProcEx (hwnd)",
      TYPE_ITEM, NULL, "EM_SETWORDBREAKPROCEX","EDITWORDBREAKPROCEX * RichEdit_SetWordBreakProcEx (hwnd, pfnWordBreakProcEx)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Event Notification Messages
ITEM itemRichEdit12[] =
    {
      TYPE_ITEM, NULL, "EM_GETEVENTMASK","DWORD RichEdit_GetEventMask (hwnd)",
      TYPE_ITEM, NULL, "EM_SETEVENTMASK","DWORD RichEdit_SetEventMask (hwnd, dwMask)",
      TYPE_ITEM, NULL, "EM_REQUESTRESIZE ","void RichEdit_RequestResize (hwnd)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// OLE Support
ITEM itemRichEdit13[] =
    {
      TYPE_ITEM, NULL, "EM_GETOLEINTERFACE ","BOOL RichEdit_GetOleInterface (hwnd, ppObject) ",
      TYPE_ITEM, NULL, "EM_SETOLECALLBACK","BOOL RichEdit_SetOleCallback (hwnd, lpObj)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Stream Support
ITEM itemRichEdit14[] =
    {
      TYPE_ITEM, NULL, "EM_STREAMIN","int RichEdit_StreamIn (hwnd, uFormat, lpStream)",
      TYPE_ITEM, NULL, "EM_STREAMOUT ","int RichEdit_StreamOut (hwnd, uFormat, lpStream)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

FOLDER fldRichEdit[] =
    {
      TYPE_FOLDER, NULL, "Read/Write Text",itemRichEdit1,
      TYPE_FOLDER, NULL, "Search",itemRichEdit2,
      TYPE_FOLDER, NULL, "Text Selection",itemRichEdit3,
      TYPE_FOLDER, NULL, "Text Attributes",itemRichEdit4,
      TYPE_FOLDER, NULL, "Scrolling",itemRichEdit5,
      TYPE_FOLDER, NULL, "Text Query",itemRichEdit6,
      TYPE_FOLDER, NULL, "Edit Menu Features",itemRichEdit7,
      TYPE_FOLDER, NULL, "Window state",itemRichEdit8,
      TYPE_FOLDER, NULL, "Set / Get Word Break Proc",itemRichEdit9,
      TYPE_FOLDER, NULL, "Printing",itemRichEdit10,
      TYPE_FOLDER, NULL, "Formatting",itemRichEdit11,
      TYPE_FOLDER, NULL, "Event Notification Messages",itemRichEdit12,
      TYPE_FOLDER, NULL, "OLE Support",itemRichEdit13,
      TYPE_FOLDER, NULL, "Stream Support",itemRichEdit14,
      TYPE_FOLDER, LASTITEM, NULL, NULL,
    };
//-------------------------------------------------------------------
// ********** Status Bar Control Messages ********** 
//-------------------------------------------------------------------
ITEM itemStatus[] =
    {
      TYPE_ITEM, NULL, "SB_GETBORDERS", "BOOL Status_GetBorders(hwnd, aBorders)",
      TYPE_ITEM, NULL, "SB_GETPARTS", "int Status_GetParts(hwnd, nParts, aRightCoord)",
      TYPE_ITEM, NULL, "SB_SETPARTS", "BOOL Status_SetParts(hwnd, nParts, aWidths)",
      TYPE_ITEM, NULL, "SB_GETRECT", "BOOL Status_GetRect(hwnd, iPart, lprc)",
      TYPE_ITEM, NULL, "SB_GETTEXT", "DWORD Status_GetText(hwnd, iPart, szText)",
      TYPE_ITEM, NULL, "SB_SETTEXT", "BOOL Status_SetText(hwnd, iPart, uType, szText)",
      TYPE_ITEM, NULL, "SB_GETTEXTLENGTH", "DWORD Status_GetTextLength(hwnd, iPart)",
      TYPE_ITEM, NULL, "SB_SETMINHEIGHT", "void Status_SetMinHeight(hwnd, minHeight)",
      TYPE_ITEM, NULL, "SB_SIMPLE", "BOOL Status_Simple(hwnd, fSimple)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

//-------------------------------------------------------------------
// ********** Tab Control Messages ********** 
//-------------------------------------------------------------------
// Initialization
ITEM itemTab1[] =
    {
      TYPE_ITEM, NULL, "TCM_SETPADDING","void TabCtrl_SetPadding(hwnd,cx, cy)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Query
ITEM itemTab2[] =
    {
      TYPE_ITEM, NULL, "TCM_ADJUSTRECT","int TabCtrl_AdjustRect(hwnd, bLarger, prc)",
      TYPE_ITEM, NULL, "TCM_GETITEMCOUNT","int TabCtrl_GetItemCount(hwnd)",
      TYPE_ITEM, NULL, "TCM_GETITEMRECT","BOOL TabCtrl_GetItemRect(hwnd, i, prc)",
      TYPE_ITEM, NULL, "TCM_HITTEST","int TabCtrl_HitTest(hwndTC, pinfo)",
      TYPE_ITEM, NULL, "TCM_GETROWCOUNT","int TabCtrl_GetRowCount(hwnd)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Modify Tab List
ITEM itemTab3[] =
    {
      TYPE_ITEM, NULL, "TCM_GETITEM","BOOL TabCtrl_GetItem(hwnd, iItem, pitem)",
      TYPE_ITEM, NULL, "TCM_SETITEM","BOOL TabCtrl_SetItem(hwnd, iItem, pitem)",
      TYPE_ITEM, NULL, "TCM_INSERTITEM","int TabCtrl_InsertItem(hwnd, iItem, pitem)",
      TYPE_ITEM, NULL, "TCM_DELETEITEM","BOOL TabCtrl_DeleteItem(hwnd, i)",
      TYPE_ITEM, NULL, "TCM_DELETEALLITEMS","BOOL TabCtrl_DeleteAllItems(hwnd)",
      TYPE_ITEM, NULL, "TCM_SETITEMEXTRA","BOOL TabCtrl_SetItemExtra(hwndTC, cb)",
      TYPE_ITEM, NULL, "TCM_SETITEMSIZE","DWORD TabCtrl_SetItemSize(hwnd, x, y)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Image List and Tool Tips
ITEM itemTab4[] =
    {       
      TYPE_ITEM, NULL, "TCM_GETIMAGELIST","HIMAGELIST TabCtrl_GetImageList(hwnd)",
      TYPE_ITEM, NULL, "TCM_SETIMAGELIST","HIMAGELIST TabCtrl_SetImageList(hwnd, himl)",
      TYPE_ITEM, NULL, "TCM_REMOVEIMAGE","void TabCtrl_RemoveImage(hwnd, i)",
      TYPE_ITEM, NULL, "TCM_GETTOOLTIPS","HWND TabCtrl_GetToolTips(hwnd)",
      TYPE_ITEM, NULL, "TCM_SETTOOLTIPS","void TabCtrl_SetToolTips(hwnd, hwndTT)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Selection and Focus
ITEM itemTab5[] =
    {
      TYPE_ITEM, NULL, "TCM_GETCURSEL","int TabCtrl_GetCurSel(hwnd)",
      TYPE_ITEM, NULL, "TCM_SETCURSEL","int TabCtrl_SetCurSel(hwnd, i)",
      TYPE_ITEM, NULL, "TCM_GETCURFOCUS","int TabCtrl_GetCurFocus(hwnd)",
      TYPE_ITEM, NULL, "TCM_SETCURFOCUS","void TabCtrl_SetCurFocus(hwnd, i)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

FOLDER fldTab[] =
    {
      TYPE_FOLDER, NULL, "Initialization",itemTab1,
      TYPE_FOLDER, NULL, "Query",itemTab2,
      TYPE_FOLDER, NULL, "Modify Tab List",itemTab3,
      TYPE_FOLDER, NULL, "Image List and Tool Tips",itemTab4,
      TYPE_FOLDER, NULL, "Selection and Focus",itemTab5,
      TYPE_FOLDER, LASTITEM, NULL, NULL,
    };

//-------------------------------------------------------------------
// ********** Tool Bar Control Messages ********** 
//-------------------------------------------------------------------
// Initialization
ITEM itemToolBar1[] =
    {
      TYPE_ITEM, NULL, "TB_BUTTONSTRUCTSIZE ", "void ToolBar_ButtonStructSize(hwnd)",
      TYPE_ITEM, NULL, "TB_ADDBITMAP", "int ToolBar_AddBitmap(hwnd, nButtons, lptbab)",
      TYPE_ITEM, NULL, "TB_ADDSTRING", "int ToolBar_AddString(hwnd, hinst, idString)",
      TYPE_ITEM, NULL, "TB_GETBITMAPFLAGS ", "int ToolBar_GetBitmapFlags(hwnd)",
      TYPE_ITEM, NULL, "TB_SETBITMAPSIZE", "BOOL ToolBar_SetBitmapSize(hwnd, dxBitmap, dyBitmap)",
      TYPE_ITEM, NULL, "TB_GETITEMRECT", "BOOL ToolBar_GetItemRect(hwnd, idButton, lprc)",
      TYPE_ITEM, NULL, "TB_SETBUTTONSIZE", "BOOL ToolBar_SetButtonSize(hwnd, dxBitmap, dyBitmap)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Handle WM_SIZE Message
ITEM itemToolBar2[] =
    {
      TYPE_ITEM, NULL, "TB_AUTOSIZE ", "void ToolBar_AutoSize(hwnd)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Modify Button List
ITEM itemToolBar3[] =
    {
      TYPE_ITEM, NULL, "TB_ADDBUTTONS ", "BOOL ToolBar_AddButtons(hwnd, uNumButtons, lpButtons)",
      TYPE_ITEM, NULL, "TB_DELETEBUTTON ", "BOOL ToolBar_DeleteButton(hwnd, idButton)",
      TYPE_ITEM, NULL, "TB_INSERTBUTTON ", "BOOL ToolBar_InsertButton(hwnd, idButton, lpButton)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Button State
ITEM itemToolBar4[] =
    {           
      TYPE_ITEM, NULL, "TB_GETSTATE ", "int ToolBar_GetState(hwnd, idButton)",
      TYPE_ITEM, NULL, "TB_SETSTATE ", "BOOL ToolBar_SetState(hwnd, idButton, fState)",
      TYPE_ITEM, NULL, "TB_ISBUTTONCHECKED", "int ToolBar_IsButtonChecked(hwnd, idButton)",
      TYPE_ITEM, NULL, "TB_CHECKBUTTON", "BOOL ToolBar_CheckButton(hwnd, idButton, fCheck )",
      TYPE_ITEM, NULL, "TB_ISBUTTONENABLED", "int ToolBar_IsButtonEnabled(hwnd, idButton)",
      TYPE_ITEM, NULL, "TB_ENABLEBUTTON ", "BOOL ToolBar_EnableButton(hwnd, idButton, fEnable )",
      TYPE_ITEM, NULL, "TB_ISBUTTONHIDDEN ", "int ToolBar_IsButtonHidden(hwnd, idButton)",
      TYPE_ITEM, NULL, "TB_HIDEBUTTON ", "BOOL ToolBar_HideButton(hwnd, idButton, fShow)",
      TYPE_ITEM, NULL, "TB_ISBUTTONINDETERMINATE", "int ToolBar_IsButtonIndeterminate(hwnd, idButton)",
      TYPE_ITEM, NULL, "TB_INDETERMINATE", "BOOL ToolBar_Indeterminate(hwnd, idButton, fIndeterminate)",
      TYPE_ITEM, NULL, "TB_ISBUTTONPRESSED", "int ToolBar_IsButtonPressed(hwnd, idButton)",
      TYPE_ITEM, NULL, "TB_PRESSBUTTON", "BOOL ToolBar_PressButton(hwnd, idButton, fPress)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Button Attributes
ITEM itemToolBar5[] =
    {
      TYPE_ITEM, NULL, "TB_CHANGEBITMAP ", "BOOL ToolBar_ChangeBitmap(hwnd, idButton, iBitmap)",
      TYPE_ITEM, NULL, "TB_SETCMDID ", "BOOL ToolBar_SetCmdID(hwnd, index, cmdId)",
      TYPE_ITEM, NULL, "TB_COMMANDTOINDEX ", "int ToolBar_CommandToIndex(hwnd, idButton)",
      TYPE_ITEM, NULL, "TB_GETBITMAP", "int ToolBar_GetBitmap(hwnd, idButton)",
      TYPE_ITEM, NULL, "TB_GETBUTTON", "BOOL ToolBar_GetButton(hwnd, idButton, lpButton)",
      TYPE_ITEM, NULL, "TB_GETBUTTONTEXT", "int ToolBar_GetButtonText(hwnd, idButton, lpszText)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Tool Bar Attributes
ITEM itemToolBar6[] =
    {
      TYPE_ITEM, NULL, "TB_BUTTONCOUNT", "int ToolBar_ButtonCount(hwnd)",
      TYPE_ITEM, NULL, "TB_SETPARENT", "void ToolBar_SetParent(hwnd, hwndParent)",
      TYPE_ITEM, NULL, "TB_GETROWS", "int ToolBar_GetRows(hwnd)",
      TYPE_ITEM, NULL, "TB_SETROWS", "void ToolBar_SetRows(hwnd, cRows, fLarger, lprc)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Display Customize Dialog Box
ITEM itemToolBar7[] =
    {
      TYPE_ITEM, NULL, "TB_CUSTOMIZE", "void ToolBar_Customize(hwnd)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Tool Tip Support
ITEM itemToolBar8[] =
    {
      TYPE_ITEM, NULL, "TB_GETTOOLTIPS", "HWND ToolBar_GetToolTips(hwnd)",
      TYPE_ITEM, NULL, "TB_SETTOOLTIPS", "void ToolBar_SetToolTips(hwnd)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Read / Write Registry Data
ITEM itemToolBar9[] =
    {
      TYPE_ITEM, NULL, "TB_SAVERESTORE", "void ToolBar_SaveRestore(hwnd, fSave, ptbsp)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

FOLDER fldToolBar[] =
    {
      TYPE_FOLDER, NULL, "Initialization",itemToolBar1,
      TYPE_FOLDER, NULL, "Handle WM_SIZE Message",itemToolBar2,
      TYPE_FOLDER, NULL, "Modify Button List",itemToolBar3,
      TYPE_FOLDER, NULL, "Button State",itemToolBar4,
      TYPE_FOLDER, NULL, "Button Attributes",itemToolBar5,
      TYPE_FOLDER, NULL, "Tool Bar Attributes",itemToolBar6,
      TYPE_FOLDER, NULL, "Display Customize Dialog Box",itemToolBar7,
      TYPE_FOLDER, NULL, "Tool Tip Support",itemToolBar8,
      TYPE_FOLDER, NULL, "Read / Write Registry Data",itemToolBar9,
      TYPE_FOLDER, LASTITEM, NULL, NULL,
    };

//-------------------------------------------------------------------
// ********** Tool Tip Control Messages ********** 
//-------------------------------------------------------------------
// Modify Tool List
ITEM itemToolTip1[] =
    {
      TYPE_ITEM, NULL, "TTM_ADDTOOL", "BOOL ToolTip_AddTool (hwnd, lpti)",
      TYPE_ITEM, NULL, "TTM_DELTOOL", "void ToolTip_DelTool (hwnd, lpti)",
      TYPE_ITEM, NULL, "TTM_NEWTOOLRECT", "void ToolTip_NewToolRect (hwnd, lpti)",
      TYPE_ITEM, NULL, "TTM_SETTOOLINFO", "void ToolTip_SetToolInfo (hwnd, lpti)",
      TYPE_ITEM, NULL, "TTM_UPDATETIPTEXT", "void ToolTip_UpdateTipText (hwnd, lpti)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Query
ITEM itemToolTip2[] =
    {
      TYPE_ITEM, NULL, "TTM_ENUMTOOLS", "BOOL ToolTip_EnumTools (hwnd, iTool, lpti)",
      TYPE_ITEM, NULL, "TTM_GETCURRENTTOOL", "BOOL ToolTip_GetCurrentTool (hwnd, lpti)",
      TYPE_ITEM, NULL, "TTM_GETTEXT", "void ToolTip_GetText (hwnd, lpti)",
      TYPE_ITEM, NULL, "TTM_GETTOOLCOUNT", "int  ToolTip_GetToolCount (hwnd)",
      TYPE_ITEM, NULL, "TTM_GETTOOLINFO", "BOOL ToolTip_GetToolInfo (hwnd, lpti)",
      TYPE_ITEM, NULL, "TTM_HITTEST", "BOOL ToolTip_HitText (hwnd, lphti)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Set Tooltip Window State
ITEM itemToolTip3[] =
    {
      TYPE_ITEM, NULL, "TTM_ACTIVATE", "void ToolTip_Activate (hwnd, fActivate)",
      TYPE_ITEM, NULL, "TTM_SETDELAYTIME", "void ToolTip_SetDelayTime (hwnd, uFlag, iDelay)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Pass Mouse Message
ITEM itemToolTip4[] =
    {
      TYPE_ITEM, NULL, "TTM_RELAYEVENT", "void ToolTip_RelayEvent (hwnd, lpmsg)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Support Subclassing of Tooltips
ITEM itemToolTip5[] =
    {
      TYPE_ITEM, NULL, "TTM_WINDOWFROMPOINT", "HWND ToolTip_WindowFromPoint (hwnd, lppt)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

FOLDER fldToolTip[] =
    {
      TYPE_FOLDER, NULL, "Modify Tool List", itemToolTip1,
      TYPE_FOLDER, NULL, "Query", itemToolTip2,
      TYPE_FOLDER, NULL, "Set Tooltip Window State", itemToolTip3,
      TYPE_FOLDER, NULL, "Pass Mouse Message", itemToolTip4,
      TYPE_FOLDER, NULL, "Support Subclassing of Tooltips", itemToolTip5,
      TYPE_FOLDER, LASTITEM, NULL, NULL,
    };

//-------------------------------------------------------------------
// ********** Track Bar Control Messages ********** 
//-------------------------------------------------------------------
// Trackbar Ticks
ITEM itemTrackBar1[] =
    {
      TYPE_ITEM, NULL, "TBM_CLEARTICS     ", "void   TrackBar_ClearTics (hwnd, fRedraw)",
      TYPE_ITEM, NULL, "TBM_GETNUMTICS    ", "LONG   TrackBar_GetNumTics (hwnd)",
      TYPE_ITEM, NULL, "TBM_GETPTICS      ", "LPLONG TrackBar_GetPTics (hwnd)",
      TYPE_ITEM, NULL, "TBM_GETTIC        ", "LONG   TrackBar_GetTic (hwnd, iTic)",
      TYPE_ITEM, NULL, "TBM_SETTIC        ", "BOOL   TrackBar_SetTic (hwnd, lPosition)",
      TYPE_ITEM, NULL, "TBM_GETTICPOS     ", "LONG   TrackBar_GetTicPos (hwnd, iTic)",
      TYPE_ITEM, NULL, "TBM_SETTICFREQ    ", "void   TrackBar_SetTicFreq (hwnd, wFreq, lPosition)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Line and Page Increments
ITEM itemTrackBar2[] =
    {
      TYPE_ITEM, NULL, "TBM_GETLINESIZE   ", "LONG   TrackBar_GetLineSize (hwnd)",
      TYPE_ITEM, NULL, "TBM_SETLINESIZE   ", "LONG   TrackBar_SetLineSize (hwnd, lLineSize)",
      TYPE_ITEM, NULL, "TBM_GETPAGESIZE   ", "LONG   TrackBar_GetPageSize (hwnd)",
      TYPE_ITEM, NULL, "TBM_SETPAGESIZE   ", "LONG   TrackBar_SetPageSize (hwnd, lPageSize)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Slider Position
ITEM itemTrackBar3[] =
    {
      TYPE_ITEM, NULL, "TBM_GETPOS        ", "LONG   TrackBar_GetPos (hwnd)",
      TYPE_ITEM, NULL, "TBM_SETPOS        ", "void   TrackBar_SetPos (hwnd, bPosition, lPosition)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Slider Range
ITEM itemTrackBar4[] =
    {
      TYPE_ITEM, NULL, "TBM_SETRANGE      ", "void   TrackBar_SetRange (hwnd, bRedraw, lMinimum, lMaximum)",
      TYPE_ITEM, NULL, "TBM_GETRANGEMAX   ", "LONG   TrackBar_GetRangeMax (hwnd)",
      TYPE_ITEM, NULL, "TBM_SETRANGEMAX   ", "void   TrackBar_SetRangeMax (hwnd, bRedraw, lMaximum)",
      TYPE_ITEM, NULL, "TBM_GETRANGEMIN   ", "LONG   TrackBar_GetRangeMin (hwnd)",
      TYPE_ITEM, NULL, "TBM_SETRANGEMIN   ", "void   TrackBar_SetRangeMin (hwnd, bRedraw, lMinimum)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Trackbar Selection
ITEM itemTrackBar5[] =
    {
      TYPE_ITEM, NULL, "TBM_CLEARSEL      ", "void   TrackBar_ClearSel (hwnd, fRedraw)",
      TYPE_ITEM, NULL, "TBM_GETSELEND     ", "LONG   TrackBar_GetSelEnd (hwnd)",
      TYPE_ITEM, NULL, "TBM_SETSELEND     ", "void   TrackBar_SetSelEnd (hwnd, bRedraw, lEnd)",
      TYPE_ITEM, NULL, "TBM_GETSELSTART   ", "LONG   TrackBar_GetSelStart (hwnd)",
      TYPE_ITEM, NULL, "TBM_SETSELSTART   ", "void   TrackBar_SetSelStart (hwnd, bRedraw, lStart)",
      TYPE_ITEM, NULL, "TBM_SETSEL        ", "void   TrackBar_SetSel (hwnd, bRedraw, lMinimum, lMaximum)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Trackbar Appearance
ITEM itemTrackBar6[] =
    {
      TYPE_ITEM, NULL, "TBM_GETCHANNELRECT", "void   TrackBar_GetChannelRect (hwnd, lprc)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Slider (Thumb) Attributes
ITEM itemTrackBar7[] =
    {
      TYPE_ITEM, NULL, "TBM_GETTHUMBLENGTH", "UINT   TrackBar_GetThumbLength (hwnd)",
      TYPE_ITEM, NULL, "TBM_SETTHUMBLENGTH", "void   TrackBar_SetThumbLength (hwnd, iLength)",
      TYPE_ITEM, NULL, "TBM_GETTHUMBRECT  ", "void   TrackBar_GetThumbRect (hwnd, lprc)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

FOLDER fldTrackBar[] =
    {
      TYPE_FOLDER, NULL, "Trackbar Ticks", itemTrackBar1,
      TYPE_FOLDER, NULL, "Line and Page Increments", itemTrackBar2,
      TYPE_FOLDER, NULL, "Slider Position", itemTrackBar3,
      TYPE_FOLDER, NULL, "Slider Range", itemTrackBar4,
      TYPE_FOLDER, NULL, "Trackbar Selection", itemTrackBar5,
      TYPE_FOLDER, NULL, "Trackbar Appearance", itemTrackBar6,
      TYPE_FOLDER, NULL, "Slider (Thumb) Attributes", itemTrackBar7,
      TYPE_FOLDER, LASTITEM, NULL, NULL,
    };
//-------------------------------------------------------------------
// ********** Tree View Control Messages ********** 
//-------------------------------------------------------------------
// Modify List
ITEM itemTreeView1[] =
    {
      TYPE_ITEM, NULL, "TVM_INSERTITEM      ", "HTREEITEM TreeView_InsertItem(hwnd, lpis)",
      TYPE_ITEM, NULL, "TVM_DELETEITEM      ", "BOOL TreeView_DeleteItem(hwnd, hitem)",
      TYPE_ITEM, NULL, "Delete All Items    ", "BOOL TreeView_DeleteAllItems(hwnd)",
      TYPE_ITEM, NULL, "TVM_GETITEM         ", "BOOL TreeView_GetItem(hwnd, pitem)",
      TYPE_ITEM, NULL, "TVM_SETITEM         ", "BOOL TreeView_SetItem(hwnd, pitem)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Item Layout, Display, and Order
ITEM itemTreeView2[] =
    {
      TYPE_ITEM, NULL, "TVM_ENSUREVISIBLE   ", "BOOL TreeView_EnsureVisible(hwnd, hitem)",
      TYPE_ITEM, NULL, "TVM_EXPAND          ", "BOOL TreeView_Expand(hwnd, hitem, code)",
      TYPE_ITEM, NULL, "TVM_GETINDENT       ", "UINT TreeView_GetIndent(hwnd)",
      TYPE_ITEM, NULL, "TVM_SETINDENT       ", "BOOL TreeView_SetIndent(hwnd, indent)",
      TYPE_ITEM, NULL, "TVM_SORTCHILDREN    ", "BOOL TreeView_SortChildren(hwnd, hitem, recurse)",
      TYPE_ITEM, NULL, "TVM_SORTCHILDRENCB  ", "BOOL TreeView_SortChildrenCB(hwnd, psort, recurse)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Query
ITEM itemTreeView3[] =
    {
      TYPE_ITEM, NULL, "TVM_GETITEMRECT     ", "BOOL TreeView_GetItemRect(hwnd, hitem, prc, code)",
      TYPE_ITEM, NULL, "TVM_GETCOUNT        ", "UINT TreeView_GetCount(hwnd)",
      TYPE_ITEM, NULL, "TVM_GETVISIBLECOUNT ", "UINT TreeView_GetVisibleCount(hwnd)",
      TYPE_ITEM, NULL, "TVM_HITTEST         ", "HTREEITEM TreeView_HitTest(hwnd, lpht)",
      TYPE_ITEM, NULL, "TVM_GETISEARCHSTRING", "BOOL TreeView_GetISearchString(hwndTV, lpsz)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Image List
ITEM itemTreeView4[] =
    {            
      TYPE_ITEM, NULL, "TVM_GETIMAGELIST    ", "HIMAGELIST TreeView_GetImageList(hwnd, iImage)",
      TYPE_ITEM, NULL, "TVM_SETIMAGELIST    ", "HIMAGELIST TreeView_SetImageList(hwnd, himl, iImage)",
      TYPE_ITEM, NULL, "TVM_CREATEDRAGIMAGE ", "HIMAGELIST TreeView_CreateDragImage(hwnd, hitem)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// In-Place Label Edit
ITEM itemTreeView5[] =
    {
      TYPE_ITEM, NULL, "TVM_EDITLABEL       ", "HWND TreeView_EditLabel(hwnd, hitem)",
      TYPE_ITEM, NULL, "TVM_GETEDITCONTROL  ", "HWND TreeView_GetEditControl(hwnd)",
      TYPE_ITEM, NULL, "TVM_ENDEDITLABELNOW ", "BOOL TreeView_EndEditLabelNow(hwnd, fCancel)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Enumerate Items
ITEM itemTreeView6[] =
    {
      TYPE_ITEM, NULL, "Get Root", "HTREEITEM TreeView_GetRoot(hwnd)",
      TYPE_ITEM, NULL, "Get Next Child", "HTREEITEM TreeView_GetChild(hwnd, hitem)",
      TYPE_ITEM, NULL, "Get Next Sibling", "HTREEITEM TreeView_GetNextSibling(hwnd, hitem)",
      TYPE_ITEM, NULL, "Get Previous Sibling ", "HTREEITEM TreeView_GetPrevSibling(hwnd, hitem)",
      TYPE_ITEM, NULL, "Get Parent ", "HTREEITEM TreeView_GetParent(hwnd, hitem)",
      TYPE_ITEM, NULL, "Get First Visible ", "HTREEITEM TreeView_GetFirstVisible(hwnd)",
      TYPE_ITEM, NULL, "Get Next Visible", "HTREEITEM TreeView_GetNextVisible(hwnd, hitem)",
      TYPE_ITEM, NULL, "Get Previous Visible", "HTREEITEM TreeView_GetPrevVisible(hwnd, hitem)",
      TYPE_ITEM, NULL, "Get Selection", "HTREEITEM TreeView_GetSelection(hwnd)",
      TYPE_ITEM, NULL, "Get Drop Highlight", "HTREEITEM TreeView_GetDropHilight(hwnd)",
      TYPE_ITEM, NULL, "TVM_GETNEXTITEM", "HTREEITEM TreeView_GetNextItem(hwnd, hitem, code)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

// Select Items
ITEM itemTreeView7[] =
    {
      TYPE_ITEM, NULL, "Simple Select", "HTREEITEM TreeView_SelectItem(hwnd, hitem)",
      TYPE_ITEM, NULL, "Drop Target Select", "HTREEITEM TreeView_SelectDropTarget(hwnd, hitem)",
      TYPE_ITEM, NULL, "Select and Scroll", "HTREEITEM TreeView_SelectSetFirstVisible(hwnd, hitem)",
      TYPE_ITEM, NULL, "TVM_SELECTITEM", "HTREEITEM TreeView_Select(hwnd, hitem, code)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };

FOLDER fldTreeView[] =
    {
      TYPE_FOLDER, NULL, "Modify List", itemTreeView1,
      TYPE_FOLDER, NULL, "Item Layout, Display, and Order", itemTreeView2,
      TYPE_FOLDER, NULL, "Query", itemTreeView3,
      TYPE_FOLDER, NULL, "Image List", itemTreeView4,
      TYPE_FOLDER, NULL, "In-Place Label Edit", itemTreeView5,
      TYPE_FOLDER, NULL, "Enumerate Items", itemTreeView6,
      TYPE_FOLDER, NULL, "Select Items", itemTreeView7,
      TYPE_FOLDER, LASTITEM, NULL, NULL,
    };
//-------------------------------------------------------------------
// ********** Up/Down Control Messages ********** 
//-------------------------------------------------------------------
ITEM itemUpDown[] =
    {
      TYPE_ITEM, NULL, "UDM_GETACCEL", "int   UpDown_GetAccel (hwnd, cAccels, paAccels)",
      TYPE_ITEM, NULL, "UDM_SETACCEL", "BOOL  UpDown_SetAccel (hwnd, nAccels, aAccels)",
      TYPE_ITEM, NULL, "UDM_GETBASE ", "int   UpDown_GetBase (hwnd)",
      TYPE_ITEM, NULL, "UDM_SETBASE ", "int   UpDown_SetBase (hwnd, nBase)",
      TYPE_ITEM, NULL, "UDM_GETBUDDY", "HWND  UpDown_GetBuddy (hwnd)",
      TYPE_ITEM, NULL, "UDM_SETBUDDY", "HWND  UpDown_SetBuddy (hwnd, hwndBuddy)",
      TYPE_ITEM, NULL, "UDM_GETPOS  ", "DWORD UpDown_GetPos (hwnd)",
      TYPE_ITEM, NULL, "UDM_SETPOS  ", "short UpDown_SetPos (hwnd, nPos)",
      TYPE_ITEM, NULL, "UDM_GETRANGE", "DWORD UpDown_GetRange (hwnd)",
      TYPE_ITEM, NULL, "UDM_SETRANGE", "void  UpDown_SetRange (hwnd, nUpper, nLower)",
      TYPE_ITEM, LASTITEM, NULL, NULL,
    };
//-------------------------------------------------------------------
//-------------------------------------------------------------------
FOLDER fldRoot[] =
    { TYPE_FOLDER, NULL, "Animation Controls", itemAnimate,
      TYPE_FOLDER, NULL, "Header Controls",    itemHeader,
      TYPE_FOLDER, NULL, "Hot-Key Controls", itemHotKey,
      TYPE_FOLDER, NULL, "Image Lists", (LPITEM)fldImageList,
      TYPE_FOLDER, NULL, "List View Controls", (LPITEM)fldListView,
      TYPE_FOLDER, NULL, "Progress Bars", itemProgress,
      TYPE_FOLDER, NULL, "Property Sheets", (LPITEM)fldPropSheet,
      TYPE_FOLDER, NULL, "Rich Edit Controls", (LPITEM)fldRichEdit,
      TYPE_FOLDER, NULL, "Status Bar Controls", itemStatus,
      TYPE_FOLDER, NULL, "Tab Controls", (LPITEM)fldTab,
      TYPE_FOLDER, NULL, "Toolbars", (LPITEM)fldToolBar,
      TYPE_FOLDER, NULL, "Tooltip Controls", (LPITEM)fldToolTip,
      TYPE_FOLDER, NULL, "Trackbars", (LPITEM)fldTrackBar,
      TYPE_FOLDER, NULL, "Tree View Controls", (LPITEM)fldTreeView,
      TYPE_FOLDER, NULL, "Up-Down Controls", itemUpDown,
    };

int cRootFolders = sizeof(fldRoot) / sizeof(FOLDER);
