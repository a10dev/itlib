//-------------------------------------------------------------------
// C Interface to private allocator
//-------------------------------------------------------------------
#define EXPORT extern "C" __declspec (dllexport)

EXPORT LPMALLOC CreateAllocator () ;

// {308D0430-1090-11cf-B92A-00AA006238F8}
DEFINE_GUID (CLSID_ALLOCATOR, 
     0x308d0430, 0x1090, 0x11cf, 0xb9, 
     0x2a, 0x0, 0xaa, 0x0, 0x62, 0x38, 0xf8) ;

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
     ULONG RefCount ;
     HANDLE hHeap ;
#endif
     } ;

// Class Factory
#undef  INTERFACE
#define INTERFACE DClassFactory

DECLARE_INTERFACE_ (DClassFactory, IClassFactory)
     {
     // *** IUnknown methods ***
     STDMETHOD  (QueryInterface) (THIS_ REFIID riid, LPVOID FAR* ppv) ;
     STDMETHOD_ (ULONG, AddRef)  (THIS) ;
     STDMETHOD_ (ULONG, Release) (THIS) ;

     // *** IClassFactory methods ***
     STDMETHOD (CreateInstance) (THIS_ LPUNKNOWN pUnkOuter,
                                 REFIID riid, LPVOID FAR *ppvObject) ;
     STDMETHOD (LockServer)     (THIS_ BOOL fLock) ;

#ifndef CINTERFACE
public :
     DClassFactory () ;
     ~DClassFactory () ;
private :
     ULONG RefCount ;
#endif
     } ;

