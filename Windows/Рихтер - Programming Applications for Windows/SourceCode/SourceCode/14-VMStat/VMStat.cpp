/******************************************************************************
Module:  VMStat.cpp
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#include "..\CmnHdr.h"     /* See Appendix A. */
#include <windowsx.h>
#include <tchar.h>
#include <stdio.h>
#include "Resource.h"


///////////////////////////////////////////////////////////////////////////////


// The update timer's ID
#define IDT_UPDATE   1


///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {

   chSETDLGICONS(hwnd, IDI_VMSTAT);

   // Set a timer so that the information updates periodically.
   SetTimer(hwnd, IDT_UPDATE, 1 * 1000, NULL);

   // Force a timer message for the initial update.
   FORWARD_WM_TIMER(hwnd, IDT_UPDATE, SendMessage);
   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnTimer(HWND hwnd, UINT id) {

   // Initialize the structure length before calling GlobalMemoryStatus.
   MEMORYSTATUS ms = { sizeof(ms) };
   GlobalMemoryStatus(&ms);

   TCHAR szData[512] = { 0 };
   _stprintf(szData, TEXT("%d\n%d\n%I64d\n%I64d\n%I64d\n%I64d\n%I64d"),
      ms.dwMemoryLoad, ms.dwTotalPhys, 
      (__int64) ms.dwAvailPhys,     (__int64) ms.dwTotalPageFile, 
      (__int64) ms.dwAvailPageFile, (__int64) ms.dwTotalVirtual, 
      (__int64) ms.dwAvailVirtual);
   SetDlgItemText(hwnd, IDC_DATA, szData);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {

   switch (id) {
      case IDCANCEL:
         KillTimer(hwnd, IDT_UPDATE);
         EndDialog(hwnd, id);
         break;
   }
}


///////////////////////////////////////////////////////////////////////////////


INT_PTR WINAPI Dlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

   switch (uMsg) {
      chHANDLE_DLGMSG(hwnd, WM_INITDIALOG, Dlg_OnInitDialog);
      chHANDLE_DLGMSG(hwnd, WM_COMMAND,    Dlg_OnCommand);
      chHANDLE_DLGMSG(hwnd, WM_TIMER,      Dlg_OnTimer);
   }
   return(FALSE);
}


///////////////////////////////////////////////////////////////////////////////


int WINAPI _tWinMain(HINSTANCE hinstExe, HINSTANCE, PTSTR pszCmdLine, int) {

   DialogBox(hinstExe, MAKEINTRESOURCE(IDD_VMSTAT), NULL, Dlg_Proc);
   return(0);
}


//////////////////////////////// End of File //////////////////////////////////
