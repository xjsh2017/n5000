#ifndef ACSI_CONST_INCLUDE
#define ACSI_CONST_INCLUDE


#define MAX_NAME_LEN 32 //�������Ƴ���
#define MAX_IP_LEN 15   //ip��ַ����
#define MAX_FILE_DIRECTORY_LEN 31	//�ļ�·������
#define MAX_FILE_NAME_LEN 255	//�ļ�������
#define MAX_TIME_LEN	31	//�ļ�����޸�ʱ�䳤��

#define SRV_RESULT_SUCCESS	9999//�������ɹ�

/** 
* ���津��ѡ��壬�ͱ������ݸ�ʽ��trgopֵһ�£�����Ϊһ���ֽ�
*/
#define     ACSI_RPT_REASON_RESERVED  0x80
#define     ACSI_RPT_REASON_DCH  0x40
#define     ACSI_RPT_REASON_QCH  0x20
#define     ACSI_RPT_REASON_DUPD  0x10
#define     ACSI_RPT_REASON_INTGPD  0x08
#define     ACSI_RPT_REASON_GI  0x04
#define     ACSI_RPT_REASON_UNKNOWN 0x02

/**                                            
* 61850��������                                
*/                                       
typedef enum                              
{  
	OBJECT_TYPE_UNKNOWN = 0,
	OBJECT_TYPE_IED,
	OBJECT_TYPE_LD,
	OBJECT_TYPE_LN,
	OBJECT_TYPE_DS,
	OBJECT_TYPE_FCN, //��fc��ln
	OBJECT_TYPE_FCD, 
	OBJECT_TYPE_FCDA,
	OBJECT_TYPE_CB  //���ƿ�
}ACSI_OBJ_TYPE;

/**
* 61850����������     
*/
typedef enum{
	VAL_TYPE_UNKNOWN = 0,
	VAL_TYPE_BOOLEAN,   
	VAL_TYPE_ENUM,
	VAL_TYPE_INT8,
	VAL_TYPE_INT16,
	VAL_TYPE_INT32,
	VAL_TYPE_INT128,
	VAL_TYPE_INT8U,
	VAL_TYPE_INT16U,
	VAL_TYPE_INT32U,
	VAL_TYPE_FLOAT32,
	VAL_TYPE_FLOAT64, 
	VAL_TYPE_CODEDENUM,     //�̶����ȵ�bitstring, 2bit
	VAL_TYPE_TCMD,			//�̶����ȵ�bitstring, 2bit
	VAL_TYPE_OSTRING6,      //gocb$DstAddress$Addr
	VAL_TYPE_OSTRING8,      //�䳤octet string,��󳤶�8���ֽ�
	VAL_TYPE_OSTRING64,     //�䳤octet string,��󳤶�64���ֽ�
	VAL_TYPE_VSTRING64,     //���ӱ䳤�ַ�������󳤶�64
	VAL_TYPE_VSTRING65,     //���ӱ䳤�ַ�������󳤶�65
	VAL_TYPE_VSTRING129,     //���ӱ䳤�ַ�������󳤶�129
	VAL_TYPE_VSTRING255	,	//���ӱ䳤�ַ�������󳤶�255
	VAL_TYPE_UNISTRING255,  //utf-8�ַ��������255���ַ�
	
	VAL_TYPE_QUALITY,		//�䳤bitstring 13bit
	VAL_TYPE_TIMESTAMP,		//utctime
	VAL_TYPE_ENTRYTIME,		//binary time��btime6��ʽ
	VAL_TYPE_OPTFLDS,		//�䳤bitstring 10bit
	VAL_TYPE_TRGC,			//�䳤bitstring 6bit
	VAL_TYPE_CHECK,			//�䳤bitstring 2bit
	VAL_TYPE_ENTRYID,		//�̶����ȵ�octet string, 8���ֽ�
	VAL_TYPE_STRUCT			//�������͵�����
}AI_VAL_TYPE;

/**
* ���ƿ�����     
*/
typedef enum                  
{  
	CB_TYPE_UNKNOWN = 0,
	CB_TYPE_SG,             
	CB_TYPE_RP,
	CB_TYPE_BR,
	CB_TYPE_LOG,
	CB_TYPE_GO
}ACSI_CB_TYPE;

/** 
* mmspdu���Ͷ���
*/
typedef enum                              
{  
	ACSI_MMSPDU_UNKNOWN = 0,
	ACSI_MMSPDU_REQ ,
	ACSI_MMSPDU_RESP,
	ACSI_MMSPDU_RPT,
	ACSI_MMSPDU_SRVERROR,
	ACSI_MMSPDU_REJECT
}ACSI_MMSPDU_TYPE;

