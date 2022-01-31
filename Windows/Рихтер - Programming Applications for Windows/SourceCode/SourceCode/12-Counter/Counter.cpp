/******************************************************************************
Module:  Counter.cpp
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#include "..\CmnHdr.h"     /* See Appendix A. */
#include <WindowsX.h>
#include <tchar.h>
#include "Resource.h"


///////////////////////////////////////////////////////////////////////////////


// The possible state of the background processing
typedef enum { 
   BPS_STARTOVER,  // Start the background processing from the beginning.
   BPS_CONTINUE,   // Continue the background processing.
   BPS_DONE        // There is no background processing to do.
} BKGNDPROCSTATE;


typedef struct {
   PVOID pFiberUI;      // User interface fiber execution context
   HWND  hwnd;          // Handle of main UI window
   BKGNDPROCSTATE bps;  // State of background processing
} FIBERINFO, *PFIBERINFO;


// A global that contains application state information. This 
// global is accessed directly by the UI fiber and indirectly
// by the background processing fiber.
FIBERINFO g_FiberInfo;


///////////////////////////////////////////////////////////////////////////////


void WINAPI FiberFunc(PVOID pvParam) {

   PFIBERINFO pFiberInfo = (PFIBERINFO) pvParam;

   // Update the window showing which fiber is executing.
   SetDlgItemText(pFiberInfo->hwnd, IDC_FIBER, TEXT("Recalculation"));

   // Get the current count in the EDIT control.
   int nCount = GetDlgItemInt(pFiberInfo->hwnd, IDC_COUNT, NULL, FALSE);
   
   // Count from 0 to nCount, updating the STATIC control.
   for (int x = 0; x <= nCount; x++) {

      // UI events have higher priority than counting.
      // If there are any UI events, handle them ASAP.
      if (HIWORD(GetQueueStatus(QS_ALLEVENTS)) != 0) {

         // The UI fiber has something to do; temporarily
         // pause counting and handle the UI events.
         SwitchToFiber(pFiberInfo->pFiberUI);

         // The UI has no more events; continue counting.
         SetDlgItemText(pFiberInfo->hwnd, IDC_FIBER, TEXT("Recalculation"));
      }

      // Update the STATIC control with the most recent count.
      SetDlgItemInt(pFiberInfo->hwnd, IDC_ANSWER, x, FALSE);

      // Sleep for a while to exaggerate the effect; remove 
      // the call to Sleep in production code.
      Sleep(200);
   }

   // Indicate that counting is complete.
   pFiberInfo->bps = BPS_DONE;

   // Reschedule the UI thread. When the UI thread is running
   // and has no events to process, the thread is put to sleep.
   // NOTE: If we just allow the fiber function to return,
   // the thread and the UI fiber die -- we don't want this!
   SwitchToFiber(pFiberInfo->pFiberUI);
}


///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {

   chSETDLGICONS(hwnd, IDI_COUNTER);

   SetDlgItemInt(hwnd, IDC_COUNT, 0, FALSE);
   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {

   switch (id) {
      case IDCANCEL: 
         PostQuitMessage(0);
         break;

      case IDC_COUNT:
         if (codeNotify == EN_CHANGE) {

            // When the user changes the count, start the 
            // background processing over from the beginning.
            g_FiberInfo.bps = BPS_STARTOVER; 
         }
         break;
   }
}


///////////////////////////////////////////////////////////////////////////////


INT_PTR WINAPI Dlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

   switch (uMsg) {
      chHANDLE_DLGMSG(hwnd, WM_INITDIALOG, Dlg_OnInitDialog);
      chHANDLE_DLGMSG(hwnd, WM_COMMAND,    Dlg_OnCommand);
   }
   return(FALSE);
}


///////////////////////////////////////////////////////////////////////////////


int WINAPI _tWinMain(HINSTANCE hinstExe, HINSTANCE, PTSTR pszCmdLine, int) {

   // Counter fiber execution context
   PVOID pFiberCounter = NULL;   

   // Convert this thread to a fiber.
   g_FiberInfo.pFiberUI = ConvertThreadToFiber(NULL);

   // Create the application's UI window.
   g_FiberInfo.hwnd = CreateDialog(hinstExe, MAKEINTRESOURCE(IDD_COUNTER), 
      NULL, Dlg_Proc);

   // Update the window showing which fiber is executing.
   SetDlgItemText(g_FiberInfo.hwnd, IDC_FIBER, TEXT("User interface"));

   // Initially, there is no background processing to be done.
   g_FiberInfo.bps = BPS_DONE;

   // While the UI window still exists...
   BOOL fQuit = FALSE;
   while (!fQuit) {

      // UI messages are higher priority than background processing.
      MSG msg;
      if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {

         // If a message exists in the queue, process it.
         fQuit = (msg.message == WM_QUIT);
         if (!IsDialogMessage(g_FiberInfo.hwnd, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
         }

      } else {

         // No UI msgs exist; check the state of the background processing.
         switch (g_FiberInfo.bps) {
            case BPS_DONE:
               // No background processing to do; wait for a UI event.
               WaitMessage();
               break;

            case BPS_STARTOVER:
               // User changed the count; restart the background processing.

               if (pFiberCounter != NULL) { 
                  // A recalculation fiber exists; delete it so that
                  // background processing starts over from the beginning.
                  DeleteFiber(pFiberCounter); 
                  pFiberCounter = NULL; 
               }

               // Create a new recalc fiber that starts from the beginning.
               pFiberCounter = CreateFiber(0, FiberFunc, &g_FiberInfo);

               // The background processing started; it should continue.
               g_FiberInfo.bps = BPS_CONTINUE;

               // Fall through to BPS_CONTINUE case...

            case BPS_CONTINUE:
               // Allow the background processing to execute...
               SwitchToFiber(pFiberCounter);

               // The background processing has been paused 
               // (because a UI message showed up) or has been 
               // stopped (because the counting has completed).

               // Update the window showing which fiber is executing.
               SetDlgItemText(g_FiberInfo.hwnd, IDC_FIBER, 
                  TEXT("User interface"));

               if (g_FiberInfo.bps == BPS_DONE) { 
                  // The background processing ran to completion. Delete the
                  // fiber so that processing will restart next time.
                  DeleteFiber(pFiberCounter); 
                  pFiberCounter = NULL; 
               }
               break;
         }  // switch on background processing state

      }  // No UI messages exist
   }  // while the window still exists
   DestroyWindow(g_FiberInfo.hwnd); 

   return(0);  // End the application.
}


//////////////////////////////// End of File //////////////////////////////////
