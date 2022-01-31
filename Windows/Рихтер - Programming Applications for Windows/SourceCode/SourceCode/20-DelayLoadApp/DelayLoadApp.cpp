/******************************************************************************
Module:  DelayLoadApp.cpp
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#include "..\CmnHdr.h"     /* See Appendix A. */
#include <Windowsx.h>
#include <tchar.h>


///////////////////////////////////////////////////////////////////////////////


#include <Delayimp.h>   // For error handling & advanced features
#include "..\20-DelayLoadLib\DelayLoadLib.h"    // My DLL function prototypes


///////////////////////////////////////////////////////////////////////////////


// Statically link __delayLoadHelper/__FUnloadDelayLoadedDLL
#pragma comment(lib, "Delayimp.lib")

// Tell the linker that my DLL should be delay loaded
// Note the 2 (\") because the filename has a space in it
#pragma comment(linker, "/DelayLoad:\"20 DelayLoadLib.dll\"")

// Tell the linker that I want to be able to unload my DLL
#pragma comment(linker, "/Delay:unload")

// Tell the linker to make the delay load DLL unbindable
// You usually want this, so I commented out this line
//#pragma comment(linker, "/Delay:nobind")


// The name of the Delay-Load module (only used by this sample app)
TCHAR g_szDelayLoadModuleName[] = TEXT("20 DelayLoadLib");


///////////////////////////////////////////////////////////////////////////////


// Forward function prototype
LONG WINAPI DelayLoadDllExceptionFilter(PEXCEPTION_POINTERS pep);


///////////////////////////////////////////////////////////////////////////////


void IsModuleLoaded(PCTSTR pszModuleName) {

   HMODULE hmod = GetModuleHandle(pszModuleName);
   char sz[100];
#ifdef UNICODE
   wsprintfA(sz, "Module \"%S\" is %Sloaded.", 
      pszModuleName, (hmod == NULL) ? L"not " : L"");
#else
   wsprintfA(sz, "Module \"%s\" is %sloaded.", 
      pszModuleName, (hmod == NULL) ? "not " : "");
#endif
   chMB(sz);
}


///////////////////////////////////////////////////////////////////////////////


int WINAPI _tWinMain(HINSTANCE hinstExe, HINSTANCE, PTSTR pszCmdLine, int) {

   // Wrap all calls to delay-load DLL functions inside SEH
   __try {
      int x = 0;

      // If you're in the debugger, try the new Debug.Modules menu item to 
      // see that the DLL is not loaded prior to executing the line below
      IsModuleLoaded(g_szDelayLoadModuleName);

      x = fnLib();  // Attempt to call delay-load function

      // Use Debug.Modules to see that the DLL is now loaded
      IsModuleLoaded(g_szDelayLoadModuleName);

      x = fnLib2(); // Attempt to call delay-load function

      // Unload the delay-loaded DLL
      // NOTE: Name must exactly match /DelayLoad:(DllName)
      __FUnloadDelayLoadedDLL("20 DelayLoadLib.dll");

      // Use Debug.Modules to see that the DLL is now unloaded
      IsModuleLoaded(g_szDelayLoadModuleName);

      x = fnLib();  // Attempt to call delay-load function

      // Use Debug.Modules to see that the DLL is loaded again
      IsModuleLoaded(g_szDelayLoadModuleName);
   }
   __except (DelayLoadDllExceptionFilter(GetExceptionInformation())) {
      // Nothing to do in here, thread continues to run normally
   }

   // More code can go here...

   return(0);
}


///////////////////////////////////////////////////////////////////////////////


LONG WINAPI DelayLoadDllExceptionFilter(PEXCEPTION_POINTERS pep) {

   // Assume we recognize this exception
   LONG lDisposition = EXCEPTION_EXECUTE_HANDLER;  

   // If this is a Delay-load problem, ExceptionInformation[0] points 
   // to a DelayLoadInfo structure that has detailed error info
   PDelayLoadInfo pdli = 
      PDelayLoadInfo(pep->ExceptionRecord->ExceptionInformation[0]);

   // Create a buffer where we construct error messages
   char sz[500] = { 0 };

   switch (pep->ExceptionRecord->ExceptionCode) {
   case VcppException(ERROR_SEVERITY_ERROR, ERROR_MOD_NOT_FOUND):
      // The DLL module was not found at runtime
      wsprintfA(sz, "Dll not found: %s", pdli->szDll); 
      break;

   case VcppException(ERROR_SEVERITY_ERROR, ERROR_PROC_NOT_FOUND):
      // The DLL module was found, but it doesn't contain the function
      if (pdli->dlp.fImportByName) {
         wsprintfA(sz, "Function %s was not found in %s",
            pdli->dlp.szProcName, pdli->szDll);
      } else {
         wsprintfA(sz, "Function ordinal %d was not found in %s",
            pdli->dlp.dwOrdinal, pdli->szDll);
      }
      break; 

   default:
      // We don't recognize this exception
      lDisposition = EXCEPTION_CONTINUE_SEARCH;  
      break;
   }

   if (lDisposition == EXCEPTION_EXECUTE_HANDLER) {
      // We recognized this error and constructed a message, show it
      chMB(sz);
   }

   return(lDisposition);
}


///////////////////////////////////////////////////////////////////////////////


// Skeleton DliHook function that does nothing interesting
FARPROC WINAPI DliHook(unsigned dliNotify, PDelayLoadInfo pdli) {

   FARPROC fp = NULL;   // Default return value

   // NOTE: The members of the DelayLoadInfo structure pointed
   // to by pdli shows the results of progress made so far. 

   switch (dliNotify) {
   case dliStartProcessing:
      // Called when __delayLoadHelper attempts to find a DLL/function
      // Return 0 to have normal behavior or nonzero to override
      // everything (you will still get dliNoteEndProcessing)
      break;

   case dliNotePreLoadLibrary:
      // Called just before LoadLibrary
      // Return NULL to have __delayLoadHelper call LoadLibary
      // or you can call LoadLibrary yourself and return the HMODULE
      fp = (FARPROC) (HMODULE) NULL;
      break;

   case dliFailLoadLib:
      // Called if LoadLibrary fails
      // Again, you can call LoadLibary yourself here and return an HMODULE
      // If you return NULL, __delayLoadHelper raises the 
      // ERROR_MOD_NOT_FOUND exception
      fp = (FARPROC) (HMODULE) NULL;
      break;

   case dliNotePreGetProcAddress:
      // Called just before GetProcAddress
      // Return NULL to have __delayLoadHelper call GetProcAddress
      // or you can call GetProcAddress yourself and return the address
      fp = (FARPROC) NULL;
      break;

   case dliFailGetProc:
      // Called if GetProcAddress fails
      // You can call GetProcAddress yourself here and return an address
      // If you return NULL, __delayLoadHelper raises the 
      // ERROR_PROC_NOT_FOUND exception
      fp = (FARPROC) NULL;
      break;

   case dliNoteEndProcessing:
      // A simple notification that __delayLoadHelper is done
      // You can examine the members of the DelayLoadInfo structure
      // pointed to by pdli and raise an exception if you desire
      break;
   }

   return(fp);
}


///////////////////////////////////////////////////////////////////////////////


// Tell __delayLoadHelper to call my hook function
PfnDliHook __pfnDliNotifyHook  = DliHook;
PfnDliHook __pfnDliFailureHook = DliHook;


//////////////////////////////// End of File //////////////////////////////////