/** 
* acsi������
*/
//typedef enum                              
//{  
//	ACSI_SRVC_TYPE_UNKNOWN = 0,
//	ACSI_SRVC_TYPE_GETSERVERDIRECTORY,
//	ACSI_SRVC_TYPE_ASSOCIATE,
//	ACSI_SRVC_TYPE_RELEASE,
//	ACSI_SRVC_TYPE_ABORT,
//	ACSI_SRVC_TYPE_GETDATAVAL,
//	ACSI_SRVC_TYPE_SETDATAVALUES,
//	ACSI_SRVC_TYPE_REPORT,
//	ACSI_SRVC_TYPE_GETFILEDIRECTORY,
//	ACSI_SRVC_TYPE_GETFILE
//}ACSI_SRVC_TYPE;
typedef enum                              
{  
	ACSI_SRVC_TYPE_UNKNOWN = 0,
	ACSI_SRVC_TYPE_GETSERVERDIR,				//GetServerDirectory
	ACSI_SRVC_TYPE_ASSOCIATE,					//����Associate
	ACSI_SRVC_TYPE_RELEASE,						//����Release
	ACSI_SRVC_TYPE_ABORT,						//�쳣��ֹAbort

	ACSI_SRVC_TYPE_GETLOGICALDEVICEDIR,			//GetLogicalDeviceDirectory

	ACSI_SRVC_TYPE_GETLOGICALNODEDIR,			//GetLogicalNodeDirectory
	ACSI_SRVC_TYPE_GETALLDATAVALS,				//GetAllDataValues
	
	ACSI_SRVC_TYPE_GETDATAVALS,					//GetDataValues
	ACSI_SRVC_TYPE_SETDATAVALS,					//SetDataValues
	ACSI_SRVC_TYPE_GETDATADIR,					//GetDataDirectory
	ACSI_SRVC_TYPE_GETDATADEF,					//GetDataDefinition

	ACSI_SRVC_TYPE_REPORT,						//����Reort
	ACSI_SRVC_TYPE_GETBRCBVALS,					//GetBRCBValues
	ACSI_SRVC_TYPE_SETBRCBVALS,					//SetBRCBValues
	ACSI_SRVC_TYPE_GETURCBVALS,					//GetURCBValues
	ACSI_SRVC_TYPE_SETURCBVALS,					//SetURCBValues

	ACSI_SRVC_TYPE_GETFILEDIRECTORY,			//GetFileDirectory
	ACSI_SRVC_TYPE_GETFILE_OPEN,				//GetFile_openfile
	ACSI_SRVC_TYPE_SETFILE,						//SetFile
	ACSI_SRVC_TYPE_DELETEFILE,					//DeleteFile

	ACSI_SRVC_TYPE_GETSGCBVALS,					//����ֵ���ƿ��ֵGetSGCBValues
	ACSI_SRVC_TYPE_GETSGVALS,					//�ٻ���ֵGetSGValues
	ACSI_SRVC_TYPE_SETSGVALS,					//�޸Ķ�ֵSetSGValues
	ACSI_SRVC_TYPE_CONFIRMEDITSGVALS,			//ȷ���޸Ķ�ֵConfirmEditSGValues
	ACSI_SRVC_TYPE_SELECTACTSG,					//�л�������SelectActSG
	ACSI_SRVC_TYPE_SELECTEDITSG,				//�л��༭��SelectEditSG

	ACSI_SRVC_TYPE_GETDSVALS,					//GetDataSetValues
	ACSI_SRVC_TYPE_SETDSVALS,					//SetDataSetValues
	ACSI_SRVC_TYPE_CREATEDS,					//CreateDataSet
	ACSI_SRVC_TYPE_DELETEDS,					//DeleteDataSet
	ACSI_SRVC_TYPE_GETDSDIR,					//GetDataSetDirectory

	ACSI_SRVC_TYPE_SELECT,						//Select
	ACSI_SRVC_TYPE_SELECTWITHVAL,				//SelectWithValue
	ACSI_SRVC_TYPE_OPERATE,						//Operate
	ACSI_SRVC_TYPE_CANCEL,						//Cancel

	ACSI_SRVC_TYPE_GETGOCBVALS,					//GetGoCBValues
	ACSI_SRVC_TYPE_SETGOCBVALS,					//SetGoCBValues
	ACSI_SRVC_TYPE_GETGSCBVALS,					//GetGsCBValues
	ACSI_SRVC_TYPE_SETGSCBVALS,					//SetGsCBValues

	ACSI_SRVC_TYPE_GETMSVCBVALS,				//GetMSVCBValues
	ACSI_SRVC_TYPE_SETMSVCBVALS,				//SetMSVCBValues
	ACSI_SRVC_TYPE_GETUSVCBVALS,				//GetUSVCBValues
	ACSI_SRVC_TYPE_SETUSVCBVALS,				//SetUSVCBValues

	ACSI_SRVC_TYPE_GETLOGSTATUSVALS,			//GetLogStatusValues
	ACSI_SRVC_TYPE_QUERYLOGBYTIME,				//QueryLogByTime
	ACSI_SRVC_TYPE_QUERYLOGAFTER,				//QueryLogAfter

	ACSI_SRVC_TYPE_GETFILE_READ,				//GetFile_readfile
	ACSI_SRVC_TYPE_GETFILE_CLOSE,				//GetFile_closefile

	ACSI_SRVC_TYPE_GETFILEATTR					//GetFileAttributes

}ACSI_SRVC_TYPE;

