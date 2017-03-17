/****************************************************************************/
/* Copyright (c) 2010,许继集团有限公司                                      */
/* All rights reserved.                                                     */
/*                                                                          */
/*  模块描述：                                                              */
/** 对象空间操作接口
*  @file  acsi_crscl.h                                                      
/*                                                                          */
/* 日期         作者    注释                                                */
/* 2010/07/17   ZYZ     创建文件                                            */
/****************************************************************************/
#ifndef ACSI_DIR_INCLUDED
#define ACSI_DIR_INCLUDED

#include "glbtypes.h"
#include "sysincs.h"
#include "acsi_crscl.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
* 查找ied
* ip               ipstr            
* 返回值           ied对象指针
*/
ACSI_IED_CTRL * acsi_get_ied_ip(ST_CHAR *ip);

/**
* 查找报告控制块对象指针
* ied            
* rptid            rptid
* 返回值           rcb对象指针
*/
ACSI_OBJ_CTRL * acsi_get_rcb_rptid(ACSI_IED_CTRL *ied, ST_CHAR *rptid);

/**
* 查找ied下某个ds
* ied            
* dsname           ds名称
* 返回值           ds对象指针
*/
ACSI_DS_CTRL * acsi_get_ds_dsname(ACSI_IED_CTRL *ied, ST_CHAR *ldname, ST_CHAR *dsname);

/**
* 获取ld及ld以下成员对象空间中指针
* ied           ied对象
* ref           对象参引
* 返回值        对象指针
*/
ACSI_COMMON_OBJ * acsi_get_obj_ref(ACSI_IED_CTRL *ied, ST_CHAR *ref);

/**
* 获取对象的61850参引
* obj           对象
* ref           对象61850参引
* 返回值        对象指针
*/
ST_RET acsi_get_ref_obj(ACSI_COMMON_OBJ *obj, ST_CHAR *ref);

/**
* 输出对象空间每个成员，测试用
*/
ST_VOID acsi_ied_output_all();
#ifdef __cplusplus
}
#endif

#endif	/* !ACSI_DIR_INCLUDED	*/