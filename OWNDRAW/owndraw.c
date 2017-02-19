/*自定义按钮 可以自己定义按钮外观以及鼠标逻辑*/

#include <Windows.h>

#define ID_SMALLER	1
#define ID_LARGER		2
#define BTN_WIDTH	(8*cxChar)
#define BTN_HEIGHT	(4*cyChar)

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hInst;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("OwnDraw");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	hInst = hInstance;

	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = szAppName;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("the program requires windows NT!"), szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName, TEXT("Ower-Draw Button Demo"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
/*画三角形*/
void Triangle(HDC hdc, POINT pt[])
{
	SelectObject(hdc, GetStockObject(BLACK_BRUSH));	//选择黑色画刷
	Polygon(hdc, pt, 3);					//画多边形
	SelectObject(hdc, GetStockObject(WHITE_BRUSH)); //恢复正常画刷(白色)

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hwndSmaller, hwndLarger;
	static int cxClient, cyClient, cxChar, cyChar;
	int cx, cy;
	LPDRAWITEMSTRUCT pdis;
	RECT rect;
	POINT pt[3];

	switch (message)
	{
	case WM_CREATE:
		cxChar = LOWORD(GetDialogBaseUnits());//获取系统dialog 的系统字体的平均宽度和高度
		cyChar = HIWORD(GetDialogBaseUnits());
		//MessageBeep(1);
		hwndSmaller = CreateWindow(TEXT("BUTTON"), TEXT(" "),					//lpclassName 参数很重要,不能随意命名
			WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
			0, 0, BTN_WIDTH, BTN_HEIGHT,
			hwnd, (HMENU)ID_SMALLER, 
			//((LPCREATESTRUCT)lParam)->hInstance
			hInst
			, NULL);

		hwndLarger= CreateWindow(TEXT("BUTTON"), TEXT(" "),
			WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
			0, 0, BTN_WIDTH, BTN_HEIGHT,
			hwnd, (HMENU)ID_LARGER,
			hInst
			//((LPCREATESTRUCT)lParam)->hInstance
			, NULL);

		return 0;
	case WM_SIZE	:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		
		//将两个按钮移动到中间
		MoveWindow(hwndSmaller, cxClient / 2-3*BTN_WIDTH/2,//从中间左移一个半按钮宽度
			cyClient / 2-BTN_HEIGHT/2,//从中间上移半个按钮高度
			BTN_WIDTH, BTN_HEIGHT, TRUE);
		MoveWindow(hwndLarger, cxClient / 2 + BTN_WIDTH / 2,//从中间→移半按钮宽度,  /**那么两个按钮像个两个按钮宽度
			cyClient / 2 - BTN_HEIGHT / 2,//从中间上移半个按钮高度
			BTN_WIDTH, BTN_HEIGHT, TRUE);
		return 0;

	case WM_COMMAND:		
		
		//MessageBeep(0);
		//缩小%10或者放大%10																								//点击按钮产生WM_COMMAND消息
		GetWindowRect(hwnd, &rect);																					//获得hwnd 的外框矩形尺寸坐标保存在rect中,相对于屏幕坐标系左上角
		//int i = HIWORD(wParam);
		switch (wParam)																											//WM_COMMAND 消息wParam 参数包含子窗口ID,lParam 包含句柄 
		{
		case ID_SMALLER:
			rect.left += cxClient / 20;
			rect.top += cyClient / 20;
			rect.right -= cxClient / 20;
			rect.bottom -= cyClient / 20;
			break;
		case ID_LARGER:
			rect.left -= cxClient / 20;
			rect.top -= cyClient / 20;
			rect.right += cxClient / 20;
			rect.bottom += cyClient / 20;
			break;
	
		}
		MoveWindow(hwnd/*(HWND)lParam*/, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);


		return 0;

	case WM_DRAWITEM:	
																													//由需要自绘的button  发送给wndproc	
		//MessageBeep(1);
		pdis=(LPDRAWITEMSTRUCT)lParam;
		/*填充按钮区域白色,边界黑色*/
		FillRect(pdis->hDC, &(pdis->rcItem), (HBRUSH)GetStockObject(WHITE_BRUSH));	//填充rect内部 白色画刷

		FrameRect(pdis->hDC,&pdis->rcItem, (HBRUSH)GetStockObject(BLACK_BRUSH));//黑色画刷画边界
		/*画内部的三角形*/
		TextOut(pdis->hDC, pdis->rcItem.right/2, pdis->rcItem.bottom/2, TEXT("button"), lstrlen(TEXT("button")));
		cx = pdis->rcItem.right - pdis->rcItem.left;											//按钮宽度
		cy = pdis->rcItem.bottom - pdis->rcItem.top;										//按钮高度

		switch (pdis->CtlID)																						//子控件标识符
		{
		case ID_SMALLER:
			pt[0].x = 3 * cx / 8, pt[0].y = 1 *cy / 8;
			pt[1].x = 5 * cx / 8, pt[1].y = 1 * cy / 8;
			pt[2].x = 4 * cx / 8, pt[2].y = 3 * cy / 8;

			Triangle(pdis->hDC, pt);
			pt[0].x = 7 * cx / 8;  pt[0].y = 3 * cy / 8;
			pt[1].x = 7 * cx / 8;  pt[1].y = 5 * cy / 8;
			pt[2].x = 5 * cx / 8;  pt[2].y = 4 * cy / 8;

			Triangle(pdis->hDC, pt);

			pt[0].x = 5 * cx / 8;  pt[0].y = 7 * cy / 8;
			pt[1].x = 3 * cx / 8;  pt[1].y = 7 * cy / 8;
			pt[2].x = 4 * cx / 8;  pt[2].y = 5 * cy / 8;

			Triangle(pdis->hDC, pt);

			pt[0].x = 1 * cx / 8;  pt[0].y = 5 * cy / 8;
			pt[1].x = 1 * cx / 8;  pt[1].y = 3 * cy / 8;
			pt[2].x = 3 * cx / 8;  pt[2].y = 4 * cy / 8;

			Triangle(pdis->hDC, pt);
			break;

		case ID_LARGER:
			pt[0].x = 5 * cx / 8;  pt[0].y = 3 * cy / 8;
			pt[1].x = 3 * cx / 8;  pt[1].y = 3 * cy / 8;
			pt[2].x = 4 * cx / 8;  pt[2].y = 1 * cy / 8;

			Triangle(pdis->hDC, pt);

			pt[0].x = 5 * cx / 8;  pt[0].y = 5 * cy / 8;
			pt[1].x = 5 * cx / 8;  pt[1].y = 3 * cy / 8;
			pt[2].x = 7 * cx / 8;  pt[2].y = 4 * cy / 8;

			Triangle(pdis->hDC, pt);

			pt[0].x = 3 * cx / 8;  pt[0].y = 5 * cy / 8;
			pt[1].x = 5 * cx / 8;  pt[1].y = 5 * cy / 8;
			pt[2].x = 4 * cx / 8;  pt[2].y = 7 * cy / 8;

			Triangle(pdis->hDC, pt);

			pt[0].x = 3 * cx / 8;  pt[0].y = 3 * cy / 8;
			pt[1].x = 3 * cx / 8;  pt[1].y = 5 * cy / 8;
			pt[2].x = 1 * cx / 8;  pt[2].y = 4 * cy / 8;

			Triangle(pdis->hDC, pt);

			break;
		}
		if (pdis->itemState & ODS_FOCUS)
		{
			InvertRect(pdis->hDC, &pdis->rcItem);
		}
		if (pdis->itemState &ODS_FOCUS)
		{
			pdis->rcItem.left += cx / 16;
			pdis->rcItem.top += cy / 16;
			pdis->rcItem.right -= cx / 16;
			pdis->rcItem.bottom -= cy / 16;
			DrawFocusRect(pdis->hDC, &pdis->rcItem);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}