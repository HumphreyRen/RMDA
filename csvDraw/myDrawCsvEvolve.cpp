#include "stdafx.h"

#include "myDrawSB.h"
#include "myTimeScale.h"
#include "myDrawCsv.h"


//��վ��Ҫ��ʱ�շֲ�ͼ
void CmyDrawCsvEvolve::myDrawEvolve(Graphics *pg,int gnw,int gnh,CArray<PMRDATA,PMRDATA> *pmrdata,float *pg_fH,PFXINFO pfxinfo,PTZINFO ptz)
{
	bHaveData=false;
	pmrs=pmrdata;
	fH=pg_fH;
	PMRDATA pmr=pmrs->GetAt(0);
	if(pmr->Ys.GetCount()<1 || pmr->nProcessor ==-1)
	{
		return;
	}
	//��ȡɫ��
	CmyDrawSB::myReadSB("param\\sb.txt",&r,&g,&b,m_sbCount);
	//��������
	m_nz=4;
	//���ñ���߼��߾�
	m_MarL=40;	m_MarR=90;
	m_MarT=25;	m_MarB=20;	
	m_nw=gnw ;        //Ϊÿ��Ҫ�ر����ռ���,�����ұ߾�
	m_nh=(gnh-35-15)/4;  //�߶�4�ȷ�,Ϊÿ��Ҫ�ر����ռ�߶�,�����±߾�	
	m_MaxHigh=pg_fH[57-pfxinfo->nHigh]; //���߶�
	if( m_MaxHigh<1)//���߶�����Ϊ1����
		m_MaxHigh=1;

	//����ʱ������������ʱ��ֵ��ʱ����ֵ(��)
	CmyTimeScale::myCalcTimeScale(pmrs,m_nw,m_MarL,m_MarR,m_TimeCount);
	m_xnum= m_TimeCount/m_nz+1;
	//����ʱ�䷽���ʱ��X����ֵ
	int *nX=(int *)malloc(m_xnum*sizeof(int));
	for(int i=0;i<m_xnum;i++)
	{
		int j=i*m_nz;
		if(i==(m_xnum-1)) j=m_TimeCount-1;
		PMRYS pys;
		for(int k=2;k>=0;k--)
		{
    		if(j<pmrs->GetAt(k)->Ys.GetCount())  
			{
		    	pys=pmrs->GetAt(k)->Ys.GetAt(j);
				break;
			}
     		else
			{
				j-=pmrs->GetAt(k)->Ys.GetCount();
			}
		}
		nX[i]=CmyTimeScale::Time2X(pys);
	}
	//����߶�����
	m_HighCount=0;
	for(int i=0;i<58;i++)
	{
		if(pg_fH[i]> m_MaxHigh) 
			break;
		m_HighCount++;
	}	
	//����߶ȷ�����߶�Y����ֵ
	int *nY[4];
	for(int k=0;k<4;k++)
	{
    	nY[k]=(int *)malloc(m_HighCount*sizeof(int));
    	for(int j=0;j<m_HighCount;j++)
     	{
	     	nY[k][j]=High2Y(pg_fH[m_HighCount-j-1],k);//
	    }
	}
	//�ĸ�Ҫ�ص�ֵ�߸���
	float **ppd[4];  //�ĸ�Ҫ�ظ������
	CmyDZX mydzx[4]; //�ĸ�Ҫ�ص�ֵ�߸��ٶ���
	//
	//�����ĸ�Ҫ�ؼ����߳�
	ThrdPARAM tps[4];
	for(int k=0;k<4;k++)
	{
		ppd[k]=(float **)malloc(sizeof(int)*m_xnum);
		tps[k].hFinish =CreateEvent(NULL,TRUE,FALSE,NULL);
		tps[k].pdraw = this;
		tps[k].nYs =k;
		tps[k].pmrs =pmrs;
		tps[k].nX =nX;
		tps[k].nY =nY[k];
		tps[k].pd =ppd[k];
		tps[k].pmydzx = &(mydzx[k]);
		AfxBeginThread((AFX_THREADPROC)myEvolveThrd, &tps[k]);
	}
	//�ȴ���Ҫ�ؼ����߳̽���
	for(int k=0;k<4;k++)
	{
		WaitForSingleObject(tps[k].hFinish, INFINITE);
		CloseHandle(tps[k].hFinish);
	}
	//���Ʊ��
	myDrawTable(pg,ptz);
	//���Ƹ�Ҫ�أ��¶ȡ�ʪ�ȡ�ˮ���ܶȡ�Һˮ������
	for(int k=0;k<4;k++)
	{
		if(tps[k].bNullData == false)
		{
			//���
    		myFillCsvEvolve(ppd[k],nX,nY[k],fMin[k],fMax[k],m_sbCount);//
			//����ֵ��
			myDrawDzx(pg,&(mydzx[k].dzxs),k);
		}
	}
	//
	//�ͷ��ڴ���Դ
	for(int k=0;k<4;k++)
	{
		//ppd
    	for(int i=0;i<m_xnum;i++)
	    	free(ppd[k][i]);
	    free(ppd[k]);
		//nY
		free(nY[k]);
	}
	free(nX);
	free(r);
	free(g);
	free(b);
	bHaveData=true;
}
void CmyDrawCsvEvolve::myDrawXZ(Graphics *pg,Gdiplus::Font *pfont,SolidBrush *pbrush,int nYs,float xz,int x,int y)
{
	char str[10];
	wsprintf(str,"%d",(int)xz);
    _bstr_t	bt=(LPSTR)str;
	pg->DrawString((BSTR)bt, -1, pfont, PointF((REAL)x,(REAL)y), pbrush);
}
//����ֵ��
void CmyDrawCsvEvolve::myDrawDzx(Graphics *pg,CArray<PDZX,PDZX> *pdzxs,int nYs)
{
	pg->SetClip(Rect(m_MarL,35+m_MarT+ m_nh*nYs,m_nw-m_MarL-m_MarR,m_nh-m_MarT-m_MarB));
	_bstr_t bt=(char *)"����";
	FontFamily  fontFamily((BSTR)bt);
	Gdiplus::Font font(&fontFamily, (float)12, FontStyleRegular, UnitPixel);//FontStyleBold
	SolidBrush brush(Color(255,0,0,0));

	glColor3ub(0,0,0);
   	glLineWidth(1);
	int nLineCount=pdzxs->GetCount();
	int nCount;
	POINT pt;
	for(int i=0;i<nLineCount;i++)
	{
		PDZX pdzx=pdzxs->GetAt(i);
		nCount=pdzx->pts.GetCount(); 
		glBegin(GL_LINE_STRIP);
	    for(int j=0;j<nCount;j++)
	    {
			pt= pdzx->pts.GetAt(j);
		    glVertex2i(pt.x,pt.y);
	    }
    	glEnd();
    	glFinish();
		//
        if(nYs<3 && pdzx->lx == 0)
		{
			pt= pdzx->pts.GetAt(nCount/2);
			myDrawXZ(pg,&font,&brush,nYs,pdzx->xz,pt.x,pt.y-5);
		}
	}
	pg->ResetClip();
}
//����¶ȡ�ʪ�ȡ�ˮ���ܶȡ�Һˮ����
void CmyDrawCsvEvolve::myFillCsvEvolve(float **pd,int *nX,int *nY,float fMin,float fMax,int sbCount)
{
	int x1,x2,y1,n;
	for(int i=0;i<m_xnum-1;i++)//ʱ�䷽��
	{
		x1=nX[i];//�õ���Чֵx1
		x2=nX[i+1];//�õ���Чֵx2
		glBegin(GL_QUAD_STRIP);
		for(int j=0;j<m_HighCount;j++)//�߶ȷ���
		{
			y1=nY[j];  //�õ���Чֵy1
			//���¶������½ǵ�λ�ü���ɫ���� 	
			n=CmyDrawSB::myGetSbColorIndex(pd[i+1][j],fMin,fMax,sbCount);
            glColor3ub(r[n],g[n],b[n]); 
            glVertex2i(x2,y1);	  				 
			//���¶������½ǵ�λ�ü���ɫ���� 
			n=CmyDrawSB::myGetSbColorIndex(pd[i][j],fMin,fMax,sbCount);
            glColor3ub(r[n],g[n],b[n]); 
            glVertex2i(x1,y1);		
		}
		glEnd();//���һ���������
	    glFlush();
	}
}

