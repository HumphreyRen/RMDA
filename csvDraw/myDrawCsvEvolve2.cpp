#include "stdafx.h"

#include "myTimeScale.h"
#include "myCalcScale.h"
#include "myDrawCsv.h"


//单站各要素时间曲线图
void CmyDrawCsvEvolve2::myDrawEvolve2(Graphics *pg,int gnw,int gnh,CArray<PMRDATA,PMRDATA> *pmrdata,PTZINFO ptz)
{
	bHaveData=false;
	pmrs=pmrdata;
	PMRDATA pmr=pmrs->GetAt(0);
	if(pmr->Ys.GetCount()<1)
	{
		return;
	}
	//设置表格宽高及边距
	m_MarTop=35;      //视图标题文本所占高度
	m_nw=gnw ;        // 为每个要素表格所占宽度
	m_nh=(gnh-m_MarTop-35)/7;  //高度7等分，为每个要素表格+名称文本所占高度
	m_MarL=60;
	m_MarR=60;
	m_MarT=25;	//每个要素名称文本所占高度
	//计算时间数量、最早时间值和时间跨度值(秒)
	CmyTimeScale::myCalcTimeScale(pmrs,m_nw,m_MarL,m_MarR,m_TimeCount);
	//计算时间方向各时间X坐标值
	int *nX=(int *)malloc(m_TimeCount*sizeof(int));
	for(int i=0;i<m_TimeCount;i++)
	{
		int j=i;
		PMRYS pys;
		for(int k=2;k>=0;k--)
		{
			PMRDATA pdata=pmrs->GetAt(k);
			int nCount=pdata->Ys.GetCount();
    		if(j<nCount)  
			{
		    	pys=pdata->Ys.GetAt(j);
				break;
			}
     		else
			{
				j-=nCount;
			}
		}
		nX[i]=CmyTimeScale::Time2X(pys);
	}
	//计算各要素刻度最大最小值
	myCalcScaleEvolve2(pmrs);
	//绘制表格
	myDrawTable(pg,ptz);
	//绘制数据线
	myDrawData(pg,pmrs,nX);
	//绘制降水
	myFillCsvEO(pg,pmrs,nX);
	//
	free(nX);
	bHaveData=true;
}
//绘制降水
void CmyDrawCsvEvolve2::myFillCsvEO(Graphics *pg,CArray<PMRDATA,PMRDATA> *pmrs,int *nX)
{
	byte r[2],g[2],b[2];
	r[0]=255; g[0]=255; b[0]=255;
	r[1]=0;   g[1]=0;   b[1]=255;
	
	int y1,y2,n;
	y1= m_MarTop+m_nh*7+1;
	y2= m_MarTop+m_nh*7+10;
	int x=0;
	glBegin(GL_QUAD_STRIP);
	for(int k=2;k>=0;k--)
	{
		PMRDATA pdata=pmrs->GetAt(k);
		for(int i=0;i<pdata->Ys.GetCount();i++)
		{
			PMRYS pys=pdata->Ys.GetAt(i);
			//以下定义颜色属性 
			n= pys->nEO==1 ? 1:0;
            glColor3ub(r[n],g[n],b[n]); 
			//以下定义下角点位置
            glVertex2i(nX[x],y1);		
            glVertex2i(nX[x],y2);	
			x++;
		}
	}
	glEnd();//填充一个矩形完成
    glFlush();
	//降水边框
	Pen pen(Color(255,128,128,128),1);
	pg->DrawRectangle(&pen,m_MarL,m_MarTop+m_nh*7,m_nw-m_MarL-m_MarR,10);
}
//绘制数据线
//   地面气压,地面温度,地面湿度,云底温度,云底高度,积分液态水,积分水汽含量,
void CmyDrawCsvEvolve2::myDrawData(Graphics *pg,CArray<PMRDATA,PMRDATA> *pmrs,int *nX)
{
	Pen pen(Color(255,128,0,0),1);	
	Point *pts=(Point *)malloc(m_TimeCount*sizeof(Point));
	//地面气压
	int x=0;
	int n=0;
	for(int k=2;k>=0;k--)
	{
		PMRDATA pdata=pmrs->GetAt(k);
		for(int i=0;i<pdata->Ys.GetCount();i++)
		{		
			PMRYS pys=pdata->Ys.GetAt(i);
			if(pys->fPP != MaxValue)
			{
				pts[n].Y = D2Y(pys->fPP,0);
				pts[n].X = nX[x];
				n++;
			}
			x++;
		}
	}
	pg->DrawLines(&pen,pts,n);
	//地面温度
	x=0;
	n=0;
	for(int k=2;k>=0;k--)
	{
		PMRDATA pdata=pmrs->GetAt(k);
		for(int i=0;i<pdata->Ys.GetCount();i++)
		{
			PMRYS pys=pdata->Ys.GetAt(i);
			if(pys->fTT != MaxValue)
			{
				pts[n].Y = D2Y(pys->fTT,1);
				pts[n].X = nX[x];
				n++;
			}
			x++;
		}
	}
	pg->DrawLines(&pen,pts,n);
	//地面湿度
	x=0;
	n=0;
	for(int k=2;k>=0;k--)
	{
		PMRDATA pdata=pmrs->GetAt(k);
		for(int i=0;i<pdata->Ys.GetCount();i++)
		{
			PMRYS pys=pdata->Ys.GetAt(i);
			if(pys->fRH != MaxValue)
			{
				pts[n].Y = D2Y(pys->fRH,2);
				pts[n].X = nX[x];
				n++;
			}
			x++;
		}
	}
	pg->DrawLines(&pen,pts,n);
	//云底温度
	x=0;
	n=0;
	for(int k=2;k>=0;k--)
	{
		PMRDATA pdata=pmrs->GetAt(k);
		for(int i=0;i<pdata->Ys.GetCount();i++)
		{
			PMRYS pys=pdata->Ys.GetAt(i);
			if(pys->fCT != MaxValue)
			{
				pts[n].Y = D2Y(pys->fCT,3);
				pts[n].X = nX[x];
				n++;
			}
			x++;
		}
	}
	pg->DrawLines(&pen,pts,n);
	//云底高度
	x=0;
	n=0;
	for(int k=2;k>=0;k--)
	{
		PMRDATA pdata=pmrs->GetAt(k);
		for(int i=0;i<pdata->Ys.GetCount();i++)
		{
			PMRYS pys=pdata->Ys.GetAt(i);
			if(pys->fCH != MaxValue)
			{
				pts[n].Y = D2Y(pys->fCH,4);
				pts[n].X = nX[x];
				n++;
			}
			x++;
		}
	}
	pg->DrawLines(&pen,pts,n);
	//积分液态水
	x=0;
	n=0;
	for(int k=2;k>=0;k--)
	{
		PMRDATA pdata=pmrs->GetAt(k);
		for(int i=0;i<pdata->Ys.GetCount();i++)
		{
			PMRYS pys=pdata->Ys.GetAt(i);
			if(pys->fLW != MaxValue)
			{
				pts[n].Y = D2Y(pys->fLW,5);
				pts[n].X = nX[x];
				n++;
			}
			x++;
		}
	}
	pg->DrawLines(&pen,pts,n);
	//积分水汽含量
	x=0;
	n=0;
	for(int k=2;k>=0;k--)
	{
		PMRDATA pdata=pmrs->GetAt(k);
		for(int i=0;i<pdata->Ys.GetCount();i++)
		{
			PMRYS pys=pdata->Ys.GetAt(i);
			if(pys->fSQ != MaxValue)
			{
				pts[n].Y = D2Y(pys->fSQ,6);
				pts[n].X = nX[x];
				n++;
			}
			x++;
		}
	}
	pg->DrawLines(&pen,pts,n);
    free(pts);	
}
//绘制表格
void CmyDrawCsvEvolve2::myDrawTable(Graphics *pg,PTZINFO ptz)
{
	_bstr_t bt=(char *)"宋体";
	FontFamily  fontFamily((BSTR)bt);
	Gdiplus::Font font(&fontFamily, (float)13, FontStyleRegular, UnitPixel);//FontStyleBold
	Pen pen(Color(255,128,128,128),1);
	SolidBrush brush(Color(255,0,0,0));
	//标题
	char str[100];
	wsprintf(str,"%s站微波辐射计观测数据时间曲线图",ptz->zm);
	bt=(LPSTR)str;
   	pg->DrawString((BSTR)bt, -1, &Gdiplus::Font(&fontFamily, (float)16, FontStyleRegular, UnitPixel), PointF(10,10), &brush);
	//
	int x0,y0,w,h;
	x0=m_MarL;w=m_nw-m_MarL-m_MarR;h=m_nh-m_MarT;
	for(int i=0;i<7;i++)
	{
		y0= m_MarTop + m_MarT+m_nh*i;
		myDrawTableKuang(pg,&font,&pen,&brush,x0,y0,w,h,i);
	}
	bt=(LPSTR)"降水";
	pg->DrawString((BSTR)bt, -1, &font, PointF((REAL)x0+w+2,(REAL)m_MarTop+m_nh*7), &brush );
	CmyTimeScale::myDrawTimeScale(pg,&font,&brush,m_MarTop+10+m_nh*7,true);
} 
//绘制表格边框
void CmyDrawCsvEvolve2::myDrawTableKuang(Graphics *pg,Gdiplus::Font *pfont,Pen *ppen,SolidBrush *pbrush,int x0,int y0,int w,int h,int n)
{	
	char sYsL[][30]={"地面气压(hPa)","地面温度(℃)","地面湿度(%)","云底温度(℃)","云底高度(km)","积分液态水(g/m^3)","积分水汽含量(g/m^3)"};
	StringFormat stringFormat;
    stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter); 
	//要素名
	_bstr_t bt;
	bt=(LPSTR)sYsL[n];
	pg->DrawString((BSTR)bt, -1, pfont, PointF((REAL)x0+w/2,(REAL)y0-15),&stringFormat, pbrush );
	//边框
	pg->DrawRectangle(ppen,x0,y0,w,h);
    stringFormat.SetAlignment(Gdiplus::StringAlignmentFar); 
	//纵轴刻度-要素值
	char str[10];
	for(int i=0;i<2;i++)
	{
		float f = i==0 ? fMinS[n] : fMaxS[n];
		int   y = i==0 ? y0+h-5 : y0+2;		
		if(n==4 || n==5 || n==6)
			sprintf(str,"%.2f",f);
		else //if(n==0 || n==1 || n==3 || n==6)
    		sprintf(str,"%.1f",f);
		bt=(LPSTR)str;
   		pg->DrawString((BSTR)bt, -1, pfont, PointF((REAL)x0,(REAL)y-5), &stringFormat, pbrush);
	}
	//纵向间隔线
	Pen pen(Color(255,128,128,128),1);
	pen.SetDashStyle(Gdiplus::DashStyleDashDot);  
	for(int i=1;i<4;i++)
	{
		int y = y0+ i* h / 4;
        pg->DrawLine(&pen,x0,y,x0+w,y); 
	}
	
}

