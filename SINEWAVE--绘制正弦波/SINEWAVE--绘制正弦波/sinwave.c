/*
	polyline  和polylineto:画一条由多条首尾相连的折线
*/

#include<windows.h>
#include<math.h>

#define NUM 1000

#define TWOPI				(2*3.14159)


LRESULT CALLBACK	 WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI	WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, PSTR szCmdLine, int iCmdShow)
{
	HWND	hwnd;
	MSG msg;
	WNDCLASS wndclass;
	static TCHAR szAppName[] = TEXT("SineWave");

	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("窗口类注册失败!!"), szAppName, MB_ICONERROR);
		return 0;
	}
	
	hwnd = CreateWindow(szAppName,
		TEXT("Sine Wave Using PolyLine "),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL
	);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}


LRESULT CALLBACK	 WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	POINT apt[NUM];
	int i;
	static int cxClient, cyClient;

	switch (message)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		MoveToEx(hdc, 0, cyClient / 2, NULL);
		LineTo(hdc, cxClient, cyClient / 2);

		for ( i = 0; i < NUM; i++)
		{
			apt[i].x = i*cxClient / NUM;
			apt[i].y = (int)(cyClient / 2 * (1 - sin(TWOPI*i / NUM)));
		}
		Polyline(hdc, apt, NUM);
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}
