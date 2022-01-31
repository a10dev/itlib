/*-------------------------------------------------
   PUBMEM.CPP -- Define a public imalloc component
                 (c) Paul Yao, 1996
  -------------------------------------------------*/
#include <windows.h>
#include "pubmem.h"

extern int cObject ;

//-------------------------------------------------------------------
// CreateAllocator -- Exported function to create allocator
//-------------------------------------------------------------------
EXPORT LPMALLOC CreateAllocator ()
     {
     DAlloc *pAllocator = NULL ;

     pAllocator = new DAlloc () ;
     if (pAllocator != NULL && pAllocator->Initialize ())
          {
          pAllocator->AddRef () ;
          }
     else
          {
          delete pAllocator ;
          }

     return (LPMALLOC) pAllocator ;
     }

//-------------------------------------------------------------------
DAlloc::DAlloc ()
     {
     RefCount = 0 ;
     hHeap = NULL ;
     }

//-------------------------------------------------------------------
DAlloc::~DAlloc ()
     {
     if (hHeap)
          HeapDestroy (hHeap) ;
     }

//-------------------------------------------------------------------
BOOL DAlloc::Initialize ()
     {
     hHeap = HeapCreate (0, 4096, 65535) ;
     return (BOOL) hHeap ;
     }

//-------------------------------------------------------------------
STDMETHODIMP
DAlloc::QueryInterface (REFIID riid, LPVOID FAR *ppvObject)
     {
     // Always initialize "out" parameters to NULL
     *ppvObject = NULL ;

     // Everyone supports IUnknown
     if (riid == IID_IUnknown)
          *ppvObject = (LPUNKNOWN) this ;

     // We support IMalloc
     if (riid == IID_IMalloc)
          *ppvObject = (LPMALLOC) this ;

     if (*ppvObject == NULL)
          {
          // Interface not supported
          return E_NOINTERFACE ;
          }
     else
          {
          // Interface supported, so increment reference count
          ((LPUNKNOWN) *ppvObject)->AddRef () ;
          return S_OK ;
          }
     }

//-------------------------------------------------------------------
STDMETHODIMP_ (ULONG)
DAlloc::AddRef (void)
     {
     return ++RefCount ;
     }

//-------------------------------------------------------------------
STDMETHODIMP_ (ULONG)
DAlloc::Release (void)
     {
	 if (0L != --RefCount)
		  return RefCount ;

	 --cObject ;
     delete this ;
	 return 0L ;
     }

//-------------------------------------------------------------------
STDMETHODIMP_ (void *)
DAlloc::Alloc (ULONG cb)
     {
     return HeapAlloc (hHeap, HEAP_ZERO_MEMORY, cb) ;
     }

//-------------------------------------------------------------------
STDMETHODIMP_ (void *)
DAlloc::Realloc (void *pv, ULONG cb)
     {
     return HeapReAlloc (hHeap, HEAP_ZERO_MEMORY, pv, cb) ;
     }

//-------------------------------------------------------------------
STDMETHODIMP_ (void)
DAlloc::Free (void *pv)
     {
     HeapFree (hHeap, 0, pv) ;
     }

//-------------------------------------------------------------------
STDMETHODIMP_ (ULONG)
DAlloc::GetSize (void *pv)
     {
     return HeapSize (hHeap, 0, pv) ;
     }

//-------------------------------------------------------------------
STDMETHODIMP_ (int)
DAlloc::DidAlloc (void *pv)
     {
     PROCESS_HEAP_ENTRY phe ;
     ZeroMemory (&phe, sizeof (PROCESS_HEAP_ENTRY)) ;

     while (HeapWalk (hHeap, &phe))
          {
          if (phe.lpData == pv)
               return 1 ;
          }

     return 0 ;
     }

//-------------------------------------------------------------------
STDMETHODIMP_ (void)
DAlloc::HeapMinimize (void)
     {
     HeapCompact (hHeap, 0) ;
     }
