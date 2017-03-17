#pragma once
/********************************************************************
*	filename: 	CGooseAnalyzer
*	author:		yinzhehong
*	created:	2011/02/25
*	purpose:  	动态库操作类，用于goose数据分析      
*   version:    1.0
*********************************************************************/
//动态库数据处理函数指针定义
/*初始化函数，设置对象空间指针和其它相关变量的值*/
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
	//加载动态库后返回的句柄
    XJHANDLE		  m_hdllInst;
	//goose初始化接口
	GOOSE_CFG_STRUCT* m_p_goose_cfg_param;
public:
	/*初始化，加载动态库，导出库函数,cLibPathFullName为动态库相对路径+动态库名称*/
	int Initialize(const char * cLibPathName);
	/*卸载动态库*/
	int UnInitialize();
	/*初始化函数，设置对象空间指针和其它相关变量的值*/
	int I_INIT(GOOSE_CFG_STRUCT *p_goose_cfg_param);
private:
	CREATE_GSA_OBJ m_pFun_Create_Gsa_obj;
	DESTRORY_GSA_OBJ m_pFun_Destroy_Gsa_obj;
	CGSAnalyze_I*    m_pgsa_obj;
	GOOSE_SETTING_STRUCT* m_pgoose_setting;
public:
	int I_SET_SNIFFER_APP(SNIFFER_APP* psniffer_app_info);
	/*GOOSE报文分析*/
	MESSAGE_ERROR_INFO_ARRAY_STRUCT* I_GOOSEANALYZE(GOOSE_INFO_STRUCT *p_goose_infos, int n_num);
	/*释放结构MESSAGE_ERROR_INFO_ARRAY_STRUCT指针的内存*/
	int I_RELEASEMESSAGEERRS(MESSAGE_ERROR_INFO_ARRAY_STRUCT* p_message_errs);
	/*设置定值*/
	void I_GOOSEANALYZE_SET_SETTING();
	GOOSE_SETTING_STRUCT* GET_GOOSEANALYZE_SETTING();
};

