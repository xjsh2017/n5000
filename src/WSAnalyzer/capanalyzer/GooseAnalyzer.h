#pragma once
/********************************************************************
*	filename: 	CGooseAnalyzer
*	author:		yinzhehong
*	created:	2011/02/25
*	purpose:  	��̬������࣬����goose���ݷ���      
*   version:    1.0
*********************************************************************/
//��̬�����ݴ�����ָ�붨��
/*��ʼ�����������ö���ռ�ָ���������ر�����ֵ*/
#include "AT_STRUCT.h"
#include "AnalyzeOnline_STRUCT.h"
#include "GOOSEAnalyze_I.h"

typedef  CGSAnalyze_I* (*CREATE_GSA_OBJ)();
typedef  void (*DESTRORY_GSA_OBJ)(CGSAnalyze_I* pGsa_obj);
class CGooseAnalyzer
{
public:
	CGooseAnalyzer(void);
	~CGooseAnalyzer(void);
private:
	//���ض�̬��󷵻صľ��
    XJHANDLE		  m_hdllInst;
	//goose��ʼ���ӿ�
	GOOSE_CFG_STRUCT* m_p_goose_cfg_param;
public:
	/*��ʼ�������ض�̬�⣬�����⺯��,cLibPathFullNameΪ��̬�����·��+��̬������*/
	int Initialize(const char * cLibPathName);
	/*ж�ض�̬��*/
	int UnInitialize();
	/*��ʼ�����������ö���ռ�ָ���������ر�����ֵ*/
	int I_INIT(GOOSE_CFG_STRUCT *p_goose_cfg_param);
private:
	CREATE_GSA_OBJ m_pFun_Create_Gsa_obj;
	DESTRORY_GSA_OBJ m_pFun_Destroy_Gsa_obj;
	CGSAnalyze_I*    m_pgsa_obj;
	GOOSE_SETTING_STRUCT* m_pgoose_setting;
public:
	int I_SET_SNIFFER_APP(SNIFFER_APP* psniffer_app_info);
	/*GOOSE���ķ���*/
	MESSAGE_ERROR_INFO_ARRAY_STRUCT* I_GOOSEANALYZE(GOOSE_INFO_STRUCT *p_goose_infos, int n_num);
	/*�ͷŽṹMESSAGE_ERROR_INFO_ARRAY_STRUCTָ����ڴ�*/
	int I_RELEASEMESSAGEERRS(MESSAGE_ERROR_INFO_ARRAY_STRUCT* p_message_errs);
	/*���ö�ֵ*/
	void I_GOOSEANALYZE_SET_SETTING();
	GOOSE_SETTING_STRUCT* GET_GOOSEANALYZE_SETTING();
};

