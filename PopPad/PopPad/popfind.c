
#include<Windows.h>
#include<commdlg.h>
#define MAX_STRING_LEN 256
TCHAR strFindText[MAX_STRING_LEN];
TCHAR strReplaceText[MAX_STRING_LEN];
static FINDREPLACE fp;
HWND PopFindFindDlg(HWND hwnd)
{
	fp.Flags = FR_HIDEUPDOWN|FR_HIDEMATCHCASE|FR_HIDEWHOLEWORD;
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
	fp.wReplaceWithLen = 0;
	return ReplaceText(&fp);

}

BOOL PopFindFindText(HWND hwndEdit, int* , LPFINDREPLACE)
{

}
BOOL PopFindReplaceTex(HWND, int*, LPFINDREPLACE);
BOOL PopFindNextText(HWND, int*);
BOOL PopFindValidFind(void);