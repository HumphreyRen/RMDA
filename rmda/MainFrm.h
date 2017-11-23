
// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "CmyDlgBar.h"

class CmyToolBar : public CToolBar
{
public:
	CComboBox      m_datatype;
	CComboBox      m_time;
	CComboBox      m_high;
};

class CMainFrame : public CFrameWnd
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:

// 操作
public:
	void mySetProcessor();
	void mySetHigh();
	void mySetTime();
	void mySetTime2();
	void myAddTime2(LPSTR sTMax,LPSTR sTMin);
 
	bool myGetCurTime(LPSTR sT);
	void myOnFxinfoChange();
	void myUpdateData();
	void myReleaseDraw();
// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

//protected:  // 控件条嵌入成员
	CmyToolBar        m_wndToolBar;
	CStatusBar        m_wndStatusBar;
	CmyDlgBar         m_wndDlgBar;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTimeLeft();
	afx_msg void OnTimeRight();
	afx_msg void OnHighUp();
	afx_msg void OnHighDown();
	afx_msg void OnCbnSelchangeTime();	
	afx_msg void OnCbnSelchangeHigh();
	afx_msg void OnCbnSelchangeDatatype();	

	afx_msg void OnUpdateTypeBox(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTimeBox(CCmdUI *pCmdUI);
	afx_msg void OnUpdateHighBox(CCmdUI *pCmdUI);

	afx_msg void OnMysetTz();
	afx_msg void OnMysetSjfx();	

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnViewAutoUpdate();
	afx_msg void OnUpdateViewAutoUpdate(CCmdUI *pCmdUI);
	afx_msg void OnViewDlgbar();
	afx_msg void OnUpdateViewDlgbar(CCmdUI *pCmdUI);	
};


