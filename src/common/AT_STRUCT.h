#ifndef AT_STRUCT_H_HEADER_INCLUDED_88888888
#define AT_STRUCT_H_HEADER_INCLUDED_88888888

#include <memory>
#include "mmsstruct.h"
//#include "..\mms2acsi\mmsstruct.h"  //modify by yinzhehong 20101118
#include "const_scan.h"

#define MAX_DATA_LEN_OBJ_REF 129+1 

#define MAX_DATA_LEN_VISIBLE_STRING  65+1 
#define MAX_DATA_LEN_ENTRY_TIME		 48+1

#define SHORT_DATA_LEN				 64+1

#define	MAX_DATA_LEN 256+1
#define MAX_REF_LEN	 128+1

//#define MAX_MAC_LEN 17+1
#define MAX_ARRIVAL_TIME_LEN 255+1

#define  DES_LEN     64
#define  CONTENT_LEN 64
//AnalyzeTxt���ݽṹ
typedef struct AT_ITEM_STRUCT{
	char c_des[DES_LEN];//MAX_DES_LEN
	char c_content[CONTENT_LEN];	//ʵ������
	
	//��ʼ�����
	AT_ITEM_STRUCT()
	{
		memset(c_des,0,sizeof(c_des));
		memset(c_content,0,sizeof(c_content));
	}
}_AT_ITEM_STRUCT;

//AnalyzeTxt����ṹ
typedef struct AT_STRUCT{
	int n_item_num;	//�ڵ����
	AT_ITEM_STRUCT *p_item;	//�ڵ�ṹ�׵�ַ

	int n_child_num;		//�ӽڵ����
	AT_STRUCT *p_child;	//�ӽڵ�ṹ�׵�ַ
	AT_STRUCT *p_parent;	//���ڵ�ָ��

	//��ʼ�����
	AT_STRUCT()
	{
		n_item_num	 =	0;
		p_item			  =	NULL;
		n_child_num		    =	0;
		p_child					=	NULL;
		p_parent				=	NULL;
	}
}_AT_STRUCT;

//������������ṹ,��Ե�һ֡����
typedef struct AT_STRUCT_STEP{
	
	//�������� 0: ���������� 1: mms 2: goose 3: smv
	int n_type;

	//��֡���Ľ������
	AT_STRUCT *p_at_struct;

	AT_STRUCT_STEP(){
		n_type=0;
		p_at_struct=NULL;
	};
	
}_AT_STRUCT_STEP;

typedef struct SMV_DATA_STRUCT{

	//ֵ
	int n_value;
	//Ʒ������
	char c_quality[9];

	SMV_DATA_STRUCT(){
		n_value=0;
                memset(c_quality,0,sizeof(c_quality));
	}

}_SMV_DATA_STRUCT;


//asdu��Ϣ�ṹ
typedef struct ASDU_INFO_STRUCT{

	int n_asdu_len;
	//MsvID��UsvID
	char c_svID[MAX_DATA_LEN_VISIBLE_STRING];

	char c_dataset[MAX_DATA_LEN_OBJ_REF];

	unsigned int n_smpCnt;
	int	 n_confRev;
	char  c_refrTm[MAX_DATA_LEN_ENTRY_TIME];
	bool  b_smpSynch;
	int	  n_smpRate;

	//ͨ�����������߷���ʱ�������е�ͨ��������
	int  n_data_num;

	SMV_DATA_STRUCT *p_smv_data_struct;

	//������ʵ��ͨ������
	unsigned short n_danum_actual;
	int  n_smpSynch;//ͬ����ȡֵ��Χ�����ܳ���true|false

	ASDU_INFO_STRUCT(){
		n_asdu_len=0;
		memset(c_svID,0,sizeof(c_svID));
		memset(c_dataset,0,sizeof(c_dataset));
		n_smpCnt=0;
		n_confRev=0;
		memset(c_refrTm,0,sizeof(c_refrTm));
		b_smpSynch=false;
		n_smpRate=0;
		n_data_num=0;
		p_smv_data_struct=NULL;
		n_danum_actual = 0;
		n_smpSynch=0;
	}


}_ASDU_INFO_STRUCT;


