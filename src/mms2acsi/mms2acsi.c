/****************************************************************************/
/* Copyright (c) 2010,许继集团有限公司                                      */
/* All rights reserved.                                                     */
/*                                                                          */
/*  模块描述：                                                              */
/** mms服务与acsi服务转换接口
*  @file  mms2acsi.c                                                      
/*                                                                          */
/* 日期         作者    注释                                                */
/* 2010/07/17   ZYZ     创建文件                                            */
/****************************************************************************/
#include "mms2acsi.h"
#include "acsi_crscl.h"
#include "acsi_dir.h"
#include "stdlib.h"

#ifdef DEBUG_SISCO
SD_CONST static ST_CHAR *SD_CONST thisFileName = __FILE__;
#endif

/**
*通过mms的DataAccessError代码获取ACSI服务错误代码
*/
ACSI_ERR_CODE get_acsi_error_code_dataAccess(int n_mms_data_access_error_code)//IEC61850 8-1 的Table20
{
	int n_acsi_err_code;

	n_acsi_err_code = ACSI_ERR_CODE_UNKNOWN;

	switch (n_mms_data_access_error_code)
	{
	case MS_DATA_ACCESS_ERR_CODE_OBJECT_INVALIDATED://对象无效
		break;
	case MS_DATA_ACCESS_ERR_CODE_HARDWARE_FAULT://硬件错误
		n_acsi_err_code = ACSI_ERR_CODE_FAILED_DUE_TO_SERVER_CONSTRAINT;
		break;
	case MS_DATA_ACCESS_ERR_CODE_TEMPORARILY_UNVAILABLE://暂时不可访问
		n_acsi_err_code = ACSI_ERR_CODE_INSTANCE_LOCKED_BY_OTHER_CLIENT;
		break;
	case MS_DATA_ACCESS_ERR_CODE_OBJECT_ACCESS_DENIED://访问被拒绝
		n_acsi_err_code = ACSI_ERR_CODE_ACCESS_VIOLATION;
		break;
	case MS_DATA_ACCESS_ERR_CODE_OBJECT_UNDEFINED://对象没有定义
		break;
	case MS_DATA_ACCESS_ERR_CODE_INVALID_ADDRESS://地址无效
		n_acsi_err_code = ACSI_ERR_CODE_PARAMETER_VALUE_INCONSISTENT;
		break;
	case MS_DATA_ACCESS_ERR_CODE_TYPE_UNSUPPORTED://类型不支持
		break;
	case MS_DATA_ACCESS_ERR_CODE_TYPE_INCONSISTENT://类型不一致
		n_acsi_err_code = ACSI_ERR_CODE_TYPE_CONFILICT;
		break;
	case MS_DATA_ACCESS_ERR_CODE_OBJECT_ATTRIBUTE_INCONSISTENT://对象属性不一致
		break;
	case MS_DATA_ACCESS_ERR_CODE_OBJECT_ACCESS_UNSUPPORTED://对象访问不支持
		break;
	case MS_DATA_ACCESS_ERR_CODE_OBJECT_NON_EXISTENT://对象不存在
		n_acsi_err_code = ACSI_ERR_CODE_INSTANCE_NOT_AVAILABLE;
		break;
	case MS_DATA_ACCESS_ERR_CODE_OBJECT_VALUE_INVALID://对象值无效
		break;

	case MS_DATA_ACCESS_ERR_CODE_NO_ERR:			//成功
		n_acsi_err_code = ACSI_ERR_CODE_NO_ERR;		//成功
		break;

	default:
		break;
	}

	return n_acsi_err_code;
}

/**
*通过mms的ServiceError的ErrorClass和ErrorCode代码获取ACSI服务错误代码
*/
ACSI_ERR_CODE get_acsi_error_code(int n_mms_error_class, int n_mms_error_code)
{
	int n_error_tmp;
	int n_acsi_err_code;

	n_acsi_err_code  = ACSI_ERR_CODE_FAILED_DUE_TO_COMMUNICATIONS_CONSTRAINT;//默认

	switch (n_mms_error_class)
	{
	case 1:
		switch (n_mms_error_code)
		{
		case 1:
			n_acsi_err_code = ACSI_ERR_CODE_INSTANCE_NOT_AVAILABLE;
			break;

		case 4:
			n_acsi_err_code = ACSI_ERR_CODE_PARAMETER_VALUE_INAPPROPRIATE;
			break;
		}
		break;

	case 2:
		switch (n_mms_error_code)
		{
		case 1:
			n_acsi_err_code = ACSI_ERR_CODE_INSTANCE_NOT_AVAILABLE;
			break;
		case 2:
			n_acsi_err_code = ACSI_ERR_CODE_PARAMETER_VALUE_INCONSISTENT;
			break;
		case 4:
			n_acsi_err_code = ACSI_ERR_CODE_TYPE_CONFILICT;
			break;
		case 5:
			n_acsi_err_code = ACSI_ERR_CODE_INSTANCE_IN_USE;
			break;
		}
		break;
		
	case 3:
		switch (n_mms_error_code)
		{
		case 5:
			n_acsi_err_code = ACSI_ERR_CODE_FAILED_DUE_TO_SERVER_CONSTRAINT;
			break;
		}
		break;

	case 4:
		switch (n_mms_error_code)
		{
		case 2:
			n_acsi_err_code = ACSI_ERR_CODE_INSTANCE_IN_USE;
			break;
		}

	case 7:
		switch (n_mms_error_code)
		{
		case 1:
			n_acsi_err_code = ACSI_ERR_CODE_ACCESS_VIOLATION;//Table 28 – ServiceError mapping for CreateDataSet
			//Table 30 – ServiceError mapping for DeleteDataSet定义为ACSI_SRV_ERR_CODE_SERVER_CONSTRAINT
			//Table 32 – ServiceError mapping for GetDataSetDirectory读数据集目录服务/删除数据定义为ACSI_SRV_ERR_CODE_SERVER_CONSTRAINT
			break;
		case 2:
			n_acsi_err_code = ACSI_ERR_CODE_INSTANCE_NOT_AVAILABLE;
			break;
		case 3:
			n_acsi_err_code = ACSI_ERR_CODE_ACCESS_VIOLATION;
			break;
		}
		break;

	case 8:
		switch (n_mms_error_code)
		{
		case 0://
			n_acsi_err_code = ACSI_ERR_CODE_FAILED_DUE_TO_SERVER_CONSTRAINT;
			break;
		case 4:
			n_acsi_err_code = ACSI_ERR_CODE_PARAMETER_VALUE_INCONSISTENT;
			break;
		case 7:
			n_acsi_err_code = ACSI_ERR_CODE_FAILED_DUE_TO_COMMUNICATIONS_CONSTRAINT;
			break;
		}
		break;

	case 9:
		switch (n_mms_error_code)
		{
		case 0://
			n_acsi_err_code = ACSI_ERR_CODE_FAILED_DUE_TO_SERVER_CONSTRAINT;
			break;
		case 1:
			n_acsi_err_code = ACSI_ERR_CODE_INSTANCE_NOT_AVAILABLE;
			break;
		}
		break;

	case 11://文件服务IEC61850 8-1 Table 67
		switch (n_mms_error_code)
		{
		case 1://filename-ambiguous
		case 3://filename-syntax-error
			n_acsi_err_code = ACSI_ERR_CODE_PARAMETER_VALUE_INAPPROPRIATE;
			break;

		case 2://file-busy
			n_acsi_err_code = ACSI_ERR_CODE_INSTANCE_LOCKED_BY_OTHER_CLIENT;
			break;
		
		case 4://content-type-invalid
			n_acsi_err_code = ACSI_ERR_CODE_TYPE_CONFILICT;
			break;

		case 5://position-invalid
			n_acsi_err_code = ACSI_ERR_CODE_PARAMETER_VALUE_INCONSISTENT;
			break;

		case 6://file-access-denied
			n_acsi_err_code = ACSI_ERR_CODE_ACCESS_VIOLATION;
			break;

		case 7://file-non-existent
			n_acsi_err_code = ACSI_ERR_CODE_INSTANCE_NOT_AVAILABLE;
			break;

		case 8://duplicate-filename
			n_acsi_err_code = ACSI_ERR_CODE_INSTANCE_IN_USE;
			break;

		case 9://insufficient-space-in-filestore
			n_acsi_err_code = ACSI_ERR_CODE_FAILED_DUE_TO_SERVER_CONSTRAINT;
			break;
		}
		break;
	}
	if (12 == n_mms_error_class)
	{
		return ACSI_ERR_CODE_FAILED_DUE_TO_COMMUNICATIONS_CONSTRAINT;
	}

	return n_acsi_err_code;
}

/**
*通过domainId和itemId获取ACSI的服务类型
* c_domain_id，对象的domainId
* c_item_id，对象的itemId
* n_type，MMS服务方式。
* ied，对象所属的ied对象空间
*/
ACSI_SRVC_TYPE get_acsi_srv_type_domid_itemid(ST_CHAR *c_domain_id, ST_CHAR *c_item_id, MMS_SRVC_TYPE n_type, ACSI_IED_CTRL *ied)
{
	ACSI_SRVC_TYPE		srv_type; 
	ST_CHAR				ref[MAX_IDENT_LEN +1] = {0};
	ACSI_COMMON_OBJ*	comm_obj = NULL;

	memset(ref, 0, MAX_IDENT_LEN);
	srv_type = ACSI_SRVC_TYPE_UNKNOWN;

	//入口判断
	if (NULL==c_domain_id || NULL==c_item_id || NULL==ied)
	{
		return srv_type;
	}

	strcpy(ref, c_domain_id);
	strcat(ref, "/");
	strcat(ref, c_item_id);
	comm_obj = acsi_get_obj_ref(ied, ref);//获得对象空间中ref对应的obj的指针
	if (NULL == comm_obj)
		srv_type = ACSI_SRVC_TYPE_UNKNOWN;
	else
	{
		if (OBJECT_TYPE_DS == comm_obj->obj_type)
		{
			if (MMS_SRVC_TYPE_READ == n_type)
			{
				srv_type = ACSI_SRVC_TYPE_GETDSVALS;//读数据集GetDataSetValues
			}
			else if (MMS_SRVC_TYPE_WRITE == n_type)
			{
				srv_type = ACSI_SRVC_TYPE_SETDSVALS;//写数据集SetDataSetValues
			}
			else if (MMS_SRVC_TYPE_DEFINENAMEDVARLST == n_type)
			{
				srv_type = ACSI_SRVC_TYPE_CREATEDS;//创建数据集CreateDataSet
			}
			else if (MMS_SRVC_TYPE_DELETENAMEDVARLST == n_type)
			{
				srv_type = ACSI_SRVC_TYPE_DELETEDS;//删除数据集DeleteDataSet
			}
			else if (MMS_SRVC_TYPE_GETNAMEDVARLSTATTR == n_type)
			{
				srv_type = ACSI_SRVC_TYPE_GETDSDIR;//数据集目录服务GetDataSetDirectory
			}
		}
		else if (OBJECT_TYPE_CB==comm_obj->obj_type)
		{
			if (NULL != strstr(c_item_id, "$BR$"))
			{
				if (MMS_SRVC_TYPE_READ == n_type)
				{
					srv_type = ACSI_SRVC_TYPE_GETBRCBVALS;//GetBRCBValues
				}
				else if (MMS_SRVC_TYPE_WRITE == n_type)
				{
					srv_type = ACSI_SRVC_TYPE_SETBRCBVALS;//SetBRCBValues
				}
			}
			else if (NULL != strstr(c_item_id, "$RP$"))
			{
				if (MMS_SRVC_TYPE_READ == n_type) 
				{
					srv_type = ACSI_SRVC_TYPE_GETURCBVALS;//GetURCBValues
				}
				else if (MMS_SRVC_TYPE_WRITE == n_type)
				{
					srv_type = ACSI_SRVC_TYPE_SETURCBVALS;//SetURCBValues
				}
				
			}
			else if (NULL != strstr(c_item_id, "$SP$SGCB"))
			{
				if (MMS_SRVC_TYPE_READ == n_type)
				{
					srv_type = ACSI_SRVC_TYPE_GETSGCBVALS;//GetSGCBValues
				}
			}
			else if (NULL != strstr(c_item_id, "$GO$"))
			{
				if (MMS_SRVC_TYPE_READ == n_type)
				{
					srv_type = ACSI_SRVC_TYPE_GETGOCBVALS;//GetGoCBValues
				}
				else if (MMS_SRVC_TYPE_WRITE == n_type)
				{
					srv_type = ACSI_SRVC_TYPE_SETGOCBVALS;//SetGoCBValues
				}
				
			}
			else if (NULL != strstr(c_item_id, "$GS$"))
			{
				if (MMS_SRVC_TYPE_READ == n_type)
				{
					srv_type = ACSI_SRVC_TYPE_GETGSCBVALS;//GetGsCBValues
				}
				else if (MMS_SRVC_TYPE_WRITE == n_type)
				{
					srv_type = ACSI_SRVC_TYPE_SETGSCBVALS;//SetGsCBValues
				}
			}
			else if (NULL != strstr(c_item_id, "$MS$"))
			{
				if (MMS_SRVC_TYPE_READ == n_type)
				{
					srv_type = ACSI_SRVC_TYPE_GETMSVCBVALS;//GetMSVCBValues
				}
				else if (MMS_SRVC_TYPE_WRITE == n_type)
				{
					srv_type = ACSI_SRVC_TYPE_SETMSVCBVALS;//SetMSVCBValues
				}
			}
			else if (NULL != strstr(c_item_id, "$US$"))
			{
				if (MMS_SRVC_TYPE_READ == n_type)
				{
					srv_type = ACSI_SRVC_TYPE_GETUSVCBVALS;//GetUSVCBValues
				}
				else if (MMS_SRVC_TYPE_WRITE == n_type)
				{
					srv_type = ACSI_SRVC_TYPE_SETUSVCBVALS;//SetUSVCBValues
				}
			}
		}
		else if (OBJECT_TYPE_LN == comm_obj->obj_type)
		{
			if (MMS_SRVC_TYPE_GETNAMELIST == n_type)
			{
				srv_type = ACSI_SRVC_TYPE_GETLOGICALNODEDIR;//GetLogicalNodeDirectory
			}
			else
			{
				srv_type = ACSI_SRVC_TYPE_GETALLDATAVALS;//GetAllDataValues
			}
		}
		else if (OBJECT_TYPE_IED == comm_obj->obj_type)
		{
			srv_type = ACSI_SRVC_TYPE_GETSERVERDIR;//GetServerDirectory
		}
		else if (OBJECT_TYPE_LD == comm_obj->obj_type)
		{
			srv_type = ACSI_SRVC_TYPE_GETLOGICALDEVICEDIR;//GetLogicalDeviceDirectory
		}
		else if (OBJECT_TYPE_FCN == comm_obj->obj_type)//add on 20110831
		{
			if (MMS_SRVC_TYPE_READ == n_type)
			{
				srv_type = ACSI_SRVC_TYPE_GETALLDATAVALS;
			}
		}
		else//add on 20110831
		{
			if (MMS_SRVC_TYPE_WRITE == n_type)
			{
				if (NULL != strstr(c_item_id, "$BR$"))
				{
					srv_type = ACSI_SRVC_TYPE_SETBRCBVALS;
				}
				else if (NULL != strstr(c_item_id, "$RP$"))
				{
					srv_type = ACSI_SRVC_TYPE_SETURCBVALS;
				}
				else if (NULL != strstr(c_item_id, "$GO$"))
				{
					srv_type = ACSI_SRVC_TYPE_SETGOCBVALS;
				}
				else if (NULL != strstr(c_item_id, "$GS$"))
				{
					srv_type = ACSI_SRVC_TYPE_SETGSCBVALS;
				}
				else if (NULL != strstr(c_item_id, "$MS$"))
				{
					srv_type = ACSI_SRVC_TYPE_SETMSVCBVALS;
				}
				else if (NULL != strstr(c_item_id, "$US$"))
				{
					srv_type = ACSI_SRVC_TYPE_SETUSVCBVALS;
				}
			}
		}
	}

	return srv_type;
}

