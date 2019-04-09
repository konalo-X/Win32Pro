
#include<Windows.h>
#include<commdlg.h>
#include<tchar.h>

#define MAX_STRING_LEN 256
TCHAR strFindText[MAX_STRING_LEN];
TCHAR strReplaceText[MAX_STRING_LEN];
static FINDREPLACE fp;
HWND PopFindFindDlg(HWND hwnd)
{
	static FINDREPLACE fp;
	fp.Flags = FR_HIDEUPDOWN| FR_HIDEMATCHCASE |FR_HIDEWHOLEWORD;
	fp.hInstance = NULL;
	fp.hwndOwner = hwnd;
	fp.lCustData = NULL;
	fp.lpfnHook = NULL;
	fp.lpstrFindWhat = strFindText;
	fp.lpstrReplaceWith = NULL;
	fp.lpTemplateName = NULL;
	fp.lStructSize = sizeof(FINDREPLACE);
	fp.wFindWhatLen = MAX_STRING_LEN;
	fp.wReplaceWithLen = 0;
	return FindText(&fp);
}
HWND PopFindReplaceDlg(HWND hwnd)
{
	static FINDREPLACE fp;
	fp.Flags = FR_HIDEUPDOWN | FR_HIDEMATCHCASE | FR_HIDEWHOLEWORD;
	fp.hInstance = NULL;
	fp.hwndOwner = hwnd;
	fp.lCustData = NULL;
	fp.lpfnHook = NULL;
	fp.lpstrFindWhat = strFindText;
	fp.lpstrReplaceWith = strReplaceText;
	fp.lpTemplateName = NULL;
	fp.lStructSize = sizeof(FINDREPLACE);
	fp.wFindWhatLen = MAX_STRING_LEN;
	fp.wReplaceWithLen = MAX_STRING_LEN;
	return ReplaceText(&fp);

}

BOOL PopFindFindText(HWND hwndEdit, int* piSearchOffset, LPFINDREPLACE pfr)
{
	
	int iLength,iPos;
	PTSTR pstrDoc,pstrPos;
	//获得edit控件里字符长度
	iLength = GetWindowTextLength(hwndEdit);

	//开辟空间，设置起始文件指针
	if (NULL== (pstrDoc = (PTSTR)malloc((iLength + 1) * sizeof(TCHAR)) ))
		return FALSE;
	GetWindowText(hwndEdit, pstrDoc, iLength + 1);

	pstrPos =_tcsstr(pstrDoc+ *piSearchOffset, pfr->lpstrFindWhat);//Return a pointer to the first occurrence of a search string in a string. 
	
	free(pstrDoc);
	if (pstrPos == NULL)
		return FALSE;
	iPos = pstrPos - pstrDoc;
	*piSearchOffset = iPos + lstrlen(pfr->lpstrFindWhat);//*piSearchOffset  要查找的字符串末尾想对于起点的偏移量
	SendMessage(hwndEdit, EM_SETSEL, iPos, *piSearchOffset);
	SendMessage(hwndEdit, EM_SCROLLCARET, 0, 0);
	return TRUE;
	
	
}
BOOL PopFindReplaceTex(HWND hwndEdit, int* piSearchOffset, LPFINDREPLACE pfr)
{
	if (PopFindFindText(hwndEdit,piSearchOffset, pfr))
	{
		SendMessage(hwndEdit, EM_REPLACESEL, 0,(LPARAM) pfr->lpstrReplaceWith);
		return TRUE;
	}
		
	else
		return FALSE;

}
BOOL PopFindNextText(HWND hwndEdit, int*piSearchOffset)
{
	FINDREPLACE fr;
	fr.lpstrFindWhat = strFindText;
		return PopFindFindText(hwndEdit, piSearchOffset, &fr);
	return FALSE;
}
BOOL PopFindValidFind(void)
{
	return *strFindText != '\0';
}