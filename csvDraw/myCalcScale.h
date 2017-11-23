//
// myCalcScale.h
//   2016-08-10
//
#pragma once
#include "..\common\common.h"

class CmyCalcScale
{
public:
	static void myCalcTimeIndex(CArray<PMRDATA,PMRDATA> *pmrs,LPSTR sTime0);
	static void myCalcScaleS1T1Yn(int nProcessor,PMRYS pYs,float *fMaxScale,float *fMinScale);
	static void myCalcScaleSnT1Y1(CArray<PMRDATA,PMRDATA> *pmrs,PFXINFO pfx,float *fMaxScale,float *fMinScale);

	static void myCalcScaleS1T1Y1(float *f,float& fMax,float& fMin);
	
	static void myCalcMaxMinValue(float f,float& fMax,float& fMin);

	static void myCalcScaleTT(float& fMaxScale,float& fMinScale);
	static void myCalcScaleRH(float& fMaxScale,float& fMinScale);
	static void myCalcScaleVD(float& fMaxScale,float& fMinScale);
	static void myCalcScaleLD(float& fMaxScale,float& fMinScale);

	static void myCalcScalePP(float& fMaxScale,float& fMinScale);

};