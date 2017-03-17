#include "Analyzerdll_mngr.h"

CAnalyzer_mngr::CAnalyzer_mngr()
{
	m_hLib = NULL;
	m_pFunCre = NULL;
	m_pFunDestroy = NULL;
}

int CAnalyzer_mngr::load_lib(const char* szAanDll)
{
	int nRet = RES_SUCCESS;
	if (m_hLib)
	{
		return nRet;
	}
	m_hLib = xj_load_library(szAanDll);
	if (NULL == m_hLib)
	{
#ifndef OS_WINDOWS
		printf("cant loadlib %d,%s",errno,strerror(errno));
#endif
		return RES_FAIL;
	}
	
	m_pFunCre = (_FUN_ANALYZER_CREATE)xj_get_addr(m_hLib, "createAnalyzer_I");
	if (NULL == m_pFunCre)
	{
#ifndef OS_WINDOWS
		printf("cant find sysmbol createAnalyzer_I %d,%s",errno,strerror(errno));
#endif
		return RES_FAIL;
	}
	
	m_pFunDestroy = (_FUN_ANALYZER_DESTROY)xj_get_addr(m_hLib, "destroyAnalyzer_I");
	if (NULL == m_pFunDestroy)
	{
#ifndef OS_WINDOWS
		printf("cant find sysmbol destroyAnalyzer_I %d,%s",errno,strerror(errno));
#endif
		return RES_FAIL;
	}
	
	m_dllName = szAanDll;
	return nRet;
}

int CAnalyzer_mngr::unload_lib()
{
	int nRet = RES_SUCCESS;
	int nFreeLib = 0;
	
	if (NULL != m_hLib)
	{
		nFreeLib = xj_free_library(m_hLib);
		if (0 != nFreeLib)
		{
			printf("Ð¶ÔØ¶¯Ì¬¿â %s Ê§°Ü! \n", m_dllName.c_str() );
			return RES_FAIL;
		}
		else
		{
			printf("Ð¶ÔØ¶¯Ì¬¿â %s ³É¹¦ \n", m_dllName.c_str());
		}
	}
	
	return nRet;
}

CAnalyzer_I* CAnalyzer_mngr::create_obj()
{
	CAnalyzer_I* p_Obj = NULL;
	
	p_Obj = m_pFunCre();
	
	return p_Obj;
}

int CAnalyzer_mngr::destroy_obj(CAnalyzer_I* theInterface)
{
	int nRet = RES_SUCCESS;
	
	if (NULL == m_pFunDestroy)
		return RES_FAIL;
	
	m_pFunDestroy(theInterface);
	
	return nRet;
}