#include "stdafx.h"
#include "myReadHsCsv.h"


//解析一行记录数据
// 格式:时间,数据1,数据2,,,数据10
void CmyReadHsCsv::myParseHsCsvYs(LPSTR lstr,CArray<PHSTATUS,PHSTATUS> *phs)
{
	if(lstr==NULL) return;
	LPSTR pNext;
	int i=0;
	PHSTATUS pdata=NULL;
	while(lstr!=NULL)
	{
		pNext=strstr(lstr,",");
		if(pNext==NULL)
		{
			if(i>0 && pdata!=NULL)
			{
				pdata->value[i-1]= atoi(lstr);
			}
			break;
		}
		else
		{
     		lstrcpyn(sBuf,lstr,pNext-lstr+1);
	    	if(i==0)
	    	{	
				if( sscanf(sBuf,"%[^/]/%[^/]/%s %[^:]:%[^:]:%s",sM,sD,sY,sH,smm,sSe)==6)//解析sBuf中的时间
				{	
					pdata=new HSTATUS;
					wsprintf(pdata->sDate,"%d-%02d-%02d",atoi(sY),atoi(sM),atoi(sD)); //日期
            		wsprintf(pdata->sTime,"%02d:%02d:%02d",atoi(sH),atoi(smm),atoi(sSe)); //时间
					phs->Add(pdata);
				}
				else
				{
					break;
				}				
			}
		    else if(i>0 && pdata!=NULL)
	     	{	
				pdata->value[i-1]= atoi(sBuf);				
			}
	    	lstr=pNext+1;
		}
		if(i==10)
			break;
		i++;
	}	
}
void CmyReadHsCsv::myParseHsCsvMem(LPSTR lstr,CArray<PHSTATUS,PHSTATUS> *phs)
{
	LPSTR pNext;
	char str[1024];
	int i=0;
	while(lstr!=NULL)
	{
		pNext=strstr(lstr,"\r\n");
		if(pNext==NULL)
			break;
		if(i>0)//跳过第一行文字说明
		{
    		lstrcpyn(str,lstr,pNext-lstr+1);
	    	myParseHsCsvYs(str,phs);
		}
		i++;
		//到下一行
	    lstr=pNext+2;
	}//end while(lstr!=NULL)
}

void CmyReadHsCsv::myReadHsCsvFile(LPSTR dPath,PFXINFO pfx,PTZINFO ptz,CArray<PHSTATUS,PHSTATUS> *phs)
{
    HANDLE hf;
    LPSTR lmem;
	DWORD cb,dw;
	char sSearchPath[MAX_PATH];

	CmyReadHsCsv::myReleaseData(phs);
	//状态文件名
	wsprintf(sSearchPath,"%s\\%d年\\%02d月\\%s%s\\healthstatus.csv",dPath,pfx->nYear,pfx->nMonth,ptz->zm,ptz->zh);
	//打开文件
	hf=CreateFile(sSearchPath, GENERIC_READ, FILE_SHARE_READ,(LPSECURITY_ATTRIBUTES)NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,(HANDLE)NULL);
    if(hf!=INVALID_HANDLE_VALUE)
	{
		cb=GetFileSize(hf,NULL);
        lmem=(char *)malloc(cb);
        ReadFile(hf,lmem,cb,&dw,NULL); 
        CloseHandle(hf);  
		//
		myParseHsCsvMem(lmem,phs);
		//
		free(lmem);
	}
}


void CmyReadHsCsv::myReleaseData(CArray<PHSTATUS,PHSTATUS> *phs)
{
	for(int j=phs->GetCount()-1;j>=0;j--)
	{
		PHSTATUS pdata=phs->GetAt(j);
		delete pdata;
	}
	phs->RemoveAll();	
}
