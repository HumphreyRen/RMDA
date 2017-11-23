#include "stdafx.h"
#include "myDrawCsv.h"
#include "myCalcScale.h"


//绘制多站单要素剖面
void CmyDrawCsvProfile2::myDrawProfile2(Graphics *pg,int gnw,int gnh,CArray<PMRDATA,PMRDATA> *pmrdata,float *pg_fH,PFXINFO pfxinfo,CArray<PTZINFO,PTZINFO> *ptzs,LPSTR sTime0)
{
    bHaveData=false;
	pmrs=pmrdata;
	fH=pg_fH;
	pfx=pfxinfo;
	//设置表格宽高及边距
	m_MarTop=40;      //视图标题文本所占高度
	if(ptzs->GetCount()>7) //两行显示
	{
		col_count = (ptzs->GetCount()+1)/2; //每行列数
		m_nh=(gnh-m_MarTop)/2;              //每行高度,含上下边距
	}
	else //一行显示
	{
		col_count = ptzs->GetCount();
		m_nh=gnh-m_MarTop;
	}
	m_nw=gnw/col_count;   //按台站数等分宽度,为每个站点要素表格所占宽度,含左右边距	
	m_MarL=30;	m_MarR=20;  //表格左右边距
	m_MarT=30;  m_MarB=30;	//表格上下边距
	//根据当前选择时次sTime0计算各站点时次序号
	CmyCalcScale::myCalcTimeIndex(pmrs,sTime0);
	//计算各站单时次单要素刻度最大值和最小值
	CmyCalcScale::myCalcScaleSnT1Y1(pmrs,pfx,fMax,fMin);
	//绘表格
	myDrawTable(pg,ptzs,sTime0);
	//绘要素曲线
	myDrawData(pg);
	bHaveData=true;
}
//绘制表格2
void CmyDrawCsvProfile2::myDrawTable(Graphics *pg,CArray<PTZINFO,PTZINFO> *ptzs,LPSTR sTime)
{
	char sYs[4][20]={"温度(℃)","相对湿度(%)","水汽密度(g/m^3)","液水含量(g/m^3)"};
	_bstr_t bt=(char *)"宋体";
	FontFamily  fontFamily((BSTR)bt);
	Gdiplus::Font font(&fontFamily, (float)13, FontStyleRegular, UnitPixel); 
	Pen pen(Color(255,128,128,128),1);
	SolidBrush brush(Color(255,0,0,0));
	//标题
	char str[100];
	wsprintf(str,"微波辐射计观测数据多站对比图 要素：%s 时间：%d-%02d-%02d %s",sYs[pfx->nYs],pfx->nYear,pfx->nMonth,pfx->nDay,sTime);
	bt=(LPSTR)str;
   	pg->DrawString((BSTR)bt, -1, &Gdiplus::Font(&fontFamily, (float)16, FontStyleRegular, UnitPixel), PointF(10,10), &brush);
	
	for(int i=0;i<ptzs->GetCount();i++)
	{
		PTZINFO ptz=ptzs->GetAt(i);
	   	myDrawTableKuang(pg,&font,&pen,&brush,pfx->nYs,i,ptz->zm);
	}
}
//绘制数据线2
void CmyDrawCsvProfile2::myDrawData(Graphics *pg)
{
	Pen pen(Color(255,128,0,0),1);
	Point pts[58];
	for(int k=0;k<pmrs->GetCount();k++)
	{
		PMRDATA pmr=pmrs->GetAt(k);
		if(pmr->nTime > -1 && pmr->nProcessor >-1)
		{
			PMRYS pYs=pmr->Ys.GetAt(pmr->nTime); 
			float *f=pYs->ff[pmr->nProcessor][pfx->nYs];
			if(f!=NULL)
			{
				int n=0;
				for(int i=0;i<58;i++)
        	    {
					pts[n].Y = H2Y2(fH[i],k); 
	            	pts[n].X = D2X2(f[i],pfx->nYs,k);
					n++;
				}
        	    pg->DrawLines(&pen,pts,n);
			}
		}
	}
}

