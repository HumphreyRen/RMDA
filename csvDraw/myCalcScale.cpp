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
//计算各站时次序号
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

//计算多站单时次单要素刻度最大值和最小值
void CmyCalcScale::myCalcScaleSnT1Y1(CArray<PMRDATA,PMRDATA> *pmrs,PFXINFO pfx,float *fMaxScale,float *fMinScale)
{
	//计算各站单时次单要素数据最大最小值
	fMaxScale[pfx->nYs]=MaxValue;fMinScale[pfx->nYs]=MinValue;
	for(int k=0;k<pmrs->GetCount();k++)
	{
		PMRDATA pmr=pmrs->GetAt(k);
		if(pmr->nTime>-1 && pmr->nProcessor>-1)//该站当前时次有数据
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
		myCalcScaleTT(fMaxScale[0],fMinScale[0]);//刻度最大最小值
		break;
	case 1:
		myCalcScaleRH(fMaxScale[1],fMinScale[1]); //刻度最大最小值
		break;
	case 2:
		myCalcScaleVD(fMaxScale[2],fMinScale[2]);//刻度最大最小值
		break;
	case 3:
		myCalcScaleLD(fMaxScale[3],fMinScale[3]);//刻度最大最小值
		break;
	default:
		break;
	}
}
//计算单站单时次各要素刻度最大值和最小值
void CmyCalcScale::myCalcScaleS1T1Yn(int nProcessor,PMRYS pYs,float *fMaxScale,float *fMinScale)
{
	//温度
	myCalcScaleS1T1Y1(pYs->ff[nProcessor][0],fMaxScale[0],fMinScale[0]); //数据最大最小值
	myCalcScaleTT(fMaxScale[0],fMinScale[0]);//刻度最大最小值
	//相对湿度
	myCalcScaleRH(fMaxScale[1],fMinScale[1]); //刻度最大最小值
	//水汽密度
    myCalcScaleS1T1Y1(pYs->ff[nProcessor][2],fMaxScale[2],fMinScale[2]);// 数据最大最小值
	myCalcScaleVD(fMaxScale[2],fMinScale[2]);//刻度最大最小值
	//液水含量
	myCalcScaleS1T1Y1(pYs->ff[nProcessor][3],fMaxScale[3],fMinScale[3]); //数据最大最小值
	myCalcScaleLD(fMaxScale[3],fMinScale[3]);//刻度最大最小值
}

//计算单站单时次单要素各高度数据最大值和最小值
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
//计算温度刻度最大值和最小值,4等分
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
//计算相对湿度刻度最大值和最小值,4等分
void CmyCalcScale::myCalcScaleRH(float& fMaxScale,float& fMinScale)
{
	fMaxScale=100;
	fMinScale=0;
}
//计算水汽密度刻度最大值和最小值,4等分
void CmyCalcScale::myCalcScaleVD(float& fMaxScale,float& fMinScale)
{
	float fStep= fMaxScale/4;
	fStep= (float)((int)(fStep*10)+1)/10;
	fMinScale=0;
	fMaxScale=fMinScale+ fStep * 4;
}
//计算液水含量刻度最大值和最小值,4等分
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


//计算气压刻度最大值和最小值
void CmyCalcScale::myCalcScalePP(float& fMaxScale,float& fMinScale)
{
	fMinScale=(float)((int)fMinScale);
	float fStep= (fMaxScale-fMinScale)/4;
	fStep= (float)((int)fStep+1);
	fMaxScale=fMinScale+ fStep * 4;
}