/**
* 转换字符串为mms服务类型
*/
MMS_SRVC_TYPE get_mms_srvType_str(ST_CHAR *type_str)
{
	//入口判断
	if (NULL == type_str)
	{
		return MMS_SRVC_TYPE_UNKNOWN;
	}

	if (strcmp(type_str, "write") == 0)
	{
		return MMS_SRVC_TYPE_WRITE;
	}
	else if (strcmp(type_str, "InformationReport") == 0)
	{
		return MMS_SRVC_TYPE_INFO;
	}
	else if (strcmp(type_str, "read") == 0)
	{
		return MMS_SRVC_TYPE_READ;
	}
	else if (strcmp(type_str, "fileOpen") == 0 )
	{
		return MMS_SRVC_TYPE_FILEOPEN;
	}
	else if (strcmp(type_str,"fileRead") == 0)
	{
		return MMS_SRVC_TYPE_FILEREAD;
	}
	else if (strcmp(type_str, "fileClose") == 0)
	{
		return MMS_SRVC_TYPE_FILECLOSE;
	}
	else if (strcmp(type_str, "fileDirectory") == 0)
	{
		return MMS_SRVC_TYPE_FILEDIRECTORY;
	}
	else if (strcmp(type_str, "initiate") == 0)
	{
		return MMS_SRVC_TYPE_INITIATE;
	}
	else if (strcmp(type_str, "conclude") == 0)
	{
		return MMS_SRVC_TYPE_CONCLUDE;
	}
	else if (strcmp(type_str, "p-abort") == 0)
	{
		return MMS_SRVC_TYPE_P_ABORT;
	}
	else if (strcmp(type_str, "u-abort") == 0)
	{
		return MMS_SRVC_TYPE_U_ABORT;
	}
	else if (strcmp(type_str, "reject") == 0)
	{
		return MMS_SRVC_TYPE_REJECT;
	}
	else if (strcmp(type_str, "cancle") == 0)
	{
		return MMS_SRVC_TYPE_CANCEL;
	}
	else if (strcmp(type_str, "obtainFile") == 0)
	{
		return MMS_SRV_TYPE_OBTAIN_FILE;
	}
	else if (strcmp(type_str, "fileDelete") == 0)
	{
		return MMS_SRV_TYPE_FILEDELETE;
	}
	else if (strcmp(type_str, "getNameList") == 0)
	{
		return MMS_SRVC_TYPE_GETNAMELIST;
	}
	else if (strcmp(type_str, "getVariableAccessAttributes") == 0)
	{
		return MMS_SRVC_TYPE_GETVARACCESSATTR;
	}
	else if (strcmp(type_str, "getNamedVariableListAttributes") == 0)
	{
		return MMS_SRVC_TYPE_GETNAMEDVARLSTATTR;
	}
	else if (strcmp(type_str, "defineNamedVariableList") == 0)
	{
		return MMS_SRVC_TYPE_DEFINENAMEDVARLST;
	}
	else if (strcmp(type_str, "deleteNamedVariableList") == 0)
	{
		return MMS_SRVC_TYPE_DELETENAMEDVARLST;
	}
	else if (strcmp(type_str, "readJournal") == 0)
	{
		return MMS_SRVC_TYPE_READJOURNAL;
	}
	
	return MMS_SRVC_TYPE_UNKNOWN;
}

/**
* 将MMS的服务类型映射到ACSI的服务类型
* c_service_type	MMS的服务类型
*/
ACSI_SRVC_TYPE get_acsiSrvType_mmsSrvType(ST_CHAR *c_service_type)
{
	ACSI_SRVC_TYPE		srv_type; 

	srv_type = ACSI_SRVC_TYPE_UNKNOWN;
	if (NULL == c_service_type)
	{
		return srv_type;
	}

	switch(get_mms_srvType_str(c_service_type))
	{	
	case MMS_SRVC_TYPE_READ://mms - read
		srv_type = ACSI_SRVC_TYPE_GETDATAVALS;//acsi - GetDataValues
		break;

	case MMS_SRVC_TYPE_WRITE://mms - write
		srv_type = ACSI_SRVC_TYPE_SETDATAVALS;//acsi - SetDataValues
		break;

	case MMS_SRVC_TYPE_INFO://mms - informationReport
		srv_type = ACSI_SRVC_TYPE_REPORT;//acsi - Report
		break;

	case MMS_SRVC_TYPE_FILEDIRECTORY://mms - fileDirectory
		srv_type = ACSI_SRVC_TYPE_GETFILEDIRECTORY;//acsi - GetFileDirectory
		break;

	case MMS_SRVC_TYPE_FILEOPEN://mms - fileOpen
		srv_type = ACSI_SRVC_TYPE_GETFILE_OPEN;//acsi - GetFile_openfile
		break;

	case MMS_SRVC_TYPE_FILEREAD://mms - fileRead
		srv_type = ACSI_SRVC_TYPE_GETFILE_READ;//acsi - GetFile_readfile
		break;

	case MMS_SRVC_TYPE_FILECLOSE://mms - fileClose
		srv_type = ACSI_SRVC_TYPE_GETFILE_CLOSE;//acsi - GetFile_closefile
		break;

	case MMS_SRV_TYPE_OBTAIN_FILE: //mms - obtainFile
		srv_type = ACSI_SRVC_TYPE_SETFILE;//acsi - SetFile
		break;

	case  MMS_SRV_TYPE_FILEDELETE://mms - fileDelete
		srv_type = ACSI_SRVC_TYPE_DELETEFILE;//acsi - DeleteFile
		break;

	case MMS_SRVC_TYPE_INITIATE://mms - initiate
		srv_type = ACSI_SRVC_TYPE_ASSOCIATE;//acsi - Associate
		break;

	case MMS_SRVC_TYPE_CONCLUDE://mms - conclude
		srv_type = ACSI_SRVC_TYPE_RELEASE;//acsi - Release
		break;

	case MMS_SRVC_TYPE_P_ABORT://mms - abort
		srv_type = ACSI_SRVC_TYPE_ABORT;//acsi - Abort
		break;

	case MMS_SRVC_TYPE_GETNAMEDVARLSTATTR://mms - getNamedVariableListAttributes
		srv_type = ACSI_SRVC_TYPE_GETDSDIR;//acsi - GetDataSetDirectory
		break;

	case MMS_SRVC_TYPE_DEFINENAMEDVARLST://mms - defineNamedVariableList
		srv_type = ACSI_SRVC_TYPE_CREATEDS;//acsi - CreateDataSet
		break;

	case MMS_SRVC_TYPE_DELETENAMEDVARLST://mms - deleteNamedVariableList
		srv_type = ACSI_SRVC_TYPE_DELETEDS;//acsi - DeleteDataSet
		break;

	default:
		break;
	}

	return srv_type;
}

