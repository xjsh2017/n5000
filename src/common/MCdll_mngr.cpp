#include "MCdll_mngr.h"
#include "xjlib.h"

#ifdef OS_LINUX  
const sint8* SCANMCENTER_LIB_NAME = "./lib/scanmcenter.so";

#else
const sint8* SCANMCENTER_LIB_NAME = "./lib/scanmcenter.dll";

#endif

CMCdll_mngr::CMCdll_mngr()
{
	m_scanmcenterLibHanle = NULL;
	m_pMCCreateFun = NULL;
	m_pMCDestoryFun = NULL;

}

int CMCdll_mngr::LoadMCLib()
{
	int nRet = RES_SUCCESS;
	
	//加载动态库
	m_scanmcenterLibHanle = xj_load_library(SCANMCENTER_LIB_NAME);
	if (NULL==m_scanmcenterLibHanle)
	{
#ifdef OS_LINUX
		printf("err: %s \n",dlerror());
#endif
		printf("加载动态库: %s 失败! \n", SCANMCENTER_LIB_NAME);
		return RES_FAIL;
	}
	
	//获取函数地址
	m_pMCCreateFun=(P_MH_CREATEMHINSTANCE_FUNC)xj_get_addr(m_scanmcenterLibHanle,"CreateMCInstance");
	if(NULL==m_pMCCreateFun)
	{
		//获取createMCInstance地址失败
		printf("获取%s函数CreateMCInstance的地址失败! \n", SCANMCENTER_LIB_NAME);
		return RES_FAIL;
	}
	
	//获取函数地址
	m_pMCDestoryFun=(P_MH_DESTORYMHINSTANCE_FUNC)xj_get_addr(m_scanmcenterLibHanle,"DestroyMCInstance");
	if (NULL==m_pMCDestoryFun)
	{
		printf("获取%s函数DestroyMCInstance的地址失败! \n", SCANMCENTER_LIB_NAME);
		return RES_FAIL;
	}
	
	return nRet;
}

int CMCdll_mngr::UnLoadMCLib()
{
	int nRet = RES_SUCCESS;
	
	int nFreeScanmcenterLib = -1;
	
	if (NULL != m_scanmcenterLibHanle)
	{
		//卸载动态库
		nFreeScanmcenterLib = xj_free_library(m_scanmcenterLibHanle);
		if (0 != nFreeScanmcenterLib)
		{
			printf("卸载动态库 %s 失败! \n", SCANMCENTER_LIB_NAME);
			nRet = RES_FAIL;
		}
		else
		{
			printf("卸载动态库 %s 成功 \n", SCANMCENTER_LIB_NAME);
		}
	}
	
	return nRet;
}

CPMC_Adapter_Proxy* CMCdll_mngr::CreMC()
{
	CPMC_Adapter_Proxy* pMC = NULL;
	
	pMC = m_pMCCreateFun();
	
	return pMC;
}

void CMCdll_mngr::DestroyMC(CPMC_Adapter_Proxy* pMC)
{
	if (NULL == pMC)
	{
		return;
	}
	
	m_pMCDestoryFun(pMC);
}