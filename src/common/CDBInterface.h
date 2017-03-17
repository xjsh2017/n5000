#ifndef CDBINTERFACE_H_HEADER_INCLUDED_B3305D3B
#define CDBINTERFACE_H_HEADER_INCLUDED_B3305D3B

#include "define_scan.h"
#include "db_type.h"

/** ���ݼ�¼����
*/
const int RECORD_TYPE_GOOSE_ANALYZE_REC_HIS = 0;   //���ķ�����ʷ���ݱ�
const int RECORD_TYPE_GOOSE_STATUS_REL = 1;        //GOOSE����״̬��¼��
const int RECORD_TYPE_GOOSE_STATUS_CHANGE_HIS = 2; //GOOSE���ı�λ��ʷ��¼��
const int RECORD_TYPE_NET_NODE_FLOW_OVRUN = 8;     //����Խ�޼�¼��
const int RECORD_TYPE_PCAPFILE_RPT = 10;           //�ļ����ɼ�¼��

const int RECORD_TYPE_MMS_ANALYZE_REC_HIS = 3;
const int RECORD_TYPE_NET_NODE_COMMUSTATUS_REL = 4;
const int RECORD_TYPE_NET_NODE_COMMUSTATUS_HIS = 5;
const int RECORD_TYPE_NET_NODE_FLOW_HIS = 6;
const int RECORD_TYPE_NET_NODE_FLOW_REL = 7;

const int RECORD_TYPE_NET_SPY_EXCEPTION = 9;

const int RECORD_TYPE_SMV_ANALYZE_REC_HIS = 11;
const int RECORD_TYPE_SYS_STATUS_HIS = 12;

/**���ݿ���ƽṹ
*/
typedef struct _DB_CTRL_STRUCT
{
	char cIP[50];	          //IP��ַ
	char cDBName[50];	      //���ݿ�����
	char cUserName[50];	      //�û���
	char cPswd[50];           //����
	char cLogPath[256];	      //��־·��
	char cMCenterName[256];  //��Ϣ��������
	int  nMcenterMode;       //��Ϣ����ģʽ 1:ȫϵͳ����  2������Ϣ���ļ���
	int nLogLevel;			 //��־����
	int nLogMaxLen;			 //��־�ļ�����󳤶ȣ���λK
	int param1;				 //����1
	char param2[256];		 //����2
	char param3[256];	     //����3
}DB_CTRL_STRUCT;

/**���ݿ��ֶ�����
*/
typedef struct DB_FIELDS_SET_STRUCT
{
	char c_field_name[20];	//�ֶ�����
	char c_field_type[10];	//�ֶ�����
	int n_field_len;	//�ֶγ���
	char c_supplement[255];	//�ֶβ���˵�����硰COLLATE gbk_bin NOT NULL�� ��DEFAULT '0'����
	bool b_key;	//���ֶ��Ƿ�Ϊkey

}_DB_FIELDS_SET_STRUCT;

/**���ݿ��ֶ�����
*/
typedef struct DB_FIELDS_CONTENT_STRUCT
{
	char c_field_name[20];	//�ֶ�����
	char c_field_type[10];	//�ֶ�����
	int n_field_len;	//�ֶγ���
	char c_field_content[255];	//�ֶ�����

}_DB_FIELDS_CONTENT_STRUCT;

/**���ݼ�¼
*/
typedef struct DB_RECORD_STRUCT
{

	int n_record_type;	//��¼����
	DB_FIELDS_CONTENT_STRUCT *p_field;	//�ֶ����������׵�ַ
	int n_field_num;	//�ֶθ���

}_DB_RECORD_STRUCT;

/**���ݱ��¼
*/
typedef struct DB_SHEET_STRUCT
{
	char c_sheet_name[100];	//����
	int n_record_type;	//��¼����
	DB_RECORD_STRUCT *p_record;	//���ݼ�¼�ṹ
	int n_record;	//��¼��Ŀ

}_DB_SHEET_STRUCT;

/**���ݱ�����
*/
typedef struct SHEET_SET_STRUCT
{

	char c_sheet_name[100];	//�������
	DB_FIELDS_SET_STRUCT *p_fields_struct;	//�ֶ����Խṹָ�������׵�ַ
	int n_fields;	//�ֶθ���
	char c_primary_key[255];	//��������
	char c_key_name[255];	//����key����

}_SHEET_SET_STRUCT;


#ifdef OS_WINDOWS

#ifdef DB_DLL_FILE 

class _declspec(dllexport) CDBInterface //������ 

#else 

