//
// myReadCsv.h
//   2016-07-27
//
#pragma once
#include "..\common\common.h"

class CmyReadCsv
{
public:
	CmyReadCsv(){};
	virtual ~CmyReadCsv(){};

    void myReadCsvFile(LPSTR strFileName,PMRDATA pdata,int nY,int nM,int nD);
	PMRDATA m_pdata;
	PMRYS m_pys;
	char sY[3],sM[3],sD[3],sH[3],smm[3],sSe[3],sBuf[30];
	int  nCount301;

	static void myAddProcessor(CArray<CString,CString&> *plist,CString cs);
    static void myParseProcessorName(LPSTR str);
	static void mySetFH(float *fH);

private:
	void myParseCsv201(LPSTR p2,LPSTR lstr);
	void myParseCsv301(LPSTR lstr);
	void myParseCsv401(int nType,LPSTR lstr);
	void myParseCsvYs(LPSTR lstr);
	void myParseCsvMem(LPSTR lstr);

};

// 数据信息
void myReadData(LPSTR dPath,PFXINFO pfx,CArray<PTZINFO,PTZINFO> *plist_tz,CArray<PMRDATA,PMRDATA> *pmrs,CArray<CString,CString&> *plist_Processor);
void myReleaseData(CArray<PMRDATA,PMRDATA> *pmrs,CArray<CString,CString&> *plist_Processor);
