#include "stdafx.h"
#include "myDrawCsv.h"
#include "myCalcScale.h"


//���ƶ�վ��Ҫ������
void CmyDrawCsvProfile2::myDrawProfile2(Graphics *pg,int gnw,int gnh,CArray<PMRDATA,PMRDATA> *pmrdata,float *pg_fH,PFXINFO pfxinfo,CArray<PTZINFO,PTZINFO> *ptzs,LPSTR sTime0)
{
    bHaveData=false;
	pmrs=pmrdata;
	fH=pg_fH;
	pfx=pfxinfo;
	//���ñ���߼��߾�
	m_MarTop=40;      //��ͼ�����ı���ռ�߶�
	if(ptzs->GetCount()>7) //������ʾ
	{
		col_count = (ptzs->GetCount()+1)/2; //ÿ������
		m_nh=(gnh-m_MarTop)/2;              //ÿ�и߶�,�����±߾�
	}
	else //һ����ʾ
	{
		col_count = ptzs->GetCount();
		m_nh=gnh-m_MarTop;
	}
	m_nw=gnw/col_count;   //��̨վ���ȷֿ��,Ϊÿ��վ��Ҫ�ر����ռ���,�����ұ߾�	
	m_MarL=30;	m_MarR=20;  //������ұ߾�
	m_MarT=30;  m_MarB=30;	//������±߾�
	//���ݵ�ǰѡ��ʱ��sTime0�����վ��ʱ�����
	CmyCalcScale::myCalcTimeIndex(pmrs,sTime0);
	//�����վ��ʱ�ε�Ҫ�ؿ̶����ֵ����Сֵ
	CmyCalcScale::myCalcScaleSnT1Y1(pmrs,pfx,fMax,fMin);
	//����
	myDrawTable(pg,ptzs,sTime0);
	//��Ҫ������
	myDrawData(pg);
	bHaveData=true;
}
//���Ʊ��2
void CmyDrawCsvProfile2::myDrawTable(Graphics *pg,CArray<PTZINFO,PTZINFO> *ptzs,LPSTR sTime)
{
	char sYs[4][20]={"�¶�(��)","���ʪ��(%)","ˮ���ܶ�(g/m^3)","Һˮ����(g/m^3)"};
	_bstr_t bt=(char *)"����";
	FontFamily  fontFamily((BSTR)bt);
	Gdiplus::Font font(&fontFamily, (float)13, FontStyleRegular, UnitPixel); 
	Pen pen(Color(255,128,128,128),1);
	SolidBrush brush(Color(255,0,0,0));
	//����
	char str[100];
	wsprintf(str,"΢������ƹ۲����ݶ�վ�Ա�ͼ Ҫ�أ�%s ʱ�䣺%d-%02d-%02d %s",sYs[pfx->nYs],pfx->nYear,pfx->nMonth,pfx->nDay,sTime);
	bt=(LPSTR)str;
   	pg->DrawString((BSTR)bt, -1, &Gdiplus::Font(&fontFamily, (float)16, FontStyleRegular, UnitPixel), PointF(10,10), &brush);
	
	for(int i=0;i<ptzs->GetCount();i++)
	{
		PTZINFO ptz=ptzs->GetAt(i);
	   	myDrawTableKuang(pg,&font,&pen,&brush,pfx->nYs,i,ptz->zm);
	}
}
//����������2
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
	//�������ұ߽�
	x0= D2X2(fMin[nYs],nYs,nX); 
	x1= D2X2(fMax[nYs],nYs,nX); 
	//���Ƹ߶���
	myDrawTableHeight(pg,pfont,ppen,pbrush,x0,nX);
	//Ҫ������
 	StringFormat stringFormat;
    stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter); 
	_bstr_t bt=(LPSTR)sTableCap;
	pg->DrawString((BSTR)bt, -1, pfont, PointF((REAL)x0+(x1-x0)/2,(REAL)m_MarTop + m_nh*(nX/col_count)), &stringFormat, pbrush);
	//���ƺ���
	y0= H2Y2(0,nX);//0km
	pg->DrawLine(ppen,x0,y0,x1,y0);
	for(int i=0;i<5;i++)
	{
		float f=fMin[nYs]+ (fMax[nYs]-fMin[nYs])*i/4;
		x1=D2X2(f,nYs,nX);
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
void CmyDrawCsvProfile2::myDrawTableHeight(Graphics *pg,Gdiplus::Font *pfont,Pen *ppen,SolidBrush *pbrush,int x0,int n)
{
	_bstr_t bt;
	char str[5];
	int y0,y1;
	//�߶�
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

//�¶�\���ʪ��\ˮ���ܶ�\Һˮ����Ҫ��ֵת��Ϊ����Xֵ
int CmyDrawCsvProfile2::D2X2(float f,int nYs,int nX)
{
	return (int)(m_nw*(nX%col_count) + m_MarL + (f-fMin[nYs])*(m_nw-m_MarL-m_MarR)/(fMax[nYs]-fMin[nYs]));
}
//�߶�(0-10km)ת��Ϊ����Yֵ
int CmyDrawCsvProfile2::H2Y2(float fH,int n)
{
	//10:10-0km
	return (int)(m_MarTop + m_nh*(n/col_count+1) - m_MarB - fH*(m_nh-m_MarT-m_MarB)/10);
}
//����Xֵת��Ϊ�¶�\���ʪ��\ˮ���ܶ�\Һˮ����Ҫ��ֵ
float CmyDrawCsvProfile2::X2D2(int x,int nYs,int nX)
{
	return (fMax[nYs]-fMin[nYs]) * (x- m_nw*(nX%col_count) - m_MarL)/(m_nw-m_MarL-m_MarR)+fMin[nYs];
}
//����Yֵת��Ϊ�߶�(0-10km)
float CmyDrawCsvProfile2::Y2H2(int y,int n)
{
	return (float)10 * (m_MarTop + m_nh*(n/col_count+1) -m_MarB-y) /(m_nh-m_MarT-m_MarB);
}

//��ȡX,Y������Ӧ����������Ϣ
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
			if(PtInRect(&rc,pt))//�����Ҫ��n��Ӧ����Ч������
			{
				float d= X2D2(x,pfx->nYs,n);
				float h= Y2H2(y,n);
				switch(pfx->nYs)
				{
				case 0:
    				sprintf(sTemp,"�¶� %.1f��",d);
					break;
				case 1:
					sprintf(sTemp,"���ʪ�� %.1f%%",d);
					break;
				case 2:
					sprintf(sTemp,"ˮ���ܶ� %.1fg/m^3",d);
					break;
				case 3:
					sprintf(sTemp,"Һˮ���� %.3fg/m^3",d);
					break;
				default:
					lstrcpy(sTemp,"");
					break;
				}
				sprintf(str,"ͼʾ���߶� %.2fkm��%s",h,sTemp);				
				break;
			}
		}
	}
}