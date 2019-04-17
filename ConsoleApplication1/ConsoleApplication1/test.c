#include <windows.h>
#include <stdio.h>

DEVMODE devmode;
int i = 0;
int main()
{
	ZeroMemory(&devmode, sizeof(DEVMODE));
	devmode.dmSize = sizeof(DEVMODE);

	while (EnumDisplaySettings(NULL, i++, &devmode))
	{
		printf("%d \n", i);
	}
	printf("Endl\n");
	getchar();
	return 0;
}