/** 
* acsi������
*/
typedef enum                              
{  
	ACSI_ERR_CODE_UNKNOWN = -1,

	ACSI_ERR_CODE_INSTANCE_NOT_AVAILABLE,	//ʵ��������
	ACSI_ERR_CODE_INSTANCE_IN_USE,			//ʵ����ʹ��
	ACSI_ERR_CODE_ACCESS_VIOLATION,			//����Υ��
	ACSI_ERR_CODE_ACCESS_NOT_ALLOWED_IN_CURENT_STATE,//�ڵ�ǰ״̬�²��������
	ACSI_ERR_CODE_PARAMETER_VALUE_INAPPROPRIATE,//����ֵ������
	ACSI_ERR_CODE_PARAMETER_VALUE_INCONSISTENT,//����ֵ��һ��
	ACSI_ERR_CODE_CLASS_NOT_SURPPORTED,//�಻֧��
	ACSI_ERR_CODE_INSTANCE_LOCKED_BY_OTHER_CLIENT,
	ACSI_ERR_CODE_CONTROL_MUST_BE_SELECTED,
	ACSI_ERR_CODE_TYPE_CONFILICT,
	ACSI_ERR_CODE_FAILED_DUE_TO_COMMUNICATIONS_CONSTRAINT,
	ACSI_ERR_CODE_FAILED_DUE_TO_SERVER_CONSTRAINT,

	ACSI_ERR_CODE_NO_ERR = 9999 //�޴���
	
}ACSI_ERR_CODE;

///**
//*����������
//*/
//typedef enum
//{
//	//n_vmd_other = 0,
//	//n_vmd_state_confilict,
//	//n_vmd_operational_problem,
//	//n_vdm_domain_transfer_problem,
//	//n_vmd_state_machine_invalid,
//
//	//n_application_reference_other = 10,
//	//n_application_unreachable,
//	//n_application_connection_lost,
//	//n_application_context_unsupported,
//
//	//n_definition_other = 20,
//	//n_definition_object_undefined,
//	//n_definition_invalid_address,
//	//n_definition_type_unsurpported,
//	//n_definition_type_inconsistent,
//	//n_definition_object_exists,
//
//	//n_resourse_other = 30,
//	//n_resourse_memory_unavailable,
//	//n_resourse_processor_resource_unavailable,
//	//n_resourse_mass_storage_unavailable,
//	//n_resourse_capability_unavaliable,
//	//n_resourse_capability_unkown,
//
//	//n_service_other = 40,
//	//n_service_primitives_out_of_sequence,
//	//n_service_object_state_conflict,
//	//n_service_continuation_invalid,
//	//n_service_object_constraint_conflict,
//
//	//n_service_preempt_other = 50,
//	//n_service_timeout,
//	//n_service_deadlock,
//	//n_service_cancel,
//
//	//n_time_resolution_other = 60,
//	//n_time_resolution_unsupportable_time_resolution,
//	//
//	//n_access_other = 70,
//	//n_access_object_access_unsupported,
//	//n_access_object_non_existent,
//	//n_access_object_access_denied,
//	//n_access_object_invalidated,
//
//	//n_initiate_other = 80,
//	//n_initiate_max_services_outstanding_calling_insufficient,
//	//n_initiate_outstanding_called_insufficient,
//	//n_initiate_services_cbb_insufficient,
//	//n_initiate_parameter_cbb_insufficient,
//	//n_initiate_nesting_level_insufficient,
//
//	//n_canclude_other = 90,
//	//n_canclude_further_communication_required,
//
//	//n_cancel_other = 100,
//	//n_cancel_invike_id_unknown,
//	//n_cancel_cancel_not_possible,
//
//	//n_file_other = 110,
//	//n_file_filename_ambiguous,
//	//n_file_file_busy,
//	//n_file_filename_syntax_error,
//	//n_file_content_type_invalid,
//	//n_file_position_invalid,
//	//n_file_file_access_denied,
//	//n_file_non_existent,
//	//n_file_duplicate_filename,
//	//n_file_insufficient_space_in_filestore,
//
//	//n_others_additionalcode = 121,
//	//n_others_additonaldescription,
//	//n_others_servicespecificinfo
//
//	ACSI_SRV_ERR_CODE_UNKNOWN = -1,
//	ACSI_SRV_ERR_CODE_INSTANCE_NOT_AVAILABLE,	//ʵ��������
//	ACSI_SRV_ERR_CODE_INSTANCE_IN_USE,			//ʵ����ʹ��
//	ACSI_SRV_ERR_CODE_ACCESS_VIOLATION,			//����Υ��
//	ACSI_SRV_ERR_CODE_ACCESS_NOT_ALLOWED_IN_CURENT_STATE,//�ڵ�ǰ״̬�²��������
//	ACSI_SRV_ERR_CODE_PARAMETER_VALUE_INAPPROPRIATE,	//����ֵ������
//	ACSI_SRV_ERR_CODE_PARAMETER_VALUE_INCONSISTENT,		//����ֵ��һ��
//	ACSI_SRV_ERR_CODE_CLASS_NOT_SURPPORTED,				//�಻֧��
//	ACSI_SRV_ERR_CODE_INSTANCE_LOCKED_BY_OTHER_CLIENT,
//	ACSI_SRV_ERR__CODE_CONTROL_MUST_BE_SELECTED,
//	ACSI_SRV_ERR_CODE_TYPE_CONFILICT,
//	ACSI_SRV_ERR_CODE_FAILED_DUE_TO_COMMUNICATIONS_CONSTRAINT,
//	ACSI_SRV_ERR_CODE_FAILED_DUE_TO_SERVER_CONSTRAINT,
//	ACSI_SRV_ERR_CODE_NO_ERR = 9999			//�޴���
//
//}ACSI_SRV_ERROR_CODE;      


