#pragma once


// CDlgSetSjfx �Ի���

class CDlgSetSjfx : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetSjfx)

public:
	CDlgSetSjfx(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSetSjfx();

// �Ի�������
	enum { IDD = IDD_DIALOG_SET_SJFX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
