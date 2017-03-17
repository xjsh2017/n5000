#ifndef MC_DLL_MNGR_H
#define MC_DLL_MNGR_H

#include "define_scan.h"
#include "PMC_Adapter_Proxy.h"

//构造CMsgHandler对象函数
typedef CPMC_Adapter_Proxy* (*P_MH_CREATEMHINSTANCE_FUNC)();

//销毁CMsgHandler对象函数
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
	* @brief	加载动态库scanmcenter.dll句柄
	*/
	XJHANDLE m_scanmcenterLibHanle;

	//构造CMsgHandler对象函数指针
	P_MH_CREATEMHINSTANCE_FUNC m_pMCCreateFun;
	
	//销毁CMsgHandler对象函数指针
	P_MH_DESTORYMHINSTANCE_FUNC m_pMCDestoryFun;

};

#endif //MC_DLL_MNGR_H   MCdll_mngr
