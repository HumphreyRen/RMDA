#include "stdafx.h"

#include "myDrawSB.h"
#include "myTimeScale.h"
#include "myDrawCsv.h"


//单站多要素时空分布图
void CmyDrawCsvEvolve::myDrawEvolve(Graphics *pg,int gnw,int gnh,CArray<PMRDATA,PMRDATA> *pmrdata,float *pg_fH,PFXINFO pfxinfo,PTZINFO ptz)
{
	bHaveData=false;
	pmrs=pmrdata;
	fH=pg_fH;
	PMRDATA pmr=pmrs->GetAt(0);
	if(pmr->Ys.GetCount()<1 || pmr->nProcessor ==-1)
	{
		return;
	}
	//读取色标
	CmyDrawSB::myReadSB("param\\sb.txt",&r,&g,&b,m_sbCount);
	//抽样参数
	m_nz=4;
	//设置表格宽高及边距
	m_MarL=40;	m_MarR=90;
	m_MarT=25;	m_MarB=20;	
	m_nw=gnw ;        //为每个要素表格所占宽度,含左右边距
	m_nh=(gnh-35-15)/4;  //高度4等分,为每个要素表格所占高度,含上下边距	
	m_MaxHigh=pg_fH[57-pfxinfo->nHigh]; //最大高度
	if( m_MaxHigh<1)//最大高度至少为1公里
		m_MaxHigh=1;

	//计算时间数量、最早时间值和时间跨度值(秒)
	CmyTimeScale::myCalcTimeScale(pmrs,m_nw,m_MarL,m_MarR,m_TimeCount);
	m_xnum= m_TimeCount/m_nz+1;
	//计算时间方向各时间X坐标值
	int *nX=(int *)malloc(m_xnum*sizeof(int));
	for(int i=0;i<m_xnum;i++)
	{
		int j=i*m_nz;
		if(i==(m_xnum-1)) j=m_TimeCount-1;
		PMRYS pys;
		for(int k=2;k>=0;k--)
		{
    		if(j<pmrs->GetAt(k)->Ys.GetCount())  
			{
		    	pys=pmrs->GetAt(k)->Ys.GetAt(j);
				break;
			}
     		else
			{
				j-=pmrs->GetAt(k)->Ys.GetCount();
			}
		}
		nX[i]=CmyTimeScale::Time2X(pys);
	}
	//计算高度数量
	m_HighCount=0;
	for(int i=0;i<58;i++)
	{
		if(pg_fH[i]> m_MaxHigh) 
			break;
		m_HighCount++;
	}	
	//计算高度方向各高度Y坐标值
	int *nY[4];
	for(int k=0;k<4;k++)
	{
    	nY[k]=(int *)malloc(m_HighCount*sizeof(int));
    	for(int j=0;j<m_HighCount;j++)
     	{
	     	nY[k][j]=High2Y(pg_fH[m_HighCount-j-1],k);//
	    }
	}
	//四个要素等值线跟踪
	float **ppd[4];  //四个要素格点阵列
	CmyDZX mydzx[4]; //四个要素等值线跟踪对象
	//
	//创建四个要素计算线程
	ThrdPARAM tps[4];
	for(int k=0;k<4;k++)
	{
		ppd[k]=(float **)malloc(sizeof(int)*m_xnum);
		tps[k].hFinish =CreateEvent(NULL,TRUE,FALSE,NULL);
		tps[k].pdraw = this;
		tps[k].nYs =k;
		tps[k].pmrs =pmrs;
		tps[k].nX =nX;
		tps[k].nY =nY[k];
		tps[k].pd =ppd[k];
		tps[k].pmydzx = &(mydzx[k]);
		AfxBeginThread((AFX_THREADPROC)myEvolveThrd, &tps[k]);
	}
	//等待各要素计算线程结束
	for(int k=0;k<4;k++)
	{
		WaitForSingleObject(tps[k].hFinish, INFINITE);
		CloseHandle(tps[k].hFinish);
	}
	//绘制表格
	myDrawTable(pg,ptz);
	//绘制各要素（温度、湿度、水汽密度、液水含量）
	for(int k=0;k<4;k++)
	{
		if(tps[k].bNullData == false)
		{
			//填充
    		myFillCsvEvolve(ppd[k],nX,nY[k],fMin[k],fMax[k],m_sbCount);//
			//画等值线
			myDrawDzx(pg,&(mydzx[k].dzxs),k);
		}
	}
	//
	//释放内存资源
	for(int k=0;k<4;k++)
	{
		//ppd
    	for(int i=0;i<m_xnum;i++)
	    	free(ppd[k][i]);
	    free(ppd[k]);
		//nY
		free(nY[k]);
	}
	free(nX);
	free(r);
	free(g);
	free(b);
	bHaveData=true;
}
void CmyDrawCsvEvolve::myDrawXZ(Graphics *pg,Gdiplus::Font *pfont,SolidBrush *pbrush,int nYs,float xz,int x,int y)
{
	char str[10];
	wsprintf(str,"%d",(int)xz);
    _bstr_t	bt=(LPSTR)str;
	pg->DrawString((BSTR)bt, -1, pfont, PointF((REAL)x,(REAL)y), pbrush);
}
//画等值线
void CmyDrawCsvEvolve::myDrawDzx(Graphics *pg,CArray<PDZX,PDZX> *pdzxs,int nYs)
{
	pg->SetClip(Rect(m_MarL,35+m_MarT+ m_nh*nYs,m_nw-m_MarL-m_MarR,m_nh-m_MarT-m_MarB));
	_bstr_t bt=(char *)"黑体";
	FontFamily  fontFamily((BSTR)bt);
	Gdiplus::Font font(&fontFamily, (float)12, FontStyleRegular, UnitPixel);//FontStyleBold
	SolidBrush brush(Color(255,0,0,0));

	glColor3ub(0,0,0);
   	glLineWidth(1);
	int nLineCount=pdzxs->GetCount();
	int nCount;
	POINT pt;
	for(int i=0;i<nLineCount;i++)
	{
		PDZX pdzx=pdzxs->GetAt(i);
		nCount=pdzx->pts.GetCount(); 
		glBegin(GL_LINE_STRIP);
	    for(int j=0;j<nCount;j++)
	    {
			pt= pdzx->pts.GetAt(j);
		    glVertex2i(pt.x,pt.y);
	    }
    	glEnd();
    	glFinish();
		//
        if(nYs<3 && pdzx->lx == 0)
		{
			pt= pdzx->pts.GetAt(nCount/2);
			myDrawXZ(pg,&font,&brush,nYs,pdzx->xz,pt.x,pt.y-5);
		}
	}
	pg->ResetClip();
}
//填充温度、湿度、水汽密度、液水含量
void CmyDrawCsvEvolve::myFillCsvEvolve(float **pd,int *nX,int *nY,float fMin,float fMax,int sbCount)
{
	int x1,x2,y1,n;
	for(int i=0;i<m_xnum-1;i++)//时间方向
	{
		x1=nX[i];//得到有效值x1
		x2=nX[i+1];//得到有效值x2
		glBegin(GL_QUAD_STRIP);
		for(int j=0;j<m_HighCount;j++)//高度方向
		{
			y1=nY[j];  //得到有效值y1
			//以下定义右下角点位置及颜色属性 	
			n=CmyDrawSB::myGetSbColorIndex(pd[i+1][j],fMin,fMax,sbCount);
            glColor3ub(r[n],g[n],b[n]); 
            glVertex2i(x2,y1);	  				 
			//以下定义左下角点位置及颜色属性 
			n=CmyDrawSB::myGetSbColorIndex(pd[i][j],fMin,fMax,sbCount);
            glColor3ub(r[n],g[n],b[n]); 
            glVertex2i(x1,y1);		
		}
		glEnd();//填充一个矩形完成
	    glFlush();
	}
}

