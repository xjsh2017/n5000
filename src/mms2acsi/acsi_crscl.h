/****************************************************************************/
/* Copyright (c) 2010,��̼������޹�˾                                      */
/* All rights reserved.                                                     */
/*                                                                          */
/*  ģ��������                                                              */
/** ����scd_info��������ied����ռ�
*  @file  acsi_crscl.h                                                      
/*                                                                          */
/* ����         ����    ע��                                                */
/* 2010/07/17   ZYZ     �����ļ�                                            */
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
* ����scd_info������ied�Ķ���ռ�
*/
ST_RET acsi_create_ied_all(SCD_INFO *scd_info);

/**
* �ͷŶ���ռ�
*/
ST_VOID acsi_ied_destroy_all();

#ifdef __cplusplus
}
#endif

#endif	/* !ACSI_CRSCL_INCLUDED	*/
