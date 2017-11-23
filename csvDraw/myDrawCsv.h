//
// myDrawCsv.h
//
#pragma once
#include "..\common\common.h"
#include "..\lib\lkDzx.h"

typedef struct
{
	HANDLE   hFinish;     //线程结束标志
	bool bNullData;       //数据是否为空
	LPVOID pdraw;
  	CArray<PMRDATA,PMRDATA> *pmrs;
	int nYs;
	int *nX;//时间方向各时间X坐标值
	int *nY;//高度方向各高度Y坐标值
	float **pd;
	CmyDZX *pmydzx;
}ThrdPARAM,*PThrdPARAM;

class CDrawCsv
{
public:
	CDrawCsv();
	virtual ~CDrawCsv();
	virtual void myGetDiagramData(int x,int y,LPSTR str)=0;

	int m_nw,m_nh,m_MarL,m_MarR,m_MarT,m_MarB; //表格宽高及左右上下边距
	float fMax[4],fMin[4];  //温度相对湿度水汽密度液水含量刻度最大最小值
	bool bHaveData;  //
	CArray<PMRDATA,PMRDATA> *pmrs;
	float  *fH;
};
//多站单要素对比图
class CmyDrawCsvProfile2 : public CDrawCsv
{
public:
	CmyDrawCsvProfile2(){};
	virtual ~CmyDrawCsvProfile2(){};
	void myDrawProfile2(Graphics *pg,int gnw,int gnh,CArray<PMRDATA,PMRDATA> *pmrs,float *pg_fH,PFXINFO pfx,CArray<PTZINFO,PTZINFO> *ptzs,LPSTR sTime0);
	virtual void myGetDiagramData(int x,int y,LPSTR str);

	PFXINFO pfx;
	int  col_count;//每行列数
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
//单站多要素廓线图
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
//单站各要素时间曲线图
class CmyDrawCsvEvolve2 : public CDrawCsv
{
public:
	CmyDrawCsvEvolve2(){};
	virtual ~CmyDrawCsvEvolve2(){};
    void myDrawEvolve2(Graphics *pg,int gnw,int gnh,CArray<PMRDATA,PMRDATA> *pmrs,PTZINFO ptz);
	virtual void myGetDiagramData(int x,int y,LPSTR str);
private:
	int  m_TimeCount;   //时间数量
	int  m_MarTop;      //上边距
	float fMaxS[7],fMinS[7];   //地面气压,地面温度,地面湿度,云底温度,云底高度,积分液态水,积分水汽含量, 刻度最大最小值
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
////单站多要素时空分布图
class CmyDrawCsvEvolve : public CDrawCsv
{
public:
	CmyDrawCsvEvolve(){};
	virtual ~CmyDrawCsvEvolve(){};
    void myDrawEvolve(Graphics *pg,int gnw,int gnh,CArray<PMRDATA,PMRDATA> *pmrs,float *pg_fH,PFXINFO pfxinfo,PTZINFO ptz);
	virtual void myGetDiagramData(int x,int y,LPSTR str);
private:
	float m_MaxHigh;  //高度最大值
	int   m_HighCount;//高度数量
	int   m_TimeCount;//时间数量
	int   m_xnum; //抽样后X方向实际格点数
	int   m_nz;   //抽样比
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

//单站设备运行状态曲线图
class CmyDrawCsvHs : public CDrawCsv
{
public:
	CmyDrawCsvHs(){};
	virtual ~CmyDrawCsvHs(){};
    void myDrawHs(Graphics *pg,int gnw,int gnh,CArray<PHSTATUS,PHSTATUS> *phs,PTZINFO ptz);
	virtual void myGetDiagramData(int x,int y,LPSTR str);
private:
	int  m_TimeCount;   //时间数量
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