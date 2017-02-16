#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>

#define DIVISIONS 5
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildWndProc (HWND, UINT, WPARAM, LPARAM);
/*  Make the class name into a global variable  */
TCHAR szAppName[ ] = _T("CodeBlocksWindowsApp");
TCHAR szChildName[]=_T("ChildWindowAPP");
int child_ID=0;
int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szAppName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;
        /* child window class*/
    wincl.cbWndExtra=sizeof(long);
    wincl.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
    wincl.hIcon=NULL;
    wincl.hIconSm=NULL;
    wincl.lpfnWndProc=ChildWndProc;
    wincl.lpszClassName=szChildName;
    RegisterClassEx(&wincl);
    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szAppName,         /* Classname */
           _T("Code::Blocks Template Windows App"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           CW_USEDEFAULT,                 /* The programs width */
           CW_USEDEFAULT,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

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


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int x,y,cxBlock,cyBlock;
    static HWND childHwnd[DIVISIONS][DIVISIONS];
    switch (message)                  /* handle the messages */
    {
    case WM_CREATE:
        for(x=0;x<DIVISIONS;x++)
            for(y=0;y<DIVISIONS;y++)
        childHwnd[x][y]=CreateWindowEx(
            0,szChildName,NULL,WS_CHILDWINDOW|WS_VISIBLE,0,0,0,0,
            hwnd,(HMENU)(y<<8|x),(HINSTANCE)GetWindowLong(hwnd,GWLP_HINSTANCE),NULL
        );

        return 0;

    case WM_SIZE:
        cxBlock=LOWORD(lParam)/DIVISIONS;
        cyBlock=HIWORD(lParam)/DIVISIONS;
        for(x=0;x<DIVISIONS;x++)
            for(y=0;y<DIVISIONS;y++)
                MoveWindow(childHwnd[x][y],x*cxBlock,y*cyBlock,cxBlock,cyBlock,TRUE);
        InvalidateRect(hwnd,NULL,FALSE);
        return 0;

    case WM_SETFOCUS:
        SetFocus(GetDlgItem(hwnd,child_ID));
        break;

        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

LRESULT CALLBACK ChildWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;
    switch (message)                  /* handle the messages */
    {
    case WM_CREATE:
        SetWindowLong(hwnd,0,0);
        break;
    case WM_LBUTTONDOWN:
        SetWindowLong(hwnd,0,1^GetWindowLong(hwnd,0));
        SetFocus(hwnd);
        InvalidateRect(hwnd,NULL,FALSE);
        break;
    case WM_SETFOCUS:
        child_ID=GetWindowLong(hwnd,GWLP_ID);
        break;
    case WM_KILLFOCUS:
        InvalidateRect(hwnd,NULL,TRUE);
        break;
    case WM_PAINT:
        hdc=BeginPaint(hwnd,&ps);
        GetClientRect(hwnd,&rect);
        Rectangle(hdc,0,0,rect.right,rect.bottom);
        if(GetWindowLong(hwnd,0))
        {
            MoveToEx(hdc,0,0,NULL);
            LineTo(hdc,rect.right,rect.bottom);

        }
        if(hwnd==GetFocus())
        {
                TextOut(hdc,0,0,szChildName,lstrlen(szChildName));
            //Rectangle(hdc,0+10,10,rect.right-10,rect.bottom-10);
        }
        EndPaint(hwnd,&ps);
        break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;

}
