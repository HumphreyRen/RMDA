#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlgBarDz 对话框

class CDlgBarDz : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgBarDz)

public:
	CDlgBarDz(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgBarDz();

	BOOL  m_bFinishCreate;

	void  myInitState(int nSel);
	void  mySetStation();
	void  mySearchNianYue();
	void  mySearchFileDate();
	void  myUpdateHealthStatus();
// 对话框数据
	enum { IDD = IDD_DIALOGBAR_DZ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CComboBox m_station;
	CTreeCtrl m_tree;
	CListBox m_list;
	afx_msg void OnCbnSelchangeComboStation();
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();

};
