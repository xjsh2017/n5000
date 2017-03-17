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

	unsigned int nMaxPktLen_goose;//GOOSE单帧报文的最大长度
	unsigned int nMaxPktLen_smv;//SMV单帧报文的最大长度
	unsigned int nMaxPktLen_other;//其它报文，包括MMS,单帧报文的最大长度
	unsigned long lMaxSMFileSize_goose;//goose共享内存文件大小
	unsigned long lMaxSMFileSize_smv;//smv共享内存文件大小
	unsigned long lMaxSMFileSize_other;//其它报文共享内存文件大小

	int nAdapterNum;

#ifdef OS_WINDOWS
	unsigned char **pAdapter_mac;//监视网卡Mac
	unsigned long lHwFilterType;
	unsigned long lKernelBuffer;//内核缓冲区，单位Byte
	unsigned long lMinToCopy;//copy缓冲区，单位Byte
	unsigned long lUsrBuffer;//单位Byte

#else
	char **pAdapter;//监视网卡名称
	unsigned int nSnapLen;//单帧报文最大长度，字节
	unsigned int nWait_for_packet;
	unsigned int nWater_mark;//pool ring中每个slot的大小
	unsigned int nClusterNum;//ring中cluster个数

#endif

	bool bNeedZip;//非goose报文是否需要压缩，默认为ture，需要压缩
	int nZipLevel;//非goose报文是否需要压缩，默认为ture，需要压缩
	bool bBindCpu;//当多CPU时，是否绑定接收报文线程到CPU1，压缩线程到CPU2，默认true
	int nsave_by_node;//报文按节点存储方式，1按端口存储，2按卡存储
	int nsave_by_protocol;//报文按报文协议类型存储方式，1 GOOSE报文单独存储，2混合存储
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
		lUsrBuffer = 955*1024;//最大多少？
#else
		pAdapter = NULL;
		nSnapLen = 5000;
		nWait_for_packet = 1;
		nWater_mark = 128;//poll ring中每个slot的大小
		nClusterNum = 8;//ring中cluster个数
#endif

		bNeedZip = true;//默认为ture，需要压缩
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

//导出类 

class _declspec(dllexport) CPacketCap_I //导出类 

#else 

class _declspec(dllimport) CPacketCap_I //导入类 

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

	//导出函数

	extern "C"  __declspec( dllexport ) CPacketCap_I * createPacketCapI(_PACKET_CAP_CFG_STRUCT *pStruct);
	extern "C"  __declspec( dllexport ) void destroyPacketCapI(CPacketCap_I * theInterface);

#else 

	//导入函数

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