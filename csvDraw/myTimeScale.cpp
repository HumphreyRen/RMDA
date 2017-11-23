//
// myTimeScale.cpp
// 2016-08-10
//
#include "stdafx.h"

#include "myTimeScale.h"

int CmyTimeScale::m_nw;
int CmyTimeScale::m_MarL;//左边距
int CmyTimeScale::m_MarR;//右边距
time_t CmyTimeScale::m_t0,CmyTimeScale::m_t1; 
int    CmyTimeScale::m_tLen;
PMRYS CmyTimeScale::m_pys0;
//
void CmyTimeScale::myCalcTimeScale(CArray<PMRDATA,PMRDATA> *pmrs,int nw,int MarL,int MarR,int& TimeCount)
{
	m_nw=nw;	m_MarL=MarL;	m_MarR=MarR;
	//时间总数
	TimeCount=0;
	for(int i=(pmrs->GetCount()-1);i>=0;i--)
	{
		PMRDATA pdata=pmrs->GetAt(i);
		TimeCount +=  pdata->Ys.GetCount();
	}
	//最早的时间
	for(int i=(pmrs->GetCount()-1);i>=0;i--)
	{
		PMRDATA pdata=pmrs->GetAt(i);
		if( pdata->Ys.GetCount()>0)
		{			
			m_pys0= pdata->Ys.GetAt(0);
			break;
		}
	}
	m_t0=StrTime2Binary(m_pys0);//最早的时间
	//最后的时间
	PMRDATA pdata=pmrs->GetAt(0);
	PMRYS pys1= pdata->Ys.GetAt(pdata->Ys.GetCount()-1);
	m_t1=StrTime2Binary(pys1); //最后的时间
	m_tLen=(int)(m_t1-m_t0);//最后的时间-最早的时间
	if(m_tLen<1) m_tLen=1;
}

//时间转换为坐标X值
int CmyTimeScale::Time2X(PMRYS pys)
{
	return m_MarL+((int)(StrTime2Binary(pys)-m_t0))*(m_nw-m_MarL-m_MarR)/m_tLen;
}
//坐标X值转换为时间
time_t CmyTimeScale::X2Timet(int x)
{
	return m_t0+(x-m_MarL)*m_tLen/(m_nw-m_MarL-m_MarR);
}
void CmyTimeScale::X2Times(int x,LPSTR sD,LPSTR sT)
{
	time_t tt=m_t0+(x-m_MarL)*m_tLen/(m_nw-m_MarL-m_MarR);
	CTime t(tt);
	sprintf(sD,"%d-%02d-%02d",t.GetYear(),t.GetMonth(),t.GetDay());
	sprintf(sT,"%02d:%02d:%02d",t.GetHour(),t.GetMinute(),t.GetSecond());
}

//时间由字符串转换为秒数,从1970-1-1算起
time_t CmyTimeScale::StrTime2Binary(PMRYS pys)
{
	char sY[5],sM[3],sD[3],sH[3],smm[3],sSe[3];

	sscanf(pys->sDate,"%[^-]-%[^-]-%s",sY,sM,sD);
	sscanf(pys->sTime,"%[^:]:%[^:]:%s",sH,smm,sSe);
	CTime t(atoi(sY),atoi(sM),atoi(sD),atoi(sH),atoi(smm),atoi(sSe));
	return t.GetTime(); 
}
//CTime转换为坐标X值
int CmyTimeScale::CTime2X(time_t t)
{
	return m_MarL+((int)(t-m_t0))*(m_nw-m_MarL-m_MarR)/m_tLen;
}
//绘制时间刻度
void CmyTimeScale::myDrawTimeScale(Graphics *pg,Gdiplus::Font *pfont,SolidBrush *pbrush,int y0,bool bDate)
{
    _bstr_t bt;	
	char sDate0[11],sDate[11];
	Pen pen(Color(255,128,128,128),1);
	StringFormat stringFormat;
    stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter); 
	//初始时间
	char sY[5],sM[3],sD[3],sH[3],smm[3],sSe[3];
	sscanf(m_pys0->sDate,"%[^-]-%[^-]-%s",sY,sM,sD);
	sscanf(m_pys0->sTime,"%[^:]:%[^:]:%s",sH,smm,sSe);
	CTime t(atoi(sY),atoi(sM),atoi(sD),atoi(sH),0,0);
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
