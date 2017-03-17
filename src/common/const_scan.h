#if !defined(CONST_SCAN_H_)
#define CONST_SCAN_H_

//using namespace std;???

/*********************************�߳�״̬����*************************/
/**	
* @brief	�߳����С�
*/
const int	THREAD_STATE_RUN							= 1;

/**	
* @brief	�߳���ͣ��
*/	
const int	THREAD_STATE_PAUSE							= 2;	

/**	
* @brief	�߳�ͣ�ˡ�
*/	
const int	THREAD_STATE_STOP							= 3;	

/**	
* @brief	�߳�æ��
*/
const int	THREAD_STATE_BUSY							= 4;

/**	
* @brief	�߳̽�����
*/
const int	THREAD_STATE_END							= 5;

/**	
* @brief	�̴߳���
*/
const int	THREAD_STATE_ERFROR							= 0;



const int	MAX_PACKET_LEN								= 1492;
// const int	MAX_PACKET_LEN								= 1520;//modifed 20130316



/*********************************�ַ�����*************************/

/**	
* @brief	·�����ŵ��ֽڡ�
*/
#ifdef OS_WINDOWS
const char	PCHAR_PATH									='\\';
#else
const char	PCHAR_PATH									='/';
#endif

/**	
* @brief	·�������ַ�����
*/
#ifdef OS_WINDOWS
const char	PSTR_PATH[]									="\\";
#else
const char	PSTR_PATH[]									="/";
#endif


enum ETHER_TYPE
{
	IEC61850_ETHER_TYPE_UNKOWN  =0,
	IEC61850_ETHER_TYPE_MMS     =1,
	IEC61850_ETHER_TYPE_GOOSE   =2,
	IEC61850_ETHER_TYPE_SMV     =3,
	IEC61850_ETHER_TYPE_SMV_91  =4,
	IEC61850_ETHER_TYPE_PTP_1588=5,
	ETHER_TYPE_ARP              =6,
	ETHER_TYPE_IP               =7,
	ETHER_TYPE_TCP              =8,
	ETHER_TYPE_UDP              =9,
	ETHER_TYPE_COTP             =13,
	ETHER_TYPE_TCP_FIN			=14,//�ͷ�TCP���� 
	ETHER_TYPE_TCP_SYN_REQ		=15,//����
	ETHER_TYPE_TCP_SYN_RESP  	=16,//��������
	ETHER_TYPE_TCP_RST			=17,//��������
	ETHER_TYPE_TCP_PSH			=18,//��Ӧ�����ݣ����շ���Ҫ���촦��
	ETHER_TYPE_TCP_ACK			=19,//��������
	ETHER_TYPE_TCP_URG			=20,//��������
	ETHER_TYPE_TPKT             =21,//add by yinzhehong 20130321
	ETHER_TYPE_DROP             =22,//add by guohuawen 20151010 //��·�㶪������
};	

#define MAX_FILE_NAME_LEN 255


enum UNIQUE_DEFINE
{
	UNIQUE_PKT_PER_SEC_NODE = 50,			//�ڵ�֡/s��unique
	UNIQUE_BYTE_PER_SEC_NODE = 51,			//�ڵ��ֽ�/s��unique
	UNIQUE_PKTNUM_NODE = 52,				//�ڵ���֡����unique
	
	UNIQUE_PKT_PER_SEC_GOOSE_NODE = 100,	//�˿�����goose�ڵ�֡/s��unique
	UNIQUE_BYTE_PER_SEC_GOOSE_NODE = 101,	//�˿�����goose�ڵ��ֽ�/s��unique
	UNIQUE_PKTNUM_NODE_GOOSE = 102,			//�˿�����goose�ڵ����֡����unique

	UNIQUE_COMMU_NODE = 1001				//�ڵ�ͨѶ״̬unique
};


//�˿����¼�ID��
enum PORT_EVENT_ID_DEFINE
{
	EVENT_ID_FILE_GENERATE = 1,						//�ļ�����
	EVENT_ID_SMV_NODE_ADD_ABRUPTLY = 2,				//SMV�ڵ�ͻ��
	EVENT_ID_GOOSE_NODE_ADD_ABRUPTLY = 3,			//GOOSE�ڵ�ͻ��
	EVENT_ID_NET_NODE_ADD_ABRUPTLY = 4,				//����ڵ�ͻ��
	EVENT_ID_FRAME_DROP = 5,				        //��·�㶪֡

