#include "stdafx.h"

#include "myDrawCsv.h"
#include "myCalcScale.h"


//绘制单站多要素廓线图
void CmyDrawCsvProfile::myDrawProfile(Graphics *pg,int gnw,int gnh,PMRDATA pdata,float *pg_fH,PFXINFO pfxinfo,PTZINFO ptz)
{
	bHaveData=false;
	fH=pg_fH;
	//设置表格宽高及边距
	m_nw=gnw/4;//宽度四等分,为每个要素表格所占宽度
	m_nh=gnh;
	m_MarL=30;	m_MarR=20;
	m_MarT=80;  m_MarB=20;	
	if(pfxinfo->nTime>=pdata->Ys.GetCount() || pdata->nProcessor==-1)
	{
		return;
	}
	//计算单站单时次各要素刻度最大值\最小值
	PMRYS pYs=pdata->Ys.GetAt(pdata->Ys.GetCount()-1-pfxinfo->nTime);
	CmyCalcScale::myCalcScaleS1T1Yn(pdata->nProcessor,pYs,fMax,fMin);
	//绘表格
	myDrawTable(pg,pYs,ptz);
	//绘要素曲线
	myDrawData(pg,pdata,pYs);
	bHaveData=true;
}

//绘制数据线
void CmyDrawCsvProfile::myDrawData(Graphics *pg,PMRDATA pdata,PMRYS pYs)
{
	Pen pen(Color(255,128,0,0),1);
	Point pts[58];
	for(int k=0;k<4;k++)
	{
		float *f=pYs->ff[pdata->nProcessor][k];
		if(f!=NULL)
		{
			int n=0;
	    	for(int i=0;i<58;i++)
        	{
			   	pts[n].X = D2X(f[i],k,k);
				pts[n].Y = H2Y(fH[i]);
				n++;
			}
        	pg->DrawLines(&pen,pts,n);
		}
	}
}

//绘制表格
void CmyDrawCsvProfile::myDrawTable(Graphics *pg,PMRYS pYs,PTZINFO ptz)
{
	char sYs[4][20]={"温度(℃)","相对湿度(%)","水汽密度(g/m^3)","液水含量(g/m^3)"};
	_bstr_t bt=(char *)"宋体";
	FontFamily  fontFamily((BSTR)bt);
	Gdiplus::Font font(&fontFamily, (float)13, FontStyleRegular, UnitPixel);//FontStyleBold
	Pen pen(Color(255,128,128,128),1);
	SolidBrush brush(Color(255,0,0,0));
	//标题
	char str[100];
	wsprintf(str,"%s站微波辐射计观测数据廓线图 时间：%s %s",ptz->zm,pYs->sDate,pYs->sTime);
	bt=(LPSTR)str;
   	pg->DrawString((BSTR)bt, -1, &Gdiplus::Font(&fontFamily, (float)16, FontStyleRegular, UnitPixel), PointF(10,10), &brush);
	//
	for(int k=0;k<4;k++)
	{
    	myDrawTableKuang(pg,&font,&pen,&brush,k,k,sYs[k]);
	}
}
void CmyDrawCsvProfile::myDrawTableKuang(Graphics *pg,Gdiplus::Font *pfont,Pen *ppen,SolidBrush *pbrush,int nYs,int nX,LPSTR sTableCap)
{
	char str[7];
	int x0,y0,x1;
	//横轴左右边界
	x0= D2X(fMin[nYs],nYs,nX); 
	x1= D2X(fMax[nYs],nYs,nX); 
	//绘制高度轴
	myDrawTableHeight(pg,pfont,ppen,pbrush,x0);
	//要素名称
 	StringFormat stringFormat;
    stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter); 
	_bstr_t bt=(LPSTR)sTableCap;
	pg->DrawString((BSTR)bt, -1, pfont, PointF((REAL)x0+(x1-x0)/2,40), &stringFormat, pbrush);
	//绘制横轴
	y0= H2Y(0);//0km
	pg->DrawLine(ppen,x0,y0,x1,y0);
	for(int i=0;i<5;i++)
	{
		float f=fMin[nYs]+ (fMax[nYs]-fMin[nYs])*i/4;
		x1=D2X(f,nYs,nX);
    	pg->DrawLine(ppen,x1,y0,x1,y0+5);//纵向间隔线
		if(nYs==0 || nYs==1)
   			wsprintf(str,"%d",(int)f);
		else if(nYs==2)
    		sprintf(str,"%.1f",f);
		else
   			sprintf(str,"%.2f",f);
    	bt=(LPSTR)str;
   		pg->DrawString((BSTR)bt, -1, pfont, PointF((REAL)x1,(REAL)y0+5), &stringFormat, pbrush);
	}
}

