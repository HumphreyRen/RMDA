//
// myCalcScale.cpp
//   2016-08-10
//
#include "stdafx.h"

#include "myCalcScale.h"

bool bValidTime(LPSTR sT0,LPSTR sT1)
{
	char sH[3],sM[3];
	sH[0]=sT0[0];sH[1]=sT0[1];sH[2]='\0';
	sM[0]=sT0[3];sM[1]=sT0[4];sM[2]='\0';
	int n0= atoi(sH)*60+ atoi(sM);
	sH[0]=sT1[0];sH[1]=sT1[1];sH[2]='\0';
	sM[0]=sT1[3];sM[1]=sT1[4];sM[2]='\0';
	int n1= atoi(sH)*60+ atoi(sM);
	
	return (n0-n1)<5?true:false;
}
//�����վʱ�����
void CmyCalcScale::myCalcTimeIndex(CArray<PMRDATA,PMRDATA> *pmrs,LPSTR sTime0)
{
	for(int i=0;i<pmrs->GetCount();i++)
	{
		PMRDATA pmr=pmrs->GetAt(i);
		pmr->nTime =-1;
		int ncount=pmr->Ys.GetCount();
		for(int j=ncount-1;j>=0;j--)
    	{
	    	if( lstrcmp(sTime0,pmr->Ys.GetAt(j)->sTime)>=0)
		    {
				if(bValidTime(sTime0,pmr->Ys.GetAt(j)->sTime))
   			    	pmr->nTime = j;
		    	break;
			}
		}
	}
}

//�����վ��ʱ�ε�Ҫ�ؿ̶����ֵ����Сֵ
void CmyCalcScale::myCalcScaleSnT1Y1(CArray<PMRDATA,PMRDATA> *pmrs,PFXINFO pfx,float *fMaxScale,float *fMinScale)
{
	//�����վ��ʱ�ε�Ҫ�����������Сֵ
	fMaxScale[pfx->nYs]=MaxValue;fMinScale[pfx->nYs]=MinValue;
	for(int k=0;k<pmrs->GetCount();k++)
	{
		PMRDATA pmr=pmrs->GetAt(k);
		if(pmr->nTime>-1 && pmr->nProcessor>-1)//��վ��ǰʱ��������
		{
			PMRYS pYs=pmr->Ys.GetAt(pmr->nTime);
			float *f= pYs->ff[pmr->nProcessor][pfx->nYs];
			//myCalcScaleS1T1Y1(f,fMaxScale[pfx->nYs],fMinScale[pfx->nYs]);
			if(f!=NULL)
			{
				for(int i=0;i<58;i++)
				{
					myCalcMaxMinValue(f[i],fMaxScale[pfx->nYs],fMinScale[pfx->nYs]);
				}
			}
		}
	}
	if(fMaxScale[pfx->nYs] < fMinScale[pfx->nYs])
	{
		fMaxScale[pfx->nYs]=10; fMinScale[pfx->nYs]=0;
	}
	switch(pfx->nYs)
	{
	case 0:
		myCalcScaleTT(fMaxScale[0],fMinScale[0]);//�̶������Сֵ
		break;
	case 1:
		myCalcScaleRH(fMaxScale[1],fMinScale[1]); //�̶������Сֵ
		break;
	case 2:
		myCalcScaleVD(fMaxScale[2],fMinScale[2]);//�̶������Сֵ
		break;
	case 3:
		myCalcScaleLD(fMaxScale[3],fMinScale[3]);//�̶������Сֵ
		break;
	default:
		break;
	}
}
//���㵥վ��ʱ�θ�Ҫ�ؿ̶����ֵ����Сֵ
void CmyCalcScale::myCalcScaleS1T1Yn(int nProcessor,PMRYS pYs,float *fMaxScale,float *fMinScale)
{
	//�¶�
	myCalcScaleS1T1Y1(pYs->ff[nProcessor][0],fMaxScale[0],fMinScale[0]); //���������Сֵ
	myCalcScaleTT(fMaxScale[0],fMinScale[0]);//�̶������Сֵ
	//���ʪ��
	myCalcScaleRH(fMaxScale[1],fMinScale[1]); //�̶������Сֵ
	//ˮ���ܶ�
    myCalcScaleS1T1Y1(pYs->ff[nProcessor][2],fMaxScale[2],fMinScale[2]);// ���������Сֵ
	myCalcScaleVD(fMaxScale[2],fMinScale[2]);//�̶������Сֵ
	//Һˮ����
	myCalcScaleS1T1Y1(pYs->ff[nProcessor][3],fMaxScale[3],fMinScale[3]); //���������Сֵ
	myCalcScaleLD(fMaxScale[3],fMinScale[3]);//�̶������Сֵ
}

//���㵥վ��ʱ�ε�Ҫ�ظ��߶��������ֵ����Сֵ
void CmyCalcScale::myCalcScaleS1T1Y1(float *f,float& fMax,float& fMin)
{
	fMax=MaxValue; fMin=MinValue;
	if(f!=NULL)
	{
		for(int i=0;i<58;i++)
		{
			myCalcMaxMinValue(f[i],fMax,fMin);
		}
		
	}
	if(f==NULL || fMax<fMin)
	{
		fMax=10;fMin=0;
	}
}
void CmyCalcScale::myCalcMaxMinValue(float f,float& fMax,float& fMin)
{
	if( f != MaxValue)
	{
		if(f>fMax)
			fMax=f;
		if(f<fMin)
			fMin=f;
	}
}
//�����¶ȿ̶����ֵ����Сֵ,4�ȷ�
void CmyCalcScale::myCalcScaleTT(float& fMaxScale,float& fMinScale)
{
	if(fMinScale<0)
		fMinScale=(float)((int)fMinScale-1);
	else
		fMinScale=(float)((int)fMinScale);
	float fStep= (fMaxScale-fMinScale)/4;
	fStep= (float)((int)fStep+1);
	fMaxScale=fMinScale+ fStep * 4;
}
//�������ʪ�ȿ̶����ֵ����Сֵ,4�ȷ�
void CmyCalcScale::myCalcScaleRH(float& fMaxScale,float& fMinScale)
{
	fMaxScale=100;
	fMinScale=0;
}
//����ˮ���ܶȿ̶����ֵ����Сֵ,4�ȷ�
void CmyCalcScale::myCalcScaleVD(float& fMaxScale,float& fMinScale)
{
	float fStep= fMaxScale/4;
	fStep= (float)((int)(fStep*10)+1)/10;
	fMinScale=0;
	fMaxScale=fMinScale+ fStep * 4;
}
//����Һˮ�����̶����ֵ����Сֵ,4�ȷ�
void CmyCalcScale::myCalcScaleLD(float& fMaxScale,float& fMinScale)
{
	float fStep= fMaxScale/4;
	if(fStep<0.1)
	{
		fStep= (float)((int)(fStep*100)+1)/100;
	}
	else// if(fStep<1)
	{
		fStep= (float)((int)(fStep*10)+1)/10;
	}
	fMinScale=0;
	fMaxScale=fMinScale+ fStep * 4;
}


//������ѹ�̶����ֵ����Сֵ
void CmyCalcScale::myCalcScalePP(float& fMaxScale,float& fMinScale)
{
	fMinScale=(float)((int)fMinScale);
	float fStep= (fMaxScale-fMinScale)/4;
	fStep= (float)((int)fStep+1);
	fMaxScale=fMinScale+ fStep * 4;
}