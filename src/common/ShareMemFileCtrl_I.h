#ifndef SMFC_I_H_HEADER_INCLUDED_B3305D3B
#define SMFC_I_H_HEADER_INCLUDED_B3305D3B

#include "define_scan.h"
#include "const_scan.h"
#include "share_scan.h"
#include "SYS_MODEL_STRUCT.h"

#ifdef OS_LINUX
#define __int64 long long 
#endif

/*
enum ETHER_TYPE
{
	ETHER_TYPE_MMS =1,
	ETHER_TYPE_GOOSE,
	ETHER_TYPE_SMV,
		
	ETHER_TYPE_UNKOWN=9999
};	*/

typedef struct PACKET_STRUCT{

	//��֡����������ʼ��ַ
	void *pPacket;
	//��֡���ĳ��ȣ�������֡Ƭ�Σ�����·���쳣ʱ������β�����ӵ�8�ֽ���Ϣ
	int nLen;

	unsigned int nAppid;

	ETHER_TYPE nEthType;

	PACKET_STRUCT()
	{
		pPacket = 0;
		nLen = 0;
		nAppid = 0;
		nEthType = IEC61850_ETHER_TYPE_UNKOWN;
	}

}PACKET_STRUCT;

//����ͳ��
typedef struct PACKET_STATISTICS_STRUCT
{
	int nStartTime;
//	unsigned long nPacketNum;
	__int64 nPacketNum;//modified on 2013/03/28 ljm ����֡����ת//�����жϻ�����д��λ�ã���֡��ͳ�ƣ���ǰ����֡Ƭ�α���Ϊ��һ֡���ݲ���������һ���������⴦����֡Ƭ�Ρ�
	unsigned long nTotalSize_MB;
	unsigned long nTotalSize_BYTE;
	//time_t����
	unsigned long	lSec;		// timestamp seconds 
	//������
	unsigned long 	lUsec;	// timestamp microseconds (nsecs for PCAP_NSEC_MAGIC) 
	
	PACKET_STATISTICS_STRUCT(){
		nStartTime=0;
		nPacketNum=0;
		nTotalSize_MB=0;
		nTotalSize_BYTE=0;
		lSec = 0;
		lUsec = 0;
	}

}_PACKET_STATISTICS_STRUCT;

//������·������ͳ��
typedef struct LINK_STATISTICS_STRUCT
{
	PACKET_STATISTICS_STRUCT statLink;

	unsigned int nAppid;

	LINK_STATISTICS_STRUCT()
	{
		nAppid = 0;
	}

}_LINK_STATISTICS_STRUCT;

//�˿�����ͳ��
typedef struct PORT_STATISTICS_STRUCT{

	PACKET_STATISTICS_STRUCT statGoose;

	PACKET_STATISTICS_STRUCT statOther;

	LINK_STATISTICS_STRUCT *pStatLinkArray;//����ֵ����ͳ��

	int nAppidNum;//SMV����·��

	PORT_STATISTICS_STRUCT()
	{
		pStatLinkArray = NULL;
		nAppidNum = 0;
	}


}_PORT_STATISTICS_STRUCT;

/*
typedef struct _PORT_INFO_STRUCT
{
	int nPort;
	
	int *pAppid;

	int nAppidNum;

	_PORT_INFO_STRUCT()
	{
		pAppid = NULL;
	}

}PORT_INFO_STRUCT;*/

typedef struct _SMFC_CFG_STRUCT
{
/*	char cLogPath[255];

	int nLogLevel;

	int nLogSize;*/

	LOG_STRUCT logInfo;

	unsigned int nMaxPktLen_goose;
	unsigned int nMaxPktLen_smv;
	unsigned int nMaxPktLen_other;
	unsigned long lMaxSMFileSize_goose;//goose�����ڴ��ļ���С
	unsigned long lMaxSMFileSize_smv;//smv�����ڴ��ļ���С
	unsigned long lMaxSMFileSize_other;//�������Ĺ����ڴ��ļ���С

	SNIFFER_PORT **pPortInfo;

	int nPortNum;

	_SMFC_CFG_STRUCT()
	{
		pPortInfo = NULL;
		nMaxPktLen_goose=0;
		nMaxPktLen_smv=0;
		nMaxPktLen_other=0;
		lMaxSMFileSize_goose = 0;
		lMaxSMFileSize_smv = 0;
		lMaxSMFileSize_other = 0;
	}

}SMFC_CFG_STRUCT;


#ifdef OS_WINDOWS

#ifdef SMFC_DLL_FILE 

class _declspec(dllexport) CSMFC_I //������ 

#else 

class _declspec(dllimport) CSMFC_I //������ 

