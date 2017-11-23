#include "stdafx.h"

#include "myDrawCsv.h"


//��վ�豸����״̬����ͼ
void CmyDrawCsvHs::myDrawHs(Graphics *pg,int gnw,int gnh,CArray<PHSTATUS,PHSTATUS> *phs,PTZINFO ptz)
{
	bHaveData=false;
	m_TimeCount=phs->GetCount(); //ʱ������
	if( m_TimeCount<1)
		return;
	//���ñ���߼��߾�
	m_MarT=50;      //��ͼ�����ı���ռ�߶�
	m_MarB=35;      //ʱ��̶���ռ�߶�
	m_MarL=100;  m_MarR=70;  //���ұ߾�
	m_nw=gnw-m_MarL-m_MarR;       //ÿ��Ҫ�ر����ռ���
	m_nh=(gnh-m_MarT-m_MarB)/10;  //�߶�10�ȷ�,Ϊÿ��Ҫ�ر��߶�
	//��������ʱ��ֵ��ʱ����ֵ(��)
	myCalcTimeScale(phs);
	//����ʱ�䷽���ʱ��X����ֵ
	int *nX=(int *)malloc(m_TimeCount*sizeof(int));
	for(int i=0;i<m_TimeCount;i++)
	{
		nX[i]=Time2X(phs->GetAt(i));
	}
	//���Ʊ��
	myDrawTable(pg,ptz);
	//����������
	myDrawData(pg,phs,nX);
	//
	free(nX);
	bHaveData=true;
}

//����������
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
//���Ʊ��
void CmyDrawCsvHs::myDrawTable(Graphics *pg,PTZINFO ptz)
{
	_bstr_t bt=(char *)"����";
	FontFamily  fontFamily((BSTR)bt);
	Gdiplus::Font font(&fontFamily, (float)13, FontStyleRegular, UnitPixel); 
	Pen pen(Color(255,128,128,128),1);
	SolidBrush brush(Color(255,0,0,0));
	//����
	char str[100];
	wsprintf(str,"%sվ΢��������豸����״̬ʱ������ͼ",ptz->zm);
	bt=(LPSTR)str;
   	pg->DrawString((BSTR)bt, -1, &Gdiplus::Font(&fontFamily, (float)16, FontStyleRegular, UnitPixel), PointF(10,10), &brush);
	//10��Ҫ�ر������˵��
	for(int i=0;i<10;i++)
	{
		myDrawTableKuang(pg,&font,&pen,&brush,m_MarL,m_MarT +m_nh*i,m_nw,m_nh,i);
	}
	//ʱ��̶� 
	myDrawTimeScale(pg,&font,&brush,m_MarT+m_nh*10,true);
} 
//���Ʊ��߿�
void CmyDrawCsvHs::myDrawTableKuang(Graphics *pg,Gdiplus::Font *pfont,Pen *ppen,SolidBrush *pbrush,int x0,int y0,int w,int h,int n)
{	
	char sYsL[][20]={"�豸����","���ذ�","���ջ�0","���ջ�1","���ǿ�����","��λ�ǿ�����","GPS","���������","���洫����","��Դ�����"};
	StringFormat stringFormat;
    stringFormat.SetAlignment(Gdiplus::StringAlignmentFar);	
	//Ҫ����
	_bstr_t bt;
	bt=(LPSTR)sYsL[n];
	pg->DrawString((BSTR)bt, -1, pfont, PointF((REAL)x0,(REAL)y0+h/2-6),&stringFormat, pbrush );
	//�߿�
	pg->DrawRectangle(ppen,x0,y0,w,h);   
	//��������
	Pen pen(Color(255,128,128,128),1);
	pen.SetDashStyle(Gdiplus::DashStyleDashDot);  
	for(int i=1;i<4;i++)
	{
		int y = y0+ i* h / 4;
        //pg->DrawLine(&pen,x0,y,x0+w,y); 
		if( i==1)
		{
    		bt=(LPSTR)"����";
   	    	pg->DrawString((BSTR)bt, -1, pfont, PointF((REAL)x0+w+2,(REAL)y-10), pbrush);
		}
		else if( i==2)
		{
    		bt=(LPSTR)"������";
   	    	pg->DrawString((BSTR)bt, -1, pfont, PointF((REAL)x0+w+2,(REAL)y-5), pbrush);
		}
		else
		{
			bt=(LPSTR)"����";
   	    	pg->DrawString((BSTR)bt, -1, pfont, PointF((REAL)x0+w+2,(REAL)y), pbrush);
		}
	}	
}

