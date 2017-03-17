#ifndef SYS_MODEL_STRUCT_INCLUDED_00000000000
#define SYS_MODEL_STRUCT_INCLUDED_00000000000
/********************************************************************
*	filename: 	SYS_MODEL_STRUCT.h
*	author:		yinzhehong
*	created:	2010/11/03
*	purpose:  	ϵͳģ�Ͷ���   
*   version:    1.0
*********************************************************************/

//#include <memory>
#include "define_scan.h"

#define MAX_SNIFFER_NUM	       16    //ϵͳ��¼װ��������
#define MAX_SNIFFER_CARD_NUM   6     //���ɼ�װ��Я���Ĳɼ��忨������
#define MAX_SNIFFER_PORT_NUM   4     //�ɼ�����Я�������˿���
#define MAX_SNIFFER_MU_NUM     16    //ÿ���˿ڿɽ�������MU����
#define MAX_SMV_ITEM           32    //����MU���Ĳ���ͨ������
#define MAX_GOOSE_ITEM         256   //�������ܲ��������goose��Ŀ
#define MAX_DESCRIBE_LEN	   256   //��������ַ�����
#define WS_MAX_IP_LEN		   31    //IP��ַ����
#define MAX_SHORT_DATA_LEN     10    //Ӧ�����ݳ���
#define MAX_NAME_LENGTH        120   //����ռ���ֽ���
#define MAX_USER_PSWD_LEN      17    //�ʺ����볤���ֽ�

//�����ӵĽڵ㣬���Ĳɼ��˿��µ�ĳ����·�շ�������
typedef struct SNIFFER_APP
{
	int nsys_appsqe;                     //ϵͳ��� ��100��ʼ���ñ�Ų�����˿ڵ�ϵͳ����ظ�
	char         cdesc[MAX_NAME_LENGTH]; //�ڵ�����
	int snifferport_sys_id;              //�����ɼ��˿�ϵͳ���
	int napptype;                        //�ڵ�����  1:mms 2:goose 3:9-2 5:1588 6������
	unsigned int nappid;                 //app_id
	char   ciedname[100];                //�ڵ��Ӧ��ied����
	char   cipaddress[20];               //�ڵ�IP��ַ
	char   cmacaddress[30];              //mac��ַ
	char   ccbname[MAX_NAME_LENGTH];    //���ƿ�����
	char   cdsname[MAX_NAME_LENGTH];    //���ݼ�����
	char   cgoid[MAX_NAME_LENGTH];      //GOOSE���ƿ�ID
	char   csvid[MAX_NAME_LENGTH];      //SMV���ƿ�ID
	int    nconfrev;                    //�汾��
	int ndssize;                        //���ݼ������������
	int nmdatalistsize;                 //���ӵ��ź�������
	int nnofaudu;                       //��֡asdu����
	int nsamprate;                      //����Ƶ��
	int nmintime;                       //���ķ�����Сʱ������Ĭ��ֵΪ2����
	int nmaxtime;                       //���ķ������ʱ������Ĭ��ֵΪ5000����
	bool         bmonitor;              //�Ƿ���м���
	float        flmtflowup;            //��������
	float        flmtflowdown;          //��������
	float        fRCT;                  //CT���
	float        fRPT;                  //PT���
	int          ntimeout;              //�ڵ��ж����ʱ�䣬��λΪ��
	bool         bck_2ad;               //�Ƿ���˫AD
	char	csubnet[30];				//����
	SNIFFER_APP()
	{
		nsys_appsqe = 100;
		memset(cdesc,0,sizeof(cdesc));
		memset(cipaddress,0,sizeof(cipaddress));
		memset(cmacaddress,0,sizeof(cmacaddress));
		memset(cgoid,0,sizeof(cgoid));
		memset(csvid,0,sizeof(csvid));
		nconfrev = 0;
		snifferport_sys_id = 1;
		napptype    = 3;
		nappid      = 0;
		memset(ciedname,0,sizeof(ciedname));
		memset(ccbname,0,sizeof(ccbname));
		memset(cdsname,0,sizeof(cdsname));
		ndssize = 12;
		nnofaudu = 1;
		nsamprate = 4000;
		nmintime = 2;
		nmaxtime = 5000;
		bmonitor = 1;
		flmtflowup = 1.0f;
		flmtflowdown = 0.001f;
		fRPT = 350.0f;
		fRCT = 600.0f;
		nmdatalistsize = 12;
		ntimeout       = 60;
		bck_2ad        = false;
	}
}_SNIFFER_APP;

