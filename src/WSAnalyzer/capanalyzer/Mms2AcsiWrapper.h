#pragma once

/********************************************************************
*	filename: 	CMms2AcsiWrapper
*	author:		yinzhehong
*	created:	2011/01/07
*	purpose:  	��̬������࣬����ʹ��mms2acsi��̬��ķ�װʹ��      
*   version:    1.0
*********************************************************************/
#include "acsistruct.h"
#include "mmsstruct.h"
#include "xjlib.h"

class CMms2AcsiWrapper
{
public:
	CMms2AcsiWrapper(void);
	~CMms2AcsiWrapper(void);
private:
	//���ض�̬��󷵻صľ��
    XJHANDLE					m_hMms2acisInst;
public:
	/*��ʼ�������ض�̬�⣬�����⺯��,cLibPathFullNameΪ��̬�����·��+��̬������*/
	int Initialize(const char * cLibPathName);
	/*ж�ض�̬��*/
	int UnInitialize();
public:
	/*��ʼ��SCD�ļ��ӿ�*/
	ST_RET         I_ACSI_INIT_ENVIR(ST_CHAR *scd_name);
	/*���ұ�����ƿ����*/
	AP_ACSI_INFO*  I_ACSI_CRE_INFO_MMSINFO(MMS_INFO_STRUCT *mms_info, ST_INT mms_num);
	/*�ͷ�acsi_infos��Ϣ*/
	ST_VOID        I_ACSI_CLEAN_ACSI_INFO(AP_ACSI_INFO* acsi_infos, ST_INT num);
	/*�ͷŻ���*/
	ST_RET         I_ACSI_UNINIT_ENVIR();
private:
	bool   m_bScdFileLoaded;//scd�ļ��Ѿ�����
public:
	/*SCD�ļ��Ƿ����*/
	bool   IsScdFileLoaded();
};

