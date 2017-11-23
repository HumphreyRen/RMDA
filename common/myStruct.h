#pragma once

#define MaxValue -9999
#define MinValue  9999

//数据分析参数
typedef struct
{
	char sdpath[128];   //数据根目录
    int  nTimerUpdate;  //数据自动更新时间间隔(分钟)
    int  nTimerUpdate1; //单站多要素廓线图保存间隔(分钟)
    int  nTimerUpdate2; //单站多要素时空分布图保存间隔(分钟)
    int  nSaveW;  //存图宽度（像素）
	int  nSaveH;  //存图高度（像素）
} FXPARAM,*PFXPARAM;  

//数据分析信息
typedef struct
{
	int  nYear;      //年
	int  nMonth;     //月
	int  nDay;       //日
	int  nFX;        //分析类别
	int  nStation;   //台站序号
	int  nTime;      //时间序号
	int  nProcessor; //处理器序号 
	int  nYs;        //要素序号, =0-温度,1-湿度,2-水汽密度,3-液水含量
	int  nHigh;      //高度序号
} FXINFO,*PFXINFO;  

//微波辐射计单站单时间各要素值
typedef struct
{
	char  sDate[11];   //日期,年-月-日
	char  sTime[9];   //时间,时:分:秒
	float fTT;        //地面温度
	float fRH;        //地面湿度
	float fPP;        //地面气压(hPa)
	float fCT;        //云底温度
	float fCH;        //云底高度
	int   nEO;        //有无降水(0,1)
	float fSQ;        //积分水汽含量 
	float fLW;        //积分液态水
    float *ff[4][4];  //4个处理器4个要素各高度值
} MRYS,*PMRYS; //

//微波辐射计单站资料
typedef struct
{
    int  nTime;       //多站显示时,与fxinfo.nTime对应的本站时次序号
	int  nProcessor;   //与当前选择处理器对应的本站数据序号
	char sProcessors[4][20];  //4个处理器名
	CArray<PMRYS,PMRYS> Ys;
} MRDATA,*PMRDATA; //

//台站信息数据结构
typedef struct
{
	char  zm[15];   //站名
	char  zh[5];    //站号
} TZINFO,*PTZINFO;  

//数据年月目录结构
typedef struct
{
	CString nian;
	CArray<CString,CString&> yue;
} NIANYUE,*PNIANYUE;

//数据文件读取线程参数
typedef struct
{
	HANDLE  hFinish;   //线程结束标志
	LPSTR   sPath;     //数据根目录 
    LPSTR   sZm;       //
    LPSTR   sZh;       //
	int nY;            //
	int nM;            //
	int nD;            //
	PMRDATA pdata;     //
}RSVPARAM,*PRSVPARAM;

//微波辐射计设备运行状态值
typedef struct
{
	char  sDate[11];   //日期,年-月-日
	char  sTime[9];    //时间,时:分:秒
	byte  value[10];   //状态值
} HSTATUS,*PHSTATUS; //