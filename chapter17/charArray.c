#include<stdio.h>
inline char eatspace(){
	char ch=getchar();
	while(ch==' '|ch=='\n')
	{
		ch=getchar();
		if(ch=='q')
			break;
		if(ch!=' '&&ch!='\n')
			return ch;


	}
	return ch;
}
int main()
{
	char ch[1000]={0};
	printf("������һ�λ�:\n");
	//eatspace();
	int i=0;
	while(1)
	{
		ch[i]=eatspace();
		i++;
		if(i==20)
            break;

	}
	printf("ch=%s",ch);
	return 0;
}
