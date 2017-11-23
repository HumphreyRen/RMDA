#include "stdafx.h"
#include "common.h"

extern char gszpath[128];
extern FXINFO fxinfo;
extern CArray<PTZINFO,PTZINFO> list_tz;
extern int gww,gwh;



//自动更新模式下保存为图像文件
void mySaveImgFile(HDC hdc,HBITMAP hbmp)
{
	char  szPath[260];
	wsprintf(szPath,"%s\\IMG\\",gszpath);
	//文件名前缀
	switch(fxinfo.nFX)
	{
	case 1:
		lstrcat(szPath,"单站多要素廓线图_");
		lstrcat(szPath,list_tz.GetAt(fxinfo.nStation)->zm);	
		break;
	case 2:    		
		lstrcat(szPath,"单站多要素时空分布图_");
		lstrcat(szPath,list_tz.GetAt(fxinfo.nStation)->zm);
		break;
	case 3:
		lstrcat(szPath,"单站各要素时间曲线图_");
		lstrcat(szPath,list_tz.GetAt(fxinfo.nStation)->zm);
		break;
    case 11:
		lstrcat(szPath,"多站单要素对比图");
		break;
	default:
		break;
	}
	lstrcat(szPath,".png");
	mySavePicToFile(hdc,hbmp,gww,gwh,szPath);
}