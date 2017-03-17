#include "GooseAnalyzer.h"
#include "wssysconfig.h"
CGooseAnalyzer::CGooseAnalyzer(void)
{
	//加载动态库后返回的句柄
	m_hdllInst				= NULL;
	m_pFun_Create_Gsa_obj   = NULL;
	m_pFun_Destroy_Gsa_obj  = NULL;
	m_p_goose_cfg_param     = new GOOSE_CFG_STRUCT;
	m_p_goose_cfg_param->sniffer_app_info.napptype = 2;//报文类型
	m_pgsa_obj              = NULL;
	m_pgoose_setting        = new GOOSE_SETTING_STRUCT;
    CWSSysconfig *wsSysconfig = CWSSysconfig::getInstance();
	//加载系统配置
    m_pgoose_setting->ndelay_T0_minpercent   = wsSysconfig->m_wsIec61850AnaCfg.n_delay_T0_minpercent_goose;
    m_pgoose_setting->ndrop_T0_minpercent    = wsSysconfig->m_wsIec61850AnaCfg.n_drop_T0_minpercent_goose;
    m_pgoose_setting->nTerr_T_minpercent     = wsSysconfig->m_wsIec61850AnaCfg.n_Terr_T_minpercent_goose;
    m_pgoose_setting->ntremble_T0_minpercent = wsSysconfig->m_wsIec61850AnaCfg.n_tremble_T0_minpercent_goose;
    m_pgoose_setting->nerr_switch_setting = wsSysconfig->m_wsIec61850AnaCfg.n_err_switch_setting_goose;

/*	m_pgoose_setting->nerr_switch_setting = 0;
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_SQNUM_JUMP;   //sqNum跳变
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_SQNUM_REVERSE;//sqNum逆转
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_STNUM_JUMP;//stNum跳变
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_STNUM_REVERSE;//stNum逆转
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_SQNUM_UNRESET;//stNum变化,sqNum未复归
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_GOOSE_DOUBLE; //重复帧
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_GOOSE_REPEAT;//内容完全重复
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_STNUMCHANGE_FAKE;//stNum虚变
//	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_GOOSE_DELAY;//报文延迟
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_TAL_ZERO;//TAL为零
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_APPID_ZERO;//APPID为零
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_NDSCOMM_TRUE;//控制块需要配置
//	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_T0_T3_ERR;//发送机制错误
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_DANUM_DIFFER_DSMEMBERNUM;//通道个数与配置不一致
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_GOCBREF_DIFFER_CFG;//gocbref与配置不一致
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_GOID_DIFFER_CFG;//goid与配置不一致
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_DS_DIFFER_CFG;//dsname与配置不一致
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_CONFREV_DIFFER_CFG;//版本号与配置不一致
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_DAENTRIES_DIFFER_DANUM;//numDataEntries与实际数据个数不一致
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_DESTMAC_ERR;//目的MAC地址错误
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_GOOSE_RESTART;//GOOSE链路重启
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_STNUMCHANGE_RIGHT;//GOOSE变位
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_TEST_TRUE;//置检修标志
//	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_FORMAT_ERR;//格式错误
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_MAC_DIFFER_CFG;//目的MAC地址与配置不一致
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_DATYPE_DIFFER_CFG;//数据类型与配置文件不一致
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_GOOSE_DROP;//丢帧
//	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_GOOSE_THEMBLE;//心跳报文抖动*/
}

