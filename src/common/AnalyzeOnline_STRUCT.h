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

//报文错误类号
enum MESSAGE_ERROR_CLASS
{
	ERROR_CLASS_MMS=1,	//MMS报文错误

	ERROR_CLASS_GOOSE,	//GOOSE报文错误

	ERROR_CLASS_SMV,		//SMV报文错误
	ERROR_CLASS_PTP=5,	//PTP报文错误

	ERROR_CLASS_UNKOWN=999
};

//GOOSE报文错误号
enum GOOSE_ERROR_NUM
{
	GOOSE_SQNUM_JUMP = 1,				//stNum不变时，sqNum跳变
	GOOSE_SQNUM_REVERSE,				//stNum不变时，sqNum顺序逆转
	GOOSE_STNUM_JUMP,					//stNum跳变
	GOOSE_STNUM_REVERSE,				//stNum逆转
	GOOSE_SQNUM_UNRESET,				//Goose新事件sqNum错误，即stNum顺序变化时，sqNum不等于0
	DOUBLE_GOOSE,						//双GOOSE
	REPEATED_GOOSE,						//重复的GOOSE报文
	GOOSE_STNUM_CHANGE_FAKE,			//stNum虚假变位
	GOOSE_DELAY,						//延迟
	GOOSE_LINK_DISCONNECT,				//链路中断
	GOOSE_TAL_ZERO,						//TAL异常
	GOOSE_APPID_ZERO,					//APPID为零
	GOOSE_NDSCOM_TRUE,					//ndscom为true
	GOOSE_T_ERROR,						//T0~T3错误
	GOOSE_DANUM_DIFFER_DSMEMBERNUM,		//报文中的实际数据个数与配置文件中datset的成员数不一致

	//以下是配置错误，考虑性能，暂时不实现
	GOOSE_GOCBREF_DIFFER_CFG,			//gocbRef与配置文件中不一致
	GOOSE_GOID_DIFFER_CFG,				//goID与配置文件中不一致
	GOOSE_DS_DIFFER_CFG,				//datset与配置文件中不一致
	GOOSE_CONFREV_DIFFER_CFG,			//confrev与配置文件中不一致
	NUMDATSETENTRIES_DIFFER_DANUM,		//numDatSetEntries与实际数据个数不一致

	GOOSE_DESTMAC_ERROR,				//目的MAC地址错误

	GOOSE_RESTART,						//链路重启
	GOOSE_LINK_CONNECT,					//链路恢复
	GOOSE_STNUM_CHANGE_RIGHT,			//状态变位

	GOOSE_TEST_FALSE,					//GOOSE处于运行状态
	GOOSE_TEST_TRUE,					//GOOSE处于测试状态

	GOOSE_FORMAT_ERR,					//GOOSE格式错误

	GOOSE_MAC_DIFFER_CFG,				//GOOSE目的MAC地址与配置不一致
	GOOSE_QUALITY_CHANGE,				//GOOSE数据品质变化
	GOOSE_DANUM_CHANGE,					//GOOSE数据个数变化
	GOOSE_DA_TYPE_DIFFER_CFG,			//GOOSE通道数据类型与配置不一致
	GOOSE_DROP_PKT,						//GOOSE丢帧
	GOOSE_TREMBLE						//GOOSE抖动

};

//SMV报文错误号
enum SMV_ERROR_NUM
{
	SMV_ASYNCHRONISM = 1,			//丢失同步信号
	SMV_DANUM_CHANGER,				//采样值个数变化
	DOUBLE_SMV,						//双SMV
	SMV_SMPCNT_JUMP,				//序号跳变
	SMV_SMPCNT_REVERSE,				//序号逆转
	SMV_DELAY,						//延迟
	SMV_LINK_DISCONNECT,			//链路中断
	SMV_DANUM_DIFFER_DSMEMBERNUM,	//报文中的数据个数与数据集成员个数不一致
	SMV_QUALITY_ERROR,				//数据品质异常
	SMV_APPID_ZERO,					//APPID为零
	SMV_PDU_LEN_ZERO,				//PDU长度问零

	//以下是配置错误
	SMV_SVID_DIFFER_CFG,			//svID与配置文件不一致
	SMV_DS_DIFFER_CFG,				//datSet与配置文件不一致
	SMV_CONFREV_DIFFER_CFG,			//confRev与配置文件不一致

	SMV_DESTMAC_ERROR,				//目的MAC地址错误
	SMV_LINK_CONNECT,				//链路恢复连接
	SMV_QUALITY_CHANGE,				//数据品质变化
	SMV_DOUBLE_AD_DATA_DIFFER,		//双AD数据不一致
	SMV_RATE_UNSTABLE,				//频率不稳定
	SMV_FORMAT_ERR,					//报文格式错误
	SMV_SYNCHRO_SIGNAL_CHANGE,		//同步信号变化

