#ifndef SCDTRANSFORMER_DEFINE_INCLUDED_00000000000
#define SCDTRANSFORMER_DEFINE_INCLUDED_00000000000

/********************************************************************
*	filename: 	scdtransformer_define.h
*	author:		yinzhehong
*	created:	2013/05/16
*	purpose:  	SCD文件加载模块接口头文件
*   version:    1.0
/*************版本修改记录********************/
//修改日期：  2013.05.20  尹浙洪  Ver 1.0.0.0
//修订内容：  1.初稿完成

/*********************************************************************/
#define  DEFINE_TS_SCD_REF_LENTH    128 //参引
#define  DEFINE_TS_SCD_DESC_LENTH   128 //描述
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
	scdparser_n_mdt_init = 999,      //初值，999
}SCD_MMS_DATA_TYPE;
//相别枚举
typedef enum
{	
	scdparser_ts_phase_n = 1,//零序
	scdparser_ts_phase_a,    //A相
	scdparser_ts_phase_b,    //B相
	scdparser_ts_phase_c,    //C相
	scdparser_ts_phase_ab,
	scdparser_ts_phase_ac,
	scdparser_ts_phase_bc,
	scdparser_ts_phase_abc,
	scdparser_ts_phase_init = 999,
	
}TS_PHASE;
//序分量枚举
typedef enum
{	
	scdparser_ts_seq_i1   = 1,//正序电流
	scdparser_ts_seq_i2,      //负序电流
	scdparser_ts_seq_i0,      //零序电流
	scdparser_ts_seq_u1,      //正序电压
	scdparser_ts_seq_u2,      //负序电压
	scdparser_ts_seq_u0,      //零序电压
	scdparser_ts_seq_init = 999,

}TS_SEQ;
//单位枚举
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

/**************SCD文件节点类型枚举*****************/
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
	TS_SCD_ITEM_TYPE_SCDSERVICE = 107,      //服务
	TS_SCD_ITEM_TYPE_SCDAccessPoint = 108,
	TS_SCD_ITEM_TYPE_SCDLDevice = 109,
	TS_SCD_ITEM_TYPE_SCDLN0 = 110,
	TS_SCD_ITEM_TYPE_SCDLN = 111,
	TS_SCD_ITEM_TYPE_SCDDataSet = 112,      //数据集
	TS_SCD_ITEM_TYPE_SCDGSEControl = 113,   //gse控制块
	TS_SCD_ITEM_TYPE_SCDSVControl = 114,    //采样Contrl
	TS_SCD_ITEM_TYPE_SCDSGControl = 115,    //定值控制块
	TS_SCD_ITEM_TYPE_SCDRPTControl = 116,   //报告控制块
	TS_SCD_ITEM_TYPE_SCDFCDA = 117,         //FCDA
	TS_SCD_ITEM_TYPE_SCDDOI = 118,          //DOI
	TS_SCD_ITEM_TYPE_SCDLNType = 119,       //LN类型
	TS_SCD_ITEM_TYPE_SCDDO = 120,           //DO
	TS_SCD_ITEM_TYPE_SCDDOType = 121,       //DO类型
	TS_SCD_ITEM_TYPE_SCDDA = 122,           //数据属性
	TS_SCD_ITEM_TYPE_SCDDAType = 123,       //数据类型
	TS_SCD_ITEM_TYPE_SCDBDA = 124,          //基本数据类型
	TS_SCD_ITEM_TYPE_SCDINPUTS = 125,       //输入端子
	TS_SCD_ITEM_TYPE_SCDINPUTS_EXREF = 126, //虚端子关联
	TS_SCD_ITEM_TYPE_SCDDAI = 127,          //数据属性实例
	TS_SCD_ITEM_TYPE_SCDSDI = 128,          //DOI下包含的结构属性
	TS_SCD_ITEM_TYPE_SCDSDO = 129,          //SDO，DOTYPE下的两个子节点 SDO和DA
	TS_SCD_ITEM_TYPE_SCDENUMType = 130,     //枚举类型
}TS_SCD_ITEM_TYPE;
/***********SCD条目项***********/
typedef struct SCD_ITEM
{
	unsigned int n_seq;//序号
	char c_item_ref[DEFINE_TS_SCD_REF_LENTH];  //参引
	char c_item_desc[DEFINE_TS_SCD_DESC_LENTH ];//描述
	int enum_mms_data_type;   //数据类型
	int enum_ts_phase; //相别
	int enum_ts_unit;  //单位
	char c_mms_data_type[32];               //mms数据类型，字符描述
	SCD_ITEM* p_child_item_list_head;       //子条目队列头
	unsigned int n_childitem_list_size;     //子条目队列长度

}_SCD_ITEM;
/***********SCD条目队列***********/
typedef struct SCD_ITEM_LIST
{
	unsigned int n_appid;//appid 号，如果是appid，就有appid号
	char c_ied_name[32]; //对应的IED名称
	char c_list_ref[DEFINE_TS_SCD_REF_LENTH];  //队列的参引
	char c_list_desc[DEFINE_TS_SCD_DESC_LENTH];//队列的描述
	unsigned int n_list_size;  //队列长度
	SCD_ITEM* p_itemlist_head; //队列头
	SCD_ITEM* p_attribute_item_list_head;       //属性条目队列头,当用来描述APP链路的各个参数时有用
	unsigned int n_attr_list_size;              //属性队列长度
}_SCD_ITEM_LIST;
#endif