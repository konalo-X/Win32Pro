#include<windows.h>
#include<tchar.h>
int main()
{
    TCHAR *pw=L"hello";
    TCHAR *p="hello";
    int len=_tcslen(pw);
    int len2=strlen(p);
    TCHAR ch[100];
    sprintf(ch,"%i",pw);
    puts(ch);
    return 0;
}
