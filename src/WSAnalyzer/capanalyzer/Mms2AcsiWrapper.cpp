#include "Mms2AcsiWrapper.h"

#include "../mms2acsi/mms2acsi.h"

typedef ST_RET (*ACSI_INIT_ENVIR)(ST_CHAR *scd_name);
typedef ST_RET (*ACSI_UNINIT_ENVIR)();
typedef AP_ACSI_INFO* (*ACSI_CRE_INFO_MMSINFO)(MMS_INFO_STRUCT *mms_info, ST_INT mms_num);
typedef ST_VOID (*ACSI_CLEAN_ACSI_INFO)(AP_ACSI_INFO* acsi_infos, ST_INT num);

ACSI_INIT_ENVIR m_pAcsi_Init_Envir;
ACSI_UNINIT_ENVIR m_pAcsi_Uninit_Envir;
ACSI_CRE_INFO_MMSINFO m_pAcsi_Cre_Info_MmsInfo;
ACSI_CLEAN_ACSI_INFO m_pAcsi_Clean_Acsi_Info;


CMms2AcsiWrapper::CMms2AcsiWrapper(void)
{
	m_hMms2acisInst = NULL;

	m_bScdFileLoaded = false;
}


CMms2AcsiWrapper::~CMms2AcsiWrapper(void)
{
	UnInitialize();//�ͷŶ�̬��
}
//��ʼ�������ض�̬�⣬�����⺯��
/**
* @brief	Initialize         ��ʼ�������ض�̬�⣬�����⺯��,�豻�ⲿ���Ե���
* @param 	const char * cLibPathName  ��̬������·��,�����·��+��̬������
* @return	int 0��ʾ�ɹ�
* @notes	��
* @sample	��
*/
int CMms2AcsiWrapper::Initialize(const char * cLibPathName)
{
///	return 0;

	if(NULL == cLibPathName)
	{
		return -1;
	}
	try
	{
		//���ؿ�
        m_hMms2acisInst = xj_load_library(cLibPathName);
		if(NULL == m_hMms2acisInst)
		{
			return -1;
		}
		//���غ���
        m_pAcsi_Init_Envir = (ACSI_INIT_ENVIR) xj_get_addr(m_hMms2acisInst,"acsi_init_envir");
        m_pAcsi_Cre_Info_MmsInfo = (ACSI_CRE_INFO_MMSINFO) xj_get_addr(m_hMms2acisInst,"acsi_cre_info_mmsInfo");
        m_pAcsi_Clean_Acsi_Info = (ACSI_CLEAN_ACSI_INFO) xj_get_addr(m_hMms2acisInst,"acsi_clean_acsi_info");
        m_pAcsi_Uninit_Envir = (ACSI_UNINIT_ENVIR)xj_get_addr(m_hMms2acisInst,"acsi_uninit_envir");
		//�ɹ��ж�
		if(m_pAcsi_Init_Envir == NULL || m_pAcsi_Cre_Info_MmsInfo == NULL || m_pAcsi_Clean_Acsi_Info == NULL
			|| m_pAcsi_Uninit_Envir == NULL)
		{
			return -1;
		}
		else
		{
			return 0;
		}
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
int CMms2AcsiWrapper::UnInitialize()
{
	if(m_bScdFileLoaded)//��ȫ�ͷ�
	{
		I_ACSI_UNINIT_ENVIR();
		m_bScdFileLoaded = false;
	}
	if(m_hMms2acisInst != NULL)
	{
        ::xj_free_library(m_hMms2acisInst);
		m_hMms2acisInst = NULL;
	}
	return 0;
}
/*��ʼ��SCD�ļ��ӿ�*/
ST_RET CMms2AcsiWrapper::I_ACSI_INIT_ENVIR(ST_CHAR *scd_name)
{
	if(NULL == scd_name   )//�����ж�||NULL == m_pAcsi_Init_Envir
	{
		return -1;
	}
	try
	{
		int nResult = 0;
		if(m_bScdFileLoaded)//scd�ļ��Ѿ����ع�
		{
			nResult = I_ACSI_UNINIT_ENVIR();//����Ѿ����ع������ͷ����е���Դ
		}
		if(nResult != 0)//�ͷ�ʧ��
		{
			return -1;
		}
		nResult = m_pAcsi_Init_Envir(scd_name);//���þ�̬��ӿں���ָ�뷵�ؽ��
		if(nResult == 0)
		{
			m_bScdFileLoaded = true;
		}
		return nResult;
	}
    catch (...)
	{
        ;
    }
	return 0;
}
/*���ұ�����ƿ����*/
AP_ACSI_INFO*  CMms2AcsiWrapper::I_ACSI_CRE_INFO_MMSINFO(MMS_INFO_STRUCT *mms_info, ST_INT mms_num)
{
	if(NULL == mms_info || 0 == mms_num )//�����ж�||m_pAcsi_Cre_Info_MmsInfo
	{
		return NULL;
	}
	try
	{
		return m_pAcsi_Cre_Info_MmsInfo(mms_info,mms_num);//���ö�̬��ӿں���ָ�뷵�ؽ��
	}
    catch (...)
	{
        ;
    }
	return NULL;
}
/*�ͷ�acsi_infos��Ϣ*/
ST_VOID  CMms2AcsiWrapper::I_ACSI_CLEAN_ACSI_INFO(AP_ACSI_INFO* acsi_infos, ST_INT num)
{
	if(NULL == acsi_infos || 0 == num)//�����ж� ||m_pAcsi_Clean_Acsi_Info
	{
		return ;
	}
	try
	{
		return m_pAcsi_Clean_Acsi_Info(acsi_infos,num);
	}
    catch (...)
	{
        ;
    }
}
/*�ͷŻ���*/
ST_RET CMms2AcsiWrapper::I_ACSI_UNINIT_ENVIR()
{
	try
	{
		m_bScdFileLoaded = false;
		return m_pAcsi_Uninit_Envir();//���þ�̬��ӿں���ָ�뷵�ؽ��
	}
    catch (...)
	{
        ;
    }
	return -1;
}
/*����SCD�ļ��Ƿ��Ѿ����ر�־λ*/
bool CMms2AcsiWrapper::IsScdFileLoaded()
{
	return m_bScdFileLoaded;
}
