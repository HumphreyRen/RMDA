#include "stdafx.h"
#include "common.h"

#include "..\lib\lkxml.h"
#pragma comment(lib,"..\\lib\\LKXML17.lib")
#pragma comment(lib,"..\\lib\\LKH.lib")
#pragma comment(lib,"..\\Lib\\LKDzx.lib")
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
//----------------------------------------------------
//  ̨վ��Ϣ
//----------------------------------------------------
//��ȡ̨վ��Ϣ�ļ�
void myReadTZInfo(CArray<PTZINFO,PTZINFO> *plist,LPSTR spath)
{
	char fn[MAX_PATH];	
	long lnNode1,lnNode2;

	//�ͷ�������Դ
    myReleaseTZInfo(plist);
	//�����ļ���
	wsprintf(fn,"%s\\param\\rm_station.xml",spath);
	//
	CoInitialize(NULL); 
	MSXML2::IXMLDOMDocumentPtr XMLDOC;
	MSXML2::IXMLDOMElementPtr XMLROOT; 
	MSXML2::IXMLDOMNodeListPtr XMLNODES,XMLNODES2; //ĳ���ڵ�������ӽڵ�

	HRESULT HR = XMLDOC.CreateInstance(_uuidof(MSXML2::DOMDocument30)); 
	if(SUCCEEDED(HR) && XMLDOC->load(fn)!=0 )
	{
		XMLROOT = XMLDOC->GetdocumentElement();//��ø��ڵ�station_info;
		XMLROOT->get_childNodes(&XMLNODES);//��ø��ڵ�������ӽڵ�; 
		XMLNODES->get_length(&lnNode1);//��ø��ڵ�������ӽڵ�ĸ���; 
		//ѭ������վ��	
		for(int i=0;i<lnNode1;i++)
		{
			CMyXml3::myGetChildNodesByIndex(XMLNODES,i,&XMLNODES2);//��λ����i��station�ڵ�������ӽڵ�
			XMLNODES2->get_length(&lnNode2);//��i��station�ڵ���ӽڵ�ĸ���

            if(lnNode2>1)//station��������2���ӽڵ�
			{
				//�����µ�PTZINFO��
				PTZINFO pdata=new TZINFO;
				//վ��
				CMyXml3::myGetNodeTextByIndex(XMLNODES2,0,pdata->zm,15);
				//վ��
				CMyXml3::myGetNodeTextByIndex(XMLNODES2,1,pdata->zh,5);
				//
				plist->Add(pdata);
            }
		}//end for(i)	
		SAFERELEASEXML(XMLNODES2);
		SAFERELEASEXML(XMLNODES);
		SAFERELEASEXML(XMLROOT);
 	}//end if(pXml->myOpenDoc(fn))
	SAFERELEASEXML(XMLDOC);
	CoUninitialize();
}
//����̨վ��Ϣ
void mySaveTZInfo(CArray<PTZINFO,PTZINFO> *plist,LPSTR spath)
{
	char fn[MAX_PATH];
	PTZINFO pdata;	
	//
	CoInitialize(NULL); 
	MSXML2::IXMLDOMDocumentPtr XMLDOC;
	MSXML2::IXMLDOMElementPtr XMLROOT; 
	MSXML2::IXMLDOMElementPtr XMLNODE;

	HRESULT HR = XMLDOC.CreateInstance(_uuidof(MSXML2::DOMDocument30)); 
	if(SUCCEEDED(HR))
	{
		//��������ָ��
		CMyXml3::myCreateProcessingInstruction(XMLDOC);
		//�������ڵ�
		XMLROOT = XMLDOC->createElement("station_info"); 
		XMLDOC->appendChild(XMLROOT); 
   
		for(int i=0;i<plist->GetCount();i++)
		{
			pdata=plist->GetAt(i);
			//����station�ڵ�
			XMLNODE = XMLDOC->createElement("station"); 
            XMLROOT->appendChild(XMLNODE); 	
			//����station�ĸ��ӽڵ�
			CMyXml3::myCreateElement(XMLDOC,XMLNODE,"zm",pdata->zm);
			CMyXml3::myCreateElement(XMLDOC,XMLNODE,"zh",pdata->zh);  		
		}
		//д������ļ� 
     	wsprintf(fn,"%s\\param\\rm_station.xml",spath);
		XMLDOC->save(fn);
		//
		SAFERELEASEXML(XMLNODE); 
        SAFERELEASEXML(XMLROOT); 
	}
	SAFERELEASEXML(XMLDOC);
	CoUninitialize();
}
//�ͷ�̨վ��Ϣ�б�
void myReleaseTZInfo(CArray<PTZINFO,PTZINFO> *plist)
{
	for(int i=0;i<plist->GetCount();i++)
	{
		PTZINFO pdata=plist->GetAt(i);
        delete pdata;
	}
	plist->RemoveAll();
}

