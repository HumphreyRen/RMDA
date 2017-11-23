#pragma once
#include <afxtempl.h>  

typedef struct
{
	CArray<POINT,POINT&> pt;   //控制点(屏幕坐标)
	CArray<POINT,POINT&> pts;  //控制点(插值后屏幕坐标)
	float xz;  //线值 
	bool  lx;  //类型，是否闭合，0为否，1为是
	bool  bLL; //以从起点到终点走向来观察，左大为true;
} DZX,*PDZX;//等值线


class AFX_EXT_CLASS CmyDZX
{
public:
	CmyDZX();
	virtual ~CmyDZX();
	CArray<PDZX,PDZX> dzxs;

	void myTraceDZX(float **ppd,CArray<float,float&> *pxzs,int *nX,int *nY,int xnum,int ynum);
private:
	void myReleaseMem();
 
};