//要素值转换为坐标Y值
int CmyDrawCsvEvolve2::D2Y(float f,int nYs)
{
	return (int)(m_MarTop + m_nh*nYs + m_MarT + (fMaxS[nYs]-f)*(m_nh-m_MarT)/(fMaxS[nYs]-fMinS[nYs]));
}
//计算单站各要素所有时间范围内的最大最小值刻度
//   地面气压,地面温度,地面湿度,云底温度,云底高度,,积分液态水,积分水汽含量
void CmyDrawCsvEvolve2::myCalcScaleEvolve2(CArray<PMRDATA,PMRDATA> *pmrs)
{
	for(int i=0;i<7;i++)
	{
		fMaxS[i]=MaxValue;  fMinS[i]=MinValue;
	}
	for(int k=2;k>=0;k--)	
	{
		PMRDATA pdata=pmrs->GetAt(k);
		for(int i=0;i<pdata->Ys.GetCount();i++)
		{
			PMRYS pys=pdata->Ys.GetAt(i);
			CmyCalcScale::myCalcMaxMinValue(pys->fPP,fMaxS[0],fMinS[0]);	
			CmyCalcScale::myCalcMaxMinValue(pys->fTT,fMaxS[1],fMinS[1]);
			CmyCalcScale::myCalcMaxMinValue(pys->fRH,fMaxS[2],fMinS[2]);
			CmyCalcScale::myCalcMaxMinValue(pys->fCT,fMaxS[3],fMinS[3]);
			CmyCalcScale::myCalcMaxMinValue(pys->fCH,fMaxS[4],fMinS[4]);
			CmyCalcScale::myCalcMaxMinValue(pys->fLW,fMaxS[5],fMinS[5]);
			CmyCalcScale::myCalcMaxMinValue(pys->fSQ,fMaxS[6],fMinS[6]);
		}
	}
	for(int i=0;i<7;i++)
	{
		if( fMaxS[i] < fMinS[i] )
	    {
	    	fMaxS[i]=10; fMinS[i]=0;
	    }
	}
	fMinS[6]=0;
}

