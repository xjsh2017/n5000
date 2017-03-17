#ifndef MC_DLL_MNGR_H
#define MC_DLL_MNGR_H

#include "define_scan.h"
#include "PMC_Adapter_Proxy.h"

//����CMsgHandler������
typedef CPMC_Adapter_Proxy* (*P_MH_CREATEMHINSTANCE_FUNC)();

//����CMsgHandler������
typedef void (*P_MH_DESTORYMHINSTANCE_FUNC)(CPMC_Adapter_Proxy * theInterface);

class CMCdll_mngr
{
public:
	CMCdll_mngr();
	virtual ~CMCdll_mngr(){};

public:
	int LoadMCLib();
	int UnLoadMCLib();

	CPMC_Adapter_Proxy* CreMC();
	void DestroyMC(CPMC_Adapter_Proxy* pMC);

private:
	/*
	* @brief	���ض�̬��scanmcenter.dll���
	*/
	XJHANDLE m_scanmcenterLibHanle;

	//����CMsgHandler������ָ��
	P_MH_CREATEMHINSTANCE_FUNC m_pMCCreateFun;
	
	//����CMsgHandler������ָ��
	P_MH_DESTORYMHINSTANCE_FUNC m_pMCDestoryFun;

};

#endif //MC_DLL_MNGR_H   MCdll_mngr
