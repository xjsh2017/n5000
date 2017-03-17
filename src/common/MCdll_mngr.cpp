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
	
	//���ض�̬��
	m_scanmcenterLibHanle = xj_load_library(SCANMCENTER_LIB_NAME);
	if (NULL==m_scanmcenterLibHanle)
	{
#ifdef OS_LINUX
		printf("err: %s \n",dlerror());
#endif
		printf("���ض�̬��: %s ʧ��! \n", SCANMCENTER_LIB_NAME);
		return RES_FAIL;
	}
	
	//��ȡ������ַ
	m_pMCCreateFun=(P_MH_CREATEMHINSTANCE_FUNC)xj_get_addr(m_scanmcenterLibHanle,"CreateMCInstance");
	if(NULL==m_pMCCreateFun)
	{
		//��ȡcreateMCInstance��ַʧ��
		printf("��ȡ%s����CreateMCInstance�ĵ�ַʧ��! \n", SCANMCENTER_LIB_NAME);
		return RES_FAIL;
	}
	
	//��ȡ������ַ
	m_pMCDestoryFun=(P_MH_DESTORYMHINSTANCE_FUNC)xj_get_addr(m_scanmcenterLibHanle,"DestroyMCInstance");
	if (NULL==m_pMCDestoryFun)
	{
		printf("��ȡ%s����DestroyMCInstance�ĵ�ַʧ��! \n", SCANMCENTER_LIB_NAME);
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
		//ж�ض�̬��
		nFreeScanmcenterLib = xj_free_library(m_scanmcenterLibHanle);
		if (0 != nFreeScanmcenterLib)
		{
			printf("ж�ض�̬�� %s ʧ��! \n", SCANMCENTER_LIB_NAME);
			nRet = RES_FAIL;
		}
		else
		{
			printf("ж�ض�̬�� %s �ɹ� \n", SCANMCENTER_LIB_NAME);
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