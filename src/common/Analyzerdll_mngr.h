#ifndef ANALYZER_DLL_MNGR_H_INCLUDE
#define ANALYZER_DLL_MNGR_H_INCLUDE

#include "Analyzer_I.h"
typedef CAnalyzer_I* (*_FUN_ANALYZER_CREATE)();
typedef void (*_FUN_ANALYZER_DESTROY)(CAnalyzer_I* theInterface);

class CAnalyzer_mngr
{
public:
	CAnalyzer_mngr();
	~CAnalyzer_mngr(){if(m_hLib)unload_lib();};
public:
	int load_lib(const char* szAanDll);
	int unload_lib();
	CAnalyzer_I *create_obj();
	int destroy_obj(CAnalyzer_I *theInterface);
	
private:
	_FUN_ANALYZER_CREATE m_pFunCre;
	_FUN_ANALYZER_DESTROY m_pFunDestroy;
	XJHANDLE m_hLib;
	string m_dllName;
};

#endif /*GOOSE_ANALYZE_DLL_MNGR_H_INCLUDE*/