//�������ݲ�׽���˿�����
typedef struct SNIFFER_PORT
{
	int ncard_portseq;                  //�ö˿������ݿ��ı�ţ���1��ʼ��Ŀǰ���ֵΪ6
	int nsys_portseq;                   //ϵͳ����˿����,ȫϵͳΨһ����1��ʼ
	char cportname[MAX_DESCRIBE_LEN];   //�˿�����
	int nusetype;                       //�˿���;  1:mms���ݲɼ� 2:goose�������ݲɼ� 3:9-2�������ݲɼ� 4:9-1���� 5:1588��ʱ����  10; ��ϱ���
	int nsniffer_cardseq;               //���ݲ�׽���忨�ı��
	float flmtflowup;                     //�˿��������ޣ���λM�ֽ�
	float flmtflowdown;                   //�˿��������ޣ���λM�ֽ�
	int nstoragegroup;                  //�ɼ����ݴ洢��ţ����̽��з��������3�飬�����������������˿ڲɼ��������ݴ浽��һ���飬����ֵΪ1-3
	int nstoragegroup_slow;            //�����ݴ洢���,����GOOSE���ݼ��������ݵĴ洢��ţ�����ֵΪ1-3
	int nfilebufferlen_goose;	       //goose���Ļ��壬��λM 
	int nfilebufferlen_other;	       //�������Ļ��庬SMV����λM

	int nappnum_goose;
	SNIFFER_APP **plisthead_snifferapp_goose;

	int nappnum_smv;
	SNIFFER_APP **plisthead_snifferapp_smv;

	int nappnum_mms;
	SNIFFER_APP **plisthead_snifferapp_mms;
	
    SNIFFER_PORT()
	{
		ncard_portseq = 1;
		nsys_portseq  = 1;
		memset(cportname,0,sizeof(cportname));
		nusetype      = 1;
		flmtflowup    = 6.0f;
		flmtflowdown  = 0.1f;
		nstoragegroup     = 1;
		nstoragegroup_slow =2;
		nappnum_goose = 0;
		plisthead_snifferapp_goose = NULL;
		nappnum_smv = 0;
		plisthead_snifferapp_smv = NULL;
		nappnum_mms = 0;
		plisthead_snifferapp_mms = NULL;

		nfilebufferlen_goose  = 1;
		nfilebufferlen_other  = 1;
	}
}_SNIFFER_PORT;

//���ݲ�׽���忨
typedef struct SNIFFER_CARD
{
	int  nsniffer_cardseq;               //�ð忨�ڲɼ�װ���е���ţ�Ҫ��Ψһ����2��ʼ�࣬��Χ2-7
	char ccardname[MAX_DESCRIBE_LEN];    //�ɼ�������
	char mfcsn[64];                      //�������ұ��룬�����������ص���;
	int  nportnum;                       //��Ч�ɼ��ڸ���(ָ����ϵͳ��ʵ��Ӧ�õĸ���)
	char cip1addr[WS_MAX_IP_LEN];         //�ɼ���ip��ַ1
	char cip2addr[WS_MAX_IP_LEN];         //�ɼ���ip��ַ2
	SNIFFER_PORT** plisthead_snifferport; //�˿ڶ���
	int  ndatasavemode;                   //���ݴ洢��ʽ��0:���˿ڴ� 1:������ ������ʱ���������ݴ浽���������õĵ�һ���˿���
	SNIFFER_CARD()
	{
		nsniffer_cardseq = 1;
		memset(ccardname,0,sizeof(ccardname));
		memset(mfcsn,0,sizeof(mfcsn));
		memset(cip1addr,0,sizeof(cip1addr));//ip��ַ1
		memset(cip2addr,0,sizeof(cip2addr));//ip��ַ2
		nportnum = 0;
		plisthead_snifferport = NULL;
		ndatasavemode         = 0;
	}
}_SNIFFER_CARD;

