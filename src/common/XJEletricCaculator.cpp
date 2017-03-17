#include "XJEletricCaculator.h"
#include <float.h>
#include <math.h>
//#include <stdio.h>//printf()
//#include <stdlib.h>//exit()
//#ifndef OS_WINDOWS
//#include <unistd.h>//sleep()
//#endif
bool myIsNaN(float x)
{
	return !(x == x);
}
bool myIsFinite(float x)
{
	return (x <= FLT_MAX && x >= -FLT_MAX);
}
//计算复数相角
static 	double Get_COMPLEX_ANGLE(double a,double b)
{
	double fresult = 0.0f;
	if(fabs(a) > 0.0001f)
	{
		fresult = atan(b/a);
		if(fresult > 0.0)//1,3象限
		{
			if(a < 0.0f && b<0.0f)//第三象限
			{
				fresult = fresult -PI;
			}
		}
		else if(fresult < -0.0f)//2,4象限)
		{
			if(a < 0.0f && b >0.0f)//第二象限
			{
				fresult = fresult + PI;
			}
		}
		else//0度或者180度
		{
			if(a > 0.0f)//0度
			{
				fresult = 0.0f;
			}
			else
			{
				fresult = PI;
			}
		}
	}
	else//实部为零
	{
		if( b > 0)
			fresult = PI/2.0;
		else if( b < 0)
			fresult = -PI/2.0;
		else 
			fresult = 0.0;
	}
	return fresult;
}

/*
 *R=幅值
 *Y=采样值
 *bDesc:是否超出asin的范围， 反正弦函数asin的范围是单调递增的1，4象限（-pi/2~pi/2）.这里要求的是（-pi~pi）的幅角，超出部分的值在单调递减范围内，二三象限的值和一四象限的值分别关于x=PI和x=-PI对称。
*/
static 	double Get_ANGLE_ASIN(double R,double Y,bool bDesc)
{
	double fresult=0.0L;
	if(R> 0.00001f)
	{
		double t=Y/R;
		if (t>1)//零漂指会使得幅值<采样值
		{
			t=1;
		}
		else if(t<-1) {
			t=-1;
		}
		fresult = asin(t)*180.0f/PI;
	}
	else
	{
		fresult = 0.0L;
	}

	if(fresult >= 0)
	{
		if(bDesc)//2象限
		{
			fresult = 180.0 - fresult;
		}
	}
	else
	{
		if(bDesc)//3象限
		{
			fresult = -180.0 -fresult;
		}
	}
	return fresult;
}

CXJEletricCaculator::CXJEletricCaculator(void)
{
}


CXJEletricCaculator::~CXJEletricCaculator(void)
{
}