//----------------------------------------------------
//  ���ݷ�������
//----------------------------------------------------
// ��ȡ���ݷ�������
void myReadSjfxParam(PFXPARAM pfxparam,LPSTR spath)
{
	char fn[MAX_PATH],str[5];	
	//�����ļ���
	wsprintf(fn,"%s\\param\\param.xml",spath);
	//
	CoInitialize(NULL); 
	MSXML2::IXMLDOMDocumentPtr XMLDOC;
	MSXML2::IXMLDOMElementPtr XMLROOT; 
	MSXML2::IXMLDOMNodeListPtr XMLNODES;
	HRESULT HR = XMLDOC.CreateInstance(_uuidof(MSXML2::DOMDocument30)); 
	if(SUCCEEDED(HR) && XMLDOC->load(fn)!=0 )
	{
		XMLROOT = XMLDOC->GetdocumentElement();//��ø��ڵ�;
		XMLROOT->get_childNodes(&XMLNODES);//��ø��ڵ�������ӽڵ�; 
		//dpath
		CMyXml3::myGetNodeTextByIndex(XMLNODES,0,pfxparam->sdpath,128);
		//Timer
		CMyXml3::myGetNodeTextByIndex(XMLNODES,1,str,5);
		pfxparam->nTimerUpdate=atoi(str);
		//Timer1
		CMyXml3::myGetNodeTextByIndex(XMLNODES,2,str,5);
		pfxparam->nTimerUpdate1=atoi(str);
		//Timer2
		CMyXml3::myGetNodeTextByIndex(XMLNODES,3,str,5);
		pfxparam->nTimerUpdate2=atoi(str);
		//SaveW
		CMyXml3::myGetNodeTextByIndex(XMLNODES,4,str,5);
		pfxparam->nSaveW=atoi(str);
		//SaveH
		CMyXml3::myGetNodeTextByIndex(XMLNODES,5,str,5);
		pfxparam->nSaveH=atoi(str);
		SAFERELEASEXML(XMLNODES);
		SAFERELEASEXML(XMLROOT);
 	}//end if(pXml->myOpenDoc(fn))
	SAFERELEASEXML(XMLDOC);
	CoUninitialize();
}
// д�����ݷ�������
void mySaveSjfxParam(PFXPARAM pfxparam,LPSTR spath)
{
	char fn[MAX_PATH],str[5];
	//
	CoInitialize(NULL); 
	MSXML2::IXMLDOMDocumentPtr XMLDOC;
	MSXML2::IXMLDOMElementPtr XMLROOT; 

	HRESULT HR = XMLDOC.CreateInstance(_uuidof(MSXML2::DOMDocument30)); 
	if(SUCCEEDED(HR))
	{
		//��������ָ��
		CMyXml3::myCreateProcessingInstruction(XMLDOC);
		//�������ڵ�
		XMLROOT = XMLDOC->createElement("param"); 
		XMLDOC->appendChild(XMLROOT); 
   		//����dpath�ڵ�
		CMyXml3::myCreateElement(XMLDOC,XMLROOT,"dpath",pfxparam->sdpath);
		//����Timer�ڵ�
		wsprintf(str,"%d",pfxparam->nTimerUpdate);
		CMyXml3::myCreateElement(XMLDOC,XMLROOT,"Timer",str);
        //����Timer1�ڵ�
		wsprintf(str,"%d",pfxparam->nTimerUpdate1);
		CMyXml3::myCreateElement(XMLDOC,XMLROOT,"Timer1",str);
        //����Timer2�ڵ�
		wsprintf(str,"%d",pfxparam->nTimerUpdate2);
		CMyXml3::myCreateElement(XMLDOC,XMLROOT,"Timer2",str);
        //����SaveW�ڵ�
		wsprintf(str,"%d",pfxparam->nSaveW);
		CMyXml3::myCreateElement(XMLDOC,XMLROOT,"SaveW",str);
        //����SaveH�ڵ�
		wsprintf(str,"%d",pfxparam->nSaveH);
		CMyXml3::myCreateElement(XMLDOC,XMLROOT,"SaveH",str);	
		//д������ļ� 
     	wsprintf(fn,"%s\\param\\param.xml",spath);
		XMLDOC->save(fn);
		//
        SAFERELEASEXML(XMLROOT); 
	}
	SAFERELEASEXML(XMLDOC);
	CoUninitialize();
}

