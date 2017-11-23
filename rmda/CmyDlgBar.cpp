// CmyDlgBar.cpp : 实现文件
//

#include "stdafx.h"
#include "rmda.h"
#include "MainFrm.h"
#include "CmyDlgBar.h"

// CmyDlgBar 对话框
IMPLEMENT_DYNAMIC(CmyDlgBar, CDialogBar)

CmyDlgBar::CmyDlgBar() : CDialogBar()
{
	m_pTab=NULL;
	m_bFinishCreate=false;
}

CmyDlgBar::~CmyDlgBar()
{
}

void CmyDlgBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CmyDlgBar, CDialogBar)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CmyDlgBar::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CmyDlgBar 消息处理程序
BOOL CmyDlgBar::Create(CWnd *pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID, BOOL bChange)
{
	if(!CDialogBar::Create(pParentWnd,nIDTemplate,nStyle,nID))
		return false;
	
	m_pTab=(CTabCtrl *)GetDlgItem(IDC_TAB1);
	m_pTab->InsertItem(0,"单站");
	m_pTab->InsertItem(1,"多站");
	
	dlgDz.Create(IDD_DIALOGBAR_DZ,this);	
	dlgDz.ShowWindow(1);

	m_bFinishCreate=true;
	return true;
}

void CmyDlgBar::OnSize(UINT nType, int cx, int cy)
{
	CDialogBar::OnSize(nType, cx, cy);
 
	if(m_bFinishCreate)
	{
    	CRect rc;
  		GetClientRect(&rc);		
		dlgDz.MoveWindow(0,24,rc.right,rc.bottom-25);		
	}
}

//切换单站与多站
void CmyDlgBar::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	CTabCtrl * pTab=(CTabCtrl *)GetDlgItem(IDC_TAB1);
	dlgDz.myInitState(pTab->GetCurSel());	
	((CMainFrame*)AfxGetMainWnd())->myReleaseDraw();
	*pResult = 0;
}
