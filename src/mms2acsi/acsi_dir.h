/****************************************************************************/
/* Copyright (c) 2010,��̼������޹�˾                                      */
/* All rights reserved.                                                     */
/*                                                                          */
/*  ģ��������                                                              */
/** ����ռ�����ӿ�
*  @file  acsi_crscl.h                                                      
/*                                                                          */
/* ����         ����    ע��                                                */
/* 2010/07/17   ZYZ     �����ļ�                                            */
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
* ����ied
* ip               ipstr            
* ����ֵ           ied����ָ��
*/
ACSI_IED_CTRL * acsi_get_ied_ip(ST_CHAR *ip);

/**
* ���ұ�����ƿ����ָ��
* ied            
* rptid            rptid
* ����ֵ           rcb����ָ��
*/
ACSI_OBJ_CTRL * acsi_get_rcb_rptid(ACSI_IED_CTRL *ied, ST_CHAR *rptid);

/**
* ����ied��ĳ��ds
* ied            
* dsname           ds����
* ����ֵ           ds����ָ��
*/
ACSI_DS_CTRL * acsi_get_ds_dsname(ACSI_IED_CTRL *ied, ST_CHAR *ldname, ST_CHAR *dsname);

/**
* ��ȡld��ld���³�Ա����ռ���ָ��
* ied           ied����
* ref           �������
* ����ֵ        ����ָ��
*/
ACSI_COMMON_OBJ * acsi_get_obj_ref(ACSI_IED_CTRL *ied, ST_CHAR *ref);

/**
* ��ȡ�����61850����
* obj           ����
* ref           ����61850����
* ����ֵ        ����ָ��
*/
ST_RET acsi_get_ref_obj(ACSI_COMMON_OBJ *obj, ST_CHAR *ref);

/**
* �������ռ�ÿ����Ա��������
*/
ST_VOID acsi_ied_output_all();
#ifdef __cplusplus
}
#endif

#endif	/* !ACSI_DIR_INCLUDED	*/