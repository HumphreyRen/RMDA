
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "rmda.h"

#include "MainFrm.h"
#include "rmdaDoc.h"
#include "rmdaView.h"

#include "DlgSetTZ.h"
#include "DlgSetSjfx.h"
#include "..\\common\\common.h"
#include "..\\csvRead\\mySearchCsv.h"
#include "..\\csvRead\\myReadCsv.h"
#include "..\\csvRead\\myReadHsCsv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

char gszpath[128];
FXPARAM fxparam;
FXINFO fxinfo;                   //
CArray<PTZINFO,PTZINFO> list_tz; //̨վ��Ϣ�б�
CArray<PMRDATA,PMRDATA> mrdata;  //�۲�����
CArray<CString,CString&> list_Processor;//�������б�
CArray<PHSTATUS,PHSTATUS> mrhs; //�豸����״̬
float g_fH[58];    //�۲�߶�,km
bool bAutoUpdate;  //�Ƿ��Զ�����

PROCESS_INFORMATION  piOutImg;  //��ʱ��ͼ���������Ϣ
UINT uMsgOutImg;                //��ʱ��ͼ������Ϣ��ʶ��

void myBroadcastSystemMessage()
{
	DWORD dwRecipients = BSM_APPLICATIONS;
	WPARAM wParam = 0;
	LPARAM lParam = 0;

	BroadcastSystemMessage( 
		BSF_IGNORECURRENTTASK, // do not send message to this process
		&dwRecipients,         // broadcast only to applications
		uMsgOutImg,            // registered private message
		wParam,                // message-specific value
		lParam );              // message-specific value
}
void myCreateOutImgProcess()
{
	STARTUPINFO si;
    char sCmd[260];
	
	//ע��ϵͳ��Ϣ
	uMsgOutImg =RegisterWindowMessage("RMDA-OUTIMG.EXE");
	//����������
	wsprintf(sCmd,"%s\\outimg.exe",gszpath);
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
	if( !CreateProcess( NULL,   // No module name (use command line)
          sCmd,           // Command line
          NULL,           // Process handle not inheritable
          NULL,           // Thread handle not inheritable
          FALSE,          // Set handle inheritance to FALSE
          0,              // No creation flags
          NULL,           // Use parent's environment block
          NULL,           // Use parent's starting directory 
          &si,            // Pointer to STARTUPINFO structure
          &(piOutImg))     // Pointer to PROCESS_INFORMATION structure
    ) 
    {
         printf( "CreateProcess failed (%d).\n", GetLastError() );
         return;
    }
}

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()

	ON_CBN_SELCHANGE(IDW_DATATYPE,OnCbnSelchangeDatatype)
	ON_CBN_SELCHANGE(IDW_TIME,OnCbnSelchangeTime)
	ON_CBN_SELCHANGE(IDW_HIGH,OnCbnSelchangeHigh)
	ON_COMMAND(IDW_TIME_LEFT, OnTimeLeft)
	ON_COMMAND(IDW_TIME_RIGHT, OnTimeRight)
	ON_COMMAND(IDW_HIGH_UP, OnHighUp)
	ON_COMMAND(IDW_HIGH_DOWN, OnHighDown)

	ON_UPDATE_COMMAND_UI(IDW_DATATYPE,OnUpdateTypeBox)
	ON_UPDATE_COMMAND_UI(IDW_TIME_LEFT,OnUpdateTimeBox)
	ON_UPDATE_COMMAND_UI(IDW_TIME_RIGHT,OnUpdateTimeBox)
	ON_UPDATE_COMMAND_UI(IDW_TIME,OnUpdateTimeBox)
	ON_UPDATE_COMMAND_UI(IDW_HIGH,OnUpdateHighBox)
	ON_UPDATE_COMMAND_UI(IDW_HIGH_UP,OnUpdateHighBox)
	ON_UPDATE_COMMAND_UI(IDW_HIGH_DOWN,OnUpdateHighBox)

	ON_COMMAND(ID_MYSET_TZ, &CMainFrame::OnMysetTz)	
	ON_COMMAND(ID_MYSET_SJFX, &CMainFrame::OnMysetSjfx)

	ON_WM_TIMER()
	ON_COMMAND(ID_VIEW_AUTO_UPDATE, &CMainFrame::OnViewAutoUpdate)
	ON_UPDATE_COMMAND_UI(ID_VIEW_AUTO_UPDATE, &CMainFrame::OnUpdateViewAutoUpdate)
	ON_COMMAND(ID_VIEW_DLGBAR, &CMainFrame::OnViewDlgbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DLGBAR, OnUpdateViewDlgbar)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	//ID_INDICATOR_NUM,
	//ID_INDICATOR_SCRL,
};
static UINT BASED_CODE styles[] =
{
	ID_SEPARATOR,   //0:IDW_DATATYPE
	ID_SEPARATOR,   //1:IDW_TIME
	ID_SEPARATOR,   //2:IDW_HIGH
	IDW_TIME_LEFT,
	IDW_TIME_RIGHT,
	IDW_HIGH_DOWN,
	IDW_HIGH_UP,
	ID_VIEW_DLGBAR,
};
// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	GetCurrentDirectory(128,gszpath); 
	myReadTZInfo(&list_tz,gszpath);
	myReadSjfxParam(&fxparam,gszpath);
	CmyReadCsv::mySetFH(g_fH);
	SYSTEMTIME st;
	GetLocalTime(&st);
	fxinfo.nYear=st.wYear; fxinfo.nMonth=st.wMonth; fxinfo.nDay=st.wDay;
}

