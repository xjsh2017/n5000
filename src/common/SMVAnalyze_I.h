#ifndef SMV_ANALYZE_H_I
#define SMV_ANALYZE_H_I

#include "LogFile.h"
#include "acsistruct.h"
#include "AT_STRUCT.h"
#include "AnalyzeOnline_STRUCT.h"
#include "SYS_MODEL_STRUCT.h"

/*******************SMV分析定值*********************************/
#define SETTING_SMV_DELAY						0X00000001				//延迟
#define SETTING_SMV_APPID_ZERO					0X00000002				//APPID为零
#define SETTING_SMV_PDULEN_ZERO					0X00000004				//PDU长度为零
#define SETTING_SMV_DESTMAC_ERR					0X00000008				//目的MAC地址错误
#define SETTING_SMV_MAC_DIFFER_CFG				0X00000010				//目的MAC地址与配置不一致
#define SETTING_SMV_FORMAT_ERR					0X00000020				//格式错误
#define SETTING_SMV_ASDU_DIFFER_CFG				0X00000040				//ASDU个数与配置不一致
#define SETTING_SMV_TREMBLE						0X00000080				//抖动
#define SETTING_SMV_RATE_UNSTABLE				0X00000100				//频率不稳定
#define SETTING_SMV_DOUBLE						0X00000200				//双SMV
#define SETTING_SMV_SMPCNT_JUMP					0X00000400				//序号跳变
#define SETTING_SMV_SMPCNT_REVERSE				0X00000800				//序号逆转
#define SETTING_SMV_ASYNCHRONISM				0X00001000				//丢失同步信号
#define SETTING_SMV_DANUM_DIFFER_DSMEMBERNUM	0X00002000				//通道数与配置不一致
#define SETTING_SMV_DS_DIFFER_CFG				0X00004000				//ds与配置不一致
#define SETTING_SMV_SVID_DIFFER_CFG				0X00008000				//svID与配置不一致
#define SETTING_SMV_CONFREV_DIFFER_CFG			0X00010000				//confRev与配置不一致
#define SETTING_SMV_DROP_PKT					0X00020000				//丢帧
#define SETTING_SMV_DELAY_TR_TG_CHANGE			0X00040000				//通道延时变化

///////////////#define SETTING_SMV_DELAY						0X00040000				//延迟
#define SETTING_SMV_QUALITY_CHANGE				0X00080000				//SMV数据品质变化
#define SETTING_SMV_DA_ERR						0X00100000				//数据异常
#define SETTING_SMV_DOUBLE_AD_DATA_DIFFER		0X00200000				//双AD数据不一致
#define SETTING_SMV_TEST						0X00400000				//置检修标志
#define SETTING_SMV_RESERVE1					0X00800000
#define SETTING_SMV_INVALID_FIELD_VAL			SETTING_SMV_RESERVE1				//字段数据值无效，比如同步标志0,1，2意外数值无效
#define SETTING_SMV_RESERVE2					0X01000000	
#define SETTING_SMV_RESYNCHRON 					SETTING_SMV_RESERVE2	 //同步恢复
#define SETTING_SMV_RESERVE3					0X02000000
#define SETTING_SMV_RESERVE4					0X04000000
#define SETTING_SMV_RESERVE5					0X08000000
#define SETTING_SMV_RESERVE6					0X10000000
#define SETTING_SMV_RESERVE7					0X20000000
#define SETTING_SMV_RESERVE8					0X40000000
#define SETTING_SMV_RESERVE9					0X80000000			


