#include "SmvAnalyzer.h"
#include "wssysconfig.h"

extern float g_fAo_DblAd_U,g_fAo_DblAd_I;
CSmvAnalyzer::CSmvAnalyzer(void)
{
	//加载动态库后返回的句柄
	m_hdllInst  = NULL;
	m_pFun_Create_Smva_obj = NULL;
	m_pFun_Destroy_Smva_obj= NULL;
	m_pCfgInfo  = new SMV_CFG_STRUCT;
	m_pCfgInfo->sniffer_app_info.napptype = 3;//采样值节点
    CWSSysconfig *wsSysconfig = CWSSysconfig::getInstance();
	m_psmva_obj = NULL;
	m_psmv_setting = new SMV_SETTING_STRUCT;
	//开关
    m_psmv_setting->nerr_switch_setting = wsSysconfig->m_wsIec61850AnaCfg.n_err_switch_setting_sv;
	//帧间隔与250us的差X，250us*ndelay_minpercent <= X <250us*ndrop_minpercent，(且序号连续),则是采样值延迟
    m_psmv_setting->ndelay_minpercent   = wsSysconfig->m_wsIec61850AnaCfg.n_delay_minpercent_sv;
	//帧间隔减250us的绝对值，>=250us*ndrop_minpercent，且序号不连续，则是采样值丢帧
    m_psmv_setting->ndrop_minpercent    = wsSysconfig->m_wsIec61850AnaCfg.n_drop_minpercent_sv;
	//帧间隔减250us的绝对值X，250us*ntremble_minpercent < X < 250us*ndelay_minpercent，(且序号连续)，则是采样值抖动
    m_psmv_setting->ntremble_minpercent = wsSysconfig->m_wsIec61850AnaCfg.n_tremble_minpercent_sv;
	//当前采样频率减4000帧/s的绝对值Y，Y > 4000*10%，则是采样值频率不稳定
    m_psmv_setting->nrate_unstable_minpercent = wsSysconfig->m_wsIec61850AnaCfg.n_rate_unstable_minpercent_sv;

	//采样值数据越限门槛值
    m_pCfgInfo->n_da_max_val = wsSysconfig->m_wsIec61850AnaCfg.n_da_max_val_sv;
	//采样值序号翻转最大值
    m_pCfgInfo->n_max_smpcnt = wsSysconfig->m_wsIec61850AnaCfg.n_max_smpcnt_sv;
	//通道延时变化百分比
    m_pCfgInfo->n_tr_delay_change_percent = wsSysconfig->m_wsIec61850AnaCfg.n_tr_delay_change_percent_sv;
	//双AD数据变化允许的百分比
    m_pCfgInfo->n_doublead_change_percent = wsSysconfig->m_wsIec61850AnaCfg.n_doublead_change_percent_sv;

/*	m_psmv_setting->nerr_switch_setting = 0;
	m_psmv_setting->nerr_switch_setting|=SETTING_SMV_DELAY;
	m_psmv_setting->nerr_switch_setting|=SETTING_SMV_APPID_ZERO;	
//	m_psmv_setting->nerr_switch_setting|=SETTING_SMV_DOUBLE_AD_DATA_DIFFER; 双AD数据不一致
	m_psmv_setting->nerr_switch_setting |= SETTING_SMV_PDULEN_ZERO;
	m_psmv_setting->nerr_switch_setting |= SETTING_SMV_DESTMAC_ERR;
	m_psmv_setting->nerr_switch_setting |= SETTING_SMV_MAC_DIFFER_CFG;
//	m_psmv_setting->nerr_switch_setting|= SETTING_SMV_FORMAT_ERR;          //格式错误
	m_psmv_setting->nerr_switch_setting |= SETTING_SMV_ASDU_DIFFER_CFG;
//	m_psmv_setting->nerr_switch_setting |=SETTING_SMV_TREMBLE;             //抖动
	m_psmv_setting->nerr_switch_setting |= SETTING_SMV_RATE_UNSTABLE;
	m_psmv_setting->nerr_switch_setting |= SETTING_SMV_DOUBLE;
	m_psmv_setting->nerr_switch_setting |= SETTING_SMV_SMPCNT_JUMP ;
	m_psmv_setting->nerr_switch_setting |= SETTING_SMV_SMPCNT_REVERSE;
	m_psmv_setting->nerr_switch_setting |= SETTING_SMV_ASYNCHRONISM;        //失步
	m_psmv_setting->nerr_switch_setting |= SETTING_SMV_DANUM_DIFFER_DSMEMBERNUM;
	m_psmv_setting->nerr_switch_setting|= SETTING_SMV_DS_DIFFER_CFG;
	m_psmv_setting->nerr_switch_setting |= SETTING_SMV_SVID_DIFFER_CFG;
	m_psmv_setting->nerr_switch_setting |= SETTING_SMV_CONFREV_DIFFER_CFG ;//版本号与配置不一致
	m_psmv_setting->nerr_switch_setting |=SETTING_SMV_DROP_PKT;
	m_psmv_setting->nerr_switch_setting |= SETTING_SMV_DELAY_TR_TG_CHANGE;
	m_psmv_setting->nerr_switch_setting |= SETTING_SMV_QUALITY_CHANGE;
//	m_psmv_setting->nerr_switch_setting |= SETTING_SMV_DA_ERR ;
	m_psmv_setting->nerr_switch_setting |= SETTING_SMV_TEST;*/
}

