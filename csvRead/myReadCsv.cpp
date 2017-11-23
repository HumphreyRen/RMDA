#include "stdafx.h"
#include "mySearchCsv.h"
#include "myReadCsv.h"

void CmyReadCsv::myParseProcessorName(LPSTR str)
{
	if(strstr(str,"ZenithBL")!=NULL)
		lstrcpy(str,"天顶边界层");	
	else if(strstr(str,"Zenith")!=NULL)
		lstrcpy(str,"天顶观测");
	else if(strstr(str,"Angle")!=NULL)
	{		
		char a[4];
		if( sscanf(str,"%*[^0-9]%[0-9]",a)==1)
		{
			if(strstr(str,"(N)")!=NULL)
			    wsprintf(str,"天顶角%s(N)",a);
			else if(strstr(str,"(S)")!=NULL)
			    wsprintf(str,"天顶角%s(S)",a);
			else if(strstr(str,"(A)")!=NULL)
			    wsprintf(str,"天顶角%s(A)",a);
		}
	}
}
void CmyReadCsv::myAddProcessor(CArray<CString,CString&> *plist,CString cs)
{
	int nCount=plist->GetCount(); 
	for(int i=0;i<nCount;i++)
	{
		if( cs.CompareNoCase( plist->GetAt(i) )==0)//相同处理器名
		{
			return;
		}
	}
	plist->Add(cs);
}

//解析记录类型201 
// 格式: 地面温度,地面湿度,地面气压,云底温度,有无降水(0,1),无用
void CmyReadCsv::myParseCsv201(LPSTR p2,LPSTR lstr)
{
	if( sscanf(p2,"%[^/]/%[^/]/%s %[^:]:%[^:]:%[^,]",sM,sD,sY,sH,smm,sSe)==6)//解析p2中的时间
	{
		//在pdata中加入新的PMRYS
    	PMRYS pys= new MRYS;
		wsprintf(pys->sDate,"20%02d-%02d-%02d",atoi(sY),atoi(sM),atoi(sD)); //日期
		wsprintf(pys->sTime,"%02d:%02d:%02d",atoi(sH),atoi(smm),atoi(sSe)); //时间
    	pys->fCH = pys->fCT = pys->fLW = pys->fPP = pys->fRH  = pys->fSQ = pys->fTT = MaxValue;
		pys->nEO =0;
		for(int p=0;p<4;p++)
		{
			for(int n=0;n<4;n++)
			{
				pys->ff[p][n]=NULL;
			}
		}
		m_pdata->Ys.Add(pys);
		m_pys=pys;
    	nCount301=0;
		//解析记录201
		LPSTR pNext;
		int i=0;
		while(lstr!=NULL)
		{
			pNext=strstr(lstr,",");
			if(pNext==NULL)
			{
				if(i==4)
					pys->nEO = atoi(lstr); //有无降水(0,1)	
				break;
			}
			else
			{
				lstrcpyn(sBuf,lstr,pNext-lstr+1);
				if(i==0)
					pys->fTT =(float)atof(sBuf)-273.15f; //地面温度
				else if(i==1)
					pys->fRH =(float)atof(sBuf); //地面湿度
				else if(i==2)
					pys->fPP =(float)atof(sBuf); //地面气压
				else if(i==3)
					pys->fCT =(float)atof(sBuf)-273.15f; //云底温度
				else if(i==4)
					pys->nEO = atoi(sBuf); //有无降水(0,1)			
				//
				lstr=pNext+1;
			}
			i++;
		}
	}
}
//解析记录类型301 
// 格式:积分水汽含量,积分液态水,云底高度,无用
void CmyReadCsv::myParseCsv301(LPSTR lstr)
{
	if(m_pys==NULL || nCount301>0)
		return;
	nCount301++;
	//解析记录301
	LPSTR pNext;
	int i=0;
	while(lstr!=NULL)
	{
		pNext=strstr(lstr,",");
		if(pNext==NULL)
		{
			if(i==2)
				m_pys->fCH = (float)atof(lstr); //云底高度	
			break;
		}
		else
		{	
			lstrcpyn(sBuf,lstr,pNext-lstr+1);
			if(i==0)
				m_pys->fSQ =(float)atof(sBuf); //积分水汽含量
			else if(i==1)
				m_pys->fLW =(float)atof(sBuf); //积分液态水
		    else if(i==2)
				m_pys->fCH = (float)atof(sBuf); //云底高度
			//
			lstr=pNext+1;
		}
		i++;
	}
}

