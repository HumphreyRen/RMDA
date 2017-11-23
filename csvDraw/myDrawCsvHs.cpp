#include "stdafx.h"

#include "myDrawCsv.h"


//单站设备运行状态曲线图
void CmyDrawCsvHs::myDrawHs(Graphics *pg,int gnw,int gnh,CArray<PHSTATUS,PHSTATUS> *phs,PTZINFO ptz)
{
	bHaveData=false;
	m_TimeCount=phs->GetCount(); //时间数量
	if( m_TimeCount<1)
		return;
	//设置表格宽高及边距
	m_MarT=50;      //视图标题文本所占高度
	m_MarB=35;      //时间刻度所占高度
	m_MarL=100;  m_MarR=70;  //左右边距
	m_nw=gnw-m_MarL-m_MarR;       //每个要素表格所占宽度
	m_nh=(gnh-m_MarT-m_MarB)/10;  //高度10等分,为每个要素表格高度
	//计算最早时间值和时间跨度值(秒)
	myCalcTimeScale(phs);
	//计算时间方向各时间X坐标值
	int *nX=(int *)malloc(m_TimeCount*sizeof(int));
	for(int i=0;i<m_TimeCount;i++)
	{
		nX[i]=Time2X(phs->GetAt(i));
	}
	//绘制表格
	myDrawTable(pg,ptz);
	//绘制数据线
	myDrawData(pg,phs,nX);
	//
	free(nX);
	bHaveData=true;
}

//绘制数据线
void CmyDrawCsvHs::myDrawData(Graphics *pg,CArray<PHSTATUS,PHSTATUS> *phs,int *nX)
{
	Pen pen(Color(255,128,0,0),1);	
	Point *pts=(Point *)malloc(m_TimeCount*sizeof(Point));
	for(int k=0;k<10;k++)
	{
    	int n=0;
    	for(int i=0;i<m_TimeCount;i++)
     	{		
	     	PHSTATUS pys=phs->GetAt(i);
	     	pts[n].Y = D2Y(pys->value[k],k);
			pts[n].X = nX[i];
			n++;
		}
    	pg->DrawLines(&pen,pts,n);
	}
	free(pts);
}
//绘制表格
void CmyDrawCsvHs::myDrawTable(Graphics *pg,PTZINFO ptz)
{
	_bstr_t bt=(char *)"宋体";
	FontFamily  fontFamily((BSTR)bt);
	Gdiplus::Font font(&fontFamily, (float)13, FontStyleRegular, UnitPixel); 
	Pen pen(Color(255,128,128,128),1);
	SolidBrush brush(Color(255,0,0,0));
	//标题
	char str[100];
	wsprintf(str,"%s站微波辐射计设备运行状态时间曲线图",ptz->zm);
	bt=(LPSTR)str;
   	pg->DrawString((BSTR)bt, -1, &Gdiplus::Font(&fontFamily, (float)16, FontStyleRegular, UnitPixel), PointF(10,10), &brush);
	//10个要素表格及文字说明
	for(int i=0;i<10;i++)
	{
		myDrawTableKuang(pg,&font,&pen,&brush,m_MarL,m_MarT +m_nh*i,m_nw,m_nh,i);
	}
	//时间刻度 
	myDrawTimeScale(pg,&font,&brush,m_MarT+m_nh*10,true);
} 
//绘制表格边框
void CmyDrawCsvHs::myDrawTableKuang(Graphics *pg,Gdiplus::Font *pfont,Pen *ppen,SolidBrush *pbrush,int x0,int y0,int w,int h,int n)
{	
	char sYsL[][20]={"设备整体","主控板","接收机0","接收机1","仰角控制器","方位角控制器","GPS","红外测温仪","地面传感器","电源管理板"};
	StringFormat stringFormat;
    stringFormat.SetAlignment(Gdiplus::StringAlignmentFar);	
	//要素名
	_bstr_t bt;
	bt=(LPSTR)sYsL[n];
	pg->DrawString((BSTR)bt, -1, pfont, PointF((REAL)x0,(REAL)y0+h/2-6),&stringFormat, pbrush );
	//边框
	pg->DrawRectangle(ppen,x0,y0,w,h);   
	//纵向间隔线
	Pen pen(Color(255,128,128,128),1);
	pen.SetDashStyle(Gdiplus::DashStyleDashDot);  
	for(int i=1;i<4;i++)
	{
		int y = y0+ i* h / 4;
        //pg->DrawLine(&pen,x0,y,x0+w,y); 
		if( i==1)
		{
    		bt=(LPSTR)"良好";
   	    	pg->DrawString((BSTR)bt, -1, pfont, PointF((REAL)x0+w+2,(REAL)y-10), pbrush);
		}
		else if( i==2)
		{
    		bt=(LPSTR)"不正常";
   	    	pg->DrawString((BSTR)bt, -1, pfont, PointF((REAL)x0+w+2,(REAL)y-5), pbrush);
		}
		else
		{
			bt=(LPSTR)"故障";
   	    	pg->DrawString((BSTR)bt, -1, pfont, PointF((REAL)x0+w+2,(REAL)y), pbrush);
		}
	}	
}

