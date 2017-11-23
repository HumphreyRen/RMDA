//
// myReadHsCsv.h
//   2017-11-18
//
#pragma once
#include "..\common\common.h"

class CmyReadHsCsv
{
public:
    CmyReadHsCsv(){};
	virtual ~CmyReadHsCsv(){};

	void myReadHsCsvFile(LPSTR dPath,PFXINFO pfx,PTZINFO ptz,CArray<PHSTATUS,PHSTATUS> *phs);

	char sY[5],sM[3],sD[3],sH[3],smm[3],sSe[3],sBuf[30];
 
    static void myReleaseData(CArray<PHSTATUS,PHSTATUS> *phs);

private:
	void myParseHsCsvYs(LPSTR lstr,CArray<PHSTATUS,PHSTATUS> *phs);
	void myParseHsCsvMem(LPSTR lstr,CArray<PHSTATUS,PHSTATUS> *phs);

};
