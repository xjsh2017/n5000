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
	//��־�ļ�
	CLogFile *pLogFile;

	ACSI_IED_CTRL *pIED_ctrls;

	//�ڵ���Ϣ
	SNIFFER_APP sniffer_app_info;

	//ң��ͨ������
	int n_yx_channel_num;
	//ң��ͨ������
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

	/**********************�оݿ�������***********************************/
	unsigned int nerr_switch_setting;

	//PTP�ж�ʱ��-��ֵ(s)" name="">11</cfgitem>
	unsigned int ao_ms_ptp_discon;                                    
	//PTPͬ����ʱʱ��-��ֵs" name="">2</cfgitem>
	unsigned int ao_ms_ptp_syncdelay;                                 
	//delay_resp(ʱ������)��ʱʱ��-��ֵ(s)" name="">2</cfgitem>
	unsigned int ao_ms_ptp_delayresp_delay;                           
	//pdelay_resp(ʱ������two-step)��ʱʱ��-��ֵ(s)" name="">2</cfgitem>
	unsigned int ao_ms_ptp_pdelayresp_delay;                          
	//CF�򳬲�-��ֵ(ns)" "name=">400000000</cfgitem>
	unsigned int ao_ns_ptp_cf; 
	//PTPͬ�������ж�ʱ��-��ֵ(s)" name="">10</cfgitem>
	unsigned int ao_ms_ptp_syn_disc;                                  
	//PTP��delay������Ӧ�ж�ʱ��-��ֵ(s)" name="">10</cfgitem>
	unsigned int ao_ms_ptp_delayresp_disc;                            
	//TP��pdelay������Ӧ�ж�ʱ��-��ֵ��ʱ��-��ֵ(s)" name="">10</cfgitem>
	unsigned int ao_ms_ptp_pdelayresp_disc;

	_PTP_SETTING_STRUCT()
	{
		//PTP�ж�ʱ��-��ֵ(s)" name="">11</cfgitem>
		ao_ms_ptp_discon=11;                                    
		//PTPͬ����ʱʱ��-��ֵs" name="">2</cfgitem>
		ao_ms_ptp_syncdelay=2;                                 
		//delay_resp(ʱ������)��ʱʱ��-��ֵ(s)" name="">2</cfgitem>
		ao_ms_ptp_delayresp_delay=2;                           
		//pdelay_resp(ʱ������two-step)��ʱʱ��-��ֵ(s)" name="">2</cfgitem>
		ao_ms_ptp_pdelayresp_delay=2;                          
		//CF�򳬲�-��ֵ(ns)" "name=">400000000</cfgitem>
		ao_ns_ptp_cf=400000000; 
		//PTPͬ�������ж�ʱ��-��ֵ(s)" name="">10</cfgitem>
		ao_ms_ptp_syn_disc=10;                                  
		//PTP��delay������Ӧ�ж�ʱ��-��ֵ(s)" name="">10</cfgitem>
		ao_ms_ptp_delayresp_disc=10;                            
		//TP��pdelay������Ӧ�ж�ʱ��-��ֵ��ʱ��-��ֵ(s)" name="">10</cfgitem>
		ao_ms_ptp_pdelayresp_disc=10;                           

		nerr_switch_setting = 0XFFFFFFFF;
	}

}PTP_SETTING_STRUCT;

#ifdef OS_WINDOWS

#ifdef DLL_FILE_PTP 

class /*_declspec(dllexport)*/ CAnalyzer_I //������ 

#else 

class /*_declspec(dllimport)*/ CAnalyzer_I //������ 

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
	//�����﷨�ṹ����
	virtual int parse(const PACKET_STRUCT *p_packet,int n_num, void **p_ptp_info)=0;
	virtual void release_info(void *p_ptp_info)=0;
	//���߷������������������ṹ��PTP_INFO_STRUCT
	virtual int anlalyze(const void *v_ptpinfo, struct timeval *ptimeval, char *cfile_name)=0;
	//�����˱������Ƶ�
	virtual MESSAGE_ERROR_INFO_ARRAY_STRUCT* analyze_online(void *p_infos, int n_num)=0;
	//�����ǿ��Ǳ�������
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
	char m_csrcmac[18];//��ʱ���ԴMAC
	char m_cdstmac[18];//��ʱ���Ŀ��MAC
	char m_cfile_name[256];//��ǰPTP����ʱ�����и澯���澯ʱ���ɵ��ļ���
	int m_nsysid;//PTP�ڵ�ϵͳ���
	int m_nPort;//port sys-id
//private:
//	CFile_mngr m_file_mngr;
};


#ifdef OS_WINDOWS

#ifdef DLL_FILE_ANALYZER 

//��������

extern "C"  __declspec( dllexport ) CAnalyzer_I * createAnalyzer_I();
extern "C"  __declspec( dllexport ) void destroyAnalyzer_I(CAnalyzer_I* theInterface);

#else 

//���뺯��

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