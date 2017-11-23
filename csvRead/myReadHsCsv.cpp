#include "stdafx.h"
#include "myReadHsCsv.h"


//����һ�м�¼����
// ��ʽ:ʱ��,����1,����2,,,����10
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
				if( sscanf(sBuf,"%[^/]/%[^/]/%s %[^:]:%[^:]:%s",sM,sD,sY,sH,smm,sSe)==6)//����sBuf�е�ʱ��
				{	
					pdata=new HSTATUS;
					wsprintf(pdata->sDate,"%d-%02d-%02d",atoi(sY),atoi(sM),atoi(sD)); //����
            		wsprintf(pdata->sTime,"%02d:%02d:%02d",atoi(sH),atoi(smm),atoi(sSe)); //ʱ��
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
		if(i>0)//������һ������˵��
		{
    		lstrcpyn(str,lstr,pNext-lstr+1);
	    	myParseHsCsvYs(str,phs);
		}
		i++;
		//����һ��
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
	//״̬�ļ���
	wsprintf(sSearchPath,"%s\\%d��\\%02d��\\%s%s\\healthstatus.csv",dPath,pfx->nYear,pfx->nMonth,ptz->zm,ptz->zh);
	//���ļ�
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
