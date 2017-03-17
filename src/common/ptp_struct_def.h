#ifndef PTP_STRUCT_DEF_H_INCLUDE
#define PTP_STRUCT_DEF_H_INCLUDE

#include "define_scan.h"
#include "const_scan.h"

/** PTP�������� */
enum PTP_MESSAGE_TYPE
{
	//EVENT
	PTP_SYNC_MESSAGE        = 0,
	PTP_DELAY_REQ_MESSAGE   = 1,
	PTP_PDELAY_REQ_MESSAGE  = 2,
	PTP_PDELAY_RESP_MESSAGE = 3,
	//����4-7
	PTP_PDELAY_RESERVE_4 = 4,
	PTP_PDELAY_RESERVE_5 = 5,
	PTP_PDELAY_RESERVE_6 = 6,
	PTP_PDELAY_RESERVE_7 = 7,
	//
	PTP_FOLLOWUP_GENERAL     = 8,
	PTP_DELAY_RESP_GENERAL   = 9,
	PTP_PDELAY_RESP_FOLLOW_UP_GENERAL  = 10,
	PTP_ANNOUNCE_GENERAL  = 11,
	PTP_SIGNALING_GENERAL  = 12,
	PTP_MANAGEMENT_GENERAL   = 13,
	//����14-15
	PTP_OTHER_GENERAL_14     = 14,
	PTP_OTHER_GENERAL_15      = 15,

};

#pragma pack (1) //ָ����һ���ֽڶ���

/** unsigned int48���� */
typedef struct _UINT48
{
	UINT u_LSB_4byte;
	u_short u_MSB_2byte;
	
}UINT48;

/** int64���� */
typedef struct _INTEGER64
{
	int n_LSB;
	int n_MSB;
	
	
}INTEGER64;

/** int64���� */
typedef struct _CORRECTION_FIELD
{
	//����
	UINT48 n_Ns_6byte;

	//�����С��
	short n_SubNs_2byte;
	
	
}CORRECTION_FIELD;

/** PTP�����¼������ */
typedef struct _PTP_TIME_STAMP
{
 	//6�ֽ�
 	UINT48 u_seconds;

	//4�ֽ�
	UINT u_nanoseconds;


}PTP_TIME_STAMP;

typedef struct _CLOCK_IDENTITY_STRUCT
{
	uint8 c_bit0;
	uint8 c_bit1;
	uint8 c_bit2;
	uint8 c_bit3;
	uint8 c_bit4;
	uint8 c_bit5;
	uint8 c_bit6;
	uint8 c_bit7;

}CLOCK_IDENTITY_STRUCT;

/* PortIdentity ���� */
typedef struct _PORTIDENTITY_STRUCT
{
	//ʱ�ӱ�ʶ
//	char c_clock_identity[11];
	CLOCK_IDENTITY_STRUCT clock_identity;

	//�˿ں�
	u_short u_port_id;
	
	
}PORTIDENTITY_STRUCT;

/* PTP Message header */
typedef struct _PTP_MSGHEADER{
	
	//��������
	uint8 enum_ptp_msg_type;
	
	//PTP�汾
	char n_ptp_vertion;
	
	//���ĳ���
	u_short n_msg_len;
	
	//��ڵ�� ����Ϣ�ķ����ڵ㲻ͬ��domainNumber��ֵ��ͬ
	char n_domain_number; 

	//�����ֽ�
	char c_reserved;
	
	//��־��
	short n_flag_field;//2�ֽ�
	
	//ʱ��������ns��* 65536(2��16�η�) ֵ��������Ϣ����
	CORRECTION_FIELD correction_field;

	//�����ֽ�
	int n_reserved;
	
	//����PTP���ĵ�Դ�˿ڵ�ַ��Ϣ
	PORTIDENTITY_STRUCT port_identity; 
	
	//���ķ������к�
	u_short n_sq_id;
	
	//������ֵ
	uint8 n_control_field;
	
	//���ݵ�һ�汾���õ�ֵ����־��Ϣ���
	char n_logmsg_periond;
	
	_PTP_MSGHEADER()
	{
		enum_ptp_msg_type = PTP_PDELAY_RESERVE_4;
		n_ptp_vertion = 2;
		n_msg_len       = 0;
		n_domain_number = 0;
		n_flag_field = 0;
//		nint64_correction_field = 0;
		n_sq_id          = 0;
		n_control_field = 0;
		n_logmsg_periond = 0;
	}
	
}PTP_MSGHEADER;

