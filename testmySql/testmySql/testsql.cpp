#include "resource.h"
#include <windows.h>
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <mysql.h>

#include <iostream>  
#pragma comment(lib,"libmysql.lib")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	static TCHAR szAppName[] = TEXT("colorscroll");
	static HWND hDlg;
	WNDCLASS wndclass;
	MSG msg;
	HWND hwnd;

	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = CreateSolidBrush(0L);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClass(&wndclass))
		return 0;

	hwnd = CreateWindow(szAppName, TEXT("Color ScrollBars"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	//ShowWindow(hwnd, nShowCmd);
	//UpdateWindow(hwnd);
	hDlg = CreateDialogParam(hInstance, TEXT("DIALOG"), hwnd, (DLGPROC)DlgProc,NULL);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (hDlg == 0 || !IsDialogMessage(hDlg, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}
BOOL CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	switch (message)
	{
		TCHAR str[16];
	case  WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			
			GetWindowText(GetDlgItem(hDlg,IDC_USER),str,16);
			MYSQL mysql;    //一个数据库结构体
			MYSQL_RES* res=NULL; //一个结果集结构体
			MYSQL_ROW row;  //char** 二维数组，存放一条条记录
							//初始化数据库
			mysql_init(&mysql);
			//设置编码方式
			mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "UTF-8");
			//连接数据库
			mysql_real_connect(&mysql, TEXT("localhost"), TEXT("root"), "xg1992321", "userInfo", 3306, NULL, 0);
			//查询数据
			mysql_query(&mysql, "select * from userInfo");
			//printf("ID  书名              作者          出版社     出版时间       售价\n");
			while (row = mysql_fetch_row(res))
			{	
				
			}
			//释放结果集
			mysql_free_result(res);
			//关闭数据库
			mysql_close(&mysql);
			return TRUE;

		}
		break;
	default:
		break;
	}
	
	return FALSE;
}