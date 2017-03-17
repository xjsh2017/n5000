#ifndef ANALYZE_H_I
#define ANALYZE_H_I

#include "LogFile.h"
#include "acsistruct.h"
#include "AnalyzeOnline_STRUCT.h"
#include "share_scan.h"
#include "SYS_MODEL_STRUCT.h"
#include "Packet2Message_I.h"
#include "AnalyzeOnline_Common.h"

typedef struct _PTP_CFG_STRUCT
{
	//日志文件
	CLogFile *pLogFile;

	ACSI_IED_CTRL *pIED_ctrls;

	//节点信息
	SNIFFER_APP sniffer_app_info;

	//遥信通道个数
	int n_yx_channel_num;
	//遥信通道队列
	CFG_REC_CHANNEL** pyx_channel_list;

	_PTP_CFG_STRUCT()
	{
		pLogFile = NULL;
		pIED_ctrls = NULL;
		n_yx_channel_num = 0;
		pyx_channel_list = NULL;
	}


}PTP_CFG_STRUCT;

typedef struct _PTP_SETTING_STRUCT
{

	/**********************判据开关配置***********************************/
	unsigned int nerr_switch_setting;

	//PTP中断时间-定值(s)" name="">11</cfgitem>
	unsigned int ao_ms_ptp_discon;                                    
	//PTP同步延时时间-定值s" name="">2</cfgitem>
	unsigned int ao_ms_ptp_syncdelay;                                 
	//delay_resp(时钟请求)延时时间-定值(s)" name="">2</cfgitem>
	unsigned int ao_ms_ptp_delayresp_delay;                           
	//pdelay_resp(时钟请求two-step)延时时间-定值(s)" name="">2</cfgitem>
	unsigned int ao_ms_ptp_pdelayresp_delay;                          
	//CF域超差-定值(ns)" "name=">400000000</cfgitem>
	unsigned int ao_ns_ptp_cf; 
	//PTP同步报文中断时间-定值(s)" name="">10</cfgitem>
	unsigned int ao_ms_ptp_syn_disc;                                  
	//PTP的delay请求响应中断时间-定值(s)" name="">10</cfgitem>
	unsigned int ao_ms_ptp_delayresp_disc;                            
	//TP的pdelay请求响应中断时间-定值断时间-定值(s)" name="">10</cfgitem>
	unsigned int ao_ms_ptp_pdelayresp_disc;

	_PTP_SETTING_STRUCT()
	{
		//PTP中断时间-定值(s)" name="">11</cfgitem>
		ao_ms_ptp_discon=11;                                    
		//PTP同步延时时间-定值s" name="">2</cfgitem>
		ao_ms_ptp_syncdelay=2;                                 
		//delay_resp(时钟请求)延时时间-定值(s)" name="">2</cfgitem>
		ao_ms_ptp_delayresp_delay=2;                           
		//pdelay_resp(时钟请求two-step)延时时间-定值(s)" name="">2</cfgitem>
		ao_ms_ptp_pdelayresp_delay=2;                          
		//CF域超差-定值(ns)" "name=">400000000</cfgitem>
		ao_ns_ptp_cf=400000000; 
		//PTP同步报文中断时间-定值(s)" name="">10</cfgitem>
		ao_ms_ptp_syn_disc=10;                                  
		//PTP的delay请求响应中断时间-定值(s)" name="">10</cfgitem>
		ao_ms_ptp_delayresp_disc=10;                            
		//TP的pdelay请求响应中断时间-定值断时间-定值(s)" name="">10</cfgitem>
		ao_ms_ptp_pdelayresp_disc=10;                           

		nerr_switch_setting = 0XFFFFFFFF;
	}

}PTP_SETTING_STRUCT;

#ifdef OS_WINDOWS

#ifdef DLL_FILE_PTP 

class /*_declspec(dllexport)*/ CAnalyzer_I //导出类 

#else 

class /*_declspec(dllimport)*/ CAnalyzer_I //导入类 

#endif//DLL_FILE_PTP

#endif//OS_WINDOWS

#ifdef OS_LINUX

class CAnalyzer_I

