
#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif
#define UNTITLE TEXT("无标题")
#define IDEDIT 1
#define IDSTATUBAR 2
#include <tchar.h>
#include <windows.h>
#include"resource.h"


#include <commctrl.h>  //状态栏
#pragma comment(lib, "comctl32.lib")  


/*  Declare Windows procedure  */
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
/*  Make the class name into a global variable  */
TCHAR szClassName[] = _T("POPPAD");
TCHAR szAppName[]=_T("记事本");


//

///ゅン巨 functions  in POPFILE.c
void PopFileInitialize(HWND);
BOOL PopFileOpenDlg(HWND,PTSTR,PTSTR);
BOOL PopFileSaveDlg(HWND,PTSTR,PTSTR);
BOOL PopFileRead(HWND,PTSTR);
BOOL PopFileWrite(HWND,PTSTR);

///琩高籔蠢传巨 POPFIND.C

HWND PopFindFindDlg(HWND);
HWND PopFindReplaceDlg(HWND);
BOOL PopFindFindText(HWND,int *,LPFINDREPLACE);
BOOL PopFindReplaceTex(HWND,int *,LPFINDREPLACE);
BOOL PopFindNextText(HWND,int*);
BOOL PopFindValidFind(void);

///砰巨 POPFONT.C
void PopFontInitialize(HWND);
BOOL PopFontChooseFont(HWND);
void PopFontSetFont(HWND);
void PopFontDeinitialize(void);

///ゴ巨钡POPPRINT.C
BOOL PopPrntPrintFile(HINSTANCE,HWND,HWND,PTSTR);




int WINAPI WinMain (HINSTANCE hisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASS winclass;        /* Data structure for the windowclass */
	
    /* The Window structure */
    winclass.hInstance =hisInstance;
    winclass.lpszClassName = szClassName;
    winclass.lpfnWndProc = WndProc;      /* This function is called by windows */
    winclass.style = CS_HREDRAW|CS_VREDRAW;                 /* Catch double-clicks */

    /* Use default icon and mouse-pointer */
    winclass.hIcon = LoadIcon (hisInstance, szClassName);
    winclass.hCursor = LoadCursor (NULL, IDC_ARROW);
    winclass.lpszMenuName = TEXT("poppad");                 /* No menu */
    winclass.cbClsExtra = 0;                      /* No extra bytes after the window class */
    winclass.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    winclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClass (&winclass))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           NULL,       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           CW_USEDEFAULT,                 /* The programs width */
           CW_USEDEFAULT,                 /* and height in pixels */
           NULL,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);
    UpdateWindow(hwnd);


    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

//设置记事本标题
void doCaption(HWND hwnd,TCHAR * szTitleName)
{
     TCHAR szBuffer[MAX_PATH+64];
    wsprintf(szBuffer,TEXT("%s - %s"),szTitleName[0]?szTitleName:UNTITLE,szAppName);
    SetWindowText(hwnd,szBuffer);
}

// 询问是否保存,返回值IDYES/IDCANCEL

