#pragma once
#include "Mms2AcsiWrapper.h"
#include "GooseAnalyzer.h"
#include "SmvAnalyzer.h"
//#include "PtpAnalyzer.h"
//#include "Analyzer_I.h"
//#include "Analyzerdll_mngr.h"
//#include "PackageCovertWrapper.h"

class CIEC61850Analyzer
{
private:
    static CIEC61850Analyzer *iec61850Instance;
public:
    static CIEC61850Analyzer *getInstance()
    {
        if(iec61850Instance == NULL)
        {
            iec61850Instance = new CIEC61850Analyzer();
        }
        return iec61850Instance;
    }
public:
	CIEC61850Analyzer(void);
	~CIEC61850Analyzer(void);
public:
	/*初始化，加载mms动态库*/
	int InitializeMms2AcsiAnalyzer(const char* cMms2AcsiAnalyzerDllFullPath);
	/*初始化，加载goose动态库*/
	int InitializeGooseAnalyzer(const char* cGooseAnalyzerDllFullPath);
	/*初始化，加载采样分析动态库*/
	int InitializeSmvAnalyzer(const char* cSmvAnalyzerDllFullPath);
	/*初始化，加载PTP分析动态库*/
	int InitializePtpAnalyzer(const char* cSmvAnalyzerDllFullPath);

	/*全部卸载*/
	int Uninitialize();
public:
	CMms2AcsiWrapper   m_Mms2AcsiWrapper; //mms2acsi分析器
	CGooseAnalyzer     m_gooseAnalyzer;   //goose分析器
	CSmvAnalyzer       m_smvAnalyzer;     //采样分析器
    //CAnalyzer_I     *m_ptpv2Analyzer;   //goose分析器
    //static CAnalyzer_mngr  m_ptpAnaMgr;
	//CPtpAnalyzer     m_ptpAnalyzer;   //goose分析器
public:
	/*释放分析结构*/
	int Release_Analyzed_STRUCT(void* pAnalyzed_Struct,int nStructType);
	
};

