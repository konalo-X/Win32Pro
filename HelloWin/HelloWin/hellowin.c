/*------------------------------------------------------------
HELLOWIN.C --Display "Hello, windows 8! in clinet area
------------------------------------------------------------*/
#include<Windows.h>
//#include<mmsystem.h>//l���ӿ��������WINMM.lib ��ý���
//#pragma comment(lib,"winmm.lib")


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);//���ڹ���     ����    ����

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance,
	PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("HelloWin");
	HWND				hwnd;
	MSG					msg;
	WNDCLASS		wndclass;				//WNDCLASS ��һ���ṹ��  ����
	/*
	ԭ��: WNDCLASS
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
	
	ԭ��:MSG
	typedef struct tagMSG {     // msg
	HWND hwnd;
	UINT message;
	WPARAM wParam;
	LPARAM lParam;
	DWORD time;
	POINT pt;
	} MSG;


	*/

	/* �����Ƕ�WNDCLASS ���� wndclass�Ǹ��ֶγ�ʼ��*/
		wndclass.style = CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc = WndProc;//����WndProc
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = hInstance;
		wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);//����ͼ��
		wndclass.hCursor = LoadCursor(NULL, IDC_APPSTARTING);//���ع��
		wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName = szAppName;
	if(!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This prigram requires Windows NT!"), szAppName, MB_ICONERROR);
		return 0;
	}
	//CreateWindow ���ڴ����ഴ��һ������
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
	
		
	//ShowWindow(hwnd, iCmdShow);//��ʾ��������Ļ��
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);//ָʾ���ڵ���������ػ� ��һ����Ϣwin_paint
	
	while (GetMessage(&msg, NULL, 0, 0))//����Ϣ���л�ȡ��Ϣ
	{
		TranslateMessage(&msg);//����һЩ������Ϣ
		DispatchMessage(&msg);//����Ϣ����windows ,��windows���ٽ���Ϣ���͸����ڹ���
	}
	return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)//msgǰ�ĸ�����
{
	HDC  hdc;//�豸���
	PAINTSTRUCT ps;
	RECT		rect;
	
	hdc = GetDC(hwnd);
	//hdc = GetWindowDC(hwnd);
	long N = 0;
	TEXTMETRIC tt;
	
	
	TCHAR wndName[] = TEXT("NOTICE");//�Զ����һ����Ϣ
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
		//PlaySound(TEXT("hellowin.wav"), NULL, SND_FILENAME | SND_ASYNC);//���������ļ�
		return 0;
		
	case WM_PAINT:
		// hdc = BeginPaint(hwnd, &ps);//�������ڻ��ƿ�ʼ

		hdc=BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);//��ȡ���ڿͻ����ĳߴ�
		DrawText(hdc, TEXT("Hello,Windows 8!"), -1, &rect,//��ʾһ���ı��ַ���
		DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	TextOut(hdc, 300, 300, mess, wcslen(mess));
		EndPaint(hwnd, &ps);//�������ڻ���
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);//��"�˳���Ϣ"������Ϣ����
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);//ִ��Ĭ�ϵ���Ϣ����,������Ϸ�������޷��˳�
}

