/*---------------------------------------------
   CLASSFAC.CPP -- OLE Class Factory component
                   (c) Paul Yao, 1996
  ---------------------------------------------*/
#include <windows.h>
#include <initguid.h>
#include "pubmem.h"

extern int cObject ;
extern int cLockCount ;

//-------------------------------------------------------------------
DClassFactory::DClassFactory ()
     {
     RefCount = 0 ;
     }

//-------------------------------------------------------------------
DClassFactory::~DClassFactory ()
     {
     }

//-------------------------------------------------------------------
STDMETHODIMP
DClassFactory::QueryInterface (REFIID riid, LPVOID FAR *ppvObj)
     {
     // Init recipient's pointer
     *ppvObj = NULL ;

     // If asking for IUnknown, we can provide
     if (riid == IID_IUnknown)
          *ppvObj = (LPUNKNOWN) this ;

     // If asking for IClassFactory, we can provide
     if (riid == IID_IClassFactory)
          *ppvObj = (LPCLASSFACTORY) this ;

     // Make sure reference count reflects access
     if (*ppvObj == NULL)
          {
          // Interface not supported
          return E_NOINTERFACE ;
          }
     else
          {
          // Interface supported to increment reference count
          ((LPUNKNOWN) *ppvObj)->AddRef () ;
          return S_OK ;
          }
     }

//-------------------------------------------------------------------
STDMETHODIMP_ (ULONG) 
DClassFactory::AddRef ()
     {
     return ++RefCount ;
     }

//-------------------------------------------------------------------
STDMETHODIMP_ (ULONG) 
DClassFactory::Release ()
     {
     if (0L != --RefCount)
		  return RefCount ;

	 delete this ;
	 return 0L ;
     }

//-------------------------------------------------------------------
STDMETHODIMP
DClassFactory::CreateInstance (LPUNKNOWN pUnkOuter, REFIID riid,
                               LPVOID FAR *ppvObject)
     {
     // Initialize return pointer
     *ppvObject = NULL ;

     // If trying to aggregate, fail
     if (pUnkOuter != NULL) 
          return CLASS_E_NOAGGREGATION ;

     // Create memory allocation object
     LPMALLOC pMalloc = CreateAllocator () ;

     if (pMalloc == NULL)
          {
          return E_OUTOFMEMORY ;
          }
     else
          {
          // Fetch interface requested by caller
          HRESULT hr = pMalloc->QueryInterface (riid, ppvObject) ;

          // Decrement reference count produced by CreateAllocator
          pMalloc->Release () ;

          // Increment count of objects
		  if (SUCCEEDED (hr))
               ++cObject ;

          return hr ;
          }
     }

//-------------------------------------------------------------------
STDMETHODIMP
DClassFactory::LockServer (BOOL fLock)
     {
     if (fLock)
          {
          ++cLockCount ;
          }
     else
          {
          --cLockCount ;
          }
     return NOERROR ;
     }
