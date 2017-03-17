// mc_struct.h: interface for the XJMsgCenterAPI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MC_STRUCT_H__6E9918EE_8193_4913_9BE0_570E03AAFF49__INCLUDED_)
#define AFX_MC_STRUCT_H__6E9918EE_8193_4913_9BE0_570E03AAFF49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000��
#include <memory.h>
#include <stdio.h>


/*��Ϣ��������ģʽö��*/
/*add by yzh 20120323�½ӿ�ʹ��*/
enum MCServiceMode
{
	MC_PROXY_S_SERVICE = 0x01,		//���շ������
		MC_PROXY_D_SERVICE,			    //˫��ͨ�Ŵ���
		MC_SWITCH_SERVICE,				//����������
		MC_MIXTURE_S_SERVICE,			//��Ϸ���(������)
		MC_MIXTURE_D_SERVICE,			//��Ϸ���(˫����)
};
/*��Ϣ���������ӿ�*/
/*add by yzh 20120323�½ӿ�ʹ��*/
typedef struct mc_config_struct
{
	unsigned int    nidentity;		  //Ӧ�ó����Ψһ��ʶ��(�������0)
	MCServiceMode   serviceType;       //��������
	char			cprogname[256];	  //Ӧ�ó������������
	char			cversion[256];	  //Ӧ�ó���İ汾��(V1.0֮��)
	char            clocalname[30];    //��ϵͳ��Ϊ��Ϣ���Ľ�������ʱ��Ӧ���е�ϵͳΨһ��ʶ[����������ʱʹ��]
	char            cremotename[15][30];//��ϵͳ��Ϊ��Ϣ���Ĵ������ʱ���������������ϵͳΨһ��ʶ��[���������ʱʹ��],���15��
	mc_config_struct()
	{
		nidentity = 10000;
		serviceType = MC_SWITCH_SERVICE;
		memset (cprogname,0,256);
		memset (cversion,0,256);
		memset (clocalname,0,30);
		memset (cremotename,0,sizeof(cremotename));
	}
}MC_CONFIG_STRUCT;

//******************************ͨ����Ϣ����ṹ**************************************//
//*****************���������¼�******************//
/**	\brief	FIELD_STRUCT
	��ṹ����
*/
#define FIELD_STRUCT_CONTENT_LEN 120
#define FIELD_STRUCT_NAME_LEN 20
typedef struct field_struct{
	
	char c_field_name[FIELD_STRUCT_NAME_LEN];      //������
	char c_field_content[FIELD_STRUCT_CONTENT_LEN];  //����
	
	field_struct()
	{
		memset (c_field_name,0,FIELD_STRUCT_NAME_LEN);
		memset(c_field_content,0,FIELD_STRUCT_CONTENT_LEN);
	}
}FIELD_STRUCT;

/**	\brief	The _Message struct
	�û��Զ��崫�͵���Ϣ���Ŀⱨ������
*/
typedef struct t_message
{
	int n_topic_type;                         //��������(ͨ���ڽṹ�����߰�Ӧ��������д),�˴��������ݱ����ͱ�ʶ��д
	int n_msgtype;                             //�������ͣ�0������ 1������ 2���ظ� event ��Ϊ0��yk��Ϊ1
	int n_unique;                              //Ψһ��ʶ �������ڵ���
	int n_event_yk_id;                         //�¼����߿�������ĵ��
	int n_result;                              //������ 0:�ɹ�  1:ʧ��,���������
	int n_reserved;                            //����
	unsigned int n_utctime;                             //�¼�����ʱ�� UTC��ʽ

	char c_sender_name[25];                    //���ķ�����,����Ϣ������д��ʹ����������д
	char c_suffix[120];                        //��׺����������д��·�����ļ���
	
	FIELD_STRUCT *p_field_struct;	           //�ֶ����������׵�ַ
	int n_field_num;						   //�ֶθ���
	char c_invokeid[40];                       //�����ź��·�ʱ���Ŀ���������
	unsigned int n_timeu;
	t_message()
	{
		n_topic_type    = 0;
		n_msgtype       = 0;
		n_unique        = 0;
		n_event_yk_id   = 0;
		n_reserved      = 0;
		n_result        = 1;
		n_utctime       = 1;
		n_timeu         = 0;
		memset(c_sender_name,0,25);
		memset(c_suffix,0,120);
		p_field_struct = NULL;
		n_field_num = 0;
		memset(c_invokeid,0,40);
	}
}T_MESSAGE;
//******************************����״̬����ṹ********************************//
/*����״̬�����ṹ*/
typedef struct mcprogramnode
{
	unsigned int	nIdentity;		//Ӧ�ó����Ψһ��ʶ��(�������0)
	char			cProgName[256];	//Ӧ�ó������������
	char			cVersion[256];	//Ӧ�ó���İ汾��(V1.0֮��)
}MCPROGRAMNODE;

