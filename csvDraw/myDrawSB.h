//
// myDrawSB.h
//   2016-08-04
//
#pragma once
#include <gdiplus.h>
using namespace Gdiplus;

class CmyDrawSB
{
public:
	static void myReadSB(LPSTR sFile,byte **pr,byte **pg,byte **pb,int& sbCount);

	static int  myGetSbColorIndex(float f,float fMin,float fMax,int sbCount);
	
	static void myDrawSBStrip(byte *r,byte *g,byte *b,int sbCount,int x0,int y0,int w,int h);
	static void myDrawSBScale(Graphics *pg,Gdiplus::Font *pfont,SolidBrush *pbrush,float fMin,float fMax,int x0,int y0,int h);
	
private:

};