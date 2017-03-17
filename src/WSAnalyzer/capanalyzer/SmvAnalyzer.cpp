#include "SmvAnalyzer.h"
#include "wssysconfig.h"

extern float g_fAo_DblAd_U,g_fAo_DblAd_I;
CSmvAnalyzer::CSmvAnalyzer(void)
{
	//���ض�̬��󷵻صľ��
	m_hdllInst  = NULL;
	m_pFun_Create_Smva_obj = NULL;
	m_pFun_Destroy_Smva_obj= NULL;
	m_pCfgInfo  = new SMV_CFG_STRUCT;
	m_pCfgInfo->sniffer_app_info.napptype = 3;//����ֵ�ڵ�
    CWSSysconfig *wsSysconfig = CWSSysconfig::getInstance();
	m_psmva_obj = NULL;
	m_psmv_setting = new SMV_SETTING_STRUCT;
	//����
    m_psmv_setting->nerr_switch_setting = wsSysconfig->m_wsIec61850AnaCfg.n_err_switch_setting_sv;
	//֡�����250us�Ĳ�X��250us*ndelay_minpercent <= X <250us*ndrop_minpercent��(���������),���ǲ���ֵ�ӳ�
    m_psmv_setting->ndelay_minpercent   = wsSysconfig->m_wsIec61850AnaCfg.n_delay_minpercent_sv;
	//֡�����250us�ľ���ֵ��>=250us*ndrop_minpercent������Ų����������ǲ���ֵ��֡
    m_psmv_setting->ndrop_minpercent    = wsSysconfig->m_wsIec61850AnaCfg.n_drop_minpercent_sv;
	//֡�����250us�ľ���ֵX��250us*ntremble_minpercent < X < 250us*ndelay_minpercent��(���������)�����ǲ���ֵ����
    m_psmv_setting->ntremble_minpercent = wsSysconfig->m_wsIec61850AnaCfg.n_tremble_minpercent_sv;
	//��ǰ����Ƶ�ʼ�4000֡/s�ľ���ֵY��Y > 4000*10%�����ǲ���ֵƵ�ʲ��ȶ�
    m_psmv_setting->nrate_unstable_minpercent = wsSysconfig->m_wsIec61850AnaCfg.n_rate_unstable_minpercent_sv;

	//����ֵ����Խ���ż�ֵ
    m_pCfgInfo->n_da_max_val = wsSysconfig->m_wsIec61850AnaCfg.n_da_max_val_sv;
	//����ֵ��ŷ�ת���ֵ
    m_pCfgInfo->n_max_smpcnt = wsSysconfig->m_wsIec61850AnaCfg.n_max_smpcnt_sv;
	//ͨ����ʱ�仯�ٷֱ�
    m_pCfgInfo->n_tr_delay_change_percent = wsSysconfig->m_wsIec61850AnaCfg.n_tr_delay_change_percent_sv;
	//˫AD���ݱ仯����İٷֱ�
    m_pCfgInfo->n_doublead_change_percent = wsSysconfig->m_wsIec61850AnaCfg.n_doublead_change_percent_sv;

/*	m_psmv_setting->nerr_switch_setting = 0;
	m_psmv_setting->nerr_switch_setting|=SETTING_SMV_DELAY;
	m_psmv_setting->nerr_switch_setting|=SETTING_SMV_APPID_ZERO;	
//	m_psmv_setting->nerr_switch_setting|=SETTING_SMV_DOUBLE_AD_DATA_DIFFER; ˫AD���ݲ�һ��
	m_psmv_setting->nerr_switch_setting |= SETTING_SMV_PDULEN_ZERO;
	m_psmv_setting->nerr_switch_setting |= SETTING_SMV_DESTMAC_ERR;
	m_psmv_setting->nerr_switch_setting |= SETTING_SMV_MAC_DIFFER_CFG;
//	m_psmv_setting->nerr_switch_setting|= SETTING_SMV_FORMAT_ERR;          //��ʽ����
	m_psmv_setting->nerr_switch_setting |= SETTING_SMV_ASDU_DIFFER_CFG;
//	m_psmv_setting->nerr_switch_setting |=SETTING_SMV_TREMBLE;             //����
	m_psmv_setting->nerr_switch_setting |= SETTING_SMV_RATE_UNSTABLE;
	m_psmv_setting->nerr_switch_setting |= SETTING_SMV_DOUBLE;
	m_psmv_setting->nerr_switch_setting |= SETTING_SMV_SMPCNT_JUMP ;
	m_psmv_setting->nerr_switch_setting |= SETTING_SMV_SMPCNT_REVERSE;
	m_psmv_setting->nerr_switch_setting |= SETTING_SMV_ASYNCHRONISM;        //ʧ��
	m_psmv_setting->nerr_switch_setting |= SETTING_SMV_DANUM_DIFFER_DSMEMBERNUM;
	m_psmv_setting->nerr_switch_setting|= SETTING_SMV_DS_DIFFER_CFG;
	m_psmv_setting->nerr_switch_setting |= SETTING_SMV_SVID_DIFFER_CFG;
	m_psmv_setting->nerr_switch_setting |= SETTING_SMV_CONFREV_DIFFER_CFG ;//�汾�������ò�һ��
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
//��ʼ�������ض�̬�⣬�����⺯��
/**
* @brief	Initialize         ��ʼ�������ض�̬�⣬�����⺯��,�豻�ⲿ���Ե���
* @param 	const char * cLibPathName  ��̬������·��,�����·��+��̬������
* @return	int 0��ʾ�ɹ�
* @notes	��
* @sample	��
*/
int CSmvAnalyzer::Initialize(const char * cLibPathName)
{
	if(NULL == cLibPathName)
	{
		return -1;
	}
	try
	{
		//���ؿ�
        m_hdllInst = xj_load_library(cLibPathName);
		if(NULL == m_hdllInst)
		{
			return -1;
		}
		//���غ���
        m_pFun_Create_Smva_obj = (CREATE_SMVA_OBJ )xj_get_addr(m_hdllInst,"createSMVAnalyze_I");
        m_pFun_Destroy_Smva_obj= (DESTRORY_SMV_OBJ)xj_get_addr(m_hdllInst,"destroySMVAnalyze_I");
		//�ɹ��ж�
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
//�ͷŶ�̬��
/**
* @brief	UnInitialize         �ͷŶ�̬��
* @param 	
* @return	int 0��ʾ�ɹ�
* @notes	��
* @sample	��
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
        xj_free_library(m_hdllInst);//�ͷŶ�̬��
		m_hdllInst = NULL;
		m_pFun_Create_Smva_obj   = NULL;
		m_pFun_Destroy_Smva_obj  = NULL;
	}
	return 0;
}
/*��ʼ�����������ö���ռ�ָ���������ر�����ֵ*/
/**
* @brief	I_INIT         ��ʼ�������ض�̬�⣬�����⺯��,�豻�ⲿ���Ե���
* @param 	INIT_STRUCT *p_init_param
* @return	int 0��ʾ�ɹ�
* @notes	��
* @sample	��
*/
int CSmvAnalyzer::I_INIT(SMV_CFG_STRUCT *pCfgInfo)
{
	if(m_psmva_obj == NULL)
	{
		m_psmva_obj = m_pFun_Create_Smva_obj();//����
		if(m_psmva_obj == NULL)
			return -1;
	}
	I_SMVANALYZE_SET_SETTING();//���ö�ֵ
	return m_psmva_obj->smvanalyze_init(pCfgInfo);

}
int CSmvAnalyzer::I_SET_SNIFFER_APP(SNIFFER_APP* psniffer_app_info)
{
	m_pCfgInfo->n_yc_channel_num = 0;
	m_pCfgInfo->pyc_channel_list = NULL;

	if(psniffer_app_info == NULL)//������Ĵ���
	{

	}
	else
	{
		//�����ڵ����
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
/*SMV���ķ���*/
/**
* @brief	I_INIT         ��ʼ�������ض�̬�⣬  �����⺯��,�豻�ⲿ���Ե���
* @param 	SMV_INFO_STRUCT *p_smv_infos        SMV�ṹ��ʼ��ַ
* @param 	int n_num                           ֡��,��ģ����Ϊ1
* @return	int 0��ʾ�ɹ�
* @notes	��
* @sample	��
*/
MESSAGE_ERROR_INFO_ARRAY_STRUCT* CSmvAnalyzer::I_SMVANALYZE(SMV_INFO_STRUCT *p_smv_infos, int n_num)
{
	if(p_smv_infos == NULL || n_num <1 || m_psmva_obj == NULL)
		return NULL;

	return m_psmva_obj->smvanalyze_offline(p_smv_infos,n_num);
}
/*�ͷŽṹMESSAGE_ERROR_INFO_ARRAY_STRUCTָ����ڴ�*/
/**
* @brief	I_INIT         �ͷŽṹMESSAGE_ERROR_INFO_ARRAY_STRUCTָ����ڴ�
* @param 	MESSAGE_ERROR_INFO_ARRAY_STRUCT* p_message_errs   goose��������ṹָ��
* @return	int 0��ʾ�ɹ�
* @notes	��
* @sample	��
*/
int CSmvAnalyzer::I_RELEASEMESSAGEERRS(MESSAGE_ERROR_INFO_ARRAY_STRUCT* p_message_errs)
{
	if(p_message_errs == NULL || m_psmva_obj == NULL)
		return -1;
	return m_psmva_obj->smvanalyze_releasemsgerr(p_message_errs);
}
/*���ö�ֵ*/
void CSmvAnalyzer::I_SMVANALYZE_SET_SETTING()
{
	if(m_psmva_obj != NULL)
		m_psmva_obj->smvanalyze_set_setting(m_psmv_setting);
}
SMV_SETTING_STRUCT* CSmvAnalyzer::GET_SMVANALYZE_SETTING()
{
	return m_psmv_setting;
}