	EVENT_ID_NET_STORM =100,						//����籩

	EVENT_ID_NODE_OVER_FLOWUP = 101,				//�ڵ�Խ����
	EVENT_ID_NODE_OVER_FLOWDOWN = 102,				//�ڵ�Խ����
	EVENT_ID_NODE_OVER_FLOW_COMEBACK = 103			//�ڵ�Խ�޻ָ�



};

//SMV�ڵ����¼�ID��
enum SMV_NODE_EVENT_ID_DEFINE
{
	SMV_NODE_EVENT_ID_ASYNCHRONISM = 1,					//��ʧͬ���ź� asynchronism
	SMV_NODE_EVENT_ID_DANUM_CHANGE,						//����ֵ�����仯
	SMV_NODE_EVENT_ID_DOUBLE_SMV,						//˫SMV
	SMV_NODE_EVENT_ID_SMPCUNT_JUMP,						//��֡
	SMV_NODE_EVENT_ID_SMPCUNT_REVERSE,					//����
	SMV_NODE_EVENT_ID_DELAY,							//�ӳ�
	SMV_NODE_EVENT_ID_DISCONNECT,						//��·�ж�
	SMV_NODE_EVENT_ID_DATANUM_DIFFER_CFG ,				//���ݸ�����SCD�����ݼ���Ա������ͬ
	SMV_NODE_EVENT_ID_QUALITY_ERROR,					//����Ʒ���쳣
	SMV_NODE_EVENT_ID_APPID_ZERO,						//APPIDΪ0
	SMV_NODE_EVENT_ID_PDU_LEN_ERROR,					//PDU����Ϊ0
	SMV_NODE_EVENT_ID_SVID_DIFFER_CFG,					//svID�����ò�ͬ
	SMV_NODE_EVENT_ID_DS_DIFFER_CFG,					//dataset���������ò�ͬ
	SMV_NODE_EVENT_ID_CONFREV_DIFFER_CFG,				//confRev�����ò�ͬ
	SMV_NODE_EVENT_ID_DESTMAC_ERROR,					//Ŀ��MAC��ַ����
	SMV_NODE_EVENT_ID_CONNECT,							//��·�ָ�
	SMV_NODE_EVENT_ID_QUALITY_CHANGE,					//����Ʒ�ʱ仯


	SMV_NODE_EVENT_ID_DOUBLE_AD_DATA_DIFFER,			//˫AD���ݲ�һ��
	SMV_NODE_EVENT_ID_RATE_UNSTABLE,					//Ƶ�ʲ��ȶ�
	SMV_NODE_EVENT_ID_PORTMAT_ERR,						//���ĸ�ʽ����



	SMV_NODE_EVENT_ID_NET_STORM =100,					//����籩
		
	SMV_NODE_EVENT_ID_NODE_OVER_FLOWUP = 101,			//�ڵ�Խ����
	SMV_NODE_EVENT_ID_NODE_OVER_FLOWDOWN = 102,			//�ڵ�Խ����
	SMV_NODE_EVENT_ID_NODE_OVER_FLOW_COMEBACK = 103		//�ڵ�Խ�޻ָ�
	

};

enum GOOSE_NODE_EVENT_ID_DEFINE
{

