/****************************************************************************/
/* Copyright (c) 2010,��̼������޹�˾                                      */
/* All rights reserved.                                                     */
/*                                                                          */
/*  ģ��������                                                              */
/** mms������acsi����ת���ӿ�
*  @file  mmsstruct.h                                                      
/*                                                                          */
/* ����         ����    ע��                                                */
/* 2010/07/17   LJM     �����ļ�                                            */
/****************************************************************************/
#ifndef MMSSTRUCT_INCLUDED
#define MMSSTRUCT_INCLUDED

#include "../common/mms/glbtypes.h"
#include <stdio.h>
#ifdef OS_WINDOWS
 #include <memory.h>
#else
#include "define_scan.h"
#endif

/** 
* mms������
*/
typedef enum                              
{  
	MMS_SRVC_TYPE_UNKNOWN = 0,

	MMS_SRVC_TYPE_READ,							//read
	MMS_SRVC_TYPE_WRITE,						//write
	MMS_SRVC_TYPE_FILEDIRECTORY,				//fileDirectory
	MMS_SRVC_TYPE_FILEOPEN,						//fileOpen
	MMS_SRVC_TYPE_FILEREAD,						//fileRead
	MMS_SRVC_TYPE_FILECLOSE,					//fileClose
	MMS_SRV_TYPE_OBTAIN_FILE,					//obtainFile
	MMS_SRV_TYPE_FILEDELETE,					//deleteFile
	MMS_SRVC_TYPE_INFO,							//informationReport
	MMS_SRVC_TYPE_INITIATE,						//Initiate
	MMS_SRVC_TYPE_P_ABORT,						//p_abort
	MMS_SRVC_TYPE_CONCLUDE,						//conclude
	MMS_SRVC_TYPE_REJECT,						//reject
	MMS_SRVC_TYPE_CANCEL,						//cancel
	MMS_SRVC_TYPE_GETNAMELIST,					//GetNameList
	MMS_SRVC_TYPE_GETVARACCESSATTR,				//GetVariableAccessAttribute
	MMS_SRVC_TYPE_GETNAMEDVARLSTATTR,			//GetNamedVariableListAttributes
	MMS_SRVC_TYPE_DEFINENAMEDVARLST,			//DefineNamedVariableList
	MMS_SRVC_TYPE_DELETENAMEDVARLST,			//DeleteNamedVariableList
	MMS_SRVC_TYPE_READJOURNAL,					//ReadJournal
	MMS_SRVC_TYPE_U_ABORT						//u_abort

}MMS_SRVC_TYPE;

#define MAX_DES_LEN			   128
#define MAX_CONTENT_LEN		   256

#define MAX_MMS_INFO_DES_LEN	63
#define MAX_MMS_INFO_FILENAME_LEN	255	//MMS�������ļ�����󳤶ȣ���������COSMOS��Ϊ255
#define MAX_MMS_INFO_FILEDIRECTORY_LEN	31	//MMS�������ļ�����󳤶ȣ���������COSMOS��Ϊ255
#define MAX_MMS_INFO_TIME_LEN	31
#define MAX_MMS_INFO_IP_LEN		31
#define MAX_MMS_INFO_DATA_LEN   257//129

#define MAX_BIT_MMS_PARAM_CBB 16
#define MAX_BIT_MMS_SURPPORTED_CALLING 88

typedef struct mms_file_attr_struct
{
	//�ļ���С
	int n_size_of_file;
	//�ļ�����޸�ʱ��
	char c_time_last_modified[MAX_MMS_INFO_TIME_LEN];

}MMS_FILE_ATTR_STRUCT;

//�ļ����Խṹ
typedef struct mms_file_entry{

	//�ļ���
	char c_file_name[MAX_MMS_INFO_FILENAME_LEN+1];

	//�ļ�����
	MMS_FILE_ATTR_STRUCT mms_file_attr;

}MMS_FILE_ENTRY_STRUCT;

//mms��������ö��,ԭ�Ĳμ�mms��һ����14.2���й����͵�˵��
typedef enum {
	n_mdt_array = 1 ,		//1
	n_mdt_structure,		//2
	n_mdt_boolean,			//3
	n_mdt_bit_string,		//4
	n_mdt_integer,			//5
	n_mdt_unsigned,			//6
	n_mdt_flating_poing,	//7
	n_mdt_reserved,			//8
	n_mdt_octet_string,		//9
	n_mdt_visible_string,	//10
	n_mdt_generalized_time,	//11
	n_mdt_binary_time,		//12
	n_mdt_bcd,				//13
	n_mdt_booleanArray,		//14
	n_mdt_obj_id,			//15
	n_mdt_mms_string,		//16
	n_mdt_utc_time,			//17

	n_mdt_init=999			//��ֵ��999
}MMS_DATA_TYPE;


