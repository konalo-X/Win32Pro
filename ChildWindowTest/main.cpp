#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
/* declare Child window Procedure */
LRESULT CALLBACK ChildWndProcedure (HWND, UINT, WPARAM, LPARAM);
/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");
TCHAR szChildName[ ] = _T("ChildWindow");
int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd1;               /* This is the handle for our window */
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

    /* 定义子窗口*/
    wincl.cbWndExtra=sizeof(long);
    wincl.hIcon=NULL;

    wincl.lpszClassName =szChildName ;
    wincl.lpfnWndProc=ChildWndProcedure;

    RegisterClassEx(&wincl);

    /* The class is registered, let's create the program*/
    hwnd1 = CreateWindow (
               /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("Father Windows App"),       /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               CW_USEDEFAULT,                  /* The programs width */
               CW_USEDEFAULT,                 /* and height in pixels */
               NULL,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL              /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd1, nCmdShow);

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
    static HWND childHwnd[2];
    int x,y,cxBlock,cyBlock;

    switch (message)                  /* handle the messages */
    {
    case WM_CREATE:
        childHwnd[0]=CreateWindow (
                      /* Extended possibilites for variation */
                      szChildName,         /* Classname */
                      NULL,       /* Title Text */
                      WS_CHILDWINDOW|WS_VISIBLE, /* default window */
                      0,       /* Windows decides the position */
                      0,       /* where the window ends up on the screen */
                      0,                  /* The programs width */
                      0,                 /* and height in pixels */
                      hwnd,        /* The window is a child-window to desktop */
                      NULL,                /* No menu */
                     (HINSTANCE) 0,//(HINSTANCE)GetWindowLong(hwnd,GWLP_HINSTANCE),       /* Program Instance handler */
                      NULL );

                      childHwnd[1]=CreateWindow (
                      /* Extended possibilites for variation */
                      szChildName,         /* Classname */
                      NULL,       /* Title Text */
                      WS_CHILDWINDOW|WS_VISIBLE, /* default window */
                      0,       /* Windows decides the position */
                      0,       /* where the window ends up on the screen */
                      0,                  /* The programs width */
                      0,                 /* and height in pixels */
                      hwnd,        /* The window is a child-window to desktop */
                      NULL,
                                      /* No menu */
                     (HINSTANCE)0, //(HINSTANCE)GetWindowLong(hwnd,GWLP_HINSTANCE),       /* Program Instance handler */
                      NULL );
        break;                /* No Window Creation data */
    case WM_SIZE:
        cxBlock=LOWORD(lParam)/4;
        cyBlock=HIWORD(lParam)/4;

        MoveWindow(childHwnd[0],0,0,cxBlock,cyBlock,TRUE);
        MoveWindow(childHwnd[01],2*cxBlock,2*cyBlock,cxBlock,cyBlock,TRUE);
        break;
    case WM_LBUTTONDOWN:
        MessageBeep(0);
        break;

    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
LRESULT CALLBACK ChildWndProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
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
        InvalidateRect(hwnd,NULL,FALSE);


       // Rectangle(hdc,0,0,rect.right,rect.bottom);


        //;

    break;
    case WM_PAINT:
        hdc=BeginPaint(hwnd,&ps);
        GetClientRect(hwnd,&rect);

        Rectangle(hdc,0,0,rect.right,rect.bottom);
        if(GetWindowLong(hwnd,0))
        {

        GetClientRect(hwnd,&rect);
        MoveToEx(hdc,0,0,NULL);
        LineTo(hdc,rect.right,rect.bottom);
        //InvalidateRect(hwnd,&rect,FALSE);
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
