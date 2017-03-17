/****************************************************************************/
/* Copyright (c) 2010,��̼������޹�˾                                      */
/* All rights reserved.                                                     */
/*                                                                          */
/*  ģ��������                                                              */
/** ����ռ�����ӿ�
*  @file  acsistruct.h                                                      
/*                                                                          */
/* ����         ����    ע��                                                */
/* 2010/07/17   LJM     �����ļ�                                            */
/****************************************************************************/

#ifndef ACSISTRUCT_INCLUDED
#define ACSISTRUCT_INCLUDED

#include "./mms/glbtypes.h"
#include "acsi_const.h"
             

/**
* ���ж���ͨ�ýṹ     
*/
typedef struct
{
	ACSI_OBJ_TYPE    obj_type;
//	ST_INT         hash_id;
	ST_CHAR        name[MAX_NAME_LEN +1];
	ST_CHAR        *desc;
	ST_VOID		   *parent;
} ACSI_COMMON_OBJ;

/**
* FC�����¶���fc,do,sdo,fcda
*/
typedef struct acsi_obj_ctrl   
{
	ACSI_COMMON_OBJ  comn_obj;
	ST_INT           sub_obj_num;
	struct acsi_obj_ctrl  *sub_objs;//����
	AI_VAL_TYPE      type;
	ST_CHAR          *val; //ָ�룬��ʼ������ռ�ʱΪNULL����������ƿ������DdtaSet��GoID��RptID�ڴ�������ռ�ʱ�������ڴ棬����ֵ
} ACSI_OBJ_CTRL;

/**
* ���ݼ�
*/
typedef struct
{
	ACSI_COMMON_OBJ  comn_obj;
	ST_INT           member_num;
	ACSI_OBJ_CTRL**  ds_members;//���飬���ݼ���Ա
} ACSI_DS_CTRL;

/**
* ���ֿ��ƿ�
*/
typedef struct
{
	ACSI_COMMON_OBJ  comn_obj;
	ST_INT           cb_num;
	ACSI_OBJ_CTRL*   cb_ctrls;//����
	ACSI_CB_TYPE     cb_type;
} ACSI_CB_CTRL;

/**
* LN
*/
typedef struct
{
	ACSI_COMMON_OBJ  comn_obj;

	ST_INT           sub_obj_num;
	ACSI_OBJ_CTRL*   sub_objs;//����

	ST_INT           ds_num;
	ACSI_DS_CTRL*    data_sets;//����

	ST_INT           cb_num;
	ACSI_CB_CTRL*    cbs;
} ACSI_LN_CTRL;

/**
* LD
*/
typedef struct
{
	ACSI_COMMON_OBJ  comn_obj;
	ST_INT           ln_num;
	ACSI_LN_CTRL*    lns;//����
} ACSI_LD_CTRL;

typedef struct
{
	ST_CHAR         ip_a[MAX_IP_LEN +1];
	ST_CHAR         ip_b[MAX_IP_LEN +1];
} ACSI_NET_CTRL;

/**
* IED
*/
typedef struct
{
	ACSI_COMMON_OBJ  comn_obj;
	ST_INT           ld_num;
	ACSI_LD_CTRL*    lds;//����
	ACSI_NET_CTRL    ip; //ͨ������
} ACSI_IED_CTRL;

/**
*acsi�ļ�����
*/
typedef struct  
{
	//�ļ���
	ST_CHAR file_name[MAX_FILE_NAME_LEN+1];
	//�ļ���С
	ST_INT size_of_file;
	//�ļ�����޸�ʱ��
	ST_CHAR time_last_modified[MAX_FILE_NAME_LEN+1];
}ACSI_FILE_ENTRY;


/**
*fileOpen��fileRead��fileClose��Obtainfile��fileDelete�ļ����
*/
typedef struct
{
	int		frsmid;	/**< �ӷ��������ص��ļ���ʶ��һ���Ƿ������˴��ļ��õ����ļ����*/

}ACSI_FILE_READ_IDFP;

////getFile,setFile,deleteFile�������
////fileOpen��fileRead��fileClose��Obtainfile��fileDelete�������
//typedef struct
//{
//	char	src_fname[MAX_FILE_NAME_LEN+1];		//Դ�ļ����ƣ�Զ���������ļ���
////	int		init_pos;
//
////	ACSI_FILE_READ_IDFP *hp;								//�ļ����
////	int		frsmid;	/**< �ӷ��������ص��ļ���ʶ��һ���Ƿ������˴��ļ��õ����ļ����*/
//
////	int		size_of_file;						//�ļ���С
////	char	time_last_modified[MAX_TIME_LEN];	//�ļ�����޸�ʱ��
//
//	char	*file_data;									/* pointer to file data octets	*/
////	BOOL	more_follows;								/* additional file content	*/
//
//	//����setFile
//	char dest_fname[MAX_FILE_NAME_LEN+1];	//Ŀ���ļ����ƣ��浽�����ļ�ϵͳ���õ�����
//
//}ACSI_FILE_SRV_PARAM_STRUCT;

/**
*getFile,setFile,deleteFile�������
*/
typedef struct
{
	ST_CHAR	src_fname[MAX_FILE_NAME_LEN+1];		//Դ�ļ����ƣ�Զ���������ļ���

	ST_CHAR	*file_data;							/* pointer to file data octets	*/

	ST_CHAR dest_fname[MAX_FILE_NAME_LEN+1];	//����setFile,Ŀ���ļ����ƣ��浽�����ļ�ϵͳ���õ�����

}ACSI_FILE_SRV_PARAM_STRUCT;

/** 
//* ͨ�ù���Accosiate��Abort��Release�����
//*/
//typedef struct 
//{
//	INT32 n_localDetailCall;
//
//}ACSI_COMM_MANAGE_PARAM_STRUCT;

/** 
* ͨ�ù���Accosiate��Abort��Release��������ṹ
*/
typedef struct
{
	ST_CHAR			  source[MAX_NAME_LEN +1];		//����Դ��ַ
	ST_CHAR			  dest[MAX_NAME_LEN +1];		//����Ŀ�ĵ�ַ
	ST_INT			  invoke_id;					//invokeID�ţ��ݴ˽���������ƥ��
	ACSI_MMSPDU_TYPE  pdu_type;						//pdu����
	ACSI_SRVC_TYPE    srvc_type;					//acsi����

	ST_INT            obj_num;						//����������������������д����Ȼ��ж��obj
	ACSI_COMMON_OBJ** obj;							//ָ������
	ACSI_ERR_CODE*    err_code;						//���ݷ��ʷ��ؽ����respʱ�õ�

	ACSI_OBJ_CTRL*    rcb;							//������ƿ�ָ�룬�����������ͱ���ʱ�õ�
	ST_CHAR **		  rpt_reasons;					//��������ԭ��

	ST_CHAR			  file_directory[MAX_FILE_DIRECTORY_LEN+1];//�ļ�Ŀ¼request�����·��
	ST_INT			  file_num;						//�ļ�Ŀ¼����response���ļ���
	ACSI_FILE_ENTRY*  file_entry;					//�ļ�Ŀ¼����response���ļ�����������ָ��	

	ACSI_FILE_SRV_PARAM_STRUCT	file_srv_param;		//fileOpen��fileRead��fileClose��ObtainFile��fileDelete�������

}AP_ACSI_INFO;

#endif	/* !ACSISTRUCT_INCLUDED	*/
