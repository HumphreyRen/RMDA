//
//  myDrawCsvEvolveThrd.cpp�����ٵ�վ��Ҫ��ʱ�շֲ�ͼ��ֵ��
//  2016-08-06
//
#include "stdafx.h"
#include "myDrawCsv.h"
#include "myCalcScale.h"

//��Ҫ�ؼ������߳�
void CmyDrawCsvEvolve::myEvolveThrd(void * pParam)
{
	PThrdPARAM ptp=(PThrdPARAM)pParam;
	ptp->bNullData=false;
	CmyDrawCsvEvolve *pdraw = (CmyDrawCsvEvolve *)ptp->pdraw;
	//
	pdraw->fMax[ptp->nYs]=MaxValue;
	pdraw->fMin[ptp->nYs]=MinValue;
	//�����������pd
	float **pd=ptp->pd;
	for(int i=0;i<pdraw->m_xnum;i++)
	{
		int ii=i*pdraw->m_nz;
		if(i==(pdraw->m_xnum-1)) ii=pdraw->m_TimeCount-1;
		PMRYS pys;
		int nProcessor;
		for(int k=2;k>=0;k--)
		{
    		if(ii<ptp->pmrs->GetAt(k)->Ys.GetCount())  
			{
		    	pys=ptp->pmrs->GetAt(k)->Ys.GetAt(ii);
				nProcessor= ptp->pmrs->GetAt(k)->nProcessor ;
				break;
			}
     		else
			{
				ii-=ptp->pmrs->GetAt(k)->Ys.GetCount();
			}
		}		 
    	pd[i]=(float *)malloc(sizeof(float)*pdraw->m_HighCount);	
		float *f=pys->ff[nProcessor][ptp->nYs];
		if(f!=NULL)
		{
			for(int j=0;j<pdraw->m_HighCount;j++)
			{
				pd[i][j]=f[pdraw->m_HighCount-j-1];
				CmyCalcScale::myCalcMaxMinValue(pd[i][j],pdraw->fMax[ptp->nYs],pdraw->fMin[ptp->nYs]);//�����Ҫ�����ֵ����Сֵ
			}
		}
		else
			ptp->bNullData=true;
	}
	if(ptp->bNullData==false)
	{
		// �����ֵ��ֵ
		CArray<float,float&> xzs;
		myCalcXZ(ptp->nYs,pdraw->fMax[ptp->nYs],pdraw->fMin[ptp->nYs],&xzs);
		//���ٵ�ֵ��
		ptp->pmydzx->myTraceDZX(pd,&xzs,ptp->nX,ptp->nY,pdraw->m_xnum,pdraw->m_HighCount); 
		//�ͷ��ڴ�
		xzs.RemoveAll();
	}
	//�����߳�
	SetEvent(ptp->hFinish);
	AfxEndThread(100,true);
}

//�����ֵ��ֵ
void CmyDrawCsvEvolve::myCalcXZ(int nYs,float fMax,float fMin,CArray<float,float&> *pxzs)
{
	float startline,dline;
	dline=(fMax-fMin)/5;
	if(nYs==0)
	{
		if(dline<=2)
			dline=2;
		else if(dline<=5)
			dline=5;
		else 
			dline=10;
       	startline=(float)((int)fMin);
	}
	else if(nYs==1)
	{
		if(dline<=10)
			dline=10;
		else
			dline=20;
		startline=10;
	}
	else if(nYs==2)
	{
		if( dline<=1)
    		dline=1;
		else
			dline=(float)((int)(dline));
		startline=dline;
	}
	else
	{
		if(dline<0.001)
		   dline=0.001f;
		else if( dline<0.01)
		{
			dline= (float)((int)(dline*1000))/1000;
		}
		else if(dline<0.1)
		{
			dline=(float)((int)(dline*100))/100;			
		}	
		else if(dline<1)
		{
			dline=(float)((int)(dline*10))/10;
		}	
		else
		{
			dline=(float)((int)(dline));
		}
		startline=dline;
	}
	for(float xz=startline;xz<=fMax;xz+=dline)
	{
		pxzs->Add(xz);			
	}
}