CMainFrame::~CMainFrame()
{
	if(piOutImg.hProcess !=NULL)
		myBroadcastSystemMessage();
	myReleaseData(&mrdata,&list_Processor);
	myReleaseTZInfo(&list_tz);
	CmyReadHsCsv::myReleaseData(&mrhs);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	//������
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME) ||
		!m_wndToolBar.SetButtons(styles, sizeof(styles)/sizeof(UINT)))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}
	//���ݴ�������
	CRect rect;	
	m_wndToolBar.SetButtonInfo(0, IDW_DATATYPE, TBBS_SEPARATOR, 110);
	m_wndToolBar.GetItemRect(0, &rect);
	rect.right -=5;
	rect.bottom = rect.top + 100;
	if (!m_wndToolBar.m_datatype.Create(CBS_DROPDOWNLIST|WS_VSCROLL|WS_VISIBLE|WS_TABSTOP,rect, &m_wndToolBar, IDW_DATATYPE))
	{
		return -1;
	}
	//ʱ��
	m_wndToolBar.SetButtonInfo(1, IDW_TIME, TBBS_SEPARATOR, 90);
	m_wndToolBar.GetItemRect(1, &rect);
	rect.right -=5;
	rect.bottom = rect.top + 100;
	if (!m_wndToolBar.m_time.Create(CBS_DROPDOWNLIST|WS_VSCROLL|WS_VISIBLE|WS_TABSTOP,rect, &m_wndToolBar, IDW_TIME))
	{
		return -1;
	}
	//�߶�
	m_wndToolBar.SetButtonInfo(2, IDW_HIGH, TBBS_SEPARATOR, 70);
	m_wndToolBar.GetItemRect(2, &rect);
	rect.right -=5;
	rect.bottom = rect.top + 100;
	if (!m_wndToolBar.m_high.Create(CBS_DROPDOWNLIST|WS_VSCROLL|WS_VISIBLE|WS_TABSTOP,rect, &m_wndToolBar, IDW_HIGH))
	{
		return -1;
	}

	//״̬��
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	m_wndStatusBar.SetPaneInfo(0,0,SBPS_POPOUT,200);
	m_wndStatusBar.SetPaneInfo(1,0,SBPS_STRETCH,600);
	m_wndStatusBar.SetPaneText(1,"");

	//�Ի���
	if (!m_wndDlgBar.Create(this, IDD_DIALOGBAR,
		CBRS_LEFT|CBRS_TOOLTIPS|CBRS_FLYBY, IDD_DIALOGBAR))
	{
		TRACE0("Failed to create DlgBar\n");
		return -1;      // fail to create
	}
	//������ʱ��ͼ�������
	myCreateOutImgProcess();
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ��������ʽ
	//���Ʋ�ע���´�����
	WNDCLASS  wc;
	GetClassInfo(AfxGetInstanceHandle(),cs.lpszClass,&wc);
	wc.lpszClassName = "RMDA-MAIN";
	AfxRegisterClass(&wc);
	cs.lpszClass = wc.lpszClassName;//�����µ�����	
	//�޸Ĵ�����ʽ
    cs.style &= ~FWS_ADDTOTITLE | CS_OWNDC | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	return TRUE;
}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������
//���ô������б��
void CMainFrame::mySetProcessor()
{
	m_wndToolBar.m_datatype.ResetContent();
	int nCount=list_Processor.GetCount();
	for(int i=0;i<nCount;i++)
	{
		m_wndToolBar.m_datatype.AddString(list_Processor.GetAt(i));
	}
	if(fxinfo.nProcessor>(nCount-1))
		fxinfo.nProcessor=0;
	m_wndToolBar.m_datatype.SetCurSel(fxinfo.nProcessor);
}

