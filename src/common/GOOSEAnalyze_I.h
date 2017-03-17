#ifndef GOOSE_ANALYZE_H_I
#define GOOSE_ANALYZE_H_I

#include "LogFile.h"
#include "acsistruct.h"
#include "AnalyzeOnline_STRUCT.h"
#include "share_scan.h"
#include "SYS_MODEL_STRUCT.h"

/*************************GOOSE分析定值**********************/
#define SETTING_GOOSE_SQNUM_JUMP				0X00000001
#define SETTING_GOOSE_SQNUM_REVERSE				0X00000002
#define SETTING_GOOSE_STNUM_JUMP				0X00000004		
#define SETTING_GOOSE_STNUM_REVERSE				0X00000008
#define SETTING_GOOSE_SQNUM_UNRESET				0X00000010
#define SETTING_GOOSE_GOOSE_DOUBLE				0X00000020
#define SETTING_GOOSE_GOOSE_REPEAT				0X00000040
#define SETTING_GOOSE_STNUMCHANGE_FAKE			0X00000080
#define SETTING_GOOSE_GOOSE_DELAY				0X00000100
#define SETTING_GOOSE_TAL_ZERO					0X00000200
#define SETTING_GOOSE_APPID_ZERO				0X00000400
#define SETTING_GOOSE_NDSCOMM_TRUE				0X00000800
#define SETTING_GOOSE_T0_T3_ERR					0X00001000
#define SETTING_GOOSE_DANUM_DIFFER_DSMEMBERNUM	0X00002000
#define SETTING_GOOSE_GOCBREF_DIFFER_CFG		0X00004000
#define SETTING_GOOSE_GOID_DIFFER_CFG			0X00008000
#define SETTING_GOOSE_DS_DIFFER_CFG				0X00010000
#define SETTING_GOOSE_CONFREV_DIFFER_CFG		0X00020000
#define SETTING_GOOSE_DAENTRIES_DIFFER_DANUM	0X00040000
#define SETTING_GOOSE_DESTMAC_ERR				0X00080000
#define SETTING_GOOSE_GOOSE_RESTART				0X00100000
#define SETTING_GOOSE_STNUMCHANGE_RIGHT			0X00200000
#define SETTING_GOOSE_TEST_TRUE					0X00400000
#define SETTING_GOOSE_FORMAT_ERR				0X00800000
#define SETTING_GOOSE_MAC_DIFFER_CFG			0X01000000
#define SETTING_GOOSE_DATYPE_DIFFER_CFG			0X02000000
#define SETTING_GOOSE_GOOSE_DROP				0X04000000
#define SETTING_GOOSE_GOOSE_THEMBLE				0X08000000
#define SETTING_GOOSE_RESERVE1					0X10000000
#define SETTING_GOOSE_RESERVE2					0X20000000
#define SETTING_GOOSE_RESERVE3					0X40000000
#define SETTING_GOOSE_RESERVE4					0X80000000

enum GS_ANALYZE_I_TYPE
{
	GS_ANALYZE_I_TYPE_ONLINE = 1, //在线分析
	GS_ANALYZE_I_TYPE_OFFLINE	//离线分析
};

typedef struct _GOOSE_CFG_STRUCT
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

	_GOOSE_CFG_STRUCT()
	{
		pLogFile = NULL;
		pIED_ctrls = NULL;
		n_yx_channel_num = 0;
		pyx_channel_list = NULL;
	}
	

}GOOSE_CFG_STRUCT;

