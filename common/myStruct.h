#pragma once

#define MaxValue -9999
#define MinValue  9999

//���ݷ�������
typedef struct
{
	char sdpath[128];   //���ݸ�Ŀ¼
    int  nTimerUpdate;  //�����Զ�����ʱ����(����)
    int  nTimerUpdate1; //��վ��Ҫ������ͼ������(����)
    int  nTimerUpdate2; //��վ��Ҫ��ʱ�շֲ�ͼ������(����)
    int  nSaveW;  //��ͼ��ȣ����أ�
	int  nSaveH;  //��ͼ�߶ȣ����أ�
} FXPARAM,*PFXPARAM;  

//���ݷ�����Ϣ
typedef struct
{
	int  nYear;      //��
	int  nMonth;     //��
	int  nDay;       //��
	int  nFX;        //�������
	int  nStation;   //̨վ���
	int  nTime;      //ʱ�����
	int  nProcessor; //��������� 
	int  nYs;        //Ҫ�����, =0-�¶�,1-ʪ��,2-ˮ���ܶ�,3-Һˮ����
	int  nHigh;      //�߶����
} FXINFO,*PFXINFO;  

//΢������Ƶ�վ��ʱ���Ҫ��ֵ
typedef struct
{
	char  sDate[11];   //����,��-��-��
	char  sTime[9];   //ʱ��,ʱ:��:��
	float fTT;        //�����¶�
	float fRH;        //����ʪ��
	float fPP;        //������ѹ(hPa)
	float fCT;        //�Ƶ��¶�
	float fCH;        //�Ƶ׸߶�
	int   nEO;        //���޽�ˮ(0,1)
	float fSQ;        //����ˮ������ 
	float fLW;        //����Һ̬ˮ
    float *ff[4][4];  //4��������4��Ҫ�ظ��߶�ֵ
} MRYS,*PMRYS; //

//΢������Ƶ�վ����
typedef struct
{
    int  nTime;       //��վ��ʾʱ,��fxinfo.nTime��Ӧ�ı�վʱ�����
	int  nProcessor;   //�뵱ǰѡ��������Ӧ�ı�վ�������
	char sProcessors[4][20];  //4����������
	CArray<PMRYS,PMRYS> Ys;
} MRDATA,*PMRDATA; //

//̨վ��Ϣ���ݽṹ
typedef struct
{
	char  zm[15];   //վ��
	char  zh[5];    //վ��
} TZINFO,*PTZINFO;  

//��������Ŀ¼�ṹ
typedef struct
{
	CString nian;
	CArray<CString,CString&> yue;
} NIANYUE,*PNIANYUE;

//�����ļ���ȡ�̲߳���
typedef struct
{
	HANDLE  hFinish;   //�߳̽�����־
	LPSTR   sPath;     //���ݸ�Ŀ¼ 
    LPSTR   sZm;       //
    LPSTR   sZh;       //
	int nY;            //
	int nM;            //
	int nD;            //
	PMRDATA pdata;     //
}RSVPARAM,*PRSVPARAM;

//΢��������豸����״ֵ̬
typedef struct
{
	char  sDate[11];   //����,��-��-��
	char  sTime[9];    //ʱ��,ʱ:��:��
	byte  value[10];   //״ֵ̬
} HSTATUS,*PHSTATUS; //