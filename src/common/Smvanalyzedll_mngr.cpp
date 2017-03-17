#include "Smvanalyzedll_mngr.h"

#ifdef OS_LINUX  
const sint8* SMVANALYZE_LIB_NAME = "./lib/smvanalyze.so"; 

#else
const sint8* SMVANALYZE_LIB_NAME = "./lib/smvanalyze.dll";

#endif

CSmvanalyzedll_mngr::CSmvanalyzedll_mngr()
{
	m_hLib = NULL;
	m_pFunCre = NULL;
	m_pFunDestroy = NULL;
}

int CSmvanalyzedll_mngr::load_lib()
{
	int nRet = RES_SUCCESS;
	
	m_hLib = xj_load_library(SMVANALYZE_LIB_NAME);
	if (NULL == m_hLib)
	{
		return RES_FAIL;
	}
	
	m_pFunCre = (_FUN_CREATE_SMV)xj_get_addr(m_hLib, "createSMVAnalyze_I");
	if (NULL == m_pFunCre)
	{
		return RES_FAIL;
	}
	
	m_pFunDestroy = (_FUN_DESTROY_SMV)xj_get_addr(m_hLib, "destroySMVAnalyze_I");
	if (NULL == m_pFunDestroy)
	{
		return RES_FAIL;
	}
	
	return nRet;
}

int CSmvanalyzedll_mngr::unload_lib()
{
	int nRet = RES_SUCCESS;
	int nFreeLib = 0;
	
	if (NULL != m_hLib)
	{
		nFreeLib = xj_free_library(m_hLib);
		if (0 != nFreeLib)
		{
			printf("Ð¶ÔØ¶¯Ì¬¿â %s Ê§°Ü! \n", SMVANALYZE_LIB_NAME );
			return RES_FAIL;
		}
		else
		{
			printf("Ð¶ÔØ¶¯Ì¬¿â %s ³É¹¦ \n", SMVANALYZE_LIB_NAME);
		}
	}
	
	return nRet;
}

CSMVAnalyze_I* CSmvanalyzedll_mngr::create_obj()
{
	CSMVAnalyze_I* p_Obj = NULL;
	
	p_Obj = m_pFunCre();
	
	return p_Obj;
}

int CSmvanalyzedll_mngr::destroy_obj(CSMVAnalyze_I* theInterface)
{
	int nRet = RES_SUCCESS;
	
	if (NULL == m_pFunDestroy)
		return RES_FAIL;
	
	m_pFunDestroy(theInterface);
	
	return nRet;
}