#ifndef SMV_ANALYZE_DLL_MNGR_H_INCLUDE
#define SMV_ANALYZE_DLL_MNGR_H_INCLUDE

#include "SMVAnalyze_I.h"

typedef CSMVAnalyze_I* (*_FUN_CREATE_SMV)();
typedef void (*_FUN_DESTROY_SMV)(CSMVAnalyze_I* theInterface);

class CSmvanalyzedll_mngr
{
public:
	CSmvanalyzedll_mngr();
	~CSmvanalyzedll_mngr(){};

public:
	int load_lib();
	int unload_lib();
	CSMVAnalyze_I* create_obj();
	int destroy_obj(CSMVAnalyze_I* theInterface);
	
private:
	_FUN_CREATE_SMV m_pFunCre;
	_FUN_DESTROY_SMV m_pFunDestroy;
	XJHANDLE m_hLib;

};

#endif /*SMV_ANALYZE_DLL_MNGR_H_INCLUDE*/