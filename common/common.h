#pragma once

#include "..\lib\lkh.h"
#include "myStruct.h"

#include "gl\gl.h"
#include "gl\glu.h"

//
// ̨վ��Ϣ
//
void myReadTZInfo(CArray<PTZINFO,PTZINFO> *plist,LPSTR spath);
void myReleaseTZInfo(CArray<PTZINFO,PTZINFO> *plist);
void mySaveTZInfo(CArray<PTZINFO,PTZINFO> *plist,LPSTR spath);
//
// ���ݷ�������
//
void myReadSjfxParam(PFXPARAM pfxparam,LPSTR spath);
void mySaveSjfxParam(PFXPARAM pfxparam,LPSTR spath);

//
class CmyDC
{
public:
	CmyDC(int w,int h);
	virtual ~CmyDC();

	void  mySetCurrent();

	HDC         m_hdc;
	HBITMAP     m_hbmp;	
	HGLRC       m_hglrc;
	int         m_dcW,m_dcH;
private:
	void myInitDC();
	void myReleaseDC();
	bool myInitGL(int w,int h);
    void myExitGL();
};