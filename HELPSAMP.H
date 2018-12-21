/*------------------------
   HELPSAMP.H header file
  ------------------------*/

#define ID_RESOURCE      1

#define IDM_FILE         100
#define IDM_NEW          101
#define IDM_OPEN         102
#define IDM_SAVE         103
#define IDM_SAVEAS       104
#define IDM_ABOUT        105

#define IDM_BOXES        200
#define IDM_DIALOG       201
#define IDM_MESSAGE_1    202
#define IDM_MESSAGE_2    203

#define IDD_ABOUT        300
#define IDD_ABOUT_HELP   301


#define DEFAULT_HELP     0
#define MSGBOX_ID1       10
#define MSGBOX_ID2       20


MRESULT EXPENTRY ClientWndProc (HWND, USHORT, MPARAM, MPARAM) ;
MRESULT EXPENTRY AboutDlgProc  (HWND, USHORT, MPARAM, MPARAM) ;
BOOL CALLBACK HelpHook(HAB, USHORT, USHORT, USHORT, PRECTL);


/* in HELPLIB.DLL  */
BOOL APIENTRY HelpBox(HWND, HWND, HAB, PSZ, PSZ);