CmyDC::CmyDC(int w,int h)
{
	m_dcW=w; m_dcH=h;
	m_hdc=NULL;
	m_hglrc=NULL;
	myInitDC();
}
CmyDC::~CmyDC()
{
	myReleaseDC();
}
//��ʼ����̨������
void CmyDC::myInitDC()
{
	BITMAPINFO info;
	byte *bits;
	m_hdc=CreateCompatibleDC(NULL);
	memset(&info,0,sizeof(info));
	info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
	info.bmiHeader.biWidth = m_dcW;
	info.bmiHeader.biHeight = m_dcH;
	info.bmiHeader.biPlanes =1;
	info.bmiHeader.biBitCount =24;
	info.bmiHeader.biCompression =BI_RGB;
	m_hbmp=CreateDIBSection(m_hdc,&info,DIB_RGB_COLORS,(void **)&bits,NULL,0);//
	SelectObject(m_hdc,m_hbmp);
		//
	PIXELFORMATDESCRIPTOR pfd = { 
            sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd 
            1,                     // version number 
            PFD_DRAW_TO_BITMAP | 
            PFD_SUPPORT_OPENGL |   // support OpenGL 
            PFD_SUPPORT_GDI ,
            PFD_TYPE_RGBA,         // RGBA type 
            24,                    // 24-bit color depth 
            0, 0, 0, 0, 0, 0,      // color bits ignored 
            0,                     // no alpha buffer 
            0,                     // shift bit ignored 
            0,                     // no accumulation buffer 
            0, 0, 0, 0,            // accum bits ignored 
            0,                     // 32-bit z-buffer 
            0,                     // no stencil buffer 
            0,                     // no auxiliary buffer 
            PFD_MAIN_PLANE,        // main layer 
            0,                     // reserved 
            0, 0, 0                // layer masks ignored 
	}; 
    int  iPixelFormat;
	
	iPixelFormat = ChoosePixelFormat(m_hdc, &pfd); 
    if( iPixelFormat == 0 || !SetPixelFormat(m_hdc, iPixelFormat, &pfd))
		return ;
   
    m_hglrc = wglCreateContext(m_hdc);
	wglMakeCurrent(m_hdc, m_hglrc); 
	
	glViewport(0,0,m_dcW,m_dcH);
	//Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//Establish clipping volume(left,right,bottom,top)
	gluOrtho2D(0,m_dcW,m_dcH,0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glShadeModel(GL_SMOOTH); 
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); //��ɫ����
    glEnable(GL_BLEND);//�����ɫ
    glEnable(GL_LINE_SMOOTH);//������ƽ��(�������)
	glEnable(GL_POINT_SMOOTH);
}
void CmyDC::mySetCurrent()
{
	if(m_hdc!=NULL && m_hglrc!=NULL)
	{
    	wglMakeCurrent(m_hdc, m_hglrc); 
	}
}
void CmyDC::myReleaseDC()
{
	if(m_hdc!=NULL)
	{
	    if(m_hglrc!=NULL)
	    {
			wglMakeCurrent(m_hdc, NULL) ; 
            wglDeleteContext(m_hglrc);
    	}
		DeleteObject(m_hbmp);
    	DeleteDC(m_hdc);
		m_hbmp=NULL;
		m_hdc=NULL;
	}
}