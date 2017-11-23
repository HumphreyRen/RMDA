
// outimgDlg.h : 头文件
//

#pragma once

#include "..\\common\\common.h"

// CoutimgDlg 对话框
class CoutimgDlg : public CDialogEx
{
// 构造
public:
	CoutimgDlg(CWnd* pParent = NULL);	// 标准构造函数

	ULONG_PTR   gdiplusToken;
	void       PostNcDestroy();
// 对话框数据
	enum { IDD = IDD_OUTIMG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	
	void myDrawCsvImg(PTZINFO ptz);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
