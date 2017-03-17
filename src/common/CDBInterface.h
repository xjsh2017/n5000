#ifndef CDBINTERFACE_H_HEADER_INCLUDED_B3305D3B
#define CDBINTERFACE_H_HEADER_INCLUDED_B3305D3B

#include "define_scan.h"
#include "db_type.h"

/** 数据记录类型
*/
const int RECORD_TYPE_GOOSE_ANALYZE_REC_HIS = 0;   //报文分析历史数据表
const int RECORD_TYPE_GOOSE_STATUS_REL = 1;        //GOOSE报文状态记录表
const int RECORD_TYPE_GOOSE_STATUS_CHANGE_HIS = 2; //GOOSE报文变位历史记录表
const int RECORD_TYPE_NET_NODE_FLOW_OVRUN = 8;     //流量越限记录表
const int RECORD_TYPE_PCAPFILE_RPT = 10;           //文件生成记录表

const int RECORD_TYPE_MMS_ANALYZE_REC_HIS = 3;
const int RECORD_TYPE_NET_NODE_COMMUSTATUS_REL = 4;
const int RECORD_TYPE_NET_NODE_COMMUSTATUS_HIS = 5;
const int RECORD_TYPE_NET_NODE_FLOW_HIS = 6;
const int RECORD_TYPE_NET_NODE_FLOW_REL = 7;

const int RECORD_TYPE_NET_SPY_EXCEPTION = 9;

const int RECORD_TYPE_SMV_ANALYZE_REC_HIS = 11;
const int RECORD_TYPE_SYS_STATUS_HIS = 12;

/**数据库控制结构
*/
typedef struct _DB_CTRL_STRUCT
{
	char cIP[50];	          //IP地址
	char cDBName[50];	      //数据库名称
	char cUserName[50];	      //用户名
	char cPswd[50];           //密码
	char cLogPath[256];	      //日志路径
	char cMCenterName[256];  //消息中心名称
	int  nMcenterMode;       //消息中心模式 1:全系统监听  2：本消息中心监听
	int nLogLevel;			 //日志级别
	int nLogMaxLen;			 //日志文件的最大长度：单位K
	int param1;				 //备用1
	char param2[256];		 //备用2
	char param3[256];	     //备用3
}DB_CTRL_STRUCT;

/**数据库字段属性
*/
typedef struct DB_FIELDS_SET_STRUCT
{
	char c_field_name[20];	//字段名字
	char c_field_type[10];	//字段类型
	int n_field_len;	//字段长度
	char c_supplement[255];	//字段补充说明，如“COLLATE gbk_bin NOT NULL” “DEFAULT '0'”等
	bool b_key;	//该字段是否为key

}_DB_FIELDS_SET_STRUCT;

/**数据库字段数据
*/
typedef struct DB_FIELDS_CONTENT_STRUCT
{
	char c_field_name[20];	//字段名字
	char c_field_type[10];	//字段类型
	int n_field_len;	//字段长度
	char c_field_content[255];	//字段内容

}_DB_FIELDS_CONTENT_STRUCT;

/**数据记录
*/
typedef struct DB_RECORD_STRUCT
{

	int n_record_type;	//记录类型
	DB_FIELDS_CONTENT_STRUCT *p_field;	//字段内容数组首地址
	int n_field_num;	//字段个数

}_DB_RECORD_STRUCT;

/**数据表记录
*/
typedef struct DB_SHEET_STRUCT
{
	char c_sheet_name[100];	//表名
	int n_record_type;	//记录类型
	DB_RECORD_STRUCT *p_record;	//数据记录结构
	int n_record;	//记录数目

}_DB_SHEET_STRUCT;

/**数据表属性
*/
typedef struct SHEET_SET_STRUCT
{

	char c_sheet_name[100];	//表格名称
	DB_FIELDS_SET_STRUCT *p_fields_struct;	//字段属性结构指针数组首地址
	int n_fields;	//字段个数
	char c_primary_key[255];	//主键名称
	char c_key_name[255];	//其他key命名

}_SHEET_SET_STRUCT;


#ifdef OS_WINDOWS

#ifdef DB_DLL_FILE 

class _declspec(dllexport) CDBInterface //导出类 

#else 

class _declspec(dllimport) CDBInterface //导入类 

#endif //DB_DLL_FILE

#endif //OS_WINDOWS


#ifdef OS_LINUX

class CDBInterface

#endif //OS_LINUX