/* ClockQuality ����*/
typedef struct _CLOCK_QUALITY_STRUCT
{
	uint8 u_clockClass;
	char n_clockAccuracy;
	uint16 u_offsetScaledLogVariance;

	
}CLOCK_QUALITY_STRUCT;

/* announce ��Ϣ���� */
typedef struct _ANNOUNCE_STRUCT
{
	//������ֵΪ0�򾫶�Ϊ-1~+1��ʱ���
	PTP_TIME_STAMP time_stamp_origin;

	//UTC��TAIʱ���߼������ʱ���
	short n_utcoffset;

	//���ͽڵ����ݼ���Ա��parentDS.grandmasterPriority1��ֵ
	u_short u_masterpriority1;

	//grandmaster��ʱ������
	CLOCK_QUALITY_STRUCT clock_quality;

	//���ͽڵ����ݼ���Ա��parentDS.grandmasterPriority2��ֵ
	uint8 u_masterpriority2;

	//grandmaster��ʱ���豸ID
//	char c_clock_identity[8];
	CLOCK_IDENTITY_STRUCT clock_identity;

	//grandmaster��Slave�豸���ʱ��(��?)
	u_short u_steps_removed;

	//ʱ��Դ����
	uint8 u_time_source;


}ANNOUNCE_STRUCT;

/* Sync ��Ϣ���� */
typedef struct _SYNC_STRUCT
{
	//ʱ���
	PTP_TIME_STAMP time_stamp_origin;
	
	
}SYNC_STRUCT;

/* Delay_Req ��Ϣ���� */
typedef struct _DELAY_REQ_STRUCT
{
	//ʱ���
	PTP_TIME_STAMP time_stamp_origin;

}DELAY_REQ_STRUCT;

/* Delay_Resp ��Ϣ���� */
typedef struct _DELAY_RESP_STRUCT
{
	//��Ӧdelay_req���ĵĽ���ʱ���(t4)������Ϊ���뼶
	PTP_TIME_STAMP time_stamp_origin;

	//delay_req�ķ����豸�˿ڱ�ʶ
	PORTIDENTITY_STRUCT port_identity;
	
	
}DELAY_RESP_STRUCT;

/* Follow_Up ��Ϣ���� */
typedef struct _FOLLOW_UP_STRUCT
{
	//ʱ���
	PTP_TIME_STAMP time_stamp_origin;
	
	
}FOLLOW_UP_STRUCT;

/* Pdelay_Req ��Ϣ���� */
typedef struct _PDELAY_REQ_STRUCT
{
	//ʱ���
	PTP_TIME_STAMP time_stamp_origin;
	
	
}PDELAY_REQ_STRUCT;

/* Pdelay_Resp ��Ϣ���� */
typedef struct _PDELAY_RESP_STRUCT
{
	//��OneStepģʽ��Ϊ0��TowStepģʽ��Ϊ��Ӧpdelay_req�Ľ���ʱ���(t2)������Ϊ���뼶����
	PTP_TIME_STAMP time_stamp_origin;
	
	//pdelay_req�ķ����豸�˿ڱ�ʶ
	PORTIDENTITY_STRUCT port_identity;
	
	
}PDELAY_RESP_STRUCT;

enum ACTITION_TYPE
{
	ACTTION_GET					=0,
	ACTTION_SET,
	ACTTION_RESPONSE,
	ACTTION_COMMOND,
	ACTTION_ACKNOWLEDGE,
	ACTTION_RESERVED1,
	ACTTION_RESERVED2,
	ACTTION_RESERVED3,
	ACTTION_RESERVED4,
	ACTTION_RESERVED5,
	ACTTION_RESERVED6,
	ACTTION_RESERVED7,
	ACTTION_RESERVED8,
	ACTTION_RESERVED9,
	ACTTION_RESERVED10,
	ACTTION_RESERVED11

};