//Ҫ��ֵת��Ϊ����Yֵ
int CmyDrawCsvHs::D2Y(float f,int nYs)
{
	return (int)(m_MarT + m_nh*nYs + (3-f)*(m_nh)/4);
}
//ʱ��ת��Ϊ����Xֵ
int CmyDrawCsvHs::Time2X(PHSTATUS pys)
{
	return m_MarL+((int)(StrTime2Binary(pys)-m_t0))*(m_nw)/m_tLen;
}
//����Xֵת��Ϊ����ʱ��
void CmyDrawCsvHs::X2Times(int x,LPSTR sD,LPSTR sT)
{
	time_t tt=m_t0+(x-m_MarL)*m_tLen/(m_nw);
	CTime t(tt);
	sprintf(sD,"%d-%02d-%02d",t.GetYear(),t.GetMonth(),t.GetDay());
	sprintf(sT,"%02d:%02d:%02d",t.GetHour(),t.GetMinute(),t.GetSecond());
}
//�����������ʱ��ֵ��ʱ����
void CmyDrawCsvHs::myCalcTimeScale(CArray<PHSTATUS,PHSTATUS> *phs)
{
	//�����ʱ��
	char sY[5],sM[3],sD[3],sH[3],smm[3],sSe[3];
	PHSTATUS pys0= phs->GetAt(0);
	sscanf(pys0->sDate,"%[^-]-%[^-]-%s",sY,sM,sD);
	sscanf(pys0->sTime,"%[^:]:%[^:]:%s",sH,smm,sSe);
	CTime t(atoi(sY),atoi(sM),atoi(sD),atoi(sH),0,0);
	m_t0=t.GetTime(); 
	//����ʱ��
	PHSTATUS pys1= phs->GetAt(m_TimeCount-1);
	m_t1=StrTime2Binary(pys1); //����ʱ��
	//ʱ����
	m_tLen=(int)(m_t1-m_t0);//����ʱ��-�����ʱ��
	if(m_tLen<1) m_tLen=1;
}
//ʱ�����ַ���ת��Ϊ����,��1970-1-1����
time_t CmyDrawCsvHs::StrTime2Binary(PHSTATUS pys)
{
	char sY[5],sM[3],sD[3],sH[3],smm[3],sSe[3];

	sscanf(pys->sDate,"%[^-]-%[^-]-%s",sY,sM,sD);
	sscanf(pys->sTime,"%[^:]:%[^:]:%s",sH,smm,sSe);
	CTime t(atoi(sY),atoi(sM),atoi(sD),atoi(sH),atoi(smm),atoi(sSe));
	return t.GetTime(); 
}
//CTimeת��Ϊ����Xֵ
int CmyDrawCsvHs::CTime2X(time_t t)
{
	return m_MarL+((int)(t-m_t0))*m_nw/m_tLen;
}
//����ʱ��̶�
void CmyDrawCsvHs::myDrawTimeScale(Graphics *pg,Gdiplus::Font *pfont,SolidBrush *pbrush,int y0,bool bDate)
{
    _bstr_t bt;	
	char sDate0[11],sDate[11],sH[3];
	Pen pen(Color(255,128,128,128),1);
	StringFormat stringFormat;
    stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter); 
	//��ʼʱ��
    CTime t(m_t0); 
	time_t t1;	 
	CTimeSpan ts(0,1,0,0);//���1Сʱ
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
			if(lstrcmp(sDate,sDate0)!=0)//�����б仯,д����
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
//��ȡX,Y������Ӧ����������Ϣ
void CmyDrawCsvHs::myGetDiagramData(int x,int y,LPSTR str)
{
	if(bHaveData && x>m_MarL && x< (m_nw+m_MarL))
	{
		char sTime[10],sDate[11];		
		RECT rc;
		POINT pt;
		pt.x =x;	pt.y=y;

		SetRect(&rc,m_MarL, m_MarT,m_MarL+m_nw+1,m_MarT+m_nh*10+1);
		if(PtInRect(&rc,pt))//�����Ҫ��n��Ӧ����Ч������
	    {
	    	X2Times(x,sDate,sTime);
	    	sprintf(str,"ʱ�䣺%s %s ",sDate,sTime);
	    	return;	    	
		}//end if(PtInRect(&rc,pt))
	}
}