CSmvAnalyzer::~CSmvAnalyzer(void)
{
	delete m_pCfgInfo;
	m_pCfgInfo = NULL;
	delete m_psmv_setting;
	m_psmv_setting = NULL;
}
//初始化，加载动态库，导出库函数
/**
* @brief	Initialize         初始化，加载动态库，导出库函数,需被外部显性调用
* @param 	const char * cLibPathName  动态库所在路径,需相对路径+动态库名称
* @return	int 0表示成功
* @notes	无
* @sample	无
*/
int CSmvAnalyzer::Initialize(const char * cLibPathName)
{
	if(NULL == cLibPathName)
	{
		return -1;
	}
	try
	{
		//加载库
        m_hdllInst = xj_load_library(cLibPathName);
		if(NULL == m_hdllInst)
		{
			return -1;
		}
		//加载函数
        m_pFun_Create_Smva_obj = (CREATE_SMVA_OBJ )xj_get_addr(m_hdllInst,"createSMVAnalyze_I");
        m_pFun_Destroy_Smva_obj= (DESTRORY_SMV_OBJ)xj_get_addr(m_hdllInst,"destroySMVAnalyze_I");
		//成功判断
		if(m_pFun_Create_Smva_obj == NULL || m_pFun_Destroy_Smva_obj == NULL)
		{
			return -1;
		}
		return 0;
	}
    catch (...)
	{
        ;
    }

	return -1;
}
//释放动态库
/**
* @brief	UnInitialize         释放动态库
* @param 	
* @return	int 0表示成功
* @notes	无
* @sample	无
*/
int CSmvAnalyzer::UnInitialize()
{
	if(m_hdllInst != NULL)
	{
		if(m_psmva_obj!= NULL && m_pFun_Destroy_Smva_obj != NULL)
		{
			m_pFun_Destroy_Smva_obj(m_psmva_obj);
			m_psmva_obj = NULL;
		}
        xj_free_library(m_hdllInst);//释放动态库
		m_hdllInst = NULL;
		m_pFun_Create_Smva_obj   = NULL;
		m_pFun_Destroy_Smva_obj  = NULL;
	}
	return 0;
}
/*初始化函数，设置对象空间指针和其它相关变量的值*/
/**
* @brief	I_INIT         初始化，加载动态库，导出库函数,需被外部显性调用
* @param 	INIT_STRUCT *p_init_param
* @return	int 0表示成功
* @notes	无
* @sample	无
*/
int CSmvAnalyzer::I_INIT(SMV_CFG_STRUCT *pCfgInfo)
{
	if(m_psmva_obj == NULL)
	{
		m_psmva_obj = m_pFun_Create_Smva_obj();//创建
		if(m_psmva_obj == NULL)
			return -1;
	}
	I_SMVANALYZE_SET_SETTING();//设置定值
	return m_psmva_obj->smvanalyze_init(pCfgInfo);

}
int CSmvAnalyzer::I_SET_SNIFFER_APP(SNIFFER_APP* psniffer_app_info)
{
	m_pCfgInfo->n_yc_channel_num = 0;
	m_pCfgInfo->pyc_channel_list = NULL;

	if(psniffer_app_info == NULL)//空情况的处理
	{

	}
	else
	{
		//拷贝节点参数
        strcpy(m_pCfgInfo->sniffer_app_info.ccbname,psniffer_app_info->ccbname);
        strcpy(m_pCfgInfo->sniffer_app_info.cdesc,psniffer_app_info->cdesc);
        strcpy(m_pCfgInfo->sniffer_app_info.cgoid,psniffer_app_info->cgoid);
        strcpy(m_pCfgInfo->sniffer_app_info.ciedname,psniffer_app_info->ciedname);
        strcpy(m_pCfgInfo->sniffer_app_info.cipaddress,psniffer_app_info->cipaddress);
        strcpy(m_pCfgInfo->sniffer_app_info.cmacaddress,psniffer_app_info->cmacaddress);
        strcpy(m_pCfgInfo->sniffer_app_info.csvid,psniffer_app_info->csvid);
        strcpy(m_pCfgInfo->sniffer_app_info.cdsname,psniffer_app_info->cdsname);
		m_pCfgInfo->sniffer_app_info.nconfrev = psniffer_app_info->nconfrev;
		m_pCfgInfo->sniffer_app_info.nappid   = psniffer_app_info->nappid;
		m_pCfgInfo->sniffer_app_info.ndssize  = psniffer_app_info->ndssize;
		m_pCfgInfo->sniffer_app_info.nmaxtime = psniffer_app_info->nmaxtime;
		m_pCfgInfo->sniffer_app_info.nmintime = psniffer_app_info->nmintime;
		m_pCfgInfo->sniffer_app_info.ntimeout = psniffer_app_info->ntimeout;
		m_pCfgInfo->sniffer_app_info.nmdatalistsize = psniffer_app_info->nmdatalistsize;
		m_pCfgInfo->sniffer_app_info.flmtflowdown   = psniffer_app_info->flmtflowdown;
		m_pCfgInfo->sniffer_app_info.flmtflowup     = psniffer_app_info->flmtflowup;
		m_pCfgInfo->sniffer_app_info.nnofaudu       = psniffer_app_info->nnofaudu;
		m_pCfgInfo->sniffer_app_info.nsamprate      = psniffer_app_info->nsamprate;
		m_pCfgInfo->sniffer_app_info.nsys_appsqe    = psniffer_app_info->nsys_appsqe;
		m_pCfgInfo->sniffer_app_info.snifferport_sys_id = psniffer_app_info->snifferport_sys_id;
		m_pCfgInfo->fLmt_DblAD_U=g_fAo_DblAd_U;
		m_pCfgInfo->fLmt_DblAD_I=g_fAo_DblAd_I;
        CWSSysconfig *wsSysconfig = CWSSysconfig::getInstance();
        APP_CHANNEL_RELATION* pList = wsSysconfig->I_GET_APP_4Y_CFGINFO(psniffer_app_info->nsys_appsqe);
		if(pList != NULL)
		{
			m_pCfgInfo->n_yc_channel_num = pList->n_yc_channel_num;
			m_pCfgInfo->pyc_channel_list = pList->pyc_channel_list;
		}
		else
		{
			;
		}
	}
	I_INIT(m_pCfgInfo);
	m_psmva_obj->reset_lastinfo();
	return 0;
}
/*SMV报文分析*/
/**
* @brief	I_INIT         初始化，加载动态库，  导出库函数,需被外部显性调用
* @param 	SMV_INFO_STRUCT *p_smv_infos        SMV结构起始地址
* @param 	int n_num                           帧数,本模块中为1
* @return	int 0表示成功
* @notes	无
* @sample	无
*/
MESSAGE_ERROR_INFO_ARRAY_STRUCT* CSmvAnalyzer::I_SMVANALYZE(SMV_INFO_STRUCT *p_smv_infos, int n_num)
{
	if(p_smv_infos == NULL || n_num <1 || m_psmva_obj == NULL)
		return NULL;

	return m_psmva_obj->smvanalyze_offline(p_smv_infos,n_num);
}
/*释放结构MESSAGE_ERROR_INFO_ARRAY_STRUCT指针的内存*/
/**
* @brief	I_INIT         释放结构MESSAGE_ERROR_INFO_ARRAY_STRUCT指针的内存
* @param 	MESSAGE_ERROR_INFO_ARRAY_STRUCT* p_message_errs   goose解析结果结构指针
* @return	int 0表示成功
* @notes	无
* @sample	无
*/
int CSmvAnalyzer::I_RELEASEMESSAGEERRS(MESSAGE_ERROR_INFO_ARRAY_STRUCT* p_message_errs)
{
	if(p_message_errs == NULL || m_psmva_obj == NULL)
		return -1;
	return m_psmva_obj->smvanalyze_releasemsgerr(p_message_errs);
}
/*设置定值*/
void CSmvAnalyzer::I_SMVANALYZE_SET_SETTING()
{
	if(m_psmva_obj != NULL)
		m_psmva_obj->smvanalyze_set_setting(m_psmv_setting);
}
SMV_SETTING_STRUCT* CSmvAnalyzer::GET_SMVANALYZE_SETTING()
{
	return m_psmv_setting;
}