/*����״̬����*/
typedef struct mc_connetmsg
{
	//��Ϣ����Proxy��Switch֮�����ӹ�ϵ������ʱ��
	unsigned int 	year;
	unsigned int    month;
	unsigned int    day;
	unsigned int    hour;
	unsigned int    minute;
	unsigned int    second;
	unsigned int    millisecond;
	//��Ϣ����Proxy��Switch֮�����ӹ�ϵ��״̬(true:��������/false:�����쳣)
	bool 			state;	
	//���ص�ַ��Ϣ(IP:PORT, ����:10.123.16.88:7064)			
	char			local[25];
    //Զ�̵�ַ��Ϣ(IP:PORT, ����:10.123.16.88:7636)		
	char            remote[25];  
	//����(�Զ�)Ӧ�ý��̵������Ϣ(Ψһ���/������������/���̰汾��)
	MCPROGRAMNODE   localnode;	
	//Զ��(�Զ�)Ӧ�ý��̵������Ϣ(Ψһ���/������������/���̰汾��)
	MCPROGRAMNODE   remotenode;
}MC_CONNECTMSG;

//******************************ģ����״̬���������ϴ�����ṹ**************************************//
/**	\brief	FIELD_STRUCT
	��ṹ����
*/
typedef struct data_field_struct
{
	unsigned int n_nodeid;//�ڵ���
	unsigned int n_unique;//���
	char         c_type;  //��������   1:��ʾ״̬��  2����ʾģ����
	double       f_value; //ֵ
	char         c_flag;  //���ݱ�ʶ   1:��Ч  0:��Ч 2::����
	unsigned int n_additional; //�������ݣ�����ʵ������ʹ��
	char         c_additional; //�������ݣ�����ʵ������ʹ��
	int          n_reserved;    //����
	data_field_struct()
	{
		n_nodeid = 0;
		n_unique  = 0;
		f_value   = 0.0f;
		c_type   = 1;
		c_flag   = 1;
		n_additional = 0;
		c_additional = 0;
		n_reserved    = 0;
	}
}DATA_FIELD_STRUCT;

/**	\brief	The DATA_MESSAGE struct
	ģ���� ״̬�� ��������
*/
typedef struct data_message
{
	int   n_topic_type;                          //��������(ͨ���ڽṹ�����߰�Ӧ��������д),�˴��������ݱ����ͱ�ʶ��д
	char  c_sender_name[25];                     //���ķ�����,����Ϣ������д��ʹ����������д,����ʱʹ��
	unsigned int   n_utctime;                             //����ʱ�� UTC��ʽ
	DATA_FIELD_STRUCT *p_data_field_struct;	     //�ֶ����������׵�ַ
	int n_field_num;						     //�ֶθ���
	char c_invokeid[40];                       //�����ź��·�ʱ���Ŀ���������
	data_message()
	{
		n_topic_type = 0;
		n_utctime    = 1;
		memset(c_sender_name,0,25);
		p_data_field_struct = NULL;
		n_field_num = 0;
		memset(c_invokeid,0,40);
	}
}DATA_MESSAGE;
//******************************ԭʼ�����ϴ�����ṹ************************//
typedef struct smv_frame_field_struct{
	
	unsigned int n_frameseq;   //֡���
	unsigned int n_framelen;   //֡����
    char         c_frame[400]; //����
	smv_frame_field_struct()
	{
		n_frameseq   = 0;
		n_framelen   = 0;
		memset(c_frame,0,400);
	}
}SMV_FRAME_FIELD_STRUCT;

typedef struct frame_message
{
	int   n_topic_type;                               //��������(ͨ���ڽṹ�����߰�Ӧ��������д),�˴��������ݱ����ͱ�ʶ��д
	int   n_msgtype;                                  //�������ͣ�0������ 1������ 2���ظ���������Ԥ��ʹ��
	char  c_dataype;                                   //��������  1:��ʾSMV
	int   n_unique;                                    //SMV�ڵ�ȫϵͳ�ڵ���
	int   n_appid;                                     //SMV��APPID��
	unsigned int   n_utctime;                          //���͵�UTCʱ��
	char  c_sender_name[25];                           //���ķ�����,����Ϣ������д��ʹ����������д,����ʱʹ��
	SMV_FRAME_FIELD_STRUCT* p_smv_frame_field_struct;  //SMV�����׵�ַ
	int n_field_num;						            //�ֶθ���

	frame_message()
	{
		n_topic_type = 0x1000 + 0x305;           //Ĭ��Ϊ����ֵ���ݴ���
		n_msgtype    = 0;
		c_dataype    = 1;
		n_unique     = 1;
		n_appid      = 0;
		n_utctime    = 1;
		memset(c_sender_name,0,25);
		p_smv_frame_field_struct = NULL;
		n_field_num = 0;
	}
}FRAME_MESSAGE;

//******************************************************************************//
//******************************����״̬����ṹ********************************//
/** @brief �ṩ���û��ص���������-ͨ����Ϣ��ȡ�ص�*/
typedef int (*PFUNPUTMESSAGE) (T_MESSAGE*);

/** @brief �ṩ���û��ص���������-�ڵ�״̬��Ϣ�ص�*/
typedef int (*PFUNMCCONNETMESSAGE) (MC_CONNECTMSG);

/** @brief �ṩ���û��ص���������-������Ϣ�ص�*/
typedef int (*PFUNMCDATAMESSAGE) (DATA_MESSAGE*);

/** @brief �ṩ���û��ص���������-������Ϣ�ص�*/
typedef int (*PFUNMCFRMEMESSAGE) (FRAME_MESSAGE*);

#endif