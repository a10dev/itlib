typedef struct tagCONTROLNOTIFICATIONS
{
     UINT  nCode ;
     LPSTR pName ;
} CONTROLNOTIFICATIONS ;

BOOL QueryNotifyText (UINT nNotifyCode, LPSTR *pName) ;
void DisplayText (LPSTR pText) ;
