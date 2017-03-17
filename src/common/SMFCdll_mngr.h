#ifndef SMFC_DLL_MNGR_H_INCLUDE
#define SMFC_DLL_MNGR_H_INCLUDE

#include "define_scan.h"
#include "ShareMemFileCtrl_I.h"

typedef CSMFC_I* (*_FUN_CREATE_SMFC)(SMFC_CFG_STRUCT* pSmfcCfg);
typedef void (*_FUN_DESTROY_SMFC)(CSMFC_I* theInterface);

class CSMFCdll_mngr
{
public:
	CSMFCdll_mngr();
	virtual ~CSMFCdll_mngr(){};

public:
	int load_smfc_lib();
	int unload_smfc_lib();
	CSMFC_I* create_smfc(SMFC_CFG_STRUCT *pSmfcCfg);
	int destroy_smfc(CSMFC_I*);

private:
	_FUN_CREATE_SMFC m_pFunCreSMFC;
	_FUN_DESTROY_SMFC m_pFunDestroySMFC;
	XJHANDLE m_hSmfcLib;


};

#endif /*SMFC_DLL_MNGR_H_INCLUDE*/