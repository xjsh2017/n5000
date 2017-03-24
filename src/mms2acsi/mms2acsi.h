/****************************************************************************/
/* Copyright (c) 2010,许继集团有限公司                                      */
/* All rights reserved.                                                     */
/*                                                                          */
/*  模块描述：                                                              */
/** mms服务与acsi服务转换接口
*  @file  mms2acsi.h                                                      
/*                                                                          */
/* 日期         作者    注释                                                */
/* 2010/07/17   ZYZ     创建文件                                            */
/****************************************************************************/
#ifndef MMS2ACSI_INCLUDED
#define MMS2ACSI_INCLUDED

#include "../common/mms/glbtypes.h"
#include "../common/mms/sysincs.h"
#include "acsi_crscl.h"
#include "acsistruct.h"
#include "mmsstruct.h"

#ifdef OS_WINDOWS

#ifdef DLL_FILE_MMS2ACSI

#ifdef __cplusplus
extern "C" {
#endif

__declspec( dllexport )	ST_RET acsi_init_envir(ST_CHAR *scd_name);
__declspec( dllexport )	AP_ACSI_INFO* acsi_cre_info_mmsInfo(MMS_INFO_STRUCT *mms_info, ST_INT mms_num);
__declspec( dllexport )	ST_VOID acsi_clean_acsi_info(AP_ACSI_INFO* acsi_infos, ST_INT num);
__declspec( dllexport )	ST_RET acsi_uninit_envir();

#ifdef __cplusplus
}
#endif

#else

#ifdef __cplusplus
extern "C" {
#endif

__declspec( dllimport )	ST_RET acsi_init_envir(ST_CHAR *scd_name);
__declspec( dllimport )	AP_ACSI_INFO* acsi_cre_info_mmsInfo(MMS_INFO_STRUCT *mms_info, ST_INT mms_num);
__declspec( dllimport )	ST_VOID acsi_clean_acsi_info(AP_ACSI_INFO* acsi_infos, ST_INT num);
__declspec( dllimport )	ST_RET acsi_uninit_envir();

#ifdef __cplusplus
}
#endif

#endif//DLL_FILE_MMS2ACSI

#endif//OS_WINDOWS

#ifdef OS_LINUX

	/**
	* @brief	function name:"acsi_init_envir"
	* @use		初始化环境
	* @param 	[type] ST_CHAR [name] scd_name SCD文件名称
	* @return	0表示成功，1表示失败
	*/
	ST_RET acsi_init_envir(ST_CHAR *scd_name);

	/**
	* @brief	function name:"acsi_cre_info_mmsInfo"
	* @use		查找报告控制块对象指针
	* @param 	[type] MMS_INFO_STRUCT [name] mms_info MMS信息结构数组
	* @param 	[type] int [name] mms_num MMS信息结构数组大小
	* @return	acsi数组，个数与传进的mms_num一致
	*/
	AP_ACSI_INFO* acsi_cre_info_mmsInfo(MMS_INFO_STRUCT *mms_info, ST_INT mms_num);

	/**
	* 释放acsi_infos信息
	*/
	/**
	* @brief	function name:"acsi_clean_acsi_info"
	* @use		释放acsi_infos信息
	* @param 	[type] AP_ACSI_INFO [name] acsi_infos ACSI信息结构数组
	* @param 	[type] int [name] num ACSI信息结构数组大小
	* @return	void
	*/
	ST_VOID acsi_clean_acsi_info(AP_ACSI_INFO* acsi_infos, ST_INT num);

	/**
	* @brief	function name:"acsi_uninit_envir"
	* @use		初始化环境
	* @param 	无
	* @return	0表示成功，1表示失败
	*/
	ST_RET acsi_uninit_envir();


#endif //OS_LINUX


#endif	/* !MMS2ACSI_INCLUDED	*/