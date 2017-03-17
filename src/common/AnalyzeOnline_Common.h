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

//���Ĵ���ṹ
typedef struct _MESSAGE_ERROR_STRUCT
{
	MESSAGE_ERROR_CLASS n_error_class;//���Ĵ������

	int n_appid;
	
	char c_avtm[MAX_ARV_TIME_LEN];//��֡���ĵĵ���ʱ��

	vector<int> v_err_numbers;//���Ĵ��������//���ļ�����

	map<int, ASDU_ERR_NUM_STRUCT> map_asdu_2_errs;//sv-asdu����(����ͨ������)����
	
	map<int, vector<int> > map_gs_daseq_2_errs;//gs-asdu����(����ͨ������)����

	_MESSAGE_ERROR_STRUCT()
	{
		n_error_class = ERROR_CLASS_UNKOWN;
		n_appid = 0;
		memset(c_avtm,0,sizeof(c_avtm));
		v_err_numbers.clear();
		map_asdu_2_errs.clear();
		map_gs_daseq_2_errs.clear();

	}

}MESSAGE_ERROR_STRUCT;//����ASDU����

#ifdef __cplusplus
extern "C" {
#endif

	/**
	* @brief	function name:"Releas_message_errs"
	* @use		�ͷŽṹMESSAGE_ERROR_INFO_ARRAY_STRUCTָ����ڴ�
	* @param 	[type] MESSAGE_ERROR_INFO_ARRAY_STRUCT [name] p_message_errs MESSAGE_ERROR_INFO_ARRAY_STRUCTָ��
	* @return	RES_SUCCESS �ɹ���RES_FAIL ʧ��
	*/
	int Releas_message_errs(MESSAGE_ERROR_INFO_ARRAY_STRUCT* p_message_errs);

	/**
	* @brief	function name:"create_message_errs"
	* @use		����vector�е����ݣ������ṹMESSAGE_ERROR_STRUCT����
	* @param 	[type] vector<MESSAGE_ERROR_STRUCT> [name] message_err_lst MESSAGE_ERROR_STRUCT�ṹ����
	* @return	�ṹMESSAGE_ERROR_INFO_ARRAY_STRUCT������׵�ַ
	*/
	MESSAGE_ERROR_INFO_ARRAY_STRUCT* CreateMsgErrArray(vector<MESSAGE_ERROR_STRUCT> message_err_lst);
	/**
	* @brief	function name:"mac_error"
	* @use		�ж��ַ���MAC��ַ��û�д���
	* @param 	[type] char [name] p_mac MAC��ַ�ַ���
	* @return	true �д���false �޴���
	*/
	bool mac_error(char *p_mac, int n_type);

	bool mac_error_smv(char *p_mac);

	/**
	* @brief	function name:"same_mms_data"
	* @use		�ж�����MMS_DATA_STRUCT�ṹָ���е������Ƿ�һ��
	* @param 	[type] MMS_DATA_STRUCT [name] p_data_fst ��һ��MMS_DATA_STRUCT�ṹָ��
	* @param 	[type] MMS_DATA_STRUCT [name] p_data_sec �ڶ���MMS_DATA_STRUCT�ṹָ��
	* @return	true �д���false �޴���
	*/
	bool same_mms_data(MMS_DATA_STRUCT *p_data_fst, MMS_DATA_STRUCT *p_data_sec);

	/**
	* @brief	function name:"zero_message_err"
	* @use		��սṹMESSAGE_ERROR_STRUCT������
	* @param 	[type] MESSAGE_ERROR_STRUCT [name] message_error ������Ϣ�ṹ
	* @return	0 �ɹ���1 ʧ��
	*/
	int zero_message_err(MESSAGE_ERROR_STRUCT &message_error);

	/** 
	* @name		str_cmp_i
	* @brief	�ַ����ȽϺ���,�����ִ�Сд
	* @param	psrc[in]: Դ�ַ���
	* @param	pdest[in]: Ŀ���ַ���
	* @return	0 ��ͬ��	��0 ����ͬ
	*/
	int str_cmp_i(const char*psrc, const char* pdest);

	/**
	* @brief	function name:"CreateAsduErr"
	* @use		����ASDU�Ĵ���������������ṹMESSAGE_ASDU_ERROR_STRUCT����
	* @param 	[type] vector<int> [name] v_asduerr ASDU�Ĵ����������
	* @param 	[type] MESSAGE_ASDU_ERROR_STRUCT* [name] p_asdu_err �ṹ�����׵�ַ
	* @return	RES_SUCCESS �ɹ���RES_FAIL ʧ��
	*/
//	int* CreateAsduErr(vector<int> v_asduerr);

	/**
	* @brief	function name:"Msgerr2MsgerrInfo"
	* @use		���ṹMESSAGE_ERROR_STRUCT����Ϣ��䵽�ṹMESSAGE_ERROR_INFO_STRUCTָ����
	* @param 	[type] MESSAGE_ERROR_STRUCT [name] msgerr 
	* @param 	[type] MESSAGE_ERROR_INFO_STRUCT* [name] p_msgerrInfo �ṹָ��
	* @return	RES_SUCCESS �ɹ���RES_FAIL ʧ��
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

	/***************************************�����Ż�*****************************************/
	MESSAGE_ERROR_INFO_STRUCT* create_msg_err_info(MESSAGE_ERROR_STRUCT* pmsg_err);
	int Msgerr2MsgerrInfo_ex(MESSAGE_ERROR_STRUCT *pmsgerr, MESSAGE_ERROR_INFO_STRUCT *p_msgerrInfo);
	MESSAGE_ERROR_INFO_ARRAY_STRUCT* CreateMsgErrArray_ex(vector<MESSAGE_ERROR_STRUCT> message_err_lst);

#ifdef __cplusplus
};
#endif

#endif //ANALYZE_COMMON_H