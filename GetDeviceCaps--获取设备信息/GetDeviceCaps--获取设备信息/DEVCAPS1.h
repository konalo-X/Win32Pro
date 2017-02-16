#pragma once
#include<Windows.h>

#define NUMLINES ((int)(sizeof devcaps/sizeof devcaps[0]))

struct 
{
	int iIndex;
	TCHAR * szLabel;
	TCHAR * szDesc;
}devcaps[] =
{
	HORZSIZE,									TEXT("HORZSIZE"),									TEXT("Width in millimetres"),
	VERTSIZE,										TEXT("VERTSIZE"),										TEXT("Height in millimetres"),
	HORZRES,										TEXT("HORZRES"),										TEXT("Width in pixels"),
	VERTRES,										TEXT("VERTRES"),										TEXT("Height in raster lines: "),
	BITSPIXEL,										TEXT("BITSPIXEL"),										TEXT("Color bits per pixel"),
	PLANES,										TEXT("PLANES"),										TEXT("Numbes of color Planes"),
	NUMBRUSHES,								TEXT("NUMBRUSHES"),								TEXT("Numbers of device brushes:"),
	NUMPENS,									TEXT("NUMPENS"),									TEXT("Numbers of device pens:"),
	NUMMARKERS,								TEXT("NUMMARKERS"),								TEXT("Numbers of device markers:"),
	NUMFONTS,									TEXT("NUMFONTS"),									TEXT("Numbres of device fonts:"),
	NUMCOLORS,								TEXT("NUMCOLORS"),								TEXT("Numbers of device colors:"),
	PDEVICESIZE,								TEXT("PDEVICESIZE"),								TEXT("Size of device structure:"),
	ASPECTX,										TEXT("ASPECTX"),										TEXT("Relative width of pixels(像素的相对宽度):"),
	ASPECTY,										TEXT("ASPECTY"),										TEXT("Relative height of pixels:"),
	ASPECTXY,									TEXT("ASPECTXY"),									TEXT("Relative diagonal of pixels(对角线像素比):"),
	LOGPIXELSX,									TEXT("LOGPIXELSX"),								TEXT("Horzontal dots per inch(水平每英寸的像素点):"),
	LOGPIXELSY,									TEXT("LOGPIXELSY"),									TEXT("Vertical dots per inch(垂直每英寸的像素点):"),
	SIZEPALETTE,								TEXT("SIZEPALETTE"),								TEXT("Num of palette entries(调色板数量):"),
	NUMRESERVED,							TEXT("NUMRESERVED"),							TEXT("reserved palette entries(保留的调色板数量):"),
	COLORRES,									TEXT("COLORRES"),									TEXT("Actual color resolution(当前的分辨率):")
};