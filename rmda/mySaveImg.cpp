#include "stdafx.h"
#include "common.h"

extern char gszpath[128];
extern FXINFO fxinfo;
extern CArray<PTZINFO,PTZINFO> list_tz;
extern int gww,gwh;



//�Զ�����ģʽ�±���Ϊͼ���ļ�
void mySaveImgFile(HDC hdc,HBITMAP hbmp)
{
	char  szPath[260];
	wsprintf(szPath,"%s\\IMG\\",gszpath);
	//�ļ���ǰ׺
	switch(fxinfo.nFX)
	{
	case 1:
		lstrcat(szPath,"��վ��Ҫ������ͼ_");
		lstrcat(szPath,list_tz.GetAt(fxinfo.nStation)->zm);	
		break;
	case 2:    		
		lstrcat(szPath,"��վ��Ҫ��ʱ�շֲ�ͼ_");
		lstrcat(szPath,list_tz.GetAt(fxinfo.nStation)->zm);
		break;
	case 3:
		lstrcat(szPath,"��վ��Ҫ��ʱ������ͼ_");
		lstrcat(szPath,list_tz.GetAt(fxinfo.nStation)->zm);
		break;
    case 11:
		lstrcat(szPath,"��վ��Ҫ�ضԱ�ͼ");
		break;
	default:
		break;
	}
	lstrcat(szPath,".png");
	mySavePicToFile(hdc,hbmp,gww,gwh,szPath);
}