//要素值转换为坐标Y值
int CmyDrawCsvHs::D2Y(float f,int nYs)
{
	return (int)(m_MarT + m_nh*nYs + (3-f)*(m_nh)/4);
}
//时间转换为坐标X值
int CmyDrawCsvHs::Time2X(PHSTATUS pys)
{
	return m_MarL+((int)(StrTime2Binary(pys)-m_t0))*(m_nw)/m_tLen;
}
//坐标X值转换为日期时间
void CmyDrawCsvHs::X2Times(int x,LPSTR sD,LPSTR sT)
{
	time_t tt=m_t0+(x-m_MarL)*m_tLen/(m_nw);
	CTime t(tt);
	sprintf(sD,"%d-%02d-%02d",t.GetYear(),t.GetMonth(),t.GetDay());
	sprintf(sT,"%02d:%02d:%02d",t.GetHour(),t.GetMinute(),t.GetSecond());
}
//计算最早最后时间值和时间跨度
void CmyDrawCsvHs::myCalcTimeScale(CArray<PHSTATUS,PHSTATUS> *phs)
{
	//最早的时间
	char sY[5],sM[3],sD[3],sH[3],smm[3],sSe[3];
	PHSTATUS pys0= phs->GetAt(0);
	sscanf(pys0->sDate,"%[^-]-%[^-]-%s",sY,sM,sD);
	sscanf(pys0->sTime,"%[^:]:%[^:]:%s",sH,smm,sSe);
	CTime t(atoi(sY),atoi(sM),atoi(sD),atoi(sH),0,0);
	m_t0=t.GetTime(); 
	//最后的时间
	PHSTATUS pys1= phs->GetAt(m_TimeCount-1);
	m_t1=StrTime2Binary(pys1); //最后的时间
	//时间跨度
	m_tLen=(int)(m_t1-m_t0);//最后的时间-最早的时间
	if(m_tLen<1) m_tLen=1;
}
//时间由字符串转换为秒数,从1970-1-1算起
time_t CmyDrawCsvHs::StrTime2Binary(PHSTATUS pys)
{
	char sY[5],sM[3],sD[3],sH[3],smm[3],sSe[3];

	sscanf(pys->sDate,"%[^-]-%[^-]-%s",sY,sM,sD);
	sscanf(pys->sTime,"%[^:]:%[^:]:%s",sH,smm,sSe);
	CTime t(atoi(sY),atoi(sM),atoi(sD),atoi(sH),atoi(smm),atoi(sSe));
	return t.GetTime(); 
}
//CTime转换为坐标X值
int CmyDrawCsvHs::CTime2X(time_t t)
{
	return m_MarL+((int)(t-m_t0))*m_nw/m_tLen;
}
//绘制时间刻度
void CmyDrawCsvHs::myDrawTimeScale(Graphics *pg,Gdiplus::Font *pfont,SolidBrush *pbrush,int y0,bool bDate)
{
    _bstr_t bt;	
	char sDate0[11],sDate[11],sH[3];
	Pen pen(Color(255,128,128,128),1);
	StringFormat stringFormat;
    stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter); 
	//初始时间
    CTime t(m_t0); 
	time_t t1;	 
	CTimeSpan ts(0,1,0,0);//间隔1小时
	lstrcpy(sDate0,"");
	int i=0;
	do
	{
		t1=t.GetTime();		
		if(t1>m_t1)
			break;
		int x0= CTime2X(t1);
		if( x0<m_MarL)
			x0=m_MarL;
		pg->DrawLine(&pen,x0,y0,x0,y0+5);
		if( i%2==0)
		{
			wsprintf(sH,"%02d",t.GetHour());
			bt=(LPSTR)sH;
			pg->DrawString((BSTR)bt, -1, pfont, PointF((REAL)x0,(REAL)y0+3),&stringFormat,pbrush);
		}
		if(bDate)
		{
			wsprintf(sDate,"%d-%02d-%02d",t.GetYear(),t.GetMonth(),t.GetDay());
			if(lstrcmp(sDate,sDate0)!=0)//日期有变化,写日期
			{
				bt=(LPSTR)sDate;
	     		pg->DrawString((BSTR)bt, -1, pfont, PointF((REAL)x0-2,(REAL)y0+15), pbrush);
				lstrcpy(sDate0,sDate);
			}
		}
		//
		t+=ts;
		i++;
	}while(1);
}
//获取X,Y坐标点对应的物理量信息
void CmyDrawCsvHs::myGetDiagramData(int x,int y,LPSTR str)
{
	if(bHaveData && x>m_MarL && x< (m_nw+m_MarL))
	{
		char sTime[10],sDate[11];		
		RECT rc;
		POINT pt;
		pt.x =x;	pt.y=y;

		SetRect(&rc,m_MarL, m_MarT,m_MarL+m_nw+1,m_MarT+m_nh*10+1);
		if(PtInRect(&rc,pt))//鼠标在要素n对应的有效区域中
	    {
	    	X2Times(x,sDate,sTime);
	    	sprintf(str,"时间：%s %s ",sDate,sTime);
	    	return;	    	
		}//end if(PtInRect(&rc,pt))
	}
}