/**
*获取ACSI的服务类型（只对mms的request）
* mms_info，MMS信息结构
* ied，对象所属的ied对象空间
*/
ACSI_SRVC_TYPE get_acsi_srv_type(MMS_INFO_STRUCT *mms_info, ACSI_IED_CTRL *ied)
{
	ACSI_SRVC_TYPE		srv_type; 
	ST_CHAR*			pTmp = NULL;
	MMS_SRVC_TYPE		n_mms_srvc_type;

	//入口判断
	srv_type = ACSI_SRVC_TYPE_UNKNOWN;
	if (NULL==mms_info)
	{
		return srv_type;
	}

	//............MMS的非request服务
	if (NULL==strstr(mms_info->c_pdu_type, "Request"))
	{
		//将MMS的服务类型映射到ACSI的服务类型
		srv_type = get_acsiSrvType_mmsSrvType(mms_info->c_service_type);
		return srv_type;
	}
	//............MMS的request服务，但非read-request和非write-request服务
	else if (NULL!=strstr(mms_info->c_pdu_type, "Request")
		&& 0!=strcmp(mms_info->c_service_type, "read")
		&& 0!=strcmp(mms_info->c_service_type, "write"))
	{
		//将MMS的服务类型映射到ACSI的服务类型
		srv_type = get_acsiSrvType_mmsSrvType(mms_info->c_service_type);
		if (ACSI_SRVC_TYPE_UNKNOWN==srv_type && NULL!=mms_info->p_variable_struct)
		{
			n_mms_srvc_type = get_mms_srvType_str(mms_info->c_service_type);
			srv_type = get_acsi_srv_type_domid_itemid(mms_info->p_variable_struct[0].c_domain_id, mms_info->p_variable_struct[0].c_item_id, n_mms_srvc_type, ied);
		}
		return srv_type;
	}

	//.................MMS的read-request和write-request服务
	if (NULL==mms_info->p_variable_struct)
	{
		return srv_type;
	}
	//MMS的read服务
	if (0 == strcmp(mms_info->c_service_type, "read"))
	{
		if (NULL!=strstr(mms_info->p_variable_struct[0].c_item_id, "$SG$") ||
			NULL!=strstr(mms_info->p_variable_struct[0].c_item_id, "$SE$") )
		{
			srv_type = ACSI_SRVC_TYPE_GETSGVALS;//召唤定值GetSGValues
		}
		else
		{
			srv_type = get_acsi_srv_type_domid_itemid(mms_info->p_variable_struct[0].c_domain_id, mms_info->p_variable_struct[0].c_item_id, MMS_SRVC_TYPE_READ, ied);
		}

		if (ACSI_SRVC_TYPE_UNKNOWN == srv_type)
		{
			srv_type = ACSI_SRVC_TYPE_GETDATAVALS;//默认GetDataValues
		}
	}
	//MMS的write服务
	else if (0 == strcmp(mms_info->c_service_type, "write"))
	{
		pTmp = strrchr(mms_info->p_variable_struct[0].c_item_id,'$');
		if (NULL != pTmp)
		{
			if (0 == strcmp(pTmp, "$CnfEdit"))
			{
				srv_type = ACSI_SRVC_TYPE_CONFIRMEDITSGVALS;//确认修改定值ConfirmEditSGValues
			}
			else if (0 == strcmp(pTmp, "$ActSG"))
			{
				srv_type = ACSI_SRVC_TYPE_SELECTACTSG;//切换激活区SelectActSG
			}
			else if (0 == strcmp(pTmp, "$EditSG"))
			{
				srv_type = ACSI_SRVC_TYPE_SELECTEDITSG;//切换编辑区SelectEditSG
			}
			else if (NULL != strstr(mms_info->p_variable_struct[0].c_item_id, "$SE$"))
			{
				srv_type = ACSI_SRVC_TYPE_SETSGVALS;//修改定值SetSGValues
			}
			else if (NULL != strstr(mms_info->p_variable_struct[0].c_item_id, "$CO$"))
			{
				if (0 == strcmp(pTmp, "$Oper"))
				{
					srv_type = ACSI_SRVC_TYPE_OPERATE;//操作Operate
				}
				else if (0 == strcmp(pTmp, "$SBO"))
				{
					srv_type = ACSI_SRVC_TYPE_SELECT;//选择Select
				}
				else if (0 == strcmp(pTmp, "$SBOw"))
				{
					srv_type = ACSI_SRVC_TYPE_SELECTWITHVAL;//带值选择SelectWithValue
				}
				else if (0 == strcmp(pTmp, "$Cancel"))
				{
					srv_type = ACSI_SRVC_TYPE_CANCEL;
				}
			}
			else
			{
				srv_type = get_acsi_srv_type_domid_itemid(mms_info->p_variable_struct[0].c_domain_id, mms_info->p_variable_struct[0].c_item_id,MMS_SRVC_TYPE_WRITE, ied);
			}
		}

		if (ACSI_SRVC_TYPE_UNKNOWN == srv_type)
		{
			srv_type = ACSI_SRVC_TYPE_SETDATAVALS;//默认SetDataValues
		}
	}

	return srv_type;
}

/**
* 获得MMS_DATA_STRUCT对应的成员，并赋值，val直接赋指针，DataSet，GoID，RptID特殊处理（对象空间已经申请了内存），拷贝字符串
* obj                   对象空间指针
* p_data_struct         mms对象信息
* 成功或者失败
*/
static ST_RET _set_obj_val(ACSI_COMMON_OBJ* obj, MMS_DATA_STRUCT *p_data_struct)
{
	ACSI_OBJ_CTRL *obj_ctrl = NULL;
	ACSI_OBJ_CTRL *sub_obj_ctrl = NULL;
	MMS_DATA_STRUCT * sub_data_struct = NULL;
	ST_INT        i=0;
	
	//入口判断
	if (NULL==obj || NULL==p_data_struct)
	{
		return SD_FAILURE;
	}
/*	if (obj->obj_type!=OBJECT_TYPE_FCD && obj->obj_type!=OBJECT_TYPE_FCDA)
	{
		return SD_FAILURE;
	}*/
	obj_ctrl = (ACSI_OBJ_CTRL *)obj;
	if (p_data_struct->n_struct_num >0)
	{
		for (i=0; i<p_data_struct->n_struct_num; i++)
		{
			sub_obj_ctrl = &obj_ctrl->sub_objs[i];
			sub_data_struct = &p_data_struct->p_struct[i];
			_set_obj_val((ACSI_COMMON_OBJ*)sub_obj_ctrl, sub_data_struct);
		}
	}
	else
	{
		if (strcmp(obj_ctrl->comn_obj.name, "DataSet") == 0
			|| strcmp(obj_ctrl->comn_obj.name, "GoID") == 0 
			|| strcmp(obj_ctrl->comn_obj.name, "RptID") == 0)//DataSet，GoID，RptID特殊处理（对象空间已经申请了内存），拷贝字符串
		{
			strcpy(obj_ctrl->val, p_data_struct->c_data);
		}
		else
		{
			obj_ctrl->val = p_data_struct->c_data;
		}
	}
	return SD_SUCCESS;
}

/**
* 获得MMS_DATA_STRUCT对应的成员，并赋值，val直接赋指针，DataSet，GoID，RptID特殊处理（对象空间已经申请了内存），拷贝字符串
* obj                   对象空间指针
* p_data_struct         mms对象信息
* 成功或者失败
*/
static ST_RET _set_new_obj_val(ACSI_COMMON_OBJ* obj, MMS_DATA_STRUCT *p_data_struct)
{
	ACSI_OBJ_CTRL *obj_ctrl = NULL;
	ACSI_OBJ_CTRL *sub_obj_ctrl = NULL;
	MMS_DATA_STRUCT * sub_data_struct = NULL;
	ST_INT        i=0;

	//入口判断
	if (NULL==obj || NULL==p_data_struct)
	{
		return SD_FAILURE;
	}

	obj_ctrl = (ACSI_OBJ_CTRL *)obj;
	if (p_data_struct->n_struct_num >0)
	{
		obj_ctrl->sub_obj_num = p_data_struct->n_struct_num;
		obj_ctrl->sub_objs = chk_calloc(obj_ctrl->sub_obj_num, sizeof(ACSI_OBJ_CTRL));
		for (i=0; i<obj_ctrl->sub_obj_num; i++)
		{
			sub_data_struct = &p_data_struct->p_struct[i];
			_set_new_obj_val((ACSI_COMMON_OBJ*)(&obj_ctrl->sub_objs[i]), sub_data_struct);
		}
	}
	else
	{
		obj_ctrl->val = p_data_struct->c_data;
	}
	return SD_SUCCESS;

}

/**
* 将字符串反序
* s                   字符串
* void
*/
//static void Reverse(char *s)
//{
//	ST_INT i=0;
//	ST_INT j=0;
//	char cTemp;
//
//	if (NULL==s)
//	{
//		return;
//	}
//
//	for (i=0; i<strlen(s)-1; i++)
//	{
//		for (j=strlen(s)-1; j>0; j--)
//		{
//			cTemp=s[i];
//			s[i]=s[j];
//			s[j]=cTemp;
//		}
//	}
//}

/**
* 求二进制数bit中位为1的位数
* bit                   二进制数
* 位为1的位数
*/
ST_INT bit_len(ST_CHAR *bit)
{
	long i=0;
	ST_INT nLen = 0;

	//入口判断
	if (NULL == bit)
	{
		return -1;//失败
	}
	i = strlen(bit)-1;
	while (i>=0)
	{
		if (bit[i]!='0' && bit[i]!='1')
		{
			return -1;
		}
		if (bit[i] == '1')
		{
			nLen++;
		}

		i--;
	}
	return nLen;
}

/**
* 去掉头部空格
* s                    字符串
* void
*/
void ltrim(char *s)
{// 去掉前部的
	int l=0,p=0,k=0;
	l = strlen(s);
	if( l == 0 ) return;
	p = 0;
	while( s[p] == ' ' || s[p] == '\t' )  p++;
	if( p == 0 ) return;
	while( s[k] != '\0') s[k++] = s[p++];
	return;
}

/**
* 去掉尾部空格
* s                    字符串
* void
*/
void rtrim(char *s)
{// 去掉尾部的
	int l=0,p=0;
	l = strlen(s);
	if( l == 0 ) return;
	p = l -1;
	while( s[p] == ' ' || s[p] == '\t' ) {
		s[p--] = '\0';
		if( p < 0 ) break;
	}
	return;
}

/**
* 将4位的16进制字符串转换为16位二进制数
* cHex                    4位的16进制字符串
* 成功或失败
*/
ST_RET Hex2Bin(char *cHex)
{
	ST_CHAR cTemp[255];
	long l_Temp;
	if (NULL == cHex)
	{
		return SD_FAILURE;
	}
	memset(cTemp,0,255);
	ltrim(cHex);
	rtrim(cHex);

	if (strlen(cHex)!= 4)
	{
		return SD_FAILURE;
	}
	
	l_Temp=strtoul(cHex,NULL,16);
	itoa(l_Temp, cHex, 2); 

	//前面补齐0
	while (strlen(cHex)<16)
	{
		strcpy(cTemp, "0");
		strcat(cTemp, cHex);
		strcpy(cHex, cTemp);
	}
	return SD_SUCCESS;
}

/**
* 将4位的16进制字符串转换为16位二进制数，并只保留前10位
* cHex                    4位的16进制字符串
* 成功或失败
*/
ST_RET Conver_char_hex(char *cOptFls_Hex)
{
	ST_CHAR cTemp[255];

	memset(cTemp, 0, 255);
	if (SD_FAILURE == Hex2Bin(cOptFls_Hex))
	{
		return SD_FAILURE;
	}

	memcpy(cTemp, cOptFls_Hex, 10);
	strcpy(cOptFls_Hex, cTemp);
	return SD_SUCCESS;
}

/**
* 从61850对象参引中找出对象名称
* Objref                    61850对象参引
* 对象名称，NULL表示失败
*/
char* FindObjname(char *Objref)
{
	char *pObjname = NULL;
	if (NULL == Objref)
	{
		return NULL;
	}

	pObjname = strstr(Objref, "$");
	if (NULL != pObjname)
	{
		pObjname++;
	}
	return pObjname;
}

/**
* 获取rcb所属的LD
* rcb                    报告控制块对象空间
* LD对象空间，NULL表示失败
*/
ACSI_LD_CTRL *get_ld_rcb(ACSI_OBJ_CTRL *rcb)
{
	ACSI_LD_CTRL *pLd = NULL;
	ACSI_COMMON_OBJ *pFc = NULL;
	ACSI_LN_CTRL  *pLn = NULL;

	if (NULL == rcb)
	{
		return NULL;
	}

	pFc = (ACSI_COMMON_OBJ*)(rcb->comn_obj.parent);
	if (NULL == pFc)
	{
		return NULL;
	}
	pLn = (ACSI_LN_CTRL *)pFc->parent;
	if (NULL == pLn)
	{
		return  NULL;
	}
	pLd = (ACSI_LD_CTRL*)(pLn->comn_obj.parent);

	return pLd;
}

/**
* 获取rcb的dataset属性
* rcb                    报告控制块对象空间
* data_set_str           输出数据集名称
* 成功或失败
*/
ST_RET get_dsname_rcb(ACSI_OBJ_CTRL *rcb, ST_CHAR *data_set_str)
{
	ST_INT i=0;
	ACSI_OBJ_CTRL* rcb_attr = NULL;
	char *pdsname = NULL;

	if (NULL==rcb || NULL==data_set_str)
	{
		return SD_FAILURE;
	}

	for (i=0; i<rcb->sub_obj_num; i++)
	{
		rcb_attr = &rcb->sub_objs[i];
		if (strcmp(rcb_attr->comn_obj.name, "DataSet") == 0)
		{
			if (NULL != rcb_attr->val)
			{
				pdsname = strrchr(rcb_attr->val,'$');
				if (NULL != pdsname)//解析scd文件保存的data_set
				{
					pdsname++;
					strcpy(data_set_str,pdsname);
				}
				else
					strcpy(data_set_str,rcb_attr->val);
				return SD_SUCCESS;
			}
		}
	}

	return SD_FAILURE;
}

