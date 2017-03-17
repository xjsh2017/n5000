#ifndef SMV_ANALYZE_H_I
#define SMV_ANALYZE_H_I

#include "LogFile.h"
#include "acsistruct.h"
#include "AT_STRUCT.h"
#include "AnalyzeOnline_STRUCT.h"
#include "SYS_MODEL_STRUCT.h"

/*******************SMV������ֵ*********************************/
#define SETTING_SMV_DELAY						0X00000001				//�ӳ�
#define SETTING_SMV_APPID_ZERO					0X00000002				//APPIDΪ��
#define SETTING_SMV_PDULEN_ZERO					0X00000004				//PDU����Ϊ��
#define SETTING_SMV_DESTMAC_ERR					0X00000008				//Ŀ��MAC��ַ����
#define SETTING_SMV_MAC_DIFFER_CFG				0X00000010				//Ŀ��MAC��ַ�����ò�һ��
#define SETTING_SMV_FORMAT_ERR					0X00000020				//��ʽ����
#define SETTING_SMV_ASDU_DIFFER_CFG				0X00000040				//ASDU���������ò�һ��
#define SETTING_SMV_TREMBLE						0X00000080				//����
#define SETTING_SMV_RATE_UNSTABLE				0X00000100				//Ƶ�ʲ��ȶ�
#define SETTING_SMV_DOUBLE						0X00000200				//˫SMV
#define SETTING_SMV_SMPCNT_JUMP					0X00000400				//�������
#define SETTING_SMV_SMPCNT_REVERSE				0X00000800				//�����ת
#define SETTING_SMV_ASYNCHRONISM				0X00001000				//��ʧͬ���ź�
#define SETTING_SMV_DANUM_DIFFER_DSMEMBERNUM	0X00002000				//ͨ���������ò�һ��
#define SETTING_SMV_DS_DIFFER_CFG				0X00004000				//ds�����ò�һ��
#define SETTING_SMV_SVID_DIFFER_CFG				0X00008000				//svID�����ò�һ��
#define SETTING_SMV_CONFREV_DIFFER_CFG			0X00010000				//confRev�����ò�һ��
#define SETTING_SMV_DROP_PKT					0X00020000				//��֡
#define SETTING_SMV_DELAY_TR_TG_CHANGE			0X00040000				//ͨ����ʱ�仯

///////////////#define SETTING_SMV_DELAY						0X00040000				//�ӳ�
#define SETTING_SMV_QUALITY_CHANGE				0X00080000				//SMV����Ʒ�ʱ仯
#define SETTING_SMV_DA_ERR						0X00100000				//�����쳣
#define SETTING_SMV_DOUBLE_AD_DATA_DIFFER		0X00200000				//˫AD���ݲ�һ��
#define SETTING_SMV_TEST						0X00400000				//�ü��ޱ�־
#define SETTING_SMV_RESERVE1					0X00800000
#define SETTING_SMV_INVALID_FIELD_VAL			SETTING_SMV_RESERVE1				//�ֶ�����ֵ��Ч������ͬ����־0,1��2������ֵ��Ч
#define SETTING_SMV_RESERVE2					0X01000000	
#define SETTING_SMV_RESYNCHRON 					SETTING_SMV_RESERVE2	 //ͬ���ָ�
#define SETTING_SMV_RESERVE3					0X02000000
#define SETTING_SMV_RESERVE4					0X04000000
#define SETTING_SMV_RESERVE5					0X08000000
#define SETTING_SMV_RESERVE6					0X10000000
#define SETTING_SMV_RESERVE7					0X20000000
#define SETTING_SMV_RESERVE8					0X40000000
#define SETTING_SMV_RESERVE9					0X80000000			


