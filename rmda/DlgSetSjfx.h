#pragma once


// CDlgSetSjfx 对话框

class CDlgSetSjfx : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetSjfx)

public:
	CDlgSetSjfx(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSetSjfx();

// 对话框数据
	enum { IDD = IDD_DIALOG_SET_SJFX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
