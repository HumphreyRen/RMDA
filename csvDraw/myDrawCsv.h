//
// myDrawCsv.h
//
#pragma once
#include "..\common\common.h"
#include "..\lib\lkDzx.h"

typedef struct
{
	HANDLE   hFinish;     //�߳̽�����־
	bool bNullData;       //�����Ƿ�Ϊ��
	LPVOID pdraw;
  	CArray<PMRDATA,PMRDATA> *pmrs;
	int nYs;
	int *nX;//ʱ�䷽���ʱ��X����ֵ
	int *nY;//�߶ȷ�����߶�Y����ֵ
	float **pd;
	CmyDZX *pmydzx;
}ThrdPARAM,*PThrdPARAM;

class CDrawCsv
{
public:
	CDrawCsv();
	virtual ~CDrawCsv();
	virtual void myGetDiagramData(int x,int y,LPSTR str)=0;

	int m_nw,m_nh,m_MarL,m_MarR,m_MarT,m_MarB; //����߼��������±߾�
	float fMax[4],fMin[4];  //�¶����ʪ��ˮ���ܶ�Һˮ�����̶������Сֵ
	bool bHaveData;  //
	CArray<PMRDATA,PMRDATA> *pmrs;
	float  *fH;
};
//��վ��Ҫ�ضԱ�ͼ
class CmyDrawCsvProfile2 : public CDrawCsv
{
public:
	CmyDrawCsvProfile2(){};
	virtual ~CmyDrawCsvProfile2(){};
	void myDrawProfile2(Graphics *pg,int gnw,int gnh,CArray<PMRDATA,PMRDATA> *pmrs,float *pg_fH,PFXINFO pfx,CArray<PTZINFO,PTZINFO> *ptzs,LPSTR sTime0);
	virtual void myGetDiagramData(int x,int y,LPSTR str);

	PFXINFO pfx;
	int  col_count;//ÿ������
	int  m_MarTop;
private:
	void myDrawData(Graphics *pg);
	void myDrawTable(Graphics *pg,CArray<PTZINFO,PTZINFO> *ptzs,LPSTR sTime);
    void myDrawTableKuang(Graphics *pg,Gdiplus::Font *pfont,Pen *ppen,SolidBrush *pbrush,int nYs,int nX,LPSTR sTableCap);
    void myDrawTableHeight(Graphics *pg,Gdiplus::Font *pfont,Pen *ppen,SolidBrush *pbrush,int x0,int n);  
	
	int D2X2(float f,int nYs,int nX);
	int H2Y2(float fH,int n);
	float X2D2(int x,int nYs,int nX);
	float Y2H2(int y,int n);
};
//��վ��Ҫ������ͼ
class CmyDrawCsvProfile : public CDrawCsv
{
public:
	CmyDrawCsvProfile(){};
	virtual ~CmyDrawCsvProfile(){};
	void myDrawProfile(Graphics *pg,int gnw,int gnh,PMRDATA pdata,float *pg_fH,PFXINFO pfxinfo,PTZINFO ptz);
	virtual void myGetDiagramData(int x,int y,LPSTR str);
private:
	void myDrawData(Graphics *pg,PMRDATA pdata,PMRYS pYs);
	void myDrawTable(Graphics *pg,PMRYS pYs,PTZINFO ptz);

	void myDrawTableKuang(Graphics *pg,Gdiplus::Font *pfont,Pen *ppen,SolidBrush *pbrush,int nYs,int nX,LPSTR sTableCap);
    void myDrawTableHeight(Graphics *pg,Gdiplus::Font *pfont,Pen *ppen,SolidBrush *pbrush,int x0);

