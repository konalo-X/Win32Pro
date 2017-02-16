#include<Windows.h>
#include<stdlib.h>
#include<stdio.h>
#include<tchar.h>
int main()
{


	TCHAR ch[] = _T("Helleo");
	TCHAR buf[100] = { 0 };
	wsprintf(buf, _T("%s"), ch);
	_tprintf(_T("%s\n"), buf);
	system("pause");
	return 0;
}