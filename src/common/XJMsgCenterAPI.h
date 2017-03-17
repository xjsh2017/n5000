// XJMsgCenterAPI.h: interface for the XJMsgCenterAPI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XJMSGCENTERAPI_H__6E9918EE_8193_4913_9BE0_570E03AAFF49__INCLUDED_)
#define AFX_XJMSGCENTERAPI_H__6E9918EE_8193_4913_9BE0_570E03AAFF49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/**	
   ��Ϣ��������ģʽ,���궨�����Ͻӿ�ʹ��
*/
#define MC_PROXY_MODE 0                /*������ģʽ*/
#define MC_SWITCH_MODE 1               /*·��ģʽ*/
#define MC_MIXTURE_MODE 2              /*��ϵ���ģʽ*/
#define MC_PROXY_D_MODE 3              /*����˫��ģʽ-����*/
#define MC_MIXTURE_D_MODE 4            /*���˫��ģʽ-����*/

/*��Ϣ���ȼ�*/
/*add by yzh 20120323�½ӿ�ʹ��*/
enum  MCMessagePri
{
	MC_MESSAGE_PRI_HIGHEST = 0x0a,  //������ȼ�
	MC_MESSAGE_PRI_NORMAL,			//��ͨ���ȼ�
	MC_MESSAGE_PRI_LOWEST			//������ȼ�
};
/*��Ӧ�ó���ı�ţ�ȫϵͳͳһ*/
enum MCApp_Indentity
{
	MC_APP_SNIFFER_SWITCH = 0X01,    //װ����Ϣ����
	MC_APP_SNIFFER_DATA_SAMPLE_NET,  //����ǧ�׿ڵ����ݲɼ�
	MC_APP_SNIFFER_DATA_SAMPLE_CPCI, //����CPCI�����ݲɼ�
	MC_APP_SNIFFER_NETANALYZE,       //װ������������̡�������ͳ��
	MC_APP_SNIFFER_DATAANALYZE,      //װ�ñ��ķ������̡������߷���
	MC_APP_SNIFFER_WAVERECORD,       //¼������ģ��
	MC_APP_SNIFFER_FILESYS_MONITOR,  //�ļ�����ϵͳ
	MC_APP_SNIFFER_HISTORY_STORE,    //��ʷ���ݴ洢
	MC_APP_SNIFFER_TIME_ADJUST,      //��ʱģ��
	MC_APP_SNIFFER_DISK_CLEANER,     //���̹���
	MC_APP_SNIFFER_IEC61850_SERVER,   //IEC61850�����  
	MC_APP_SNIFFER_DATAANALYZE_SMV,		//SMV���߷�������
	MC_APP_SNIFFER_DATAANALYZE_MMS		//SMV���߷�������
};
/**	\brief	PmMyMessageStruct enum
	�ṹ����ע��(�����û�������)�����ڷֲ�����������,���0x1000��ʼ
*/
enum PmMyMessageStruct
{
	REFVALUE_SCAN_STRUCT 	   = 0x1000+0x100,  //ͨ�����ݴ���ṹ
	REFVALUE_SCAN_DATA_STRUCT  = 0x1000+0x101,  //�����ݴ���ṹ
	REFVALUE_SCAN_FRAME_STRUCT = 0x1000+0x102, //ԭʼ֡���ݴ���ṹ
};
/**	\brief MC TOPIC ��Ϣ��������
	�ṹ����ע��(�����û�������)�����ڷֲ�����������,���0x1000��ʼ
*/
//��������
/**********************************����Ϊ�°汾 add by yinzhehong 20120327***************************************************/
#define MC_MCENTER_DEFAULT                              0x0001

#define MC_TOPIC_USER_APP_NO_RECEIVE                    (0x1000 + 0x999)//��������У�ֻ���Ͳ�������Ϣ�Ŀͻ���ʹ��
#define MC_EVENT_MESSAGE_REQ                            (0x1000 + 0x109)//�¼���Ϣ_���������·������������ڲɼ�װ��ֻ�������ݣ���֧������
#define MC_EVENT_MESSAGE_NETANALYZE_REPORT              (0x1000 + 0x100)//�¼���Ϣ_�������
#define MC_EVENT_MESSAGE_DATAANALYZE_REPORT             (0x1000 + 0x102)//�¼���Ϣ_���ݷ���
#define MC_EVENT_MESSAGE_FAULTRECORD_REPORT             (0x1000 + 0x103)//�¼���Ϣ_����¼��
#define MC_EVENT_MESSAGE_DATAANALYZE_REPORT_NEW_SMV		(0x1000 + 0x104)//�¼���Ϣ_���ݷ���_SMV�ڵ�ͻ����
#define MC_EVENT_MESSAGE_DATAANALYZE_REPORT_NEW_GOOSE	(0x1000 + 0x105)//�¼���Ϣ_���ݷ���_GOOSE�ڵ�ͻ����
#define MC_EVENT_MESSAGE_DATAANALYZE_REPORT_MMS         (0x1000 + 0x106)//�¼���Ϣ_MMS���ĸ澯
#define MC_EVENT_MESSAGE_DATAANALYZE_REPORT_PTP         (0x1000 + 0x107)//�¼���Ϣ_PTP���ĸ澯
//#define MC_EVENT_MESSAGE_REQ                            (0x1000 + 0x109)//ռλ

