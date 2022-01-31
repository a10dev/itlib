/******************************************************************************
Module:  DIPS.cpp
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#include "..\CmnHdr.h"     /* See Appendix A. */
#include <WindowsX.h>
#include <tchar.h>
#include "Resource.h"
#include "..\22-DIPSLib\DIPSLib.h"


///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {

   chSETDLGICONS(hwnd, IDI_DIPS);
   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
   
   switch (id) {
      case IDC_SAVE:
      case IDC_RESTORE:
      case IDCANCEL:
         EndDialog(hwnd, id);
         break;
   }
}


///////////////////////////////////////////////////////////////////////////////


BOOL WINAPI Dlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

   switch (uMsg) {
      chHANDLE_DLGMSG(hwnd, WM_INITDIALOG, Dlg_OnInitDialog);
      chHANDLE_DLGMSG(hwnd, WM_COMMAND,    Dlg_OnCommand);
   }

   return(FALSE);
}


///////////////////////////////////////////////////////////////////////////////


int WINAPI _tWinMain(HINSTANCE hinstExe, HINSTANCE, PTSTR pszCmdLine, int) {

   // Convert command-line character to uppercase.
   CharUpperBuff(pszCmdLine, 1);
   TCHAR cWhatToDo = pszCmdLine[0];

   if ((cWhatToDo != TEXT('S')) && (cWhatToDo != TEXT('R'))) {

      // An invalid command-line argument; prompt the user.
      cWhatToDo = 0;
   }

   if (cWhatToDo == 0) {
      // No command-line argument was used to tell us what to
      // do; show usage dialog box and prompt the user.
      switch (DialogBox(hinstExe, MAKEINTRESOURCE(IDD_DIPS), NULL, Dlg_Proc)) {
         case IDC_SAVE:    
            cWhatToDo = TEXT('S'); 
            break;

         case IDC_RESTORE: 
            cWhatToDo = TEXT('R');
            break;
      }
   }

   if (cWhatToDo == 0) {
      // The user doesn't want to do anything.
      return(0);
   }
   
   // The Desktop ListView window is the grandchild of the ProgMan window.
   HWND hwndLV = GetFirstChild(GetFirstChild(
      FindWindow(TEXT("ProgMan"), NULL)));
   chASSERT(IsWindow(hwndLV));

   // Set hook that injects our DLL into the Explorer's address space. After 
   // setting the hook, the DIPS hidden modeless dialog box is created. We 
   // send messages to this window to tell it what we want it to do.
   chVERIFY(SetDIPSHook(GetWindowThreadProcessId(hwndLV, NULL)));

   // Wait for the DIPS server window to be created.
   MSG msg;
   GetMessage(&msg, NULL, 0, 0);

   // Find the handle of the hidden dialog box window.
   HWND hwndDIPS = FindWindow(NULL, TEXT("Richter DIPS"));

   // Make sure that the window was created.
   chASSERT(IsWindow(hwndDIPS));

   // Tell the DIPS window which ListView window to manipulate
   // and whether the items should be saved or restored.
   SendMessage(hwndDIPS, WM_APP, (WPARAM) hwndLV, (cWhatToDo == TEXT('S')));

   // Tell the DIPS window to destroy itself. Use SendMessage 
   // instead of PostMessage so that we know the window is 
   // destroyed before the hook is removed.
   SendMessage(hwndDIPS, WM_CLOSE, 0, 0);

   // Make sure that the window was destroyed.
   chASSERT(!IsWindow(hwndDIPS));

   // Unhook the DLL, removing the DIPS dialog box procedure 
   // from the Explorer's address space.
   SetDIPSHook(0);  

   return(0);
}


//////////////////////////////// End of File //////////////////////////////////
