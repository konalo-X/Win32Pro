
#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif
#define UNTITLE TEXT("未命名")
#define EDIT 1
#include <tchar.h>
#include <windows.h>
#include"resource.h"
/*  Declare Windows procedure  */
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
/*  Make the class name into a global variable  */
TCHAR szClassName[] = _T("PodPad");
TCHAR szAppName[]=_T("記事本");


//

///文件操作 functions  in POPFILE.c
void PopFileInitialize(HWND);
BOOL PopFileOpenDlg(HWND,PTSTR,PTSTR);
BOOL PopFileSaveDlg(HWND,PTSTR,PTSTR);
BOOL PopFileRead(HWND,PTSTR);
BOOL PopFileWrite(HWND,PTSTR);

///查詢與替換操作 POPFIND.C

HWND PopFindFindDlg(HWND);
HWND PopFindReplaceDlg(HWND);
BOOL PopFindFindText(HWND,int *,LPFINDREPLACE);
BOOL PopFindReplaceTex(HWND,int *,LPFINDREPLACE);
BOOL PopFindNextText(HWND,int*);
BOOL PopFindValidFind(void);

///字體操作 POPFONT.C
void PopFontInitialize(HWND);
BOOL PopFontChooseFont(HWND);
void PopFontSetFont(HWND);
void PopFontDeinitialize(void);

///打印操作接口POPPRINT.C
BOOL PopPrntPrintFile(HINSTANCE,HWND,HWND,PTSTR);




int WINAPI WinMain (HINSTANCE hisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX winclass;        /* Data structure for the windowclass */

    /* The Window structure */
    winclass.hInstance =hisInstance;
    winclass.lpszClassName = szClassName;
    winclass.lpfnWndProc = WndProc;      /* This function is called by windows */
    winclass.style = CS_DBLCLKS;                 /* Catch double-clicks */
    winclass.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    winclass.hIcon = LoadIcon (hisInstance, szClassName);
    winclass.hIconSm = LoadIcon (hisInstance, szClassName);
    winclass.hCursor = LoadCursor (NULL, IDC_ARROW);
    winclass.lpszMenuName = TEXT("poppad");                 /* No menu */
    winclass.cbClsExtra = 0;                      /* No extra bytes after the window class */
    winclass.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    winclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&winclass))
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

void doCaption(HWND hwnd,TCHAR * szTitleName)
{
     TCHAR szBuffer[MAX_PATH+60];
    wsprintf(szBuffer,TEXT("%s - %s"),szTitleName[0]?szTitleName:UNTITLE,szAppName);
    SetWindowText(hwnd,szBuffer);
}


LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HINSTANCE hInst;
    static HWND hwndEdit;
    static HINSTANCE hInstance;

   static TCHAR szTitleName[MAX_PATH],szFileName[MAX_PATH];
    switch (message)                  /* handle the messages*/
    {
    case WM_CREATE:
        //szTitleName[MAX_PATH]=TEXT("H.txt");
        hInst=((LPCREATESTRUCT)lParam)->hInstance;
            hwndEdit = CreateWindow (
           TEXT("edit"),         /* Classname */
           NULL,       /* Title Text */
           WS_CHILD|WS_HSCROLL|WS_VSCROLL|WS_VISIBLE|ES_AUTOHSCROLL|ES_AUTOVSCROLL|ES_MULTILINE|ES_NOHIDESEL, /* default window */
           0,       /* Windows decides the position */
           0,       /* where the window ends up on the screen */
           0,                 /* The programs width */
           0,                 /* and height in pixels */
           hwnd,        /* The window is a child-window to desktop */
           (HMENU)EDIT,                /* No menu */
           hInst,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );
           SendMessage(hwndEdit,EM_LIMITTEXT,32000,0L);
            PopFileInitialize(hwnd);

           doCaption(hwnd,szTitleName);
        return 0;

    case WM_SETFOCUS:
        SetFocus(hwndEdit);
        return 0;

    case WM_SIZE:
        MoveWindow(hwndEdit,0,0,LOWORD(lParam),HIWORD(lParam),FALSE);
        return 0;
    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case IDM_FILE_OPEN:
            PopFileOpenDlg(hwnd,szFileName,szTitleName);
            doCaption(hwnd,szTitleName);
            return 0;
        }

        return 0;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue*/
            break;
        default:                      /* for messages that we don't deal with*/
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}


/*
BOOL CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
    case WM_INITDIALOG:
        SetWindowText(hDlg,TEXT("Hell Dialog Box"));
        return TRUE;
    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case IDOK:
            EndDialog(hDlg,0);
            return TRUE;
        }
         break;
    }


    return FALSE;
}
*/
