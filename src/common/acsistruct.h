/****************************************************************************/
/* Copyright (c) 2010,许继集团有限公司                                      */
/* All rights reserved.                                                     */
/*                                                                          */
/*  模块描述：                                                              */
/** 对象空间操作接口
*  @file  acsistruct.h                                                      
/*                                                                          */
/* 日期         作者    注释                                                */
/* 2010/07/17   LJM     创建文件                                            */
/****************************************************************************/

#ifndef ACSISTRUCT_INCLUDED
#define ACSISTRUCT_INCLUDED

#include "./mms/glbtypes.h"
#include "acsi_const.h"
             

/**
* 所有对象通用结构     
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
* FC及以下对象：fc,do,sdo,fcda
*/
typedef struct acsi_obj_ctrl   
{
	ACSI_COMMON_OBJ  comn_obj;
	ST_INT           sub_obj_num;
	struct acsi_obj_ctrl  *sub_objs;//数组
	AI_VAL_TYPE      type;
	ST_CHAR          *val; //指针，初始化对象空间时为NULL，但报告控制块的属性DdtaSet、GoID及RptID在创建对象空间时申请了内存，并赋值
} ACSI_OBJ_CTRL;

/**
* 数据集
*/
typedef struct
{
	ACSI_COMMON_OBJ  comn_obj;
	ST_INT           member_num;
	ACSI_OBJ_CTRL**  ds_members;//数组，数据集成员
} ACSI_DS_CTRL;

/**
* 各种控制块
*/
typedef struct
{
	ACSI_COMMON_OBJ  comn_obj;
	ST_INT           cb_num;
	ACSI_OBJ_CTRL*   cb_ctrls;//数组
	ACSI_CB_TYPE     cb_type;
} ACSI_CB_CTRL;

/**
* LN
*/
typedef struct
{
	ACSI_COMMON_OBJ  comn_obj;

	ST_INT           sub_obj_num;
	ACSI_OBJ_CTRL*   sub_objs;//数组

	ST_INT           ds_num;
	ACSI_DS_CTRL*    data_sets;//数组

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
	ACSI_LN_CTRL*    lns;//数组
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
	ACSI_LD_CTRL*    lds;//数组
	ACSI_NET_CTRL    ip; //通信配置
} ACSI_IED_CTRL;

/**
*acsi文件属性
*/
typedef struct  
{
	//文件名
	ST_CHAR file_name[MAX_FILE_NAME_LEN+1];
	//文件大小
	ST_INT size_of_file;
	//文件最后修改时间
	ST_CHAR time_last_modified[MAX_FILE_NAME_LEN+1];
}ACSI_FILE_ENTRY;


/**
*fileOpen，fileRead，fileClose，Obtainfile，fileDelete文件句柄
*/
typedef struct
{
	int		frsmid;	/**< 从服务器返回的文件标识，一般是服务器端打开文件得到的文件句柄*/

}ACSI_FILE_READ_IDFP;

////getFile,setFile,deleteFile服务参数
////fileOpen，fileRead，fileClose，Obtainfile，fileDelete服务参数
//typedef struct
//{
//	char	src_fname[MAX_FILE_NAME_LEN+1];		//源文件名称，远方服务器文件名
////	int		init_pos;
//
////	ACSI_FILE_READ_IDFP *hp;								//文件句柄
////	int		frsmid;	/**< 从服务器返回的文件标识，一般是服务器端打开文件得到的文件句柄*/
//
////	int		size_of_file;						//文件大小
////	char	time_last_modified[MAX_TIME_LEN];	//文件最后修改时间
//
//	char	*file_data;									/* pointer to file data octets	*/
////	BOOL	more_follows;								/* additional file content	*/
//
//	//对于setFile
//	char dest_fname[MAX_FILE_NAME_LEN+1];	//目标文件名称，存到本地文件系统中用的名字
//
//}ACSI_FILE_SRV_PARAM_STRUCT;

/**
*getFile,setFile,deleteFile服务参数
*/
typedef struct
{
	ST_CHAR	src_fname[MAX_FILE_NAME_LEN+1];		//源文件名称，远方服务器文件名

	ST_CHAR	*file_data;							/* pointer to file data octets	*/

	ST_CHAR dest_fname[MAX_FILE_NAME_LEN+1];	//对于setFile,目标文件名称，存到本地文件系统中用的名字

}ACSI_FILE_SRV_PARAM_STRUCT;

/** 
//* 通用管理Accosiate，Abort、Release服务参
//*/
//typedef struct 
//{
//	INT32 n_localDetailCall;
//
//}ACSI_COMM_MANAGE_PARAM_STRUCT;

/** 
* 通用管理Accosiate，Abort、Release服务参数结构
*/
typedef struct
{
	ST_CHAR			  source[MAX_NAME_LEN +1];		//服务源地址
	ST_CHAR			  dest[MAX_NAME_LEN +1];		//服务目的地址
	ST_INT			  invoke_id;					//invokeID号，据此进行命令结果匹配
	ACSI_MMSPDU_TYPE  pdu_type;						//pdu类型
	ACSI_SRVC_TYPE    srvc_type;					//acsi类型

	ST_INT            obj_num;						//服务操作对象个数，读服务，写服务等会有多个obj
	ACSI_COMMON_OBJ** obj;							//指针数组
	ACSI_ERR_CODE*    err_code;						//数据访问返回结果，resp时用到

	ACSI_OBJ_CTRL*    rcb;							//报告控制块指针，解析报告类型报文时用到
	ST_CHAR **		  rpt_reasons;					//报告上送原因

	ST_CHAR			  file_directory[MAX_FILE_DIRECTORY_LEN+1];//文件目录request服务的路径
	ST_INT			  file_num;						//文件目录服务response的文件数
	ACSI_FILE_ENTRY*  file_entry;					//文件目录服务response的文件属性数组首指针	

	ACSI_FILE_SRV_PARAM_STRUCT	file_srv_param;		//fileOpen、fileRead、fileClose、ObtainFile、fileDelete服务参数

}AP_ACSI_INFO;

#endif	/* !ACSISTRUCT_INCLUDED	*/
