// ClipBoard.cpp : ����Ӧ�ó������ڵ㡣
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

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������

// �˴���ģ���а����ĺ�����ǰ������: 
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

    // TODO: �ڴ˷��ô��롣

    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIPBOARD, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIPBOARD));

    MSG msg;

    // ����Ϣѭ��: 
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
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
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

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
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
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
            // �����˵�ѡ��: 
            switch (wmId)
            {

			case IDM_PASTE:
				//�Ӽ�����ȡ������ճ���ڴ��ڣ����ȴ򿪼�����
				OpenClipboard(hWnd);
				//�жϼ������Ƿ�����Ҫ������
				if (hGlobal = GetClipboardData(CF_TCHAR))
				{
					//���ָ��hGlobal�ڴ��ָ��
					pGlobal = (PTSTR)GlobalLock(hGlobal);
					if (pText)//���ptext �����ݣ����ͷ��ڴ棬�������
					{
						free(pText);
						pText = NULL;
					}
					pText = (PTSTR)malloc(GlobalSize(hGlobal));//���ٿռ䲢δ�ͷ�
					lstrcpy(pText, pGlobal);
					InvalidateRect(hWnd, NULL, TRUE);
				}
					CloseClipboard();
				
				return 0;
				//�������ϴ���������,һ��Ҫ�������ݲſ����ϴ�
			case IDM_CUT:
			case IDM_COPY:
				//�ж�pText�Ƿ�Ϊ��,����ǿ�,��ֱ�ӷ���
				if (!pText)
					return 0;
				//pText��������,�򿪱�ȫ�ֿռ�
				hGlobal = GlobalAlloc(GHND, (lstrlen(pText) + 1) * sizeof(TCHAR));
				//���ָ��ȫ�ֿռ��ָ��
				pGlobal = (PTSTR)GlobalLock(hGlobal);
				//ptext���ݸ��Ƶ�pglobal
				lstrcpy(pGlobal, pText);
				//����hGlobal
				GlobalUnlock(hGlobal);
				//�򿪼�����
				OpenClipboard(hWnd);
				//��ȫ���ڴ�ռ���������õ�ClipBoard
				SetClipboardData(CF_TCHAR, hGlobal);
				//�رռ�����
				CloseClipboard();
				//�ж��Ƿ���IDM_CUT��Ϣ,�����,��������ʾΪ��,������������
				if (IDM_COPY == (LOWORD(wParam)))
					return 0;
				//IDM_CUT== (LOWORD(wParam),������ִ��,���windows ��ʾ
			case IDM_CLEAR:
				if (pText)//���ptext �������ݣ���ʼ�ͷ��ڴ�
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
				//�������ݷ�ŶpText
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
            // TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
			GetClientRect(hWnd, &rect);
			if (pText!=NULL)//pText ��������
			{
				//��ʼ��Text ����
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

// �����ڡ������Ϣ�������
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
