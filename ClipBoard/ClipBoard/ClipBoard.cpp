// ClipBoard.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "ClipBoard.h"

#define MAX_LOADSTRING 100
#ifdef UNICODE
#define CF_TCHAR CF_UNICODETEXT
TCHAR szDefaultText[] = TEXT("Default Text -- Unicode version");
TCHAR szCaption[] = TEXT(".ClipBoard Text Transfer - Unicode Version");
#else
#define CF_TCHAR CF_TEXT
TCHAR szDefaultText[] = TEXT("Default Text -- ASCI version");
TCHAR szCaption[] = TEXT(".ClipBoard Text Transfer - ASCI Version");
#endif // UNICODE

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIPBOARD, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIPBOARD));

    MSG msg;

    // 主消息循环: 
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIPBOARD));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CLIPBOARD);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT rect;
	HGLOBAL hGlobal;
	static PTSTR pText;
	PTSTR pGlobal;
	int iEnable;
	HDC hdc;
	PAINTSTRUCT ps;
    switch (message)
    {
	case WM_CREATE:
		SendMessage(hWnd, WM_COMMAND, (WPARAM)IDM_RESET, 0);
		return 0;
	case WM_INITMENUPOPUP:
		EnableMenuItem((HMENU)wParam, IDM_PASTE, IsClipboardFormatAvailable(CF_TCHAR)?MF_ENABLED:MF_GRAYED);
		iEnable = pText ? MF_ENABLED : MF_GRAYED;
		EnableMenuItem((HMENU)wParam, IDM_CUT, iEnable);
		EnableMenuItem((HMENU)wParam, IDM_COPY, iEnable);
		return 0;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
			int iLength = 0;
            // 分析菜单选择: 
            switch (wmId)
            {

			case IDM_PASTE:
				//从剪贴板取出数据粘贴在窗口，首先打开剪贴板
				OpenClipboard(hWnd);
				//判断剪贴板是否有需要的数据
				if (hGlobal = GetClipboardData(CF_TCHAR))
				{
					//获得指向hGlobal内存的指针
					pGlobal = (PTSTR)GlobalLock(hGlobal);
					if (pText)//如果ptext 有内容，先释放内存，清空内容
					{
						free(pText);
						pText = NULL;
					}
					pText = (PTSTR)malloc(GlobalSize(hGlobal));//开辟空间并未释放
					lstrcpy(pText, pGlobal);
					InvalidateRect(hWnd, NULL, TRUE);
				}
					CloseClipboard();
				
				return 0;
				//把内容上传到剪贴板,一定要先有内容才可以上传
			case IDM_CUT:
			case IDM_COPY:
				//判断pText是否为空,如果是空,则直接返回
				if (!pText)
					return 0;
				//pText含有内容,则开辟全局空间
				hGlobal = GlobalAlloc(GHND, (lstrlen(pText) + 1) * sizeof(TCHAR));
				//获得指向全局空间的指针
				pGlobal = (PTSTR)GlobalLock(hGlobal);
				//ptext内容复制到pglobal
				lstrcpy(pGlobal, pText);
				//解锁hGlobal
				GlobalUnlock(hGlobal);
				//打开剪贴板
				OpenClipboard(hWnd);
				//把全局内存空间的内容设置到ClipBoard
				SetClipboardData(CF_TCHAR, hGlobal);
				//关闭剪贴板
				CloseClipboard();
				//判断是否是IDM_CUT消息,如果是,则清理显示为空,不是则不作处理
				if (IDM_COPY == (LOWORD(wParam)))
					return 0;
				//IDM_CUT== (LOWORD(wParam),则向下执行,清空windows 显示
			case IDM_CLEAR:
				if (pText)//如果ptext 含有内容，则开始释放内存
				{
					free(pText);
					pText = NULL;
				}
				InvalidateRect(hWnd, NULL, TRUE);
				return 0;
			case IDM_RESET:
				//free(pText);
				//pText = NULL;
				//SendMessage(hWnd, WM_CREATE, 0, 0);
				//InvalidateRect(hWnd, NULL, TRUE);
				if (pText)//ptext!=NULL
				{
					//free memory
					free(pText);
					pText = NULL;
				}
				//alloc space for ptext;
				pText = (PTSTR)malloc((lstrlen(szDefaultText) + 1) * sizeof(TCHAR));
				//复制内容发哦pText
				lstrcpy(pText, szDefaultText);
				InvalidateRect(hWnd, NULL, TRUE);
				return 0;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            }
        }
        break;
    case WM_PAINT:
             hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
			GetClientRect(hWnd, &rect);
			if (pText!=NULL)//pText 含有内容
			{
				//开始画Text 内容
				DrawText(hdc, pText, -1, &rect, DT_WORDBREAK|DT_EXPANDTABS);
			}
            EndPaint(hWnd, &ps);
			return 0;
    case WM_DESTROY:
		if(pText)
			free(pText);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