//幅值计算、相角计算
//要求：nsampleCount 至少大于8
//返回值: pResult_Out[0]: 有效值
//        pResult_Out[1]: 幅值
//        pResult_Out[2]: 相角
void CXJEletricCaculator::Cal_fuzhi(int* pValueList_In,float* pResult_Out,int nsampleCount)
{
	if(nsampleCount < 8)
	{
		return;
	}
	float fTemp = 0.0f,fAvg=0.0f,fAmp=0.0f;//I,D,A,I^2=积分(D+Asint)^2dt=D^2+A^2/2
	for(int i = 0; i < nsampleCount; i++)
	{
		fTemp +=  (pValueList_In[i]/10000.0f)*(pValueList_In[i]/10000.0f);
		fAvg+=pValueList_In[i];
	}
	fAvg/=nsampleCount;//   D=favg
	//fAmp=2*fTemp*10000.0f;//I^2=(2D^2+A^2)

	pResult_Out[0] = sqrt(fTemp/(nsampleCount*1.0f)) *10000.0f;//方均根得到有效值I=sqrt(D^2+A^2/2)
	//pResult_Out[1] = pResult_Out[0] *1.41421356f;    //幅值
	pResult_Out[0]= (pResult_Out[0]*pResult_Out[0]-fAvg*fAvg);//幅值A^2=(2I^2-2D^2),正弦波有效值E^2=I^2-D^2
	if (pResult_Out[0]>0)
	{
		pResult_Out[0]=sqrt(pResult_Out[0]);
	}
	else
	{
		pResult_Out[0]=0;
	}
	pResult_Out[1]= pResult_Out[0]*1.41421356f;//A=E*sqrt(2);//正弦波幅值
	//相角计算
	bool bDesc = pValueList_In[nsampleCount-1] < pValueList_In[nsampleCount-2];
	//pResult_Out[2] = Get_ANGLE_ASIN(pResult_Out[1],pValueList_In[nsampleCount-1],bDesc);
	pResult_Out[2] = Get_ANGLE_ASIN(pResult_Out[1],pValueList_In[nsampleCount-1]-fAvg,bDesc);
	//if(pResult_Out[2] >= 0)
	//{
	//	if(pValueList_In[nsampleCount-1] < pValueList_In[nsampleCount-2])
	//		pResult_Out[2] = 180 - pResult_Out[2];
	//}
	//else
	//{
	//	if(pValueList_In[nsampleCount-1] < pValueList_In[nsampleCount-2])
	//		pResult_Out[2] = -180 -pResult_Out[2];
	//}
}
//直流分量计算
float CXJEletricCaculator::Cal_n_zhiliu(int* pValueList_In,int nsampleCount)
{
	float fTemp = 0.0f;
	for(int i = 0; i < nsampleCount; i++)
	{
		fTemp +=  pValueList_In[i]*1.0f;
	}
	fTemp /= nsampleCount;
	return fTemp;
}
//n点 n次谐波计算-通用计算方法，没有使用预设置参数参与计算
//要求：nsampleCount >=8
//nxiebo:>=1
//实部： Xrn =  (2/N)* ∑X(k)sin(n*(k+1) * 2∏/N); k=0,N-1   
//虚部： Xin =  (2/N)* ∑X(k)cos(n*(k+1) * 2∏/N); k=0,N-1
//返回值:
//     pResult_Out[0]: 实部
//     pResult_Out[1]：虚部
//     pResult_Out[2]：有效值
//     pResult_Out[3]：相角

