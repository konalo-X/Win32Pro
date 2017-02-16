///*相对于checker3,这里增加了键盘接口*/
//
#include<windows.h>

#define DIVISIONS 5

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildWndProc(HWND, UINT, WPARAM, LPARAM);

int idFocus = 0;
TCHAR szChidClass[] = TEXT("Checker4_Child");

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprevInstance, PSTR szCmdLine, int iCmdShow)
{

	static TCHAR szAppName[] = TEXT("Checker4");
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
		wndclass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName = szAppName;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("Program requires Windows NT!"), szAppName, MB_ICONERROR);
		return 0;
	}
	/* 注册子窗口类	*/
	wndclass.lpfnWndProc = ChildWndProc;
	wndclass.cbWndExtra = sizeof(long);
	wndclass.hIcon = NULL;
	wndclass.lpszClassName = szChidClass;

	/*  注册子窗口类*/
	RegisterClass(&wndclass);

	/*创建主窗口*/
	hwnd = CreateWindow(szAppName, TEXT("Checker4 Mouse hit-test Demo!"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
	/*转发消息*/
	while (GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

/*主窗口消息处理 窗口过程*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hwndChild[DIVISIONS][DIVISIONS];

	int x, y, cxBlock, cyBlock,i=0;

	switch (message)
	{
	case WM_CREATE :
		for (x = 0; x < DIVISIONS; x++)
			for (y = 0; y < DIVISIONS; y++)
				hwndChild[x][y] = CreateWindow(szChidClass, NULL,
					WS_VISIBLE | WS_CHILDWINDOW,
					0, 0, 0, 0,/*子窗口位置坐标(左上角)以及宽度高度*/
					hwnd,//父窗口句柄
					(HMENU)(y << 8 | x),//y左移8位 再与x按位或运算
					//(HMENU) i++,
					(HINSTANCE)GetWindowLong(hwnd, GWLP_HINSTANCE),//获得子窗口句柄
					NULL
				);
	case WM_SIZE:
		cxBlock = LOWORD(lParam)/DIVISIONS;
		cyBlock = HIWORD(lParam) / DIVISIONS;
		for (x = 0; x < DIVISIONS; x++)
			for (y = 0; y < DIVISIONS; y++)
				MoveWindow(hwndChild[x][y], x*cxBlock, y*cyBlock, cxBlock, cyBlock, TRUE);
		break;
	case WM_LBUTTONDOWN:
		MessageBeep(0);
		break;

		//主窗口获得焦点是,设置焦点到子窗口

	case WM_SETFOCUS:
		SetFocus(GetDlgItem(hwnd, idFocus));
		return 0;

		//keydown message ,change the focus window

	case WM_KEYDOWN:
		
		
		x = idFocus & 0xFF;
		y = idFocus >> 8;

		switch (wParam)
		{
		case VK_UP:y--; break;
		case VK_DOWN: y++; break;
		case VK_LEFT:x--; break;
		case VK_RIGHT:x++; break;
		case VK_HOME:x = y = 0; break;
		case VK_END:x = y = DIVISIONS - 1; break;

		default:
			break;
		}
		x = (x + DIVISIONS) % DIVISIONS;
		y = (y + DIVISIONS) % DIVISIONS;
		idFocus = y << 8 | x;
		
		
		
		SetFocus(GetDlgItem(hwnd, idFocus));

		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

/*子窗口消息处理 窗口过程*/
LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;

	switch (message)
	{
	case WM_CREATE:
		SetWindowLong(hwnd, 0, 0);
		return 0;
	case WM_KEYDOWN:

		if (wParam != VK_RETURN && wParam != VK_SPACE)
		{
			SendMessage(GetParent(hwnd), message, wParam, lParam);
			return 0;
		}

	case WM_LBUTTONDOWN:
		SetWindowLong(hwnd, 0, 1 ^ GetWindowLong(hwnd, 0));
		SetFocus(hwnd);
		InvalidateRect(hwnd, NULL, FALSE);
		return 0;

	case WM_SETFOCUS:
		idFocus = GetWindowLong(hwnd, GWL_ID);
		
	case WM_KILLFOCUS:
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		GetClientRect(hwnd, &rect);
		Rectangle(hdc, 0, 0, rect.right, rect.bottom);

		if (GetWindowLong(hwnd, 0))
		{
			MoveToEx(hdc, 0, 0, NULL);
			LineTo(hdc, rect.right, rect.bottom);
			MoveToEx(hdc,0,rect.bottom, NULL);
			LineTo(hdc, rect.right, 0);
		}
		if (hwnd == GetFocus())
		{
			rect.left += rect.right / 20;
			rect.right -= rect.left;
			rect.top += rect.bottom / 20;
			rect.bottom -= rect.top;

			SelectObject(hdc, GetStockObject(NULL_BRUSH));
			SelectObject(hdc, CreatePen(PS_DASH, 0, 0));
			Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

			DeleteObject(SelectObject(hdc, GetStockObject(BLACK_PEN)));
		}
		EndPaint(hwnd, &ps);
		break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