/**
* rpt服务转换函数
* acsi_info         acsi服务信息
* mms_ctrl          mms服务信息
* dest_ied          ied
* 成功或者失败
*/
static ST_RET _creat_acsi_rpt(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl, ACSI_IED_CTRL *source_ied)
{
	MMS_DATA_STRUCT *p_data = NULL;
	ST_CHAR* OptFlds= NULL;
	ST_CHAR* includebit= NULL;
	ST_CHAR  data_set_str[MAX_IDENT_LEN +1 ]= {0};
	ST_INT i=0;
	ST_INT j=0;
	ST_INT index = 2;
	ST_INT ds_index = 2;
	ACSI_DS_CTRL * ds_ctrl = NULL;
	ST_INT obj_num = 0;
	ACSI_OBJ_CTRL* obj_ptr = NULL;
//	long n_temp;
	ST_CHAR *pObjname = NULL;
	ACSI_LD_CTRL *pLd = NULL;

	//入口判断
	if (NULL==acsi_info || NULL==mms_ctrl || NULL==source_ied)
	{
		return SD_FAILURE;
	}
	acsi_info->srvc_type = ACSI_SRVC_TYPE_REPORT;

	//获取报告控制块rptid
	p_data = &mms_ctrl->p_data_struct[0];

	acsi_info->rcb = acsi_get_rcb_rptid(source_ied, p_data->c_data);
	if (acsi_info->rcb == NULL)
	{
		return SD_FAILURE;
	}
	//通过rcb获取所属的LD
	pLd = get_ld_rcb(acsi_info->rcb);
	if (NULL == pLd)
	{
		return SD_FAILURE;
	}
	//获取rcb的数据集名称
	get_dsname_rcb(acsi_info->rcb, data_set_str);

	//将OptFlds转换为二进制数，并只取前10位
	OptFlds = mms_ctrl->p_data_struct[1].c_data;
////////////////////////////////////del 2010/11/29
//	if (SD_FAILURE == Conver_char_hex(OptFlds))
//	{
//		return SD_FAILURE;
//	}
/////////////////////////////////////////
	if (OptFlds[4] == '0'&& strlen(data_set_str) == 0) //对象空间中和报文中都没有dataset，无法解析
	{
		return SD_FAILURE;
	}
	if (OptFlds[4] == '1')//包含数据集,判断数据集与scd文件中配置的数据集是否一致
	{
		if (OptFlds[1] == '1')
		{
			ds_index ++;
		}
		if (OptFlds[2] == '1')
		{
			ds_index ++;
		}
		//从报文中的数据集引用获取数据集的名称
		pObjname = FindObjname(mms_ctrl->p_data_struct[ds_index].c_data);
		if (NULL == pObjname)
		{
			return SD_FAILURE;
		}
		if (strlen(data_set_str) > 0)
		{
			//对象空间中和报文中dataset不一致，返回失败
			if (0 != strcmp(data_set_str, pObjname))
			{
				return SD_FAILURE;
			}
		}
		//根据网络报文里的dsname，从对象空间得到数据集
		ds_ctrl = acsi_get_ds_dsname(source_ied, pLd->comn_obj.name, pObjname);
	}
	else
	{
		ds_ctrl = acsi_get_ds_dsname(source_ied, pLd->comn_obj.name, data_set_str);
	}
	if (ds_ctrl == NULL)
	{
		return SD_FAILURE;
	}

	for (i=0; i<10; i++)
	{
		if (i==0 || i == 3 || i == 5)
		{
			continue;
		}
		if (OptFlds[i] == '1')
		{
			index ++;
			if (i == 9)
			{
				index ++;
			}
		}
	}
	
	//includebit,转换为2进制表示
	includebit = mms_ctrl->p_data_struct[index].c_data;
////////////////////////////////////del 2010/11/29，因为在AnalyzeTxt中已经转换
//	n_temp=strtoul(includebit,NULL,16);
//	ltoa(n_temp,includebit,2); 
//	if (bit_len(includebit) != ds_ctrl->member_num)
////////////////////////////////////del 2010/11/29
	if (strlen(includebit) != ds_ctrl->member_num)
	{
		return SD_FAILURE;
	}
	for (i=0; i<ds_ctrl->member_num; i++)//计算实际数据个数
	{
		if (includebit[i] == '1')
		{
			obj_num ++;
		}
	}
	
	if (OptFlds[5] == '1')
	{
		index = index + obj_num;//跳过数据集成员参引
		index++;//指向val
	}
	else
	{
		index ++;//指向val
	}

	//报告元素的val
	acsi_info->obj_num = obj_num;
	acsi_info->obj = chk_calloc(obj_num, sizeof(ACSI_OBJ_CTRL *));
	for (i=0; i<ds_ctrl->member_num; i++)
	{
		if (includebit[i] == '1')
		{
			acsi_info->obj[j] = (ACSI_COMMON_OBJ*)(ds_ctrl->ds_members[i]);
			_set_obj_val(acsi_info->obj[j], &mms_ctrl->p_data_struct[index]);
			index ++;
			j ++;
		}
	}

	//报告的原因
	acsi_info->rpt_reasons = chk_calloc(obj_num, sizeof(ST_CHAR *));
	for (i=0; i<obj_num; i++)
	{
		acsi_info->rpt_reasons[i] = mms_ctrl->p_data_struct[index].c_data;
		index ++;
	}
	return SD_SUCCESS;
}
/**
* write服务转换函数
* acsi_info         acsi服务信息
* mms_ctrl          mms服务信息
* dest_ied          ied
* 成功或者失败
*/
static ST_RET _creat_acsi_write_req(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl, ACSI_IED_CTRL *dest_ied)
{
	ACSI_COMMON_OBJ *comm_obj = NULL;
	ST_CHAR         ref[MAX_IDENT_LEN +1] = {0};
	MMS_VARIABLE_STRUCT *variable = NULL;
	MMS_DATA_STRUCT     *data = NULL;
	ST_INT i = 0;

	//入口判断
	if (NULL==acsi_info || NULL==mms_ctrl || NULL==dest_ied)
	{
		return SD_FAILURE;
	}

	acsi_info->srvc_type = get_acsi_srv_type(mms_ctrl, dest_ied);//写服务，其他控制服务写定值服务等需要再做详细处理
	acsi_info->obj_num = mms_ctrl->n_variable_num;
	acsi_info->obj = chk_calloc(acsi_info->obj_num, sizeof(ACSI_COMMON_OBJ *));

	for (i=0; i<mms_ctrl->n_variable_num; i++)
	{
		variable = &mms_ctrl->p_variable_struct[i];
		strcpy(ref, variable->c_domain_id);
		strcat(ref, "/");
		strcat(ref, variable->c_item_id);
		comm_obj = acsi_get_obj_ref(dest_ied, ref);//获得对象空间中ref对应的obj的指针
		if (comm_obj == NULL)
		{
			return SD_FAILURE;
		}
		acsi_info->obj[i] = comm_obj;
		data = &mms_ctrl->p_data_struct[i];
		if (_set_obj_val(comm_obj, data) == SD_FAILURE)
		{
			return SD_FAILURE;
		}
	}
	return SD_SUCCESS;
}

/**
* read服务转换函数
* acsi_info         acsi服务信息
* mms_ctrl          mms服务信息
* dest_ied          ied
* 成功或者失败
*/
/*static ST_RET _creat_acsi_read_req(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl, ACSI_IED_CTRL *dest_ied)
{
	ACSI_COMMON_OBJ *comm_obj = NULL;
	ST_CHAR         ref[MAX_IDENT_LEN +1] = {0};
	MMS_VARIABLE_STRUCT *variable = NULL;
	MMS_DATA_STRUCT     *data = NULL;
	ST_INT i = 0;

	//入口判断
	if (NULL==acsi_info || NULL==mms_ctrl || NULL==dest_ied)
	{
		return SD_FAILURE;
	}

	//服务类型
	acsi_info->srvc_type = get_acsi_srv_type(mms_ctrl, dest_ied);
	acsi_info->obj_num = mms_ctrl->n_variable_num;
	acsi_info->obj = chk_calloc(acsi_info->obj_num, sizeof(ACSI_COMMON_OBJ *));

	//变量
	for (i=0; i<mms_ctrl->n_variable_num; i++)
	{
		variable = &mms_ctrl->p_variable_struct[i];
		strcpy(ref, variable->c_domain_id);
		strcat(ref, "/");
		strcat(ref, variable->c_item_id);
		comm_obj = acsi_get_obj_ref(dest_ied, ref);//获得对象空间中ref对应的obj的指针
		if (comm_obj == NULL)
		{
			return SD_FAILURE;
		}
		acsi_info->obj[i] = comm_obj;
	}

	return SD_SUCCESS;
}*/

ACSI_DS_CTRL *get_acsi_ds_ctrl(MMS_INFO_STRUCT * mms_ctrl ,ACSI_IED_CTRL *dest_ied)
{
	ACSI_COMMON_OBJ *comm_obj = NULL;
	ST_CHAR         ref[MAX_IDENT_LEN +1] = {0};
	MMS_VARIABLE_STRUCT *variable = NULL;
	MMS_DATA_STRUCT     *data = NULL;
	ST_INT i = 0;
	ACSI_DS_CTRL *ds = NULL;

	variable = &mms_ctrl->p_variable_struct[0];
	strcpy(ref, variable->c_domain_id);
	strcat(ref, "/");
	strcat(ref, variable->c_item_id);
	comm_obj = acsi_get_obj_ref(dest_ied, ref);//获得对象空间中ref对应的obj的指针
	if (comm_obj == NULL)
	{
		return NULL;
	}
	ds = (ACSI_DS_CTRL*)comm_obj;

	return ds;
}

static ST_RET _creat_acsi_read_req(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl, ACSI_IED_CTRL *dest_ied)
{
	ACSI_COMMON_OBJ *comm_obj = NULL;
	ST_CHAR         ref[MAX_IDENT_LEN +1] = {0};
	MMS_VARIABLE_STRUCT *variable = NULL;
	ST_INT i = 0;
	ACSI_DS_CTRL *ds = NULL;

	//入口判断
	if (NULL==acsi_info || NULL==mms_ctrl || NULL==dest_ied)
	{
		return SD_FAILURE;
	}

	//服务类型
	acsi_info->srvc_type = get_acsi_srv_type(mms_ctrl, dest_ied);

	//变量
	if (acsi_info->srvc_type == ACSI_SRVC_TYPE_GETDSVALS)//added on 20110821
	{
		ds = get_acsi_ds_ctrl(mms_ctrl, dest_ied);
		acsi_info->obj_num = ds->member_num;
		acsi_info->obj = chk_calloc(acsi_info->obj_num, sizeof(ACSI_COMMON_OBJ *));
		for (i=0; i<acsi_info->obj_num; i++)
		{
			acsi_info->obj[i] = &ds->ds_members[i]->comn_obj;
		}

		return SD_SUCCESS;
	}
	
	acsi_info->obj_num = mms_ctrl->n_variable_num;
	acsi_info->obj = chk_calloc(acsi_info->obj_num, sizeof(ACSI_COMMON_OBJ *));
	for (i=0; i<mms_ctrl->n_variable_num; i++)
	{
		variable = &mms_ctrl->p_variable_struct[i];
		strcpy(ref, variable->c_domain_id);
		strcat(ref, "/");
		strcat(ref, variable->c_item_id);
		comm_obj = acsi_get_obj_ref(dest_ied, ref);//获得对象空间中ref对应的obj的指针
		if (comm_obj == NULL)
		{
			return SD_FAILURE;
		}
		acsi_info->obj[i] = comm_obj;
	}

	return SD_SUCCESS;
}

/**
* filedirectory服务转换函数
* acsi_info         acsi服务信息
* mms_ctrl          mms服务信息
* 成功或者失败
*/
static ST_RET _creat_acsi_filedirectory_req(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl/*, ACSI_IED_CTRL *dest_ied*/)
{
	ACSI_COMMON_OBJ *comm_obj = NULL;
	ST_CHAR         ref[MAX_IDENT_LEN +1] = {0};
	MMS_VARIABLE_STRUCT *variable = NULL;
	MMS_DATA_STRUCT     *data = NULL;
	ST_INT i = 0;
	char *p_tmp = NULL;

	//入口判断
	if (NULL==acsi_info || NULL==mms_ctrl)
	{
		return SD_FAILURE;
	}

	//服务类型
	p_tmp = strchr(mms_ctrl->mms_file_directory.c_file_request_directory,'.');
	if (NULL != p_tmp)
		acsi_info->srvc_type = ACSI_SRVC_TYPE_GETFILEATTR;
	else 
		acsi_info->srvc_type = ACSI_SRVC_TYPE_GETFILEDIRECTORY;

	//文件目录
	strcpy(acsi_info->file_directory, mms_ctrl->mms_file_directory.c_file_request_directory);

	return SD_SUCCESS;
}