//mms���ݽṹ
typedef struct mms_data_struct{

	//mms�������ͣ�ö��
	MMS_DATA_TYPE n_data_type;
	//mms���ݣ�������ʱʹ��
	char c_data[MAX_MMS_INFO_DATA_LEN];

	//mms���ݽṹ��Ա����
	int n_struct_num;
	//mms���ݽṹ���ͣ�����Ա���ݽṹ
	struct mms_data_struct *p_struct;

	int result;
}MMS_DATA_STRUCT;

//MMS�����ṹ�壬��Ӧ61850��Ĳ���
typedef struct mms_variable_struct{

	// domain_id�ַ���
	char c_domain_id[MAX_MMS_INFO_DES_LEN];
	// item_id�ַ���
	char c_item_id[MAX_MMS_INFO_DES_LEN];
}MMS_VARIABLE_STRUCT;

//fileOpen��fileRead��fileClose��Obtainfile��fileDelete�ļ����
typedef struct mms_file_read_idfp
{
	//�ӷ��������ص��ļ���ʶ��һ���Ƿ������˴��ļ��õ����ļ����
	int		n_frsmid;	
	//���ش򿪵��ļ����
        FILE	*fp;

}MMS_FILE_READ_IDFP;

//fileOpen��fileRead��fileClose��Obtainfile��fileDelete�������
typedef struct mms_file_srv_param_struct
{
	//Դ�ļ����ƣ�Զ���������ļ���
	char c_src_fname[MAX_MMS_INFO_FILENAME_LEN+1];	
	
	int n_init_pos;

	//�ӷ��������ص��ļ���ʶ��һ���Ƿ������˴��ļ��õ����ļ���
	int		n_frsmid;	
	//�ļ�����
	MMS_FILE_ATTR_STRUCT mms_file_attr;

	/* pointer to file data octets	*/
	char *p_file_data;			
	/* additional file content	*/

        unsigned char b_more_follows;

	//����Obtainfile	
	char dest_fname[MAX_MMS_INFO_FILENAME_LEN+1];	//Ŀ���ļ����ƣ��浽�����ļ�ϵͳ���õ�����

}MMS_FILE_SRV_PARAM_STRUCT;

/** 
* DataAccessError����
*/
typedef enum                              
{ 
	MS_DATA_ACCESS_ERR_CODE_OBJECT_INVALIDATED,							//������Ч
	MS_DATA_ACCESS_ERR_CODE_HARDWARE_FAULT,				//Ӳ������
	MS_DATA_ACCESS_ERR_CODE_TEMPORARILY_UNVAILABLE,		//��ʱ���ɷ���
	MS_DATA_ACCESS_ERR_CODE_OBJECT_ACCESS_DENIED,		//������ʾܾ�
	MS_DATA_ACCESS_ERR_CODE_OBJECT_UNDEFINED,							//����û����
	MS_DATA_ACCESS_ERR_CODE_INVALID_ADDRESS,			//��Ч��ַ
	MS_DATA_ACCESS_ERR_CODE_TYPE_UNSUPPORTED,							//���Ͳ�֧��
	MS_DATA_ACCESS_ERR_CODE_TYPE_INCONSISTENT,			//���Ͳ�һ��
	MS_DATA_ACCESS_ERR_CODE_OBJECT_ATTRIBUTE_INCONSISTENT,				//�������Բ�һ��
	MS_DATA_ACCESS_ERR_CODE_OBJECT_ACCESS_UNSUPPORTED,					//������ʲ�֧��
	MS_DATA_ACCESS_ERR_CODE_OBJECT_NON_EXISTENT,		//���󲻴���
	MS_DATA_ACCESS_ERR_CODE_OBJECT_VALUE_INVALID,						//����ֵ��Ч

	MS_DATA_ACCESS_ERR_CODE_NO_ERR = 9999				//�޴���
	
}MMS_DATA_ACCESS_ERR_CODE;

//������ϸ��
typedef struct mms_init_detail_struct
{
	//���н���/����Э�� �汾��
	signed short n_VersionNum;

	//����/Э�� ����
	char c_paramCBB[MAX_BIT_MMS_PARAM_CBB+1];

	char c_addition_paramCBB[MAX_BIT_MMS_PARAM_CBB+1];

	//����/���� ֧�ֵķ���
	char c_supportedCall[MAX_BIT_MMS_SURPPORTED_CALLING+1];

	char c_addition_supportedCall[MAX_BIT_MMS_SURPPORTED_CALLING+1];

}MMS_INIT_DETAIL_STRUCT;

