#pragma once
#include <afxtempl.h>  

typedef struct
{
	CArray<POINT,POINT&> pt;   //���Ƶ�(��Ļ����)
	CArray<POINT,POINT&> pts;  //���Ƶ�(��ֵ����Ļ����)
	float xz;  //��ֵ 
	bool  lx;  //���ͣ��Ƿ�պϣ�0Ϊ��1Ϊ��
	bool  bLL; //�Դ���㵽�յ��������۲죬���Ϊtrue;
} DZX,*PDZX;//��ֵ��


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
