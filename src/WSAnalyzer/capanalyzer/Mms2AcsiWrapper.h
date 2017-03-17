#pragma once

/********************************************************************
*	filename: 	CMms2AcsiWrapper
*	author:		yinzhehong
*	created:	2011/01/07
*	purpose:  	静态库操作类，用于使用mms2acsi静态库的封装使用      
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
	//加载动态库后返回的句柄
    XJHANDLE					m_hMms2acisInst;
public:
	/*初始化，加载动态库，导出库函数,cLibPathFullName为动态库相对路径+动态库名称*/
	int Initialize(const char * cLibPathName);
	/*卸载动态库*/
	int UnInitialize();
public:
	/*初始化SCD文件接口*/
	ST_RET         I_ACSI_INIT_ENVIR(ST_CHAR *scd_name);
	/*查找报告控制块对象*/
	AP_ACSI_INFO*  I_ACSI_CRE_INFO_MMSINFO(MMS_INFO_STRUCT *mms_info, ST_INT mms_num);
	/*释放acsi_infos信息*/
	ST_VOID        I_ACSI_CLEAN_ACSI_INFO(AP_ACSI_INFO* acsi_infos, ST_INT num);
	/*释放环境*/
	ST_RET         I_ACSI_UNINIT_ENVIR();
private:
	bool   m_bScdFileLoaded;//scd文件已经加载
public:
	/*SCD文件是否加载*/
	bool   IsScdFileLoaded();
};