//������ͨ�ù�������ṹ
typedef struct mms_comm_manage_param_struct
{
	//Max_Mms_Pdu_Length
        int n_localDetailCall;

	//����/Э��������з�������Ŀ
        short n_maxOutstandingCalling;

	//����/Э����󱻽з�������Ŀ
        short n_maxOutstandingCalled;

	//
        char n_dataStructNestingLevel;

	//��������/����ϸ��
	MMS_INIT_DETAIL_STRUCT struct_init_Detail;

}MMS_COMM_MANAGE_PARAM_STRUCT;

//�ļ�Ŀ¼��������ṹ
typedef struct mms_file_directory_struct
{
	//�ļ�·��
	char c_file_request_directory[MAX_MMS_INFO_FILEDIRECTORY_LEN+1];

	//�ļ�����
	int n_file_num;			
	//�ļ��ṹ��������ָ��
	MMS_FILE_ENTRY_STRUCT *p_file_entry_struct;	
	//�Ƿ��к���֡
	ST_BOOLEAN b_morefollows;

}MMS_FILE_DIRECTORY_STRUCT;

//MMS����������
typedef enum 
{
	N_WMD_STATE,
	N_APPLICATION_REFERENCE,
	N_DEFINITION,
	N_RESOURCE,
	N_SERVICE,
	N_SERVICE_PREEMPT,
	N_TIME_RESOLUTION,
	N_ACCESS,
	N_INITIATE,
	N_CONCLUDE,
	N_CANCEL,
	N_FILE,
	N_OTHERS,

	N_NO_ERROR_CLASS = 9999,

}MMS_SRV_ERROR_CLASS;

//MMS��Service-Error�ṹ
typedef struct mms_service_error
{
	//����������
	int n_error_class;

	//����������
	int	n_error_code;

	int n_serviceSpecificInfo;

}MMS_SERVICE_ERROR;

//MMS��rejectPdu
typedef enum
{
	REJECT_UNKOWN_PDU = -1,
	REJECT_CONFIRMED_REQUEST_PDU = 1,
	REJECT_CONFIRMED_RESPONSE_PDU,
	REJECT_CONFIRMED_ERROR_PDU,
	REJECT_UNCONFIRMED_PDU,
	REJECT_PDU_ERROR,
	REJECT_CANCEL_REQUEST_PDU,
	REJECT_CANCEL_RESPONSE_PDU,
	REJECT_CANCEL_ERROR_PDU,
	REJECT_CANCLUDE_REQUEST_PDU,
	REJECT_CANCLUDE_RESPONSE_PDU,
	REJECT_CANCLUDE_ERROR_PDU

}REJECT_PDU;

//MMS��reject����ṹ
typedef struct mms_service_reject
{
	int n_originalInvoikeID;

	//pdu
	REJECT_PDU n_reject_pdu;

	//�ܾ�ԭ�����
	int	n_reject_reason_code; 

}MMS_SERVICE_REJECT;

//MMS������Ϣ�ṹ��
typedef struct mms_info_struct{
	
	//Դ��ַip
	char c_src_ip[MAX_MMS_INFO_IP_LEN];
	//Ŀ�ĵ�ַip
	char c_dest_ip[MAX_MMS_INFO_IP_LEN];

	//invokeID�ţ��ݴ˽���������ƥ��
	int n_invoke_id;						
	
	//pdu���ͣ�Request����Response
	char c_pdu_type[MAX_MMS_INFO_DES_LEN];	

	//mms��������
	char c_service_type[MAX_MMS_INFO_DES_LEN];
	
	//��������
	int n_variable_num;	
	//�����ṹ��������ָ��
	MMS_VARIABLE_STRUCT *p_variable_struct;	

	//data������������ֵ/ֵ�Ľṹ�壩
	int n_data_num;		
	//data�ṹ��ԭ���ݽṹָ�룬�������ݸ��ݷ���������������
	MMS_DATA_STRUCT *p_data_struct;			

	//�ļ�Ŀ¼��������ṹ
	MMS_FILE_DIRECTORY_STRUCT mms_file_directory;

	//�ļ���������ṹ
	MMS_FILE_SRV_PARAM_STRUCT struct_file_srv_param;		

	//ͨ�ù������initiate��conclude��abort�����ṹ
	MMS_COMM_MANAGE_PARAM_STRUCT struct_comm_manage_param;	//mmslite��������mvla_initiate_req

	//service-error
	MMS_SERVICE_ERROR struct_mms_service_error;

	//reject����
	MMS_SERVICE_REJECT struct_mms_service_reject;


}MMS_INFO_STRUCT;