//���ø߶���Ͽ�
void CMainFrame::mySetHigh()
{
	m_wndToolBar.m_high.ResetContent();
	char str[7];
	
	for(int i=57;i>=0;i--)
	{
		sprintf(str,"%.2f",g_fH[i]);
		m_wndToolBar.m_high.AddString(str);
	}
	int nCount=m_wndToolBar.m_high.GetCount()-1; 
	if(fxinfo.nHigh>nCount)
		fxinfo.nHigh=0;
	m_wndToolBar.m_high.SetCurSel(fxinfo.nHigh);
}
//
//����ʱ����Ͽ�
void CMainFrame::mySetTime()
{
	m_wndToolBar.m_time.ResetContent();
	if(mrdata.GetCount()>0)
	{
    	if(fxinfo.nFX >10)//��վ����
		{
	    	mySetTime2();
		}
		else//��վ����
		{
			int nCount=mrdata.GetAt(0)->Ys.GetCount()-1; 
			for(int i=nCount;i>=0;i--)
			{
				LPSTR str=mrdata.GetAt(0)->Ys.GetAt(i)->sTime;
				m_wndToolBar.m_time.AddString(str);
			}
		}
		fxinfo.nTime=0;//��λ������ʱ��
	}
	m_wndToolBar.m_time.SetCurSel(fxinfo.nTime);
}
void CMainFrame::myAddTime2(LPSTR sTMax,LPSTR sTMin)
{
	char sT[9];
	for(int h=0;h<24;h++)
	{
		for(int m=0;m<60;m+=5)
		{
			wsprintf(sT,"%02d:%02d:00",h,m);
			if(lstrcmp(sT,sTMin)>0)
    			m_wndToolBar.m_time.InsertString(0,sT);
			if(lstrcmp(sT,sTMax)>0)
				return;
		}
	}
}
void CMainFrame::mySetTime2()
{
	//��������վ���е�����ʱ��
	char sTMax[9],sTMin[9];
	lstrcpy(sTMax,"00:00:00");
	lstrcpy(sTMin,"24:00:00");
	for(int i=0;i<mrdata.GetCount();i++)
	{
		PMRDATA pmr=mrdata.GetAt(i);
		int ncount= pmr->Ys.GetCount(); 
		if(ncount>0)
		{
    		PMRYS pys= pmr->Ys.GetAt(ncount-1); //���һ��ʱ��
	     	if( lstrcmp(pys->sTime,sTMax)>0)
		    	lstrcpy(sTMax,pys->sTime);
			pys=pmr->Ys.GetAt(0);//��һ��ʱ��
			if( lstrcmp(pys->sTime,sTMin)<0)
				lstrcpy(sTMin,pys->sTime);
		}
	}
	myAddTime2(sTMax,sTMin);
}

bool CMainFrame::myGetCurTime(LPSTR sT)
{
	if(fxinfo.nTime<m_wndToolBar.m_time.GetCount())
	{
		m_wndToolBar.m_time.GetLBText(fxinfo.nTime,sT);
		return true;
	}
	return false;
}


//������-ʱ��ѡ��ı�
void CMainFrame::OnCbnSelchangeTime()
{
	fxinfo.nTime =m_wndToolBar.m_time.GetCurSel();
	myOnFxinfoChange();
}
//������-ʱ����ǰ
void CMainFrame::OnTimeLeft()
{
	int nCount=m_wndToolBar.m_time.GetCount()-1; 
	if(fxinfo.nTime<nCount)
	{
		fxinfo.nTime++;
		m_wndToolBar.m_time.SetCurSel(fxinfo.nTime);
		myOnFxinfoChange();
	}
}
//������-ʱ�����
void CMainFrame::OnTimeRight()
{
	if(fxinfo.nTime >0)
	{
    	fxinfo.nTime --;
		m_wndToolBar.m_time.SetCurSel(fxinfo.nTime);
		myOnFxinfoChange();
	}
}
//������-�߶�ѡ��ı�
void CMainFrame::OnCbnSelchangeHigh()
{
	fxinfo.nHigh =m_wndToolBar.m_high.GetCurSel();
	myOnFxinfoChange();
}
//������-�߶�����
void CMainFrame::OnHighDown()
{
	int nCount=m_wndToolBar.m_high.GetCount()-1;// 
	if(fxinfo.nHigh<nCount)
	{
		fxinfo.nHigh++;
		m_wndToolBar.m_high.SetCurSel(fxinfo.nHigh);
		myOnFxinfoChange();
	}
}
//������-�߶�����
void CMainFrame::OnHighUp()
{
	if(fxinfo.nHigh >0)
	{
    	fxinfo.nHigh --;
		m_wndToolBar.m_high.SetCurSel(fxinfo.nHigh);
		myOnFxinfoChange();
	}
}