//解析记录类型401 
// 格式:处理器,数据1,数据2,,,数据58
void CmyReadCsv::myParseCsv401(int nType,LPSTR lstr)
{
	if(m_pys==NULL)
		return;
	LPSTR pNext;
	int  nProcessor;
	//
	int i=0;
	while(lstr!=NULL)
	{
		pNext=strstr(lstr,",");
		if(pNext==NULL)
		{
			if(i>0)
			{
				m_pys->ff[nProcessor][nType][i-1] = nType==0? (float)atof(lstr)-273.15f : (float)atof(lstr);
			}
			break;
		}
		else
		{
     		lstrcpyn(sBuf,lstr,pNext-lstr+1);
	    	if(i==0)
	    	{	
				nProcessor=4;
				for(int k=0;k<4;k++)
				{
					if( lstrcmp(m_pdata->sProcessors[k],sBuf)==0)
					{
						nProcessor=k;
						break;
					}
					else if( lstrcmp(m_pdata->sProcessors[k],"")==0)
					{
						lstrcpy(m_pdata->sProcessors[k],sBuf);
						nProcessor=k;
						break;
					}
				}				
				//myParseProcessorName(sBuf,sProcessor);				
				//nProcessor = myAddProcessor(&(m_pdata->list_Processor),sProcessor);
				if( nProcessor>3) //单站最多读取4个数据观测类型
				{
					break;
				}
				m_pys->ff[nProcessor][nType]=(float *) malloc(58*sizeof(float));
				
			}
		    else 
	     	{	
				m_pys->ff[nProcessor][nType][i-1] = nType==0? (float)atof(sBuf)-273.15f : (float)atof(sBuf);				
			}
	    	lstr=pNext+1;
		}
		if(i==58)
			break;
		i++;
	}
}

//解析一行记录数据
void CmyReadCsv::myParseCsvYs(LPSTR lstr)
{
	if(lstr==NULL) return;
	//跳过第1个记录
	LPSTR pNext,p2;
	if((pNext=strstr(lstr,","))!=NULL)
		lstr=pNext+1; //指向第2个记录
	//记住并跳过第2个记录
	p2=lstr;
	if((pNext=strstr(lstr,","))!=NULL)
		lstr=pNext+1;//指向第3个记录

	if(lstr==NULL) return;
	//第3个记录:记录类型
    pNext=strstr(lstr,",");
	if(pNext==NULL) return;
	lstrcpyn(sBuf,lstr,pNext-lstr+1);
	lstr=pNext+1;//指向第4个记录头

	switch(atoi(sBuf))
	{
	case 201:
		myParseCsv201(p2,lstr);
		break;
	case 301:
		myParseCsv301(lstr);
	    break;
	case 401:
		myParseCsv401(0,lstr);
	    break;
	case 402:
		myParseCsv401(2,lstr);
	    break;
	case 403:
		myParseCsv401(3,lstr);
	    break;
	case 404:
		myParseCsv401(1,lstr);
	    break;
	default:
		break;
	}
}
void CmyReadCsv::myParseCsvMem(LPSTR lstr)
{
	LPSTR pNext;
	char str[1024];
	while(lstr!=NULL)
	{
		pNext=strstr(lstr,"\r\n");
		if(pNext==NULL)
			break;
		lstrcpyn(str,lstr,pNext-lstr+1);
		myParseCsvYs(str);
		//到下一行
	    lstr=pNext+2;
	}//end while(lstr!=NULL)
}
void CmyReadCsv::myReadCsvFile(LPSTR strFileName,PMRDATA pdata,int nY,int nM,int nD)
{
    HANDLE hf;
    LPSTR lmem;
	DWORD cb,dw;
		
	//wsprintf(sD,"%d/%02d/%02d",nY,nM,nD);//资料日期
	hf=CreateFile(strFileName, GENERIC_READ, FILE_SHARE_READ,(LPSECURITY_ATTRIBUTES)NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,(HANDLE)NULL);
    if(hf!=INVALID_HANDLE_VALUE)
	{
		cb=GetFileSize(hf,NULL);
        lmem=(char *)malloc(cb);
        ReadFile(hf,lmem,cb,&dw,NULL); 
        CloseHandle(hf);  
		//
		m_pdata=pdata;
		m_pys=NULL;
		myParseCsvMem(lmem);
		//
		free(lmem);
	}
}
void CmyReadCsv::mySetFH(float *fH)
{
	fH[0]=0.00f;fH[1]=0.05f;fH[2]=0.10f;fH[3]=0.15f;fH[4]=0.20f;fH[5]=0.25f;fH[6]=0.30f;fH[7]=0.35f;fH[8]=0.40f;fH[9]=0.45f;
	fH[10]=0.50f;fH[11]=0.60f;fH[12]=0.70f;fH[13]=0.80f;fH[14]=0.90f;fH[15]=1.00f;fH[16]=1.10f;fH[17]=1.20f;fH[18]=1.30f;fH[19]=1.40f;
	fH[20]=1.50f;fH[21]=1.60f;fH[22]=1.70f;fH[23]=1.80f;fH[24]=1.90f;fH[25]=2.00f;fH[26]=2.25f;fH[27]=2.50f;fH[28]=2.75f;fH[29]=3.00f;
	fH[30]=3.25f;fH[31]=3.50f;fH[32]=3.75f;fH[33]=4.00f;fH[34]=4.25f;fH[35]=4.50f;fH[36]=4.75f;fH[37]=5.00f;fH[38]=5.25f;fH[39]=5.50f;
	fH[40]=5.75f;fH[41]=6.00f;fH[42]=6.25f;fH[43]=6.50f;fH[44]=6.75f;fH[45]=7.00f;fH[46]=7.25f;fH[47]=7.50f;fH[48]=7.75f;fH[49]=8.00f;
	fH[50]=8.25f;fH[51]=8.50f;fH[52]=8.75f;fH[53]=9.00f;fH[54]=9.25f;fH[55]=9.50f;fH[56]=9.75f;fH[57]=10.00f; 
}