#endif //OS_LINUX

{
protected:
	CAnalyzer_I(){};
	
public:
	virtual int analyze_init(void *pCfgInfo)=0;
	//报文语法结构解析
	virtual int parse(const PACKET_STRUCT *p_packet,int n_num, void **p_ptp_info)=0;
	virtual void release_info(void *p_ptp_info)=0;
	//在线分析，单个初步解析结构体PTP_INFO_STRUCT
	virtual int anlalyze(const void *v_ptpinfo, struct timeval *ptimeval, char *cfile_name)=0;
	//考虑了闭锁机制的
	virtual MESSAGE_ERROR_INFO_ARRAY_STRUCT* analyze_online(void *p_infos, int n_num)=0;
	//不考虑考虑闭锁机制
	virtual MESSAGE_ERROR_INFO_ARRAY_STRUCT* analyze_offline(void *p_infos)=0;

	virtual int special_pkt_discon_analyze(struct timeval *ptimeval){return 0;}

	virtual int analyze_releasemsgerr(MESSAGE_ERROR_INFO_ARRAY_STRUCT* p_message_errs)=0;

	virtual int analyze_end()=0;
	//virtual int GetPTP_MSG_TIME(PTP_INFO_STRUCT* p_ptp_info_struct,__int64 &time_s, UINT &time_ns)=0;

	virtual int I_SET_SNIFFER_APP(SNIFFER_APP* pSniffApp)=0;
	virtual int I_GET_STRING_INFO(void* pInfo,int infoType,void *strRet)=0;
public:
	virtual int reset_alarmlock()=0;
	//virtual int reset_gserr()=0;
	//virtual void set_pPktconvert(CPacketConvert_I *g_pPktconvert)=0; 

public:
	virtual int InitTime(struct timeval *ptimeval)=0;
	virtual int analze_set_setting(void* p_setiing)=0;
	virtual int disconnect_analyze(struct timeval *ptimeval)=0;
	virtual int reset_all(struct timeval *ptimeval)=0;
	virtual int setAlarmLock(bool blarmLock)
	{
		return 0;
	}
	virtual void ana_msg_reset()
	{
		 zero_message_err(m_message_err);
	}
	virtual MESSAGE_ERROR_INFO_ARRAY_STRUCT *make_ana_info()=0;
	//virtual int set_err_packet_filename_by_time(char *cTime)
	//{
	//	int nRet = RES_SUCCESS;

	//	if (NULL==cTime)
	//	{
	//		return RES_FAIL;
	//	}

	//	m_file_mngr.set_filename(cTime,m_nPort );
	//	m_file_mngr.set_setFilename_flag(true);

	//	return nRet;
	//}
public:
	MESSAGE_ERROR_STRUCT m_message_err;
public:
	char m_csrcmac[18];//临时存放源MAC
	char m_cdstmac[18];//临时存放目标MAC
	char m_cfile_name[256];//当前PTP分析时，如有告警，告警时生成的文件名
	int m_nsysid;//PTP节点系统编号
	int m_nPort;//port sys-id
//private:
//	CFile_mngr m_file_mngr;
};


#ifdef OS_WINDOWS

#ifdef DLL_FILE_ANALYZER 

//导出函数

extern "C"  __declspec( dllexport ) CAnalyzer_I * createAnalyzer_I();
extern "C"  __declspec( dllexport ) void destroyAnalyzer_I(CAnalyzer_I* theInterface);

#else 

//导入函数

extern "C"  __declspec( dllimport ) CAnalyzer_I * createAnalyzer_I();
extern "C"  __declspec( dllimport ) void destroyAnalyzer_I(CAnalyzer_I* theInterface);

#endif //DLL_FILE_PTP
#endif //OS_WINDOWS

#ifdef OS_LINUX
#ifdef __cplusplus
extern "C"
{
#endif
	CAnalyzer_I * createAnalyzer_I();
	void destroyAnalyzer_I(CAnalyzer_I* theInterface);
	
#ifdef __cplusplus
}
#endif

#endif //OS_LINUX

#endif /*PTP_ANALYZE_H_I*/