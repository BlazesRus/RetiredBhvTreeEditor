#if !defined(APPDefines_IncludeGuard)
#define APPDefines_IncludeGuard

//#define		CM_INSERTCHILD				WM_APP + 10000
//#define		CM_INSERTSIBLING			WM_APP + 10001
#define		CM_DELETENODE				WM_APP + 10000
#define		CM_LOADFILE				WM_APP + 10001
#define		CM_SAVEFILE				WM_APP + 10002
#define		CM_MOVETONODE				WM_APP + 10003
#define		CM_ADDEVENT				WM_APP + 10004
#define		CM_ADDVARIABLE				WM_APP + 10005
#define		CM_MODIFYINFONODETEXT			WM_APP + 10009
#define		CM_MODIFYNODETEXT			WM_APP + 10010
#define		CM_MODIFYCONTENTTEXT			WM_APP + 10011
#define		CM_MODIFYFIELDTEXT			WM_APP + 10012
#define		CM_MODIFYPARAMETER			WM_APP + 10013
#define		CM_DELETEINFONODE				WM_APP + 10000
//#define		CM_TOGGLECONNECTINGLINES	WM_APP + 10010
//#define		CM_SETCONNECTINGLINESCOLOR	WM_APP + 10011
//#define		CM_SETFONT					WM_APP + 10020
//#define		CM_SETDEFAULTCOLOR			WM_APP + 10021

#if !defined(STRCPY)
#if defined( _UNICODE )
#define STRCPY(x,y)				wcscpy(x,y)
#define	ODDCHAR					USHORT
#else
#define STRCPY(x,y)				strcpy(x,y)
#define	ODDCHAR					TCHAR
#endif
#endif

#endif
