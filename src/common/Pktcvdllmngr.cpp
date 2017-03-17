#include "Pktcvdllmngr.h"
#include "xjlib.h"

#ifdef OS_LINUX  
const sint8* PKTCONVERT_LIB_NAME = "./lib/packetconvert.so"; 

#else
const sint8* PKTCONVERT_LIB_NAME = "./lib/packetconvert.dll";

#endif


CPktcvdllmngr::CPktcvdllmngr()
{
	m_hLib = NULL;
	m_pFunCrePktcv = NULL;
	m_pFunDestroyPktcv = NULL;
}

int CPktcvdllmngr::load_pktcv_lib()
{
	int nRet = RES_SUCCESS;
	
	m_hLib = xj_load_library(PKTCONVERT_LIB_NAME);
	if (NULL == m_hLib)
	{
		return RES_FAIL;
	}
	
	m_pFunCrePktcv = (_FUN_CREATE_PKTCV)xj_get_addr(m_hLib, "createPktConvertI");
	if (NULL == m_pFunCrePktcv)
	{
		return RES_FAIL;
	}
	
	m_pFunDestroyPktcv = (_FUN_DESTROY_PKTCV)xj_get_addr(m_hLib, "destroyPktConvertI");
	if (NULL == m_pFunDestroyPktcv)
	{
		return RES_FAIL;
	}
	
	return nRet;
}

int CPktcvdllmngr::unload_pktcv_lib()
{
	int nRet = RES_SUCCESS;
	int nFreeLib = 0;
	
	if (NULL != m_hLib)
	{
		nFreeLib = xj_free_library(m_hLib);
		if (0 != nFreeLib)
		{
			printf("Ð¶ÔØ¶¯Ì¬¿â %s Ê§°Ü! \n", PKTCONVERT_LIB_NAME );
			return RES_FAIL;
		}
		else
		{
			printf("Ð¶ÔØ¶¯Ì¬¿â %s ³É¹¦ \n", PKTCONVERT_LIB_NAME);
		}
	}
	
	return nRet;
}

CPacketConvert_I* CPktcvdllmngr::create_pktcv()
{
	CPacketConvert_I* p_pktcv = NULL;

	p_pktcv = m_pFunCrePktcv();

	return p_pktcv;
}

int CPktcvdllmngr::destroy_pktcv(CPacketConvert_I* theInterface)
{
	int nRet = RES_SUCCESS;

	if (NULL == m_pFunDestroyPktcv)
		return RES_FAIL;
	
	m_pFunDestroyPktcv(theInterface);

	return nRet;
}