BOOL AskAboutSave(HWND hwnd,PTSTR szTitle)
{
	TCHAR szBuffer[MAX_PATH+64];
	int iReturn;
	wsprintf(szBuffer, TEXT("是否保存当前文件 [ %s ]"), szTitle[0]?szTitle: UNTITLE);
    
	iReturn=MessageBox(hwnd, szBuffer, szAppName, MB_YESNOCANCEL | MB_ICONQUESTION);

	if (iReturn==IDYES)
	{
		if (!SendMessage(hwnd, WM_COMMAND, (WPARAM)IDM_FILE_SAVE, 0))
			iReturn = IDCANCEL;
	}
	return iReturn;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HINSTANCE hInst;
    static HWND hwndEdit ,hStatus;
    static HINSTANCE hInstance;
	static BOOL bNeedSave;
	RECT rectBar;
   static TCHAR szTitleName[MAX_PATH],szFileName[MAX_PATH];
    switch (message)                  /* handle the messages*/
    {
    case WM_CREATE:
													//szTitleName[MAX_PATH]=TEXT("H.txt");
        hInst=((LPCREATESTRUCT)lParam)->hInstance;
            hwndEdit = CreateWindow (
           TEXT("edit"),         /* ClassName */
           NULL,       /* Title Text */
           WS_CHILD|WS_HSCROLL|WS_VSCROLL|WS_VISIBLE|ES_AUTOHSCROLL|ES_AUTOVSCROLL|ES_MULTILINE|ES_NOHIDESEL, /* default window */
           0,       /* Windows decides the position */
           0,       /* where the window ends up on the screen */
           0,                 /* The programs width */
           0,                 /* and height in pixels */
           hwnd,        /* The window is a child-window to desktop */
           (HMENU)IDEDIT,                /* No menu */
           hInst,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );
           SendMessage(hwndEdit,EM_LIMITTEXT,32000,0L);
            PopFileInitialize(hwnd);

           doCaption(hwnd,szTitleName);


		   INITCOMMONCONTROLSEX icex;// Ensure that the common control DLL is loaded.   
		   icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
		   icex.dwICC = ICC_BAR_CLASSES;
		   InitCommonControlsEx(&icex);
		   hStatus = CreateWindow(STATUSCLASSNAME, _T("111"), WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP|SBS_BOTTOMALIGN, 0, 0, 0, 0, hwnd, (HMENU)IDSTATUBAR, hInst,NULL);
		  // int nSize[4] = { 200, 250, 300, -1 };
		  // SendMessage(hStatus, SB_SETPARTS, sizeof(nSize) / sizeof(nSize[0]), (LPARAM)nSize);
	
        return 0;

    case WM_SETFOCUS:
        SetFocus(hwndEdit);
        return 0;

    case WM_SIZE:
		
		GetClientRect(hStatus, &rectBar);
        MoveWindow(hwndEdit,0,0,LOWORD(lParam),HIWORD(lParam),FALSE);
		MoveWindow(hStatus, 0, HIWORD(lParam) -(rectBar.bottom- rectBar.top)-100, LOWORD(hwnd), rectBar.bottom - rectBar.top,FALSE);
		InvalidateRect(hwnd, NULL, TRUE);
        return 0;
    case WM_COMMAND:
		
		
		/*首先检查是否是Edit 控件发送的消息*/

		if (lParam&&LOWORD(wParam) == IDEDIT)
		{
			switch (HIWORD(wParam))
			{
			case EN_UPDATE:
				bNeedSave = TRUE;
				return 0;
			case EN_MAXTEXT:
			case EN_ERRSPACE:
				MessageBox(hwnd, TEXT("记事本存储空间不足!"), szAppName, MB_ICONSTOP);
				return 0;
			}
			break;
		}
        switch(LOWORD(wParam))
        {
		case IDM_FILE_NEW:

			if (bNeedSave&&IDCANCEL == AskAboutSave(hwnd, szTitleName))		//文本有变更，且没保存，不能新建
			{
				return 0;  
			}
			SetWindowText(hwnd, TEXT("\0"));
			szFileName[0] = '\0';
			szTitleName[0] = '\0';
			doCaption(hwnd, szTitleName);
			bNeedSave = FALSE;
			return 0;

        case IDM_FILE_OPEN:
			if (bNeedSave&&IDCANCEL == AskAboutSave(hwnd, szTitleName))		//文本有变更，且没保存，不能新建
				return 0;
            if(PopFileOpenDlg(hwnd,szFileName,szTitleName));
			{
				if (!PopFileRead(hwndEdit, szFileName));
			}
           
			szFileName[0] = '\0';
			szTitleName[0] = '\0';
            return 0;
		case IDM_FILE_SAVE:
			if (PopFileSaveDlg(hwnd, szFileName, szTitleName))
			{
				PopFileWrite(hwnd, szFileName);
			}
			return 0;
        }

        break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue*/
            break;
        default:                      /* for messages that we don't deal with*/
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}