//数据文件读取线程
void myReadDataOne(void * pParam)
{
	PRSVPARAM rsv=(PRSVPARAM)pParam;
	char sSearchPath[MAX_PATH];
	wsprintf(sSearchPath,"%s\\%d年\\%02d月\\%s%s",rsv->sPath,rsv->nY,rsv->nM,rsv->sZm,rsv->sZh);
	for(int k=0;k<4;k++)
	{
	    lstrcpy(rsv->pdata->sProcessors[k],"");
	}
	//搜索指定年月日的所有文件
	CArray<CString,CString&> list_fn;
	CmySearchCsv::mySearchCsvFileByDate(sSearchPath,rsv->nY,rsv->nM,rsv->nD,&list_fn);
	CmyReadCsv myReadCsv;
	//读取列表中的所有文件
	for(int i=0;i<list_fn.GetCount();i++)
	{
    	wsprintf(sSearchPath,"%s\\%d年\\%02d月\\%s%s\\%s",rsv->sPath,rsv->nY,rsv->nM,rsv->sZm,rsv->sZh,list_fn.GetAt(i));
	   	myReadCsv.myReadCsvFile(sSearchPath,rsv->pdata,rsv->nY,rsv->nM,rsv->nD);
	}
	list_fn.RemoveAll();
	//修改pdata中的处理器名 
	for(int k=0;k<4;k++)
	{
	    if( lstrcmp(rsv->pdata->sProcessors[k],"")!=0)
		{
			CmyReadCsv::myParseProcessorName(rsv->pdata->sProcessors[k]);			
		}
	}		
	//移除pdata->ys中的空数据
	for(int i=rsv->pdata->Ys.GetCount()-1;i>=0;i--)
	{
	   	PMRYS pys=rsv->pdata->Ys.GetAt(i);
		if(pys->ff[0][0]==NULL)
		{
    		for(int p=0;p<4;p++)
			{
         		for(int n=0;n<4;n++)
	    	    {
		    	   	if(pys->ff[p][n]!=NULL)
			        	free(pys->ff[p][n]);
				}
		    }
	    	delete pys;
			rsv->pdata->Ys.RemoveAt(i); 
		}
	}
	SetEvent(rsv->hFinish);
   	AfxEndThread(200,true);
}

