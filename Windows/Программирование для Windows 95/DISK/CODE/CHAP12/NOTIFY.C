/*-----------------------------------------
   NOTIFY.C -- Support notification lookup
               (c) Paul Yao, 1996
  -----------------------------------------*/
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <prsht.h>
#include "notify.h"

CONTROLNOTIFICATIONS cnLookupTable[] =
     {
     NM_OUTOFMEMORY,       "NM_OUTOFMEMORY",
     NM_CLICK,             "NM_CLICK",
     NM_DBLCLK,            "NM_DBLCLK",
     NM_RETURN,            "NM_RETURN",
     NM_RCLICK,            "NM_RCLICK",
     NM_RDBLCLK,           "NM_RDBLCLK",
     NM_SETFOCUS,          "NM_SETFOCUS",
     NM_KILLFOCUS,         "NM_KILLFOCUS",
     LVN_ITEMCHANGING,     "LVN_ITEMCHANGING",
     LVN_ITEMCHANGED,      "LVN_ITEMCHANGED",
     LVN_INSERTITEM,       "LVN_INSERTITEM",
     LVN_DELETEITEM,       "LVN_DELETEITEM",
     LVN_DELETEALLITEMS,   "LVN_DELETEALLITEMS",
     LVN_BEGINLABELEDITA,  "LVN_BEGINLABELEDITA",
     LVN_BEGINLABELEDITW,  "LVN_BEGINLABELEDITW",
     LVN_ENDLABELEDITA,    "LVN_ENDLABELEDITA",
     LVN_ENDLABELEDITW,    "LVN_ENDLABELEDITW",
     LVN_COLUMNCLICK,      "LVN_COLUMNCLICK",
     LVN_BEGINDRAG,        "LVN_BEGINDRAG",
     LVN_BEGINRDRAG,       "LVN_BEGINRDRAG",
     LVN_GETDISPINFOA,     "LVN_GETDISPINFOA",
     LVN_GETDISPINFOW,     "LVN_GETDISPINFOW",
     LVN_SETDISPINFOA,     "LVN_SETDISPINFOA",
     LVN_SETDISPINFOW,     "LVN_SETDISPINFOW",
     LVN_KEYDOWN,          "LVN_KEYDOWN",
     HDN_ITEMCHANGINGA,    "HDN_ITEMCHANGINGA",
     HDN_ITEMCHANGINGW,    "HDN_ITEMCHANGINGW",
     HDN_ITEMCHANGEDA,     "HDN_ITEMCHANGEDA",
     HDN_ITEMCHANGEDW,     "HDN_ITEMCHANGEDW",
     HDN_ITEMCLICKA,       "HDN_ITEMCLICKA",
     HDN_ITEMCLICKW,       "HDN_ITEMCLICKW",
     HDN_ITEMDBLCLICKA,    "HDN_ITEMDBLCLICKA",
     HDN_ITEMDBLCLICKW,    "HDN_ITEMDBLCLICKW",
     HDN_DIVIDERDBLCLICKA, "HDN_DIVIDERDBLCLICKA",
     HDN_DIVIDERDBLCLICKW, "HDN_DIVIDERDBLCLICKW",
     HDN_BEGINTRACKA,      "HDN_BEGINTRACKA",
     HDN_BEGINTRACKW,      "HDN_BEGINTRACKW",
     HDN_ENDTRACKA,        "HDN_ENDTRACKA",
     HDN_ENDTRACKW,        "HDN_ENDTRACKW",
     HDN_TRACKA,           "HDN_TRACKA",
     HDN_TRACKW,           "HDN_TRACKW",
     TVN_SELCHANGINGA,     "TVN_SELCHANGINGA",
     TVN_SELCHANGINGW,     "TVN_SELCHANGINGW",
     TVN_SELCHANGEDA,      "TVN_SELCHANGEDA",
     TVN_SELCHANGEDW,      "TVN_SELCHANGEDW",
     TVN_GETDISPINFOA,     "TVN_GETDISPINFOA",
     TVN_GETDISPINFOW,     "TVN_GETDISPINFOW",
     TVN_SETDISPINFOA,     "TVN_SETDISPINFOA",
     TVN_SETDISPINFOW,     "TVN_SETDISPINFOW",
     TVN_ITEMEXPANDINGA,   "TVN_ITEMEXPANDINGA",
     TVN_ITEMEXPANDINGW,   "TVN_ITEMEXPANDINGW",
     TVN_ITEMEXPANDEDA,    "TVN_ITEMEXPANDEDA",
     TVN_ITEMEXPANDEDW,    "TVN_ITEMEXPANDEDW",
     TVN_BEGINDRAGA,       "TVN_BEGINDRAGA",
     TVN_BEGINDRAGW,       "TVN_BEGINDRAGW",
     TVN_BEGINRDRAGA,      "TVN_BEGINRDRAGA",
     TVN_BEGINRDRAGW,      "TVN_BEGINRDRAGW",
     TVN_DELETEITEMA,      "TVN_DELETEITEMA",
     TVN_DELETEITEMW,      "TVN_DELETEITEMW",
     TVN_BEGINLABELEDITA,  "TVN_BEGINLABELEDITA",
     TVN_BEGINLABELEDITW,  "TVN_BEGINLABELEDITW",
     TVN_ENDLABELEDITA,    "TVN_ENDLABELEDITA",
     TVN_ENDLABELEDITW,    "TVN_ENDLABELEDITW",
     TVN_KEYDOWN,          "TVN_KEYDOWN",
     TTN_NEEDTEXTA,        "TTN_NEEDTEXTA",
     TTN_NEEDTEXTW,        "TTN_NEEDTEXTW",
     TTN_SHOW,             "TTN_SHOW",
     TTN_POP,              "TTN_POP",
     TCN_KEYDOWN,          "TCN_KEYDOWN",
     TCN_SELCHANGE,        "TCN_SELCHANGE",
     TCN_SELCHANGING,      "TCN_SELCHANGING",
     TBN_GETBUTTONINFOA,   "TBN_GETBUTTONINFOA",
     TBN_GETBUTTONINFOW,   "TBN_GETBUTTONINFOW",
     TBN_BEGINDRAG,        "TBN_BEGINDRAG",
     TBN_ENDDRAG,          "TBN_ENDDRAG",
     TBN_BEGINADJUST,      "TBN_BEGINADJUST",
     TBN_ENDADJUST,        "TBN_ENDADJUST",
     TBN_RESET,            "TBN_RESET",
     TBN_QUERYINSERT,      "TBN_QUERYINSERT",
     TBN_QUERYDELETE,      "TBN_QUERYDELETE",
     TBN_TOOLBARCHANGE,    "TBN_TOOLBARCHANGE",
     TBN_CUSTHELP,         "TBN_CUSTHELP",
     UDN_DELTAPOS,         "UDN_DELTAPOS",
     PSN_SETACTIVE,        "PSN_SETACTIVE",
     PSN_KILLACTIVE,       "PSN_KILLACTIVE",
     PSN_APPLY,            "PSN_APPLY",
     PSN_RESET,            "PSN_RESET",
     PSN_HELP,             "PSN_HELP",
     PSN_WIZBACK,          "PSN_WIZBACK",
     PSN_WIZNEXT,          "PSN_WIZNEXT",
     PSN_WIZFINISH,        "PSN_WIZFINISH",
     PSN_QUERYCANCEL,      "PSN_QUERYCANCEL"
     } ;

int NOTIFY_COUNT = sizeof (cnLookupTable) / sizeof (CONTROLNOTIFICATIONS) ;

//-------------------------------------------------------------------
// QueryNotifyText: Convert notification codes into text.
//-------------------------------------------------------------------
BOOL QueryNotifyText (UINT nNotifyCode, LPSTR *pName)
     {
     BOOL bFound = FALSE ;
     int  iNotify ;

     for (iNotify = 0 ; iNotify < NOTIFY_COUNT ; iNotify++)
          {
          if (cnLookupTable[iNotify].nCode == nNotifyCode)
               {
               *pName = cnLookupTable[iNotify].pName ;
               return TRUE ;
               }
          }

     // Unknown notification code.
     *pName = "** Unknown **" ;
     return FALSE ; 
     }