//���Ʊ��
void CmyDrawCsvEvolve::myDrawTable(Graphics *pg,PTZINFO ptz)
{
	_bstr_t bt=(char *)"����";
	FontFamily  fontFamily((BSTR)bt);
	Gdiplus::Font font(&fontFamily, (float)13, FontStyleRegular, UnitPixel);//FontStyleBold
	Pen pen(Color(255,128,128,128),1);
	SolidBrush brush(Color(255,0,0,0));
	//����
	char str[100];
	wsprintf(str,"%sվ΢������ƹ۲�����ʱ�շֲ�ͼ",ptz->zm);
	bt=(LPSTR)str;
   	pg->DrawString((BSTR)bt, -1, &Gdiplus::Font(&fontFamily, (float)16, FontStyleRegular, UnitPixel), PointF(10,10), &brush);
	//
	int x0,y0,w,h;
	x0=m_MarL;w=m_nw-m_MarL-m_MarR;h=m_nh-m_MarT-m_MarB;
	for(int i=0;i<4;i++)
	{
		y0= 35 + m_MarT+m_nh*i;
		myDrawTableKuang(pg,&font,&pen,&brush,x0,y0,w,h,i);
		CmyDrawSB::myDrawSBStrip(r,g,b,m_sbCount,m_nw-60,y0,10,h);
	    CmyDrawSB::myDrawSBScale(pg,&font,&brush,fMin[i],fMax[i],m_nw-50,y0,h);
		CmyTimeScale::myDrawTimeScale(pg,&font,&brush,y0+m_nh-m_MarT-m_MarB,i==3?true:false);
	}	
} 