//读取观测数据,按台站和日期检索文件
void myReadData(LPSTR dPath,PFXINFO pfx,CArray<PTZINFO,PTZINFO> *plist_tz,CArray<PMRDATA,PMRDATA> *pmrs,CArray<CString,CString&> *plist_Processor)
{
	SetCursor(LoadCursor(NULL, IDC_WAIT));
	myReleaseData(pmrs,plist_Processor);

	CArray<PRSVPARAM,PRSVPARAM> list_rsv;    //数据读取线程阵列

	if( pfx->nFX>10 ) //多站1天数据
	{
		for(int j=0;j<plist_tz->GetCount();j++)
		{	
			PTZINFO ptz=plist_tz->GetAt(j);
			PMRDATA pdata=new MRDATA;
			pmrs->Add(pdata);
			PRSVPARAM rsv=new RSVPARAM;
	        rsv->hFinish  = CreateEvent(NULL,TRUE,FALSE,NULL);
			rsv->sPath = dPath;
			rsv->sZm  = ptz->zm;
			rsv->sZh  = ptz->zh;
			rsv->nY = pfx->nYear;
			rsv->nM = pfx->nMonth;
			rsv->nD = pfx->nDay;
			rsv->pdata = pdata;
      	    list_rsv.Add(rsv);
	        AfxBeginThread((AFX_THREADPROC)myReadDataOne, rsv);
		}
	}
	else//单站3天数据
	{
		PTZINFO ptz=plist_tz->GetAt(pfx->nStation);
		for(int j=0;j<3;j++)
		{
			PMRDATA pdata=new MRDATA;
			pmrs->Add(pdata);
			PRSVPARAM rsv=new RSVPARAM;
	        rsv->hFinish  = CreateEvent(NULL,TRUE,FALSE,NULL);
			rsv->sPath = dPath;
			rsv->sZm  = ptz->zm;
			rsv->sZh  = ptz->zh;
			rsv->nY = pfx->nYear;
			rsv->nM = pfx->nMonth;
			rsv->nD = pfx->nDay;
            myGetNextDay(rsv->nY,rsv->nM,rsv->nD,-1*j);
			rsv->pdata = pdata;
      	    list_rsv.Add(rsv);
	        AfxBeginThread((AFX_THREADPROC)myReadDataOne, rsv);
		}
	}
	//等待数据读取线程退出
	for(int i=0;i<list_rsv.GetCount();i++)
    {
		PRSVPARAM rsv=list_rsv.GetAt(i);
        WaitForSingleObject(rsv->hFinish, INFINITE);
	    CloseHandle(rsv->hFinish);
	    delete rsv;
    }
    list_rsv.RemoveAll();
	//添加pdata中的处理器到全局处理器列表 
	for(int i=0;i<pmrs->GetCount();i++)
	{
		PMRDATA pdata=pmrs->GetAt(i);
    	for(int k=0;k<4;k++)
     	{
			if( lstrcmp(pdata->sProcessors[k],"")!=0)
			{
				CmyReadCsv::myAddProcessor(plist_Processor,pdata->sProcessors[k]);
			}
		}
	}		
	SetCursor(LoadCursor(NULL, IDC_ARROW));
}
void myReleaseData(CArray<PMRDATA,PMRDATA> *pmrs,CArray<CString,CString&> *plist_Processor)
{
	for(int j=pmrs->GetCount()-1;j>=0;j--)
	{
		PMRDATA pdata=pmrs->GetAt(j);
		for(int i=pdata->Ys.GetCount()-1;i>=0;i--)
	    {
	    	PMRYS pys=pdata->Ys.GetAt(i);
			for(int p=0;p<4;p++)
			{
         		for(int n=0;n<4;n++)
	    	    {
		    	   	if(pys->ff[p][n]!=NULL)
			        	free(pys->ff[p][n]);
				}
		    }
	    	delete pys;
		}
		pdata->Ys.RemoveAll();
		delete pdata;
	}
	pmrs->RemoveAll();	
	plist_Processor->RemoveAll();
}