	int D2X(float f,int nYs,int nX);
	int H2Y(float fH);
	float X2D(int x,int nYs,int nX);
	float Y2H(int y);
};
//��վ��Ҫ��ʱ������ͼ
class CmyDrawCsvEvolve2 : public CDrawCsv
{
public:
	CmyDrawCsvEvolve2(){};
	virtual ~CmyDrawCsvEvolve2(){};
    void myDrawEvolve2(Graphics *pg,int gnw,int gnh,CArray<PMRDATA,PMRDATA> *pmrs,PTZINFO ptz);
	virtual void myGetDiagramData(int x,int y,LPSTR str);
private:
	int  m_TimeCount;   //ʱ������
	int  m_MarTop;      //�ϱ߾�
	float fMaxS[7],fMinS[7];   //������ѹ,�����¶�,����ʪ��,�Ƶ��¶�,�Ƶ׸߶�,����Һ̬ˮ,����ˮ������, �̶������Сֵ
	//
	void myCalcScaleEvolve2(CArray<PMRDATA,PMRDATA> *pmrs);
	//
	void myDrawData(Graphics *pg,CArray<PMRDATA,PMRDATA> *pmrs,int *nX);
	void myFillCsvEO(Graphics *pg,CArray<PMRDATA,PMRDATA> *pmrs,int *nX);
	//
	void myDrawTable(Graphics *pg,PTZINFO ptz);
	void myDrawTableKuang(Graphics *pg,Gdiplus::Font *pfont,Pen *ppen,SolidBrush *pbrush,int x0,int y0,int w,int h,int nYs);
	//
	int D2Y(float f,int nYs);
};
////��վ��Ҫ��ʱ�շֲ�ͼ
class CmyDrawCsvEvolve : public CDrawCsv
{
public:
	CmyDrawCsvEvolve(){};
	virtual ~CmyDrawCsvEvolve(){};
    void myDrawEvolve(Graphics *pg,int gnw,int gnh,CArray<PMRDATA,PMRDATA> *pmrs,float *pg_fH,PFXINFO pfxinfo,PTZINFO ptz);
	virtual void myGetDiagramData(int x,int y,LPSTR str);
private:
	float m_MaxHigh;  //�߶����ֵ
	int   m_HighCount;//�߶�����
	int   m_TimeCount;//ʱ������
	int   m_xnum; //������X����ʵ�ʸ����
	int   m_nz;   //������
	int   m_sbCount;  //
	byte  *r,*g,*b;   //

	//
	static void myEvolveThrd(void * pParam);
	static void myCalcXZ(int nYs,float fMax,float fMin,CArray<float,float&> *pxzs);
	//
	void myDrawXZ(Graphics *pg,Gdiplus::Font *pfont,SolidBrush *pbrush,int nYs,float xz,int x,int y);
	void myDrawDzx(Graphics *pg,CArray<PDZX,PDZX> *pdzxs,int nYs);
	void myFillCsvEvolve(float **pd,int *nX,int *nY,float fMin,float fMax,int sbCount);
	//
	void myDrawTable(Graphics *pg,PTZINFO ptz);
	void myDrawTableKuang(Graphics *pg,Gdiplus::Font *pfont,Pen *ppen,SolidBrush *pbrush,int x0,int y0,int w,int h,int nYs);
	//
	int High2Y(float fH,int nYs);
	float Y2High(int y,int nYs);

};

//��վ�豸����״̬����ͼ
class CmyDrawCsvHs : public CDrawCsv
{
public:
	CmyDrawCsvHs(){};
	virtual ~CmyDrawCsvHs(){};
    void myDrawHs(Graphics *pg,int gnw,int gnh,CArray<PHSTATUS,PHSTATUS> *phs,PTZINFO ptz);
	virtual void myGetDiagramData(int x,int y,LPSTR str);
private:
	int  m_TimeCount;   //ʱ������
	//
	void myDrawData(Graphics *pg,CArray<PHSTATUS,PHSTATUS> *phs,int *nX);
	//
	void myDrawTable(Graphics *pg,PTZINFO ptz);
	void myDrawTableKuang(Graphics *pg,Gdiplus::Font *pfont,Pen *ppen,SolidBrush *pbrush,int x0,int y0,int w,int h,int nYs);
	//
	int D2Y(float f,int nYs);
	//
	time_t m_t0,m_t1;
	int m_tLen;   

	int Time2X(PHSTATUS pys);
    void X2Times(int x,LPSTR sD,LPSTR sT);
	void  myCalcTimeScale(CArray<PHSTATUS,PHSTATUS> *phs);
	time_t StrTime2Binary(PHSTATUS pys);
	int CTime2X(time_t t);
    void myDrawTimeScale(Graphics *pg,Gdiplus::Font *pfont,SolidBrush *pbrush,int y0,bool bDate); 
};