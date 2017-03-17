#ifndef SCDTRANSFORMER_DEFINE_INCLUDED_00000000000
#define SCDTRANSFORMER_DEFINE_INCLUDED_00000000000

/********************************************************************
*	filename: 	scdtransformer_define.h
*	author:		yinzhehong
*	created:	2013/05/16
*	purpose:  	SCD�ļ�����ģ��ӿ�ͷ�ļ�
*   version:    1.0
/*************�汾�޸ļ�¼********************/
//�޸����ڣ�  2013.05.20  �����  Ver 1.0.0.0
//�޶����ݣ�  1.�������

/*********************************************************************/
#define  DEFINE_TS_SCD_REF_LENTH    128 //����
#define  DEFINE_TS_SCD_DESC_LENTH   128 //����
typedef enum {
	scdparser_n_mdt_array = 1 ,  //1
	scdparser_n_mdt_structure,  //2
	scdparser_n_mdt_boolean,   //3
	scdparser_n_mdt_bit_string,  //4
	scdparser_n_mdt_integer,   //5
	scdparser_n_mdt_unsigned,   //6
	scdparser_n_mdt_flating_poing, //7
	scdparser_n_mdt_reserved,   //8
	scdparser_n_mdt_octet_string,  //9
	scdparser_n_mdt_visible_string, //10
	scdparser_n_mdt_generalized_time, //11
	scdparser_n_mdt_binary_time,  //12
	scdparser_n_mdt_bcd,    //13
	scdparser_n_mdt_booleanArray,  //14
	scdparser_n_mdt_obj_id,   //15
	scdparser_n_mdt_mms_string,  //16
	scdparser_n_mdt_utc_time,   //17
	scdparser_n_mdt_init = 999,      //��ֵ��999
}SCD_MMS_DATA_TYPE;
//���ö��
typedef enum
{	
	scdparser_ts_phase_n = 1,//����
	scdparser_ts_phase_a,    //A��
	scdparser_ts_phase_b,    //B��
	scdparser_ts_phase_c,    //C��
	scdparser_ts_phase_ab,
	scdparser_ts_phase_ac,
	scdparser_ts_phase_bc,
	scdparser_ts_phase_abc,
	scdparser_ts_phase_init = 999,
	
}TS_PHASE;
//�����ö��
typedef enum
{	
	scdparser_ts_seq_i1   = 1,//�������
	scdparser_ts_seq_i2,      //�������
	scdparser_ts_seq_i0,      //�������
	scdparser_ts_seq_u1,      //�����ѹ
	scdparser_ts_seq_u2,      //�����ѹ
	scdparser_ts_seq_u0,      //�����ѹ
	scdparser_ts_seq_init = 999,

}TS_SEQ;
//��λö��
typedef enum
{
	scdparser_ts_ten_unit_mv = 1,
	scdparser_ts_unit_v,
	scdparser_ts_unit_kv,
	scdparser_ts_unit_ma,
	scdparser_ts_unit_a,
	scdparser_ts_unit_ka,
	scdparser_ts_unit_mvar,
	scdparser_ts_unit_init = 999,	
}TS_UNIT;

/**************SCD�ļ��ڵ�����ö��*****************/
typedef enum
{
	TS_SCD_ITEM_TYPE_SCDUNKNOW        = 99,
	TS_SCD_ITEM_TYPE_SCDCommunication = 100,
	TS_SCD_ITEM_TYPE_SCDSubNet = 101,
	TS_SCD_ITEM_TYPE_SCDConnectAP = 102,
	TS_SCD_ITEM_TYPE_SCDGSE = 103,
	TS_SCD_ITEM_TYPE_SCDSMV = 104,
	TS_SCD_ITEM_TYPE_SCDAddress = 105,
	TS_SCD_ITEM_TYPE_SCDIED = 106,
	TS_SCD_ITEM_TYPE_SCDSERVICE = 107,      //����
	TS_SCD_ITEM_TYPE_SCDAccessPoint = 108,
	TS_SCD_ITEM_TYPE_SCDLDevice = 109,
	TS_SCD_ITEM_TYPE_SCDLN0 = 110,
	TS_SCD_ITEM_TYPE_SCDLN = 111,
	TS_SCD_ITEM_TYPE_SCDDataSet = 112,      //���ݼ�
	TS_SCD_ITEM_TYPE_SCDGSEControl = 113,   //gse���ƿ�
	TS_SCD_ITEM_TYPE_SCDSVControl = 114,    //����Contrl
	TS_SCD_ITEM_TYPE_SCDSGControl = 115,    //��ֵ���ƿ�
	TS_SCD_ITEM_TYPE_SCDRPTControl = 116,   //������ƿ�
	TS_SCD_ITEM_TYPE_SCDFCDA = 117,         //FCDA
	TS_SCD_ITEM_TYPE_SCDDOI = 118,          //DOI
	TS_SCD_ITEM_TYPE_SCDLNType = 119,       //LN����
	TS_SCD_ITEM_TYPE_SCDDO = 120,           //DO
	TS_SCD_ITEM_TYPE_SCDDOType = 121,       //DO����
	TS_SCD_ITEM_TYPE_SCDDA = 122,           //��������
	TS_SCD_ITEM_TYPE_SCDDAType = 123,       //��������
	TS_SCD_ITEM_TYPE_SCDBDA = 124,          //������������
	TS_SCD_ITEM_TYPE_SCDINPUTS = 125,       //�������
	TS_SCD_ITEM_TYPE_SCDINPUTS_EXREF = 126, //����ӹ���
	TS_SCD_ITEM_TYPE_SCDDAI = 127,          //��������ʵ��
	TS_SCD_ITEM_TYPE_SCDSDI = 128,          //DOI�°����Ľṹ����
	TS_SCD_ITEM_TYPE_SCDSDO = 129,          //SDO��DOTYPE�µ������ӽڵ� SDO��DA
	TS_SCD_ITEM_TYPE_SCDENUMType = 130,     //ö������
}TS_SCD_ITEM_TYPE;
/***********SCD��Ŀ��***********/
typedef struct SCD_ITEM
{
	unsigned int n_seq;//���
	char c_item_ref[DEFINE_TS_SCD_REF_LENTH];  //����
	char c_item_desc[DEFINE_TS_SCD_DESC_LENTH ];//����
	int enum_mms_data_type;   //��������
	int enum_ts_phase; //���
	int enum_ts_unit;  //��λ
	char c_mms_data_type[32];               //mms�������ͣ��ַ�����
	SCD_ITEM* p_child_item_list_head;       //����Ŀ����ͷ
	unsigned int n_childitem_list_size;     //����Ŀ���г���

}_SCD_ITEM;
/***********SCD��Ŀ����***********/
typedef struct SCD_ITEM_LIST
{
	unsigned int n_appid;//appid �ţ������appid������appid��
	char c_ied_name[32]; //��Ӧ��IED����
	char c_list_ref[DEFINE_TS_SCD_REF_LENTH];  //���еĲ���
	char c_list_desc[DEFINE_TS_SCD_DESC_LENTH];//���е�����
	unsigned int n_list_size;  //���г���
	SCD_ITEM* p_itemlist_head; //����ͷ
	SCD_ITEM* p_attribute_item_list_head;       //������Ŀ����ͷ,����������APP��·�ĸ�������ʱ����
	unsigned int n_attr_list_size;              //���Զ��г���
}_SCD_ITEM_LIST;
#endif