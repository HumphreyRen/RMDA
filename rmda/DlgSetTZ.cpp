// DlgSetTZ.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "rmda.h"
#include "DlgSetTZ.h"
#include "afxdialogex.h"

extern char gszpath[128];

// CDlgSetTZ �Ի���

IMPLEMENT_DYNAMIC(CDlgSetTZ, CDialogEx)

CDlgSetTZ::CDlgSetTZ(CWnd* pParent )
	: CDialogEx(CDlgSetTZ::IDD, pParent)
{

}

CDlgSetTZ::~CDlgSetTZ()
{
	myReleaseTZInfo(&m_list_tz);
}

void CDlgSetTZ::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list1);
}


BEGIN_MESSAGE_MAP(CDlgSetTZ, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST1, &CDlgSetTZ::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDlgSetTZ::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CDlgSetTZ::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDOK, &CDlgSetTZ::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_MOD, &CDlgSetTZ::OnBnClickedButtonMod)
END_MESSAGE_MAP()


// CDlgSetTZ ��Ϣ�������

BOOL CDlgSetTZ::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//���ñ༭���ı���������
	SendDlgItemMessage(IDC_EDIT1,EM_LIMITTEXT,14,0);  //վ��
	SendDlgItemMessage(IDC_EDIT2,EM_LIMITTEXT,4,0);   //վ��
	//��ȡ��վ��Ϣ�ļ�
	myReadTZInfo(&m_list_tz,gszpath);
	m_curSel=-1;
	mySetListBox();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
//���Ӳ�վ
void CDlgSetTZ::OnBnClickedButtonAdd()
{
	char zm[15],zh[5];
	GetDlgItemText(IDC_EDIT1,zm,15);
	GetDlgItemText(IDC_EDIT2,zh,5);
	if(lstrlen(zm)>1 && lstrlen(zh)>3)
	{
     	PTZINFO ptz=new TZINFO;
    	ZeroMemory(ptz,sizeof(TZINFO));
    	lstrcpy(ptz->zm,zm);
		lstrcpy(ptz->zh,zh);
     	m_list_tz.Add(ptz);
    	m_curSel=m_list_tz.GetCount()-1; 
    	mySetListBox();
	}
}
//�޸�̨վ��Ϣ
void CDlgSetTZ::OnBnClickedButtonMod()
{
	char zm[15],zh[5];
	GetDlgItemText(IDC_EDIT1,zm,15);
	GetDlgItemText(IDC_EDIT2,zh,5);
	if(lstrlen(zm)>1 && lstrlen(zh)>3 && m_curSel>=0)
	{
		PTZINFO ptz=m_list_tz.GetAt(m_curSel);
		lstrcpy(ptz->zm,zm);
		lstrcpy(ptz->zh,zh);
		mySetListBox();
	}
}
//ɾ����վ
void CDlgSetTZ::OnBnClickedButtonDel()
{
	if(m_curSel>=0)
	{
		PTZINFO ptz=m_list_tz.GetAt(m_curSel);
		delete ptz;
		m_list_tz.RemoveAt(m_curSel);
		m_curSel=-1;
		mySetListBox();
    	SetDlgItemText(IDC_EDIT1,"");
		SetDlgItemText(IDC_EDIT2,"");
	}
}

//��վѡ��ı�
void CDlgSetTZ::OnLbnSelchangeList1()
{
	m_curSel=m_list1.GetCurSel();
	mySetEditBox();
}
//����̨վ�б��
void CDlgSetTZ::mySetListBox()
{
	char str[20];
	m_list1.ResetContent();
	for(int i=0;i<m_list_tz.GetCount();i++)
	{
		PTZINFO ptz=m_list_tz.GetAt(i);
		wsprintf(str,"%s%s",ptz->zm,ptz->zh);
		m_list1.AddString(str);
	}
	m_list1.SetCurSel(m_curSel);
}
//���ò�վ��Ϣ�༭��
void CDlgSetTZ::mySetEditBox()
{
	if(m_curSel>=0)
	{
		PTZINFO ptz=m_list_tz.GetAt(m_curSel);
		SetDlgItemText(IDC_EDIT1,ptz->zm);
		SetDlgItemText(IDC_EDIT2,ptz->zh);
	}
}

void CDlgSetTZ::OnBnClickedOk()
{
	mySaveTZInfo(&m_list_tz,gszpath);
	CDialogEx::OnOK();
}


