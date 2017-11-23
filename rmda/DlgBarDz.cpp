// DlgBarDz.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "rmda.h"
#include "MainFrm.h"
#include "DlgBarDz.h"
#include "afxdialogex.h"

#include "..\\common\\common.h"
#include "..\\csvRead\\mySearchCsv.h"
#include "..\\csvRead\\myReadCsv.h"
#include "..\\csvRead\\myReadHsCsv.h"

extern FXPARAM fxparam;
extern FXINFO fxinfo;
extern CArray<PTZINFO,PTZINFO> list_tz;
extern CArray<PHSTATUS,PHSTATUS> mrhs; //�豸����״̬
CArray<PNIANYUE,PNIANYUE> nianyue;
// CDlgBarDz �Ի���

IMPLEMENT_DYNAMIC(CDlgBarDz, CDialogEx)

CDlgBarDz::CDlgBarDz(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgBarDz::IDD, pParent)
{
	m_bFinishCreate=false;
}

CDlgBarDz::~CDlgBarDz()
{
	CmySearchCsv::myReleaseNianYue(&nianyue);
}

void CDlgBarDz::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_STATION, m_station);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_TREE1, m_tree);
}


BEGIN_MESSAGE_MAP(CDlgBarDz, CDialogEx)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_COMBO_STATION, &CDlgBarDz::OnCbnSelchangeComboStation)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CDlgBarDz::OnTvnSelchangedTree1)
	ON_LBN_SELCHANGE(IDC_LIST1, &CDlgBarDz::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_RADIO1, &CDlgBarDz::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CDlgBarDz::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CDlgBarDz::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CDlgBarDz::OnBnClickedRadio4)
END_MESSAGE_MAP()


// CDlgBarDz ��Ϣ�������
void CDlgBarDz::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if(m_bFinishCreate)
	{
    	CRect rc;
        GetClientRect(&rc);
		GetDlgItem(IDC_LIST1)->MoveWindow(10,320,rc.right-20,rc.bottom - 330);
	}
}
BOOL CDlgBarDz::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	myInitState(0);
	m_bFinishCreate=true;
	return TRUE;  
}
//�л���վ���վʱ�������ݷ�����ť״̬ 
void CDlgBarDz::myInitState(int nSel)
{
	if(nSel==0)//��վ
	{
    	fxinfo.nFX=1;
    	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_RADIO1))->SetWindowText("��վ��Ҫ������ͼ");
    	((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(0);
    	((CButton *)GetDlgItem(IDC_RADIO3))->SetCheck(0);
    	((CButton *)GetDlgItem(IDC_RADIO4))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_RADIO2))->ShowWindow(1);
		((CButton *)GetDlgItem(IDC_RADIO3))->ShowWindow(1);
		((CButton *)GetDlgItem(IDC_RADIO4))->ShowWindow(1);
	}
	else
	{
		fxinfo.nFX=11;
		((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_RADIO1))->SetWindowText("��վ��Ҫ�ضԱ�ͼ");
		((CButton *)GetDlgItem(IDC_RADIO2))->ShowWindow(0);
		((CButton *)GetDlgItem(IDC_RADIO3))->ShowWindow(0);
		((CButton *)GetDlgItem(IDC_RADIO4))->ShowWindow(0);
	}
	m_list.ResetContent();
	mySetStation();
	mySearchNianYue();
}
//���ò�վ��Ͽ�
void CDlgBarDz::mySetStation()
{
	m_station.ResetContent();
	if(fxinfo.nFX >10)
	{
		((CEdit *)GetDlgItem(IDC_STATIC_STATION))->SetWindowText("Ҫ��");
		//����Ҫ����Ͽ�
   		m_station.AddString("�¶�");
   		m_station.AddString("���ʪ��");
   		m_station.AddString("ˮ���ܶ�");
   		m_station.AddString("Һˮ����");
		m_station.SetCurSel(fxinfo.nYs);
	}
	else
	{
		((CEdit *)GetDlgItem(IDC_STATIC_STATION))->SetWindowText("̨վ");
		for(int i=0;i<list_tz.GetCount();i++)
		{
			PTZINFO pdata=list_tz.GetAt(i);
			m_station.AddString(pdata->zm);
		}
		if(fxinfo.nStation>(list_tz.GetCount()-1))
			fxinfo.nStation=0;
		m_station.SetCurSel(fxinfo.nStation);
	}
}
//�������ݸ�Ŀ¼����������,����m_tree���ο�
void CDlgBarDz::mySearchNianYue()
{
	m_tree.DeleteAllItems();
	CmySearchCsv::mySearchNian(fxparam.sdpath,&nianyue);
	for(int i=0;i<nianyue.GetCount();i++)
	{
		PNIANYUE pdata= nianyue.GetAt(i);
	    HTREEITEM ht1=m_tree.InsertItem(pdata->nian);
		for(int j=0;j<pdata->yue.GetCount();j++)
		{
			int lparam=MAKELONG(i+1,j);
           	m_tree.InsertItem(TVIF_TEXT|TVIF_PARAM,pdata->yue.GetAt(j),0,0,0,0,lparam,ht1,TVI_LAST);
		}
	}
	if(nianyue.GetCount()>0)
	{
		HTREEITEM htroot= m_tree.GetRootItem();
     	m_tree.Expand(htroot,TVE_EXPAND);
		HTREEITEM ht1 = m_tree.GetChildItem(htroot); 
		m_tree.SelectItem(ht1);
		m_tree.SetFocus();
	}
}
//����ѡ��ı�
void CDlgBarDz::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int n= pNMTreeView->itemNew.lParam;
	if(n>0 )
	{
		PNIANYUE pnian= nianyue.GetAt(LOWORD(n)-1);
		fxinfo.nYear = myGetStringsDigtal(pnian->nian);
		fxinfo.nMonth = myGetStringsDigtal(pnian->yue.GetAt(HIWORD(n)));
		mySearchFileDate();
	}
	*pResult = 0;
} 
//����ָ�����²�վ�������ļ�����,����m_list�б��
void CDlgBarDz::mySearchFileDate()
{
	m_list.ResetContent();
	CArray<CString,CString&> list_fn;
	char sSearchPath[MAX_PATH];
	if( fxinfo.nFX <10) //��վ
	{
    	if( fxinfo.nStation >=0 && fxinfo.nStation < list_tz.GetCount())
	    {
        	PTZINFO ptz=list_tz.GetAt(fxinfo.nStation);
 	    	wsprintf(sSearchPath,"%s\\%d��\\%02d��\\%s%s",fxparam.sdpath,fxinfo.nYear,fxinfo.nMonth,ptz->zm,ptz->zh);
        	CmySearchCsv::mySearchCsvFileDateByStation(sSearchPath,&list_fn);
     	}
	}
	else//��վ
	{
		for(int i=0;i<list_tz.GetCount();i++) 
	    {
        	PTZINFO ptz=list_tz.GetAt(i);
 	    	wsprintf(sSearchPath,"%s\\%d��\\%02d��\\%s%s",fxparam.sdpath,fxinfo.nYear,fxinfo.nMonth,ptz->zm,ptz->zh);
        	CmySearchCsv::mySearchCsvFileDateByStation(sSearchPath,&list_fn);
    	}
	}
	//���б��е��������ڼ��뵽list1�б����
    for(int i=0;i<list_fn.GetCount();i++)
    {
		m_list.AddString(list_fn.GetAt(i)); 
	}
   	list_fn.RemoveAll();
	if(fxinfo.nFX==4) 
    	myUpdateHealthStatus();
}

