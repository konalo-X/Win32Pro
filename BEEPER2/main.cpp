
/*
*
*       方法二:WM_TIMER 消息发送给另一个回调(CALLBACK)函数:TimerProc
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

/* Declare TIMER Method */
VOID   CALLBACK TimerProc(HWND,UINT,UINT,DWORD);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("BEEPER2");
BOOL fFlipFlop=FALSE;

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
    wincl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("BEEPER2 App"),       /* Title Text */
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
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;
    HBRUSH hBrush;


    switch (message)                  /* handle the messages */
    {
    case WM_CREATE:
        SetTimer(hwnd,ID_TIMER,1000,TimerProc);
        return 0;
  //  case WM_PAINT:
  //      hdc=BeginPaint(hwnd,&ps);
  //      hBrush=CreateSolidBrush(fFlipFlop?RGB(255,0,0):RGB(0,0,255));
   //     GetClientRect(hwnd,&rect);
   //     FillRect(hdc,&rect,hBrush);
//
   //     EndPaint(hwnd,&ps);
   //     return 0;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            KillTimer(hwnd,ID_TIMER);
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

VOID   CALLBACK TimerProc(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime)
{

    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;
    HBRUSH hBrush;

        hdc=GetDC(hwnd);
        MessageBeep(-1);fFlipFlop=!fFlipFlop;
        //InvalidateRect(hwnd,NULL,FALSE);
        hBrush=CreateSolidBrush(fFlipFlop?RGB(255,0,0):RGB(0,0,255));
        GetClientRect(hwnd,&rect);
        FillRect(hdc,&rect,hBrush);
        ReleaseDC(hwnd,hdc);



}
