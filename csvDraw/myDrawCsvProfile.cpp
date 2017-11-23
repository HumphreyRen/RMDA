#include "stdafx.h"

#include "myDrawCsv.h"
#include "myCalcScale.h"


//���Ƶ�վ��Ҫ������ͼ
void CmyDrawCsvProfile::myDrawProfile(Graphics *pg,int gnw,int gnh,PMRDATA pdata,float *pg_fH,PFXINFO pfxinfo,PTZINFO ptz)
{
	bHaveData=false;
	fH=pg_fH;
	//���ñ���߼��߾�
	m_nw=gnw/4;//����ĵȷ�,Ϊÿ��Ҫ�ر����ռ���
	m_nh=gnh;
	m_MarL=30;	m_MarR=20;
	m_MarT=80;  m_MarB=20;	
	if(pfxinfo->nTime>=pdata->Ys.GetCount() || pdata->nProcessor==-1)
	{
		return;
	}
	//���㵥վ��ʱ�θ�Ҫ�ؿ̶����ֵ\��Сֵ
	PMRYS pYs=pdata->Ys.GetAt(pdata->Ys.GetCount()-1-pfxinfo->nTime);
	CmyCalcScale::myCalcScaleS1T1Yn(pdata->nProcessor,pYs,fMax,fMin);
	//����
	myDrawTable(pg,pYs,ptz);
	//��Ҫ������
	myDrawData(pg,pdata,pYs);
	bHaveData=true;
}

//����������
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

//���Ʊ��
void CmyDrawCsvProfile::myDrawTable(Graphics *pg,PMRYS pYs,PTZINFO ptz)
{
	char sYs[4][20]={"�¶�(��)","���ʪ��(%)","ˮ���ܶ�(g/m^3)","Һˮ����(g/m^3)"};
	_bstr_t bt=(char *)"����";
	FontFamily  fontFamily((BSTR)bt);
	Gdiplus::Font font(&fontFamily, (float)13, FontStyleRegular, UnitPixel);//FontStyleBold
	Pen pen(Color(255,128,128,128),1);
	SolidBrush brush(Color(255,0,0,0));
	//����
	char str[100];
	wsprintf(str,"%sվ΢������ƹ۲���������ͼ ʱ�䣺%s %s",ptz->zm,pYs->sDate,pYs->sTime);
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
	//�������ұ߽�
	x0= D2X(fMin[nYs],nYs,nX); 
	x1= D2X(fMax[nYs],nYs,nX); 
	//���Ƹ߶���
	myDrawTableHeight(pg,pfont,ppen,pbrush,x0);
	//Ҫ������
 	StringFormat stringFormat;
    stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter); 
	_bstr_t bt=(LPSTR)sTableCap;
	pg->DrawString((BSTR)bt, -1, pfont, PointF((REAL)x0+(x1-x0)/2,40), &stringFormat, pbrush);
	//���ƺ���
	y0= H2Y(0);//0km
	pg->DrawLine(ppen,x0,y0,x1,y0);
	for(int i=0;i<5;i++)
	{
		float f=fMin[nYs]+ (fMax[nYs]-fMin[nYs])*i/4;
		x1=D2X(f,nYs,nX);
    	pg->DrawLine(ppen,x1,y0,x1,y0+5);//��������
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

//���Ƹ߶���
void CmyDrawCsvProfile::myDrawTableHeight(Graphics *pg,Gdiplus::Font *pfont,Pen *ppen,SolidBrush *pbrush,int x0)
{
	_bstr_t bt;
	char str[5];
	int y0,y1;
	//�߶�
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

//�¶�\���ʪ��\ˮ���ܶ�\Һˮ����Ҫ��ֵת��Ϊ����Xֵ
int CmyDrawCsvProfile::D2X(float f,int nYs,int nX)
{
	return (int)(m_nw*nX + m_MarL + (f-fMin[nYs])*(m_nw-m_MarL-m_MarR)/(fMax[nYs]-fMin[nYs]));
}
//�߶�(0-10km)ת��Ϊ����Yֵ
int CmyDrawCsvProfile::H2Y(float fH)
{
	return (int)(m_nh-m_MarB-fH*(m_nh-m_MarT-m_MarB)/10);
}
//����Xֵת��Ϊ�¶�\���ʪ��\ˮ���ܶ�\Һˮ����Ҫ��ֵ
float CmyDrawCsvProfile::X2D(int x,int nYs,int nX)
{
	return (fMax[nYs]-fMin[nYs]) * (x- m_nw*nX - m_MarL)/(m_nw-m_MarL-m_MarR)+fMin[nYs];
}
//����Yֵת��Ϊ�߶�(0-10km)
float CmyDrawCsvProfile::Y2H(int y)
{
	return (float)10 * (m_nh-m_MarB-y) /(m_nh-m_MarT-m_MarB);
}
//��ȡX,Y������Ӧ����������Ϣ
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
			if(PtInRect(&rc,pt))//�����Ҫ��n��Ӧ����Ч������
			{
				float d= X2D(x,n,n);//Ҫ��ֵ
				float h= Y2H(y);    //�߶�ֵ
				if(n==0)
    				sprintf(sTemp,"�¶� %.1f��",d);
				else if(n==1)
					sprintf(sTemp,"���ʪ�� %.1f%%",d);
				else if(n==2)
					sprintf(sTemp,"ˮ���ܶ� %.1fg/m^3",d);
				else 
					sprintf(sTemp,"Һˮ���� %.3fg/m^3",d);

				sprintf(str,"ͼʾ���߶� %.2fkm��%s",h,sTemp);
				
				break;
			}
		}
	}
}