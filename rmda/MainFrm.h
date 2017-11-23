
// MainFrm.h : CMainFrame ��Ľӿ�
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
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:

// ����
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
// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

//protected:  // �ؼ���Ƕ���Ա
	CmyToolBar        m_wndToolBar;
	CStatusBar        m_wndStatusBar;
	CmyDlgBar         m_wndDlgBar;

// ���ɵ���Ϣӳ�亯��
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


