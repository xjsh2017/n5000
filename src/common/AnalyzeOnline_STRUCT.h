#ifndef ANALYZE_ONLINE_STRUCT_H
#define ANALYZE_ONLINE_STRUCT_H

#include "acsistruct.h"
#include "mmsstruct.h"
#include "AT_STRUCT.h"
#include "share_scan.h"
#include "ptp_struct_def.h"

#define ERROR_NUMBER_UNKOWN 9999

#define TYPE_GOOSE_MAC 1
#define TYPE_SMV_MAC 2

//���Ĵ������
enum MESSAGE_ERROR_CLASS
{
	ERROR_CLASS_MMS=1,	//MMS���Ĵ���

	ERROR_CLASS_GOOSE,	//GOOSE���Ĵ���

	ERROR_CLASS_SMV,		//SMV���Ĵ���
	ERROR_CLASS_PTP=5,	//PTP���Ĵ���

	ERROR_CLASS_UNKOWN=999
};

//GOOSE���Ĵ����
enum GOOSE_ERROR_NUM
{
	GOOSE_SQNUM_JUMP = 1,				//stNum����ʱ��sqNum����
	GOOSE_SQNUM_REVERSE,				//stNum����ʱ��sqNum˳����ת
	GOOSE_STNUM_JUMP,					//stNum����
	GOOSE_STNUM_REVERSE,				//stNum��ת
	GOOSE_SQNUM_UNRESET,				//Goose���¼�sqNum���󣬼�stNum˳��仯ʱ��sqNum������0
	DOUBLE_GOOSE,						//˫GOOSE
	REPEATED_GOOSE,						//�ظ���GOOSE����
	GOOSE_STNUM_CHANGE_FAKE,			//stNum��ٱ�λ
	GOOSE_DELAY,						//�ӳ�
	GOOSE_LINK_DISCONNECT,				//��·�ж�
	GOOSE_TAL_ZERO,						//TAL�쳣
	GOOSE_APPID_ZERO,					//APPIDΪ��
	GOOSE_NDSCOM_TRUE,					//ndscomΪtrue
	GOOSE_T_ERROR,						//T0~T3����
	GOOSE_DANUM_DIFFER_DSMEMBERNUM,		//�����е�ʵ�����ݸ����������ļ���datset�ĳ�Ա����һ��

	//���������ô��󣬿������ܣ���ʱ��ʵ��
	GOOSE_GOCBREF_DIFFER_CFG,			//gocbRef�������ļ��в�һ��
	GOOSE_GOID_DIFFER_CFG,				//goID�������ļ��в�һ��
	GOOSE_DS_DIFFER_CFG,				//datset�������ļ��в�һ��
	GOOSE_CONFREV_DIFFER_CFG,			//confrev�������ļ��в�һ��
	NUMDATSETENTRIES_DIFFER_DANUM,		//numDatSetEntries��ʵ�����ݸ�����һ��

	GOOSE_DESTMAC_ERROR,				//Ŀ��MAC��ַ����

	GOOSE_RESTART,						//��·����
	GOOSE_LINK_CONNECT,					//��·�ָ�
	GOOSE_STNUM_CHANGE_RIGHT,			//״̬��λ

	GOOSE_TEST_FALSE,					//GOOSE��������״̬
	GOOSE_TEST_TRUE,					//GOOSE���ڲ���״̬

	GOOSE_FORMAT_ERR,					//GOOSE��ʽ����

	GOOSE_MAC_DIFFER_CFG,				//GOOSEĿ��MAC��ַ�����ò�һ��
	GOOSE_QUALITY_CHANGE,				//GOOSE����Ʒ�ʱ仯
	GOOSE_DANUM_CHANGE,					//GOOSE���ݸ����仯
	GOOSE_DA_TYPE_DIFFER_CFG,			//GOOSEͨ���������������ò�һ��
	GOOSE_DROP_PKT,						//GOOSE��֡
	GOOSE_TREMBLE						//GOOSE����

};

//SMV���Ĵ����
enum SMV_ERROR_NUM
{
	SMV_ASYNCHRONISM = 1,			//��ʧͬ���ź�
	SMV_DANUM_CHANGER,				//����ֵ�����仯
	DOUBLE_SMV,						//˫SMV
	SMV_SMPCNT_JUMP,				//�������
	SMV_SMPCNT_REVERSE,				//�����ת
	SMV_DELAY,						//�ӳ�
	SMV_LINK_DISCONNECT,			//��·�ж�
	SMV_DANUM_DIFFER_DSMEMBERNUM,	//�����е����ݸ��������ݼ���Ա������һ��
	SMV_QUALITY_ERROR,				//����Ʒ���쳣
	SMV_APPID_ZERO,					//APPIDΪ��
	SMV_PDU_LEN_ZERO,				//PDU��������

