#include "stdafx.h"

#include "mySearchCsv.h"

//�������ݸ�Ŀ¼�����µ����
void CmySearchCsv::mySearchNewNian(LPSTR dPath,LPSTR sY)
{
	HANDLE hff;
    WIN32_FIND_DATA ffd;
	char sSearch[MAX_PATH];
    wsprintf(sSearch,"%s\\*.",dPath);
	lstrcpy(sY,"");
	if((hff=FindFirstFile(sSearch,&ffd))!=INVALID_HANDLE_VALUE)
	{
		do 
	    {
			if((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=0 && lstrcmp(ffd.cFileName,".")!=0 && lstrcmp(ffd.cFileName,"..")!=0)//����Ŀ¼
	        {
				CharUpper(ffd.cFileName);
		        if( lstrcmp(ffd.cFileName,"TEMP")!=0 )
				{
					if( lstrcmp(ffd.cFileName,sY)>0)
					{
						lstrcpyn(sY,ffd.cFileName,7);
					}
				}
			}
		}while(FindNextFile(hff,&ffd)); 
	    FindClose(hff);
	}
}
//�������ݸ�Ŀ¼��ָ���������µ��·�
void CmySearchCsv::mySearchNewYue(LPSTR dPath,LPSTR sY,LPSTR sM)
{
	HANDLE hff;
    WIN32_FIND_DATA ffd;
	char sSearch[MAX_PATH];
	wsprintf(sSearch,"%s\\%s\\*.",dPath,sY);
	lstrcpy(sM,"");
	if((hff=FindFirstFile(sSearch,&ffd))!=INVALID_HANDLE_VALUE)
	{
		do 
	    {
			if((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=0 && lstrcmp(ffd.cFileName,".")!=0 && lstrcmp(ffd.cFileName,"..")!=0)//����Ŀ¼
	        {
				if(lstrcmp(ffd.cFileName,sM)>0)
				{
					lstrcpyn(sM,ffd.cFileName,5);
				}
			}
		}while(FindNextFile(hff,&ffd)); 
	    FindClose(hff);
	}
}
//�������ݸ�Ŀ¼��ָ����,��,վ�������µ�CSV�ļ�
bool CmySearchCsv::mySearchNewFile(LPSTR dPath,LPSTR sYear,LPSTR sMonth,LPSTR sZm,LPSTR sZh,PFXINFO pfx)
{
	HANDLE hff;
    WIN32_FIND_DATA ffd;
	char sSearch[280],sY[5],sM[3],sD[3],sFile[30];
	
	wsprintf(sSearch,"%s\\%s\\%s\\%s%s\\*_lv2.csv",dPath,sYear,sMonth,sZm,sZh);
	lstrcpy(sFile,"");
	if( (hff=FindFirstFile(sSearch,&ffd)) != INVALID_HANDLE_VALUE )
	{
		do
		{
			if(lstrcmp(ffd.cFileName,sFile)>0)
			{
    			lstrcpy(sFile,ffd.cFileName);
			}
		}while(FindNextFile(hff,&ffd));
		FindClose(hff);	
	}
	if(lstrcmp(sFile,"")>0)
	{
	    wsprintf(sSearch,"%s\\%s\\%s\\%s%s\\%s",dPath,sYear,sMonth,sZm,sZh,sFile);
		hff=CreateFile(sSearch, GENERIC_READ, FILE_SHARE_READ,(LPSECURITY_ATTRIBUTES)NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,(HANDLE)NULL);
        if(hff!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(hff);
			if( sscanf(sFile,"%[^-]-%[^-]-%[^_]_%*s",sY,sM,sD)==3)
			{
				pfx->nYear = atoi(sY);
				pfx->nMonth =atoi(sM);
				pfx->nDay =atoi(sD);
				return true;
			}
		}
	}
	return false;
}

//�������ݸ�Ŀ¼��������
void CmySearchCsv::mySearchNian(LPSTR dPath,CArray<PNIANYUE,PNIANYUE> *plist)
{
	HANDLE hff;
    WIN32_FIND_DATA ffd;
	char sSearch[MAX_PATH];

	myReleaseNianYue(plist);
	wsprintf(sSearch,"%s\\*.",dPath);
	if((hff=FindFirstFile(sSearch,&ffd))!=INVALID_HANDLE_VALUE)
	{
		do 
	    {
			if((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=0 && lstrcmp(ffd.cFileName,".")!=0 && lstrcmp(ffd.cFileName,"..")!=0)//����Ŀ¼
	        {
				CharUpper(ffd.cFileName);
		        if( lstrcmp(ffd.cFileName,"TEMP")!=0 )
				{
					PNIANYUE pdata= new NIANYUE;
					pdata->nian = ffd.cFileName;
					plist->InsertAt(0,pdata);
					mySearchYue(dPath,pdata);
				}
			}
		}while(FindNextFile(hff,&ffd)); 
	    FindClose(hff);
	}
}
//�������ݸ�Ŀ¼��ָ�����������
void CmySearchCsv::mySearchYue(LPSTR dPath,PNIANYUE plist)
{
	HANDLE hff;
    WIN32_FIND_DATA ffd;
	char sSearch[MAX_PATH];
	wsprintf(sSearch,"%s\\%s\\*.",dPath,plist->nian);
	if((hff=FindFirstFile(sSearch,&ffd))!=INVALID_HANDLE_VALUE)
	{
		do 
	    {
			if((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=0 && lstrcmp(ffd.cFileName,".")!=0 && lstrcmp(ffd.cFileName,"..")!=0)//����Ŀ¼
	        {
				CString yue=ffd.cFileName;
				plist->yue.InsertAt(0,yue);
			}
		}while(FindNextFile(hff,&ffd)); 
	    FindClose(hff);
	}
}
//�ͷ������б�
void CmySearchCsv::myReleaseNianYue(CArray<PNIANYUE,PNIANYUE> *plist)
{
	for(int i=0;i<plist->GetCount();i++)
	{
		PNIANYUE pdata=plist->GetAt(i);
		pdata->yue.RemoveAll();
        delete pdata;
	}
	plist->RemoveAll();
}

//����ǰ����˳��,���CSV�ļ�����plist��
void CmySearchCsv::myAddCsvFileName(CArray<CString,CString&> *plist,LPSTR fn)
{
	CString str=fn;
	for(int i=0;i<plist->GetCount();i++)
	{
		CString cs=plist->GetAt(i);
		if( cs.CompareNoCase(str)>0)
		{
			plist->InsertAt(i,str);
			return;
		}
	}
	plist->Add(str);
}
//����·���������������е�CSV�ļ�,���뵽plist�б���
void CmySearchCsv::mySearchCsvFileByDate(LPSTR sPath,int nYear,int nMonth,int nDay,CArray<CString,CString&> *plist)
{
	WIN32_FIND_DATA filedata;
    HANDLE hsearch;
	char sSearch[280];
    //��������ͨ����	
   	wsprintf(sSearch,"%s\\%d-%02d-%02d_*_lv2.csv",sPath,nYear,nMonth,nDay);
	if( (hsearch=FindFirstFile(sSearch,&filedata)) != INVALID_HANDLE_VALUE )
	{
		do
		{
			myAddCsvFileName(plist,filedata.cFileName);
		}while(FindNextFile(hsearch,&filedata));
		FindClose(hsearch);	
	}
}

//���ݲ�վ����·�����������е�CSV�ļ�����,���뵽plist�б���
void CmySearchCsv::mySearchCsvFileDateByStation(LPSTR sPath,CArray<CString,CString&> *plist)
{
	WIN32_FIND_DATA filedata;
    HANDLE hsearch;
	char sSearch[280];
    //��������ͨ����	
   	wsprintf(sSearch,"%s\\*_lv2.csv",sPath);
	if( (hsearch=FindFirstFile(sSearch,&filedata)) != INVALID_HANDLE_VALUE )
	{
		do
		{
			if(lstrlen(filedata.cFileName)>18)
			{
    			lstrcpyn(sSearch,filedata.cFileName,11);
	    		myAddCsvFileDate(plist,sSearch);
			}
		}while(FindNextFile(hsearch,&filedata));
		FindClose(hsearch);	
	}
}
//���Ӻ�ǰ˳��,���CSV�ļ����ڵ�plist��,����
void CmySearchCsv::myAddCsvFileDate(CArray<CString,CString&> *plist,LPSTR fn)
{
	CString str=fn;
	for(int i=0;i<plist->GetCount();i++)
	{
		CString cs=plist->GetAt(i);
		if( cs.CompareNoCase(str)<0)
		{
			plist->InsertAt(i,str);
			return;
		}
		else if( cs.CompareNoCase(str)==0)
		{
			return;
		}
	}
	plist->Add(str);
}