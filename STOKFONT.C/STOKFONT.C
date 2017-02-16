#include <Windows.h>
/*
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE preInstance, PSTR nCmdLine, int nShowCmd)
{
	static TCHAR szAppName[] = TEXT("StokFont");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hbrBackground = GetStockObject(WHITE_BRUSH);

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("窗口注册失败!"), szAppName, MB_OK);
		return 0;
	}

	hwnd = CreateWindow(szAppName, TEXT("Stok Fonts"), 
		WS_OVERLAPPEDWINDOW | WS_VSCROLL, 
		CW_USEDEFAULT, CW_USEDEFAULT, 
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	while (GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static struct 
	{
		int idStockFont;
		TCHAR *szStockFont;
	}
	stockfont[] = {
		OEM_FIXED_FONT,		TEXT("OEM_FIXED_FONT"),
		ANSI_FIXED_FONT,		TEXT("ANSI_FIXED_FONT"),
		ANSI_VAR_FONT,			TEXT("ANSI_VAR_FONT"),
		SYSTEM_FONT,				TEXT("SYSTEM_FONT"),
		DEVICE_DEFAULT_FONT,TEXT("DEVICE_DEFAULT_FONT"),
		SYSTEM_FIXED_FONT,TEXT("SYSTEM_FIXED_FONT"),
		DEFAULT_GUI_FONT,TEXT("DEFAULT_GUI_FONT")	};

	static int iFont, cFonts = (int)(sizeof stockfont / sizeof stockfont[0]);
	HDC hdc;
	int i ,x, y, cxGrid, cyGrid;
	PAINTSTRUCT ps;
	TCHAR szFaceName[LF_FACESIZE], szBuffer[LF_FACESIZE + 64];
	TEXTMETRIC tm;

	switch (message)
	{
	case WM_CREATE:
		SetScrollRange(hwnd, SB_VERT, 0, cFonts - 1, TRUE);
		return 0;
	case WM_DISPLAYCHANGE:
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;
	case WM_VSCROLL:

		switch (LOWORD(wParam))
		{
		case SB_TOP:			iFont = 0;		break;
		case SB_BOTTOM:	iFont = cFonts - 1; break;
		case SB_LINEUP:		iFont -= 1; break;
		case SB_LINEDOWN:iFont += 1; break;
		case SB_THUMBPOSITION:iFont = HIWORD(wParam);
		default:
			break;
		}
		iFont = max(0, min(iFont, cFonts - 1));
		SetScrollPos(hwnd, SB_VERT, iFont, TRUE);
		InvalidateRect(hwnd, NULL, TRUE);

		return 0;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_HOME: SendMessage(hwnd, WM_VSCROLL, SB_TOP, 0); break;
		case VK_END:	SendMessage(hwnd, WM_VSCROLL, SB_BOTTOM, 0); break;
		case VK_PRIOR:
		case VK_LEFT:
		case VK_UP:SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0); break;
		case VK_NEXT:
		case VK_RIGHT:
		case VK_DOWN:SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0); break;

		default:
			break;
		}
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		SelectObject(hdc, GetStockObject(stockfont[iFont].idStockFont));
		GetTextFace(hdc, LF_FACESIZE, szFaceName);
		GetTextMetrics(hdc, &tm);
		cxGrid = max(3 * tm.tmAveCharWidth, 2 * tm.tmMaxCharWidth);
		cyGrid = tm.tmHeight + 3;

		TextOut(hdc, 0, 0, szBuffer, wsprintf(szBuffer, TEXT("%s:Face Name= %s, CharSet=%i"),stockfont[iFont].szStockFont,szFaceName,tm.tmCharSet));
		SetTextAlign(hdc, TA_TOP | TA_CENTER);

		for (i=0;i<17;i++)
		{
			MoveToEx(hdc, (i + 2)*cxGrid, 3*cyGrid, NULL);
			LineTo(hdc, (i + 2)*cxGrid, 19 * cyGrid);

			MoveToEx(hdc, cxGrid, (i + 3)*cyGrid, NULL);
			LineTo(hdc, 19 * cxGrid, (i + 3)*cyGrid);
		}
		for (i=0;i<16;i++)
		{
			TextOut(hdc, (2 * i + 5)*cxGrid/2,2*cyGrid+2,szBuffer,wsprintf(szBuffer,TEXT("%X-"),i));

			TextOut(hdc, 3 * cxGrid / 2, (i + 3)*cyGrid + 2, szBuffer, wsprintf(szBuffer, TEXT("-%X"), i));

		}
		for (y=0;y<16;y++)
		
		{
			for (x = 0; x < 16; x++) { TextOut(hdc, (2 * x + 5)*cxGrid / 2, (y + 3)*cyGrid + 2, szBuffer, wsprintf(szBuffer, TEXT("%c"), 16 * x + y)); }
				
		}
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}
*/

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
}cFonts[] = {
	ANSI_FIXED_FONT,TEXT("ANSI_FIXED_FONT"),
	ANSI_VAR_FONT,TEXT("ANSI_VAR_FONT"),
	DEVICE_DEFAULT_FONT,TEXT("DEVICE_DEFAULT_FONT"),
	DEFAULT_GUI_FONT,TEXT("DEFAULT_GUI_FONT"),
	OEM_FIXED_FONT,TEXT("OEM_FIXED_FONT"),
	SYSTEM_FONT,TEXT("SYSTEM_FONT"),
	SYSTEM_FIXED_FONT,TEXT("SYSTEM_FIXED_FONT")
};
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[] = TEXT("STOCKFONT");

