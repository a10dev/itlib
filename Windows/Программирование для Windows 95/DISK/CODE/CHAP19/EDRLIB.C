/*-------------------------------------------------
   EDRLIB.C -- Easy Drawing Routine Library module
               (c) Charles Petzold,  1996
  -------------------------------------------------*/

#include <windows.h>
#include <string.h>
#include "edrlib.h"

int WINAPI DllMain (HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved)
     {
     return TRUE ;
     }

EXPORT BOOL CALLBACK EdrCenterText (HDC hdc, PRECT prc, PSTR pString)
     {
     int  iLength ;
     SIZE size ;

     iLength = strlen (pString) ;

     GetTextExtentPoint32 (hdc, pString, iLength, &size) ;

     return TextOut (hdc, (prc->right - prc->left - size.cx) / 2,
                          (prc->bottom - prc->top - size.cy) / 2,
                     pString, iLength) ;
     }