	SMV_MAC_DIFFER_CFG,				//目的MAC地址与配置不一致
	SMV_DA_ERR,						//数值异常变大
	SMV_MU_ASYNCHRONISM,			//MU之间失步
	SMV_TREMBLE,					//数据帧抖动
	SMV_DROP_PKT,					//丢帧
	SMV_CHANNEL_DELAY_CHANGE,		//通道延时变化
	SMV_ASDU_DIFFER_CFG,			//ASDU个数与配置不一致
	SMV_TEST,						//置检修标志
	SMV_INVALID_FIELD_VAL,         //
	SMV_RESYNCHRON,			    //恢复同步信号

};

//需要保留的数据类型
enum RESERVED_DATA_TYPE
{
	RESERVED_ALL_DATA=1,		//保留所有的数据，即解析出GOOSE的MMS_DATA_STRUCT，SMV的SMV_DATA_STRUCT
	RESERVED_GOOSE_DATA,		//只保留GOOSE的MMS_DATA_STRUCT
	RESERVED_SMV_DATA,			//只保留SMV的SMV_DATA_STRUCT
	RESERVED_NO_DATA			//不保留任何数据，即GOOSE的MMS_DATA_STRUCT和SMV的SMV_DATA_STRUCT都不保留
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

	int n_num_daerr;//数据错误个数

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
	MESSAGE_ERROR_CLASS n_error_class;//报文错误类号

	int n_appid;//报文的APPID

	char c_avtm[MAX_ARV_TIME_LEN];

	int n_num_errcode;//报文错误号个数

	int *p_errcode;//报文错误号数组首地址

	int n_num_asduerr;//出错的ASDU个数

	MESSAGE_ASDU_ERROR_STRUCT *p_asduerr;//ASDU错误结构数组

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
	MESSAGE_ERROR_INFO_STRUCT *p_msg_err;//数组首先地址

	int n_msg_err;//数组大小

	_MESSAGE_ERROR_INFO_ARRAY_STRUCT()
	{
		p_msg_err = NULL;
		n_msg_err = 0;
	}

}MESSAGE_ERROR_INFO_ARRAY_STRUCT;

//报文分析动态库参数的结构
typedef struct _LIB_MSGANALYZE_PARAM_STRUCT
{
	//对象空间
	ACSI_IED_CTRL *p_ied_ctrls;

	//判断链路为中断的时间间隔
	int n_time_link_disconnect;

	//判断报文延迟的时间间隔
	int n_time_delay;

	//日志路径
	char cLogPath[256];

	//日志级别
	int nLogLevel;

	//日志文件的最大长度：单位K
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
	//goose报文个数
	int n_num_goose;

	//goose报文数组首地址
	GOOSE_INFO_STRUCT *p_goose_infos;

	//goose报文在原始报文中的位置
	int *p_goose_pos_original;

	//SMV报文个数
	int n_num_smv;

	//SMV报文数组首地址
	SMV_INFO_STRUCT *p_smv_infos;

	//smv报文在原始报文中的位置
	int *p_smv_pos_original;

	//MMS报文数组首地址
	int n_num_mms;

	//MMS报文数组首地址
	MMS_INFO_STRUCT *p_mms_infos;

	//mms报文在原始报文中的位置
	int *p_mms_pos_original;
	//PTP报文个数
	int n_num_ptp;
	//PTP报文报文数组首地址
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


//原始报文转换动态库参数的结构
typedef struct _LIB_PACKETCONVERT_PARAM_STRUCT
{
	//日志路径
	char cLogPath[256];

	//日志级别
	int nLogLevel;

	//日志文件的最大长度：单位K
	int nLogMaxLen;

	//需要解析的报文类型
	int nMsgType;

	//需要保留报文的数据类型
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
	//goose解析报文
	GOOSE_INFO_STRUCT goose_info;

	//goose原始报文
	char cPacket[MAX_PACKET_LEN];

	_GOOSE_PACKET()
	{
		memset(cPacket,0,sizeof(cPacket));
	}

}GOOSE_PACKET;

typedef struct _SMV_PACKET
{
	//smv解析报文
	SMV_INFO_STRUCT smv_info;

	//smv原始报文
	char cPacket[MAX_PACKET_LEN];

	_SMV_PACKET()
	{
		memset(cPacket,0,sizeof(cPacket));
	}

}SMV_PACKET;


#endif //ANALYZE_ONLINE_STRUCT_H