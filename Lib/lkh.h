#pragma once
#include <gdiplus.h>
using namespace Gdiplus;



__declspec(dllexport) int  myGetStringsDigtal(LPCSTR str);
__declspec(dllexport) void myGetNextDay(int& nYear,int& nMonth,int& nDay,int nDir);
__declspec(dllexport) void mySavePicToFile(HDC hdc,HBITMAP hbmp,int w,int h,char bkfile[]);

__declspec(dllexport) void myGetExeuteFileName(LPSTR sFullPath,LPSTR sFileName);
