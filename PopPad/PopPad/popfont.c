#include<Windows.h>
#include<commdlg.h>
#include"resource.h"
static HFONT hFont;
static LOGFONT lgfont;

COLORREF co;

static CHOOSEFONT cf;

void PopFontInitialize(HWND hwndEdit)
{
	  GetObject(GetStockObject(SYSTEM_FIXED_FONT), sizeof(LOGFONT), &lgfont);
	

	hFont = CreateFontIndirect(&lgfont);
	SendMessage(hwndEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
}
BOOL PopFontChooseFont(HWND hwnd)
{
	
	cf.Flags = CF_EFFECTS;
	cf.hDC = NULL;
	cf.hInstance = NULL;
	cf.hwndOwner = hwnd;
	cf.iPointSize = 0;
	cf.lCustData = (LPARAM)NULL;
	cf.lpfnHook = NULL;
	cf.lpLogFont = &lgfont;
	cf.lpszStyle = NULL;
	cf.lpTemplateName = NULL;
	cf.lStructSize = sizeof(CHOOSEFONT);
	cf.nFontType = REGULAR_FONTTYPE;
	cf.nSizeMax = 0;
	cf.nSizeMin = 0;
	cf.rgbColors = color;
	return ChooseFont(&cf);
}
void PopFontSetFont(HWND hwndEdit) 
{
	RECT rect;
	HFONT hFontNew;
	
	hFontNew = CreateFontIndirect(&lgfont);
	SendMessage(hwndEdit, EM_SETRECT, (WPARAM)hFontNew, 0);
	DeleteObject(hFont);
	hFont = hFontNew;
	GetClientRect(hwndEdit, &rect);
	color=cf.rgbColors;
	InvalidateRect(hwndEdit, &rect, TRUE);


}
void PopFontDeinitialize(void)
{
	DeleteObject(hFont); //BOOL DeleteObject(HGDIOBJ hObject   // handle to graphic object);
}