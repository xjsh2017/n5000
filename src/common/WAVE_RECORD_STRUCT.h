#ifndef WAVE_RECORD_STRUCT_INCLUDED_00000000000
#define WAVE_RECORD_STRUCT_INCLUDED_00000000000
/********************************************************************
*	filename: 	SYS_MODEL_STRUCT.h
*	author:		yinzhehong
*	created:	2012/03/16
*	purpose:  	¼��ͨ�����   
*   version:    1.0
*********************************************************************/

#include "define_scan.h"
#include"SYS_MODEL_STRUCT.h"
//SCD�еĶ���
typedef struct SCDSRC
{                     
	char napp_id;   //Ӧ�ñ��
	char cref[MAX_DESCRIBE_LEN];         //����
	char cdsname[MAX_DESCRIBE_LEN];      //���ݼ�����
	int  ndsseq;                         //�����ݼ��е���ţ���0��ʼ��
	char ciedname[MAX_DESCRIBE_LEN];     //ied����
	SCDSRC()
	{
		memset(cref,0,sizeof(cref));
		memset(cdsname,0,sizeof(cdsname));
		ndsseq = 0;
		napp_id =0;
		memset(ciedname,0,sizeof(ciedname));
	}
}_SCDSRC;

// ͨͨ���ṹ����typedef struct CFG_REC_CHANNEL
// {
// 	int  nsys_channelseq;                 //ͨ����ţ��ɱ�ϵͳ���䣬ȫϵͳΨһ��ţ����е�ͨ����˳���ţ���1��ʼ
// 	char cname[MAX_DESCRIBE_LEN];         //ͨ������
// 	char cdesc[MAX_DESCRIBE_LEN];         //˵��
// 	int  nsnifferport_sys_id;             //���ݲɼ��ڶ�Ӧ��sys_id
// 	bool bsynthetic;                     //��ͨ���Ƿ�Ϊ�߼��ϳ�ͨ��,��ʱ����
// 	int  nchaneltype;                     //ͨ�����  0��ģ����ͨ�� 1��������ͨ��
// 	float fa;                            //ͨ������ϵ��
// 	float fb;                            //ƫ����
// 	char  cunits[12];                     //��λ
// 	float fratio;                        //���ct����pt
// 	char ccbm[64];                       //�����ӵ��豸
// 	char cphase[4];                      //��� A��B��C��N
// 	float fmin;                          //��Сֵ
// 	float fmax;                          //���ֵ
// 	float fskew;                         //������ʱ
// 	int  nstatus;                        //״ֵ̬ ͨ������״̬��������1 ���գ�0  ������ͨ��ʱ����
// 	SCDSRC  struct_scdsrc;               //scd����������
// 	int  n61850_data_type;               //61850��������
// 	//��ʼ��
// 	CFG_REC_CHANNEL()
// 	{
// 		nsys_channelseq = 1;
// 		nsnifferport_sys_id = 0;
// 		bsynthetic      = false;
// 		fa = 1.0;
// 		fb = 0.0;
// 		fratio =1.0;
// 		memset(cname,0,sizeof(cname));
// 		memset(cunits,0,sizeof(cunits));
// 		memset(ccbm,0,sizeof(ccbm));
// 		memset(cphase,0,sizeof(cphase));
// 		fmin = 0.0;
// 		fmax = 0.0;
// 		fskew = 0.0;//��λΪ΢��
// 		nchaneltype = 0;
// 		nstatus = 0;
// 		n61850_data_type = 999;
// 	}
// }_CFG_REC_CHANNEL;//ͨ��
// 
// Ӧ�ýڵ�ͨ������
// typedef struct _APP_CHANNEL_RELATION
// {
// 	int n_sniffer_app_sys_id;
// 	int n_yc_channel_num;
// 	int n_yx_channel_num;
// 	CFG_REC_CHANNEL** pyc_channel_list;//ң��ͨ������
// 	CFG_REC_CHANNEL** pyx_channel_list;//ң��ͨ������
// 
// 	int n_sys_yc_channel_num;
// 	int n_sys_yx_channel_num;
// 
// 	CFG_REC_CHANNEL** pyc_channel_list;//ң��ͨ������
// 	CFG_REC_CHANNEL** pyx_channel_list;//ң��ͨ������
// 
// 	_APP_CHANNEL_RELATION()
// 	{
// 		n_sniffer_app_sys_id = 0;
// 		n_yc_channel_num     = 0;
// 		n_yx_channel_num     = 0;
// 		pyc_channel_list     = 0;
// 		pyx_channel_list     = 0;
// 	}
// }APP_CHANNEL_RELATION;

#endif //WAVE_RECORD_STRUCT_INCLUDED_00000000000
