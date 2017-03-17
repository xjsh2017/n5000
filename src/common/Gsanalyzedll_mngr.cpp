#include "Gsanalyzedll_mngr.h"

#ifdef OS_LINUX  
const sint8* GSNALYZE_LIB_NAME = "./lib/gooseanalyze.so"; 

#else
const sint8* GSNALYZE_LIB_NAME = "./lib/gooseanalyze.dll";

#endif

CGsAnalyzedll_mngr::CGsAnalyzedll_mngr()
{
	m_hLib = NULL;
	m_pFunCre = NULL;
	m_pFunDestroy = NULL;
}

int CGsAnalyzedll_mngr::load_lib()
{
	int nRet = RES_SUCCESS;
	
	m_hLib = xj_load_library(GSNALYZE_LIB_NAME);
	if (NULL == m_hLib)
	{
		return RES_FAIL;
	}
	
	m_pFunCre = (_FUN_CREATE)xj_get_addr(m_hLib, "createGSAnalyze_I");
	if (NULL == m_pFunCre)
	{
		return RES_FAIL;
	}
	
	m_pFunDestroy = (_FUN_DESTROY)xj_get_addr(m_hLib, "destroyGSAnalyze_I");
	if (NULL == m_pFunDestroy)
	{
		return RES_FAIL;
	}
	
	return nRet;
}

int CGsAnalyzedll_mngr::unload_lib()
{
	int nRet = RES_SUCCESS;
	int nFreeLib = 0;
	
	if (NULL != m_hLib)
	{
		nFreeLib = xj_free_library(m_hLib);
		if (0 != nFreeLib)
		{
			printf("Ð¶ÔØ¶¯Ì¬¿â %s Ê§°Ü! \n", GSNALYZE_LIB_NAME );
			return RES_FAIL;
		}
		else
		{
			printf("Ð¶ÔØ¶¯Ì¬¿â %s ³É¹¦ \n", GSNALYZE_LIB_NAME);
		}
	}
	
	return nRet;
}

CGSAnalyze_I* CGsAnalyzedll_mngr::create_obj()
{
	CGSAnalyze_I* p_Obj = NULL;
	
	p_Obj = m_pFunCre();
	
	return p_Obj;
}

int CGsAnalyzedll_mngr::destroy_obj(CGSAnalyze_I* theInterface)
{
	int nRet = RES_SUCCESS;
	
	if (NULL == m_pFunDestroy)
		return RES_FAIL;
	
	m_pFunDestroy(theInterface);
	
	return nRet;
}