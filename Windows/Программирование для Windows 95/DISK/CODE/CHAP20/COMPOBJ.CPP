/*----------------------------------------------
   COMPOBJ.CPP -- Component Object registration
                  (c) Paul Yao, 1996
  ----------------------------------------------*/
#include <windows.h>
#include "pubmem.h"

int cObject    = 0 ;
int cLockCount = 0 ;

//-------------------------------------------------------------------
HRESULT APIENTRY 
DllGetClassObject (REFCLSID rclsid, REFIID riid, LPVOID *ppvObj)
     {
     // Initialize "out" pointer to known value
	 *ppvObj = NULL ;

     if (rclsid != CLSID_ALLOCATOR)
          {
          return CLASS_E_CLASSNOTAVAILABLE ;
          }

	 DClassFactory *pClassFactory = new DClassFactory () ;
	 if (pClassFactory == NULL)
          {
          return E_OUTOFMEMORY ;
          }
     else
          {
          return pClassFactory->QueryInterface (riid, ppvObj) ;
	      }
     }

//-------------------------------------------------------------------
HRESULT APIENTRY 
DllCanUnloadNow (void)
     {
     if (cObject > 0 || cLockCount > 0)
          {
          return S_FALSE ;
          }
     else
          {
          return S_OK ;
          }
     }
