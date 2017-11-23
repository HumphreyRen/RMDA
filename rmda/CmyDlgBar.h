#pragma once

#include "DlgBarDz.h"
//#include "DlgBarMz.h"
// CmyDlgBar 对话框

class CmyDlgBar : public CDialogBar
{
	DECLARE_DYNAMIC(CmyDlgBar)

public:
	CmyDlgBar();   // 标准构造函数CWnd* pParent = NULL
	virtual ~CmyDlgBar();

// 对话框数据
	enum { IDD = IDD_DIALOGBAR };

	BOOL Create(CWnd* pParentWnd,UINT nIDTemplate,UINT nStyle,UINT nID,BOOL =TRUE);   
	BOOL  m_bFinishCreate;

	CTabCtrl  *m_pTab;
	CDlgBarDz  dlgDz;
	//CDlgBarMz  dlgMz;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};