{

protected:

	CDBInterface(){};

public:

	/**
	* @name		Init
	* @brief	初始化资源
	* @param 	pDbCtrlStruct[in]: 数据库控制结构指针
	* @return	RES_SUCCESS 成功				RES_FAIL 失败
	*/
	virtual int  init(DB_CTRL_STRUCT *pDbCtrlStruct)=0;

	/**
	* @name		Release
	* @brief	释放资源
	* @param 	void
	* @return	RES_SUCCESS 成功	RES_FAIL 失败
	*/
	virtual int  release(void)=0;

	/**
	* @name		create_sheet
	* @brief	创建数据表
	* @param 	p_sheet_set_struct 数据表设置
	* @return	RES_SUCCESS 成功	RES_FAIL 失败
	*/
	virtual int create_sheet(SHEET_SET_STRUCT *p_sheet_set_struct) = 0;

	/**
	* @name		destroy_sheet
	* @brief	删除数据表
	* @param 	c_sheet_name 数据表名称
	* @return	RES_SUCCESS 成功	RES_FAIL 失败
	*/
	virtual int destroy_sheet(const char *c_sheet_name) = 0;

	virtual int get_sheet_name_by_type(int n_record_type, char *c_sheet_name) = 0;

	/**
	* @name		add_record
	* @brief	添加记录
	* @param 	p_record 数据结构
	* @return	RES_SUCCESS 成功	RES_FAIL 失败
	*/
	virtual int  add_record(DB_RECORD_STRUCT *p_record) = 0;

	/**
	* @name		del_record
	* @brief	删除记录
	* @param 	p_record 数据结构
	* @return	RES_SUCCESS 成功	RES_FAIL 失败
	*/
	virtual int del_record(DB_RECORD_STRUCT *p_record) = 0;

	/**
	* @name		del_records_by_time
	* @brief	按时间段删除记录
	* @param 	n_record_type 数据类型	t_begin 起始时间	t_end 结束时间
	* @return	RES_SUCCESS 成功	RES_FAIL 失败
	*/
	virtual int del_records_by_time(int n_record_type,time_t t_begin, time_t t_end) = 0;

	/**
	* @name		get_record
	* @brief	读取记录
	* @param 	db_sheet_struct 数据结构
	* @return	RES_SUCCESS 成功	RES_FAIL 失败
	*/
	virtual int get_record(DB_SHEET_STRUCT & db_sheet_struct) = 0;

	/**
	* @name		get_records_by_sql
	* @brief	直接根据sql语句查询
	* @param 	db_sheet_struct 数据结构
	* @param 	c_condition     条件
	* @return	DB_SHEET_STRUCT* 非NULL 表示成功
	*/
	virtual DB_SHEET_STRUCT*  get_records_by_sql(char* c_sql) = 0;

		/**
	* @name		release_db_records
	* @brief	清除查询结果
	* @param 	db_sheet_struct 数据结构
	* @param 	c_condition     条件
	* @return	DB_SHEET_STRUCT* 非NULL 表示成功
	*/
	virtual int release_db_records(DB_SHEET_STRUCT* pdb_sheet_struct) = 0;

   /**
	* @name		get_fields
	* @brief	读取字段属性
	* @param 	db_record_struct 数据结构
	* @return	RES_SUCCESS 成功	RES_FAIL 失败
	*/
	virtual int get_fields(DB_RECORD_STRUCT & db_record_struct) = 0;
	
	/**
	* @name		get_records_by_time
	* @brief	按时间段读取记录
	* @param 	db_sheet_struct 数据结构	t_begin 起始时间	t_end 结束时间
	* @return	RES_SUCCESS 成功	RES_FAIL 失败
	*/
	virtual int get_records_by_time(DB_SHEET_STRUCT & db_sheet_struct,time_t t_begin,time_t t_end) = 0;

	/**	
	*函数名	release_db_record_field
	*功能	释放db_record_struct结构中p_field的资源
	*参数	db_record_struct 数据存储结构
	*返回	RES_SUCCESS 成功; RES_FAIL 失败
	*/	
	virtual int release_db_record_field(DB_RECORD_STRUCT& db_record_struct) = 0;
	/**	
	*函数名	delete_tb_his_record
	*功能	获取记录个数
	*参数int n_record_type 表类型  int n_remain_num 需保留的个数
	*返回	删除的记录个数
	*/	
	virtual int delete_tb_his_records(int n_record_type,int n_remain_num) = 0;
		/**	
	*函数名	delete_tb_his_record
	*功能	获取记录个数
	*参数int n_record_type 表类型  int n_remain_num 需保留的个数
	*返回	删除的记录个数
	*/	
	virtual int execute_sql(char *c_sql)=0;

};


#ifdef OS_WINDOWS

#ifdef DB_DLL_FILE 

//导出类 

extern "C"  __declspec( dllexport ) CDBInterface * create_db_instance(void);
extern "C"  __declspec( dllexport ) void destroy_db_instance(CDBInterface * theInterface);

#else 

//导入类 

extern "C"  __declspec( dllimport ) CDBInterface * create_db_instance();
extern "C"  __declspec( dllimport ) void destroy_db_instance(CDBInterface * theInterface);

#endif //DB_DLL_FILE

#endif //OS_WINDOWS


#ifdef OS_LINUX

#ifdef __cplusplus
extern "C"{
#endif //__cplusplus
	
	CDBInterface * create_db_instance(void);
	void destroy_db_instance(CDBInterface * theInterface);
	
#ifdef __cplusplus
}
#endif //__cplusplus

#endif //OS_LINUX

#endif //CDBINTERFACE_H_HEADER_INCLUDED_B3305D3B