typedef struct SMV_INFO_STRUCT{

	//ԴMAC��ַ
	char c_src_mac[MAX_MAC_LEN];

	//Ŀ��MAC��ַ
	char c_dest_mac[MAX_MAC_LEN];

	//���ĵ���ʱ�䣬�Լ�Ԫ���ʾ//��ֱ�ӽ����У���pcap��Ԫͷ��8�ֽ�sec.usec����wireshark�����У��Ǳ������Ľ���ַ���
	char c_avtm_epocsec[MAX_ARV_TIME_LEN];

	//APPID��
	unsigned int n_app_id;
	//���ĳ���
	int n_msg_len;

	//asdu����(���߷���ʱ�������е�asdu����)
	int n_asdu_num;
	
	//ASDU��Ϣ�ṹ�����׵�ַ
	ASDU_INFO_STRUCT *p_asdu_info_struct;

	//������ʵ��ASDU����
	unsigned short n_nofasdu_actual;

	//Bit31:packet_len<76�ֽ� Bit30:��ʾmsg_len�쳣 Bit29:��ʾASDU����Ϊ0
	//Bit15:nofasdu Bit14:sv_id Bit13:smpCunt Bit12:confRev Bit11:SmpSynch Bit10:DataSet Bit9:RefrTm Bit8:SmpRate  Bit7:sequence of data)������BitλԤ��
	unsigned int u_result;//������� add on 2013/03/28 ljm Ĭ��0x00000000�����ֽڵ�4���ֽڱ�ʾ�����쳣�������3���ֽ�Ԥ������1��2���ֽڱ�ʾ���������ʶ

	//��ʼ�����
	SMV_INFO_STRUCT()
	{
		memset(c_src_mac,0,sizeof(c_src_mac));
		memset(c_dest_mac,0,sizeof(c_dest_mac));
		memset(c_avtm_epocsec,0,sizeof(c_avtm_epocsec));//modified on 20110827
		n_app_id=0;
		n_msg_len=0;
		n_asdu_num=0;
		p_asdu_info_struct=NULL;
		n_nofasdu_actual = 0;
		u_result = 0x00000000;

	}

}_SMV_INFO_STRUCT;


typedef struct GOOSE_INFO_STRUCT{

	//ԴMAC��ַ
	char c_src_mac[MAX_MAC_LEN];//add by ljm

	//Ŀ��MAC��ַ
	char c_dest_mac[MAX_MAC_LEN];//add by ljm

	//���ĵ���ʱ�䣬�Լ�Ԫ���ʾ
	char c_avtm_epocsec[MAX_ARV_TIME_LEN];

	//APPID��
	unsigned int  n_app_id;
	//���ĳ���
	int n_msg_len;
	//�����ֶ�1
	char c_reserved1[SHORT_DATA_LEN];
	//�����ֶ�2
	char c_reserved2[SHORT_DATA_LEN];

	//goose������ƿ�  ����
	char c_gocbRef[MAX_DATA_LEN_OBJ_REF];

	//��Ϣ���������Чʱ��
	int n_timeAllowedtoLive;

	//���ݼ�����
	char c_dataSet[MAX_DATA_LEN_OBJ_REF];

	//goose���ƿ�ID��
	char c_goID[MAX_DATA_LEN_VISIBLE_STRING];

	//utcʱ��
	char c_t[SHORT_DATA_LEN];

	//st��
	unsigned int n_stNum;//modified on 20110825

	//sq��
	unsigned int n_sqNum; //modified on 20110825

	//���Ա�־λ
	bool b_test;
	
	//���ð汾hao
	int n_confRev;

	//NdsCom
	bool b_ndsCom;

	//���ݼ���Ŀ��
	int n_numDatSetEntries;

	//δ��
	int n_security;

	//data�ṹ��ԭ���ݽṹָ�룬�������ݸ��ݷ���������������
	MMS_DATA_STRUCT *p_data_struct;

	//ʵ�ʵ�������Ŀ
	int n_numDataStructs;//add by yinzhehong 20101124

	//Bit31:packet_len�쳣��Bit30:msg_len�쳣
	//Bit23:PDUͷ��tag��ȷ=1��Bit22:gocbRef, Bit21:timeAllowed Bit20:dataSet Bit19:T Bit18:stNum Bit17:sqNum Bit16:test Bit15:confRev Bit14:ndsCom Bit13:numDataSetEntries
	//Bit12:allData Bit11:goid ����Ԥ��
	unsigned int u_result;//������� add on 2013/03/28 ljm Ĭ��0x00000000�����ֽڵ�4���ֽڱ�ʾ�����쳣�����3��2��1���ֽڱ�ʾ���������ʶ

	GOOSE_INFO_STRUCT(){
		memset(c_src_mac,0,sizeof(c_src_mac));
		memset(c_dest_mac,0,sizeof(c_dest_mac));
		memset(c_avtm_epocsec,0,sizeof(c_avtm_epocsec));//modified on 20110827
		n_app_id = -1;
		n_msg_len=0;
		memset(c_reserved1,0,sizeof(c_reserved1));
		memset(c_reserved2,0,sizeof(c_reserved2));
		
		memset(c_gocbRef,0,sizeof(c_gocbRef));
		n_timeAllowedtoLive = 0;
		memset(c_dataSet,0,sizeof(c_dataSet));
		memset(c_goID,0,sizeof(c_goID));
		memset(c_t,0,sizeof(c_t));
		n_stNum =0;
		n_sqNum =0;
		b_test = false;
		n_confRev = 0;
		b_ndsCom = false;
		n_numDatSetEntries=0;

		n_security=0;
		//data�ṹ��ԭ���ݽṹָ�룬�������ݸ��ݷ���������������
		p_data_struct=NULL;
		n_numDataStructs = 0;
		u_result = 0x00000000;
	}

}_GOOSE_INFO_STRUCT;


#endif  /*AT_STRUCT_H_HEADER_INCLUDED_88888888*/
