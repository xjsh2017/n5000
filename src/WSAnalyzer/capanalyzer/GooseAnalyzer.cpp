#include "GooseAnalyzer.h"
#include "wssysconfig.h"
CGooseAnalyzer::CGooseAnalyzer(void)
{
	//���ض�̬��󷵻صľ��
	m_hdllInst				= NULL;
	m_pFun_Create_Gsa_obj   = NULL;
	m_pFun_Destroy_Gsa_obj  = NULL;
	m_p_goose_cfg_param     = new GOOSE_CFG_STRUCT;
	m_p_goose_cfg_param->sniffer_app_info.napptype = 2;//��������
	m_pgsa_obj              = NULL;
	m_pgoose_setting        = new GOOSE_SETTING_STRUCT;
    CWSSysconfig *wsSysconfig = CWSSysconfig::getInstance();
	//����ϵͳ����
    m_pgoose_setting->ndelay_T0_minpercent   = wsSysconfig->m_wsIec61850AnaCfg.n_delay_T0_minpercent_goose;
    m_pgoose_setting->ndrop_T0_minpercent    = wsSysconfig->m_wsIec61850AnaCfg.n_drop_T0_minpercent_goose;
    m_pgoose_setting->nTerr_T_minpercent     = wsSysconfig->m_wsIec61850AnaCfg.n_Terr_T_minpercent_goose;
    m_pgoose_setting->ntremble_T0_minpercent = wsSysconfig->m_wsIec61850AnaCfg.n_tremble_T0_minpercent_goose;
    m_pgoose_setting->nerr_switch_setting = wsSysconfig->m_wsIec61850AnaCfg.n_err_switch_setting_goose;

/*	m_pgoose_setting->nerr_switch_setting = 0;
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_SQNUM_JUMP;   //sqNum����
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_SQNUM_REVERSE;//sqNum��ת
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_STNUM_JUMP;//stNum����
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_STNUM_REVERSE;//stNum��ת
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_SQNUM_UNRESET;//stNum�仯,sqNumδ����
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_GOOSE_DOUBLE; //�ظ�֡
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_GOOSE_REPEAT;//������ȫ�ظ�
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_STNUMCHANGE_FAKE;//stNum���
//	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_GOOSE_DELAY;//�����ӳ�
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_TAL_ZERO;//TALΪ��
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_APPID_ZERO;//APPIDΪ��
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_NDSCOMM_TRUE;//���ƿ���Ҫ����
//	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_T0_T3_ERR;//���ͻ��ƴ���
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_DANUM_DIFFER_DSMEMBERNUM;//ͨ�����������ò�һ��
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_GOCBREF_DIFFER_CFG;//gocbref�����ò�һ��
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_GOID_DIFFER_CFG;//goid�����ò�һ��
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_DS_DIFFER_CFG;//dsname�����ò�һ��
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_CONFREV_DIFFER_CFG;//�汾�������ò�һ��
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_DAENTRIES_DIFFER_DANUM;//numDataEntries��ʵ�����ݸ�����һ��
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_DESTMAC_ERR;//Ŀ��MAC��ַ����
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_GOOSE_RESTART;//GOOSE��·����
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_STNUMCHANGE_RIGHT;//GOOSE��λ
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_TEST_TRUE;//�ü��ޱ�־
//	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_FORMAT_ERR;//��ʽ����
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_MAC_DIFFER_CFG;//Ŀ��MAC��ַ�����ò�һ��
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_DATYPE_DIFFER_CFG;//���������������ļ���һ��
	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_GOOSE_DROP;//��֡
//	m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_GOOSE_THEMBLE;//�������Ķ���*/
}

