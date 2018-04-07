#include"resource.h"


/*
typedef struct tagOFNA {
	DWORD lStructSize;
	HWND hwndOwner;
	HINSTANCE hInstance;
	LPCSTR lpstrFilter;
	LPSTR lpstrCustomFilter;
	DWORD nMaxCustFilter;
	DWORD nFilterIndex;
	LPSTR lpstrFile;
	DWORD nMaxFile;
	LPSTR lpstrFileTitle;
	DWORD nMaxFileTitle;
	LPCSTR lpstrInitialDir;
	LPCSTR lpstrTitle;
	DWORD Flags;
	WORD nFileOffset;
	WORD nFileExtension;
	LPCSTR lpstrDefExt;
	DWORD lCustData;
	LPOFNHOOKPROC lpfnHook;
	LPCSTR lpTemplateName;
#if (_WIN32_WINNT >= 0x0500)
	void * pvReserved;
	DWORD dwReserved;
	DWORD FlagsEx;
#endif
} OPENFILENAMEA,*LPOPENFILENAMEA;
 (_WIN32_WINNT >= 0x0500)
 */


static OPENFILENAME ofn;

void PopFileInitialize(HWND hwnd)
{
    static TCHAR szFilter[]=TEXT("Text Files (*.TXT)\0*.txt\0")\
                            TEXT("ASCII Files (*.ASC)\0*.asc\0")\
                            TEXT("ALL Files (*.*)\0*.*\0");
    ofn.lStructSize=sizeof(OPENFILENAME);
    ofn.hwndOwner=hwnd;
    ofn.hInstance;
    ofn.lpstrFilter=szFilter;
    ofn.lpstrCustomFilter=NULL;
    ofn.nMaxCustFilter=0;
    ofn.nFilterIndex=0;
    ofn.lpstrFile=NULL;     ///set in open and close function
    ofn.nMaxFile=MAX_PATH;
    ofn.lpstrFileTitle=NULL;///set in open and close function
    ofn.nMaxFileTitle=MAX_PATH;
    ofn.lpstrInitialDir=NULL;
    ofn.lpstrTitle=NULL;
    ofn.Flags=0;               ///set in open and close function
    ofn.nFileOffset=0;
    ofn.nFileExtension=0;
    ofn.lpstrDefExt=TEXT("txt");
    ofn.lCustData=0L;
    ofn.lpfnHook=NULL;
    ofn.lpTemplateName=NULL;
}



BOOL PopFileOpenDlg(HWND hwnd,PTSTR pstrFileName,PTSTR pstrTitleName)
{
    ofn.hwndOwner=hwnd;
    ofn.lpstrFile=pstrFileName;
    ofn.Flags=OFN_HIDEREADONLY|OFN_CREATEPROMPT;
    return GetOpenFileName(&ofn);
    return 1;

}
BOOL PopFileSaveDlg(HWND hwnd,PTSTR pstrFileName,PTSTR pstrTitleName ){}
BOOL PopFileRead(HWND hwnd,PTSTR pstrFileName)
{

}
BOOL PopFileWrite(HWND hwnd,PTSTR pstrFileName){}

