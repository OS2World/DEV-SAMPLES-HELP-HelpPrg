/*-----------------------------------------------------------
   HELPSAMP.C -- Demonstration of HELPLIB.DLL
  -----------------------------------------------------------*/

#define INCL_WIN
#include <os2.h>
#include "helpsamp.h"


/*   GLOBALS   */
HWND hwndFrame;
CHAR szHelpFile[] = "c:\\os2\\system\\helpsamp.hlp";


/***************************************************************************/

int main (void)
{
     static CHAR  szClientClass[] = "HelpSamp" ;
     static ULONG flFrameFlags = FCF_TITLEBAR      | FCF_SYSMENU |
                                 FCF_SIZEBORDER    | FCF_MINMAX  |
                                 FCF_SHELLPOSITION | FCF_TASKLIST |
                                 FCF_MENU          | FCF_ICON ;
     HAB          hab ;
     HMQ          hmq ;
     HWND         hwndClient ;
     QMSG         qmsg ;

     hab = WinInitialize (0) ;
     hmq = WinCreateMsgQueue (hab, 0) ;

     WinRegisterClass (hab, szClientClass, ClientWndProc, 0L, 0) ;

     hwndFrame = WinCreateStdWindow (HWND_DESKTOP, WS_VISIBLE,
                                     &flFrameFlags, szClientClass, NULL,
                                     0L, NULL, ID_RESOURCE, &hwndClient) ;

     /* install the help hook    */
     WinSetHook(hab, hmq, HK_HELP, (PFN)HelpHook, NULL);

     while (WinGetMsg (hab, &qmsg, NULL, 0, 0))
          WinDispatchMsg (hab, &qmsg) ;


     /* release the help hook */
     WinReleaseHook(hab, hmq, HK_HELP, (PFN)HelpHook, NULL);

     WinDestroyWindow (hwndFrame) ;
     WinDestroyMsgQueue (hmq) ;
     WinTerminate (hab) ;
     return 0;
}

/***************************************************************************/

MRESULT EXPENTRY ClientWndProc (HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2)
     {
     switch (msg)
          {
          case WM_COMMAND:
               switch (COMMANDMSG(&msg)->cmd)
                    {
                    case IDM_NEW:
                    case IDM_OPEN:
                    case IDM_SAVE:
                    case IDM_SAVEAS:
                         WinAlarm (HWND_DESKTOP, WA_NOTE) ;
                         return 0 ;

                    case IDM_ABOUT:
                    case IDM_DIALOG:
                         WinDlgBox (HWND_DESKTOP, hwnd, AboutDlgProc,
                                    NULL, IDD_ABOUT, NULL) ;
                         return 0 ;

                    case IDM_MESSAGE_1:
                         WinMessageBox(HWND_DESKTOP, hwnd,
                             (PSZ)"Press F1 for help on Message Box 1",
                             (PSZ)"Message Box 1", (USHORT)MSGBOX_ID1,
                             MB_OK|MB_HELP);
                         return 0;

                    case IDM_MESSAGE_2:
                         WinMessageBox(HWND_DESKTOP, hwnd,
                             (PSZ)"Press F1 for help on Message Box 2",
                             (PSZ)"Message Box 2", (USHORT)MSGBOX_ID2,
                             MB_OK|MB_HELP);
                         return 0;

                    }
               break ;
     
          case WM_ERASEBACKGROUND:
               return 1 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }

/***************************************************************************/

MRESULT EXPENTRY AboutDlgProc (HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2)
     {
     switch (msg)
          {
          case WM_COMMAND:
               switch (COMMANDMSG(&msg)->cmd)
                    {
                    case DID_OK:
                    case DID_CANCEL:
                         WinDismissDlg (hwnd, TRUE) ;
                         return 0 ;
                    }
               break ;

          case WM_HELP:    /* process HELP msg only if F1 was pressed */
             if(SHORT1FROMMP(mp2) == CMDSRC_ACCELERATOR)  /* means F1 was pressed */
             {
                 /* Force focus to HELP button */
                 WinSetFocus(HWND_DESKTOP, WinWindowFromID(hwnd, IDD_ABOUT_HELP));

                 /* pass on the message modified for button */
                 return(WinDefDlgProc(hwnd,
                              WM_HELP,
                              MPFROMSHORT((SHORT)IDD_ABOUT_HELP),
                              MPFROM2SHORT((SHORT)CMDSRC_PUSHBUTTON, FALSE)));
             }
             break;

          }


     return WinDefDlgProc (hwnd, msg, mp1, mp2) ;
     }

/***************************************************************************/
BOOL CALLBACK HelpHook(hab, usMode, idTopic, idSubTopic, prcPosition)
HAB hab;
USHORT usMode;
USHORT idTopic;
USHORT idSubTopic;
PRECTL prcPosition;
{
     CHAR pszHelpTopic[256];
     USHORT usStringNum = DEFAULT_HELP;

     switch(usMode)
     {
         case HLPM_MENU:
             switch(idTopic)
             {
                 case IDM_FILE:
                 case IDM_BOXES:
                     switch(idSubTopic)                     
                     {
                         case 0xFFFF:        /* menu bar itself highlighted */
                             usStringNum = idTopic;
                             break;

                         default:
                             usStringNum = idSubTopic;
                     }        
                     break;

                 default:
                     usStringNum = DEFAULT_HELP;
                     break;
             }
             break;

         case HLPM_FRAME:
             break;

         case HLPM_WINDOW:
             switch(idTopic)
             {
                 /* dialog boxes with help buttons */
                 case IDD_ABOUT:
                     switch(idSubTopic)
                     {
                         case IDD_ABOUT_HELP:
                             usStringNum = idSubTopic;
                             break;

                         default:
                             usStringNum = DEFAULT_HELP;
                     }
                     break;

                 /* Message Boxes with help Buttons */
                 case MSGBOX_ID1:
                 case MSGBOX_ID2:
                     usStringNum = idTopic;
                     break;

                 default:
                     usStringNum = DEFAULT_HELP;
             }
             break;
     }

     WinLoadString(hab, NULL, usStringNum, sizeof(pszHelpTopic), pszHelpTopic); 
     HelpBox(hwndFrame, WinQueryActiveWindow(HWND_DESKTOP, FALSE),
             hab, (PSZ)szHelpFile, (PSZ)pszHelpTopic);
     return(FALSE);
}
