
// outimgDlg.h : ͷ�ļ�
//

#pragma once

#include "..\\common\\common.h"

// CoutimgDlg �Ի���
class CoutimgDlg : public CDialogEx
{
// ����
public:
	CoutimgDlg(CWnd* pParent = NULL);	// ��׼���캯��

	ULONG_PTR   gdiplusToken;
	void       PostNcDestroy();
// �Ի�������
	enum { IDD = IDD_OUTIMG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	
	void myDrawCsvImg(PTZINFO ptz);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
    afx_msg LRESULT OnMyMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
