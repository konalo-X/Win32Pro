#pragma once
#include<windows.h>

void drawLineDemo(HDC hdc,INT cxClient, INT cyClient) {
	Rectangle(hdc, cxClient / 8, cyClient / 8, 7 * cxClient / 8, 7 * cyClient / 8);

	MoveToEx(hdc, 0, 0, NULL);
	LineTo(hdc, cxClient, cyClient);

	MoveToEx(hdc, cxClient, 0, NULL);
	LineTo(hdc, 0, cyClient);

	Ellipse(hdc, cxClient / 8, cyClient / 8, 7 * cxClient / 8, 7 * cyClient / 8);

	RoundRect(hdc, cxClient / 4, cyClient / 4, 3 * cxClient / 4, 3 * cyClient / 4, cxClient / 4, cyClient / 4);
}