//������-���ݴ������ı�
void CMainFrame::OnCbnSelchangeDatatype()
{
	fxinfo.nProcessor =m_wndToolBar.m_datatype.GetCurSel();
    myOnFxinfoChange(); 
}

//�˵�-���ò�վ��Ϣ
void CMainFrame::OnMysetTz()
{
	CDlgSetTZ dlg;
	if( dlg.DoModal()== IDOK)//��վ��Ϣ�б�
	{
		myReadTZInfo(&list_tz,gszpath);
		m_wndDlgBar.dlgDz.mySetStation();	
	}
}
//�˵�-�������ݷ�������
void CMainFrame::OnMysetSjfx()
{
	CDlgSetSjfx dlg;
	dlg.DoModal();
}


//������-״̬�仯
void CMainFrame::OnUpdateTypeBox(CCmdUI *pCmdUI)
{
   	pCmdUI->Enable(mrdata.GetCount()>0);
}
void CMainFrame::OnUpdateTimeBox(CCmdUI *pCmdUI)
{
   	pCmdUI->Enable(mrdata.GetCount()>0 && ( fxinfo.nFX==1 || fxinfo.nFX==11));
}
void CMainFrame::OnUpdateHighBox(CCmdUI *pCmdUI)
{
   	pCmdUI->Enable(mrdata.GetCount()>0 && (fxinfo.nFX==2));
}

//�Զ����¶�ʱ��
void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==1)
	{
		if( fxinfo.nFX==2 && fxinfo.nStation >=0 && fxinfo.nStation < list_tz.GetCount())
		{
        	PTZINFO ptz=list_tz.GetAt(fxinfo.nStation);
			char sY[7],sM[5];
			CmySearchCsv::mySearchNewNian(fxparam.sdpath,sY); //�������µ���
         	CmySearchCsv::mySearchNewYue(fxparam.sdpath,sY,sM); //�������µ���
      		if(CmySearchCsv::mySearchNewFile(fxparam.sdpath,sY,sM,ptz->zm,ptz->zh,&fxinfo)) //�������ļ���Ч
			{
				m_wndDlgBar.dlgDz.mySearchNianYue(); 
				//�������ݼ���ʾ
         		myUpdateData();
			}
		}		
	}
	CFrameWnd::OnTimer(nIDEvent);
}

//�Զ�����
void CMainFrame::OnViewAutoUpdate()
{
	bAutoUpdate = !bAutoUpdate;
	if(bAutoUpdate)
		SetTimer(1,fxparam.nTimerUpdate*60000,NULL);
	else
		KillTimer(1);
}
void CMainFrame::OnUpdateViewAutoUpdate(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(bAutoUpdate);
}
//��ȡ���ݲ����¹������ϸ߶Ⱥ�ʱ����Ͽ�
void CMainFrame::myUpdateData()
{
	myReadData(fxparam.sdpath,&fxinfo,&list_tz,&mrdata,&list_Processor);
	mySetProcessor();
	mySetHigh();
	mySetTime();
	//������ʾ
	myOnFxinfoChange();	
}
//������ʾ
void CMainFrame::myOnFxinfoChange()
{
    ((CrmdaView*)GetActiveView())->myOnDraw();
}
//�����ǰ��ͼ
void CMainFrame::myReleaseDraw()
{
	((CrmdaView*)GetActiveView())->myDelDraw();
	myReleaseData(&mrdata,&list_Processor);
	((CrmdaView*)GetActiveView())->myOnDraw();
}
//-------------------------------------
// �л���ʾ״̬
//-------------------------------------
// ��������
void CMainFrame::OnViewDlgbar()
{
	m_wndDlgBar.ShowWindow( (m_wndDlgBar.IsWindowVisible())==0);
	RecalcLayout();
}
void CMainFrame::OnUpdateViewDlgbar(CCmdUI *pCmdUI)
{
   	pCmdUI->SetCheck((m_wndDlgBar.IsWindowVisible())!=0);
}
