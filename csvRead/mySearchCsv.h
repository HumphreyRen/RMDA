//
// mySearchCsv.h
//   2016-07-29
//
#pragma once
#include "..\common\myStruct.h"

class CmySearchCsv
{
public:
	static void mySearchNewNian(LPSTR dPath,LPSTR sY);
	static void mySearchNewYue(LPSTR dPath,LPSTR sY,LPSTR sM);
	static bool mySearchNewFile(LPSTR dPath,LPSTR sY,LPSTR sM,LPSTR sZm,LPSTR sZh,PFXINFO pfx);

	static void mySearchNian(LPSTR dPath,CArray<PNIANYUE,PNIANYUE> *plist);
	static void mySearchYue(LPSTR dPath,PNIANYUE plist);
    static void myReleaseNianYue(CArray<PNIANYUE,PNIANYUE> *plist);
	
	static void mySearchCsvFileByDate(LPSTR sPath,int nYear,int nMonth,int nDay,CArray<CString,CString&> *plist);
 
	static void mySearchCsvFileDateByStation(LPSTR sPath,CArray<CString,CString&> *plist);
private:
	static void myAddCsvFileName(CArray<CString,CString&> *plist,LPSTR fn);
	static void myAddCsvFileDate(CArray<CString,CString&> *plist,LPSTR fn);
};