//���Ʊ��߿�
void CmyDrawCsvEvolve::myDrawTableKuang(Graphics *pg,Gdiplus::Font *pfont,Pen *ppen,SolidBrush *pbrush,int x0,int y0,int w,int h,int nYs)
{	
	char sYs[4][20]={"�¶�(��)","���ʪ��(%)","ˮ���ܶ�(g/m^3)","Һˮ����(g/m^3)"};
	StringFormat stringFormat;
    stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter); 
	//Ҫ����
	_bstr_t bt;
	bt=(LPSTR)sYs[nYs];
	pg->DrawString((BSTR)bt, -1, pfont, PointF((REAL)x0+w/2,(REAL)y0-15), &stringFormat, pbrush);
	//�߿�
	pg->DrawRectangle(ppen,x0,y0,w,h);
	//�߶ȵ�λ
    stringFormat.SetAlignment(Gdiplus::StringAlignmentFar); 
	bt=(LPSTR)"km";
	pg->DrawString((BSTR)bt, -1, pfont, PointF((REAL)x0,(REAL)y0-18), &stringFormat, pbrush);	
	//�߶ȼ��
	char str[5];
    int y1;
	float fStep=1;
	if(m_MaxHigh<=1)
		fStep=0.2f;
	else if(m_MaxHigh>6)
		fStep=2;
	float f=0;
	int i=0;
	do
	{
		y1=(int)(y0+h- f*h/m_MaxHigh);
    	pg->DrawLine(ppen,x0-5,y1,x0,y1);//�߶ȼ����
		if(m_MaxHigh<=1)
         	sprintf(str,"%.1f",f);
		else
         	sprintf(str,"%d",(int)f);
		bt=(LPSTR)str;
		pg->DrawString((BSTR)bt, -1, pfont, PointF((REAL)x0-5,(REAL)y1-5), &stringFormat, pbrush);
		i++;		
		f+=fStep;
	}while(f<=m_MaxHigh);
}

//�߶�ת��Ϊ����Yֵ
int CmyDrawCsvEvolve::High2Y(float fH,int nYs)
{
	return (int)(35 + m_nh*nYs+ m_nh -m_MarB - fH*(m_nh-m_MarT-m_MarB)/m_MaxHigh);
}
//����Yֵת��Ϊ�߶�
float CmyDrawCsvEvolve::Y2High(int y,int nYs)
{
	return (float)(35 + m_nh*nYs+ m_nh -m_MarB - y)*m_MaxHigh/(m_nh-m_MarT-m_MarB);
}
//��ȡX,Y������Ӧ����������Ϣ
void CmyDrawCsvEvolve::myGetDiagramData(int x,int y,LPSTR str)
{
	if(bHaveData && pmrs->GetCount()==3)
	{
		RECT rc;
		POINT pt;
		char sTemp[50],sTime[10],sDate[11];
		pt.x =x;pt.y=y;
		for(int n=0;n<4;n++)
		{
			SetRect(&rc,m_MarL,35 + m_MarT+m_nh*n,m_nw-m_MarR+1,35+m_nh*(n+1)-m_MarB+1);
			if(PtInRect(&rc,pt))//�����Ҫ��n��Ӧ����Ч������
			{
				CmyTimeScale::X2Times(x,sDate,sTime);
				float h= Y2High(y,n);
				//��߶�h��ʱ��sTime��Ӧ��Ҫ��ֵ
				int nh=0;
				for(int i=0;i<58;i++)
				{
					if( h<=fH[i]) 
					{
						nh=i;
						break;
					}
				}
				for(int k=2;k>=0;k--)
				{
					PMRDATA pdata=pmrs->GetAt(k);
					for(int i=0;i<pdata->Ys.GetCount();i++)
			        {
		    	    	PMRYS pys=pdata->Ys.GetAt(i);
				    	if(lstrcmp(sDate,pys->sDate)==0 && lstrcmp(sTime,pys->sTime )<=0)
					    {
						    float *f=pys->ff[pdata->nProcessor][n];
							if(f!=NULL)
							{
								if(n==0)
    								sprintf(sTemp,"�¶� %.1f��",f[nh]);
								else if(n==1)
									sprintf(sTemp,"���ʪ�� %.1f%%",f[nh]);
								else if(n==2)
									sprintf(sTemp,"ˮ���ܶ� %.1fg/m^3",f[nh]);
								else 
									sprintf(sTemp,"Һˮ���� %.3fg/m^3",f[nh]);

								sprintf(str,"ͼʾ��ʱ�� %s %s���߶� %.2fkm��%s",sDate,sTime,h,sTemp);
							}
						    return;
					    }
					}//end for(i)
				}//end for(k)				
			}// end if(PtInRect(&rc,pt))
		}//end for(n)
	}
}