//绘制表格
void CmyDrawCsvEvolve::myDrawTable(Graphics *pg,PTZINFO ptz)
{
	_bstr_t bt=(char *)"宋体";
	FontFamily  fontFamily((BSTR)bt);
	Gdiplus::Font font(&fontFamily, (float)13, FontStyleRegular, UnitPixel);//FontStyleBold
	Pen pen(Color(255,128,128,128),1);
	SolidBrush brush(Color(255,0,0,0));
	//标题
	char str[100];
	wsprintf(str,"%s站微波辐射计观测数据时空分布图",ptz->zm);
	bt=(LPSTR)str;
   	pg->DrawString((BSTR)bt, -1, &Gdiplus::Font(&fontFamily, (float)16, FontStyleRegular, UnitPixel), PointF(10,10), &brush);
	//
	int x0,y0,w,h;
	x0=m_MarL;w=m_nw-m_MarL-m_MarR;h=m_nh-m_MarT-m_MarB;
	for(int i=0;i<4;i++)
	{
		y0= 35 + m_MarT+m_nh*i;
		myDrawTableKuang(pg,&font,&pen,&brush,x0,y0,w,h,i);
		CmyDrawSB::myDrawSBStrip(r,g,b,m_sbCount,m_nw-60,y0,10,h);
	    CmyDrawSB::myDrawSBScale(pg,&font,&brush,fMin[i],fMax[i],m_nw-50,y0,h);
		CmyTimeScale::myDrawTimeScale(pg,&font,&brush,y0+m_nh-m_MarT-m_MarB,i==3?true:false);
	}	
} 

