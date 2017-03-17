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
	UnInitialize();//释放动态库
}
//初始化，加载动态库，导出库函数
/**
* @brief	Initialize         初始化，加载动态库，导出库函数,需被外部显性调用
* @param 	const char * cLibPathName  动态库所在路径,需相对路径+动态库名称
* @return	int 0表示成功
* @notes	无
* @sample	无
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
		//加载库
        m_hMms2acisInst = xj_load_library(cLibPathName);
		if(NULL == m_hMms2acisInst)
		{
			return -1;
		}
		//加载函数
        m_pAcsi_Init_Envir = (ACSI_INIT_ENVIR) xj_get_addr(m_hMms2acisInst,"acsi_init_envir");
        m_pAcsi_Cre_Info_MmsInfo = (ACSI_CRE_INFO_MMSINFO) xj_get_addr(m_hMms2acisInst,"acsi_cre_info_mmsInfo");
        m_pAcsi_Clean_Acsi_Info = (ACSI_CLEAN_ACSI_INFO) xj_get_addr(m_hMms2acisInst,"acsi_clean_acsi_info");
        m_pAcsi_Uninit_Envir = (ACSI_UNINIT_ENVIR)xj_get_addr(m_hMms2acisInst,"acsi_uninit_envir");
		//成功判断
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
//释放动态库
/**
* @brief	UnInitialize         释放动态库
* @param 	
* @return	int 0表示成功
* @notes	无
* @sample	无
*/
int CMms2AcsiWrapper::UnInitialize()
{
	if(m_bScdFileLoaded)//安全释放
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
/*初始化SCD文件接口*/
ST_RET CMms2AcsiWrapper::I_ACSI_INIT_ENVIR(ST_CHAR *scd_name)
{
	if(NULL == scd_name   )//参数判断||NULL == m_pAcsi_Init_Envir
	{
		return -1;
	}
	try
	{
		int nResult = 0;
		if(m_bScdFileLoaded)//scd文件已经加载过
		{
			nResult = I_ACSI_UNINIT_ENVIR();//如果已经加载过，先释放已有的资源
		}
		if(nResult != 0)//释放失败
		{
			return -1;
		}
		nResult = m_pAcsi_Init_Envir(scd_name);//调用静态库接口函数指针返回结果
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
/*查找报告控制块对象*/
AP_ACSI_INFO*  CMms2AcsiWrapper::I_ACSI_CRE_INFO_MMSINFO(MMS_INFO_STRUCT *mms_info, ST_INT mms_num)
{
	if(NULL == mms_info || 0 == mms_num )//参数判断||m_pAcsi_Cre_Info_MmsInfo
	{
		return NULL;
	}
	try
	{
		return m_pAcsi_Cre_Info_MmsInfo(mms_info,mms_num);//调用动态库接口函数指针返回结果
	}
    catch (...)
	{
        ;
    }
	return NULL;
}
/*释放acsi_infos信息*/
ST_VOID  CMms2AcsiWrapper::I_ACSI_CLEAN_ACSI_INFO(AP_ACSI_INFO* acsi_infos, ST_INT num)
{
	if(NULL == acsi_infos || 0 == num)//参数判断 ||m_pAcsi_Clean_Acsi_Info
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
/*释放环境*/
ST_RET CMms2AcsiWrapper::I_ACSI_UNINIT_ENVIR()
{
	try
	{
		m_bScdFileLoaded = false;
		return m_pAcsi_Uninit_Envir();//调用静态库接口函数指针返回结果
	}
    catch (...)
	{
        ;
    }
	return -1;
}
/*返回SCD文件是否已经加载标志位*/
bool CMms2AcsiWrapper::IsScdFileLoaded()
{
	return m_bScdFileLoaded;
}
