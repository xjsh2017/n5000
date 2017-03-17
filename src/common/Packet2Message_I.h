#ifndef PACKET_2_MESSAGE_H_I
#define PACKET_2_MESSAGE_H_I

#include "share_scan.h"
#include "AT_STRUCT.h"
#include "ShareMemFileCtrl_I.h"
#include "ptp_struct_def.h"

#ifdef OS_LINUX
#define __int64 long long
#endif

typedef struct _PKTCONVERT_CFG_STRUCT
{
	LOG_STRUCT logInfo;

	bool bParseSmvDA;
	bool bParseGooseDA;


	_PKTCONVERT_CFG_STRUCT()
	{
		bParseSmvDA = false;
		bParseGooseDA = false;

	}

}PKTCONVERT_CFG_STRUCT;

#ifdef OS_WINDOWS

#ifdef DLL_FILE_PACKETCONVERT

//导出类 

class _declspec(dllexport) CPacketConvert_I //导出类 

#else 

class _declspec(dllimport) CPacketConvert_I //导入类 

#endif//DLL_FILE_PACKETCONVERT

#endif//OS_WINDOWS


#ifdef OS_LINUX

class CPacketConvert_I

#endif

{
protected:
	CPacketConvert_I(){};
	
public:
	virtual int packetconvert_init(PKTCONVERT_CFG_STRUCT CfgInfo)=0;

	//外部调用时，p_msg数组资源已分配，但p_msg的SMV_DATA_STRUCT数组资源是由本函数分配
	//virtual int packetconvert_2smv(PACKET_CHAR_STRUCT* p_packet,int n_num, SMV_INFO_STRUCT *p_msg)=0;

	//virtual int packetconvert_2goose(PACKET_CHAR_STRUCT* p_packet,int n_num, GOOSE_INFO_STRUCT* p_msg)=0;

	//virtual int packetconvert_2ptp(PACKET_CHAR_STRUCT* p_packet,int n_num, PTP_INFO_STRUCT* p_msg)=0;

	virtual int packetconvert_get_appid(const char *p_Buf)=0;

	//virtual int packetconvert_get_date_time(char *p_packet_buf, char *c_data_time)=0;//年月日时分秒.毫秒

	virtual int packetconvert_get_utctime(char *p_packet_buf, char *c_utctime)=0;

	virtual ETHER_TYPE packetconvert_get_ether_type(char *p_packet_buf)=0;

	virtual int packetconvert_release_goose_mms_data(GOOSE_INFO_STRUCT* pGoose, int n_num)=0;

	virtual int packetconvert_release_mms_data(MMS_DATA_STRUCT* p_mms_data, int n_num)=0;
	
	virtual int packetconvert_release_smv_asdu(SMV_INFO_STRUCT* pSmv, int n_num)=0;

	//virtual int packetconvert_release_asdu_info(ASDU_INFO_STRUCT* p_asdu_info, int n_num)=0;

	///virtual int packetconvert_get_dstip(char *p_packet_buf, char *pIP)=0;

	virtual int packetconvert_get_srctip(char *p_packet_buf, char *pIP)=0;

	virtual int packetconvert_get_dest_mac(char *p_packet_buf, char *p_mac)=0;

	virtual int packetconvert_get_src_mac(char *p_packet_buf, char *p_mac)=0;

	virtual int packetconvert_2goose_ex(PACKET_STRUCT* p_packet,int n_num, GOOSE_INFO_STRUCT* p_msg)=0;

	//virtual int get_correction_val(CORRECTION_FIELD correctionField, double &d_correction)=0;

	//virtual int get_timestamp_s_ns(PTP_TIME_STAMP timestamp, __int64 &time_s, UINT &time_ns)=0;

	//virtual __int64 reverse_6byte(UINT nuint16_val, u_short nshort_val)=0;

	//性能优化扩展,p_msg的资源及其SMV_DATA_STRUCT数组资源，外部调用时全部已分配
	virtual int packetconvert_2smv_ex2(PACKET_STRUCT* p_packet,int n_num, SMV_INFO_STRUCT *p_msg)=0;
	virtual int packetconvert_2smv_ex(PACKET_STRUCT* p_packet,int n_num, SMV_INFO_STRUCT *p_msg)=0;
	virtual int packetconvert_2ptp_ex(PACKET_STRUCT* p_packet,int n_num, PTP_INFO_STRUCT* p_msg)=0;
	virtual void packetconvert_set_linkType(int nLinkType)=0;
	virtual int  packetconvert_get_linkType()=0;

};


#ifdef OS_WINDOWS

#ifdef DLL_FILE_PACKETCONVERT 

//导出函数

extern "C"  __declspec( dllexport ) CPacketConvert_I * createPktConvertI();
extern "C"  __declspec( dllexport ) void destroyPktConvertI(CPacketConvert_I * theInterface);

#else 

//导入函数

extern "C"  __declspec( dllimport ) CPacketConvert_I * createPktConvertI();
extern "C"  __declspec( dllimport ) void destroyPktConvertI(CPacketConvert_I * theInterface);

#endif //DLL_FILE_PACKETCONVERT
#endif //OS_WINDOWS

#ifdef OS_LINUX
#ifdef __cplusplus
extern "C"
{
#endif
	
	CPacketConvert_I * createPktConvertI();
	void destroyPktConvertI(CPacketConvert_I * theInterface);
	
#ifdef __cplusplus
}
#endif

#endif //OS_LINUX


#endif /*PACKET_2_MESSAGE_H_I*/