	//���������ô���
	SMV_SVID_DIFFER_CFG,			//svID�������ļ���һ��
	SMV_DS_DIFFER_CFG,				//datSet�������ļ���һ��
	SMV_CONFREV_DIFFER_CFG,			//confRev�������ļ���һ��

	SMV_DESTMAC_ERROR,				//Ŀ��MAC��ַ����
	SMV_LINK_CONNECT,				//��·�ָ�����
	SMV_QUALITY_CHANGE,				//����Ʒ�ʱ仯
	SMV_DOUBLE_AD_DATA_DIFFER,		//˫AD���ݲ�һ��
	SMV_RATE_UNSTABLE,				//Ƶ�ʲ��ȶ�
	SMV_FORMAT_ERR,					//���ĸ�ʽ����
	SMV_SYNCHRO_SIGNAL_CHANGE,		//ͬ���źű仯

	SMV_MAC_DIFFER_CFG,				//Ŀ��MAC��ַ�����ò�һ��
	SMV_DA_ERR,						//��ֵ�쳣���
	SMV_MU_ASYNCHRONISM,			//MU֮��ʧ��
	SMV_TREMBLE,					//����֡����
	SMV_DROP_PKT,					//��֡
	SMV_CHANNEL_DELAY_CHANGE,		//ͨ����ʱ�仯
	SMV_ASDU_DIFFER_CFG,			//ASDU���������ò�һ��
	SMV_TEST,						//�ü��ޱ�־
	SMV_INVALID_FIELD_VAL,         //
	SMV_RESYNCHRON,			    //�ָ�ͬ���ź�

};

//��Ҫ��������������
enum RESERVED_DATA_TYPE
{
	RESERVED_ALL_DATA=1,		//�������е����ݣ���������GOOSE��MMS_DATA_STRUCT��SMV��SMV_DATA_STRUCT
	RESERVED_GOOSE_DATA,		//ֻ����GOOSE��MMS_DATA_STRUCT
	RESERVED_SMV_DATA,			//ֻ����SMV��SMV_DATA_STRUCT
	RESERVED_NO_DATA			//�������κ����ݣ���GOOSE��MMS_DATA_STRUCT��SMV��SMV_DATA_STRUCT��������
};

typedef struct _MESSAGE_DATA_ERR_STRUCT
{
	int n_da_seq;
	int n_num_errcode;
	int *p_errcode;

	_MESSAGE_DATA_ERR_STRUCT()
	{
		n_da_seq = 0;
		n_num_errcode = 0;
		p_errcode = NULL;
	}

}MESSAGE_DATA_ERR_STRUCT;

typedef struct _MESSAGE_ASDU_ERROR_STRUCT
{
	int n_asdu_no;

	int n_num_errcode;

	int *p_errcode;

	int n_num_daerr;//���ݴ������

	MESSAGE_DATA_ERR_STRUCT* p_daerr;

	_MESSAGE_ASDU_ERROR_STRUCT()
	{
		n_asdu_no = -1;
		n_num_errcode = 0;
		p_errcode = NULL;
		n_num_daerr = 0;
		p_daerr = NULL;
	}
	
}MESSAGE_ASDU_ERROR_STRUCT;

typedef struct _MESSAGE_ERROR_INFO_STRUCT
{
	MESSAGE_ERROR_CLASS n_error_class;//���Ĵ������

	int n_appid;//���ĵ�APPID

	char c_avtm[MAX_ARV_TIME_LEN];

	int n_num_errcode;//���Ĵ���Ÿ���

	int *p_errcode;//���Ĵ���������׵�ַ

	int n_num_asduerr;//�����ASDU����

	MESSAGE_ASDU_ERROR_STRUCT *p_asduerr;//ASDU����ṹ����

/*	char cPacket_pre2[MAX_PACKET_LEN];
	char cPacket_pre[MAX_PACKET_LEN];
	char cPacket[MAX_PACKET_LEN];*/

	_MESSAGE_ERROR_INFO_STRUCT()
	{
		n_error_class = ERROR_CLASS_UNKOWN;

		memset(c_avtm,0,sizeof(c_avtm));
		n_num_errcode = 0;
		p_errcode = NULL;
		n_num_asduerr = 0;
		p_asduerr = NULL;

/*		memset(cPacket,0,sizeof(cPacket));
		memset(cPacket_pre,0,sizeof(cPacket_pre));
		memset(cPacket_pre2,0,sizeof(cPacket_pre2));*/
	}

}MESSAGE_ERROR_INFO_STRUCT;