void CmyDrawCsvProfile2::myDrawTableKuang(Graphics *pg,Gdiplus::Font *pfont,Pen *ppen,SolidBrush *pbrush,int nYs,int nX,LPSTR sTableCap)
{
	char str[7];
	int x0,y0,x1;
	//横轴左右边界
	x0= D2X2(fMin[nYs],nYs,nX); 
	x1= D2X2(fMax[nYs],nYs,nX); 
	//绘制高度轴
	myDrawTableHeight(pg,pfont,ppen,pbrush,x0,nX);
	//要素名称
 	StringFormat stringFormat;
    stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter); 
	_bstr_t bt=(LPSTR)sTableCap;
	pg->DrawString((BSTR)bt, -1, pfont, PointF((REAL)x0+(x1-x0)/2,(REAL)m_MarTop + m_nh*(nX/col_count)), &stringFormat, pbrush);
	//绘制横轴
	y0= H2Y2(0,nX);//0km
	pg->DrawLine(ppen,x0,y0,x1,y0);
	for(int i=0;i<5;i++)
	{
		float f=fMin[nYs]+ (fMax[nYs]-fMin[nYs])*i/4;
		x1=D2X2(f,nYs,nX);
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
void CmyDrawCsvProfile2::myDrawTableHeight(Graphics *pg,Gdiplus::Font *pfont,Pen *ppen,SolidBrush *pbrush,int x0,int n)
{
	_bstr_t bt;
	char str[5];
	int y0,y1;
	//高度
	StringFormat stringFormat;
    stringFormat.SetAlignment(Gdiplus::StringAlignmentFar); 
	y0= H2Y2(0,n);//0km
	y1= H2Y2(10,n);//10km
	bt=(LPSTR)"km";
	pg->DrawString((BSTR)bt, -1, pfont, PointF((REAL)x0,(REAL)y1-18), &stringFormat, pbrush);	
	pg->DrawLine(ppen,x0,y0,x0,y1);
	for(int i=0;i<=10;i++)
	{
		y1=H2Y2((float)i,n);
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
int CmyDrawCsvProfile2::D2X2(float f,int nYs,int nX)
{
	return (int)(m_nw*(nX%col_count) + m_MarL + (f-fMin[nYs])*(m_nw-m_MarL-m_MarR)/(fMax[nYs]-fMin[nYs]));
}
//高度(0-10km)转换为坐标Y值
int CmyDrawCsvProfile2::H2Y2(float fH,int n)
{
	//10:10-0km
	return (int)(m_MarTop + m_nh*(n/col_count+1) - m_MarB - fH*(m_nh-m_MarT-m_MarB)/10);
}
//坐标X值转换为温度\相对湿度\水汽密度\液水含量要素值
float CmyDrawCsvProfile2::X2D2(int x,int nYs,int nX)
{
	return (fMax[nYs]-fMin[nYs]) * (x- m_nw*(nX%col_count) - m_MarL)/(m_nw-m_MarL-m_MarR)+fMin[nYs];
}
//坐标Y值转换为高度(0-10km)
float CmyDrawCsvProfile2::Y2H2(int y,int n)
{
	return (float)10 * (m_MarTop + m_nh*(n/col_count+1) -m_MarB-y) /(m_nh-m_MarT-m_MarB);
}

//获取X,Y坐标点对应的物理量信息
void CmyDrawCsvProfile2::myGetDiagramData(int x,int y,LPSTR str)
{
	if(bHaveData)
	{
		RECT rc;
		POINT pt;
		char sTemp[50];
		pt.x =x;pt.y=y;
		for(int n=0;n<pmrs->GetCount();n++)
		{
			SetRect(&rc,m_nw*(n%col_count)+m_MarL,m_MarTop + m_nh*(n/col_count)+m_MarT,m_nw*(n%col_count+1)-m_MarR+1,m_MarTop + m_nh*(n/col_count+1)-m_MarB+1);
			if(PtInRect(&rc,pt))//鼠标在要素n对应的有效区域中
			{
				float d= X2D2(x,pfx->nYs,n);
				float h= Y2H2(y,n);
				switch(pfx->nYs)
				{
				case 0:
    				sprintf(sTemp,"温度 %.1f℃",d);
					break;
				case 1:
					sprintf(sTemp,"相对湿度 %.1f%%",d);
					break;
				case 2:
					sprintf(sTemp,"水汽密度 %.1fg/m^3",d);
					break;
				case 3:
					sprintf(sTemp,"液水含量 %.3fg/m^3",d);
					break;
				default:
					lstrcpy(sTemp,"");
					break;
				}
				sprintf(str,"图示：高度 %.2fkm，%s",h,sTemp);				
				break;
			}
		}
	}
}