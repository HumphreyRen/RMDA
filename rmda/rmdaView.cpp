
// rmdaView.cpp : CrmdaView 类的实现
//
#include "stdafx.h"
#ifndef SHARED_HANDLERS
#include "rmda.h"
#endif
#include "MainFrm.h"
#include "rmdaDoc.h"
#include "rmdaView.h"

#include "..\\common\\common.h"
#include "..\\csvDraw\myDrawCsv.h"

#pragma comment(lib,"gdiplus")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern FXINFO fxinfo;
extern CArray<PTZINFO,PTZINFO> list_tz;
extern CArray<CString,CString&> list_Processor;//处理器列表
extern CArray<PMRDATA,PMRDATA> mrdata;//多站观测数据
extern CArray<PHSTATUS,PHSTATUS> mrhs; //设备运行状态
extern char gszpath[128];
extern float g_fH[58];//观测高度,km

CmyDC *gpDC;
CDrawCsv *gpDraw;
int gww,gwh;
 
// CrmdaView

IMPLEMENT_DYNCREATE(CrmdaView, CView)

BEGIN_MESSAGE_MAP(CrmdaView, CView)
	ON_WM_SIZE()	
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_FILE_SAVE_AS_IMG, &CrmdaView::OnFileSaveAsImg)
END_MESSAGE_MAP()

// CrmdaView 构造/析构

CrmdaView::CrmdaView()
{
	GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	gpDC= new CmyDC(GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
	gpDraw=NULL;
}

CrmdaView::~CrmdaView()
{
	myDelDraw();
	delete gpDC;
	GdiplusShutdown(gdiplusToken);
}
// CrmdaView 绘制
void CrmdaView::OnDraw(CDC* pDC)
{
	if(gpDC->m_hdc !=NULL)
	{
		BitBlt(pDC->m_hDC, 0,0,gww,gwh,gpDC->m_hdc,0,0,SRCCOPY);
	}
}
void CrmdaView::myOnDraw()
{
	if(gpDC->m_hdc !=NULL)
	{
		myDrawSJFX();
		CDC* pDC=this->GetDC();
		if(pDC!=NULL)
		{
       		BitBlt(pDC->m_hDC, 0,0,gww,gwh,gpDC->m_hdc,0,0,SRCCOPY);
			ReleaseDC(pDC); 
		}
	}
}
void CrmdaView::myDelDraw()
{
    if(gpDraw!=NULL)
	{
		delete gpDraw;
		gpDraw=NULL;
	}
}
void CrmdaView::myDrawSJFX()
{
	myDelDraw();	
	Graphics gp(gpDC->m_hdc);
    gp.SetSmoothingMode(SmoothingModeHighQuality);
    gp.SetTextRenderingHint(TextRenderingHintAntiAlias);
    gp.FillRectangle(&SolidBrush(Color(255,255,255,255)),-1,-1,gww+1,gwh+1);
	if(fxinfo.nFX ==4)//设备运行状态
	{
		gpDraw=new CmyDrawCsvHs();
		((CmyDrawCsvHs *)gpDraw)->myDrawHs(&gp,gww,gwh,&mrhs,list_tz.GetAt(fxinfo.nStation)); 
		return;
	}
	if( list_Processor.GetCount()> fxinfo.nProcessor)	
	{
		//计算与当前选择的处理器对应的各站处理器序号
		for(int i=0;i<mrdata.GetCount();i++)
		{
			PMRDATA pdata=mrdata.GetAt(i);
			pdata->nProcessor=-1;
			for(int j=0;j<4;j++)
			{
				if( lstrcmp(pdata->sProcessors[j],list_Processor.GetAt(fxinfo.nProcessor))==0)//相同处理器类型
				{
					pdata->nProcessor=j;
					break;
				}
			}
		}
		SetCursor(LoadCursor(NULL, IDC_WAIT));
		switch(fxinfo.nFX)
		{
		case 1://单站多要素廓线图
			gpDraw=new CmyDrawCsvProfile();
			((CmyDrawCsvProfile *)gpDraw)->myDrawProfile(&gp,gww,gwh,mrdata.GetAt(0),g_fH,&fxinfo,list_tz.GetAt(fxinfo.nStation));
			break;
		case 2://单站多要素时空分布图
			gpDraw=new CmyDrawCsvEvolve();
			((CmyDrawCsvEvolve *)gpDraw)->myDrawEvolve(&gp,gww,gwh,&mrdata,g_fH,&fxinfo,list_tz.GetAt(fxinfo.nStation));
			break;
		case 3://单站各要素时间曲线图
			gpDraw=new CmyDrawCsvEvolve2();
			((CmyDrawCsvEvolve2 *)gpDraw)->myDrawEvolve2(&gp,gww,gwh,&mrdata,list_tz.GetAt(fxinfo.nStation));
			break;
		case 11://多站单要素对比图
			gpDraw=new CmyDrawCsvProfile2();			
            char sTime0[9]; //当前选择时次	
            if(	((CMainFrame*)AfxGetMainWnd())->myGetCurTime(sTime0) )//判断当前选择时次是否有效
			{
	    		((CmyDrawCsvProfile2 *)gpDraw)->myDrawProfile2(&gp,gww,gwh,&mrdata,g_fH,&fxinfo,&list_tz,sTime0);
			}
			break;
		default:
			break;
		}
		SetCursor(LoadCursor(NULL, IDC_ARROW));
	}		
}
// CrmdaView 诊断

#ifdef _DEBUG
void CrmdaView::AssertValid() const
{
	CView::AssertValid();
}

void CrmdaView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CrmdaDoc* CrmdaView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CrmdaDoc)));
	return (CrmdaDoc*)m_pDocument;
}
#endif //_DEBUG


// CrmdaView 消息处理程序
//窗体大小改变响应
void CrmdaView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
    RECT rc;
    GetClientRect(&rc);
	gww=rc.right -rc.left;
	gwh=rc.bottom-rc.top;    
	if(gww<500)
		gww=500;
	if(gwh<400)
		gwh=400;
	myOnDraw();
}

//鼠标移动响应
void CrmdaView::OnMouseMove(UINT nFlags, CPoint point)
{
	char str[260];
	lstrcpy(str,"");
	if(gpDraw!=NULL)
	{
		gpDraw->myGetDiagramData(point.x,point.y,str);
	}
	CStatusBar* pStatus=&((CMainFrame*)AfxGetMainWnd())->m_wndStatusBar;
	if(pStatus)
	{
		pStatus->SetPaneText(1,str);
	}
	CView::OnMouseMove(nFlags, point);
}

//菜单命令--另存为图像
void CrmdaView::OnFileSaveAsImg()
{
	if(gpDC->m_hdc!=NULL)
	{
		OPENFILENAME ofn;
		char  szFile[260],szPath[260];

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.lpstrFile=szFile;
		lstrcpy(ofn.lpstrFile,".PNG"); 
		ofn.nMaxFile = 260;
		ofn.lpstrFilter = "图像文件(*.PNG)\0*.PNG\0";
		ofn.nFilterIndex = 0;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		wsprintf(szPath,"%s\\IMG",gszpath);
		ofn.lpstrInitialDir = szPath;
		ofn.Flags = OFN_OVERWRITEPROMPT;
		if (GetSaveFileName(&ofn)==TRUE)
		{
     		SetCurrentDirectory(gszpath);
			mySavePicToFile(gpDC->m_hdc,gpDC->m_hbmp,gww,gwh,ofn.lpstrFile);
    		ShellExecute(NULL,"open",ofn.lpstrFile, NULL, NULL, SW_SHOWNORMAL);
		}
	}
}