typedef struct _MESSAGE_ERROR_INFO_ARRAY_STRUCT
{
	MESSAGE_ERROR_INFO_STRUCT *p_msg_err;//�������ȵ�ַ

	int n_msg_err;//�����С

	_MESSAGE_ERROR_INFO_ARRAY_STRUCT()
	{
		p_msg_err = NULL;
		n_msg_err = 0;
	}

}MESSAGE_ERROR_INFO_ARRAY_STRUCT;

//���ķ�����̬������Ľṹ
typedef struct _LIB_MSGANALYZE_PARAM_STRUCT
{
	//����ռ�
	ACSI_IED_CTRL *p_ied_ctrls;

	//�ж���·Ϊ�жϵ�ʱ����
	int n_time_link_disconnect;

	//�жϱ����ӳٵ�ʱ����
	int n_time_delay;

	//��־·��
	char cLogPath[256];

	//��־����
	int nLogLevel;

	//��־�ļ�����󳤶ȣ���λK
	int nLogMaxLen;

	_LIB_MSGANALYZE_PARAM_STRUCT()
	{
		p_ied_ctrls = NULL;
		n_time_link_disconnect = 0;
		n_time_delay = 0;

		memset(&cLogPath, 0, sizeof(cLogPath));
		nLogLevel = 0;
		nLogMaxLen = 0;
	}

}LIB_MSGANALYZE_PARAM_STRUCT;

typedef struct _MESSAGE_STRUCT
{
	//goose���ĸ���
	int n_num_goose;

	//goose���������׵�ַ
	GOOSE_INFO_STRUCT *p_goose_infos;

	//goose������ԭʼ�����е�λ��
	int *p_goose_pos_original;

	//SMV���ĸ���
	int n_num_smv;

	//SMV���������׵�ַ
	SMV_INFO_STRUCT *p_smv_infos;

	//smv������ԭʼ�����е�λ��
	int *p_smv_pos_original;

	//MMS���������׵�ַ
	int n_num_mms;

	//MMS���������׵�ַ
	MMS_INFO_STRUCT *p_mms_infos;

	//mms������ԭʼ�����е�λ��
	int *p_mms_pos_original;
	//PTP���ĸ���
	int n_num_ptp;
	//PTP���ı��������׵�ַ
	PTP_INFO_STRUCT* p_ptp_infos;

	_MESSAGE_STRUCT()
	{
		n_num_goose=0;
		p_goose_infos = NULL;
		p_goose_pos_original = NULL;
		n_num_smv = 0;
		p_smv_infos = NULL;
		p_smv_pos_original = NULL;
		n_num_mms = 0;
		p_mms_pos_original = NULL;
	}

}MESSAGE_STRUCT;


//ԭʼ����ת����̬������Ľṹ
typedef struct _LIB_PACKETCONVERT_PARAM_STRUCT
{
	//��־·��
	char cLogPath[256];

	//��־����
	int nLogLevel;

	//��־�ļ�����󳤶ȣ���λK
	int nLogMaxLen;

	//��Ҫ�����ı�������
	int nMsgType;

	//��Ҫ�������ĵ���������
	int nReserveDaType;

	_LIB_PACKETCONVERT_PARAM_STRUCT()
	{
		memset(&cLogPath, 0, sizeof(cLogPath));
		nLogLevel = 0;
		nLogMaxLen = 0;
		nMsgType = 0;
		nReserveDaType = 0;
	}

}LIB_PACKETCONVERT_PARAM_STRUCT;

typedef struct _GOOSE_PACKET
{
	//goose��������
	GOOSE_INFO_STRUCT goose_info;

	//gooseԭʼ����
	char cPacket[MAX_PACKET_LEN];

	_GOOSE_PACKET()
	{
		memset(cPacket,0,sizeof(cPacket));
	}

}GOOSE_PACKET;

typedef struct _SMV_PACKET
{
	//smv��������
	SMV_INFO_STRUCT smv_info;

	//smvԭʼ����
	char cPacket[MAX_PACKET_LEN];

	_SMV_PACKET()
	{
		memset(cPacket,0,sizeof(cPacket));
	}

}SMV_PACKET;


#endif //ANALYZE_ONLINE_STRUCT_H