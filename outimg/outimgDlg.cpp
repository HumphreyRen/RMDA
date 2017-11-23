
// outimgDlg.cpp : 实现文件
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
UINT uStopProcessMsg;    //通信消息 

FXPARAM fxparam;
FXINFO fxinfo;                   //
CArray<PTZINFO,PTZINFO> list_tz; //
CArray<PMRDATA,PMRDATA> mrdata;  //观测数据
CArray<CString,CString&> list_Processor;//处理器列表
float g_fH[58];    //观测高度,km
CmyDC *gpDC;

// CoutimgDlg 对话框
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


// CoutimgDlg 消息处理程序

BOOL CoutimgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//取程序运行路径
	GetCurrentDirectory(120,gszpath);
	uStopProcessMsg = RegisterWindowMessage("RMDA-OUTIMG.EXE"); //注册自定义消息   
	//
	myReadSjfxParam(&fxparam,gszpath);
	gpDC= new CmyDC(fxparam.nSaveW,fxparam.nSaveH);
	if( fxparam.nTimerUpdate1 >0)
    	SetTimer(1,fxparam.nTimerUpdate1*60000,NULL);
	if( fxparam.nTimerUpdate2 >0)
    	SetTimer(2,fxparam.nTimerUpdate2*60000,NULL);
	//
	CmyReadCsv::mySetFH(g_fH);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CoutimgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
	if(nIDEvent==1 || nIDEvent==2)//1-单站多要素廓线图,2-单站多要素时空分布图
	{
		//搜索数据根目录下的最新的年月
		char sY[7],sM[5];
		CmySearchCsv::mySearchNewNian(fxparam.sdpath,sY); //搜索最新的年
       	CmySearchCsv::mySearchNewYue(fxparam.sdpath,sY,sM); //搜索最新的月
		//
		fxinfo.nFX = nIDEvent;
		//
		myReadTZInfo(&list_tz,gszpath);
		//循环各站点
		for(int i=0;i<list_tz.GetCount();i++)
		{
			PTZINFO ptz=list_tz.GetAt(i);	
			fxinfo.nStation =i;
			if(CmySearchCsv::mySearchNewFile(fxparam.sdpath,sY,sM,ptz->zm,ptz->zh,&fxinfo)) //新数据文件有效
			{
				//读新数据文件
				myReadData(fxparam.sdpath,&fxinfo,&list_tz,&mrdata,&list_Processor);
				//绘制图形
				myDrawCsvImg(ptz);
			}			
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}
//后台绘制数据分析图形
void CoutimgDlg::myDrawCsvImg(PTZINFO ptz)
{	
	Graphics gp(gpDC->m_hdc);
    gp.SetSmoothingMode(SmoothingModeHighQuality);
    gp.SetTextRenderingHint(TextRenderingHintAntiAlias);
    gp.FillRectangle(&SolidBrush(Color(255,255,255,255)),-1,-1,fxparam.nSaveW+1,fxparam.nSaveH+1);
	//处理器序号设为0
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
    	case 1://单站多要素廓线图
			gpDraw=new CmyDrawCsvProfile();
			pdata=mrdata.GetAt(0);
			((CmyDrawCsvProfile *)gpDraw)->myDrawProfile(&gp,fxparam.nSaveW,fxparam.nSaveH,pdata,g_fH,&fxinfo,ptz);
			//文件名
			pYs=pdata->Ys.GetAt(pdata->Ys.GetCount()-1 - fxinfo.nTime);//最新时次
			sT=pYs->sTime;
			wsprintf(sFileName,"%s\\temp\\%s_%s_单站多要素廓线图_%s_%c%c%c%c.png",fxparam.sdpath,ptz->zm,ptz->zh,pYs->sDate,sT[0],sT[1],sT[3],sT[4]); 
			break;
		case 2: //单站多要素时空分布图   	
			gpDraw=new CmyDrawCsvEvolve();
			((CmyDrawCsvEvolve *)gpDraw)->myDrawEvolve(&gp,fxparam.nSaveW,fxparam.nSaveH,&mrdata,g_fH,&fxinfo,ptz);
			//文件名
			wsprintf(sFileName,"%s\\temp\\%s_%s_单站多要素时空分布图.png",fxparam.sdpath,ptz->zm,ptz->zh); 
			break;
		default:
			break;
	}
	//存图
	mySavePicToFile(gpDC->m_hdc,gpDC->m_hbmp,fxparam.nSaveW,fxparam.nSaveH,sFileName);
	//
	if(gpDraw!=NULL)
		delete gpDraw;
	
}