class _declspec(dllimport) CDBInterface //������ 

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
	* @brief	��ʼ����Դ
	* @param 	pDbCtrlStruct[in]: ���ݿ���ƽṹָ��
	* @return	RES_SUCCESS �ɹ�				RES_FAIL ʧ��
	*/
	virtual int  init(DB_CTRL_STRUCT *pDbCtrlStruct)=0;

	/**
	* @name		Release
	* @brief	�ͷ���Դ
	* @param 	void
	* @return	RES_SUCCESS �ɹ�	RES_FAIL ʧ��
	*/
	virtual int  release(void)=0;

	/**
	* @name		create_sheet
	* @brief	�������ݱ�
	* @param 	p_sheet_set_struct ���ݱ�����
	* @return	RES_SUCCESS �ɹ�	RES_FAIL ʧ��
	*/
	virtual int create_sheet(SHEET_SET_STRUCT *p_sheet_set_struct) = 0;

	/**
	* @name		destroy_sheet
	* @brief	ɾ�����ݱ�
	* @param 	c_sheet_name ���ݱ�����
	* @return	RES_SUCCESS �ɹ�	RES_FAIL ʧ��
	*/
	virtual int destroy_sheet(const char *c_sheet_name) = 0;

	virtual int get_sheet_name_by_type(int n_record_type, char *c_sheet_name) = 0;

	/**
	* @name		add_record
	* @brief	��Ӽ�¼
	* @param 	p_record ���ݽṹ
	* @return	RES_SUCCESS �ɹ�	RES_FAIL ʧ��
	*/
	virtual int  add_record(DB_RECORD_STRUCT *p_record) = 0;

	/**
	* @name		del_record
	* @brief	ɾ����¼
	* @param 	p_record ���ݽṹ
	* @return	RES_SUCCESS �ɹ�	RES_FAIL ʧ��
	*/
	virtual int del_record(DB_RECORD_STRUCT *p_record) = 0;

	/**
	* @name		del_records_by_time
	* @brief	��ʱ���ɾ����¼
	* @param 	n_record_type ��������	t_begin ��ʼʱ��	t_end ����ʱ��
	* @return	RES_SUCCESS �ɹ�	RES_FAIL ʧ��
	*/
	virtual int del_records_by_time(int n_record_type,time_t t_begin, time_t t_end) = 0;

	/**
	* @name		get_record
	* @brief	��ȡ��¼
	* @param 	db_sheet_struct ���ݽṹ
	* @return	RES_SUCCESS �ɹ�	RES_FAIL ʧ��
	*/
	virtual int get_record(DB_SHEET_STRUCT & db_sheet_struct) = 0;

	/**
	* @name		get_records_by_sql
	* @brief	ֱ�Ӹ���sql����ѯ
	* @param 	db_sheet_struct ���ݽṹ
	* @param 	c_condition     ����
	* @return	DB_SHEET_STRUCT* ��NULL ��ʾ�ɹ�
	*/
	virtual DB_SHEET_STRUCT*  get_records_by_sql(char* c_sql) = 0;

		/**
	* @name		release_db_records
	* @brief	�����ѯ���
	* @param 	db_sheet_struct ���ݽṹ
	* @param 	c_condition     ����
	* @return	DB_SHEET_STRUCT* ��NULL ��ʾ�ɹ�
	*/
	virtual int release_db_records(DB_SHEET_STRUCT* pdb_sheet_struct) = 0;

   /**
	* @name		get_fields
	* @brief	��ȡ�ֶ�����
	* @param 	db_record_struct ���ݽṹ
	* @return	RES_SUCCESS �ɹ�	RES_FAIL ʧ��
	*/
	virtual int get_fields(DB_RECORD_STRUCT & db_record_struct) = 0;
	
	/**
	* @name		get_records_by_time
	* @brief	��ʱ��ζ�ȡ��¼
	* @param 	db_sheet_struct ���ݽṹ	t_begin ��ʼʱ��	t_end ����ʱ��
	* @return	RES_SUCCESS �ɹ�	RES_FAIL ʧ��
	*/
	virtual int get_records_by_time(DB_SHEET_STRUCT & db_sheet_struct,time_t t_begin,time_t t_end) = 0;

	/**	
	*������	release_db_record_field
	*����	�ͷ�db_record_struct�ṹ��p_field����Դ
	*����	db_record_struct ���ݴ洢�ṹ
	*����	RES_SUCCESS �ɹ�; RES_FAIL ʧ��
	*/	
	virtual int release_db_record_field(DB_RECORD_STRUCT& db_record_struct) = 0;
	/**	
	*������	delete_tb_his_record
	*����	��ȡ��¼����
	*����int n_record_type ������  int n_remain_num �豣���ĸ���
	*����	ɾ���ļ�¼����
	*/	
	virtual int delete_tb_his_records(int n_record_type,int n_remain_num) = 0;
		/**	
	*������	delete_tb_his_record
	*����	��ȡ��¼����
	*����int n_record_type ������  int n_remain_num �豣���ĸ���
	*����	ɾ���ļ�¼����
	*/	
	virtual int execute_sql(char *c_sql)=0;

};


#ifdef OS_WINDOWS

#ifdef DB_DLL_FILE 

//������ 

extern "C"  __declspec( dllexport ) CDBInterface * create_db_instance(void);
extern "C"  __declspec( dllexport ) void destroy_db_instance(CDBInterface * theInterface);

#else 

//������ 

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
