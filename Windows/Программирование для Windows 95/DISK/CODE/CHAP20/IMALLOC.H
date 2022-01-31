//-------------------------------------------------------------------
// C Interface to private allocator
//-------------------------------------------------------------------
#define EXPORT extern "C" __declspec (dllexport)

EXPORT LPMALLOC CreateAllocator () ;

//-------------------------------------------------------------------
// Implementation of allocator interface
//-------------------------------------------------------------------
#undef  INTERFACE
#define INTERFACE DAlloc

DECLARE_INTERFACE_ (DAlloc, IMalloc)
     {
     // *** IUnknown methods ***
     STDMETHOD  (QueryInterface) (THIS_ REFIID riid, LPVOID FAR *ppv) ;
     STDMETHOD_ (ULONG, AddRef)  (THIS) ;
     STDMETHOD_ (ULONG, Release) (THIS) ;

     // *** IMalloc methods ***
     STDMETHOD_ (void *, Alloc)        (THIS_ ULONG cb) ;
     STDMETHOD_ (void *, Realloc)      (THIS_ void *pv, ULONG cb) ;
     STDMETHOD_ (void,   Free)         (THIS_ void *pv) ;
     STDMETHOD_ (ULONG,  GetSize)      (THIS_ void *pv) ;
     STDMETHOD_ (int,    DidAlloc)     (THIS_ void *pv) ;
     STDMETHOD_ (void,   HeapMinimize) (THIS) ;

#ifndef CINTERFACE
public :
     DAlloc () ;
     ~DAlloc () ;
     BOOL Initialize () ;

private :
     ULONG  RefCount ;
     HANDLE hHeap ;
#endif
     } ;
