#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
static struct {

    int font;
    TCHAR *szDesc;
}cFonts[]={
ANSI_FIXED_FONT,_T("ANSI_FIXED_FONT"),
ANSI_VAR_FONT,_T("ANSI_VAR_FONT"),
DEVICE_DEFAULT_FONT,_T("DEVICE_DEFAULT_FONT"),
DEFAULT_GUI_FONT,_T("DEFAULT_GUI_FONT"),
OEM_FIXED_FONT,_T("OEM_FIXED_FONT"),
SYSTEM_FONT,_T("SYSTEM_FONT"),
 SYSTEM_FIXED_FONT,_T("SYSTEM_FIXED_FONT")
 };
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("STOCKFONT");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASS wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_HREDRAW|CS_VREDRAW;                 /* Catch double-clicks */
//    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
//    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClass (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindow (
                        /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("STOCKFONT"),       /* Title Text */
           WS_OVERLAPPEDWINDOW|WS_VSCROLL, /* default window */
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
    UpdateWindow(hwnd);
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
    static int iFont=(int)(sizeof cFonts/sizeof cFonts[0]);
    HDC hdc;
    PAINTSTRUCT ps;
    TEXTMETRIC tm;
    static int cxChar,cyChar,cxClient,cyClient;
    TCHAR szFontName[20],szFaceName[LF_FACESIZE];//LF_FACESIZE  32  getfacename()获得当前字体名字
    int i,cMaxLine;
   static int vpos;
   RECT rec;
    switch (message)                  /* handle the messages */
    {
    case WM_CREATE:
        hdc=GetDC(hwnd);
        GetTextMetrics(hdc,&tm);
        cxChar=max(tm.tmAveCharWidth*3/2,tm.tmMaxCharWidth);
        cyChar=tm.tmHeight;
        GetClientRect(hwnd,&rec);
        ReleaseDC(hwnd,hdc);
        return 0;
    case WM_SIZE:
        cxClient=LOWORD(wParam);
        cyClient=HIWORD(wParam);

        SetScrollRange(hwnd,SB_VERT,0,iFont-1,TRUE);
        SetScrollPos(hwnd,SB_VERT,0,TRUE);
        return 0;
    case WM_VSCROLL:
        switch(LOWORD(wParam))
        {
        case SB_TOP:
            vpos=0;
            break;
        case SB_BOTTOM:
            vpos=iFont-1;
            break;
        case SB_LINEDOWN:
            vpos+=1;
            break;
        case SB_LINEUP:
            vpos-=1;
            break;
        case SB_THUMBTRACK:
            vpos=HIWORD(wParam);//wParam 低位通知码,高位位置
            break;
        default:
            break;
        }
    vpos=max(0,min(vpos,iFont-1));
    SetScrollPos(hwnd,SB_VERT,vpos,TRUE);
    InvalidateRect(hwnd,&rec,TRUE);
        return 0;
    case WM_KEYDOWN:
            switch(wParam)
            {
                case VK_HOME:SendMessage(hwnd,WM_VSCROLL,SB_TOP,NULL);break;
                case VK_END:SendMessage(hwnd,WM_VSCROLL,SB_BOTTOM,NULL);break;
                case VK_UP:
                case VK_LEFT:SendMessage(hwnd,WM_VSCROLL,SB_LINEUP,NULL);break;
                case VK_DOWN:
                case VK_RIGHT:SendMessage(hwnd,WM_VSCROLL,SB_LINEDOWN,NULL);break;
                default:break;
            }
    return 0;
                case WM_PAINT:

                hdc=BeginPaint(hwnd,&ps);
                TextOut(hdc,0,cyChar+3,szFontName,wsprintf(szFontName,_T("%s"),cFonts[vpos].szDesc));
                   // TextOut(hdc,30,30,_T("hello"),lstrlen(_T("hello")));
                   //DrawText(hdc,_T("hello"),lstrlen(_T("hello")),&rec,DT_SINGLELINE|DT_TOP|DT_LEFT);
                EndPaint(hwnd,&ps);
                    return 0;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
                             /* for messages that we don't deal with */

    }

     return DefWindowProc (hwnd, message, wParam, lParam);
}