typedef struct _GOOSE_SETTING_STRUCT
{
	/**********************判据阀值配置************************************/
	//T0的百分比阀值,帧间隔的绝对值 >=T0*ntremble_T0_minpercent 且 <=T0*ntremble_T0_minpercent 则判心跳报文抖动
	int ntremble_T0_minpercent;
	
	//T0的百分比阀值,帧间隔 >T0*ndelay_T0_minpercent 且 <=T0*ndrop_T0_minpercent 则判延迟
	int ndelay_T0_minpercent;
	
	//T0的百分比阀值,帧间隔>T0*ndrop_T0_minpercent 且 序号不连续，则判丢帧
	int ndrop_T0_minpercent;
	
	//T的百分比阀值,变位后，且此次无变位，第一/二个帧间隔的绝对值>T1*nTerr_T_minpercent，或第三个帧间隔的绝对值>2*T1*nTerr_T_minpercent，
	//或第四个帧间隔的绝对值>4*T1*nTerr_T_minpercent，判发送机制错误
	int nTerr_T_minpercent;
	/*********************************************************************/

	/**********************判据开关配置***********************************/
	unsigned int nerr_switch_setting;
	unsigned int nerr_switch_export;
	/************各bit位顺序如下**********
	bopen_sqNum_jump					第0bit	值0X00000001
	bopen_sqNum_reverse					第1bit	值0X00000002
	bopen_stNum_jump					第2bit	值0X00000004		
	bopen_stNum_reverse					第3bit	值0X00000008
	bopen_sqNum_unreset					第4bit	值0X00000010
	bopen_goose_double					第5bit	值0X00000020
	bopen_goose_repeat					第6bit	值0X00000040
	bopen_stNumChange_fake				第7bit	值0X00000080
	bopen_goose_delay					第8bit	值0X00000100
	bopen_tal_zero						第9bit	值0X00000200
	bopen_appid_zero					第10bit 值0X00000400
	bopen_ndscomm_true					第11bit 值0X00000800
	bopen_T0_T3_err						第12bit 值0X00001000
	bopen_daNum_differ_dsmembernum		第13bit 值0X00002000
	bopen_gocbref_differ_cfg			第14bit 值0X00004000
	bopen_goid_differ_cfg				第15bit 值0X00008000
	bopen_ds_differ_cfg					第16bit 值0X00010000
	bopen_confrev_differ_cfg			第17bit 值0X00020000
	bopen_daEntries_differ_danum		第18bit 值0X00040000
	bopen_destmac_err					第19bit 值0X00080000
	bopen_goose_restart					第20bit 值0X00100000
	bopen_stNumChange_right				第21bit 值0X00200000
	bopen_test_true						第22bit 值0X00400000
	bopen_format_err					第23bit 值0X00800000
	bopen_mac_differ_cfg				第24bit 值0X01000000
	bopen_datype_differ_cfg				第25bit 值0X02000000
	bopen_goose_drop					第26bit 值0X04000000
	bopen_goose_themble					第27bit 值0X08000000
	breserve1							第28bit 值0X10000000
	breserve2							第29bit 值0X20000000
	breserve3							第30bit 值0X40000000
	breserve4							第31bit 值0X80000000
	**************************************/

	_GOOSE_SETTING_STRUCT()
	{
		ntremble_T0_minpercent=10;
		ndelay_T0_minpercent=20;
		ndrop_T0_minpercent=100;
		nTerr_T_minpercent=20;
		nerr_switch_setting = 0XFFFFFFFF;
	}

}GOOSE_SETTING_STRUCT;

#ifdef OS_WINDOWS

#ifdef DLL_FILE_GOOSE 

class _declspec(dllexport) CGSAnalyze_I //导出类 

#else 

class _declspec(dllimport) CGSAnalyze_I //导入类 

#endif//DLL_FILE_GOOSE

#endif//OS_WINDOWS

#ifdef OS_LINUX

class CGSAnalyze_I

#endif //OS_LINUX

