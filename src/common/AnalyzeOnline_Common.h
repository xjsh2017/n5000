#ifndef ANALYZE_COMMON_H
#define ANALYZE_COMMON_H

#include "AnalyzeOnline_STRUCT.h"
#include "mmsstruct.h"


typedef struct _ASDU_ERR_NUM_STRUCT
{
//	int n_asdu_seq;

	map<int, vector<int> > map_daseq_2_daerrs;

	vector<int> v_err_numbers;

	_ASDU_ERR_NUM_STRUCT()
	{
//		n_asdu_seq = 0;
		map_daseq_2_daerrs.clear();
		v_err_numbers.clear();
	}

}ASDU_ERR_NUM_STRUCT;

//报文错误结构
typedef struct _MESSAGE_ERROR_STRUCT
{
	MESSAGE_ERROR_CLASS n_error_class;//报文错误类号

	int n_appid;
	
	char c_avtm[MAX_ARV_TIME_LEN];//本帧报文的到达时间

	vector<int> v_err_numbers;//报文错误号链表//报文级错误

	map<int, ASDU_ERR_NUM_STRUCT> map_asdu_2_errs;//sv-asdu级别(测量通道数据)错误
	
	map<int, vector<int> > map_gs_daseq_2_errs;//gs-asdu级别(测量通道数据)错误

	_MESSAGE_ERROR_STRUCT()
	{
		n_error_class = ERROR_CLASS_UNKOWN;
		n_appid = 0;
		memset(c_avtm,0,sizeof(c_avtm));
		v_err_numbers.clear();
		map_asdu_2_errs.clear();
		map_gs_daseq_2_errs.clear();

	}

}MESSAGE_ERROR_STRUCT;//单个ASDU错误

#ifdef __cplusplus
extern "C" {
#endif

	/**
	* @brief	function name:"Releas_message_errs"
	* @use		释放结构MESSAGE_ERROR_INFO_ARRAY_STRUCT指针的内存
	* @param 	[type] MESSAGE_ERROR_INFO_ARRAY_STRUCT [name] p_message_errs MESSAGE_ERROR_INFO_ARRAY_STRUCT指针
	* @return	RES_SUCCESS 成功，RES_FAIL 失败
	*/
	int Releas_message_errs(MESSAGE_ERROR_INFO_ARRAY_STRUCT* p_message_errs);

	/**
	* @brief	function name:"create_message_errs"
	* @use		根据vector中的内容，创建结构MESSAGE_ERROR_STRUCT数组
	* @param 	[type] vector<MESSAGE_ERROR_STRUCT> [name] message_err_lst MESSAGE_ERROR_STRUCT结构链表
	* @return	结构MESSAGE_ERROR_INFO_ARRAY_STRUCT数组的首地址
	*/
	MESSAGE_ERROR_INFO_ARRAY_STRUCT* CreateMsgErrArray(vector<MESSAGE_ERROR_STRUCT> message_err_lst);
	/**
	* @brief	function name:"mac_error"
	* @use		判断字符串MAC地址有没有错误
	* @param 	[type] char [name] p_mac MAC地址字符串
	* @return	true 有错误，false 无错误
	*/
	bool mac_error(char *p_mac, int n_type);

	bool mac_error_smv(char *p_mac);

	/**
	* @brief	function name:"same_mms_data"
	* @use		判断两个MMS_DATA_STRUCT结构指针中的内容是否一致
	* @param 	[type] MMS_DATA_STRUCT [name] p_data_fst 第一个MMS_DATA_STRUCT结构指针
	* @param 	[type] MMS_DATA_STRUCT [name] p_data_sec 第二个MMS_DATA_STRUCT结构指针
	* @return	true 有错误，false 无错误
	*/
	bool same_mms_data(MMS_DATA_STRUCT *p_data_fst, MMS_DATA_STRUCT *p_data_sec);

	/**
	* @brief	function name:"zero_message_err"
	* @use		清空结构MESSAGE_ERROR_STRUCT的内容
	* @param 	[type] MESSAGE_ERROR_STRUCT [name] message_error 错误信息结构
	* @return	0 成功，1 失败
	*/
	int zero_message_err(MESSAGE_ERROR_STRUCT &message_error);

	/** 
	* @name		str_cmp_i
	* @brief	字符串比较函数,不区分大小写
	* @param	psrc[in]: 源字符串
	* @param	pdest[in]: 目的字符串
	* @return	0 相同，	非0 不相同
	*/
	int str_cmp_i(const char*psrc, const char* pdest);

	/**
	* @brief	function name:"CreateAsduErr"
	* @use		根据ASDU的错误编码链表，创建结构MESSAGE_ASDU_ERROR_STRUCT数组
	* @param 	[type] vector<int> [name] v_asduerr ASDU的错误编码链表
	* @param 	[type] MESSAGE_ASDU_ERROR_STRUCT* [name] p_asdu_err 结构数组首地址
	* @return	RES_SUCCESS 成功，RES_FAIL 失败
	*/
//	int* CreateAsduErr(vector<int> v_asduerr);

	/**
	* @brief	function name:"Msgerr2MsgerrInfo"
	* @use		将结构MESSAGE_ERROR_STRUCT的信息填充到结构MESSAGE_ERROR_INFO_STRUCT指针中
	* @param 	[type] MESSAGE_ERROR_STRUCT [name] msgerr 
	* @param 	[type] MESSAGE_ERROR_INFO_STRUCT* [name] p_msgerrInfo 结构指针
	* @return	RES_SUCCESS 成功，RES_FAIL 失败
	*/
	int Msgerr2MsgerrInfo(MESSAGE_ERROR_STRUCT msgerr, MESSAGE_ERROR_INFO_STRUCT *p_msgerrInfo);

	double getsec(char *c_avtm1, char *c_avtm2);
	double getsec_ex(char *c_avtm1, char *c_avtm2);

	MESSAGE_DATA_ERR_STRUCT* CreateDAerr(ASDU_ERR_NUM_STRUCT asdu_da_err);

	int release_msg_err_info(MESSAGE_ERROR_INFO_STRUCT* p_msg_err, int n_num);
	int release_asdu_err(MESSAGE_ASDU_ERROR_STRUCT *p_asduerr, int n_num);
	int release_da_err(MESSAGE_DATA_ERR_STRUCT* p_daerr, int n_num);

	MESSAGE_ASDU_ERROR_STRUCT * create_asdu_err(map<int, ASDU_ERR_NUM_STRUCT> map_asdu_2_errs);
	MESSAGE_DATA_ERR_STRUCT* create_smv_da_err(map<int, vector<int> > map_daseq_2_daerrs);
	MESSAGE_ASDU_ERROR_STRUCT * create_gsda_err(map<int, vector<int> >map_daseq2errs);
	int* create_errcode(vector<int> v_err_numbers);

	/***************************************性能优化*****************************************/
	MESSAGE_ERROR_INFO_STRUCT* create_msg_err_info(MESSAGE_ERROR_STRUCT* pmsg_err);
	int Msgerr2MsgerrInfo_ex(MESSAGE_ERROR_STRUCT *pmsgerr, MESSAGE_ERROR_INFO_STRUCT *p_msgerrInfo);
	MESSAGE_ERROR_INFO_ARRAY_STRUCT* CreateMsgErrArray_ex(vector<MESSAGE_ERROR_STRUCT> message_err_lst);

#ifdef __cplusplus
};
#endif

#endif //ANALYZE_COMMON_H