	GOOSE_NODE_EVENT_ID_SQNUM_JUMP = 1,				//sqNum����
	GOOSE_NODE_EVENT_ID_SQNUM_REVERSE,				//sqNum��ת
	GOOSE_NODE_EVENT_ID_STNUM_JUMP,					//stNum����
	GOOSE_NODE_EVENT_ID_STNUM_REVERSE,				//stNum��ת
	GOOSE_NODE_EVENT_ID_SQNUM_UNRESET,				//Goose���¼�sqNumδ����
	GOOSE_NODE_EVENT_ID_DOUBLE_GOOSE,				//˫GOOSE
	GOOSE_NODE_EVENT_ID_REPEATED_GOOSE,				//�ظ�GOOSE
	GOOSE_NODE_EVENT_ID_STNUM_CHANGE_FAKE,			//stNum��ٱ�λ
	GOOSE_NODE_EVENT_ID_DELAY,						//�ӳ�
	GOOSE_NODE_EVENT_ID_DISCONNECT,					//��·�ж�
	GOOSE_NODE_EVENT_ID_TAL_ERROR,					//TALΪ0
	GOOSE_NODE_EVENT_ID_APPID_ZERO,					//APPIDΪ0
	GOOSE_NODE_EVENT_ID_NDSCOM_TRUE,				//ndscomΪtrue
	GOOSE_NODE_EVENT_ID_T_ERROR,					//T0~T3����
	GOOSE_NODE_EVENT_ID_DANUM_DIFFER_CFG,			//ʵ�����ݸ�����datset�ĳ�Ա����һ��
	GOOSE_NODE_EVENT_ID_GOCBREF_DIFFER_CFG,			//gocbRef�����ò�һ��
	GOOSE_NODE_EVENT_ID_GOID_DIFFER_CFG,			//goID�����ò�һ��
	GOOSE_NODE_EVENT_ID_DS_DIFFER_CFG,				//datset�����ò�һ��
	GOOSE_NODE_EVENT_ID_CONFREV_DIFFER_CFG,			//confrev�����ò�һ��

	//numDatSetEntries����datset�ĳ�Ա����һ��
	GOOSE_NODE_EVENT_ID_NUMDATSETENTRIES_DIFFER_CFG,
	
	GOOSE_NODE_EVENT_ID_DESTMAC_ERROR,				//Ŀ��MAC��ַ����
	GOOSE_NODE_EVENT_ID_RESTART,					//��·����
	GOOSE_NODE_EVENT_ID_CONNECT,					//��·�ָ�����
	GOOSE_NODE_EVENT_ID_STNUM_CHANGE_RIGHT,			//״̬��λ

	GOOSE_NODE_EVENT_ID_TEST_TRUE,					//GOOSE��������ģʽ
	GOOSE_NODE_EVENT_ID_TEST_FALSE,					//GOOSE���ڲ���ģʽ


	GOOSE_NODE_EVENT_ID_PORMAT_ERR,					//GOOSE��ʽ����
	GOOSE_NODE_EVENT_ID_APPID_CFG,					//��·δ����
	

	GOOSE_NODE_EVENT_ID_NET_STORM =100,				//����籩
	
	GOOSE_NODE_EVENT_ID_NODE_OVER_FLOWUP = 101,			//�ڵ�Խ����
	GOOSE_NODE_EVENT_ID_NODE_OVER_FLOWDOWN = 102,		//�ڵ�Խ����
	GOOSE_NODE_EVENT_ID_NODE_OVER_FLOW_COMEBACK = 103	//�ڵ�Խ�޻ָ�


};

enum SMV_YK_ID
{
	SMV_YK_ID_GET_ORIGINAL = 1,							//��ȡԭʼ����
	SMV_YK_ID_GET_EFFECTIVE_VAL = 2,					//��ȡ����������� 
	SMV_YK_ID_GET_INSTANTANEOUS_VAL = 101,				//��ȡָ��ͨ��YC��������ֵ_���ڻ���ʵʱ����
	SMV_YK_ID_GET_MULTI_SMV_VAL = 102,					//��ȡͬһʱ�̶���ϲ���Ԫ���������ݣ�˲ʱֵ����Чֵ����ǣ�
	SMV_YK_ID_GET_DISPERSION	= 103					//��ȡָ��YC����ɢ��
};

enum GOOSE_YK_ID
{
	GOOSE_YK_ID_GET_ORIGINAL = 1,						//��ȡԭʼ����
	GOOSE_YK_ID_GET_EFFECTIVE_VAL = 2					//��ȡ����������� 
};


#define MAX_MAC_LEN 17+1
#define MAX_ARV_TIME_LEN 32+1

/**********************************add on 2013/03/28 ���Ľ������**************************/
//Bit31:pakcet_len�쳣=0, Bit30:msg_len�쳣=0 , Bit15:nofasdu Bit14:sv_id Bit13:smpCnt Bit12:confRev Bit11:SmpSynch��Ϊ1 Bit7:sequence of data
#define DISSECT_SMV_RESULT_OK				0x0000F880	

