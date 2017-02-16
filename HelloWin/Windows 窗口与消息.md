
##WINDOWS 一些标识符常量

windows 程序设计hellownd.c解读



	/**------------------------------------------------------------
	HELLOWIN.C --Display "Hello, windows 8! in clinet area
	------------------------------------------------------------**/
	#include<Windows.h>


	//#include<mmsystem.h>//l链接库中添加了WINMM.lib 多媒体库
	//#pragma comment(lib,"winmm.lib")


	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);//窗口过程函数    声明

	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance,
	PSTR szCmdLine, int iCmdShow)
	{
	static TCHAR szAppName[] = TEXT("HelloWin");
	HWND				hwnd;
	MSG					msg;
	WNDCLASS		wndclass;				//WNDCLASS 是一个结构体,声明一个窗口类wndclass
	/*
	原型:
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
	/* 如下是对WNDCLASS 对象 wndclass 十个字段初始化*/
		wndclass.style = CS_HREDRAW | CS_VREDRAW;//水平尺寸|垂直尺寸
		wndclass.lpfnWndProc = WndProc;//引用WndProc-- 指向基于该窗口类所有窗口对象的窗口过程地址
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = hInstance;//应用程序的一个实例句柄 winmain的一个参数
		wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);//加载图标,程序窗口左上角的图标,这里使用默认图标
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);//加载光标
		wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName = szAppName;//窗口类名称,允许用户任意命名,一般只创建一个窗口时,通常和程序名相同
	if(!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This prigram requires Windows NT!"), szAppName, MB_ICONERROR);
		return 0;
	}
	//CreateWindow 基于窗口类创建一个窗口,其返回值为创建的窗口句柄,程序中用句柄对窗口进行引用
	hwnd = CreateWindow(szAppName,//将创建的窗口与声明的窗口类关连
		TEXT("The Hello Program"),
		WS_OVERLAPPEDWINDOW,//默认的一种窗口风格之一
		CW_USEDEFAULT,//初始x坐标
		CW_USEDEFAULT,//初始y坐标
		CW_USEDEFAULT,//初始x方向尺寸
		CW_USEDEFAULT,//初始y方向尺寸
		NULL,//父窗口句柄
		NULL,//窗口菜单句柄
		hInstance,//程序实例句柄
		NULL//创建参数
		);
	ShowWindow(hwnd, iCmdShow);//显示窗口在屏幕中
	UpdateWindow(hwnd);//指示窗口的自身进行重绘
	
	while (GetMessage(&msg, NULL, 0, 0))//重消息队列获取消息
	{
		TranslateMessage(&msg);//翻译一些键盘消息
		DispatchMessage(&msg);//将消息发送给窗口过程
	}
	return msg.wParam;
	}


	LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)//窗口过程即是系统对消息的处理函数
	{
	HDC  hdc;
	PAINTSTRUCT ps;
	RECT		rect;
	TCHAR wndName[] = TEXT("NOTICE");//自定义的一个消息
	switch (message)
	{
	case WM_CREATE:
		PlaySound(TEXT("hellowin.wav"), NULL, SND_FILENAME | SND_ASYNC);//播放声音文件
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);//表明窗口绘制开始

		GetClientRect(hwnd, &rect);//获取窗口客户区的尺寸
		DrawText(hdc, TEXT("Hello,Windows 8!"), -1, &rect,//显示一个文本字符串
			DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		EndPaint(hwnd, &ps);//结束窗口绘制
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);//将"退出消息"插入消息队列
		return 0;
	case WM_LBUTTONDOWN://自己添加的一条消息
		MessageBox(NULL, TEXT("LEFT BUTTON WAS PRESSED!"), wndName, MB_ICONWARNING);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);//执行默认的消息处理,必须加上否则程序无法退出,这里只定义了三种消息,我自己添加了一条用来处理鼠标左键消息
}

```





