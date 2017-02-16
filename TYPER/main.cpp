#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>

#define BUFFER(x,y) *(pBuffer + y * cxBuffer + x )
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("TYPER");

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
           _T("Typer"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           CW_USEDEFAULT,                /* The programs width */
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
    HDC hdc;
    TEXTMETRIC tm;
    PAINTSTRUCT ps;
    static int cxChar,cyChar,cxClient,cyClient,cxBuffer,cyBuffer,xCaret,yCaret,dwCharset;
    int x,y,i;
    static TCHAR* pBuffer=NULL;

    switch (message)                  /* handle the messages */
    {
        case WM_INPUTLANGCHANGE:
            dwCharset=wParam;
            break;
        case WM_CREATE:
            hdc=GetDC(hwnd);
            SelectObject(hdc,CreateFont(0,0,0,0,0,0,0,0,dwCharset,0,0,0,FIXED_PITCH,NULL));
            GetTextMetrics(hdc,&tm);
            cxChar=tm.tmAveCharWidth;
            cyChar=tm.tmHeight;
            DeleteObject(SelectObject(hdc,GetStockObject(SYSTEM_FONT)));
            ReleaseDC(hwnd,hdc);
            break;
        case WM_SIZE:
            if(message==WM_SIZE)
            {
                cxClient=LOWORD(lParam);
                cyClient=HIWORD(lParam);
            }
            cxBuffer=max(0,cxClient/cxChar);
            cyBuffer=max(0,cyClient/cyChar);
            if(pBuffer!=NULL)
                free(pBuffer);
            pBuffer=(TCHAR *)malloc(cxBuffer*cyBuffer*sizeof(TCHAR));
            for(y=0;y<cyBuffer;y++)
                for(x=0;x<cxBuffer;x++)
                BUFFER(x,y)=' ';
            xCaret=0;
            yCaret=0;
            if(hwnd==GetFocus())
            SetCaretPos(xCaret*cxChar,yCaret*cyChar);
            InvalidateRect(hwnd,NULL,TRUE);
            break;
        case WM_SETFOCUS:
            CreateCaret(hwnd,NULL,cxChar,cyChar);//后两位表示宽度
            SetCaretPos(xCaret*cxChar,yCaret*cyChar);
            ShowCaret(hwnd);
            break;
        case WM_KILLFOCUS:
            HideCaret(hwnd);
            DestroyCaret();
            break;
        case WM_KEYDOWN:
            switch(wParam)
            {
            case VK_HOME:
                xCaret=0;
                break;
            case VK_END:
                xCaret=cxBuffer-1;
                break;
            case VK_PRIOR://page up
                yCaret=0;
                break;
            case VK_NEXT:
                yCaret=cyBuffer-1;//
                break;
            case VK_LEFT:
                xCaret=max(0,xCaret-1);
                break;
            case VK_RIGHT:
                xCaret=min(xCaret+1,cxBuffer-1);
                break;
            case VK_UP:
                yCaret=max(yCaret-1,0);
                break;
            case VK_DOWN:
                yCaret=min(cyBuffer-1,yCaret+1);
                break;
            case VK_DELETE:
                for(x=xCaret;x<cxBuffer-1;x++)
                    BUFFER(x,yCaret)=BUFFER(x+1,yCaret);//?
                BUFFER(cxBuffer-1,yCaret)=' ';
                HideCaret(hwnd);
                hdc=GetDC(hwnd);
                SelectObject(hdc,CreateFont(0,0,0,0,0,0,0,0,dwCharset,0,0,0,FIXED_PITCH,NULL));

                TextOut(hdc,xCaret*cxChar,yCaret*cyChar,&BUFFER(xCaret,yCaret),
                cxBuffer-xCaret
                        //lstrlen(&BUFFER(xCaret,yCaret))
                       );

                        DeleteObject(SelectObject(hdc,GetStockObject(SYSTEM_FONT)));
                        ReleaseDC(hwnd,hdc);
                        ShowCaret(hwnd);
                        break;

            }
            SetCaretPos(xCaret*cxChar,yCaret*cyChar);

            return 0;
            case WM_CHAR:
                for(i=0;i<(int)LOWORD(lParam);i++)
                {
                    switch(wParam)
                    {
                    case '\b'://backspace 键(删除键)
                        if(xCaret>0)
                        {
                            xCaret--;
                            SendMessage(hwnd,WM_KEYDOWN,VK_DELETE,1);
                        }
                        break;
                    case '\t'://tab键
                        do{
                            SendMessage(hwnd,WM_CHAR,' ',1);
                        }
                        while(xCaret%8!=0);//一次空8字符
                        break;
                    case '\n':// Ctrl + Enter键
                        if(++yCaret==cyBuffer)
                            yCaret=0;
                       break;
                    case '\r'://Enter 键
                        xCaret=0;
                         if(++yCaret==cyBuffer)
                            yCaret=0;
                       break;
                    case '\x1B': //ESC 键
                        for(y=yCaret;y<cyBuffer;y++)
                            for(x=xCaret;x<cxBuffer;x++)
                            BUFFER(x,y)=' ';
                        xCaret=0;
                        yCaret=0;
                        InvalidateRect(hwnd,NULL,FALSE);
                        break;
                    default:
                        BUFFER(xCaret,yCaret)=(TCHAR)wParam;
                        HideCaret(hwnd);
                        hdc=GetDC(hwnd);
                        SelectObject(hdc,CreateFont(0,0,0,0,0,0,0,0,0,dwCharset,0,0,FIXED_PITCH,NULL));

                        TextOut(hdc,xCaret*cxChar,yCaret*cyChar,&BUFFER(xCaret,yCaret),1);//输出一个字符
                        DeleteObject(SelectObject(hdc,GetStockObject(SYSTEM_FONT)));
                        ReleaseDC(hwnd,hdc);

                        ShowCaret(hwnd);

                        if(++xCaret==cxBuffer)
                        {
                            xCaret=0;
                            if(++yCaret==cyBuffer)
                                yCaret=0;
                        }
                        break;
                    }
                }
                SetCaretPos(xCaret*cxChar,yCaret*cyChar);
                return 0;

       case WM_PAINT:
            hdc=BeginPaint(hwnd,&ps);
             SelectObject(hdc,CreateFont(0,0,0,0,0,0,0,0,dwCharset,0,0,0,FIXED_PITCH,NULL));


             for(y=0;y<cyBuffer;y++)
                TextOut(hdc,0,y*cyChar,&BUFFER(0,y),cxBuffer);
              DeleteObject(SelectObject(hdc,GetStockObject(SYSTEM_FONT)));
              EndPaint(hwnd,&ps);
           return 0;

        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