//绘制高度轴
void CmyDrawCsvProfile::myDrawTableHeight(Graphics *pg,Gdiplus::Font *pfont,Pen *ppen,SolidBrush *pbrush,int x0)
{
	_bstr_t bt;
	char str[5];
	int y0,y1;
	//高度
	StringFormat stringFormat;
    stringFormat.SetAlignment(Gdiplus::StringAlignmentFar); 
	y0= H2Y(0);//0km
	y1= H2Y(10);//10km
	bt=(LPSTR)"km";
	pg->DrawString((BSTR)bt, -1, pfont, PointF((REAL)x0,(REAL)y1-18), &stringFormat, pbrush);	
	pg->DrawLine(ppen,x0,y0,x0,y1);
	for(int i=0;i<=10;i++)
	{
		y1=H2Y((float)i);
		pg->DrawLine(ppen,x0-5,y1,x0,y1);
		if(i%2==0)
		{
			wsprintf(str,"%d",i);
			bt=(LPSTR)str;
			pg->DrawString((BSTR)bt, -1, pfont, PointF((REAL)x0-5,(REAL)y1-5), &stringFormat, pbrush);
		}
	}	
}

//温度\相对湿度\水汽密度\液水含量要素值转换为坐标X值
int CmyDrawCsvProfile::D2X(float f,int nYs,int nX)
{
	return (int)(m_nw*nX + m_MarL + (f-fMin[nYs])*(m_nw-m_MarL-m_MarR)/(fMax[nYs]-fMin[nYs]));
}
//高度(0-10km)转换为坐标Y值
int CmyDrawCsvProfile::H2Y(float fH)
{
	return (int)(m_nh-m_MarB-fH*(m_nh-m_MarT-m_MarB)/10);
}
//坐标X值转换为温度\相对湿度\水汽密度\液水含量要素值
float CmyDrawCsvProfile::X2D(int x,int nYs,int nX)
{
	return (fMax[nYs]-fMin[nYs]) * (x- m_nw*nX - m_MarL)/(m_nw-m_MarL-m_MarR)+fMin[nYs];
}
//坐标Y值转换为高度(0-10km)
float CmyDrawCsvProfile::Y2H(int y)
{
	return (float)10 * (m_nh-m_MarB-y) /(m_nh-m_MarT-m_MarB);
}
//获取X,Y坐标点对应的物理量信息
void CmyDrawCsvProfile::myGetDiagramData(int x,int y,LPSTR str)
{
	if(bHaveData)
	{
		RECT rc;
		POINT pt;
		char sTemp[50];
		pt.x =x;pt.y=y;
		for(int n=0;n<4;n++)
		{
			SetRect(&rc,m_nw*n+m_MarL,m_MarT,m_nw*(n+1)-m_MarR+1,m_nh-m_MarB+1);
			if(PtInRect(&rc,pt))//鼠标在要素n对应的有效区域中
			{
				float d= X2D(x,n,n);//要素值
				float h= Y2H(y);    //高度值
				if(n==0)
    				sprintf(sTemp,"温度 %.1f℃",d);
				else if(n==1)
					sprintf(sTemp,"相对湿度 %.1f%%",d);
				else if(n==2)
					sprintf(sTemp,"水汽密度 %.1fg/m^3",d);
				else 
					sprintf(sTemp,"液水含量 %.3fg/m^3",d);

				sprintf(str,"图示：高度 %.2fkm，%s",h,sTemp);
				
				break;
			}
		}
	}
}