/*********************************************������ 2012-11-16 ����*********************************************/
//ACSI��ϸ�������Ͷ���
typedef enum                              
{  
	ACSI_DETAIL_SRVC_TYPE_UNKNOWN = 0,

	/*****************************SERVER*******************************************************/
	ACSI_DETAIL_SRVC_TYPE_GETSERVERDIR_REQ,					//GetServerDirectory request
	ACSI_DETAIL_SRVC_TYPE_GETSERVERDIR_RESP,				//GetServerDirectory response

	ACSI_DETAIL_SRVC_TYPE_ASSOCIATE_REQ,					//����Associate request
	ACSI_DETAIL_SRVC_TYPE_ASSOCIAT_RESP,					//����Associate response

	ACSI_DETAIL_SRVC_TYPE_RELEASE_REQ,						//����Release request
	ACSI_DETAIL_SRVC_TYPE_RELEASE_RESP,						//����Release response

	ACSI_DETAIL_SRVC_TYPE_ABORT_REQ,						//�쳣��ֹAbort request
	ACSI_DETAIL_SRVC_TYPE_ABORT_RESP,						//�쳣��ֹAbort response

	/******************************logical device**********************************************/
	ACSI_DETAIL_SRVC_TYPE_GETLOGICALDEVICEDIR_REQ,			//GetLogicalDeviceDirectory request
	ACSI_DETAIL_SRVC_TYPE_GETLOGICALDEVICEDIR_RESP,			//GetLogicalDeviceDirectory response
	
	/******************************logical node**********************************************/
	ACSI_DETAIL_SRVC_TYPE_GETLOGICALNODEDIR_REQ,			//GetLogicalNodeDirectory request
	ACSI_DETAIL_SRVC_TYPE_GETLOGICALNODEDIR_RESP,			//GetLogicalNodeDirectory response

	ACSI_DETAIL_SRVC_TYPE_GETALLDATAVALS_REQ,				//GetAllDataValues request
	ACSI_DETAIL_SRVC_TYPE_GETALLDATAVALS_RESP,				//GetAllDataValues response

	/******************************logical device**********************************************/
	ACSI_DETAIL_SRVC_TYPE_GETDATAVALS_REQ,					//GetDataValues request
	ACSI_DETAIL_SRVC_TYPE_GETDATAVALS_RESP,					//GetDataValues response

	ACSI_DETAIL_SRVC_TYPE_SETDATAVALS_REQ,					//SetDataValues request
	ACSI_DETAIL_SRVC_TYPE_SETDATAVALS_RESP,					//SetDataValues response

	ACSI_DETAIL_SRVC_TYPE_GETDATADIR_REQ,					//GetDataDirectory request
	ACSI_DETAIL_SRVC_TYPE_GETDATADIR_RESP,					//GetDataDirectory response

	ACSI_DETAIL_SRVC_TYPE_GETDATADEF_REQ,					//GetDataDefinition request
	ACSI_DETAIL_SRVC_TYPE_GETDATADEF_RESP,					//GetDataDefinition response

	/******************************report control block******************************************/
	ACSI_DETAIL_SRVC_TYPE_REPORT,							//����Reort

	ACSI_DETAIL_SRVC_TYPE_GETBRCBVALS_REQ,					//GetBRCBValues request
	ACSI_DETAIL_SRVC_TYPE_GETBRCBVALS_RESP,					//GetBRCBValues response

	ACSI_DETAIL_SRVC_TYPE_SETBRCBVALS_REQ,					//SetBRCBValues request
	ACSI_DETAIL_SRVC_TYPE_SETBRCBVALS_RESP,					//SetBRCBValues response

	ACSI_DETAIL_SRVC_TYPE_GETURCBVALS_REQ,					//GetURCBValues request
	ACSI_DETAIL_SRVC_TYPE_GETURCBVALS_RESP,					//GetURCBValues response

	ACSI_DETAIL_SRVC_TYPE_SETURCBVALS_REQ,					//SetURCBValues request
	ACSI_DETAIL_SRVC_TYPE_SETURCBVALS_RESP,					//SetURCBValues response

	/******************************File transfer*************************************************/
	ACSI_DETAIL_SRVC_TYPE_GETFILEDIRECTORY_REQ,				//GetFileDirectory request
	ACSI_DETAIL_SRVC_TYPE_GETFILEDIRECTORY_RESP,			//GetFileDirectory response

	ACSI_DETAIL_SRVC_TYPE_GETFILE_OPEN_REQ,					//GetFile_openfile request
	ACSI_DETAIL_SRVC_TYPE_GETFILE_OPEN_RESP,				//GetFile_openfile response

	ACSI_DETAIL_SRVC_TYPE_GETFILE_READ_REQ,					//GetFile_readfile request
	ACSI_DETAIL_SRVC_TYPE_GETFILE_READ_RESP,				//GetFile_readfile response
	
	ACSI_DETAIL_SRVC_TYPE_GETFILE_CLOSE_REQ,				//GetFile_closefile request
	ACSI_DETAIL_SRVC_TYPE_GETFILE_CLOSE_RESP,				//GetFile_closefile response

	ACSI_DETAIL_SRVC_TYPE_SETFILE_REQ,						//SetFile request
	ACSI_DETAIL_SRVC_TYPE_SETFILE_RESP,						//SetFile response

	ACSI_DETAIL_SRVC_TYPE_DELETEFILE_REQ,					//DeleteFile request
	ACSI_DETAIL_SRVC_TYPE_DELETEFILE_RESP,					//DeleteFile response

	/******************************setting control block******************************************/
	ACSI_DETAIL_SRVC_TYPE_GETSGCBVALS_REQ,					//����ֵ���ƿ��ֵGetSGCBValues request
	ACSI_DETAIL_SRVC_TYPE_GETSGCBVALS_RESP,					//����ֵ���ƿ��ֵGetSGCBValues response

	ACSI_DETAIL_SRVC_TYPE_GETSGVALS_REQ,					//�ٻ���ֵGetSGValues request
	ACSI_DETAIL_SRVC_TYPE_GETSGVALS_RESP,					//�ٻ���ֵGetSGValues response

	ACSI_DETAIL_SRVC_TYPE_SETSGVALS_REQ,					//�޸Ķ�ֵSetSGValues request
	ACSI_DETAIL_SRVC_TYPE_SETSGVALS_RESP,					//�޸Ķ�ֵSetSGValues response

	ACSI_DETAIL_SRVC_TYPE_CONFIRMEDITSGVALS_REQ,			//ȷ���޸Ķ�ֵConfirmEditSGValues request
	ACSI_DETAIL_SRVC_TYPE_CONFIRMEDITSGVALS_RESP,			//ȷ���޸Ķ�ֵConfirmEditSGValues response

	ACSI_DETAIL_SRVC_TYPE_SELECTACTSG_REQ,					//�л�������SelectActSG request
	ACSI_DETAIL_SRVC_TYPE_SELECTACTSG_RESP,					//�л�������SelectActSG response

	ACSI_DETAIL_SRVC_TYPE_SELECTEDITSG_REQ,					//�л��༭��SelectEditSG request
	ACSI_DETAIL_SRVC_TYPE_SELECTEDITSG_RESP,				//�л��༭��SelectEditSG response

	/******************************data set********************************************************/
	ACSI_DETAIL_SRVC_TYPE_GETDSVALS_REQ,					//GetDataSetValues request
	ACSI_DETAIL_SRVC_TYPE_GETDSVALS_RESP,					//GetDataSetValues response

	ACSI_DETAIL_SRVC_TYPE_SETDSVALS_REQ,					//SetDataSetValues request
	ACSI_DETAIL_SRVC_TYPE_SETDSVALS_RESP,					//SetDataSetValues response

	ACSI_DETAIL_SRVC_TYPE_CREATEDS_REQ,						//CreateDataSet request
	ACSI_DETAIL_SRVC_TYPE_CREATEDS_RESP,					//CreateDataSet response

	ACSI_DETAIL_SRVC_TYPE_DELETEDS_REQ,						//DeleteDataSet request
	ACSI_DETAIL_SRVC_TYPE_DELETEDS_RESP,					//DeleteDataSet response

	ACSI_DETAIL_SRVC_TYPE_GETDSDIR_REQ,						//GetDataSetDirectory request
	ACSI_DETAIL_SRVC_TYPE_GETDSDIR_RESP,					//GetDataSetDirectory response

	/******************************Control********************************************************/
	ACSI_DETAIL_SRVC_TYPE_SELECT_REQ,						//Select request
	ACSI_DETAIL_SRVC_TYPE_SELECT_RESP,						//Select response

	ACSI_DETAIL_SRVC_TYPE_SELECTWITHVAL_REQ,				//SelectWithValue request
	ACSI_DETAIL_SRVC_TYPE_SELECTWITHVAL_RESP,				//SelectWithValue response

	ACSI_DETAIL_SRVC_TYPE_OPERATE_REQ,						//Operate request
	ACSI_DETAIL_SRVC_TYPE_OPERATE_RESP,						//Operate response

	ACSI_DETAIL_SRVC_TYPE_CANCEL_REQ,						//Cancel request
	ACSI_DETAIL_SRVC_TYPE_CANCEL_RESP,						//Cancel response

	ACSI_DETAIL_SRVC_TYPE_CommandTermination_REQ,			//CommandTermination request
	ACSI_DETAIL_SRVC_TYPE_CommandTermination_RESP,			//CommandTermination response
	
	ACSI_DETAIL_SRVC_TYPE_TimeActivatedOperate_REQ,			//TimeActivatedOperate request
	ACSI_DETAIL_SRVC_TYPE_TimeActivatedOperate_RESP,		//TimeActivatedOperate response
	
	/******************************GOOSE********************************************************/
	ACSI_DETAIL_SRVC_TYPE_GETGOCBVALS_REQ,					//GetGoCBValues request
	ACSI_DETAIL_SRVC_TYPE_GETGOCBVALS_RESP,					//GetGoCBValues response

	ACSI_DETAIL_SRVC_TYPE_SETGOCBVALS_REQ,					//SetGoCBValues request
	ACSI_DETAIL_SRVC_TYPE_SETGOCBVALS_RESP,					//SetGoCBValues response

	ACSI_DETAIL_SRVC_TYPE_GetGoReference_REQ,				//GetGoReference  request
	ACSI_DETAIL_SRVC_TYPE_GetGoReference_RESP,				//GetGoReference response
	
	ACSI_DETAIL_SRVC_TYPE_GetGOOSEElementNumber_REQ,		//GetGOOSEElementNumber  request
	ACSI_DETAIL_SRVC_TYPE_GetGOOSEElementNumber_RESP,		//GetGOOSEElementNumber response
	
	ACSI_DETAIL_SRVC_TYPE_SendGOOSEMessage_REQ,				//SendGOOSEMessage  request
	ACSI_DETAIL_SRVC_TYPE_SendGOOSEMessage_RESP,			//SendGOOSEMessage response

	/******************************GSSE********************************************************/
	ACSI_DETAIL_SRVC_TYPE_GETGSCBVALS_REQ,					//GetGsCBValues request
	ACSI_DETAIL_SRVC_TYPE_GETGSCBVALS_RESP,					//GetGsCBValues response

	ACSI_DETAIL_SRVC_TYPE_SETGSCBVALS_REQ,					//SetGsCBValues request
	ACSI_DETAIL_SRVC_TYPE_SETGSCBVALS_RESP,					//SetGsCBValues response
	
	/******************************MSVCB********************************************************/
	ACSI_DETAIL_SRVC_TYPE_GETMSVCBVALS_REQ,					//GetMSVCBValues request
	ACSI_DETAIL_SRVC_TYPE_GETMSVCBVALS_RESP,				//GetMSVCBValues response

	ACSI_DETAIL_SRVC_TYPE_SETMSVCBVALS_REQ,					//SetMSVCBValues request
	ACSI_DETAIL_SRVC_TYPE_SETMSVCBVALS_RESP,				//SetMSVCBValues response

	/******************************USVCB********************************************************/
	ACSI_DETAIL_SRVC_TYPE_GETUSVCBVALS_REQ,					//GetUSVCBValues request
	ACSI_DETAIL_SRVC_TYPE_GETUSVCBVALS_RESP,				//GetUSVCBValues response

	ACSI_DETAIL_SRVC_TYPE_SETUSVCBVALS_REQ,					//SetUSVCBValues request
	ACSI_DETAIL_SRVC_TYPE_SETUSVCBVALS_RESP,				//SetUSVCBValues response
	
	/******************************log control block********************************************************/

	ACSI_DETAIL_SRVC_TYPE_GETLCBVALS_REQ,					//GetLCBValues  request
	ACSI_DETAIL_SRVC_TYPE_GETLCBVALS_RESP,					//GetLCBValues  response

	ACSI_DETAIL_SRVC_TYPE_SETLCBVALS_REQ,					//SetLCBValues request
	ACSI_DETAIL_SRVC_TYPE_SETLCBVALS_RESP,					//SetLCBValues response

	ACSI_DETAIL_SRVC_TYPE_GETLOGSTATUSVALS_REQ,				//GetLogStatusValues request
	ACSI_DETAIL_SRVC_TYPE_GETLOGSTATUSVALS_RESP,			//GetLogStatusValues response

	ACSI_DETAIL_SRVC_TYPE_QUERYLOGBYTIME_REQ,				//QueryLogByTime request
	ACSI_DETAIL_SRVC_TYPE_QUERYLOGBYTIME_RESP,				//QueryLogByTime response

	ACSI_DETAIL_SRVC_TYPE_QUERYLOGAFTER_REQ,				//QueryLogAfter request
	ACSI_DETAIL_SRVC_TYPE_QUERYLOGAFTER_RESP,				//QueryLogAfter response

	/*********************************************************************************/
	ACSI_DETAIL_SRVC_TYPE_SERVICE_FAIL_RESP,				//ServiceResponse- �������Ӧ
	ACSI_DETAIL_SRVC_TYPE_SERVICE_ERROR,					//ServiceError
	ACSI_DETAIL_SRVC_TYPE_REJECT,							//RejectError
	ACSI_DETAIL_SRVC_TYPE_ASSOCIATE_FAIL,					//����Associate response
	ACSI_DETAIL_SRVC_TYPE_SETBRCBVALS_RPTENA_REQ,			//SetBRCBValues request
	/******************************READ WRITE**********************************************/
	ACSI_DETAIL_SRVC_TYPE_MMS_READ_,				    //GetAllDataValues request
	ACSI_DETAIL_SRVC_TYPE_MMS_READ_RESP_,			//GetAllDataValues request
	ACSI_DETAIL_SRVC_TYPE_MMS_WRITE_,				//GetAllDataValues response
	ACSI_DETAIL_SRVC_TYPE_MMS_WRITE_RESP_,				//GetAllDataValues response
	ACSI_DETAIL_SRVC_TYPE_GETFILEDIRECTORY_FAIL_,
	ACSI_DETAIL_ETHER_TYPE_TCP_FIN,
	ACSI_DETAIL_ETHER_TYPE_TCP_SYN_REQ,
	ACSI_DETAIL_ETHER_TYPE_TCP_SYN_RESP,
	ACSI_DETAIL_ETHER_TYPE_TCP_RST,
	ACSI_DETAIL_ETHER_TYPE_TCP_PSH,
	ACSI_DETAIL_ETHER_TYPE_TCP_ACK,
	ACSI_DETAIL_ETHER_TYPE_TCP_URG,
	
}ACSI_DETAIL_SRVC_TYPE; //�ο� MMS_DETAIL_SRVC_TYPE��Ϊ���䣬�ͱ������ظ�

