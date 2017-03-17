#ifndef PACKET_CAP_H_I_INCLUDE
#define PACKET_CAP_H_I_INCLUDE

#include "share_scan.h"
#include "SYS_MODEL_STRUCT.h"

#define MAX_ADAPTER_MAC_LEN 10
#define MAX_ADAPTER_NAME_LEN 10

typedef struct _PACKET_CAP_CFG_STRUCT
{
	LOG_STRUCT logInfo;

	int nPortNum;
	SNIFFER_PORT **pPortInfo;

	int nMaxPartitionUsg;

	unsigned long lMaxPacketFileLen;

	unsigned int nMaxPktLen_goose;//GOOSE��֡���ĵ���󳤶�
	unsigned int nMaxPktLen_smv;//SMV��֡���ĵ���󳤶�
	unsigned int nMaxPktLen_other;//�������ģ�����MMS,��֡���ĵ���󳤶�
	unsigned long lMaxSMFileSize_goose;//goose�����ڴ��ļ���С
	unsigned long lMaxSMFileSize_smv;//smv�����ڴ��ļ���С
	unsigned long lMaxSMFileSize_other;//�������Ĺ����ڴ��ļ���С

	int nAdapterNum;

#ifdef OS_WINDOWS
	unsigned char **pAdapter_mac;//��������Mac
	unsigned long lHwFilterType;
	unsigned long lKernelBuffer;//�ں˻���������λByte
	unsigned long lMinToCopy;//copy����������λByte
	unsigned long lUsrBuffer;//��λByte

#else
	char **pAdapter;//������������
	unsigned int nSnapLen;//��֡������󳤶ȣ��ֽ�
	unsigned int nWait_for_packet;
	unsigned int nWater_mark;//pool ring��ÿ��slot�Ĵ�С
	unsigned int nClusterNum;//ring��cluster����

#endif

	bool bNeedZip;//��goose�����Ƿ���Ҫѹ����Ĭ��Ϊture����Ҫѹ��
	int nZipLevel;//��goose�����Ƿ���Ҫѹ����Ĭ��Ϊture����Ҫѹ��
	bool bBindCpu;//����CPUʱ���Ƿ�󶨽��ձ����̵߳�CPU1��ѹ���̵߳�CPU2��Ĭ��true
	int nsave_by_node;//���İ��ڵ�洢��ʽ��1���˿ڴ洢��2�����洢
	int nsave_by_protocol;//���İ�����Э�����ʹ洢��ʽ��1 GOOSE���ĵ����洢��2��ϴ洢
	SNIFFER_CARD** pSniffer_card;
	int ncardnum;
	bool bFrameHasLinkFcs;
	_PACKET_CAP_CFG_STRUCT()
	{
		nPortNum = 0;
		pPortInfo = NULL;

		nMaxPartitionUsg = 0;
		lMaxPacketFileLen = 0;

		nMaxPktLen_goose = 0;
		nMaxPktLen_smv = 0;
		nMaxPktLen_other = 0;
		lMaxSMFileSize_goose = 0;
		lMaxSMFileSize_smv = 0;
		lMaxSMFileSize_other = 0;

		nAdapterNum = 0;

#ifdef OS_WINDOWS
		pAdapter_mac = NULL;
		lHwFilterType = 0x00000020;
		lKernelBuffer = 64;
		lMinToCopy = 256;
		lUsrBuffer = 955*1024;//�����٣�
#else
		pAdapter = NULL;
		nSnapLen = 5000;
		nWait_for_packet = 1;
		nWater_mark = 128;//poll ring��ÿ��slot�Ĵ�С
		nClusterNum = 8;//ring��cluster����
#endif

		bNeedZip = true;//Ĭ��Ϊture����Ҫѹ��
		bBindCpu = true;
		nsave_by_node = 1;
		nsave_by_protocol = 1;
		pSniffer_card = NULL;
		ncardnum = 0;
		bFrameHasLinkFcs=false;
		nZipLevel=6;
	}

}PACKET_CAP_CFG_STRUCT;

#ifdef OS_WINDOWS

#ifdef DLL_FILE_PACKETCAP

//������ 

class _declspec(dllexport) CPacketCap_I //������ 

#else 

class _declspec(dllimport) CPacketCap_I //������ 

#endif//DLL_FILE_PACKETCAP

#endif//OS_WINDOWS


#ifdef OS_LINUX

	class CPacketCap_I

#endif

	{
		public:
			CPacketCap_I(){};

		public:
			virtual int packetcap_begin()=0;
			virtual int packetcap_end()=0;
	protected:
	private:
	};


#ifdef OS_WINDOWS

#ifdef DLL_FILE_PACKETCAP 

	//��������

	extern "C"  __declspec( dllexport ) CPacketCap_I * createPacketCapI(_PACKET_CAP_CFG_STRUCT *pStruct);
	extern "C"  __declspec( dllexport ) void destroyPacketCapI(CPacketCap_I * theInterface);

#else 

	//���뺯��

	extern "C"  __declspec( dllimport ) CPacketCap_I * createPacketCapI(_PACKET_CAP_CFG_STRUCT *pStruct);
	extern "C"  __declspec( dllimport ) void destroyPacketCapI(CPacketCap_I * theInterface);

#endif //DLL_FILE_PACKETCAP
#endif //OS_WINDOWS

#ifdef OS_LINUX
#ifdef __cplusplus
	extern "C"
	{
#endif

		CPacketCap_I * createPacketCapI(_PACKET_CAP_CFG_STRUCT *pStruct);
		void destroyPacketCapI(CPacketCap_I * theInterface);

#ifdef __cplusplus
	}
#endif

#endif //OS_LINUX



#endif//PACKET_CAP_H_I_INCLUDE