/**
* getfile服务转换函数
* acsi_info         acsi服务信息
* mms_ctrl          mms服务信息
* 成功或者失败
*/
static ST_RET _creat_acsi_getfile_req(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl)
{
	ST_RET nRet = SD_SUCCESS;

	//入口判断
	if (NULL==acsi_info || NULL==mms_ctrl)
	{
		return SD_FAILURE;
	}

	acsi_info->srvc_type = get_acsiSrvType_mmsSrvType(mms_ctrl->c_service_type);

	switch(acsi_info->srvc_type)
	{
		case ACSI_SRVC_TYPE_GETFILE_OPEN:
			strcpy(acsi_info->file_srv_param.src_fname, mms_ctrl->struct_file_srv_param.c_src_fname);
			break;

		case ACSI_SRVC_TYPE_GETFILE_READ:
			break;

		case ACSI_SRVC_TYPE_GETFILE_CLOSE:
			break;

		default:
			nRet = SD_FAILURE;
			break;
	}

	return nRet;
}

/**
* Initiate转换函数
* acsi_info         acsi服务信息
* mms_ctrl          mms服务信息
* 成功或者失败
*/
static ST_RET _create_acsi_initiate_req(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl)
{
	if (NULL==acsi_info || NULL==mms_ctrl)
	{
		return SD_FAILURE;
	}
	//服务类型
	acsi_info->srvc_type = ACSI_SRVC_TYPE_ASSOCIATE;

	return SD_SUCCESS;
}

/**
* Conclude转换函数
* acsi_info         acsi服务信息
* mms_ctrl          mms服务信息
* 成功或者失败
*/
ST_RET _create_acsi_conclude_req(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl)
{
	if (NULL==acsi_info || NULL==mms_ctrl)
	{
		return SD_FAILURE;
	}
	//服务类型
	acsi_info->srvc_type = ACSI_SRVC_TYPE_RELEASE;

	return SD_SUCCESS;
}

/**
* Abort转换函数
* acsi_info         acsi服务信息
* mms_ctrl          mms服务信息
* 成功或者失败
*/
ST_RET _create_acsi_abort_req(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl)
{
	if (NULL==acsi_info || NULL==mms_ctrl)
	{
		return SD_FAILURE;
	}
	//服务类型
	acsi_info->srvc_type = ACSI_SRVC_TYPE_ABORT;

	return SD_SUCCESS;
}

/**
* setfile服务转换函数
* acsi_info         acsi服务信息
* mms_ctrl          mms服务信息
* 成功或者失败
*/
static ST_RET _creat_acsi_setfile_req(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl)
{
	//入口判断
	if (NULL==acsi_info || NULL==mms_ctrl)
	{
		return SD_FAILURE;
	}

	acsi_info->srvc_type = ACSI_SRVC_TYPE_SETFILE;

	strcpy(acsi_info->file_srv_param.src_fname, mms_ctrl->struct_file_srv_param.c_src_fname);
	strcpy(acsi_info->file_srv_param.dest_fname, mms_ctrl->struct_file_srv_param.dest_fname);
	
	return SD_SUCCESS;
}

static ST_RET _creat_acsi_deletefile_req(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl)
{
	//入口判断
	if (NULL==acsi_info || NULL==mms_ctrl)
	{
		return SD_FAILURE;
	}

	acsi_info->srvc_type = ACSI_SRVC_TYPE_DELETEFILE;

	strcpy(acsi_info->file_srv_param.src_fname, mms_ctrl->struct_file_srv_param.c_src_fname);

	return SD_SUCCESS;
}
/*
static ST_RET _creat_acsi_getnamelist_req(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl, ACSI_IED_CTRL *dest_ied)
{
	ACSI_COMMON_OBJ *comm_obj = NULL;
	ACSI_OBJ_CTRL *obj = NULL;
	//入口判断
	if (NULL==acsi_info || NULL==mms_ctrl || NULL==dest_ied)
	{
		return SD_FAILURE;
	}
	if (NULL==mms_ctrl->p_variable_struct || mms_ctrl->n_variable_num <= 0)
	{
		return SD_FAILURE;
	}

	if (0==strcmp(mms_ctrl->p_variable_struct->c_item_id,"IED"))//GetServerDirectory
	{
		acsi_info->srvc_type = ACSI_SRVC_TYPE_GETSERVERDIR;
		acsi_info->obj_num = mms_ctrl->n_variable_num;
		acsi_info->obj = chk_calloc(acsi_info->obj_num, sizeof(ACSI_COMMON_OBJ *));
		
/////		acsi_info->obj[0] = &dest_ied->comn_obj;//modified on 20110821
		acsi_info->obj[0] = chk_calloc(1,sizeof(ACSI_COMMON_OBJ));
		memcpy(acsi_info->obj[0],&dest_ied->comn_obj,sizeof(ACSI_COMMON_OBJ));
		obj = (ACSI_OBJ_CTRL*)(acsi_info->obj[0]);
		obj->sub_obj_num = 0;
		obj->sub_objs = NULL;
		obj->val = NULL;
		
		return SD_SUCCESS;
	}

	//以下是20110820打开的，之前是注释掉的
	comm_obj = acsi_get_obj_ref(dest_ied, mms_ctrl->p_variable_struct->c_item_id);
	if (NULL==comm_obj)
	{
		SD_FAILURE;
	}

	if (comm_obj->obj_type == OBJECT_TYPE_LD)
	{
		acsi_info->srvc_type = ACSI_SRVC_TYPE_GETLOGICALDEVICEDIR;//GetLogicalDeviceDirectory
	}
//	else if (comm_obj->obj_type == OBJECT_TYPE_LN)
//	{
//		acsi_info->srvc_type = ACSI_SRVC_TYPE_GETLOGICALNODEDIR;//GetLogicalNodeDirectory
//	}

	acsi_info->obj_num = mms_ctrl->n_variable_num;
	acsi_info->obj = chk_calloc(acsi_info->obj_num, sizeof(ACSI_COMMON_OBJ *));
	acsi_info->obj[0] = comm_obj;

	return SD_SUCCESS;

	/////////////////////////

///	return SD_FAILURE;
}*/

static ST_RET _creat_acsi_getnamelist_req(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl, ACSI_IED_CTRL *dest_ied)
{
	ACSI_COMMON_OBJ *comm_obj = NULL;
	ACSI_OBJ_CTRL *obj = NULL;
	int i;

	//入口判断
	if (NULL==acsi_info || NULL==mms_ctrl || NULL==dest_ied)
	{
		return SD_FAILURE;
	}
	if (NULL==mms_ctrl->p_variable_struct || mms_ctrl->n_variable_num <= 0)
	{
		return SD_FAILURE;
	}

	if (0==strcmp(mms_ctrl->p_variable_struct->c_item_id,"IED"))//GetServerDirectory
	{
		acsi_info->srvc_type = ACSI_SRVC_TYPE_GETSERVERDIR;
	}
	else 
	{
		comm_obj = acsi_get_obj_ref(dest_ied, mms_ctrl->p_variable_struct->c_item_id);
		if (NULL==comm_obj)
		{
			return SD_FAILURE;
		}

		if (comm_obj->obj_type == OBJECT_TYPE_LD)
		{
			acsi_info->srvc_type = ACSI_SRVC_TYPE_GETLOGICALDEVICEDIR;//GetLogicalDeviceDirectory
		}
	}

	acsi_info->obj_num = mms_ctrl->n_variable_num;
	acsi_info->obj = chk_calloc(acsi_info->obj_num, sizeof(ACSI_OBJ_CTRL *));

	for (i=0; i<acsi_info->obj_num; i++)
	{
		acsi_info->obj[i] = chk_calloc(1,sizeof(ACSI_OBJ_CTRL));
		if (ACSI_SRVC_TYPE_GETSERVERDIR == acsi_info->srvc_type)
		{
			memcpy(acsi_info->obj[i],&dest_ied->comn_obj,sizeof(ACSI_COMMON_OBJ));
		}
		else 
		{
			comm_obj = acsi_get_obj_ref(dest_ied, mms_ctrl->p_variable_struct[i].c_item_id);
			memcpy(acsi_info->obj[i],comm_obj,sizeof(ACSI_COMMON_OBJ));
		}

		obj = (ACSI_OBJ_CTRL*)(acsi_info->obj[i]);
		obj->sub_obj_num = 0;
		obj->sub_objs = NULL;
		obj->val = NULL;
	}

	return SD_SUCCESS;
}

static ST_RET _creat_acsi_getvaraccessattr_req(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl, ACSI_IED_CTRL *dest_ied)
{
	ACSI_COMMON_OBJ *comm_obj =NULL;
	MMS_VARIABLE_STRUCT *p_var = NULL;
	char c_tmp[512];
	int i;

	//入口判断
	if (NULL==acsi_info || NULL==mms_ctrl || NULL==dest_ied)
	{
		return SD_FAILURE;
	}

	p_var = mms_ctrl->p_variable_struct;
	if (NULL==p_var)
	{
		return SD_FAILURE;
	}

	memset(c_tmp,0,sizeof(c_tmp));
	strcpy(c_tmp,p_var[0].c_domain_id);
	strcat(c_tmp,"/");
	strcat(c_tmp,p_var[0].c_item_id);
	comm_obj = acsi_get_obj_ref(dest_ied,c_tmp);
	if (NULL==comm_obj)
	{
		return SD_FAILURE;
	}
	
	if (comm_obj->obj_type == OBJECT_TYPE_LN)
	{
		acsi_info->srvc_type = ACSI_SRVC_TYPE_GETLOGICALNODEDIR;//GetLogicalNodeDirectory
	}
	else 
	{
		acsi_info->srvc_type  =  ACSI_SRVC_TYPE_GETDATADIR;//GetDataDirectory或GetDataDefinition?
	}

	acsi_info->obj_num = mms_ctrl->n_variable_num;
	acsi_info->obj = chk_calloc(acsi_info->obj_num, sizeof(ACSI_COMMON_OBJ *));
	for (i=0; i<acsi_info->obj_num; i++)
	{
		memset(c_tmp,0,sizeof(c_tmp));
		strcpy(c_tmp,p_var[i].c_domain_id);
		strcat(c_tmp,"/");
		strcat(c_tmp,p_var[i].c_item_id);
		comm_obj = acsi_get_obj_ref(dest_ied, mms_ctrl->p_variable_struct[i].c_item_id);

		acsi_info->obj[0] = comm_obj;
	}
	return SD_SUCCESS;
}

static ST_RET _creat_acsi_getnamevaraccessattrs_req(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl, ACSI_IED_CTRL *dest_ied)
{
	//入口判断
	if (NULL==acsi_info || NULL==mms_ctrl || NULL==dest_ied)
	{
		return SD_FAILURE;
	}

	_creat_acsi_getvaraccessattr_req(acsi_info,mms_ctrl,dest_ied);
	acsi_info->srvc_type  =  ACSI_SRVC_TYPE_GETDSDIR;//GetDataSetDirectory;

	return SD_SUCCESS;
}
static ST_RET _creat_acsi_defnamevarlist_req(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl, ACSI_IED_CTRL *dest_ied)
{
	//入口判断
	if (NULL==acsi_info || NULL==mms_ctrl || NULL==dest_ied)
	{
		return SD_FAILURE;
	}

	acsi_info->srvc_type  =  ACSI_SRVC_TYPE_CREATEDS;//CreateDataSet

	return SD_SUCCESS;
}
static ST_RET _creat_acsi_delnamevarlist_req(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl, ACSI_IED_CTRL *dest_ied)
{
	//入口判断
	if (NULL==acsi_info || NULL==mms_ctrl || NULL==dest_ied)
	{
		return SD_FAILURE;
	}

	acsi_info->srvc_type  =  ACSI_SRVC_TYPE_DELETEDS;//DeleteDataSet

	return SD_SUCCESS;
}

static ST_RET _creat_acsi_readjourl_req(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl, ACSI_IED_CTRL *dest_ied)
{
	//入口判断
	if (NULL==acsi_info || NULL==mms_ctrl || NULL==dest_ied)
	{
		return SD_FAILURE;
	}


	return SD_SUCCESS;
}