#endif//SMFC_DLL_FILE

#endif//OS_WINDOWS

#ifdef OS_LINUX

	class CSMFC_I

#endif //OS_LINUX

	{
		protected:
			CSMFC_I(){};

		public:
			
			virtual int smfc_write_packet(int nPort, PACKET_STRUCT *pStruct)=0;

			//�����
			virtual int smfc_read_packet(int nPort, PACKET_STRUCT *pStruct, int nPacketNum)=0;

			//��������
			virtual int smfc_read_step_packet(int nPort, PACKET_STRUCT *pStruct, int nPacketNum, int nStep)=0;

			//��ǰ��
			virtual int smfc_read_rpacket(int nPort, PACKET_STRUCT *pStruct, int nPacketNum)=0;

			//��ǰ�����
			virtual int smfc_read_rstep_packet(int nPort, PACKET_STRUCT *pStruct, int nPacketNum, int nStep)=0;

			virtual int smfc_read_statistics(int nPort, PORT_STATISTICS_STRUCT *pStruct)=0;

			virtual int smfc_release_file()=0;
			virtual int smfc_read_rpacket_ex(int nPort, PACKET_STRUCT *pStruct, int nPacketNum)=0;

			//��smfc_read_rpacket�ӿڵ���ʱ����¼��дָ����ǰ��nPacketNum֡�������Ѷ����ġ��˿ڲ�������ʱ������-1���ɹ�����0��ʧ�ܷ���1
			virtual int smfc_read_rpacket_from_lastwritepos(int nPort, PACKET_STRUCT *pStruct, int nPacketNum)=0;

			virtual int smfc_read_rstep_packet_from_lastwritepos(int nPort, PACKET_STRUCT *pStruct, int nPacketNum, int nStep)=0;

			//��ָ��SMV�ڵ��ͳ��ֵ
			virtual int smfc_read_smvlink_statistics(int nPort, LINK_STATISTICS_STRUCT* pStruct)=0;
			//��ָ��GOOSE�ڵ��ͳ��ֵ
			virtual int smfc_read_gooselink_statistics(int nPort, LINK_STATISTICS_STRUCT* pStruct)=0;
			//��ָ��PTP�ڵ��ͳ��ֵ
			virtual int smfc_read_ptp_statistics(int nPort, char *cMac, PACKET_STATISTICS_STRUCT* pStatic)=0;

			//д��Ա����(�ó�Ա�������ڼ�¼ͳ�ƹ����ڴ��е�֡����������ʱ����ʼλ��)
			virtual int smfc_set_member_lastpktnum(int nPort, int nEthType, unsigned int nAppid)=0;
			//���������ĵ�ͳ��ֵ������δ����smv����������
			virtual int smfc_read_other_statistics(int nPort, PACKET_STATISTICS_STRUCT *pstatistics)=0;
			//���㲥���ĵ�ͳ��ֵ
			virtual int smfc_read_broadcast_statistics(int nPort, PACKET_STATISTICS_STRUCT *pstatistics)=0;
			//��MMS���ĵ�ͳ��ֵ
			virtual int smfc_read_mms_statistics(int nPort, PACKET_STATISTICS_STRUCT *pstatistics)=0;

			//ɾ����¼δ����appid���ļ�
			virtual int smfc_delete_uncfg_appid_file()=0;

			//add on 2013/03/18 ljm ���Ӷ˿�������ͳ��
			virtual int smfc_read_port_whole_statistics(int nPort, PACKET_STATISTICS_STRUCT *pstatistics)=0;

			
	};


#ifdef OS_WINDOWS

#ifdef SMFC_DLL_FILE 

//��������

extern "C"  __declspec( dllexport ) CSMFC_I * createSMFC_I(SMFC_CFG_STRUCT* pSmfcCfg);
extern "C"  __declspec( dllexport ) void destroySMFC_I(CSMFC_I* theInterface);

#else 

//���뺯��

extern "C"  __declspec( dllimport ) CSMFC_I * createSMFC_I(SMFC_CFG_STRUCT* pSmfcCfg);
extern "C"  __declspec( dllimport ) void destroySMFC_I(CSMFC_I* theInterface);

#endif //DLL_FILE_PACKETCAP
#endif //OS_WINDOWS

#ifdef OS_LINUX
#ifdef __cplusplus
extern "C"
{
#endif
	
	CSMFC_I * createSMFC_I(SMFC_CFG_STRUCT* pSmfcCfg);
	void destroySMFC_I(CSMFC_I* theInterface);
	
#ifdef __cplusplus
}
#endif

#endif //OS_LINUX

#endif /* SMFC_I_H_HEADER_INCLUDED_B3305D3B */