CGooseAnalyzer::~CGooseAnalyzer(void)
{
	delete m_p_goose_cfg_param;
	delete m_pgoose_setting;
	m_pgoose_setting = NULL;
}
//��ʼ�������ض�̬�⣬�����⺯��
/**
* @brief	Initialize         ��ʼ�������ض�̬�⣬�����⺯��,�豻�ⲿ���Ե���
* @param 	const char * cLibPathName  ��̬������·��,�����·��+��̬������
* @return	int 0��ʾ�ɹ�
* @notes	��
* @sample	��
*/
int CGooseAnalyzer::Initialize(const char * cLibPathName)
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
        m_pFun_Create_Gsa_obj  = (CREATE_GSA_OBJ)   xj_get_addr(m_hdllInst,"createGSAnalyze_I");
        m_pFun_Destroy_Gsa_obj = (DESTRORY_GSA_OBJ) xj_get_addr(m_hdllInst,"destroyGSAnalyze_I");
		//�ɹ��ж�
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
//�ͷŶ�̬��
/**
* @brief	UnInitialize         �ͷŶ�̬��
* @param 	
* @return	int 0��ʾ�ɹ�
* @notes	��
* @sample	��
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
        xj_free_library(m_hdllInst);//�ͷŶ�̬��
		m_hdllInst = NULL;
		m_pFun_Create_Gsa_obj   = NULL;
		m_pFun_Destroy_Gsa_obj  = NULL;
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
int CGooseAnalyzer::I_INIT(GOOSE_CFG_STRUCT *p_init_param)
{
	if(m_pgsa_obj == NULL)
	{
		m_pgsa_obj = m_pFun_Create_Gsa_obj();//����
		if(m_pgsa_obj == NULL)
			return -1;
	}
	m_pgsa_obj->gooseanalyze_init(p_init_param);
	I_GOOSEANALYZE_SET_SETTING();//���ö�ֵ����
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
		//�����ڵ����
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
	//���³�ʼ��
	I_INIT(m_p_goose_cfg_param);//�ɹ�
	m_pgsa_obj->reset_lastinfo();//����
	return 0;
	
}
/*GOOSE���ķ���*/
/**
* @brief	I_INIT         ��ʼ�������ض�̬�⣬����?⺯�?�豻�ⲿ���Ե���
* @param 	GOOSE_INFO_STRUCT *p_goose_infos   goose�ṹ��ʼ��ַ
* @param 	int n_num                          ֡��,��ģ����Ϊ1
* @return	int 0��ʾ�ɹ�
* @notes	��
* @sample	��
*/
MESSAGE_ERROR_INFO_ARRAY_STRUCT* CGooseAnalyzer::I_GOOSEANALYZE(GOOSE_INFO_STRUCT *p_goose_infos, int n_num)
{
	if(p_goose_infos == NULL || n_num < 1 ||m_pgsa_obj == NULL)
		return NULL;
	return m_pgsa_obj->gooseanalyze_offline(p_goose_infos,n_num);
}
/*�ͷŽṹMESSAGE_ERROR_INFO_ARRAY_STRUCTָ����ڴ�*/
/**
* @brief	I_INIT         �ͷŽṹMESSAGE_ERROR_INFO_ARRAY_STRUCTָ����ڴ�
* @param 	MESSAGE_ERROR_INFO_ARRAY_STRUCT* p_message_errs   goose��������ṹָ��
* @return	int 0��ʾ�ɹ�
* @notes	��
* @sample	��
*/
int CGooseAnalyzer::I_RELEASEMESSAGEERRS(MESSAGE_ERROR_INFO_ARRAY_STRUCT* p_message_errs)
{
	if(m_pgsa_obj!= NULL && p_message_errs != NULL)
		m_pgsa_obj->gooseanalyze_releasemsgerr(p_message_errs);		
	return 0;
}
/*���ö�ֵ*/
void CGooseAnalyzer::I_GOOSEANALYZE_SET_SETTING()
{
	if(m_pgsa_obj != NULL && m_pgoose_setting != NULL)
		m_pgsa_obj->gooseanalze_set_setting(m_pgoose_setting);
}
GOOSE_SETTING_STRUCT* CGooseAnalyzer::GET_GOOSEANALYZE_SETTING()
{
	return m_pgoose_setting;
}