/* management TLV field */
typedef struct _PTP_MANAGEMENT_TLV 
{
	//tlvType
	short n_tlv_type;

	//tlvLength
	u_short u_length_field;

	//ֵΪ0X0000~0XFFFF
	u_short u_management_id;

	//
	char *p_value_field;


}PTP_MANAGEMENT_TLV;


/* manage ������Ϣ���� */
typedef struct _PTP_MANAGE_STRUCT
{
	//�˿ڱ�ʶ
	PORTIDENTITY_STRUCT port_identity;

	/*���ڷ���Ӧ�Ĺ�����Ϣ��startingBoundaryHops��������仯�ġ�
	������Ӧ�Ĺ�����Ϣ������ֵ��startingBoundaryHops��boundaryHops*/
	uint8 u_starting_boundary_hops;

	//������Ϣ�ڱ߽�ʱ�ӵ�ʣ��������ת������
	uint8 boundary_hops;

	uint8/*ACTITION_TYPE*/ n_action_filed;//4bit ֵΪ0~F

	//�����ֽ�
	char c_reserved;

	PTP_MANAGEMENT_TLV management_tlv;
	
	
}PTP_MANAGE_STRUCT;

typedef struct _PDELAY_FOLLOW_UP_STRUCT
{
	//��Ӧpdelay_resp���ĵķ���ʱ���(t3)������Ϊns������
	PTP_TIME_STAMP time_stamp_origin;
	
	//pdelay_resp�ķ����豸�˿ڱ�ʶ
	PORTIDENTITY_STRUCT port_identity;

}PDELAY_FOLLOW_UP_STRUCT;

typedef struct _SIGNALING_STRUCT
{
	//Ŀ���豸�Ķ˿ڱ�ʶ
	PORTIDENTITY_STRUCT port_identity;

}SIGNALING_STRUCT;
enum enumPtpErr
{
	PTP_PKTERR_NONE=0,
	PTP_PKTERR_LEN=1,
	PTP_PKTERR_VAL=2
};
typedef struct _PTP_INFO_STRUCT
{
	/****************��������***********************/
	char c_avtm_epocsec[MAX_ARV_TIME_LEN];
	//ԴMAC��ַ
	char c_src_mac[MAX_MAC_LEN];	
	//Ŀ��MAC��ַ
	char c_dest_mac[MAX_MAC_LEN];

	/****************PTP����ͷ**********************/
	PTP_MSGHEADER ptp_header;

	/****************PTP���ľ�������****************/
	union
	{
		ANNOUNCE_STRUCT msg_announce;					//announce ��Ϣ
		SYNC_STRUCT msg_sync;							//sync ��Ϣ
		DELAY_REQ_STRUCT msg_delay_req;					//delay_req ��Ϣ
		DELAY_RESP_STRUCT msg_delay_resp;				//delay_resp ��Ϣ
		FOLLOW_UP_STRUCT msg_follow_up;					//follow_up ��Ϣ
		PDELAY_REQ_STRUCT msg_pdelay_req;				//pdelay_req ��Ϣ
		PDELAY_RESP_STRUCT msg_pdelay_resp;				//pdelay_resp ��Ϣ
		PTP_MANAGE_STRUCT msg_ptp_manage;				//���� ��Ϣ
		PDELAY_FOLLOW_UP_STRUCT msg_pdelay_followup;	//pdelay_follow_up
		SIGNALING_STRUCT msg_signaling;
	}u;

	//���Ľ������
	unsigned int u_result;

	_PTP_INFO_STRUCT()
	{
		memset(c_avtm_epocsec,0,sizeof(c_avtm_epocsec));
		memset(c_src_mac,0,sizeof(c_src_mac));
		memset(c_dest_mac,0,sizeof(c_dest_mac));
		u_result = 0x00000000;
		
	}

}PTP_INFO_STRUCT;

#pragma pack () /*ȡ��ָ�����룬�ָ�ȱʡ����*/

#endif /*PTP_STRUCT_DEF_H_INCLUDE*/