typedef struct _SMV_CFG_STRUCT
{
	//��־�ļ�������Ϊ�գ���ʱ��д��־
	CLogFile *pLogFile;
	
	ACSI_IED_CTRL *pIED_ctrls;
	
	//��Ϊsmv�ӳٵ�ʱ�䷧ֵ����λms
	float ftime_smv_delay;
	//�ڵ���Ϣ
	SNIFFER_APP sniffer_app_info;
	//����ֵ���ݷ�ֵ
	unsigned int n_da_max_val;
	//SMV������ת���ֵ
	unsigned int n_max_smpcnt;
	//˫AD���ݱ仯�ٷֱȷ�ֵ
	int n_doublead_change_percent;
	//ң��ͨ������
	int n_yc_channel_num;
	//ң��ͨ������
	CFG_REC_CHANNEL** pyc_channel_list;
	//ͨ���ӳٱ仯�ٷֱȷ�ֵ
	int n_tr_delay_change_percent;
	//˫AD�澯������ƣ��������ѹ�����ֵ��
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
	//֡�����250us�ľ���ֵX��250us*ntremble_minpercent < X < 250us*ndelay_minpercent��(���������)�����ǲ���ֵ����
	int ntremble_minpercent; 

	//֡�����250us�Ĳ�X��250us*ndelay_minpercent <= X <250us*ndrop_minpercent��(���������),���ǲ���ֵ�ӳ�
	int ndelay_minpercent; 

	//֡�����250us�ľ���ֵ��>=250us*ndrop_minpercent������Ų����������ǲ���ֵ��֡
	int ndrop_minpercent; 

	//��ǰ����Ƶ�ʼ�4000֡/s�ľ���ֵY��Y > 4000*10%�����ǲ���ֵƵ�ʲ��ȶ�
	int nrate_unstable_minpercent;

	//�оݿ�������
	unsigned int nerr_switch_setting;
	//�оݿ�������
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

class _declspec(dllexport) CSMVAnalyze_I //������ 

#else 

class _declspec(dllimport) CSMVAnalyze_I //������ 

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
	
	/***********************δʵ��*******************************************************/
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

	/*******************************20120525����******************************************/
	virtual bool asdu_differ_cfg(SMV_INFO_STRUCT *smv_info)=0;
	virtual bool double_ad_data_differ(ASDU_INFO_STRUCT *asdu_info)=0;
	virtual bool delay_tr_tg_change(ASDU_INFO_STRUCT *asdu_info_fst, ASDU_INFO_STRUCT *asdu_info_sec)=0;
	virtual bool smv_tremble(SMV_INFO_STRUCT *smv_info_fst, SMV_INFO_STRUCT *smv_info_sec)=0;
	virtual bool drop_pkt(double d_sec_interval, ASDU_INFO_STRUCT *asdu_info_fst, ASDU_INFO_STRUCT *asdu_info_sec)=0;
	virtual bool rate_unstable(double d_sec_interval, ASDU_INFO_STRUCT *asdu_info_fst, ASDU_INFO_STRUCT *asdu_info_sec)=0;
	/************************************************************************************/

	//����Ĵ��󣬰��������ò�һ�¡���ʧͬ���źš�Ŀ��MAC��ַ����
	virtual MESSAGE_ERROR_INFO_STRUCT* smvanalyze_const_error(SMV_INFO_STRUCT *p_smv_info)=0;
	//ͨ������
	virtual MESSAGE_ERROR_INFO_STRUCT* smvanalyze_da_error(SMV_INFO_STRUCT *p_smv_info)=0;
	//�����Ĵ���
	virtual MESSAGE_ERROR_INFO_STRUCT* smvanalyze_context_error(SMV_INFO_STRUCT *p_smvinfo1, SMV_INFO_STRUCT *p_smvinfo2)=0;
	virtual int smvanalyze_releasemsgerr_ex(MESSAGE_ERROR_INFO_STRUCT* p_message_err)=0;

};

#ifdef OS_WINDOWS

#ifdef DLL_FILE_SMV

//��������
extern "C" __declspec( dllexport ) CSMVAnalyze_I* createSMVAnalyze_I();
extern "C" __declspec( dllexport ) void destroySMVAnalyze_I(CSMVAnalyze_I* theInterface);

#else
//��������
extern "C" __declspec( dllimport ) CSMVAnalyze_I* createSMVAnalyze_I();
extern "C" __declspec( dllimport ) void destroySMVAnalyze_I(CSMVAnalyze_I* theInterface);

//���뺯��

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