typedef struct _SMV_CFG_STRUCT
{
	//日志文件，可以为空，空时不写日志
	CLogFile *pLogFile;
	
	ACSI_IED_CTRL *pIED_ctrls;
	
	//认为smv延迟的时间阀值，单位ms
	float ftime_smv_delay;
	//节点信息
	SNIFFER_APP sniffer_app_info;
	//采样值数据阀值
	unsigned int n_da_max_val;
	//SMV计数翻转最大值
	unsigned int n_max_smpcnt;
	//双AD数据变化百分比阀值
	int n_doublead_change_percent;
	//遥测通道个数
	int n_yc_channel_num;
	//遥测通道队列
	CFG_REC_CHANNEL** pyc_channel_list;
	//通道延迟变化百分比阀值
	int n_tr_delay_change_percent;
	//双AD告警最低限制（电流差、电压差绝对值）
	float fLmt_DblAD_U,fLmt_DblAD_I;
	_SMV_CFG_STRUCT()
	{
		pLogFile = NULL;
		pIED_ctrls = NULL;
		
		ftime_smv_delay=0.5;
		n_da_max_val = 250000;
		n_max_smpcnt = 65535;
		n_doublead_change_percent = 10;
		n_yc_channel_num = 0;
		pyc_channel_list = NULL;
		n_tr_delay_change_percent = 10;
		fLmt_DblAD_U=fLmt_DblAD_I=0;
	}
	
	
}SMV_CFG_STRUCT;

typedef struct _SMV_SETTING_STRUCT
{
	//帧间隔减250us的绝对值X，250us*ntremble_minpercent < X < 250us*ndelay_minpercent，(且序号连续)，则是采样值抖动
	int ntremble_minpercent; 

	//帧间隔与250us的差X，250us*ndelay_minpercent <= X <250us*ndrop_minpercent，(且序号连续),则是采样值延迟
	int ndelay_minpercent; 

	//帧间隔减250us的绝对值，>=250us*ndrop_minpercent，且序号不连续，则是采样值丢帧
	int ndrop_minpercent; 

	//当前采样频率减4000帧/s的绝对值Y，Y > 4000*10%，则是采样值频率不稳定
	int nrate_unstable_minpercent;

	//判据开关配置
	unsigned int nerr_switch_setting;
	//判据开关配置
	unsigned int nerr_switch_export;

	_SMV_SETTING_STRUCT()
	{
		ntremble_minpercent = 10;
		ndelay_minpercent = 20;
		ndrop_minpercent = 100;
		nerr_switch_setting = 0XFFFFFFFF;
	}

}SMV_SETTING_STRUCT;


#ifdef OS_WINDOWS

#ifdef DLL_FILE_SMV 

class _declspec(dllexport) CSMVAnalyze_I //导出类 

#else 

class _declspec(dllimport) CSMVAnalyze_I //导入类 

#endif//DLL_FILE_SMV

#endif//OS_WINDOWS

#ifdef OS_LINUX

class CSMVAnalyze_I

#endif //OS_LINUX