#define MC_DATA_MESSAGE_NET_FLOW_REPORT                   (0x1000 + 0x110)//�����¼�_��������
#define MC_DATA_MESSAGE_NET_SAMPLE_DATA_REPORT            (0x1000 + 0x111)//�����¼�_ͨ������ڵ������������
#define MC_DATA_MESSAGE_FAULTRECORD_SAMPLE_DATA_REPORT    (0x1000 + 0x112)//�����¼�_ͨ��¼���ڵ�������͵�����
#define MC_DATA_MESSAGE_COMMON_DATA_REPORT                (0x1000 + 0x113)//�����¼�_ͨ������-�������͵����ݣ�����״̬����λ���ڵ�״̬��
#define MC_DATA_MESSAGE_NET_SAMPLE_DATA_GOOSE_SNAP_REPORT (0x1000 + 0x114)//�����¼�_ͨ������ڵ��������GOOSE��������
#define MC_DATA_MESSAGE_NET_SAMPLE_DATA_SMV_SNAP_REPORT   (0x1000 + 0x115)//�����¼�_ͨ������ڵ��������SMV��������
#define MC_DATA_MESSAGE_NET_SAMPLE_DATA_SMV_SAMP_REPORT   (0x1000 + 0x116)//�����¼�_ͨ������ڵ��������SMV��������
#define MC_DATA_MESSAGE_NET_SAMPLE_DATA_MULTI_SMV_SNAP_REPORT   (0x1000 + 0x117)//�����¼�_ͨ������ڵ��������ͬһʱ�̶��SMV��������

#define MC_FRAME_MESSAGE_REPORT                           (0x1000 + 0x120)//ԭʼ�����¼�

#define MC_DATA_MESSAGE_MMS_DATA_REPORT                (0x1000 + 0x118)//�����¼�_MMS���͵����ݣ��ڵ�״̬
#define MC_DATA_MESSAGE_SMV_DISPERSION_REPORT                (0x1000 + 0x119)//�����¼�_YC��ɢ��


/**************************************����Ϊ�ϰ汾****************************************************************/
//�����¼�ǲ���
#define MC_TOPIC_USER_APP_GOOSE_ANALYZE_REPORT          (0x1000 + 0x301) //GOOSE�����������
#define MC_TOPIC_USER_APP_GOOSE_STATE_REPORT            (0x1000 + 0x302) //GOOSEԭʼ���ݱ���
#define MC_TOPIC_USER_APP_GOOSE_STATE_CHANGE_REPORT     (0x1000 + 0x303) //GOOSE״̬��λ����
#define MC_TOPIC_USER_APP_SMV_ANALYZE_REPORT            (0x1000 + 0x304) //SMV�����������
#define MC_TOPIC_USER_APP_SMV_VALUE_REPORT              (0x1000 + 0x305) //SMVԭʼ�������ݱ���
#define MC_TOPIC_USER_APP_MMS_ANALYZE_REPORT            (0x1000 + 0x306) //MMS�����������
#define MC_TOPIC_USER_APP_GOOSE_STATE_REQ               (0x1000 + 0x307) //GOOSE���ķ���/ȡ����������
#define MC_TOPIC_USER_APP_SMV_STATE_REQ                 (0x1000 + 0x308) //SMV���ķ���/ȡ����������

#define MC_TOPIC_USER_APP_SCANFILE_REPORT               (0x1000 + 0x320) //��¼�ļ����ɱ���
#define MC_TOPIC_USER_APP_SCANFILE_GET_REQ              (0x1000 + 0x321) //��¼�ļ���ȡ����
#define MC_TOPIC_USER_APP_SCANFILE_MAKE_REQ             (0x1000 + 0x322) //���ɼ�¼�ļ�Ҫ��

#define MC_TOPIC_USER_APP_SNIFFER_FLOW_REPORT           (0x1000 + 0x352) //�ɼ�װ����������
#define MC_TOPIC_USER_APP_SNIFFER_FLOW_REQ              (0x1000 + 0x353) //�ɼ�װ��������ѯ����
#define MC_TOPIC_USER_APP_NETWORK_NODE_FLOWALARM_REPORT (0x1000 + 0x354) //����ڵ�����Խ�ޱ���
#define MC_TOPIC_USER_APP_NETWORK_NODE_CMMUSTATE_REPORT (0x1000 + 0x355) //����ڵ�ͨ��״̬����
#define MC_TOPIC_USER_APP_NETWORK_NODE_CMMUSTATE_REQ    (0x1000 + 0x356) //����ڵ�ͨ��״̬��ѯ����

//¼��������
#define MC_TOPIC_USER_APP_COMTRADEFILE_REPORT            (0x1000 + 0x401) //����¼���ļ�����
#define MC_TOPIC_USER_APP_COMTRADEFILE_MAKE_REQ          (0x1000 + 0x402) //����¼���ļ�Ҫ��

//��ͨ����
#define MC_TOPIC_USER_APP_COMMAND_REQ_RESULT            (0x1000 + 0x801) //����ִ�н������
#define MC_TOPIC_USER_APP_SYS_STATE_REPORT              (0x1000 + 0x802) //ϵͳ����״̬����
#define MC_TOPIC_USER_APP_SYS_RESTART_REQ               (0x1000 + 0x803) //������������
#define MC_TOPIC_USER_APP_SYS_RELOADCFG_REQ             (0x1000 + 0x804) //���¼��������ļ�����(���޸������ļ��Ժ�ʹ��)

#define MC_TOPIC_USER_APP_SYS_TIME_REPORT               (0x1000 + 0x810) //ϵͳʱ�䱨��
#define MC_TOPIC_USER_APP_SYS_TIME_REQ                  (0x1000 + 0x811) //ϵͳʱ���ѯҪ��
#define MC_TOPIC_USER_APP_SYS_TIME_MODIFY_REQ           (0x1000 + 0x812) //ϵͳʱ���޸�Ҫ��

#endif // !defined(AFX_XJMSGCENTERAPI_H__6E9918EE_8193_4913_9BE0_570E03AAFF49__INCLUDED_)