//绘制表格边框
void CmyDrawCsvEvolve::myDrawTableKuang(Graphics *pg,Gdiplus::Font *pfont,Pen *ppen,SolidBrush *pbrush,int x0,int y0,int w,int h,int nYs)
{	
	char sYs[4][20]={"温度(℃)","相对湿度(%)","水汽密度(g/m^3)","液水含量(g/m^3)"};
	StringFormat stringFormat;
    stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter); 
	//要素名
	_bstr_t bt;
	bt=(LPSTR)sYs[nYs];
	pg->DrawString((BSTR)bt, -1, pfont, PointF((REAL)x0+w/2,(REAL)y0-15), &stringFormat, pbrush);
	//边框
	pg->DrawRectangle(ppen,x0,y0,w,h);
	//高度单位
    stringFormat.SetAlignment(Gdiplus::StringAlignmentFar); 
	bt=(LPSTR)"km";
	pg->DrawString((BSTR)bt, -1, pfont, PointF((REAL)x0,(REAL)y0-18), &stringFormat, pbrush);	
	//高度间隔
	char str[5];
    int y1;
	float fStep=1;
	if(m_MaxHigh<=1)
		fStep=0.2f;
	else if(m_MaxHigh>6)
		fStep=2;
	float f=0;
	int i=0;
	do
	{
		y1=(int)(y0+h- f*h/m_MaxHigh);
    	pg->DrawLine(ppen,x0-5,y1,x0,y1);//高度间隔线
		if(m_MaxHigh<=1)
         	sprintf(str,"%.1f",f);
		else
         	sprintf(str,"%d",(int)f);
		bt=(LPSTR)str;
		pg->DrawString((BSTR)bt, -1, pfont, PointF((REAL)x0-5,(REAL)y1-5), &stringFormat, pbrush);
		i++;		
		f+=fStep;
	}while(f<=m_MaxHigh);
}

//高度转换为坐标Y值
int CmyDrawCsvEvolve::High2Y(float fH,int nYs)
{
	return (int)(35 + m_nh*nYs+ m_nh -m_MarB - fH*(m_nh-m_MarT-m_MarB)/m_MaxHigh);
}
//坐标Y值转换为高度
float CmyDrawCsvEvolve::Y2High(int y,int nYs)
{
	return (float)(35 + m_nh*nYs+ m_nh -m_MarB - y)*m_MaxHigh/(m_nh-m_MarT-m_MarB);
}
//获取X,Y坐标点对应的物理量信息
void CmyDrawCsvEvolve::myGetDiagramData(int x,int y,LPSTR str)
{
	if(bHaveData && pmrs->GetCount()==3)
	{
		RECT rc;
		POINT pt;
		char sTemp[50],sTime[10],sDate[11];
		pt.x =x;pt.y=y;
		for(int n=0;n<4;n++)
		{
			SetRect(&rc,m_MarL,35 + m_MarT+m_nh*n,m_nw-m_MarR+1,35+m_nh*(n+1)-m_MarB+1);
			if(PtInRect(&rc,pt))//鼠标在要素n对应的有效区域中
			{
				CmyTimeScale::X2Times(x,sDate,sTime);
				float h= Y2High(y,n);
				//求高度h和时间sTime对应的要素值
				int nh=0;
				for(int i=0;i<58;i++)
				{
					if( h<=fH[i]) 
					{
						nh=i;
						break;
					}
				}
				for(int k=2;k>=0;k--)
				{
					PMRDATA pdata=pmrs->GetAt(k);
					for(int i=0;i<pdata->Ys.GetCount();i++)
			        {
		    	    	PMRYS pys=pdata->Ys.GetAt(i);
				    	if(lstrcmp(sDate,pys->sDate)==0 && lstrcmp(sTime,pys->sTime )<=0)
					    {
						    float *f=pys->ff[pdata->nProcessor][n];
							if(f!=NULL)
							{
								if(n==0)
    								sprintf(sTemp,"温度 %.1f℃",f[nh]);
								else if(n==1)
									sprintf(sTemp,"相对湿度 %.1f%%",f[nh]);
								else if(n==2)
									sprintf(sTemp,"水汽密度 %.1fg/m^3",f[nh]);
								else 
									sprintf(sTemp,"液水含量 %.3fg/m^3",f[nh]);

								sprintf(str,"图示：时间 %s %s，高度 %.2fkm，%s",sDate,sTime,h,sTemp);
							}
						    return;
					    }
					}//end for(i)
				}//end for(k)				
			}// end if(PtInRect(&rc,pt))
		}//end for(n)
	}
}
