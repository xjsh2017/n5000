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
void ReverseBytes(void *vbuf,int nNum);//原位转换，广泛适用已知必须转换字节序。
//host to net,also net to host,易用于整数字节类型
void ntohBytes(void *vbuf,int nNum);//原位转换主机类型到网络字节序。
#define htonBytes ntohBytes
uint64_t ntohXJ(unsigned char *vbuf,int nNum);//网络字节序得到int型，适用于自定义长度整数
void htonXJ(unsigned char *vbuf,int nNum,uint64_t nVal);//整数转换为网络字节序nNum字节缓冲区，适用于子定义长度缓冲区

#endif // XJNhtohn_h__
