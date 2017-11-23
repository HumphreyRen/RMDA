//
// myTimeScale.h
// 2016-08-10
//
#pragma once

#include "..\common\common.h"

class CmyTimeScale
{
public:
	static void myCalcTimeScale(CArray<PMRDATA,PMRDATA> *pmrs,int nw,int MarL,int MarR,int& TimeCount);
	static int Time2X(PMRYS pys);
	static time_t X2Timet(int x);
	static void X2Times(int x,LPSTR sD,LPSTR sT);
	static void myDrawTimeScale(Graphics *pg,Gdiplus::Font *pfont,SolidBrush *pbrush,int y0,bool bDate); 

private:
	static PMRYS m_pys0;
    static int m_nw,m_MarL,m_MarR;
	static time_t m_t0,m_t1;
	static int m_tLen;
 
	static time_t StrTime2Binary(PMRYS pys);
	static int CTime2X(time_t t);

};
	