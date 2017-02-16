
#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable*/
TCHAR szClassName[] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain(HINSTANCE hThisInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpszArgument,
	int nCmdShow)
{
	HWND hwnd;               /* This is the handle for our window*/
	MSG messages;            /* Here messages to the application are saved*/
	WNDCLASSEX wincl;        /* Data structure for the windowclass */

							 /* The Window structure*/
	wincl.hInstance = hThisInstance;
	wincl.lpszClassName = szClassName;
	wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows*/
	wincl.style = CS_DBLCLKS;                 /* Catch double-clicks*/
	wincl.cbSize = sizeof(WNDCLASSEX);

	/* Use default icon and mouse-pointer */
	wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL;                 /* No menu */
	wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
	wincl.cbWndExtra = 0;                      /* structure or the window instance */
											   /* Use Windows's default colour as the background of the window */
	wincl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	/* Register the window class, and if it fails quit the program */
	if (!RegisterClassEx(&wincl))
		return 0;

	/* The class is registered, let's create the program*/
	hwnd = CreateWindowEx(
		0,                   /* Extended possibilites for variation */
		szClassName,         /* Classname */
		_T("Code::Blocks Template Windows App"),       /* Title Text */
		WS_OVERLAPPEDWINDOW, /* default window */
		CW_USEDEFAULT,       /* Windows decides the position */
		CW_USEDEFAULT,       /* where the window ends up on the screen */
		544,                 /* The programs width */
		375,                 /* and height in pixels */
		NULL,        /* The window is a child-window to desktop */
		NULL,                /* No menu */
		hThisInstance,       /* Program Instance handler */
		NULL                 /* No Window Creation data */
	);

	/* Make the window visible on the screen */
	ShowWindow(hwnd, nCmdShow);

	/* Run the message loop. It will run until GetMessage() returns 0 */
	while (GetMessage(&messages, NULL, 0, 0))
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

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int cxChar, cyChar, cxCaps, cxClient, cyClient, cLinesMax, cLines,pos,itype;
	static PMSG pmsg;
	static RECT rect;
	HDC hdc;
	TEXTMETRIC tm;
	PAINTSTRUCT ps;
	int i, y;
	static TCHAR * KEYNAME;
	static TCHAR VK, vkbuf[32];
	static int externmsg;
	static TCHAR* format[2] = { TEXT("%c"),TEXT("%5s") };
	TCHAR name[20] =  _T("hello") ;
	HGDIOBJ old;
	switch (message)                  /* handle the messages */
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		hdc=GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hwnd,hdc);
		return 0;
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_SYSCHAR:
		case WM_CHAR:
		case WM_DEADCHAR:
		case WM_SYSDEADCHAR:


		if (KEYNAME != NULL)
		{

			free(KEYNAME);
		}
		KEYNAME = (TCHAR *)malloc(32 * sizeof(TCHAR));
		//KEYNAME="hello";
		GetKeyNameText(lParam, KEYNAME, lstrlen(KEYNAME));
		VK = wParam;
		itype = (
			message == WM_SYSKEYDOWN ||
			message == WM_SYSKEYUP ||
			message == WM_SYSDEADCHAR ||
			message==WM_DEADCHAR

			);
		externmsg = lParam;
		pos++;
		GetClientRect(hwnd, &rect);
		InvalidateRect(hwnd, &rect, FALSE);
		return 0;
	case WM_PAINT:

		hdc = BeginPaint(hwnd, &ps);
		 old=SelectObject (hdc, (HGDIOBJ)GetStockObject(SYSTEM_FIXED_FONT));
		for (i = cyClient / cyChar; i >= 0; i--) {
			y = cyChar*(cyClient / cyChar - pos);
			TextOut(hdc, 0, y, KEYNAME, lstrlen(KEYNAME));
			TextOut(hdc, 3 * cxCaps, y, vkbuf, wsprintf(vkbuf,TEXT("%12c"),VK));
			TextOut(hdc, 5 * cxCaps, y, vkbuf, wsprintf(vkbuf, _T("%c%d%5d%d%8x%16d"), externmsg));
		}


		SelectObject(hdc, old);

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
		return 0;
	default:                      /* for messages that we don't deal with */
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}

