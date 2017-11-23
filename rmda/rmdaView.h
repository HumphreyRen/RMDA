
// rmdaView.h : CrmdaView 类的接口
//

#pragma once
#include <gdiplus.h>
using namespace Gdiplus;

class CrmdaView : public CView
{
protected: // 仅从序列化创建
	CrmdaView();
	DECLARE_DYNCREATE(CrmdaView)

// 特性
public:
	CrmdaDoc* GetDocument() const;

	ULONG_PTR   gdiplusToken;
// 操作
public:
	void myOnDraw();         //窗口刷新
	void myDelDraw();
	void myDrawSJFX();
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
protected:

// 实现
public:
	virtual ~CrmdaView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnFileSaveAsImg();
};

#ifndef _DEBUG  // rmdaView.cpp 中的调试版本
inline CrmdaDoc* CrmdaView::GetDocument() const
   { return reinterpret_cast<CrmdaDoc*>(m_pDocument); }
#endif

