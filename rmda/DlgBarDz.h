#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlgBarDz �Ի���

class CDlgBarDz : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgBarDz)

public:
	CDlgBarDz(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgBarDz();

	BOOL  m_bFinishCreate;

	void  myInitState(int nSel);
	void  mySetStation();
	void  mySearchNianYue();
	void  mySearchFileDate();
	void  myUpdateHealthStatus();
// �Ի�������
	enum { IDD = IDD_DIALOGBAR_DZ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