//获取X,Y坐标点对应的物理量信息
void CmyDrawCsvEvolve2::myGetDiagramData(int x,int y,LPSTR str)
{
	if(bHaveData && x>m_MarL && x< (m_nw-m_MarR) && pmrs->GetCount()==3)
	{
		char sTemp[50],sTime[10],sDate[11];		
		RECT rc;
		POINT pt;
		pt.x =x;	pt.y=y;
		for(int n=0;n<7;n++)
		{
			SetRect(&rc,m_MarL, m_MarTop+ m_MarT+m_nh*n,m_nw-m_MarR+1,m_MarTop+m_nh*(n+1)+1);
			if(PtInRect(&rc,pt))//鼠标在要素n对应的有效区域中
		    {
		    	CmyTimeScale::X2Times(x,sDate,sTime);
		    	//时间sTime对应的要素值
				for(int k=2;k>=0;k--)
				{
					PMRDATA pdata=pmrs->GetAt(k);
					for(int i=0;i<pdata->Ys.GetCount();i++)
			        {
		    	    	PMRYS pys=pdata->Ys.GetAt(i);
			         	if(lstrcmp(sDate,pys->sDate)==0 && lstrcmp(sTime,pys->sTime )<=0)
			    	    {
			                sprintf(str,"时间：%s %s， ",sDate,sTime);
							switch(n)
							{
							case 0:
	        					sprintf(sTemp,"地面气压%.1fhPa",pys->fPP);	
		    					break;
							case 1:
								sprintf(sTemp,"地面温度%.1f℃",pys->fTT);
								break;
							case 2:
								sprintf(sTemp,"地面湿度%.1f%%",pys->fRH);
								break;
							case 3:
								sprintf(sTemp,"云底温度%.1f℃",pys->fCT);
								break;
							case 4:
								sprintf(sTemp,"云底高度%.2fkm",pys->fCH);
								break;
							case 5:
								sprintf(sTemp,"积分液态水%.2fg/m^3",pys->fLW);
								break;
							default:
								sprintf(sTemp,"积分水汽含量%.2fg/m^3",pys->fSQ);
								break;
							}
    						lstrcat(str,sTemp);
		    		    	return;
			    	    }
		    	    }//end for(i)
				}//end for(k)	    	
			}//end if(PtInRect(&rc,pt))
		}//end for(n)
	}
}