/**
* req服务转换函数
* acsi_info         acsi服务信息
* mms_ctrl          mms服务信息
* dest_ied          ied
* 成功或者失败
*/
static ST_RET _creat_acsi_req(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl, ACSI_IED_CTRL *dest_ied)
{
	//入口判断
	if (NULL==acsi_info || NULL==mms_ctrl)
	{
		return SD_FAILURE;
	}

	switch(get_mms_srvType_str(mms_ctrl->c_service_type))
	{
	case MMS_SRVC_TYPE_READ:		//读请求
		return _creat_acsi_read_req(acsi_info, mms_ctrl, dest_ied);

	case MMS_SRVC_TYPE_WRITE:		//写请求
		return _creat_acsi_write_req(acsi_info, mms_ctrl, dest_ied);

	case MMS_SRVC_TYPE_FILEDIRECTORY://文件目录请求
		return _creat_acsi_filedirectory_req(acsi_info, mms_ctrl);

	case MMS_SRVC_TYPE_FILEOPEN:	//打开文件
	case MMS_SRVC_TYPE_FILEREAD:	//读文件
	case MMS_SRVC_TYPE_FILECLOSE:	//关闭文件
		return _creat_acsi_getfile_req(acsi_info, mms_ctrl);

	case  MMS_SRVC_TYPE_INITIATE:	//启动
		return _create_acsi_initiate_req(acsi_info, mms_ctrl);

	case MMS_SRVC_TYPE_CONCLUDE:	//结束
		return _create_acsi_conclude_req(acsi_info, mms_ctrl);

	case MMS_SRVC_TYPE_P_ABORT:		//异常终止
		return _create_acsi_abort_req(acsi_info, mms_ctrl);

	case MMS_SRV_TYPE_OBTAIN_FILE: //obtainFile
		return _creat_acsi_setfile_req(acsi_info, mms_ctrl);

	case MMS_SRV_TYPE_FILEDELETE:
		return _creat_acsi_deletefile_req(acsi_info, mms_ctrl);

	case MMS_SRVC_TYPE_GETNAMELIST:				//GetNameList
		return _creat_acsi_getnamelist_req(acsi_info, mms_ctrl, dest_ied);
		break;

	case MMS_SRVC_TYPE_GETVARACCESSATTR:		//GetVariableAccessAttribute
		return _creat_acsi_getvaraccessattr_req(acsi_info, mms_ctrl, dest_ied); 
		break;

	case MMS_SRVC_TYPE_GETNAMEDVARLSTATTR:		//GetNamedVariableListAttributes
		return _creat_acsi_getnamevaraccessattrs_req(acsi_info, mms_ctrl, dest_ied);
		break;

	case MMS_SRVC_TYPE_DEFINENAMEDVARLST:		//DefineNamedVariableList
		return _creat_acsi_defnamevarlist_req(acsi_info, mms_ctrl, dest_ied);
		break;

	case MMS_SRVC_TYPE_DELETENAMEDVARLST:		//DeleteNamedVariableList
		return _creat_acsi_delnamevarlist_req(acsi_info, mms_ctrl, dest_ied);
		break;

	case MMS_SRVC_TYPE_READJOURNAL:				//ReadJournal
		return _creat_acsi_readjourl_req(acsi_info, mms_ctrl, dest_ied);
		break;

	default:
		return SD_FAILURE;
	}

	return SD_FAILURE;
}
/**
* write的resp服务转换函数
* acsi_info         acsi服务信息
* mms_ctrl          mms服务信息
* source_ied        ied
* 成功或者失败
*/
static ST_RET _creat_acsi_write_resp(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl, ACSI_IED_CTRL *source_ied)
{
	ST_INT i=0;

	//入口判断
	if (NULL==acsi_info || NULL==mms_ctrl || NULL==source_ied)
	{
		return SD_FAILURE;
	}

	acsi_info->srvc_type = get_acsi_srv_type(mms_ctrl, source_ied);//写服务，其他控制服务写定值服务等需要再做详细处理
	acsi_info->err_code = chk_calloc(mms_ctrl->n_data_num, sizeof(ACSI_ERR_CODE));
	
	for (i=0; i<mms_ctrl->n_data_num; i++)
	{
		acsi_info->err_code[i] = get_acsi_error_code_dataAccess(mms_ctrl->p_data_struct[i].result);
//		acsi_info->err_code[i] = mms_ctrl->p_data_struct[i].result;
	}
	return SD_SUCCESS;
}

/**
* read的resp服务转换函数
* acsi_info         acsi服务信息
* mms_ctrl          mms服务信息
* source_ied        ied
* 成功或者失败
*/
static ST_RET _creat_acsi_read_resp(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl, ACSI_IED_CTRL *source_ied)
{
	ST_INT					i=0;
	ACSI_COMMON_OBJ*		comm_obj = NULL;
	ST_CHAR					ref[MAX_IDENT_LEN +1] = {0};
	MMS_VARIABLE_STRUCT*	variable = NULL;
	MMS_DATA_STRUCT*		data = NULL;
//	ACSI_OBJ_CTRL*			pObj = NULL;

	//入口判断
	if (NULL==acsi_info || NULL==mms_ctrl || NULL==source_ied)
	{
		return SD_FAILURE;
	}
	memset(ref, 0, MAX_IDENT_LEN);
	acsi_info->srvc_type = get_acsi_srv_type(mms_ctrl, source_ied);//服务类型
	acsi_info->obj_num = mms_ctrl->n_data_num;//数据个数
	acsi_info->obj = chk_calloc(acsi_info->obj_num, sizeof(ACSI_OBJ_CTRL *));//给对象空间数组分配内存
	acsi_info->err_code = chk_calloc(mms_ctrl->n_data_num, sizeof(ACSI_ERR_CODE));//错误代码分配内存

	//变量和值
	for (i=0; i<mms_ctrl->n_data_num; i++)
	{
		acsi_info->err_code[i] = get_acsi_error_code_dataAccess(mms_ctrl->p_data_struct[i].result);
/////		acsi_info->err_code[i] = mms_ctrl->p_data_struct[i].result;

//		pObj = acsi_info->obj[i];
		acsi_info->obj[i] = chk_calloc(1, sizeof(ACSI_OBJ_CTRL));

		//设置acsi_info->obj[i]的值，acsi_info->obj[i]不是从SCD对象空间中获取的，而是新申请的内存
		_set_new_obj_val(acsi_info->obj[i], &mms_ctrl->p_data_struct[i]);

	}
	
	return SD_SUCCESS;
}

/**
* filedirectory的resp服务转换函数
* acsi_info         acsi服务信息
* mms_ctrl          mms服务信息
* 成功或者失败
*/
static ST_RET _create_acsi_filedirectory_resp(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl/*, ACSI_IED_CTRL *source_ied*/)
{
	ST_INT i=0;
	MMS_FILE_ENTRY_STRUCT *file_entry = NULL;

	//入口判断
	if (NULL==acsi_info || NULL==mms_ctrl)
	{
		return SD_FAILURE;
	}

	acsi_info->srvc_type = ACSI_SRVC_TYPE_GETFILEDIRECTORY;

	acsi_info->file_num = mms_ctrl->mms_file_directory.n_file_num;
	acsi_info->file_entry = (ACSI_FILE_ENTRY*)chk_calloc(mms_ctrl->mms_file_directory.n_file_num, sizeof(ACSI_FILE_ENTRY));
	for (i=0; i<mms_ctrl->mms_file_directory.n_file_num; i++)
	{
		strcpy(acsi_info->file_entry[i].file_name, mms_ctrl->mms_file_directory.p_file_entry_struct[i].c_file_name);
		acsi_info->file_entry[i].size_of_file = mms_ctrl->mms_file_directory.p_file_entry_struct[i].mms_file_attr.n_size_of_file;
		strcpy(acsi_info->file_entry[i].time_last_modified, mms_ctrl->mms_file_directory.p_file_entry_struct[i].mms_file_attr.c_time_last_modified);
	}
	
	return SD_SUCCESS;
}

/**
* getfile的resp服务转换函数
* acsi_info         acsi服务信息
* mms_ctrl          mms服务信息
* 成功或者失败
*/
static ST_RET _crate_acsi_getfile_resp(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl/*, ACSI_IED_CTRL *source_ied*/)
{
	ST_RET nRet = SD_SUCCESS;

	//入口判断
	if (NULL==acsi_info || NULL==mms_ctrl)
	{
		return SD_FAILURE;
	}

	//服务类型
	acsi_info->srvc_type = get_acsiSrvType_mmsSrvType(mms_ctrl->c_service_type);

	switch(acsi_info->srvc_type)
	{
		case ACSI_SRVC_TYPE_GETFILE_OPEN:
			break;

		case ACSI_SRVC_TYPE_GETFILE_READ:
			acsi_info->file_srv_param.file_data = mms_ctrl->struct_file_srv_param.p_file_data;
//			acsi_info->file_srv_param.more_follows = mms_ctrl->struct_file_srv_param.b_more_follows;
			break;

		case ACSI_SRVC_TYPE_GETFILE_CLOSE:
			break;

		default:
			nRet = SD_FAILURE;
			break;
	}

	return nRet;
}

/**
* MMS启动服务Initiate服务与ACSI服务的转换
* acsi_info			ACSI信息结构指针
* mms_ctrl			MMS信息结构指针
* 成功或失败
*/
ST_RET _create_acsi_initiate_resp(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl/*, ACSI_IED_CTRL *source_ied*/)
{
	if (NULL==acsi_info)
	{
		return SD_FAILURE;
	}
	//服务类型
	acsi_info->srvc_type = ACSI_SRVC_TYPE_ASSOCIATE;

	return SD_SUCCESS;
}

/**
*service-error的转换函数
* acsi_info			ACSI信息结构指针
* mms_ctrl			MMS信息结构指针
* 成功或失败
*/
ST_RET _create_acsi_conclude_resp(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl/*, ACSI_IED_CTRL *dest_ied*/)
{
	if (NULL==acsi_info || NULL==mms_ctrl)
	{
		return SD_FAILURE;
	}
	//服务类型
	acsi_info->srvc_type = ACSI_SRVC_TYPE_RELEASE;

	return SD_SUCCESS;
}

/**
*service-error的转换函数
* acsi_info			ACSI信息结构指针
* mms_ctrl			MMS信息结构指针
* 成功或失败
*/
ST_RET _create_acsi_abort_resp(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl/*, ACSI_IED_CTRL *source_ied*/)
{
	if (NULL==acsi_info || NULL==mms_ctrl)
	{
		return SD_FAILURE;
	}
	//服务类型
	acsi_info->srvc_type = ACSI_SRVC_TYPE_ABORT;

	return SD_SUCCESS;
}


ST_RET _create_acsi_deletefile_resp(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl)
{
	if (NULL==acsi_info || NULL==mms_ctrl)
	{
		return SD_FAILURE;
	}
	//服务类型
	acsi_info->srvc_type = ACSI_SRVC_TYPE_DELETEFILE;

	return SD_SUCCESS;
}

ST_RET _create_acsi_setfile_resp(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl)
{
	if (NULL==acsi_info || NULL==mms_ctrl)
	{
		return SD_FAILURE;
	}
	//服务类型
	acsi_info->srvc_type = ACSI_SRVC_TYPE_SETFILE;

	return SD_SUCCESS;
}

static ST_RET _creat_acsi_getnamelist_resp(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl, ACSI_IED_CTRL *dest_ied)
{
	ACSI_COMMON_OBJ *com_obj  =NULL;
	int i;
	ACSI_OBJ_CTRL *obj = NULL;
	ACSI_OBJ_CTRL *var_com_obj = NULL;
	//入口判断
	if (NULL==acsi_info || NULL==mms_ctrl || NULL==dest_ied)
	{
		return SD_FAILURE;
	}

	if (NULL==mms_ctrl->p_variable_struct)
	{
		return SD_FAILURE;
	}

	com_obj = acsi_get_obj_ref(dest_ied,mms_ctrl->p_variable_struct[0].c_item_id);
	if (NULL!=com_obj)
	{
		if (OBJECT_TYPE_LD == com_obj->obj_type)
			acsi_info->srvc_type = ACSI_SRVC_TYPE_GETSERVERDIR;
	}
	else
	{
		acsi_info->srvc_type = ACSI_SRVC_TYPE_GETLOGICALDEVICEDIR;
	}

	acsi_info->obj_num = mms_ctrl->n_variable_num;
	acsi_info->obj = chk_calloc(acsi_info->obj_num,sizeof(ACSI_OBJ_CTRL *));
	for (i=0; i<acsi_info->obj_num; i++)
	{
		acsi_info->obj[i] = chk_calloc(1,sizeof(ACSI_OBJ_CTRL));
		if (ACSI_SRVC_TYPE_GETSERVERDIR == acsi_info->srvc_type)
		{
			var_com_obj = acsi_get_obj_ref(dest_ied,mms_ctrl->p_variable_struct[i].c_item_id);
			memcpy(acsi_info->obj[i],var_com_obj,sizeof(ACSI_COMMON_OBJ));
		}
		else
		{
			strcpy(acsi_info->obj[i]->name,mms_ctrl->p_variable_struct[i].c_item_id);
			acsi_info->obj[i]->desc = NULL;
			acsi_info->obj[i]->parent = NULL;
		}
		obj = (ACSI_OBJ_CTRL*)acsi_info->obj[i];
		obj->sub_obj_num = 0;
		obj->sub_objs = NULL;
		obj->val = NULL;
	}
	
	return SD_SUCCESS;
}

