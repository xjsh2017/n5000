#ifndef GOOSE_ANALYZE_H_I
#define GOOSE_ANALYZE_H_I

#include "LogFile.h"
#include "acsistruct.h"
#include "AnalyzeOnline_STRUCT.h"
#include "share_scan.h"
#include "SYS_MODEL_STRUCT.h"

/*************************GOOSE������ֵ**********************/
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
	GS_ANALYZE_I_TYPE_ONLINE = 1, //���߷���
	GS_ANALYZE_I_TYPE_OFFLINE	//���߷���
};

typedef struct _GOOSE_CFG_STRUCT
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
	/**********************�оݷ�ֵ����************************************/
	//T0�İٷֱȷ�ֵ,֡����ľ���ֵ >=T0*ntremble_T0_minpercent �� <=T0*ntremble_T0_minpercent �����������Ķ���
	int ntremble_T0_minpercent;
	
	//T0�İٷֱȷ�ֵ,֡��� >T0*ndelay_T0_minpercent �� <=T0*ndrop_T0_minpercent �����ӳ�
	int ndelay_T0_minpercent;
	
	//T0�İٷֱȷ�ֵ,֡���>T0*ndrop_T0_minpercent �� ��Ų����������ж�֡
	int ndrop_T0_minpercent;
	
	//T�İٷֱȷ�ֵ,��λ���Ҵ˴��ޱ�λ����һ/����֡����ľ���ֵ>T1*nTerr_T_minpercent���������֡����ľ���ֵ>2*T1*nTerr_T_minpercent��
	//����ĸ�֡����ľ���ֵ>4*T1*nTerr_T_minpercent���з��ͻ��ƴ���
	int nTerr_T_minpercent;
	/*********************************************************************/

	/**********************�оݿ�������***********************************/
	unsigned int nerr_switch_setting;
	unsigned int nerr_switch_export;
	/************��bitλ˳������**********
	bopen_sqNum_jump					��0bit	ֵ0X00000001
	bopen_sqNum_reverse					��1bit	ֵ0X00000002
	bopen_stNum_jump					��2bit	ֵ0X00000004		
	bopen_stNum_reverse					��3bit	ֵ0X00000008
	bopen_sqNum_unreset					��4bit	ֵ0X00000010
	bopen_goose_double					��5bit	ֵ0X00000020
	bopen_goose_repeat					��6bit	ֵ0X00000040
	bopen_stNumChange_fake				��7bit	ֵ0X00000080
	bopen_goose_delay					��8bit	ֵ0X00000100
	bopen_tal_zero						��9bit	ֵ0X00000200
	bopen_appid_zero					��10bit ֵ0X00000400
	bopen_ndscomm_true					��11bit ֵ0X00000800
	bopen_T0_T3_err						��12bit ֵ0X00001000
	bopen_daNum_differ_dsmembernum		��13bit ֵ0X00002000
	bopen_gocbref_differ_cfg			��14bit ֵ0X00004000
	bopen_goid_differ_cfg				��15bit ֵ0X00008000
	bopen_ds_differ_cfg					��16bit ֵ0X00010000
	bopen_confrev_differ_cfg			��17bit ֵ0X00020000
	bopen_daEntries_differ_danum		��18bit ֵ0X00040000
	bopen_destmac_err					��19bit ֵ0X00080000
	bopen_goose_restart					��20bit ֵ0X00100000
	bopen_stNumChange_right				��21bit ֵ0X00200000
	bopen_test_true						��22bit ֵ0X00400000
	bopen_format_err					��23bit ֵ0X00800000
	bopen_mac_differ_cfg				��24bit ֵ0X01000000
	bopen_datype_differ_cfg				��25bit ֵ0X02000000
	bopen_goose_drop					��26bit ֵ0X04000000
	bopen_goose_themble					��27bit ֵ0X08000000
	breserve1							��28bit ֵ0X10000000
	breserve2							��29bit ֵ0X20000000
	breserve3							��30bit ֵ0X40000000
	breserve4							��31bit ֵ0X80000000
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

class _declspec(dllexport) CGSAnalyze_I //������ 

#else 

class _declspec(dllimport) CGSAnalyze_I //������ 

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
	//�����˱������Ƶ�
	virtual MESSAGE_ERROR_INFO_ARRAY_STRUCT* gooseanalyze_online(GOOSE_INFO_STRUCT *p_goose_infos, int n_num)=0;//����֧��
	//�����ǿ��Ǳ�������
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
	
	//stNum��ȷ��λ
	virtual bool stNumChange_right(GOOSE_INFO_STRUCT *goose_info_fst, GOOSE_INFO_STRUCT *goose_info_sec)=0;
	
	virtual bool sqNum_unreset(GOOSE_INFO_STRUCT *goose_info_fst, GOOSE_INFO_STRUCT *goose_info_sec)=0;
	
	virtual bool goose_double(GOOSE_INFO_STRUCT *goose_info_fst, GOOSE_INFO_STRUCT *goose_info_sec)=0;
	
	virtual bool goose_repeat(GOOSE_INFO_STRUCT *goose_info_fst, GOOSE_INFO_STRUCT *goose_info_sec)=0;//�������ܣ���ʱ��ʵ��
		
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

//��������

extern "C"  __declspec( dllexport ) CGSAnalyze_I * createGSAnalyze_I();
extern "C"  __declspec( dllexport ) void destroyGSAnalyze_I(CGSAnalyze_I* theInterface);

#else 

//���뺯��

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
