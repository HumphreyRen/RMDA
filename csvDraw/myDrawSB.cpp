#include "stdafx.h"

#include "..\common\common.h"
#include "myDrawSB.h"

//��ȡɫ��
void CmyDrawSB::myReadSB(LPSTR sFile,byte **pr,byte **pg,byte **pb,int& sbCount)
{
	FILE *fp;
	if( (fp=fopen(sFile,"rt"))!=NULL)
	{
		CArray<int,int&> cr,cg,cb;
		int nr,ng,nb;
		while(!feof(fp))
		{
			if(fscanf(fp,"%d %d %d ",&nr,&ng,&nb)==3)
			{
				cr.Add(nr);
				cg.Add(ng);
				cb.Add(nb);
			}
		}
		fclose(fp);
		sbCount=cr.GetCount();
		*pr=(byte *)malloc(sbCount*sizeof(byte));
		*pg=(byte *)malloc(sbCount*sizeof(byte));
		*pb=(byte *)malloc(sbCount*sizeof(byte));
		for(int i=0;i<sbCount;i++)
		{
			(*pr)[i]=cr.GetAt(i);
			(*pg)[i]=cg.GetAt(i);
			(*pb)[i]=cb.GetAt(i);
		}
		cr.RemoveAll();
		cg.RemoveAll();
		cb.RemoveAll();
	}
}
//���ݸ�����ݺ������Сֵ�����ȡɫ��ֵ
int CmyDrawSB::myGetSbColorIndex(float f,float fMin,float fMax,int sbCount)
{   
	float fline=fMax-fMin;
	if(fline>0)
    	return (int)( (f-fMin)*(sbCount-1)/fline);
	return 0;
}

//����ɫ����
void CmyDrawSB::myDrawSBStrip(byte *r,byte *g,byte *b,int sbCount,int x0,int y0,int w,int h)
{
	float x1,x2,y;
	x1=(float)x0;
	x2=(float)x0+w;
	glBegin(GL_QUAD_STRIP);
	for(int k=0;k<sbCount;k++)
	{	
		y= y0+ (float)k * h /(sbCount-1);//�õ���Чֵy
		int n=sbCount-1-k;		
		//���¶������½ǵ�λ�ü���ɫ���� 	
		glColor3ub(r[n],g[n],b[n]); 
        glVertex2f(x2,y);	  				 
		//���¶������½ǵ�λ�ü���ɫ���� 
		glColor3ub(r[n],g[n],b[n]); 
        glVertex2f(x1,y);	
	}
	glEnd();//���һ���������
    glFlush();
}
//����ɫ��̶�
void CmyDrawSB::myDrawSBScale(Graphics *pg,Gdiplus::Font *pfont,SolidBrush *pbrush,float fMin,float fMax,int x0,int y0,int h)
{
	float fline=fMax-fMin;
	if(fline>0)
	{
		_bstr_t bt;
		char str[8];
		float nStep= fline/4;
 		for(int i=0;i<5;i++)
		{
			float f=fMin+nStep*i;
			int y= (int)(y0+h-(f-fMin)*h/fline);
			if(nStep<0.001)
				sprintf(str,"%.4f",f);
			else if(nStep<0.01)
				sprintf(str,"%.3f",f);
			else if(nStep<0.1)
    			sprintf(str,"%.2f",f);
			else if(nStep<1)
    			sprintf(str,"%.1f",f);
			else
				sprintf(str,"%d",(int)f); 
			bt=(LPSTR)str;
			if( i==0 ) y-=2;
			if( i==4 ) y+=2;
			pg->DrawString((BSTR)bt, -1, pfont, PointF((REAL)x0,(REAL)y-5), pbrush);
		}
	}
}