//���ݲ�׽��
typedef struct SNIFFER
{
	int  nsys_seq;                           //��׽����ţ�ϵͳ��Ψһ����1��ʼ��д
	char csniffername[MAX_DESCRIBE_LEN];     //��׽������
	int  ncardnum;                           //�ɼ��忨����
	SNIFFER_CARD** plisthead_sniffercard;    //�ɼ���ָ��
	char cip1address[WS_MAX_IP_LEN];         //��׽��IP��ַ1������ϵͳ����
	char cip2address[WS_MAX_IP_LEN];         //��׽��IP��ַ2������ϵͳ����
	char cFtpUser[MAX_USER_PSWD_LEN];         //��׽��ftp
	char cFtpPswd[MAX_USER_PSWD_LEN];         //��׽��ftp
	int  nFtpPort;                            //��׽��ftp
	SNIFFER()
	{
		nsys_seq = 1;
		memset(csniffername,0,sizeof(csniffername));
		ncardnum = 1;
		nFtpPort = 21;
		plisthead_sniffercard = NULL;
		memset(cip1address,0,sizeof(cip1address));
		memset(cip2address,0,sizeof(cip2address));
		strcpy(cFtpUser,"ftp");
		strcpy(cFtpPswd,"ftp");
	}
}_SNIFFER;
typedef struct SCANNER
{
	char csysname[MAX_DESCRIBE_LEN];     //ϵͳ���� ����ʵ�ʹ��������޸�
	char csysdate[32];                   //�����ļ�����ʱ��yyyy-mm-dd hh-mm-ss
	char csysmaker[16];                  //�����ļ�����������
	char cnotes[MAX_DESCRIBE_LEN];       //��ע˵��
	int  nsniffernum;                    //��¼װ�ø���,����1��
	SNIFFER** plisthead_sniffer;         //��¼װ���׵�ַ

	SCANNER()//��ʼ�����
	{
		memset(csysname,0,sizeof(csysname));
		memset(csysdate,0,sizeof(csysdate));
		memset(csysmaker,0,sizeof(csysmaker));
		memset(cnotes,0,sizeof(cnotes));
		nsniffernum  = 0;
		plisthead_sniffer= NULL;
	}
}_SCANNER;


//ͨͨ���ṹ����
typedef struct CFG_REC_CHANNEL
{
	int  nsys_channelseq;                 //"id"ͨ����ţ��ɱ�ϵͳ���䣬ȫϵͳΨһ��ţ����е�ͨ����˳���ţ���1��ʼ
	char cname[MAX_DESCRIBE_LEN];         //ͨ������
	char cdesc[MAX_DESCRIBE_LEN];         //˵��
	int  nsnifferport_sys_id;             //���ݲɼ��ڶ�Ӧ��sys_id
	int  nchaneltype;                     //ͨ�����  0��ģ����ͨ�� 1��������ͨ��
	char  cphase[5];                      //���
	char  cunits[12];                     //��λ
	int nTwinChannel;                    //˫ͨ��
	float fratio;                        //���ȡ���λ������Ӧ�ĸ���ֵ//Ӧ�ò��Ǳ��ct����pt
	float fratioSpec;                    //���ⱶ��
	float fmin;                          //��Сֵ
	float fmax;                          //���ֵ
	int  nstatus;                        //״ֵ̬ ͨ������״̬��������1 ���գ�0  ������ͨ��ʱ����
	int  n61850_data_type;               //61850��������
	char c_scd_ref[MAX_DESCRIBE_LEN];    //61850����
	int  n_sniffapp_sys_appseq;          //����Ӧ����·�����
	int  n_index_in_list;               //����·��ͨ�������е�λ��
	//��ʼ��
	CFG_REC_CHANNEL()
	{
		nsys_channelseq = 1;
		nsnifferport_sys_id = 0;
		fratio =1.0;
		fratioSpec = -1.0;
		nTwinChannel = -1;
		memset(cname,0,sizeof(cname));
		memset(cunits,0,sizeof(cunits));
		memset(cphase,0,sizeof(cphase));
		memset(cdesc,0,sizeof(cdesc));
		fmin = 0.0;
		fmax = 0.0;
		nchaneltype = 0;
		nstatus = 0;
		n61850_data_type = 999;
		memset(c_scd_ref,0,sizeof(c_scd_ref));
		n_sniffapp_sys_appseq = 0;
		n_index_in_list = 0;
	}
}_CFG_REC_CHANNEL;//ͨ��

