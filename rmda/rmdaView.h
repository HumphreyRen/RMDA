
// rmdaView.h : CrmdaView ��Ľӿ�
//

#pragma once
#include <gdiplus.h>
using namespace Gdiplus;

class CrmdaView : public CView
{
protected: // �������л�����
	CrmdaView();
	DECLARE_DYNCREATE(CrmdaView)

// ����
public:
	CrmdaDoc* GetDocument() const;

	ULONG_PTR   gdiplusToken;
// ����
public:
	void myOnDraw();         //����ˢ��
	void myDelDraw();
	void myDrawSJFX();
// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
protected:

// ʵ��
public:
	virtual ~CrmdaView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnFileSaveAsImg();
};

#ifndef _DEBUG  // rmdaView.cpp �еĵ��԰汾
inline CrmdaDoc* CrmdaView::GetDocument() const
   { return reinterpret_cast<CrmdaDoc*>(m_pDocument); }
#endif

