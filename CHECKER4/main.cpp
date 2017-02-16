
#include <windows.h>
#define DIVISIONS 5

int Child_ID=0;
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
/*  Declare child Windows procedure  */
LRESULT CALLBACK ChildWndProcedure (HWND, UINT, WPARAM, LPARAM);
/*  Make the class name into a global variable  */

TCHAR szChildName[ ] = TEXT("CodeBlocksWindowsAp");
int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     PSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASS wincl;        /* Data structure for the windowclass */
TCHAR szAppName[ ] = TEXT("CodeBlocksWindowsApp");
    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szAppName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_HREDRAW|CS_VREDRAW;                 /* Catch double-clicks */


    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);

    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClass (&wincl))
        return 0;

    /** child window structure */
    wincl.cbWndExtra=sizeof(long);
    wincl.hIcon=NULL;

    wincl.lpfnWndProc=ChildWndProcedure;
    wincl.lpszClassName=szChildName;

    /** register child window class*/

    RegisterClass(&wincl);



    /* The class is registered, let's create the program*/
    hwnd = CreateWindow (
                              /* Extended possibilites for variation */
           szAppName,         /* Classname */
           TEXT("Checker4 App"),       /* Title Text */
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

    switch (message)                  /* handle the messages */
    {
        static HWND ChildHwnd[DIVISIONS][DIVISIONS];
        int x,y,cxBlock,cyBlock;

    case WM_CREATE:
        for(x=0;x<DIVISIONS;x++)
            for(y=0;y<DIVISIONS;y++)
                ChildHwnd[x][y]=CreateWindow(szChildName,NULL,
                                       WS_CHILDWINDOW|WS_VISIBLE,
                                       0,0,0,0,hwnd,
                                       (HMENU)(y<<8|x),
                                       (HINSTANCE)GetWindowLong(hwnd,GWLP_HINSTANCE)
                                       ,NULL);


        break;
    case WM_SIZE:
        cxBlock=LOWORD(lParam)/DIVISIONS;
        cyBlock=HIWORD(lParam)/DIVISIONS;

         for(x=0;x<DIVISIONS;x++)
            for(y=0;y<DIVISIONS;y++)
                MoveWindow(ChildHwnd[x][y],x*cxBlock,y*cyBlock,cxBlock,cyBlock,TRUE);

        break;

    case WM_SETFOCUS:
        SetFocus(GetDlgItem(hwnd,Child_ID));

        break;
    case WM_KEYDOWN:

    x=Child_ID&0xFF;
    y=Child_ID>>8;
    switch(wParam)
    {
    case VK_UP:
        y--;
        break;
    case VK_DOWN:
        y++;
        break;
    case VK_LEFT:
        x--;
        break;
    case VK_RIGHT:
        x++;
        break;
    case VK_HOME:
        x=y=0;
        break;
    case VK_END:
        x=y=DIVISIONS-1;
        break;


    default:
        return 0;
    }
    x=(x+DIVISIONS)%DIVISIONS;
    y=(y+DIVISIONS)%DIVISIONS;
    Child_ID=(y<<8|x);
    SetFocus(GetDlgItem(hwnd,Child_ID));
        break;

    case WM_LBUTTONDOWN:

        MessageBeep(0);
        break;

        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;

    }

   return DefWindowProc (hwnd, message, wParam, lParam);
}

LRESULT CALLBACK ChildWndProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;

      switch (message)                  /* handle the messages */
    {
case WM_CREATE:
    SetWindowLong(hwnd,0,0);        /*»­X¿ª¹Ø*/
    break;


case WM_KEYDOWN:
    if(wParam!=VK_RETURN&&wParam!=VK_SPACE)
    {
        SendMessage(GetParent(hwnd),message,wParam,lParam);
        return 0;
    }
case WM_LBUTTONDOWN:
    SetWindowLong(hwnd,0,1^GetWindowLong(hwnd,0));
    SetFocus(hwnd);
    InvalidateRect(hwnd,NULL,FALSE);
    break;

case WM_SETFOCUS:
    Child_ID=GetWindowLong(hwnd,GWLP_ID);
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
            MoveToEx(hdc,0,rect.bottom,NULL);
            LineTo(hdc,rect.right,0);
        }
        if(hwnd==GetFocus())
        {
            rect.left+=rect.right/10;
            rect.right-=rect.left;
            rect.top+=rect.bottom/10;
            rect.bottom-=rect.top;
            SelectObject(hdc,GetStockObject(NULL_BRUSH));
            SelectObject(hdc,CreatePen(PS_DASH,1,0));
            Rectangle(hdc,rect.left,rect.top,rect.right,rect.bottom);
            DeleteObject(SelectObject(hdc,GetStockObject(BLACK_PEN)));


        }
        EndPaint(hwnd,&ps);
break;

        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
                            /* for messages that we don't deal with */

    }

    return DefWindowProc (hwnd, message, wParam, lParam);
}
