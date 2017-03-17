#pragma once
/********************************************************************
*	filename: 	CSmvAnalyzer
*	author:		yinzhehong
*	created:	2011/02/25
*	purpose:  	��̬������࣬����ֵ����
*   version:    1.0
*********************************************************************/

//��̬�����ݴ�����ָ�붨��
/*��ʼ�����������ö���ռ�ָ���������ر�����ֵ*/
#include "AT_STRUCT.h"
#include "AnalyzeOnline_STRUCT.h"
#include "SMVAnalyze_I.h"

typedef  CSMVAnalyze_I* (*CREATE_SMVA_OBJ)();
typedef  void (*DESTRORY_SMV_OBJ)(CSMVAnalyze_I* pSmva_obj);

class CSmvAnalyzer
{
public:
	CSmvAnalyzer(void);
	~CSmvAnalyzer(void);
private:
	//���ض�̬��󷵻صľ��
    XJHANDLE		  m_hdllInst;
	//smv��ʼ���ӿ�
	SMV_CFG_STRUCT*   m_pCfgInfo;
private:
	CREATE_SMVA_OBJ m_pFun_Create_Smva_obj;
	DESTRORY_SMV_OBJ m_pFun_Destroy_Smva_obj;
	CSMVAnalyze_I*    m_psmva_obj;
public:
	SMV_SETTING_STRUCT*   m_psmv_setting;
private:
	/*��ʼ�����������ö���ռ�ָ���������ر�����ֵ*/
	int I_INIT(SMV_CFG_STRUCT *pCfgInfo);
public:
	/*��ʼ�������ض�̬�⣬�����⺯��,cLibPathFullNameΪ��̬�����·��+��̬������*/
    int Initialize(const char * cLibPathName);
	/*ж�ض�̬��*/
	int UnInitialize();
public:
	int I_SET_SNIFFER_APP(SNIFFER_APP* psniffer_app_info);
	/*SMV���ķ���*/
	MESSAGE_ERROR_INFO_ARRAY_STRUCT* I_SMVANALYZE(SMV_INFO_STRUCT *p_smv_infos, int n_num);
	/*�ͷŽṹMESSAGE_ERROR_INFO_ARRAY_STRUCTָ����ڴ�*/
	int I_RELEASEMESSAGEERRS(MESSAGE_ERROR_INFO_ARRAY_STRUCT* p_message_errs);
	/*���ö�ֵ*/
	void I_SMVANALYZE_SET_SETTING();
	SMV_SETTING_STRUCT* GET_SMVANALYZE_SETTING();
};