/**********************************************���� 20121109**************************************/

/** 
* mms������
*/
typedef enum                              
{  
	MMS_DETAIL_SRVC_TYPE_UNKNOWN = 9999,
		
	MMS_DETAIL_SRVC_TYPE_READ_REQ,							//read request
	MMS_DETAIL_SRVC_TYPE_READ_RESP,							//read response

	MMS_DETAIL_SRVC_TYPE_WRITE_REQ,							//write request
	MMS_DETAIL_SRVC_TYPE_WRITE_RESP,						//write response

	MMS_DETAIL_SRVC_TYPE_FILEDIRECTORY_REQ,					//fileDirectory request
	MMS_DETAIL_SRVC_TYPE_FILEDIRECTORY_RESP,				//fileDirectory response

	MMS_DETAIL_SRVC_TYPE_FILEOPEN_REQ,						//fileOpen request
	MMS_DETAIL_SRVC_TYPE_FILEOPEN_RESP,						//fileOpen response

	MMS_DETAIL_SRVC_TYPE_FILEREAD_REQ,						//fileRead request
	MMS_DETAIL_SRVC_TYPE_FILEREAD_RESP,						//fileRead response

	MMS_DETAIL_SRVC_TYPE_FILECLOSE_REQ,						//fileClose request
	MMS_DETAIL_SRVC_TYPE_FILECLOSE_RESP,					//fileClose response

	MMS_SRV_TYPE_OBTAIN_FILE_REQ,							//obtainFile request
	MMS_SRV_TYPE_OBTAIN_FILE_RESP,							//obtainFile response

	MMS_SRV_TYPE_FILEDELETE_REQ,							//deleteFile request
	MMS_SRV_TYPE_FILEDELETE_RESP,							//deleteFile response

	MMS_DETAIL_SRVC_TYPE_INFO,								//informationReport

	MMS_DETAIL_SRVC_TYPE_INITIATE_REQ,						//Initiate request
	MMS_DETAIL_SRVC_TYPE_INITIATE_RESP,						//Initiate response

	MMS_DETAIL_SRVC_TYPE_P_ABORT_REQ,						//p_abort request
	MMS_DETAIL_SRVC_TYPE_P_ABORT_RESP,						//p_abort response

	MMS_DETAIL_SRVC_TYPE_CONCLUDE_REQ,						//conclude request
	MMS_DETAIL_SRVC_TYPE_CONCLUDE_RESP,						//conclude response

	MMS_DETAIL_SRVC_TYPE_REJECT,							//reject

	MMS_DETAIL_SRVC_TYPE_CANCEL_REQ,						//cancel request
	MMS_DETAIL_SRVC_TYPE_CANCEL_RESP,						//cancel response

	MMS_DETAIL_SRVC_TYPE_GETNAMELIST_REQ,					//GetNameList request
	MMS_DETAIL_SRVC_TYPE_GETNAMELIST_RESP,					//GetNameList response

	MMS_DETAIL_SRVC_TYPE_GETVARACCESSATTR_REQ,				//GetVariableAccessAttribute request
	MMS_DETAIL_SRVC_TYPE_GETVARACCESSATTR_RESP,				//GetVariableAccessAttribute response

	MMS_DETAIL_SRVC_TYPE_GETNAMEDVARLSTATTR_REQ,			//GetNamedVariableListAttributes request
	MMS_DETAIL_SRVC_TYPE_GETNAMEDVARLSTATTR_RESP,			//GetNamedVariableListAttributes response

	MMS_DETAIL_SRVC_TYPE_DEFINENAMEDVARLST_REQ,				//DefineNamedVariableList request
	MMS_DETAIL_SRVC_TYPE_DEFINENAMEDVARLST_RESP,			//DefineNamedVariableList response

	MMS_DETAIL_SRVC_TYPE_DELETENAMEDVARLST_REQ,				//DeleteNamedVariableList request
	MMS_DETAIL_SRVC_TYPE_DELETENAMEDVARLST_RESP,			//DeleteNamedVariableList response

	MMS_DETAIL_SRVC_TYPE_READJOURNAL_REQ,					//ReadJournal request
	MMS_DETAIL_SRVC_TYPE_READJOURNAL_RESP,					//ReadJournal response

	MMS_DETAIL_SRVC_TYPE_U_ABORT_REQ,						//u_abort request
	MMS_DETAIL_SRVC_TYPE_U_ABORT_RESP						//u_abort response
		
}MMS_DETAIL_SRVC_TYPE;


#endif	/* !MMSSTRUCT_INCLUDED	*/