#define DISSECT_SMV_PKTLEN_ERROR			0x80000000
#define DISSECT_SMV_MSGLEN_ERROR			0x40000000
#define DISSECT_SMV_ASDU_NUM_ZERO			0x20000000
//#define DISSECT_ETH_DROP			        0x10000000
//#define DISSECT_SMV_ETH_DROP			    DISSECT_ETH_DROP //0x10000000

#define DISSECT_SMV_NOFASDU_OK				0x00008000
#define DISSECT_SMV_SVID_OK					0x00004000
#define DISSECT_SMV_SMPCNT_OK				0x00002000
#define DISSECT_SMV_CONFREV_OK				0x00001000
#define DISSECT_SMV_SMPSYNCH_OK				0x00000800

#define DISSECT_SMV_DATASET_OK				0x00000400
#define DISSECT_SMV_REFRTM_OK				0x00000200
#define DISSECT_SMV_SMPRATE_OK				0x00000100
#define DISSECT_SMV_SEQUENCEOFDATA_OK		0x00000080
#define DISSECT_SMV_INVALID_FIELD_VAL		0x00000040

	
//Bit31:packet_len�쳣=0��Bit30:msg_len�쳣=0
//Bit23:PDUͷ��tag��ȷ=1��Bit22:gocbRef, Bit21:timeAllowed Bit20:dataSet Bit19:T Bit18:stNum Bit17:sqNum Bit16:test Bit15:confRev Bit14:ndsCom Bit13:numDataSetEntries��Ϊ1
#define DISSECT_GOOSE_RESULT_OK				0x00FFE000

#define DISSECT_GOOSE_PKTLEN_ERROR			0x80000000
#define DISSECT_GOOSE_MSGLEN_ERROR			0x40000000
//#define DISSECT_GOOSE_ETH_DROP             DISSECT_ETH_DROP			//0x10000000

#define DISSECT_GOOSE_PDU_TAG_OK			0x00800000
#define DISSECT_GOOSE_GOCB_OK				0x00400000
#define DISSECT_GOOSE_TAL_OK				0x00200000
#define DISSECT_GOOSE_DATASET_OK			0x00100000
#define DISSECT_GOOSE_T_OK					0x00080000
#define DISSECT_GOOSE_STNUM_OK				0x00040000
#define DISSECT_GOOSE_SQNUM_OK				0x00020000
#define DISSECT_GOOSE_TEST_OK				0x00010000
#define DISSECT_GOOSE_CONFREV_OK			0x00008000
#define DISSECT_GOOSE_NDSCOM_OK				0x00004000
#define DISSECT_GOOSE_NUMDATASETENTRIES_OK	0x00002000

#define DISSECT_GOOSE_ALL_DATA_TAG_OK		0x00001000
#define DISSECT_GOOSE_GOID_OK				0x00000800
//#define DISSECT_GOOSE_INVALID_FIELD_VAL		0x00000040
/*****************************************************************************************/

/********************************PTP�¼�����**********************************************///add on 2013/03/31
enum
{
	EVENT_ID_PTP_DISCONNECT			= 10000,//PTP��·�ж�
	EVENT_ID_PTP_CONNECT			= 10001,//PTP��·�ָ�
	EVENT_ID_PTP_SYNC_DELAY			= 10002,//PTPͬ�����ĳ�ʱ
	EVENT_ID_PTP_DELAY_RESP_DELAY	= 10003,//PTP��delay������Ӧ��ʱ
	EVENT_ID_PTP_PDELAY_RESP_DELAY	= 10004,//PTP��pdelay������Ӧ��ʱ
	EVENT_ID_PTP_CF_OVERLMT			= 10005,//PTP����CF���Խ��
	EVENT_ID_PTP_SYNC_DISC			= 10006,//PTPͬ�������ж�
	EVENT_ID_PTP_DELAY_RESP_DISC	= 10007,//PTP��delay������Ӧ�ж�
	EVENT_ID_PTP_PDELAY_RESP_DISC	= 10008//PTP��pdelay������Ӧ�ж�
};


#endif //CONST_SCAN_H_