//��վѡ��ı�
void CDlgBarDz::OnCbnSelchangeComboStation()
{
	if(fxinfo.nFX >10) //��վҪ��ѡ��ı�
	{
		fxinfo.nYs =m_station.GetCurSel();
		((CMainFrame*)AfxGetMainWnd())->myOnFxinfoChange();
	}
	else//��վ��վѡ��ı�
	{
		((CMainFrame*)AfxGetMainWnd())->myReleaseDraw();
		fxinfo.nStation =m_station.GetCurSel();
		mySearchFileDate();
	}
}
//����ѡ��ı�
void CDlgBarDz::OnLbnSelchangeList1()
{
	int nSel=m_list.GetCurSel(); 
	char sDate[11];
	m_list.GetText(nSel,sDate); 
	char sY[5],sM[3],sD[3];
	sscanf(sDate,"%[^-]-%[^-]-%s",sY,sM,sD);
	fxinfo.nDay=atoi(sD);
	((CMainFrame*)AfxGetMainWnd())->myUpdateData();  
}

//��վ��Ҫ������ͼ
void CDlgBarDz::OnBnClickedRadio1()
{
	if( fxinfo.nFX <10)
    	fxinfo.nFX=1; 
	((CMainFrame*)AfxGetMainWnd())->myOnFxinfoChange(); 
}

//��վ��Ҫ��ʱ�շֲ�ͼ
void CDlgBarDz::OnBnClickedRadio2()
{
    fxinfo.nFX=2; 
	((CMainFrame*)AfxGetMainWnd())->myOnFxinfoChange();
}

//��վ��Ҫ��ʱ������ͼ
void CDlgBarDz::OnBnClickedRadio3()
{
	fxinfo.nFX=3; 
	((CMainFrame*)AfxGetMainWnd())->myOnFxinfoChange();
}

//��վ�豸����״̬ͼ
void CDlgBarDz::OnBnClickedRadio4()
{
	fxinfo.nFX=4; 
	myUpdateHealthStatus();
}
//���µ�ǰѡ��̨վ�豸״̬ͼ
void CDlgBarDz::myUpdateHealthStatus()
{
	if( fxinfo.nStation >=0 && fxinfo.nStation < list_tz.GetCount())
    {
       	PTZINFO ptz=list_tz.GetAt(fxinfo.nStation);
       	CmyReadHsCsv myReadHs;
       	myReadHs.myReadHsCsvFile(fxparam.sdpath,&fxinfo,ptz,&mrhs);
	}
	((CMainFrame*)AfxGetMainWnd())->myOnFxinfoChange();
}