int FillObj(MMS_DATA_STRUCT *p_data_struct,MMS_VARIABLE_STRUCT *p_variable_struct,ACSI_OBJ_CTRL* obj, int nIndex)
{
	int i=0;
	if (NULL==p_data_struct || NULL==p_variable_struct || NULL==obj)
	{
		return SD_FAILURE;
	}

	if (n_mdt_structure == p_data_struct->n_data_type)//结构数据类型
	{
		nIndex++;
		strcpy(obj->comn_obj.name, p_variable_struct[nIndex].c_item_id);
		obj->comn_obj.desc = NULL;
		obj->sub_obj_num = p_data_struct->n_struct_num;
		if (obj->sub_obj_num > 0)
		{
			obj->sub_objs = chk_calloc(obj->sub_obj_num,sizeof(ACSI_OBJ_CTRL));
		}
		for (i=0; i<obj->sub_obj_num; i++)
		{
			nIndex = FillObj(&p_data_struct->p_struct[i],p_variable_struct,&obj->sub_objs[i],nIndex);
		}
	}
	else
	{
		nIndex++;
		obj->val = chk_calloc(256, sizeof(char));
		strcpy(obj->val, p_data_struct[i].c_data);
		strcpy(obj->comn_obj.name, p_variable_struct[nIndex].c_item_id);
		obj->comn_obj.desc = NULL;
	}
	
	return nIndex;
}

static ST_RET _creat_acsi_getvaraccessattr_resp(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl, ACSI_IED_CTRL *dest_ied)
{
	int i;
	ACSI_OBJ_CTRL *obj = NULL;
	int nIndex = -1;

	//入口判断
	if (NULL==acsi_info || NULL==mms_ctrl || NULL==dest_ied)
	{
		return SD_FAILURE;
	}

	acsi_info->srvc_type  =  ACSI_SRVC_TYPE_GETLOGICALNODEDIR;//或GetDataDirectory或GetDataDefinition?

	if (mms_ctrl->n_data_num <= 0)
	{
		return SD_FAILURE;
	}
	acsi_info->obj_num = mms_ctrl->n_data_num;
	acsi_info->obj = chk_calloc(acsi_info->obj_num, sizeof(ACSI_COMMON_OBJ *));
	for (i=0; i<acsi_info->obj_num; i++)
	{
		acsi_info->obj[i] = chk_calloc(1,sizeof(ACSI_COMMON_OBJ));
		obj = (ACSI_OBJ_CTRL *)acsi_info->obj[i];
		obj->sub_obj_num = 0;
		obj->sub_objs = NULL;
		obj->val = NULL;

		nIndex = FillObj(&mms_ctrl->p_data_struct[i], mms_ctrl->p_variable_struct, obj, nIndex);
	}

	return SD_SUCCESS;
}

static ST_RET _creat_acsi_getnamevaraccessattrs_resp(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl, ACSI_IED_CTRL *dest_ied)
{
	ACSI_COMMON_OBJ *comm_obj = NULL;
	ST_CHAR         ref[MAX_IDENT_LEN +1] = {0};
	MMS_VARIABLE_STRUCT *variable = NULL;
	MMS_DATA_STRUCT     *data = NULL;
	ST_INT i = 0;
//	ACSI_DS_CTRL *ds = NULL;
//	ACSI_DS_CTRL *ds = NULL;

	//入口判断
	if (NULL==acsi_info || NULL==mms_ctrl || NULL==dest_ied)
	{
		return SD_FAILURE;
	}

	acsi_info->srvc_type  =  ACSI_SRVC_TYPE_GETDSDIR;//GetDataSetDirectory;

//	ds = get_acsi_ds_ctrl(mms_ctrl, dest_ied);

	if (NULL==mms_ctrl->p_variable_struct)
	{
		return SD_FAILURE;
	}
	if (mms_ctrl->n_variable_num > 0)
	{
		acsi_info->obj = chk_calloc(mms_ctrl->n_variable_num, sizeof(ACSI_COMMON_OBJ *));
	}
	acsi_info->obj_num = mms_ctrl->n_variable_num;
	for (i=0; i<acsi_info->obj_num; i++)
	{
		variable = &mms_ctrl->p_variable_struct[i];
		strcpy(ref, variable->c_domain_id);
		strcat(ref, "/");
		strcat(ref, variable->c_item_id);
		comm_obj = acsi_get_obj_ref(dest_ied, ref);//获得对象空间中ref对应的obj的指针
		
		acsi_info->obj[i] = comm_obj;
	}

	return SD_SUCCESS;
}
static ST_RET _creat_acsi_defnamevarlist_resp(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl, ACSI_IED_CTRL *dest_ied)
{
	//入口判断
	if (NULL==acsi_info || NULL==mms_ctrl || NULL==dest_ied)
	{
		return SD_FAILURE;
	}

	acsi_info->srvc_type  =  ACSI_SRVC_TYPE_CREATEDS;//CreateDataSet

	return SD_SUCCESS;
}
static ST_RET _creat_acsi_delnamevarlist_resp(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl, ACSI_IED_CTRL *dest_ied)
{
	//入口判断
	if (NULL==acsi_info || NULL==mms_ctrl || NULL==dest_ied)
	{
		return SD_FAILURE;
	}

	acsi_info->srvc_type  =  ACSI_SRVC_TYPE_DELETEDS;//DeleteDataSet

	return SD_SUCCESS;
}

static ST_RET _creat_acsi_readjourl_resp(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl, ACSI_IED_CTRL *dest_ied)
{
	//入口判断
	if (NULL==acsi_info || NULL==mms_ctrl || NULL==dest_ied)
	{
		return SD_FAILURE;
	}


	return SD_SUCCESS;
}

/**
* resp服务转换函数
* acsi_info         acsi服务信息
* mms_ctrl          mms服务信息
* source_ied        ied
* 成功或者失败
*/
static ST_RET _creat_acsi_resp(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl, ACSI_IED_CTRL *source_ied)
{
	//入口判断
	if (NULL==acsi_info || NULL==mms_ctrl/* || NULL==source_ied*/)//modified on 20110831
	{
		return SD_FAILURE;
	}

	switch(get_mms_srvType_str(mms_ctrl->c_service_type))
	{
	case MMS_SRVC_TYPE_READ:
		return _creat_acsi_read_resp(acsi_info, mms_ctrl, source_ied);

	case MMS_SRVC_TYPE_WRITE:
		return _creat_acsi_write_resp(acsi_info, mms_ctrl, source_ied);

	case MMS_SRVC_TYPE_FILEDIRECTORY:
		return _create_acsi_filedirectory_resp(acsi_info, mms_ctrl);

	case MMS_SRVC_TYPE_FILEOPEN:
	case MMS_SRVC_TYPE_FILEREAD:
	case MMS_SRVC_TYPE_FILECLOSE:
		return _crate_acsi_getfile_resp(acsi_info, mms_ctrl);

	case MMS_SRVC_TYPE_INITIATE:
		return _create_acsi_initiate_resp(acsi_info, mms_ctrl);

	case MMS_SRVC_TYPE_CONCLUDE:
		return _create_acsi_conclude_resp(acsi_info, mms_ctrl);

	case MMS_SRVC_TYPE_P_ABORT:
		return _create_acsi_abort_resp(acsi_info, mms_ctrl);

	case MMS_SRV_TYPE_OBTAIN_FILE:
		return _create_acsi_setfile_resp(acsi_info, mms_ctrl);

	case MMS_SRV_TYPE_FILEDELETE:
		return _create_acsi_deletefile_resp(acsi_info, mms_ctrl);

	case MMS_SRVC_TYPE_GETNAMELIST:				//GetNameList
		return _creat_acsi_getnamelist_resp(acsi_info, mms_ctrl, source_ied);
		break;

	case MMS_SRVC_TYPE_GETVARACCESSATTR:		//GetVariableAccessAttribute->GetDataDirectory
		return _creat_acsi_getvaraccessattr_resp(acsi_info, mms_ctrl, source_ied); 
		break;

	case MMS_SRVC_TYPE_GETNAMEDVARLSTATTR:		//GetNamedVariableListAttributes->GetDataSetDirectory
		return _creat_acsi_getnamevaraccessattrs_resp(acsi_info, mms_ctrl, source_ied);
		break;

	case MMS_SRVC_TYPE_DEFINENAMEDVARLST:		//DefineNamedVariableList->CreateDataSet
		return _creat_acsi_defnamevarlist_resp(acsi_info, mms_ctrl, source_ied);
		break;

	case MMS_SRVC_TYPE_DELETENAMEDVARLST:		//DeleteNamedVariableList->DeleteDataSet
		return _creat_acsi_delnamevarlist_resp(acsi_info, mms_ctrl, source_ied);
		break;

	case MMS_SRVC_TYPE_READJOURNAL:				//ReadJournal
		return _creat_acsi_readjourl_resp(acsi_info, mms_ctrl, source_ied);
		break;

	default:
		return SD_FAILURE;
	}
}

/**
*service-error的转换函数
* acsi_info		ACSI信息结构指针
* mms_ctrl		MMS信息结构指针
* 成功或失败
*/
ST_RET _create_acsi_srv_error(AP_ACSI_INFO *acsi_info, MMS_INFO_STRUCT *mms_ctrl)
{
	if (NULL==acsi_info || NULL==mms_ctrl)
	{
		return SD_FAILURE;
	}
	acsi_info->err_code = chk_calloc(1,sizeof(ACSI_ERR_CODE));
	acsi_info->err_code[0] = ACSI_ERR_CODE_UNKNOWN;

	//将MMS服务的ErrorClass和ErrorCode映射成ACSI的服务错误代码
	acsi_info->err_code[0] = get_acsi_error_code(mms_ctrl->struct_mms_service_error.n_error_class, mms_ctrl->struct_mms_service_error.n_error_code);

	return SD_SUCCESS;
}

/**
*将mms的rejectPdu、rejectReason映射成acsi的服务错误代码
* n_rejectPdu		MMS拒绝服务的PDU类型
* n_Reject_Reason	MMS拒绝服务的具体原因代号
* 返回ACSI_ERR_CODE
*/
ACSI_ERR_CODE get_acsi_error_code_rejectReason(REJECT_PDU n_rejectPdu, int n_Reject_Reason)
{
	int n_acsi_erro_code = ACSI_ERR_CODE_UNKNOWN;
	switch (n_rejectPdu)
	{
	case REJECT_CONFIRMED_REQUEST_PDU:
		switch (n_Reject_Reason)
		{
		case 0://other
		case 1://unrecognized-service
		case 6://max_serv_outstanding-exceeded
			n_acsi_erro_code = ACSI_ERR_CODE_FAILED_DUE_TO_SERVER_CONSTRAINT;
			break;

		default:
			break;

		}
		break;

	default:
		break;
	}

	return n_acsi_erro_code;
}

