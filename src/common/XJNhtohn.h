//
// String.h
//
// This is a string operator  class
//
// Copyright (C) 2006 Jiangjianlin
// Version 1.0
//


#ifndef XJNhtohn_h__
#define XJNhtohn_h__

#include "define_scan.h"
#include <stdint.h>
using namespace std;
extern bool bNetEndian;
void ReverseBytes(void *vbuf,int nNum);//ԭλת�����㷺������֪����ת���ֽ���
//host to net,also net to host,�����������ֽ�����
void ntohBytes(void *vbuf,int nNum);//ԭλת���������͵������ֽ���
#define htonBytes ntohBytes
uint64_t ntohXJ(unsigned char *vbuf,int nNum);//�����ֽ���õ�int�ͣ��������Զ��峤������
void htonXJ(unsigned char *vbuf,int nNum,uint64_t nVal);//����ת��Ϊ�����ֽ���nNum�ֽڻ��������������Ӷ��峤�Ȼ�����

#endif // XJNhtohn_h__
