
// outimgDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "outimg.h"
#include "outimgDlg.h"
#include "afxdialogex.h"


#include "..\\csvRead\\mySearchCsv.h"
#include "..\\csvRead\\myReadCsv.h"
#include "..\\csvDraw\myDrawCsv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

char gszpath[128];
UINT uStopProcessMsg;    //ͨ����Ϣ 

FXPARAM fxparam;
FXINFO fxinfo;                   //
CArray<PTZINFO,PTZINFO> list_tz; //
CArray<PMRDATA,PMRDATA> mrdata;  //�۲�����
CArray<CString,CString&> list_Processor;//�������б�
float g_fH[58];    //�۲�߶�,km
CmyDC *gpDC;

// CoutimgDlg �Ի���
CoutimgDlg::CoutimgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CoutimgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

void CoutimgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CoutimgDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_WM_DESTROY()
	ON_REGISTERED_MESSAGE(uStopProcessMsg,OnMyMsg)
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CoutimgDlg ��Ϣ�������

BOOL CoutimgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//ȡ��������·��
	GetCurrentDirectory(120,gszpath);
	uStopProcessMsg = RegisterWindowMessage("RMDA-OUTIMG.EXE"); //ע���Զ�����Ϣ   
	//
	myReadSjfxParam(&fxparam,gszpath);
	gpDC= new CmyDC(fxparam.nSaveW,fxparam.nSaveH);
	if( fxparam.nTimerUpdate1 >0)
    	SetTimer(1,fxparam.nTimerUpdate1*60000,NULL);
	if( fxparam.nTimerUpdate2 >0)
    	SetTimer(2,fxparam.nTimerUpdate2*60000,NULL);
	//
	CmyReadCsv::mySetFH(g_fH);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CoutimgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CoutimgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CoutimgDlg::OnMyMsg(WPARAM wParam, LPARAM lParam)
{
	UINT uCode=(UINT)wParam;
	if(uCode == 0)
	{
		DestroyWindow();
	}
	return 0;
}
void CoutimgDlg::PostNcDestroy()
{
	CDialog::PostNcDestroy();	
	delete this;
}
void CoutimgDlg::OnClose()
{
	CDialog::OnClose();
	DestroyWindow();
}
void CoutimgDlg::OnDestroy()
{
	CDialog::OnDestroy();
	KillTimer(1);
	KillTimer(2);
	myReleaseData(&mrdata,&list_Processor);
	myReleaseTZInfo(&list_tz);
	delete gpDC;
	GdiplusShutdown(gdiplusToken);
}
//
void CoutimgDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==1 || nIDEvent==2)//1-��վ��Ҫ������ͼ,2-��վ��Ҫ��ʱ�շֲ�ͼ
	{
		//�������ݸ�Ŀ¼�µ����µ�����
		char sY[7],sM[5];
		CmySearchCsv::mySearchNewNian(fxparam.sdpath,sY); //�������µ���
       	CmySearchCsv::mySearchNewYue(fxparam.sdpath,sY,sM); //�������µ���
		//
		fxinfo.nFX = nIDEvent;
		//
		myReadTZInfo(&list_tz,gszpath);
		//ѭ����վ��
		for(int i=0;i<list_tz.GetCount();i++)
		{
			PTZINFO ptz=list_tz.GetAt(i);	
			fxinfo.nStation =i;
			if(CmySearchCsv::mySearchNewFile(fxparam.sdpath,sY,sM,ptz->zm,ptz->zh,&fxinfo)) //�������ļ���Ч
			{
				//���������ļ�
				myReadData(fxparam.sdpath,&fxinfo,&list_tz,&mrdata,&list_Processor);
				//����ͼ��
				myDrawCsvImg(ptz);
			}			
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}
//��̨�������ݷ���ͼ��
void CoutimgDlg::myDrawCsvImg(PTZINFO ptz)
{	
	Graphics gp(gpDC->m_hdc);
    gp.SetSmoothingMode(SmoothingModeHighQuality);
    gp.SetTextRenderingHint(TextRenderingHintAntiAlias);
    gp.FillRectangle(&SolidBrush(Color(255,255,255,255)),-1,-1,fxparam.nSaveW+1,fxparam.nSaveH+1);
	//�����������Ϊ0
	for(int i=0;i<mrdata.GetCount();i++)
	{
		PMRDATA pdata=mrdata.GetAt(i);
		pdata->nProcessor=0;
	}
    CDrawCsv *gpDraw=NULL;
	char sFileName[MAX_PATH];
	PMRDATA pdata;
	PMRYS pYs;
	LPSTR sT;
	switch(fxinfo.nFX)
	{
    	case 1://��վ��Ҫ������ͼ
			gpDraw=new CmyDrawCsvProfile();
			pdata=mrdata.GetAt(0);
			((CmyDrawCsvProfile *)gpDraw)->myDrawProfile(&gp,fxparam.nSaveW,fxparam.nSaveH,pdata,g_fH,&fxinfo,ptz);
			//�ļ���
			pYs=pdata->Ys.GetAt(pdata->Ys.GetCount()-1 - fxinfo.nTime);//����ʱ��
			sT=pYs->sTime;
			wsprintf(sFileName,"%s\\temp\\%s_%s_��վ��Ҫ������ͼ_%s_%c%c%c%c.png",fxparam.sdpath,ptz->zm,ptz->zh,pYs->sDate,sT[0],sT[1],sT[3],sT[4]); 
			break;
		case 2: //��վ��Ҫ��ʱ�շֲ�ͼ   	
			gpDraw=new CmyDrawCsvEvolve();
			((CmyDrawCsvEvolve *)gpDraw)->myDrawEvolve(&gp,fxparam.nSaveW,fxparam.nSaveH,&mrdata,g_fH,&fxinfo,ptz);
			//�ļ���
			wsprintf(sFileName,"%s\\temp\\%s_%s_��վ��Ҫ��ʱ�շֲ�ͼ.png",fxparam.sdpath,ptz->zm,ptz->zh); 
			break;
		default:
			break;
	}
	//��ͼ
	mySavePicToFile(gpDC->m_hdc,gpDC->m_hbmp,fxparam.nSaveW,fxparam.nSaveH,sFileName);
	//
	if(gpDraw!=NULL)
		delete gpDraw;
	
}