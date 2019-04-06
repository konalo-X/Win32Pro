#include<windows.h>
#include<commdlg.h>
#include "resource.h"
#define  EDITID 1
#define  UNTITLED TEXT("无标题")

//function in popfile.c

void PopFileInitialize(HWND);
BOOL PopFileOpenDlg(HWND, PTSTR, PTSTR);
BOOL PopFileSaveDlg(HWND, PTSTR, PTSTR);
BOOL PopFileRead(HWND, PTSTR);
BOOL PopFileWrite(HWND, PTSTR);

//function in popfind.c

HWND PopFindFindDlg(HWND);
HWND PopFindReplaceDlg(HWND);
BOOL PopFindFindText(HWND, int*, LPFINDREPLACE);
BOOL PopFindReplaceTex(HWND, int*, LPFINDREPLACE);
BOOL PopFindNextText(HWND, int*);
BOOL PopFindValidFind(void);

//function in popfont.c
void PopFontInitialize(HWND);
BOOL PopFontChooseFont(HWND);
void PopFontSetFont(HWND);
void PopFontDeinitialize(void);

//function in popprint.c
BOOL PopPrntPrintFile(HINSTANCE, HWND, HWND, PTSTR);


// golbal variables
static HWND hDlgModeless;
static TCHAR szAppName[] = TEXT("记事本");
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	MSG msg;
	HWND hwnd;
	HACCEL hAccel;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = TEXT("poppad");

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This Program requires Windows NT!"), szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName, NULL, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, lpCmdLine);
	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	hAccel = LoadAccelerators(hInstance, szAppName);
	while (GetMessage(&msg,NULL,0,0))
	{
		if (hDlgModeless==NULL||!IsDialogMessage(hDlgModeless,&msg))
		{
			if (!TranslateAccelerator(hwnd,hAccel,&msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
	return msg.wParam;
}
void  DoCaption(HWND hwnd,TCHAR *szTitleName)
{
	TCHAR szCaption[MAX_PATH + 64];
	wsprintf(szCaption, TEXT("%s - %s"),szTitleName[0] ? szTitleName : UNTITLED, szAppName );
	SetWindowText(hwnd, szCaption);
}

short AskToSave(HWND hwnd, TCHAR *szTitleName)
{
	int iReturn;
	TCHAR szBuffer[MAX_PATH+64];
	wsprintf(szBuffer, TEXT("Do you want to save change to %s?"), szTitleName);
	iReturn = MessageBox(hwnd, szBuffer, szAppName, MB_YESNOCANCEL);
	if (iReturn==IDYES)
	{
		if (!SendMessage(hwnd, WM_COMMAND, IDM_FILE_SAVE, 0))
			iReturn = IDCANCEL;
	}
	return iReturn;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HINSTANCE hInst;
	static HWND hwndEdit;
	static TCHAR szFileName[MAX_PATH], szTitleName[MAX_PATH];
	static BOOL bNeedSave = FALSE;
	int iSelBegin, iSelEnd,iEnable;
	
	switch (message)
	{
	case WM_CREATE:
		hInst = ((LPCREATESTRUCT)lParam)->hInstance;
		hwndEdit = CreateWindow(TEXT("edit"), NULL, WS_CHILD  | WS_HSCROLL | WS_VSCROLL | WS_VISIBLE | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_NOHIDESEL
			, 0, 0, 0, 0, hwnd, (HMENU)EDITID, hInst, NULL);
		SendMessage(hwndEdit, EM_SETLIMITTEXT, (WPARAM)32000, 0);
		PopFileInitialize(hwnd);
		PopFontInitialize(hwndEdit);
		memset(szFileName, 0, MAX_PATH);
		memset(szTitleName, 0, MAX_PATH);
		DoCaption(hwnd, szTitleName);
		return 0;
	case WM_SIZE:
		MoveWindow(hwndEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
		return 0;
	case WM_INITMENUPOPUP:
		switch (lParam)
		{
		case 1:
			EnableMenuItem((HMENU)wParam, IDM_EDIT_UNDO, SendMessage(hwndEdit,EM_CANUNDO,0,0)?MF_ENABLED:MF_GRAYED);
			 SendMessage(hwndEdit, EM_GETSEL, (WPARAM)&iSelBegin, (LPARAM)&iSelEnd);
			 iEnable = iSelBegin == iSelEnd ? MF_GRAYED : MF_ENABLED;
			 EnableMenuItem((HMENU)wParam, IDM_EDIT_CUT, iEnable);
			 EnableMenuItem((HMENU)wParam, IDM_EDIT_COPY, iEnable);
			 EnableMenuItem((HMENU)wParam, IDM_EDIT_PASTE, IsClipboardFormatAvailable(CF_TEXT)? MF_ENABLED: MF_GRAYED);
			 EnableMenuItem((HMENU)wParam, IDM_EDIT_DELETE, iEnable );
			break;
		case 2:
			iEnable = hDlgModeless == NULL ? MF_ENABLED : MF_GRAYED;
			EnableMenuItem((HMENU)wParam, IDM_SEARCH_FIND, iEnable);
			EnableMenuItem((HMENU)wParam, IDM_SEARCH_NEXT, iEnable);
			EnableMenuItem((HMENU)wParam, IDM_SEARCH_REPLACE, iEnable);
		}
		
		

		
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_FILE_NEW:
			szTitleName[0] = '\0';
			SetWindowText(hwndEdit, TEXT(""));
			DoCaption(hwnd, szTitleName);
			return 0;
		case IDM_FILE_OPEN:
			if (bNeedSave && IDCANCEL == AskToSave(hwnd, szTitleName))
				return 0;//bNeedSave==FALSE 不需要保存并且取消，直接不反应
			if (PopFileOpenDlg(hwnd, szFileName, szTitleName))
			{
				if (!PopFileRead(hwndEdit, szFileName))
				{
					szTitleName[0] = '\0';
				    szFileName[0] = '\0';

				}
			}
			DoCaption(hwnd, szTitleName);
			bNeedSave = FALSE;
			
			return 0;
		case IDM_FILE_SAVE:
			if (bNeedSave)
				return 0;
			if (szFileName[0] == '\0')
			{
				//wsprintf(szTitleName, TEXT("*.txt"));
				wsprintf(szFileName, TEXT("*.txt"));
				if (PopFileSaveDlg(hwnd, szFileName, szTitleName))
				{
					if (!PopFileWrite(hwndEdit,szFileName))
					{
						MessageBox(hwnd, TEXT("reruires more space!"), szAppName, MB_ICONERROR);
						return 0;
					}
				}
			}
			else
				if (!PopFileWrite(hwndEdit, szFileName))
				{
					MessageBox(hwnd, TEXT("reruires more space!"), szAppName, MB_ICONERROR);
					return 0;
				}

			DoCaption(hwnd, szTitleName);
			bNeedSave = FALSE;
			return 0;
		case IDM_FILE_SAVEAS:
			if (PopFileSaveDlg(hwnd, szFileName, szTitleName))
			{
				if (!PopFileWrite(hwndEdit, szFileName))
				{
					MessageBox(hwnd, TEXT("reruires more space!"), szAppName, MB_ICONERROR);
					return 0;
				}
			}
			DoCaption(hwnd, szTitleName);
			return 0;
		case IDM_FILE_PRINT:

			PopPrntPrintFile(hInst, hwnd, hwndEdit, szTitleName);
			return 0;
		case IDM_APP_EXIT:
			PostQuitMessage(0);
			return 0;
		case IDM_EDIT_UNDO:
			SendMessage(hwndEdit, WM_UNDO, 0, 0);
			return 0;
		case IDM_EDIT_CUT:
			SendMessage(hwndEdit, WM_CUT, 0, 0);
			return 0;
		case IDM_EDIT_COPY:
			SendMessage(hwndEdit, WM_COPY, 0, 0);
			return 0;
		case IDM_EDIT_PASTE:
			SendMessage(hwndEdit, WM_PASTE, 0, 0);
			return 0;
		case IDM_EDIT_DELETE:
			SendMessage(hwndEdit, WM_CLEAR, 0, 0);
			return 0;
		case IDM_EDIT_SELECTALL:
			SendMessage(hwndEdit, EM_SETSEL, 0, -1);
			return 0;
			//message form search menu
		case IDM_SEARCH_FIND:
			hDlgModeless=PopFindFindDlg(hwnd);
			return 0;
		case IDM_SEARCH_REPLACE:
			hDlgModeless=PopFindReplaceDlg(hwnd);
			return 0;
		case IDM_FORMAT_FONT:
			
			if (PopFontChooseFont(hwnd))
				PopFontSetFont(hwndEdit);
			
			return 0;

		case IDM_APP_ABOUT:
			//hwndDlg = CreateWindow(NULL, NULL, WS_VISIBLE | WS_CHILD, 0, 0, 300, 200, hwnd, NULL, hInst, NULL);
			//对话框有自己的函数，无须createwindows
			DialogBox(hInst, TEXT("ABOUTBOX"), hwnd, AboutDlgProc);
			return 0;
		}
		break;


	case WM_CTLCOLOREDIT:
			SetTextColor((HDC)wParam, (COLORREF)color);
			//SetBkMode((HDC)wParam, TRANSPARENT);
		

		return 0;
	case WM_DESTROY:
		PopFontDeinitialize();
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

