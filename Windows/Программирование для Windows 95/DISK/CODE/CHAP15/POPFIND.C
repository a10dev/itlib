/*--------------------------------------------------------
   POPFIND.C -- Popup Editor Search and Replace Functions
  --------------------------------------------------------*/

#include <windows.h>
#include <commdlg.h>
#include <string.h>
#define MAX_STRING_LEN   256

static char szFindText [MAX_STRING_LEN] ;
static char szReplText [MAX_STRING_LEN] ;

HWND PopFindFindDlg (HWND hwnd)
     {
     static FINDREPLACE fr ;       // must be static for modeless dialog!!!

     fr.lStructSize      = sizeof (FINDREPLACE) ;
     fr.hwndOwner        = hwnd ;
     fr.hInstance        = NULL ;
     fr.Flags            = FR_HIDEUPDOWN | FR_HIDEMATCHCASE | FR_HIDEWHOLEWORD ;
     fr.lpstrFindWhat    = szFindText ;
     fr.lpstrReplaceWith = NULL ;
     fr.wFindWhatLen     = sizeof (szFindText) ;
     fr.wReplaceWithLen  = 0 ;
     fr.lCustData        = 0 ;
     fr.lpfnHook         = NULL ;
     fr.lpTemplateName   = NULL ;

     return FindText (&fr) ;
     }

HWND PopFindReplaceDlg (HWND hwnd)
     {
     static FINDREPLACE fr ;       // must be static for modeless dialog!!!

     fr.lStructSize      = sizeof (FINDREPLACE) ;
     fr.hwndOwner        = hwnd ;
     fr.hInstance        = NULL ;
     fr.Flags            = FR_HIDEUPDOWN | FR_HIDEMATCHCASE | FR_HIDEWHOLEWORD ;
     fr.lpstrFindWhat    = szFindText ;
     fr.lpstrReplaceWith = szReplText ;
     fr.wFindWhatLen     = sizeof (szFindText) ;
     fr.wReplaceWithLen  = sizeof (szReplText) ;
     fr.lCustData        = 0 ;
     fr.lpfnHook         = NULL ;
     fr.lpTemplateName   = NULL ;

     return ReplaceText (&fr) ;
     }

BOOL PopFindFindText (HWND hwndEdit, int *piSearchOffset, LPFINDREPLACE pfr)
     {
     int   iLength, iPos ;
     PSTR  pstrDoc, pstrPos ;

               // Read in the edit document

     iLength = GetWindowTextLength (hwndEdit) ;

     if (NULL == (pstrDoc = (PSTR) malloc (iLength + 1)))
          return FALSE ;

     GetWindowText (hwndEdit, pstrDoc, iLength + 1) ;

               // Search the document for the find string

     pstrPos = strstr (pstrDoc + *piSearchOffset, pfr->lpstrFindWhat) ;
     free (pstrDoc) ;

               // Return an error code if the string cannot be found

     if (pstrPos == NULL)
          return FALSE ;

               // Find the position in the document and the new start offset

     iPos = pstrPos - pstrDoc ;
     *piSearchOffset = iPos + strlen (pfr->lpstrFindWhat) ;

               // Select the found text

     SendMessage (hwndEdit, EM_SETSEL, iPos, *piSearchOffset) ;
     SendMessage (hwndEdit, EM_SCROLLCARET, 0, 0) ;

     return TRUE ;
     }

BOOL PopFindNextText (HWND hwndEdit, int *piSearchOffset)
     {
     FINDREPLACE fr ;

     fr.lpstrFindWhat = szFindText ;

     return PopFindFindText (hwndEdit, piSearchOffset, &fr) ;
     }

BOOL PopFindReplaceText (HWND hwndEdit, int *piSearchOffset, LPFINDREPLACE pfr)
     {
               // Find the text

     if (!PopFindFindText (hwndEdit, piSearchOffset, pfr))
          return FALSE ;

               // Replace it

     SendMessage (hwndEdit, EM_REPLACESEL, 0, (LPARAM) pfr->lpstrReplaceWith) ;

     return TRUE ;
     }

BOOL PopFindValidFind (void)
     {
     return *szFindText != '\0' ;
     }
