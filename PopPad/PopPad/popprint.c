
#include<Windows.h>
#include<commdlg.h>
#include"resource.h"

BOOL bUserAbort;
HWND hDlgPrint;

BOOL CALLBACK AboutDlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hwndDlg, 0);
			return TRUE;

		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK PrintDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
		EnableMenuItem(GetSystemMenu(hDlg, FALSE), SC_CLOSE, MF_GRAYED);
		return TRUE;
	case WM_COMMAND:
		bUserAbort = TRUE;
		EnableWindow(GetParent(hDlg), TRUE);
		DestroyWindow(hDlg);
		hDlgPrint = NULL;
		return TRUE;

	}
	return FALSE;
}

BOOL CALLBACK AbortProc(HDC hPrinterDC, int iCode)
{
	MSG msg;
	while (!bUserAbort && !PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (!hDlgPrint || !IsDialogMessage(hDlgPrint, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return !bUserAbort;//��ӡ�������У�����ֵ����ΪTRUE��
}

BOOL PopPrntPrintFile(HINSTANCE hInst, HWND hwnd, HWND hwndEdit, PTSTR szTitleName)
{
	static DOCINFO di = { sizeof(DOCINFO) };
	static  PRINTDLG pd;
	BOOL bSuccess;
	int yChar, iCharsPerLine, iLinesPerPage, iTotalLines, iTotalPages, iPage, iLine, iLineNum;
	PTSTR pstrBuffer;
	TCHAR szJobName[64 + MAX_PATH];
	static TEXTMETRIC tm;
	WORD iColCopy, iNoiColCopy;

	//ivoke print commom dialog box

	pd.lStructSize = sizeof(PRINTDLG);
	pd.hwndOwner = hwnd;
	pd.hDevMode = NULL;
	pd.hDevNames = NULL;
	pd.hDC = NULL;
	pd.Flags = PD_ALLPAGES	| PD_NOSELECTION|PD_RETURNDC ;
	pd.nFromPage =0;
	pd.nToPage = 0;
	pd.nMinPage = 0;
	pd.nMaxPage = 0;
	pd.nCopies =1;
	pd.hInstance = NULL;
	pd.lCustData = 0L;
	pd.lpfnPrintHook = NULL;
	pd.lpfnSetupHook = NULL;
	pd.lpPrintTemplateName = NULL;
	pd.lpSetupTemplateName = NULL;
	pd.hPrintTemplate = NULL;
	pd.hSetupTemplate = NULL;




	if (!PrintDlg(&pd))//PrintDlg ȡ�������жϣ�����ֵΪ0��
		return TRUE;
	//���������
	if (0 == (iTotalLines = SendMessage(hwndEdit, EM_GETLINECOUNT, 0, 0)))
		return TRUE;
	//����ַ�����������
	GetTextMetrics(pd.hDC, &tm);
	//����ַ����
	yChar = tm.tmExternalLeading + tm.tmHeight;
	//���ÿ�е��ַ���
	iCharsPerLine = GetDeviceCaps(pd.hDC, HORZRES) / tm.tmAveCharWidth;
	//ÿҳ������
	iLinesPerPage = GetDeviceCaps(pd.hDC, VERTRES) / yChar;
	//��ҳ��
	//iTotalPages = iTotalLines / iLinesPerPage + 1-1/ iLinesPerPage;//////////////////////////////////////////////////////////////?
	iTotalPages = (iTotalLines + iLinesPerPage - 1) / iLinesPerPage;

	//����pstrBuffer �洢ÿ�е��ַ�
	pstrBuffer = malloc((iCharsPerLine + 1) * sizeof(TCHAR));

	//��ʾ��ӡ�Ի���
	EnableWindow(hwnd, FALSE);

	bSuccess = TRUE;
	bUserAbort = FALSE;

	//���������Ի���
	hDlgPrint=CreateDialog(hInst, TEXT("printdlgbox"), hwnd, PrintDlgProc);
	//���öԻ���item����
	SetDlgItemText(hDlgPrint, IDC_FILENAME, szTitleName);//This function sets the title or text of a control in a dialog box.
	
														 
    //���ô�ӡ�ж�
	SetAbortProc(pd.hDC, AbortProc);
	//��ʼ��ӡ����

	GetWindowText(hwnd, szJobName, sizeof(szJobName));
	di.lpszDocName = szJobName;
	
	if (StartDoc(pd.hDC, &di) > 0)//��ʼ��ӡ����The StartDoc function starts a print job. 
	{
		MessageBeep(0);
		if (StartPage(pd.hDC) > 0)//The StartPage function prepares the printer driver to accept data. 
		{
			//��ӡÿҳ��ÿ��
			for (iPage = 0; iPage < iTotalPages; iPage++)
			{
				for (iLine = 0; iLine < iLinesPerPage; iLine++)
				{
					iLineNum = iPage * iLinesPerPage + iLine;
					if (iLineNum > iTotalLines) break;
					*(int*)pstrBuffer = iCharsPerLine;
					
					TextOut(pd.hDC, 0, yChar*iLine, pstrBuffer, (int)SendMessage(hwndEdit, EM_GETLINE, (WPARAM)(iLineNum), (LPARAM)pstrBuffer));

				}
				if (EndPage(pd.hDC) < 0)
				{
					bSuccess = FALSE;
					break;
				}
				if (bUserAbort)
					break;
				if (!bSuccess || bUserAbort)
					break;
			}
			//TextOut(pd.hDC, 0, 0, pstrBuffer, (int)SendMessage(hwndEdit, EM_GETLINE, 0, (LPARAM)pstrBuffer));
			//if (EndPage(pd.hDC) > 0)
			//	EndDoc(pd.hDC);
//else
			//	bSuccess = FALSE;
		}
		else
			bSuccess = FALSE;
		if (bSuccess)
			EndDoc(pd.hDC);
		if (!bUserAbort)
		{
			EnableWindow(hwnd, TRUE);
			DestroyWindow(hDlgPrint);
		}

	}
	
	free(pstrBuffer);
	DeleteDC(pd.hDC);
	return bSuccess && !bUserAbort;


}
/*
	if (StartDoc(pd.hDC, &di) > 0)
	{
		// Collation requires this loop and iNoiColCopy

		for (iColCopy = 0;
			iColCopy < ((WORD)pd.Flags & PD_COLLATE ? pd.nCopies : 1);
			iColCopy++)
		{
			for (iPage = 0; iPage < iTotalPages; iPage++)
			{
				for (iNoiColCopy = 0;
					iNoiColCopy < (pd.Flags & PD_COLLATE ? 1 : pd.nCopies);
					iNoiColCopy++)
				{
					// Start the page

					if (StartPage(pd.hDC) < 0)
					{
						bSuccess = FALSE;
						break;
					}

					// For each page, print the lines

					for (iLine = 0; iLine < iLinesPerPage; iLine++)
					{
						iLineNum = iLinesPerPage * iPage + iLine;

						if (iLineNum > iTotalLines)
							break;

						*(int *)pstrBuffer = iCharsPerLine;

						TextOut(pd.hDC, 0, yChar * iLine, pstrBuffer,
							(int)SendMessage(hwndEdit, EM_GETLINE,
							(WPARAM)iLineNum, (LPARAM)pstrBuffer));
					}

					if (EndPage(pd.hDC) < 0)
					{
						bSuccess = FALSE;
						break;
					}

					if (bUserAbort)
						break;
				}

				if (!bSuccess || bUserAbort)
					break;
			}

			if (!bSuccess || bUserAbort)
				break;
		}
	}
	else
		bSuccess = FALSE;

	if (bSuccess)
		EndDoc(pd.hDC);

	if (!bUserAbort)
	{
		EnableWindow(hwnd, TRUE);
		DestroyWindow(hDlgPrint);
	}

	free(pstrBuffer);
	DeleteDC(pd.hDC);

	return bSuccess && !bUserAbort;
}*/