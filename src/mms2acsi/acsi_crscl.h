/****************************************************************************/
/* Copyright (c) 2010,许继集团有限公司                                      */
/* All rights reserved.                                                     */
/*                                                                          */
/*  模块描述：                                                              */
/** 根据scd_info创建所有ied对象空间
*  @file  acsi_crscl.h                                                      
/*                                                                          */
/* 日期         作者    注释                                                */
/* 2010/07/17   ZYZ     创建文件                                            */
/****************************************************************************/
#ifndef ACSI_CRSCL_INCLUDED
#define ACSI_CRSCL_INCLUDED

#include "../common/mms/glbtypes.h"
#include "../common/mms/sysincs.h"
#include "../common/mms/scl.h"
#include "acsistruct.h"


#ifdef __cplusplus
extern "C" {
#endif


/**
* 创建scd_info中所有ied的对象空间
*/
ST_RET acsi_create_ied_all(SCD_INFO *scd_info);

/**
* 释放对象空间
*/
ST_VOID acsi_ied_destroy_all();

#ifdef __cplusplus
}
#endif

#endif	/* !ACSI_CRSCL_INCLUDED	*/
