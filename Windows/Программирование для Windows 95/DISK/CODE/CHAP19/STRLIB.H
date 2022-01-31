/*----------------------
   STRLIB.H header file
  ----------------------*/

typedef BOOL (CALLBACK *PSTRCB) (PSTR, PVOID) ;
#define MAX_STRINGS 256

#define EXPORT  extern "C" __declspec (dllexport)

EXPORT BOOL CALLBACK AddString    (PSTR) ;
EXPORT BOOL CALLBACK DeleteString (PSTR) ;
EXPORT int  CALLBACK GetStrings   (PSTRCB, PVOID) ;
