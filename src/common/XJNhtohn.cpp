// XJString.cpp: implementation of the CXJString class.
//
//////////////////////////////////////////////////////////////////////
#pragma warning(disable : 4275)
#include "XJNhtohn.h"

static const int nEndTest=0X0001; 
bool bNetEndian=*(char*)(&nEndTest)=='\0';
void ReverseBytes(void *vbuf,int nNum)//原位转换，广泛适用已知必须转换字节序,已知源头和目的是相反字节序情况。
{
	char *buf=(char*)vbuf;
	for (int i=0;i<nNum/2;i++)
	{
		buf[i]^=buf[nNum-1-i]^=buf[i]^=buf[nNum-1-i];
	}
}
//host to net,also net to host,易用于整数字节类型
void ntohBytes(void *vbuf,int nNum)//原位转换主机类型到网络字节序。
{
	if(!bNetEndian)ReverseBytes(vbuf,nNum);
}
uint64_t ntohXJ(unsigned char *vbuf,int nNum)//网络字节序得到int型，适用于自定义长度整数
{
	uint64_t nVal=0;
	if (bNetEndian)
	{
		memcpy((char*)&nVal+8-nNum,vbuf,nNum);
	}
	else
	{ 
		nVal=vbuf[0];
		for (int i=1;i<nNum;i++)
		{
			nVal<<=8;
			nVal+=vbuf[i];
		}
	}
	return nVal;
}
void htonXJ(unsigned char *vbuf,int nNum,uint64_t nVal)//整数转换为网络字节序nNum字节缓冲区，适用于子定义长度缓冲区
{	
	if (bNetEndian)
	{
		const char *pRet=(const char*)&nVal;
		memcpy(vbuf,pRet+8-nNum,nNum);
	}
	else
	{
		vbuf[nNum-1]=nVal & 0xFF;
		for (int i=nNum-2;i>=0;i--)
		{
			nVal>>=8;
			vbuf[i]=nVal & 0xFF;
		}
	}
	//unsigned char* buf=(unsigned char*)vbuf;
	//buf[0]=pRet[nNum-1];
	//for (int i=1;i<nNum;i++)
	//{
	//	buf[i]=pRet[nNum-1-i];
	//}
}