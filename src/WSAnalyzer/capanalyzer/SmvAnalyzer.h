#pragma once
/********************************************************************
*	filename: 	CSmvAnalyzer
*	author:		yinzhehong
*	created:	2011/02/25
*	purpose:  	动态库操作类，采样值分析
*   version:    1.0
*********************************************************************/

//动态库数据处理函数指针定义
/*初始化函数，设置对象空间指针和其它相关变量的值*/
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
	//加载动态库后返回的句柄
    XJHANDLE		  m_hdllInst;
	//smv初始化接口
	SMV_CFG_STRUCT*   m_pCfgInfo;
private:
	CREATE_SMVA_OBJ m_pFun_Create_Smva_obj;
	DESTRORY_SMV_OBJ m_pFun_Destroy_Smva_obj;
	CSMVAnalyze_I*    m_psmva_obj;
public:
	SMV_SETTING_STRUCT*   m_psmv_setting;
private:
	/*初始化函数，设置对象空间指针和其它相关变量的值*/
	int I_INIT(SMV_CFG_STRUCT *pCfgInfo);
public:
	/*初始化，加载动态库，导出库函数,cLibPathFullName为动态库相对路径+动态库名称*/
    int Initialize(const char * cLibPathName);
	/*卸载动态库*/
	int UnInitialize();
public:
	int I_SET_SNIFFER_APP(SNIFFER_APP* psniffer_app_info);
	/*SMV报文分析*/
	MESSAGE_ERROR_INFO_ARRAY_STRUCT* I_SMVANALYZE(SMV_INFO_STRUCT *p_smv_infos, int n_num);
	/*释放结构MESSAGE_ERROR_INFO_ARRAY_STRUCT指针的内存*/
	int I_RELEASEMESSAGEERRS(MESSAGE_ERROR_INFO_ARRAY_STRUCT* p_message_errs);
	/*设置定值*/
	void I_SMVANALYZE_SET_SETTING();
	SMV_SETTING_STRUCT* GET_SMVANALYZE_SETTING();
};

