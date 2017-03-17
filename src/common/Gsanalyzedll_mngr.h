#ifndef GOOSE_ANALYZE_DLL_MNGR_H_INCLUDE
#define GOOSE_ANALYZE_DLL_MNGR_H_INCLUDE

#include "GOOSEAnalyze_I.h"


typedef CGSAnalyze_I* (*_FUN_CREATE)();
typedef void (*_FUN_DESTROY)(CGSAnalyze_I* theInterface);

class CGsAnalyzedll_mngr
{
public:
	CGsAnalyzedll_mngr();
	~CGsAnalyzedll_mngr(){};

public:
	int load_lib();
	int unload_lib();
	CGSAnalyze_I* create_obj();
	int destroy_obj(CGSAnalyze_I* theInterface);
	
private:
	_FUN_CREATE m_pFunCre;
	_FUN_DESTROY m_pFunDestroy;
	XJHANDLE m_hLib;

};

#endif /*GOOSE_ANALYZE_DLL_MNGR_H_INCLUDE*/