/**
* reject的转换函数
* acsi_info		ACSI信息结构指针
* mms_ctrl		MMS信息结构
*/
ST_RET _create_acsi_reject(AP_ACSI_INFO*acsi_info, MMS_INFO_STRUCT * mms_ctrl)
{
	if (NULL==acsi_info || NULL==mms_ctrl)
	{
		return SD_FAILURE;
	}
	acsi_info->err_code = chk_calloc(1,sizeof(ACSI_ERR_CODE));
	acsi_info->err_code[0] = ACSI_ERR_CODE_UNKNOWN;

	//将MMS服务的rejectReason映射成ACSI的服务错误代码
	acsi_info->err_code[0] = get_acsi_error_code_rejectReason(mms_ctrl->struct_mms_service_reject.n_reject_pdu,mms_ctrl->struct_mms_service_reject.n_reject_reason_code);
	return SD_SUCCESS;
}
/**
* 查找报告控制块对象指针
* mms_info         mms服务数组
* mms_num          mms服务个数
* AP_ACSI_INFO*    acsi数组，个数与传进的mms_num一致
*/
AP_ACSI_INFO* acsi_cre_info_mmsInfo(MMS_INFO_STRUCT *mms_info, ST_INT mms_num)
{
	AP_ACSI_INFO* acsi_info= NULL;
	ACSI_IED_CTRL *source_ied = NULL;
	ACSI_IED_CTRL *dest_ied = NULL;
	ST_INT i=0;

	//入口判断
	if (NULL==mms_info || 0==mms_num)
	{
		return NULL;
	}
	
	acsi_info = (AP_ACSI_INFO*)chk_calloc(mms_num, sizeof(AP_ACSI_INFO));
	for (i=0; i<mms_num; i++)
	{
		//PDU类型
		if (strstr(mms_info[i].c_pdu_type, "Request") != NULL)
		{
			acsi_info[i].pdu_type = ACSI_MMSPDU_REQ;
		}
		else if (strstr(mms_info[i].c_pdu_type, "Response") != NULL)
		{
			acsi_info[i].pdu_type = ACSI_MMSPDU_RESP;
		}
		else if (strstr(mms_info[i].c_pdu_type, "Error") != NULL)
		{
			acsi_info[i].pdu_type = ACSI_MMSPDU_SRVERROR;
		}
		else if (strstr(mms_info[i].c_pdu_type, "reject") != NULL)
		{
			acsi_info[i].pdu_type = ACSI_MMSPDU_REJECT;
		}
		else 
		{
			acsi_info[i].pdu_type = ACSI_MMSPDU_RPT;
		}
		//源地址对应的ied
		source_ied = acsi_get_ied_ip(mms_info[i].c_src_ip);
		//目的地址对应的ied
		dest_ied = acsi_get_ied_ip(mms_info[i].c_dest_ip);
		if (source_ied)
		{
			strcpy(acsi_info[i].source, source_ied->comn_obj.name);
		}
		if (dest_ied)
		{
			strcpy(acsi_info[i].dest, dest_ied->comn_obj.name);
		}
		//invokeID
		acsi_info[i].invoke_id = mms_info[i].n_invoke_id;
		//具体信息
		switch(acsi_info[i].pdu_type)
		{
		case ACSI_MMSPDU_RPT://报告
			_creat_acsi_rpt(&acsi_info[i], &mms_info[i], source_ied);
			break;
		case ACSI_MMSPDU_REQ://请求
			_creat_acsi_req(&acsi_info[i], &mms_info[i], dest_ied);
			break;
		case ACSI_MMSPDU_RESP://回复
			_creat_acsi_resp(&acsi_info[i], &mms_info[i], source_ied);
			break;
		case ACSI_MMSPDU_SRVERROR://服务失败
			_create_acsi_srv_error(&acsi_info[i], &mms_info[i]);
			break;
		case ACSI_MMSPDU_REJECT://拒绝
			_create_acsi_reject(&acsi_info[i], &mms_info[i]);
			break;
		default:
//			chk_free(acsi_info);//del by ljm
//			acsi_info = NULL;//del by ljm
			break;
		}
	}
	return acsi_info;
}

/**
* 将ACSI的服务类型映射到MMS的服务类型
* n_acsi_srv_type	ACSI的服务类型
*/
MMS_SRVC_TYPE get_mmsSrvType_acsiSrvType(ACSI_SRVC_TYPE n_acsi_srv_type)
{
	MMS_SRVC_TYPE n_mms_srv_type = MMS_SRVC_TYPE_UNKNOWN;

	switch (n_acsi_srv_type)
	{
	case ACSI_SRVC_TYPE_GETDATAVALS:
	case ACSI_SRVC_TYPE_GETALLDATAVALS:
	case ACSI_SRVC_TYPE_GETBRCBVALS:
	case ACSI_SRVC_TYPE_GETURCBVALS:
	case ACSI_SRVC_TYPE_GETSGCBVALS:
	case ACSI_SRVC_TYPE_GETSGVALS:
	case ACSI_SRVC_TYPE_GETDSVALS:
	case ACSI_SRVC_TYPE_GETGOCBVALS:
	case ACSI_SRVC_TYPE_GETGSCBVALS:
	case ACSI_SRVC_TYPE_GETMSVCBVALS:
	case ACSI_SRVC_TYPE_GETUSVCBVALS:
	case ACSI_SRVC_TYPE_GETLOGSTATUSVALS:
		n_mms_srv_type = MMS_SRVC_TYPE_READ;
		break;

	case ACSI_SRVC_TYPE_SETDATAVALS:
	case ACSI_SRVC_TYPE_SETBRCBVALS:
	case ACSI_SRVC_TYPE_SETURCBVALS:
	case ACSI_SRVC_TYPE_SETSGVALS:
	case ACSI_SRVC_TYPE_CONFIRMEDITSGVALS:
	case ACSI_SRVC_TYPE_SELECTACTSG:
	case ACSI_SRVC_TYPE_SELECTEDITSG:
	case ACSI_SRVC_TYPE_SETDSVALS:
	case ACSI_SRVC_TYPE_SELECT:
	case ACSI_SRVC_TYPE_SELECTWITHVAL:
	case ACSI_SRVC_TYPE_OPERATE:
	case ACSI_SRVC_TYPE_CANCEL:
	case ACSI_SRVC_TYPE_SETGOCBVALS:
	case ACSI_SRVC_TYPE_SETGSCBVALS:
	case ACSI_SRVC_TYPE_SETMSVCBVALS:
	case ACSI_SRVC_TYPE_SETUSVCBVALS:
		n_mms_srv_type = MMS_SRVC_TYPE_WRITE;
		break;

	case ACSI_SRVC_TYPE_GETSERVERDIR:
	case ACSI_SRVC_TYPE_GETLOGICALDEVICEDIR:
	case ACSI_SRVC_TYPE_GETLOGICALNODEDIR:
		n_mms_srv_type = MMS_SRVC_TYPE_GETNAMELIST;
		break;

	case ACSI_SRVC_TYPE_GETDSDIR:
		n_mms_srv_type = MMS_SRVC_TYPE_GETNAMEDVARLSTATTR;
		break;
	case ACSI_SRVC_TYPE_CREATEDS:
		n_mms_srv_type = MMS_SRVC_TYPE_DEFINENAMEDVARLST;
		break;
	case ACSI_SRVC_TYPE_DELETEDS:
		n_mms_srv_type = MMS_SRVC_TYPE_DELETENAMEDVARLST;
		break;

	case ACSI_SRVC_TYPE_GETDATADIR:
	case ACSI_SRVC_TYPE_GETDATADEF:
		n_mms_srv_type = MMS_SRVC_TYPE_GETVARACCESSATTR;
		break;

	case ACSI_SRVC_TYPE_QUERYLOGBYTIME:
	case ACSI_SRVC_TYPE_QUERYLOGAFTER:
		n_mms_srv_type = MMS_SRVC_TYPE_READJOURNAL;
		break;	

	case  ACSI_SRVC_TYPE_ASSOCIATE:
		n_mms_srv_type = MMS_SRVC_TYPE_INITIATE;
		break;

	case ACSI_SRVC_TYPE_RELEASE:
		n_mms_srv_type = MMS_SRVC_TYPE_CONCLUDE;
		break;
		
	case ACSI_SRVC_TYPE_ABORT:
		n_mms_srv_type = MMS_SRVC_TYPE_P_ABORT;
		 break;

	case ACSI_SRVC_TYPE_GETFILEDIRECTORY:
		n_mms_srv_type = MMS_SRVC_TYPE_FILEDIRECTORY;
		break;

	case ACSI_SRVC_TYPE_GETFILE_OPEN:
		n_mms_srv_type = MMS_SRVC_TYPE_FILEOPEN;
		break;

	case ACSI_SRVC_TYPE_GETFILE_READ:
		n_mms_srv_type = MMS_SRVC_TYPE_FILEREAD;
		break;

	case ACSI_SRVC_TYPE_GETFILE_CLOSE:
		n_mms_srv_type = MMS_SRVC_TYPE_FILECLOSE;
		break;

	case ACSI_SRVC_TYPE_SETFILE:
		n_mms_srv_type = MMS_SRV_TYPE_OBTAIN_FILE;
		break;

	case ACSI_SRVC_TYPE_DELETEFILE:
		n_mms_srv_type = MMS_SRV_TYPE_FILEDELETE;
		break;

	case ACSI_SRVC_TYPE_REPORT:
		n_mms_srv_type = MMS_SRVC_TYPE_INFO;
		break;
	}

	return n_mms_srv_type;

}

/**
*释放ACSI_OBJ_CTRL的sub_objs的资源
* obj_ctrl	ACSI_OBJ_CTRL结构指针
*/
ST_RET acsi_clean_acsi_sub_obj_ctrl(ACSI_OBJ_CTRL *obj_ctrl)
{
	ST_INT k = 0;
	ACSI_OBJ_CTRL *p_subObj = NULL;

	if (NULL == obj_ctrl)
	{
		return SD_FAILURE;
	}
	if (NULL != obj_ctrl->sub_objs)
	{
		for (k=0; k<obj_ctrl->sub_obj_num; k++)
		{
			p_subObj = &obj_ctrl->sub_objs[k];
			if (NULL != p_subObj)
			{
				if (p_subObj->sub_obj_num > 0)
				{
					//递归
					acsi_clean_acsi_sub_obj_ctrl(p_subObj);
				}
			}		
		}
		chk_free(obj_ctrl->sub_objs);
	}
	return SD_SUCCESS;
}

ST_VOID clean_sub_obj_ctrl_val(ACSI_OBJ_CTRL *pObj)
{
	int i;

	if (NULL == pObj)
	{
		return;
	}

	if (NULL != pObj->val)
	{
		chk_free(pObj->val);
	}

	if (NULL != pObj->sub_objs)
	{
		for(i=0; i<pObj->sub_obj_num; i++)
		{
			clean_sub_obj_ctrl_val(&pObj->sub_objs[i]);
		}
		chk_free(pObj->sub_objs);
	}
}
/**
* 释放acsi_infos信息
* acsi_infos	ACSI信息结构数组首地址
* num			数组大小
*/
ST_VOID acsi_clean_acsi_info(AP_ACSI_INFO* acsi_infos, ST_INT num)
{
	ST_INT i=0;
	ST_INT j=0;
	ST_INT k=0;
	AP_ACSI_INFO* acsi_info = NULL;
	ACSI_OBJ_CTRL*  pObj = NULL;

	//入口判断
	if (NULL == acsi_infos)
	{
		return;
	}
	for (i=0; i<num; i++)
	{	
		acsi_info = &acsi_infos[i];
		if (acsi_info->obj)
		{
			//MMS的read-request服务特殊处理
			if (acsi_info->pdu_type==ACSI_MMSPDU_RESP && MMS_SRVC_TYPE_READ==get_mmsSrvType_acsiSrvType(acsi_info->srvc_type))
			{
				for (j=0; j<acsi_info->obj_num; j++)
				{
					pObj = (ACSI_OBJ_CTRL*)(acsi_info->obj[j]);
					if (NULL != pObj)
					{
						acsi_clean_acsi_sub_obj_ctrl(pObj);
						chk_free(pObj);
					}
				}
				chk_free(acsi_info->obj);
			}
			else if (acsi_info->srvc_type==ACSI_SRVC_TYPE_GETSERVERDIR||acsi_info->srvc_type == ACSI_SRVC_TYPE_GETLOGICALDEVICEDIR)
			{
				for (i=0; i<acsi_info->obj_num; i++)
				{
					chk_free(acsi_info->obj[i]);
				}
				chk_free(acsi_info->obj);
			}
			else if (acsi_info->srvc_type==ACSI_SRVC_TYPE_GETLOGICALNODEDIR)
			{	
				for (j=0; j<acsi_info->obj_num; j++)
				{
					pObj = (ACSI_OBJ_CTRL*)(acsi_info->obj[j]);
					clean_sub_obj_ctrl_val(pObj);
				}
				chk_free(acsi_info->obj);
			}
			else
			{
				chk_free(acsi_info->obj);
			}
		}
		if (acsi_info->rpt_reasons)
		{
			chk_free(acsi_info->rpt_reasons);
		}
		if (acsi_info->err_code)
		{
			chk_free(acsi_info->err_code);
		}
		if (acsi_info->file_entry)
		{
			chk_free(acsi_info->file_entry);
		}
	}
	chk_free(acsi_infos);
}

/**
* @brief	function name:"acsi_init_envir"
* @use		初始化环境
* @param 	[type] ST_CHAR [name] scd_name SCD文件名称
* @return	0表示成功，1表示失败
*/
ST_RET acsi_init_envir(ST_CHAR *scd_name)
{
	SCD_INFO	scd_info = {0};
	ST_RET		nRet = SD_FAILURE;

	if (NULL == scd_name)
	{
		return SD_FAILURE;
	}

	//解析SCD
	nRet = ai_parse_scd(scd_name, &scd_info);
	if (SD_FAILURE == nRet)
	{
		return SD_FAILURE;
	}

	//创建所有的对象空间
	nRet = acsi_create_ied_all(&scd_info);

	//释放SCD信息结构
	ai_cleanup_scd_info(&scd_info);

	return nRet;
}

/**
* @brief	function name:"acsi_uninit_envir"
* @use		初始化环境
* @param 	无
* @return	0表示成功，1表示失败
*/
ST_RET acsi_uninit_envir()
{
	//释放所有的对象空间
	acsi_ied_destroy_all();

	return SD_SUCCESS;
}