//MMS->ACSI�¼��Ŷ���
typedef enum
{
	MMS_2_ACSI_EVENT_ID_UNKOWN=0,					//δ֪MMS�¼�
	MMS_2_ACSI_EVENT_ID_ASSOCIATE,					//Associate
	MMS_2_ACSI_EVENT_ID_ABORT,						//Abort
	MMS_2_ACSI_EVENT_ID_RELEASE,					//Release
	MMS_2_ACSI_EVENT_ID_REPORT,						//Report
	MMS_2_ACSI_EVENT_ID_SETSGVALUES,				//SetSGValues
	MMS_2_ACSI_EVENT_ID_CONFIRMEDITSGVALUES,		//ConfirmEditSGValues				
	MMS_2_ACSI_EVENT_ID_SELECTACTIVESG,				//SelectActiveSG
	MMS_2_ACSI_EVENT_ID_SELECT,						//Select
	MMS_2_ACSI_EVENT_ID_SELECTWITHVALUE,			//SelectWithValue
	MMS_2_ACSI_EVENT_ID_OPERATE,					//Operate
	MMS_2_ACSI_EVENT_ID_CANCLE,						//Cancel

	MMS_2_ACSI_EVENT_ID_SERVICE_FAIL_RESP,			//�������Ӧ
	MMS_2_ACSI_EVENT_ID_SERVICE_ERROR,				//�������
	MMS_2_ACSI_EVENT_ID_SOE_DATA_VALUES_INVALID,	//SOE����������ֵ��Ч
	MMS_2_ACSI_EVENT_ID_RCB_INIT_ERROR,				//������ƿ��ʼ���쳣
	MMS_2_ACSI_EVENT_ID_DIFFER_SCD,					//��SCD�ļ����ò�һ��
	MMS_2_ACSI_EVENT_ID_REJECT,						//����ܾ�
	MMS_2_ACSI_EVENT_ID_TCP_FIN,					//�ͷ�����					
	MMS_2_ACSI_EVENT_ID_TCP_SYN_REQ,				//��������			
	MMS_2_ACSI_EVENT_ID_TCP_SYN_RESP,				//��������
	MMS_2_ACSI_EVENT_ID_TCP_RST,					//��������
	MMS_2_ACSI_EVENT_ID_TCP_PUSH,					//Ӧ�ò������ݣ����ܷ���Ҫ���촦��
	MMS_2_ACSI_EVENT_ID_TCP_ACK,					//��������
	MMS_2_ACSI_EVENT_ID_TCP_URG,					//��������
	MMS_2_ACSI_EVENT_ID_GETBRCBVALS_REQ,       //��������ƿ�
	MMS_2_ACSI_EVENT_ID_SETBRCBVALS_REQ,      //д������ƿ�
	MMS_2_ACSI_EVENT_ID_SETBRCBVALS_RPTENA_REQ,     //����ʹ������
	MMS_2_ACSI_EVENT_ID_ASSOCIATE_SUC,			    //Associate
	MMS_2_ACSI_EVENT_ID_ASSOCIATE_FAIL,             //����ʧ��
	MMS_2_ACSI_EVENT_ID_GETFILEDIRECTORY,           //��ȡ�ļ��б�
	MMS_2_ACSI_EVENT_ID_GETFILEDIRECTORY_SUC,       //��ȡ�ļ��б�ɹ�
	MMS_2_ACSI_EVENT_ID_CANCLE_SUC,                 //ȡ�����Ƴɹ�
	MMS_2_ACSI_EVENT_ID_SELECTEDITSG,               //ѡ��༭��ֵ����
	MMS_2_ACSI_EVENT_ID_SERVICE_GENERAL_READ,       //������
	MMS_2_ACSI_EVENT_ID_SERVICE_GENERAL_READ_SUC,   //������ɹ�
	MMS_2_ACSI_EVENT_ID_SERVICE_GENERAL_WRITE,      //д����
	MMS_2_ACSI_EVENT_ID_SERVICE_GENERAL_WRITE_SUC,  //д����ɹ�
	MMS_2_ACSI_EVENT_ID_GETFILEDIRECTORY_FAIL,      //��ȡ�ļ��б�ʧ��
	MMS_2_ACSI_EVENT_ID_GETURCBVALS_REQ,       //��������ƿ� �ǻ���
	MMS_2_ACSI_EVENT_ID_SETURCBVALS_REQ,      //д������ƿ� �ǻ���
	//������չ
	MMS_2_ACSI_EVENT_ID_GETALLDATAVALS,//����
	MMS_2_ACSI_EVENT_ID_GETDSVALS, //��д���ݼ�
	MMS_2_ACSI_EVENT_ID_SETDSVALS,
	MMS_2_ACSI_EVENT_ID_GETSGVALS,//�ٻ���ֵ
	MMS_2_ACSI_EVENT_ID_GETLCBVALS,//��д��־������ƿ�
	MMS_2_ACSI_EVENT_ID_SETLCBVALS,
	MMS_2_ACSI_EVENT_ID_GETGOCBVALS,//goose���ƿ�
	MMS_2_ACSI_EVENT_ID_SETGOCBVALS,
	MMS_2_ACSI_EVENT_ID_GETGSCBVALS,//gsse���ƿ�
	MMS_2_ACSI_EVENT_ID_SETGSCBVALS,
	MMS_2_ACSI_EVENT_ID_GETMSVCBVALS,//�������ƿ��·
	MMS_2_ACSI_EVENT_ID_SETMSVCBVALS,
	MMS_2_ACSI_EVENT_ID_GETUSVCBVALS,//�������ƿ鵥·
	MMS_2_ACSI_EVENT_ID_SETUSVCBVALS_REQ,
	MMS_2_ACSI_EVENT_ID_GETSGCBVALS,//����ǰ��ֵ������?
	MMS_2_ACSI_EVENT_ID_GETDATAVALS,
	MMS_2_ACSI_EVENT_ID_SETDATAVALS,

}MMS_2_ACSI_EVENT_ID;