//Ӧ�ýڵ�ͨ������
typedef struct _APP_CHANNEL_RELATION
{
	int n_sniffer_app_sys_id;
	int n_yc_channel_num;
	int n_yx_channel_num;
	CFG_REC_CHANNEL** pyc_channel_list;//ң��ͨ������
	CFG_REC_CHANNEL** pyx_channel_list;//ң��ͨ������

	int n_sys_yc_channel_num;             //ϵͳ���е�ң�Ÿ���
	int n_sys_yx_channel_num;             //ϵͳ���е�ң�����

	CFG_REC_CHANNEL** pyc_sys_channel_list;//ң��ͨ������_ϵͳ����
	CFG_REC_CHANNEL** pyx_sys_channel_list;//ң��ͨ������_ϵͳ����

	_APP_CHANNEL_RELATION()
	{
		n_sniffer_app_sys_id = 0;
		n_yc_channel_num     = 0;
		n_yx_channel_num     = 0;
		pyc_channel_list     = NULL;
		pyx_channel_list     = NULL;
		n_sys_yc_channel_num = 0;
		n_sys_yx_channel_num = 0;
		pyc_sys_channel_list = NULL;
		pyx_sys_channel_list = NULL;
	}
}APP_CHANNEL_RELATION;

/**********����ӽṹ************/
typedef struct TS_IED_EXTREF
{
	char c_iedname[32];    //iedname
	char c_apname[32];     //accesspoint
	char c_ldname[64];     //ld����
	char c_lddesc[64];     //ld����
	char c_intaddr[128];   //�ڲ���ַ
	char c_intdesc[128];   //�ڲ�����
	char c_iedname_ext[32];//����IED����
	char c_ldinst_ext[32]; //����IED��LD
	char c_prefix_ext[32]; //����LN��ǰ׺
	char c_lnclass_ext[32];//�����LN����
	char c_lninst_ext[10]; //�����LNʵ����
	char c_doname_ext[32]; //�����doname
	char c_daname_ext[64];//�����daname ������Ϊ��������
	char c_outdesc_ext[128];//������ⲿ����
	void* pvoid;           //һ�����õ�ָ��
	TS_IED_EXTREF()
	{
		//�ڲ�����
		memset(c_iedname,0,sizeof(c_iedname));
		memset(c_apname,0,sizeof(c_apname));
		memset(c_ldname,0,sizeof(c_ldname));
		memset(c_lddesc,0,sizeof(c_lddesc));
		memset(c_intaddr,0,sizeof(c_intaddr));
		memset(c_intdesc,0,sizeof(c_intdesc));
		//�ⲿ����
		memset(c_iedname_ext,0,sizeof(c_iedname_ext));
		memset(c_ldinst_ext,0,sizeof(c_ldinst_ext));
		memset(c_prefix_ext,0,sizeof(c_prefix_ext));
		memset(c_lnclass_ext,0,sizeof(c_lnclass_ext));
		memset(c_lninst_ext,0,sizeof(c_lninst_ext));
		memset(c_doname_ext,0,sizeof(c_doname_ext));
		memset(c_daname_ext,0,sizeof(c_daname_ext));
		memset(c_outdesc_ext,0,sizeof(c_outdesc_ext));
		pvoid = NULL;
	}
}_TS_IED_EXTREF;
/**********����ӹ�����ϵ************/
typedef struct TS_EXTREF_REC_CHANNE_RELATION
{
	TS_IED_EXTREF*         p_ts_ied_exref;       //����ͨ����ָ��
	CFG_REC_CHANNEL*       p_cfg_channel;        //���ͨ����ָ��                 
	TS_EXTREF_REC_CHANNE_RELATION()
	{
		p_ts_ied_exref = NULL;
		p_cfg_channel  = NULL;
	}
}_TS_EXTREF_REC_CHANNE_RELATION;
/**********��·������������************/
//����·���й���
/*typedef struct TS_SNIFFER_APP_INOUT_MNGR
{
	SNIFFER_APP* p_sniffer_app;    //Ӧ�ýڵ�
	TS_EXTREF_REC_CHANNE_RELATION** p_ts_extref_rec_channel_relationlist;
	int n_relationlist_size;       //���г���
	TS_SNIFFER_APP_INOUT_MNGR()
	{
		p_ts_extref_rec_channel_relationlist = NULL;
		p_sniffer_app = NULL;
		n_relationlist_size = 0;
	}
}_TS_SNIFFER_APP_INOUT_MNGR;*/
/**********װ��������������************/
/*struct TS_IED;
typedef struct TS_IED_INOUT_MNGR
{
	TS_IED* p_ts_ied;
	TS_SNIFFER_APP_INOUT_MNGR** p_ts_sniffer_app_inout_mngr_list;
	int n_inout_mngr_list_size;
	TS_IED_INOUT_MNGR()
	{
		p_ts_ied = NULL;
		p_ts_sniffer_app_inout_mngr_list = NULL;
		n_inout_mngr_list_size           = 0;
	}
}_TS_IED_INOUT_MNGR;*/
/**********�����ն˽ṹ************/
typedef struct TS_IED
{
	 char c_name[32];                    //���ƣ�ȫվΨһ
	 char c_desc[64];                    //����
	 char c_manufacturer[64];            //��������
	 char c_type[64];                    //�ͺ�
	 TS_IED_EXTREF** p_ts_ied_extreflist;//����Ӷ���
	 int n_extreflist_size;              //����Ӹ���
	 SNIFFER_APP** p_sniff_out_applist;     //IED�¶�Ӧ�������·
	 int n_snff_out_applist_size;         //IED�¶�Ӧ�������·����
	 SNIFFER_APP** p_sniff_in_applist;     //IED�¶�Ӧ��������·
	 int n_snff_in_applist_size;         //IED�¶�Ӧ��������·����
	 TS_EXTREF_REC_CHANNE_RELATION** p_ts_ied_in_relation_list;//�����������ied�б�
	 int  n_ts_ied_in_relation_list_size;//���������ied�ĸ���
	 TS_EXTREF_REC_CHANNE_RELATION** p_ts_ied_out_relation_list;//�����������ied�б�
	 int  n_ts_ied_out_relation_list_size;//���������ied�ĸ���
	 TS_IED()
	 {
		memset(c_name,0,sizeof(c_name));
		memset(c_desc,0,sizeof(c_desc));
		memset(c_manufacturer,0,sizeof(c_manufacturer));
		memset(c_type,0,sizeof(c_type));
		p_ts_ied_extreflist = NULL;
		n_extreflist_size = 0;
		p_sniff_out_applist = NULL;
		n_snff_out_applist_size = 0;
		p_sniff_in_applist  = NULL;
		n_snff_in_applist_size = 0;
		p_ts_ied_in_relation_list = NULL;
		n_ts_ied_in_relation_list_size = 0;
		p_ts_ied_out_relation_list = NULL;
		n_ts_ied_out_relation_list_size = 0;
	}
}_TS_IED;

#endif //SYS_MODEL_STRUCT_INCLUDED_00000000000
