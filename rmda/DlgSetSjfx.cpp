// DlgSetSjfx.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "rmda.h"
#include "DlgSetSjfx.h"
#include "afxdialogex.h"
#include "..\\common\\common.h"

extern char gszpath[128];
extern FXPARAM fxparam;
 
// CDlgSetSjfx �Ի���

IMPLEMENT_DYNAMIC(CDlgSetSjfx, CDialogEx)

CDlgSetSjfx::CDlgSetSjfx(CWnd* pParent )
	: CDialogEx(CDlgSetSjfx::IDD, pParent)
{

}

CDlgSetSjfx::~CDlgSetSjfx()
{
}

void CDlgSetSjfx::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSetSjfx, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgSetSjfx::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgSetSjfx ��Ϣ�������


BOOL CDlgSetSjfx::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//���ñ༭���ı���������
	SendDlgItemMessage(IDC_EDIT1,EM_LIMITTEXT,128,0);    //���ݸ�Ŀ¼
	SendDlgItemMessage(IDC_EDIT2,EM_LIMITTEXT,4,0);     //�����Զ�����ʱ����(����)
	SendDlgItemMessage(IDC_EDIT3,EM_LIMITTEXT,4,0);     //��վ��Ҫ������ͼ������(����)
	SendDlgItemMessage(IDC_EDIT4,EM_LIMITTEXT,4,0);     //��վ��Ҫ��ʱ�շֲ�ͼ������(����)
	SendDlgItemMessage(IDC_EDIT5,EM_LIMITTEXT,4,0);     //����ͼ����
	SendDlgItemMessage(IDC_EDIT6,EM_LIMITTEXT,4,0);     //����ͼ��߶�
	//
	SetDlgItemText(IDC_EDIT1,fxparam.sdpath);
	char str[10];	
	sprintf(str,"%d",fxparam.nTimerUpdate);
	SetDlgItemText(IDC_EDIT2,str);
	sprintf(str,"%d",fxparam.nTimerUpdate1);
	SetDlgItemText(IDC_EDIT3,str);
	sprintf(str,"%d",fxparam.nTimerUpdate2);
	SetDlgItemText(IDC_EDIT4,str);
	sprintf(str,"%d",fxparam.nSaveW);
	SetDlgItemText(IDC_EDIT5,str);
	sprintf(str,"%d",fxparam.nSaveH);
	SetDlgItemText(IDC_EDIT6,str);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgSetSjfx::OnBnClickedOk()
{
	char str[10];	
	//
	GetDlgItemText(IDC_EDIT1,fxparam.sdpath,128);
	//
	GetDlgItemText(IDC_EDIT2,str,10);
	fxparam.nTimerUpdate=atoi(str);
	if(fxparam.nTimerUpdate<1)
		fxparam.nTimerUpdate=2;
	GetDlgItemText(IDC_EDIT3,str,10);
	fxparam.nTimerUpdate1=atoi(str);
	if(fxparam.nTimerUpdate1<1)
		fxparam.nTimerUpdate1=60;
	GetDlgItemText(IDC_EDIT4,str,10);
	fxparam.nTimerUpdate2=atoi(str);
	if(fxparam.nTimerUpdate2<1)
		fxparam.nTimerUpdate2=2;
	GetDlgItemText(IDC_EDIT5,str,10);
	fxparam.nSaveW=atoi(str);
	if(fxparam.nSaveW<1)
		fxparam.nSaveW=1280;
	GetDlgItemText(IDC_EDIT6,str,10);
	fxparam.nSaveH=atoi(str);
	if(fxparam.nSaveH<1)
		fxparam.nSaveH=1024;
	mySaveSjfxParam(&fxparam,gszpath);
	CDialogEx::OnOK();
}