{
protected:
	CSMVAnalyze_I(){};
	
public:
	virtual int smvanalyze_init(SMV_CFG_STRUCT *pCfgInfo)=0;
	//virtual MESSAGE_ERROR_INFO_ARRAY_STRUCT* smvanalyze_online(SMV_INFO_STRUCT *p_smv_infos, int n_num)=0;
	virtual MESSAGE_ERROR_INFO_ARRAY_STRUCT* smvanalyze_offline(SMV_INFO_STRUCT *p_smv_infos, int n_num)=0;
	virtual int smvanalyze_releasemsgerr(MESSAGE_ERROR_INFO_ARRAY_STRUCT* p_message_errs)=0;
	virtual int smvanalyze_end()=0;
	virtual int smvanalyze_set_setting(SMV_SETTING_STRUCT *p_setting)=0;

public:
	virtual int reset_lastinfo()=0;
	virtual int reset_lasterr()=0;

	
public:
	virtual bool appid_zero(SMV_INFO_STRUCT *smv_info)=0;
	virtual bool pdulen_zero(SMV_INFO_STRUCT *smv_info)=0;
	virtual bool destmac_err(SMV_INFO_STRUCT *smv_info)=0;
	virtual bool asynchronism(ASDU_INFO_STRUCT *asdu_info)=0;
	virtual bool svid_differ_cfg(ASDU_INFO_STRUCT *asdu_info)=0;
	virtual bool ds_differ_cfg(ASDU_INFO_STRUCT *asdu_info)=0;
	virtual bool confrev_differ_cfg(ASDU_INFO_STRUCT *asdu_info)=0;
	virtual bool danum_differ_dsmembernum(ASDU_INFO_STRUCT *asdu_info)=0;
	virtual bool mac_differ_cfg(SMV_INFO_STRUCT *smv_info)=0;
	
	/***********************未实现*******************************************************/
	virtual bool format_err(SMV_INFO_STRUCT *smv_info)=0;
	virtual bool quality_err(SMV_DATA_STRUCT *smv_data)=0;
	/************************************************************************************/
	
public:
	virtual bool smv_delay(SMV_INFO_STRUCT *smv_info_fst, SMV_INFO_STRUCT *smv_info_sec)=0;
	virtual bool smv_double(ASDU_INFO_STRUCT *asdu_info_fst, ASDU_INFO_STRUCT *asdu_info_sec)=0;
	virtual bool smpcnt_jump(ASDU_INFO_STRUCT *asdu_info_fst, ASDU_INFO_STRUCT *asdu_info_sec)=0;
	virtual bool smpcnt_reverse(ASDU_INFO_STRUCT *asdu_info_fst, ASDU_INFO_STRUCT *asdu_info_sec)=0;
	virtual bool danum_change(ASDU_INFO_STRUCT *asdu_info_fst, ASDU_INFO_STRUCT *asdu_info_sec)=0;
	virtual bool quality_change(SMV_DATA_STRUCT *smv_data_fst, SMV_DATA_STRUCT *smv_data_sec)=0;

	/*******************************20120525新增******************************************/
	virtual bool asdu_differ_cfg(SMV_INFO_STRUCT *smv_info)=0;
	virtual bool double_ad_data_differ(ASDU_INFO_STRUCT *asdu_info)=0;
	virtual bool delay_tr_tg_change(ASDU_INFO_STRUCT *asdu_info_fst, ASDU_INFO_STRUCT *asdu_info_sec)=0;
	virtual bool smv_tremble(SMV_INFO_STRUCT *smv_info_fst, SMV_INFO_STRUCT *smv_info_sec)=0;
	virtual bool drop_pkt(double d_sec_interval, ASDU_INFO_STRUCT *asdu_info_fst, ASDU_INFO_STRUCT *asdu_info_sec)=0;
	virtual bool rate_unstable(double d_sec_interval, ASDU_INFO_STRUCT *asdu_info_fst, ASDU_INFO_STRUCT *asdu_info_sec)=0;
	/************************************************************************************/

	//不变的错误，包括与配置不一致、丢失同步信号、目的MAC地址错误
	virtual MESSAGE_ERROR_INFO_STRUCT* smvanalyze_const_error(SMV_INFO_STRUCT *p_smv_info)=0;
	//通道错误
	virtual MESSAGE_ERROR_INFO_STRUCT* smvanalyze_da_error(SMV_INFO_STRUCT *p_smv_info)=0;
	//上下文错误
	virtual MESSAGE_ERROR_INFO_STRUCT* smvanalyze_context_error(SMV_INFO_STRUCT *p_smvinfo1, SMV_INFO_STRUCT *p_smvinfo2)=0;
	virtual int smvanalyze_releasemsgerr_ex(MESSAGE_ERROR_INFO_STRUCT* p_message_err)=0;

};

#ifdef OS_WINDOWS

#ifdef DLL_FILE_SMV

//导出函数
extern "C" __declspec( dllexport ) CSMVAnalyze_I* createSMVAnalyze_I();
extern "C" __declspec( dllexport ) void destroySMVAnalyze_I(CSMVAnalyze_I* theInterface);

#else
//导出函数
extern "C" __declspec( dllimport ) CSMVAnalyze_I* createSMVAnalyze_I();
extern "C" __declspec( dllimport ) void destroySMVAnalyze_I(CSMVAnalyze_I* theInterface);

//导入函数

#endif /*DLL_FILE_SMV*/

#endif /*OS_WINDOWS*/


#ifdef OS_LINUX
#ifdef __cplusplus
extern "C"
{
#endif
	
	CSMVAnalyze_I* createSMVAnalyze_I();
	void destroySMVAnalyze_I(CSMVAnalyze_I* theInterface);
	
#ifdef __cplusplus
}
#endif

#endif //OS_LINUX



#endif /*SMV_ANALYZE_H_I*/