{
protected:
	CGSAnalyze_I(){};
	
public:
	virtual int gooseanalyze_init(GOOSE_CFG_STRUCT *pCfgInfo)=0;
	//考虑了闭锁机制的
	virtual MESSAGE_ERROR_INFO_ARRAY_STRUCT* gooseanalyze_online(GOOSE_INFO_STRUCT *p_goose_infos, int n_num)=0;//不再支持
	//不考虑考虑闭锁机制
	virtual MESSAGE_ERROR_INFO_ARRAY_STRUCT* gooseanalyze_offline(GOOSE_INFO_STRUCT *p_goose_infos, int n_num)=0;

	virtual int gooseanalyze_releasemsgerr(MESSAGE_ERROR_INFO_ARRAY_STRUCT* p_message_errs)=0;

	virtual int gooseanalyze_end()=0;

public:
	virtual int reset_lastinfo()=0;
	virtual int reset_gserr()=0;

public:

	virtual bool sqNum_jump(GOOSE_INFO_STRUCT *goose_info_fst, GOOSE_INFO_STRUCT *goose_info_sec)=0;
	
	virtual bool sqNum_reverse(GOOSE_INFO_STRUCT *goose_info_fst, GOOSE_INFO_STRUCT *goose_info_sec)=0;
	
	virtual bool stNum_jump(GOOSE_INFO_STRUCT *goose_info_fst, GOOSE_INFO_STRUCT *goose_info_sec)=0;
	
	virtual bool stNum_reverse(GOOSE_INFO_STRUCT *goose_info_fst, GOOSE_INFO_STRUCT *goose_info_sec)=0;
	
	//stNum正确变位
	virtual bool stNumChange_right(GOOSE_INFO_STRUCT *goose_info_fst, GOOSE_INFO_STRUCT *goose_info_sec)=0;
	
	virtual bool sqNum_unreset(GOOSE_INFO_STRUCT *goose_info_fst, GOOSE_INFO_STRUCT *goose_info_sec)=0;
	
	virtual bool goose_double(GOOSE_INFO_STRUCT *goose_info_fst, GOOSE_INFO_STRUCT *goose_info_sec)=0;
	
	virtual bool goose_repeat(GOOSE_INFO_STRUCT *goose_info_fst, GOOSE_INFO_STRUCT *goose_info_sec)=0;//考虑性能，暂时不实现
		
	virtual bool stNumChange_fake(GOOSE_INFO_STRUCT *goose_info_fst, GOOSE_INFO_STRUCT *goose_info_sec)=0;
	//no use
	virtual bool goose_delay(GOOSE_INFO_STRUCT *goose_info_fst, GOOSE_INFO_STRUCT *goose_info_sec)=0;
	
	virtual bool goose_test(GOOSE_INFO_STRUCT *goose_info)=0;
	
	virtual bool tal_err(GOOSE_INFO_STRUCT *goose_info)=0;
	
	virtual bool appid_zero(GOOSE_INFO_STRUCT *goose_info)=0;
	
	virtual bool ndsCom_true(GOOSE_INFO_STRUCT *goose_info)=0;
	
	virtual bool goose_restart(GOOSE_INFO_STRUCT *goose_info)=0;
	
	virtual bool mms_datas_no_change(GOOSE_INFO_STRUCT *goose_info_fst,GOOSE_INFO_STRUCT *goose_info_sec)=0;
	
	virtual bool daNum_differ_dsmembernum(GOOSE_INFO_STRUCT *goose_info)=0;
	virtual bool gocbref_differ_cfg(GOOSE_INFO_STRUCT *goose_info)=0;
	virtual bool goid_differ_cfg(GOOSE_INFO_STRUCT *goose_info)=0;
	virtual bool ds_differ_cfg(GOOSE_INFO_STRUCT *goose_info)=0;
	virtual bool confrev_differ_cfg(GOOSE_INFO_STRUCT *goose_info)=0;
	virtual bool daEntries_differ_danum(GOOSE_INFO_STRUCT *goose_info)=0;
	
	virtual bool destmac_err(GOOSE_INFO_STRUCT *goose_info)=0;
	virtual bool mac_differ_cfg(GOOSE_INFO_STRUCT *goose_info)=0;
	
	virtual int gooseanalze_set_setting(GOOSE_SETTING_STRUCT* p_setiing)=0;
	virtual MESSAGE_ERROR_INFO_ARRAY_STRUCT* gooseanalyze_withcool(GOOSE_INFO_STRUCT *p_goose_infos, int n_num,bool bCool)=0;

};


#ifdef OS_WINDOWS

#ifdef DLL_FILE_GOOSE 

//导出函数

extern "C"  __declspec( dllexport ) CGSAnalyze_I * createGSAnalyze_I();
extern "C"  __declspec( dllexport ) void destroyGSAnalyze_I(CGSAnalyze_I* theInterface);

#else 

//导入函数

extern "C"  __declspec( dllimport ) CGSAnalyze_I * createGSAnalyze_I();
extern "C"  __declspec( dllimport ) void destroyGSAnalyze_I(CGSAnalyze_I* theInterface);

#endif //DLL_FILE_GOOSE
#endif //OS_WINDOWS

#ifdef OS_LINUX
#ifdef __cplusplus
extern "C"
{
#endif
	
	CGSAnalyze_I * createGSAnalyze_I();
	void destroyGSAnalyze_I(CGSAnalyze_I* theInterface);
	
#ifdef __cplusplus
}
#endif

#endif //OS_LINUX


#endif /*GOOSE_ANALYZE_H_I*/
