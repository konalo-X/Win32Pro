#include<windows.h>
#include <commdlg.h>
static OPENFILENAME ofn;


void PopFileInitialize(HWND hwnd)
{
	
	static TCHAR szFilter[] = TEXT("TEXT Files (*.TXT)\0*.txt\0")\
							TEXT("HPP Files (*.H)\0*.h\0")\
							TEXT("ALL Files (*.*)\0*.*\0\0");
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = szFilter;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = NULL;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = 0;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = TEXT("txt");
	ofn.lCustData = 0L;
	ofn.dwReserved = 0;
	ofn.FlagsEx = 0;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	ofn.pvReserved = NULL;
}
BOOL PopFileOpenDlg(HWND hwnd, PTSTR szFileName, PTSTR szTitleName)
{

	ofn.hwndOwner = hwnd;
	ofn.Flags = OFN_CREATEPROMPT|OFN_HIDEREADONLY
		;
	ofn.lpstrFile = szFileName;
	ofn.lpstrFileTitle = szTitleName;
	return GetOpenFileName(&ofn);
}

BOOL PopFileSaveDlg(HWND hwnd, PTSTR szFileName, PTSTR szTitleName)
{
	ofn.hwndOwner = hwnd;
	ofn.Flags = OFN_OVERWRITEPROMPT;
	ofn.lpstrFile = szFileName;
	ofn.lpstrFileTitle = szTitleName;

	return GetSaveFileName(&ofn);
}


BOOL PopFileRead(HWND hwndEdit, PTSTR szFileName)
{
	HANDLE hFile;
	int iFileLength, NumberOfBytesRead,iUniTest;
	PBYTE pBuffer, pText, pConv;
	BYTE bySwap;
	if (INVALID_HANDLE_VALUE == (hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL)))
		return FALSE;

	iFileLength=GetFileSize(hFile, 0);
	pBuffer = malloc(iFileLength + 2);
	ReadFile(hFile, pBuffer, iFileLength, &NumberOfBytesRead, NULL);
	pBuffer[iFileLength] = '\0';
	pBuffer[iFileLength+1] = '\0';
	//close hande
	CloseHandle(hFile);
	iUniTest = IS_TEXT_UNICODE_REVERSE_SIGNATURE | IS_TEXT_UNICODE_SIGNATURE;
	
	if (IsTextUnicode(pBuffer, iFileLength, &iUniTest))
	{
		pText = pBuffer + 2;
		iFileLength -= 2;
		if (IS_TEXT_UNICODE_REVERSE_SIGNATURE&iUniTest)
		{

				for (int i = 0; i < iFileLength/2; i++)
			{
				bySwap = ((BYTE*)pText)[2 * i];
				((BYTE*)pText)[2 * i] = ((BYTE*)pText)[2 * i + 1];
				((BYTE*)pText)[2 * i + 1] = bySwap;
			}
		}

		pConv = malloc(iFileLength + 2);
#ifndef UNICODE
		WideCharToMultiByte(CP_ACP, 0, (PWSTR)pText, -1, pConv, iFileLength + 2,NULL,NULL);
#else
		lstrcpy((PTSTR)pConv, (PTSTR)pText);
#endif // !UNICODE

	}
	
	else
	{
		pText = pBuffer;
		pConv = malloc(2 * iFileLength + 2);

#ifndef UNICODE
		lstrcpy((PSTR)pConv, (PSTR)pText);
#else
		MultiByteToWideChar(CP_ACP, 0, pText, -1, (PTSTR)pConv, iFileLength + 1);
#endif // !UNICODE

	}
	SetWindowText(hwndEdit,(PTSTR)pConv);
	free(pBuffer);
	free(pConv);
	return TRUE;
}



/*
BOOL PopFileWrite(HWND hwndEdit, PTSTR szFileName)
{
	HANDLE hFile;
	int iFileLength, NumberOfBytesWrite, iUniTest;
	PBYTE pBuffer, pText, pConv;
	BYTE bySwap;
	WORD uniMask = 0xFEFF;
	if (INVALID_HANDLE_VALUE == (hFile = CreateFile(szFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL)))
		return FALSE;

	iFileLength = GetWindowTextLength(hwndEdit);//获得略大于长度的值，表示字符长度
	//edit isnot unicode
#ifndef UNICODE
	pBuffer = malloc(iFileLength + 1);
	
#else
	pBuffer = malloc(2 * iFileLength + 2);

#endif // !UNICODE

	GetWindowText(hwndEdit, pBuffer, iFileLength + 1);
	
	pConv = malloc(2 * iFileLength + 4);
	//
	lstrcpy(pConv, pBuffer);
#ifndef UNICODE  // 0xFEFF
	WriteFile(hFile, pConv, iFileLength + 1,&NumberOfBytesWrite,0);
#else
	WriteFile(hFile, &uniMask, 2, &NumberOfBytesWrite, 0);
	WriteFile(hFile, pConv, iFileLength + 1, &NumberOfBytesWrite, 0);
#endif // !UNICODE
	CloseHandle(hFile);
	free(pConv);
	free(pBuffer);
	return TRUE;
}
*/
BOOL PopFileWrite(HWND hwndEdit, PTSTR szFileName)
{
	DWORD dwbyteWriten;
	HANDLE hFile;
	int iLength;
	PTSTR pstrBuffer;
	WORD wbyteOrderMask = 0xFEFF;

	//open file ,creating if necessary

	if (INVALID_HANDLE_VALUE == (hFile = (CreateFile(szFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL))))
	{
		return FALSE;
	}

	//get length of  characterd in the edit and allocate memory for them 

	iLength = GetWindowTextLength(hwndEdit);
	pstrBuffer=(PTSTR)malloc((iLength+1)*sizeof(TCHAR));//  ------- excellent

	if (!pstrBuffer)
	{
		CloseHandle(hFile);
		return FALSE;
	}

	//if the edit control will return inicode text,write unicode mask  to the file
#ifdef UNICODE
	WriteFile(hFile, &wbyteOrderMask, 2, &dwbyteWriten, NULL);

#endif // UNICODE
	//
	GetWindowText(hwndEdit, pstrBuffer, iLength + 1);//亮点
	WriteFile(hFile, pstrBuffer,iLength*sizeof(TCHAR), &dwbyteWriten, NULL);

	if (iLength*sizeof(TCHAR)!=dwbyteWriten)
	{
		CloseHandle(hFile);
		free(pstrBuffer);
		return FALSE;
	}
	CloseHandle(hFile);
	free(pstrBuffer);
	return TRUE;


}

/*
BOOL PopPrntPrintFile(HINSTANCE hinst, HWND hwnd, HWND hwndEdit, PTSTR pstrFileName)
{
	int iLength;
	PTSTR pBuffer;
	static DOCINFO di = { sizeof(DOCINFO),TEXT("NULL") };
	BOOL bSuccess = TRUE;
	HDC hdcPrn;
	int xPage, yPage;
	hdcPrn = getPrinterDC();
	iLength = GetWindowTextLength(hwndEdit);
	pBuffer = malloc((iLength + 1) * sizeof(TCHAR));
	GetWindowText(hwndEdit, pBuffer, iLength + 1);
	TextOut(hdcPrn, 0, 0, pBuffer, iLength);
	RestoreDC(hdcPrn, -1);
	return TRUE;
}*/