int WINAPI WinMain(HINSTANCE hThisInstance,
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
	wincl.style = CS_HREDRAW | CS_VREDRAW;                 /* Catch double-clicks */
														   //    wincl.cbSize = sizeof (WNDCLASSEX);

														   /* Use default icon and mouse-pointer */
	wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	//    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL;                 /* No menu */
	wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
	wincl.cbWndExtra = 0;                      /* structure or the window instance */
											   /* Use Windows's default colour as the background of the window */
	wincl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	/* Register the window class, and if it fails quit the program */
	if (!RegisterClass(&wincl))
		return 0;

	/* The class is registered, let's create the program*/
	hwnd = CreateWindow(
		/* Extended possibilites for variation */
		szClassName,         /* Classname */
		TEXT("STOCKFONT"),       /* Title Text */
		WS_OVERLAPPEDWINDOW | WS_VSCROLL, /* default window */
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
	static int iFont = (int)(sizeof cFonts / sizeof cFonts[0]);
	HDC hdc;
	PAINTSTRUCT ps;
	TEXTMETRIC tm;
	static int cxGrid, cyGrid, cxClient, cyClient;
	TCHAR szFontName[LF_FACESIZE+64], szFaceName[LF_FACESIZE],szbuffer[10];//LF_FACESIZE  32  getfacename()获得当前字体名字
	int i, y;
	static int vpos;
	static RECT rec;
	switch (message)                  /* handle the messages */
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		cxGrid = max(tm.tmAveCharWidth * 3  , 2*tm.tmMaxCharWidth);
		cyGrid = tm.tmHeight*3/2;
		GetClientRect(hwnd, &rec);
		ReleaseDC(hwnd, hdc);
		return 0;
	case WM_SIZE:
		cxClient = LOWORD(wParam);
		cyClient = HIWORD(wParam);

		SetScrollRange(hwnd, SB_VERT, 0, iFont - 1, TRUE);
		SetScrollPos(hwnd, SB_VERT, 0, TRUE);
		return 0;
	case WM_VSCROLL:
		switch (LOWORD(wParam))
		{
		case SB_TOP:
			vpos = 0;
			break;
		case SB_BOTTOM:
			vpos = iFont - 1;
			break;
		case SB_LINEDOWN:
			vpos += 1;
			break;
		case SB_LINEUP:
			vpos -= 1;
			break;
		case SB_THUMBTRACK:
			vpos = HIWORD(wParam);//wParam 低位通知码,高位位置
			break;
		default:
			break;
		}
		vpos = max(0, min(vpos, iFont - 1));
		SetScrollPos(hwnd, SB_VERT, vpos, TRUE);
		InvalidateRect(hwnd, &rec, TRUE);
		return 0;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_HOME:SendMessage(hwnd, WM_VSCROLL, SB_TOP, 0); break;
		case VK_END:SendMessage(hwnd, WM_VSCROLL, SB_BOTTOM, 0); break;
		case VK_UP:
		case VK_LEFT:SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0); break;
		case VK_DOWN:
		case VK_RIGHT:SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0); break;
		default:break;
		}
		return 0;
	case WM_PAINT:
		
		hdc=BeginPaint(hwnd, &ps);
		SelectObject(hdc, GetStockObject(cFonts[vpos].font));
		GetTextFace(hdc, LF_FACESIZE, szFaceName);//
		GetTextMetrics(hdc, &tm);
		TextOut(hdc,0,0,szFontName,wsprintf(szFontName,_T("%s: font name :%s Charset: %d"),cFonts[vpos].szDesc,szFaceName,tm.tmCharSet));
		//int cxGrid = 3 * cxChar / 2;
		//int cyGrid= 3 * cyChar / 2;
		for (i=0;i<17;i++)
		{
			MoveToEx(hdc, (i+3)*cxGrid, 2*cyGrid, NULL);
			LineTo(hdc, cxGrid*(i+3), cyGrid *19);

			MoveToEx(hdc, 2 * cxGrid, (i + 3)*cyGrid, NULL);
			LineTo(hdc, 19 * cxGrid, (i + 3)*cyGrid);
			
		}
		for (i=0;i<16;i++)
		{
			TextOut(hdc, (i + 3)*cxGrid+cxGrid/3, 2 * cyGrid+cyGrid/3, szbuffer, wsprintf(szbuffer, TEXT("%-X"), i));
			TextOut(hdc, 2 * cxGrid+cxGrid/3, (i + 3)*cyGrid+cyGrid / 3, szbuffer, wsprintf(szbuffer, TEXT("%-X"), i));
		}
		for (i=0;i<16;i++)
		{
			for (y=0;y<16;y++)
			{
				TextOut(hdc, (i + 3)*cxGrid + cxGrid / 3, (y + 3)*cyGrid + cyGrid / 3, szbuffer, wsprintf(szbuffer, TEXT("%c"), 16*i+y));
			}
		}
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
		break;
		/* for messages that we don't deal with */

	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}