//��������
typedef enum 
{
	DETAIL_OBJECT_TYPE_UNKOWN = 0,

	DETAIL_OBJECT_TYPE_IED,		//IED
	DETAIL_OBJECT_TYPE_LD,		//LD
	DETAIL_OBJECT_TYPE_LN,		//LN
	DETAIL_OBJECT_TYPE_DS,		//DataSet

// 	DETAIL_OBJECT_TYPE_FCN,		//��fc��ln
// 	DETAIL_OBJECT_TYPE_FCD, 
// 	DETAIL_OBJECT_TYPE_FCDA,

	DETAIL_OBJECT_TYPE_ST,
	DETAIL_OBJECT_TYPE_MX,
	DETAIL_OBJECT_TYPE_CO,
	DETAIL_OBJECT_TYPE_SP,
	DETAIL_OBJECT_TYPE_SV,
	DETAIL_OBJECT_TYPE_CF,
	DETAIL_OBJECT_TYPE_DC,
	DETAIL_OBJECT_TYPE_SG,
	DETAIL_OBJECT_TYPE_SE,
	DETAIL_OBJECT_TYPE_EX,

	DETAIL_OBJECT_TYPE_BRCB,			//���汨����ƿ�
	DETAIL_OBJECT_TYPE_URCB,			//�ǻ��汨����ƿ�
	DETAIL_OBJECT_TYPE_LCB,				//��־���ƿ�
	DETAIL_OBJECT_TYPE_GOCB,			//GOOSE���ƿ�
	DETAIL_OBJECT_TYPE_GSCB,			//GSSE���ƿ�
	DETAIL_OBJECT_TYPE_MSVCB,			//��·�㲥����ֵ
	DETAIL_OBJECT_TYPE_USVCB,			//��·�������ֵ���ƿ�
	DETAIL_OBJECT_TYPE_SGCB,			//��ֵ���ƿ�

	DETAIL_OBJECT_TYPE_ACTSG,		//��ǰ��ֵ����
	DETAIL_OBJECT_TYPE_EDITSG,		//�༭��ֵ����
	DETAIL_OBJECT_TYPE_CNFEDIT,		//CnfEdit
	DETAIL_OBJECT_TYPE_LACTTM,		//LActTm
	DETAIL_OBJECT_TYPE_OPER,		//Oper
	DETAIL_OBJECT_TYPE_SBO,			//SBO
	DETAIL_OBJECT_TYPE_SBOW,		//SBOw	
	DETAIL_OBJECT_TYPE_BRCB_RPTENA,		//���汨����ƿ�ʹ��
	DETAIL_OBJECT_TYPE_CANCEL,      //cancelң�س���

}DETAIL_OBJ_TYPE;


#endif /*ACSI_CONST_INCLUDE*/