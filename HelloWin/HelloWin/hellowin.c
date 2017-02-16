/*------------------------------------------------------------
HELLOWIN.C --Display "Hello, windows 8! in clinet area
------------------------------------------------------------*/
#include<Windows.h>
//#include<mmsystem.h>//l链接库中添加了WINMM.lib 多媒体库
//#pragma comment(lib,"winmm.lib")


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);//窗口过程     函数    声明

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance,
	PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("HelloWin");
	HWND				hwnd;
	MSG					msg;
	WNDCLASS		wndclass;				//WNDCLASS 是一个结构体  声明
	/*
	原型: WNDCLASS
	typedef struct tagWNDCLASSA
	{
	UINT			style;
	WNDPROC	lpfnWndProc;
	int  cbClsExtra;
	int cbWndExtra;
	HINSTANCE hInstance;
	HICON hIcon;
	HCURSOR hCursor;
	HBRUSH hbrBackground;
	LPCSTR	  lpszMenuName;
	LPCSTR  lpszClassName;
	}WNDCLASSA, *PWNDCLASSA,NEAR *NPWNDCLASSA,FAR*LPWNDCLASSA;
	
	原型:MSG
	typedef struct tagMSG {     // msg
	HWND hwnd;
	UINT message;
	WPARAM wParam;
	LPARAM lParam;
	DWORD time;
	POINT pt;
	} MSG;


	*/

	/* 如下是对WNDCLASS 对象 wndclass是个字段初始化*/
		wndclass.style = CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc = WndProc;//引用WndProc
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = hInstance;
		wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);//加载图标
		wndclass.hCursor = LoadCursor(NULL, IDC_APPSTARTING);//加载光标
		wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName = szAppName;
	if(!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This prigram requires Windows NT!"), szAppName, MB_ICONERROR);
		return 0;
	}
	//CreateWindow 基于窗口类创建一个窗口
	hwnd = CreateWindow(szAppName,
		TEXT("The Hello Program"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
		);
	
		
	//ShowWindow(hwnd, iCmdShow);//显示窗口在屏幕中
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);//指示窗口的自身进行重绘 第一条消息win_paint
	
	while (GetMessage(&msg, NULL, 0, 0))//重消息队列获取消息
	{
		TranslateMessage(&msg);//翻译一些键盘消息
		DispatchMessage(&msg);//将消息发送windows ,而windows会再将信息发送给窗口过程
	}
	return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)//msg前四个参数
{
	HDC  hdc;//设备句柄
	PAINTSTRUCT ps;
	RECT		rect;
	
	hdc = GetDC(hwnd);
	//hdc = GetWindowDC(hwnd);
	long N = 0;
	TEXTMETRIC tt;
	
	
	TCHAR wndName[] = TEXT("NOTICE");//自定义的一个消息
	GetTextMetrics(hdc, &tt);
	int height = tt.tmHeight + tt.tmExternalLeading;
	TCHAR buf[100] ;
	int len=wsprintf(buf, TEXT("THE NUM IS %ld"), height);
	TextOut(hdc,5,5,buf,len);
	ReleaseDC(hwnd, hdc);
	InvalidateRect(hwnd, &rect,1);
	TCHAR mess[] = TEXT("Hello Windows8.1!");


	switch (message)
	{
	case WM_LBUTTONDOWN:
		if( MessageBox(NULL, TEXT("LEFT BUTTON WAS PRESSED!"), wndName, MB_ICONWARNING)==IDOK)
			PostQuitMessage(0);
		return 0;
	case WM_CREATE:
		//PlaySound(TEXT("hellowin.wav"), NULL, SND_FILENAME | SND_ASYNC);//播放声音文件
		return 0;
		
	case WM_PAINT:
		// hdc = BeginPaint(hwnd, &ps);//表明窗口绘制开始

		hdc=BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);//获取窗口客户区的尺寸
		DrawText(hdc, TEXT("Hello,Windows 8!"), -1, &rect,//显示一个文本字符串
		DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	TextOut(hdc, 300, 300, mess, wcslen(mess));
		EndPaint(hwnd, &ps);//结束窗口绘制
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);//将"退出消息"插入消息队列
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);//执行默认的消息处理,必须加上否则程序无法退出
}