void CXJEletricCaculator::Cal_n_xiebo_n(int* pValueList_In,float* pResult_Out,int nsampleCount,int nxiebo)
{
	if( nxiebo < 0 ||nsampleCount < 8)//nxiebo <= 0 ||
		return;
	float Xr,Xi;
	Xr = 0.0f;
	Xi = 0.0f;
	float N = 1.0f/nsampleCount;
	float yinzi = 2*PI *N;
	for(int i = 0 ; i < nsampleCount; i++)    
	{
		Xr += (float)pValueList_In[i] * sin((i+1)*nxiebo * yinzi);
		Xi += (float)pValueList_In[i] * cos((i+1)*nxiebo * yinzi);
	}
	pResult_Out[0] = 2*N*Xr;//实部
	pResult_Out[1] = 2*N*Xi; //+ pValueList_In[0]+pValueList_In[nsampleCount-1])*N;//虚部

	pResult_Out[2] = sqrt(pResult_Out[0]*pResult_Out[0] + pResult_Out[1]*pResult_Out[1])/1.41421356f;//有效值
	//相角
	pResult_Out[3] = Get_COMPLEX_ANGLE(pResult_Out[0],pResult_Out[1])*180.0f/PI;
	//if(pResult_Out[3] > 0.0f)//1,3象限
	//{
	//	if(pResult_Out[0] < 0.0f && pResult_Out[1]<0.0f)//第三象限
	//	{
	//	 	 pResult_Out[3] = pResult_Out[3] -180;
	//	}
	//}
	//else if(pResult_Out[3] < 0.0f)//2,4象限
	//{
	//	if(pResult_Out[0] < 0.0f && pResult_Out[1]>0.0f)//第二象限
	//	{
	//		pResult_Out[3] = pResult_Out[3] + 180;
	//	}
	//}
	//else
	//{
	//	;
	//}
}
//正序分量计算
//输入值:
// pValueList_In[0] A相幅值
// pValueList_In[1] A相相角
// pValueList_In[2] B相幅值
// pValueList_In[3] B相相角
// pValueList_In[4] C相幅值
// pValueList_In[5] C相相角
//返回值:
// pResult_Out[0]: 实部
// pResult_Out[1]：虚部
// pResult_Out[2]：幅值
// pResult_Out[3]：相角
// return (a + b * Vector(-0.5, 0.8660254) + c * Vector(-0.5, -0.8660254)) / Vector(3, 0);
/*正序分量计算公式
E1=（A+B*α+C*α*α）/3   α=-0.5+j0.8660254  α*α= -0.5-j0.8660254
复数计算: (a+jb)*(c+jd)=(ac-bd)
                      +j(ad+bc)
*/
void CXJEletricCaculator::Cal_E1(float* pValueList_In,float* pResult_Out)
{
	float a_r,a_i;
	float b_r,b_i;
	float c_r,c_i;
	a_r = pValueList_In[0]*cos(pValueList_In[1]/180 * PI);
	a_i = pValueList_In[0]*sin(pValueList_In[1]/180 * PI);
	b_r = pValueList_In[2]*cos(pValueList_In[3]/180 * PI);
	b_i = pValueList_In[2]*sin(pValueList_In[3]/180 * PI);
	c_r = pValueList_In[4]*cos(pValueList_In[5]/180 * PI);
	c_i = pValueList_In[4]*sin(pValueList_In[5]/180 * PI);
	
	pResult_Out[0] = (a_r+(b_r*(-0.5)-b_i*0.8660254) + (c_r*(-0.5)- c_i*(-0.8660254)))/3;//实部
	pResult_Out[1] = (a_i+(b_r*0.8660254 + b_i*(-0.5)) + (c_r*(-0.8660254)+c_i*(-0.5)))/3;//虚部

	pResult_Out[2] = sqrt(pResult_Out[0]*pResult_Out[0] + pResult_Out[1]*pResult_Out[1]);

	//相角
	pResult_Out[3] = Get_COMPLEX_ANGLE(pResult_Out[0],pResult_Out[1])*180.0f/PI;
	//if(pResult_Out[3] > 0.0f)//1,3象限
	//{
	//	if(pResult_Out[0] < 0.0f && pResult_Out[1]<0.0f)//第三象限
	//	{
	//		pResult_Out[3] = pResult_Out[3] -180;
	//	}
	//}
	//else if(pResult_Out[3] < 0.0f)//2,4象限
	//{
	//	if(pResult_Out[0] < 0.0f && pResult_Out[1]>0.0f)//第二象限
	//	{
	//		pResult_Out[3] = pResult_Out[3] + 180;
	//	}
	//}
	//else
	//{
	//	;
	//}
}
//负序分量计算
//输入值:
// pValueList_In[0] A相幅值
// pValueList_In[1] A相相角
// pValueList_In[2] B相幅值
// pValueList_In[3] B相相角
// pValueList_In[4] C相幅值
// pValueList_In[5] C相相角
//返回值:
// pResult_Out[0]: 实部
// pResult_Out[1]：虚部
// pResult_Out[2]：幅值
// pResult_Out[3]：相角
//return (a + b * Vector(-0.5, -0.8660254) + c * Vector(-0.5, 0.8660254)) / Vector(3, 0);
/*负序分量计算公式
E2=（A+Bα*α+C*α）/3  α*α= -0.5-j0.8660254 α=-0.5+j0.8660254
复数计算: (a+jb)*(c+jd)=(ac-bd)+j(ad+bc)
*/
void CXJEletricCaculator::Cal_E2(float* pValueList_In,float* pResult_Out)
{
	float a_r,a_i;
	float b_r,b_i;
	float c_r,c_i;
	a_r = pValueList_In[0]*cos(pValueList_In[1]/180 * PI);
	a_i = pValueList_In[0]*sin(pValueList_In[1]/180 * PI);
	b_r = pValueList_In[2]*cos(pValueList_In[3]/180 * PI);
	b_i = pValueList_In[2]*sin(pValueList_In[3]/180 * PI);
	c_r = pValueList_In[4]*cos(pValueList_In[5]/180 * PI);
	c_i = pValueList_In[4]*sin(pValueList_In[5]/180 * PI);

	pResult_Out[0] = (a_r+(b_r*(-0.5)-b_i*(-0.8660254)) + (c_r*(-0.5)- c_i*0.8660254))/3;   //实部
	pResult_Out[1] = (a_i+(b_r*(-0.8660254) + b_i*(-0.5)) + (c_r*(0.8660254)+c_i*(-0.5)))/3;//虚部

	pResult_Out[2] = sqrt(pResult_Out[0]*pResult_Out[0] + pResult_Out[1]*pResult_Out[1]);

	//相角
	pResult_Out[3] = Get_COMPLEX_ANGLE(pResult_Out[0],pResult_Out[1])*180.0f/PI;
	//if(pResult_Out[3] > 0.0f)//1,3象限
	//{
	//	if(pResult_Out[0] < 0.0f && pResult_Out[1]<0.0f)//第三象限
	//	{
	//		pResult_Out[3] = pResult_Out[3] -180;
	//	}
	//}
	//else if(pResult_Out[3] < 0.0f)//2,4象限
	//{
	//	if(pResult_Out[0] < 0.0f && pResult_Out[1]>0.0f)//第二象限
	//	{
	//		pResult_Out[3] = pResult_Out[3] + 180;
	//	}
	//}
	//else
	//{
	//	;
	//}
}
//零序分量计算
//输入值:
// pValueList_In[0] A相幅值
// pValueList_In[1] A相相角
// pValueList_In[2] B相幅值
// pValueList_In[3] B相相角
// pValueList_In[4] C相幅值
// pValueList_In[5] C相相角
//返回值:
// pResult_Out[0]: 实部
// pResult_Out[1]：虚部
// pResult_Out[2]：幅值
// pResult_Out[3]：相角
/*零序分量计算公式
E2=A+B+C
*/
void CXJEletricCaculator::Cal_3E0(float* pValueList_In,float* pResult_Out)
{
	float a_r,a_i;
	float b_r,b_i;
	float c_r,c_i;
	a_r = pValueList_In[0]*cos(pValueList_In[1]/180 * PI);
	a_i = pValueList_In[0]*sin(pValueList_In[1]/180 * PI);
	b_r = pValueList_In[2]*cos(pValueList_In[3]/180 * PI);
	b_i = pValueList_In[2]*sin(pValueList_In[3]/180 * PI);
	c_r = pValueList_In[4]*cos(pValueList_In[5]/180 * PI);
	c_i = pValueList_In[4]*sin(pValueList_In[5]/180 * PI);

	pResult_Out[0] = a_r + b_r + c_r;
	pResult_Out[1] = a_i + b_i + c_i;

	pResult_Out[2] = sqrt(pResult_Out[0]*pResult_Out[0] + pResult_Out[1]*pResult_Out[1]);

	//相角
	pResult_Out[3] = Get_COMPLEX_ANGLE(pResult_Out[0],pResult_Out[1])*180.0f/PI;
	//if(pResult_Out[3] > 0.0f)//1,3象限
	//{
	//	if(pResult_Out[0] < 0.0f && pResult_Out[1]<0.0f)//第三象限
	//	{
	//		pResult_Out[3] = pResult_Out[3] -180;
	//	}
	//}
	//else if(pResult_Out[3] < 0.0f)//2,4象限
	//{
	//	if(pResult_Out[0] < 0.0f && pResult_Out[1]>0.0f)//第二象限
	//	{
	//		pResult_Out[3] = pResult_Out[3] + 180;
	//	}
	//}
	//else
	//{
	//	;
	//}

}
/************************************16点谐波计算部分***********************************/
//16点基波计算、相角计算使用
void CXJEletricCaculator::Cal_1xiebo_16(int* pValueList_In,float* pResult_Out)
{

}
//16点3次谐波计算
void CXJEletricCaculator::Cal_3xiebo_16(int* pValueList_In,float* pResult_Out)
{

}
//16点5次谐波计算
void CXJEletricCaculator::Cal_5xiebo_16(int* pValueList_In,float* pResult_Out)
{

}
//16点7次谐波计算
void CXJEletricCaculator::Cal_7xiebo_16(int* pValueList_In,float* pResult_Out)
{

}
/************************************80点谐波计算部分***********************************/
//80点基波计算、相角计算
void CXJEletricCaculator::Cal_1xiebo_80(int* pValueList_In,float* pResult_Out)
{

}
//80点3次谐波计算
void CXJEletricCaculator::Cal_3xiebo_80(int* pValueList_In,float* pResult_Out)
{

}
//80点5次谐波计算
void CXJEletricCaculator::Cal_5xiebo_80(int* pValueList_In,float* pResult_Out)
{

}
//80点7次谐波计算
void CXJEletricCaculator::Cal_7xiebo_80(int* pValueList_In,float* pResult_Out)
{

}