CGooseAnalyzer::~CGooseAnalyzer(void)
{
	delete m_p_goose_cfg_param;
	delete m_pgoose_setting;
	m_pgoose_setting = NULL;
}
//初始化，加载动态库，导出库函数
/**
* @brief	Initialize         初始化，加载动态库，导出库函数,需被外部显性调用
* @param 	const char * cLibPathName  动态库所在路径,需相对路径+动态库名称
* @return	int 0表示成功
* @notes	无
* @sample	无
*/
int CGooseAnalyzer::Initialize(const char * cLibPathName)
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
        m_pFun_Create_Gsa_obj  = (CREATE_GSA_OBJ)   xj_get_addr(m_hdllInst,"createGSAnalyze_I");
        m_pFun_Destroy_Gsa_obj = (DESTRORY_GSA_OBJ) xj_get_addr(m_hdllInst,"destroyGSAnalyze_I");
		//成功判断
		if(m_pFun_Create_Gsa_obj == NULL || m_pFun_Destroy_Gsa_obj == NULL)
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
int CGooseAnalyzer::UnInitialize()
{
	if(m_hdllInst != NULL)
	{
		if(m_pgsa_obj!= NULL && m_pFun_Destroy_Gsa_obj != NULL)
		{
			m_pFun_Destroy_Gsa_obj(m_pgsa_obj);
			m_pgsa_obj = NULL;
		}
        xj_free_library(m_hdllInst);//释放动态库
		m_hdllInst = NULL;
		m_pFun_Create_Gsa_obj   = NULL;
		m_pFun_Destroy_Gsa_obj  = NULL;
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
int CGooseAnalyzer::I_INIT(GOOSE_CFG_STRUCT *p_init_param)
{
	if(m_pgsa_obj == NULL)
	{
		m_pgsa_obj = m_pFun_Create_Gsa_obj();//创建
		if(m_pgsa_obj == NULL)
			return -1;
	}
	m_pgsa_obj->gooseanalyze_init(p_init_param);
	I_GOOSEANALYZE_SET_SETTING();//设置定值参数
	return 0;
}
int CGooseAnalyzer::I_SET_SNIFFER_APP(SNIFFER_APP* psniffer_app_info)
{
	m_p_goose_cfg_param->n_yx_channel_num = 0;
	m_p_goose_cfg_param->pyx_channel_list = NULL;
	if(psniffer_app_info == NULL)
	{

	}
	else
	{
		//拷贝节点参数
		strcpy(m_p_goose_cfg_param->sniffer_app_info.ccbname,psniffer_app_info->ccbname);
		strcpy(m_p_goose_cfg_param->sniffer_app_info.cdesc,psniffer_app_info->cdesc);
		strcpy(m_p_goose_cfg_param->sniffer_app_info.cgoid,psniffer_app_info->cgoid);
		strcpy(m_p_goose_cfg_param->sniffer_app_info.ciedname,psniffer_app_info->ciedname);
		strcpy(m_p_goose_cfg_param->sniffer_app_info.cipaddress,psniffer_app_info->cipaddress);
		strcpy(m_p_goose_cfg_param->sniffer_app_info.cmacaddress,psniffer_app_info->cmacaddress);
		strcpy(m_p_goose_cfg_param->sniffer_app_info.csvid,psniffer_app_info->csvid);
		strcpy(m_p_goose_cfg_param->sniffer_app_info.cdsname,psniffer_app_info->cdsname);
		m_p_goose_cfg_param->sniffer_app_info.nconfrev = psniffer_app_info->nconfrev;
		m_p_goose_cfg_param->sniffer_app_info.nappid   = psniffer_app_info->nappid;
		m_p_goose_cfg_param->sniffer_app_info.ndssize  = psniffer_app_info->ndssize;
		m_p_goose_cfg_param->sniffer_app_info.nmaxtime = psniffer_app_info->nmaxtime;
		m_p_goose_cfg_param->sniffer_app_info.nmintime = psniffer_app_info->nmintime;
		m_p_goose_cfg_param->sniffer_app_info.ntimeout = psniffer_app_info->ntimeout;
		m_p_goose_cfg_param->sniffer_app_info.nmdatalistsize = psniffer_app_info->nmdatalistsize;
		m_p_goose_cfg_param->sniffer_app_info.flmtflowdown   = psniffer_app_info->flmtflowdown;
		m_p_goose_cfg_param->sniffer_app_info.flmtflowup     = psniffer_app_info->flmtflowup;
		m_p_goose_cfg_param->sniffer_app_info.nnofaudu       = psniffer_app_info->nnofaudu;
		m_p_goose_cfg_param->sniffer_app_info.nsamprate      = psniffer_app_info->nsamprate;
		m_p_goose_cfg_param->sniffer_app_info.nsys_appsqe    = psniffer_app_info->nsys_appsqe;
		m_p_goose_cfg_param->sniffer_app_info.snifferport_sys_id = psniffer_app_info->snifferport_sys_id;
        CWSSysconfig *wsSysconfig = CWSSysconfig::getInstance();
        APP_CHANNEL_RELATION* pList = wsSysconfig->I_GET_APP_4Y_CFGINFO(psniffer_app_info->nsys_appsqe);
		if(pList != NULL)
		{
			m_p_goose_cfg_param->n_yx_channel_num = pList->n_yx_channel_num;
			m_p_goose_cfg_param->pyx_channel_list = pList->pyx_channel_list;
		}
		else
		{
			;
		}
	}
	//重新初始化
	I_INIT(m_p_goose_cfg_param);//成功
	m_pgsa_obj->reset_lastinfo();//重设
	return 0;
	
}
/*GOOSE报文分析*/
/**
* @brief	I_INIT         初始化，加载动态库，导出?夂?需被外部显性调用
* @param 	GOOSE_INFO_STRUCT *p_goose_infos   goose结构起始地址
* @param 	int n_num                          帧数,本模块中为1
* @return	int 0表示成功
* @notes	无
* @sample	无
*/
MESSAGE_ERROR_INFO_ARRAY_STRUCT* CGooseAnalyzer::I_GOOSEANALYZE(GOOSE_INFO_STRUCT *p_goose_infos, int n_num)
{
	if(p_goose_infos == NULL || n_num < 1 ||m_pgsa_obj == NULL)
		return NULL;
	return m_pgsa_obj->gooseanalyze_offline(p_goose_infos,n_num);
}
/*释放结构MESSAGE_ERROR_INFO_ARRAY_STRUCT指针的内存*/
/**
* @brief	I_INIT         释放结构MESSAGE_ERROR_INFO_ARRAY_STRUCT指针的内存
* @param 	MESSAGE_ERROR_INFO_ARRAY_STRUCT* p_message_errs   goose解析结果结构指针
* @return	int 0表示成功
* @notes	无
* @sample	无
*/
int CGooseAnalyzer::I_RELEASEMESSAGEERRS(MESSAGE_ERROR_INFO_ARRAY_STRUCT* p_message_errs)
{
	if(m_pgsa_obj!= NULL && p_message_errs != NULL)
		m_pgsa_obj->gooseanalyze_releasemsgerr(p_message_errs);		
	return 0;
}
/*设置定值*/
void CGooseAnalyzer::I_GOOSEANALYZE_SET_SETTING()
{
	if(m_pgsa_obj != NULL && m_pgoose_setting != NULL)
		m_pgsa_obj->gooseanalze_set_setting(m_pgoose_setting);
}
GOOSE_SETTING_STRUCT* CGooseAnalyzer::GET_GOOSEANALYZE_SETTING()
{
	return m_pgoose_setting;
}
