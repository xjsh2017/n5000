#ifndef SHARE_SCAN_H
#define SHARE_SCAN_H

#include "define_scan.h"
#include "const_scan.h"

typedef struct _PORT_ATTRIBUTE_STRUCT
{
	int n_port;//�˿ں�

	int n_max_flow;//�����������ֵ

	int n_min_fow;//����������Сֵ

	_PORT_ATTRIBUTE_STRUCT()
	{
		n_port = 0;
		n_max_flow = 0;
		n_min_fow = 0;
	}

}PORT_ATTRIBUTE_STRUCT;

typedef struct _ANALYZEONLINE_RUN_PARAM_STRUCT
{
	//�˿����Խṹ�����׵�ַ
	PORT_ATTRIBUTE_STRUCT *p_port_attribute;

	//�˿ڸ���
	int n_port_nums;

	//��־·��
	char cLogPath[256];

	//��־����
	int nLogLevel;

	//��־�ļ�����󳤶ȣ���λK
	int nLogMaxLen;

	//����ֵ�����ӳٵ�ʱ����
	double dTimeSmvdelay;

	//����ֵ������ʱ����
	double dTimeSmvLinkDisconnected;

	//����ͳ�Ƶ�ʱ����
	int nNetstatTimeInterval;

	//���߷������ͣ�1��ʾֻ����GOOSE��2��ʾֻ����SMV��3��ʾֻ����MMS��4��ʾ����GOOSE��SMV��5��ʾ����GOOSE��MMS��6��ʾ����SMV��MMS��7��ʾ����GOOSE��SMV��MMS
	int n_ao_type;

	//�������ĵ���������
	int n_reserve_da_type;

	_ANALYZEONLINE_RUN_PARAM_STRUCT()
	{
		n_port_nums = 0;
		p_port_attribute = NULL;
		memset(&cLogPath,0,256);
		nLogLevel = 0;
		nLogMaxLen = 0;
		dTimeSmvdelay = 60;
		dTimeSmvLinkDisconnected = 300;
		nNetstatTimeInterval = 3;
		n_ao_type = 0;
		n_reserve_da_type = 0;
	}
	

}ANALYZEONLINE_RUN_PARAM_STRUCT;

//ԭʼ������char�����ʾ�Ľṹ
typedef struct _PACKET_CHAR_STRUCT
{
	char c_pacekt[MAX_PACKET_LEN+1];

	//��֡ʵ�ʱ��ĳ���
	int nLen;

	_PACKET_CHAR_STRUCT()
	{
		memset(&c_pacekt,0,sizeof(c_pacekt));
		nLen = 0;
	}

}PACKET_CHAR_STRUCT;


typedef struct _LOG_STRUCT
{
	//��־·��
	char cLogPath[256];

	//��־����
	int nLogLevel;

	//��־�ļ�����󳤶ȣ���λK
	int nLogMaxLen;

	_LOG_STRUCT()
	{
		memset(&cLogPath,0,256);
		nLogLevel = 0;
		nLogMaxLen = 0;
	}

}LOG_STRUCT;

typedef struct _TCP_STRUCT
{
	//Դ�˿�
	int n_srcport;

	//Ŀ�Ķ˿�
	int n_dstport;

	//���
	unsigned int n_seq;

	//ȷ�Ϻ�
	unsigned int n_ack;

	unsigned char c_flag;

	_TCP_STRUCT()
	{
		n_srcport=0;
		n_dstport=0;
		n_seq=0;
		n_ack=0;
		c_flag=0;
	}
}TCP_STRUCT;

//IP���Ľṹ
typedef struct _IP_STRUCT
{
	//IP��ͷ����
	int n_headerlen;

	//ԴIP
	char c_srcip[20];

	//Ŀ��IP
	char c_dstip[20];

	TCP_STRUCT tcp_info;

	_IP_STRUCT()
	{
		n_headerlen=0;
		memset(c_srcip, 0, sizeof(c_srcip));
		memset(c_dstip, 0, sizeof(c_dstip));
	}

}IP_STRUCT;

#endif //SHARE_SCAN_H