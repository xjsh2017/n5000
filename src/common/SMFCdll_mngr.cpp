#include "SMFCdll_mngr.h"
#include "xjlib.h"


#ifdef OS_LINUX  
const sint8* SMFC_LIB_NAME = "./lib/smfc.so";

#else
const sint8* SMFC_LIB_NAME = "./lib/smfc.dll";

#endif

CSMFCdll_mngr::CSMFCdll_mngr()
{
	m_hSmfcLib = NULL;
	m_pFunCreSMFC = NULL;
	m_pFunDestroySMFC = NULL;

}

int CSMFCdll_mngr::load_smfc_lib()
{
	int nRet = RES_SUCCESS;
	
	m_hSmfcLib = xj_load_library(SMFC_LIB_NAME);
	if (NULL == m_hSmfcLib)
	{
		printf("%s \n", strerror(errno));

		return RES_FAIL;
	}
	
	m_pFunCreSMFC = (_FUN_CREATE_SMFC)xj_get_addr(m_hSmfcLib, "createSMFC_I");
	if (NULL == m_pFunCreSMFC)
	{
		return RES_FAIL;
	}
	
	m_pFunDestroySMFC = (_FUN_DESTROY_SMFC)xj_get_addr(m_hSmfcLib, "destroySMFC_I");
	if (NULL == m_pFunDestroySMFC)
	{
		return RES_FAIL;
	}
	
	return nRet;
}

int CSMFCdll_mngr::unload_smfc_lib()
{
	int nRet = RES_SUCCESS;
	int nFreeLib = 0;
	
	if (NULL != m_hSmfcLib)
	{
		nFreeLib = xj_free_library(m_hSmfcLib);
		if (0 != nFreeLib)
		{
			printf("卸载动态库 %s 失败! \n", SMFC_LIB_NAME );
			return RES_FAIL;
		}
		else
		{
			printf("卸载动态库 %s 成功 \n", SMFC_LIB_NAME);
		}
	}
	
	return nRet;
}

CSMFC_I* CSMFCdll_mngr::create_smfc(SMFC_CFG_STRUCT *pSmfcCfg)
{
	CSMFC_I* pSMFC = NULL;

	if (NULL==pSmfcCfg || NULL==m_pFunCreSMFC)
	{
		return NULL;
	}

	pSMFC = m_pFunCreSMFC(pSmfcCfg);

	return pSMFC;
}

int CSMFCdll_mngr::destroy_smfc(CSMFC_I* p_SFMC)
{
	int nRet = RES_SUCCESS;
	
	if (NULL != m_pFunDestroySMFC)
	{
		m_pFunDestroySMFC(p_SFMC);
	}

	printf("销毁CSMFC_I对象成功 \n");
	return nRet;
}