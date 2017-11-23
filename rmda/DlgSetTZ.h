#pragma once
#include "afxwin.h"

#include "..\common\common.h"
// CDlgSetTZ 对话框

class CDlgSetTZ : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetTZ)

public:
	CDlgSetTZ(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSetTZ();

	CArray<PTZINFO,PTZINFO> m_list_tz;
	int m_curSel;//当前选择台站序号
	
	void mySetListBox();
	void mySetEditBox();
// 对话框数据
	enum { IDD = IDD_DIALOG_SET_TZ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_list1;
	afx_msg void OnLbnSelchangeList1();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonMod();
};
