// XJString.cpp: implementation of the CXJString class.
//
//////////////////////////////////////////////////////////////////////
#pragma warning(disable : 4275)
#include "XJNhtohn.h"

static const int nEndTest=0X0001; 
bool bNetEndian=*(char*)(&nEndTest)=='\0';
void ReverseBytes(void *vbuf,int nNum)//ԭλת�����㷺������֪����ת���ֽ���,��֪Դͷ��Ŀ�����෴�ֽ��������
{
	char *buf=(char*)vbuf;
	for (int i=0;i<nNum/2;i++)
	{
		buf[i]^=buf[nNum-1-i]^=buf[i]^=buf[nNum-1-i];
	}
}
//host to net,also net to host,�����������ֽ�����
void ntohBytes(void *vbuf,int nNum)//ԭλת���������͵������ֽ���
{
	if(!bNetEndian)ReverseBytes(vbuf,nNum);
}
uint64_t ntohXJ(unsigned char *vbuf,int nNum)//�����ֽ���õ�int�ͣ��������Զ��峤������
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
void htonXJ(unsigned char *vbuf,int nNum,uint64_t nVal)//����ת��Ϊ�����ֽ���nNum�ֽڻ��������������Ӷ��峤�Ȼ�����
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