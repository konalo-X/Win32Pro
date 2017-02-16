/*
*
*       方法一:WM_TIMER 消息发送给正常的窗口过程
*
*/

#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#define ID_TIMER 1

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("BEEPER1");

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
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS|CS_VREDRAW|CS_HREDRAW;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
    {
        MessageBox(NULL,TEXT("The Program requires windows NT!"),szClassName,MB_ICONERROR);
        return 0;
    }


    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Beeper1 App"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           CW_USEDEFAULT,                 /* The programs width */
           CW_USEDEFAULT,                 /* and height in pixels */
           NULL,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
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


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static BOOL fFilpFlop=FALSE;
    HBRUSH hBrush;
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;
    switch (message)                  /* handle the messages */
    {
    case WM_CREATE:
        SetTimer(hwnd,ID_TIMER,1000,NULL);
        return 0;

    case WM_TIMER:
        MessageBeep(16);
        fFilpFlop=!fFilpFlop;
        InvalidateRect(hwnd,NULL,FALSE);
        return 0;
    case WM_PAINT:
        hdc=BeginPaint(hwnd,&ps);
        GetClientRect(hwnd,&rect);
        hBrush=CreateSolidBrush(fFilpFlop?RGB(255,0,0):RGB(0,0,255));//指定画笔的颜色 COLORREF 结构。 颜色指定RGB值，并可以构造与WINDOWS.H.的 RGB 宏
        FillRect(hdc,&rect,hBrush);
        /*
        void FillRect(
            LPCRECT lpRect,
            CBrush* pBrush
        );用指定的画刷填充矩形

                */
        EndPaint(hwnd,&ps);
        DeleteObject(hBrush);
        return 0;

        case WM_DESTROY:
            KillTimer(hwnd,ID_TIMER);//结束计时器
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
