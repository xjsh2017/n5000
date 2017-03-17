#include "IEC61850Analyzer.h"
#include "wsconstant.h"
#include "xjlib.h"
CIEC61850Analyzer *CIEC61850Analyzer::iec61850Instance = NULL;
CIEC61850Analyzer::CIEC61850Analyzer(void)
{
}
//CAnalyzer_mngr  CIEC61850Analyzer::m_ptpAnaMgr;

CIEC61850Analyzer::~CIEC61850Analyzer(void)
{
}
/*初始化，加载mms动态库*/
int CIEC61850Analyzer::InitializeMms2AcsiAnalyzer(const char* cMmsanalyzerDllFullPath)
{
	return m_Mms2AcsiWrapper.Initialize(cMmsanalyzerDllFullPath);
}
/*初始化，加载goose动态库*/
int CIEC61850Analyzer::InitializeGooseAnalyzer(const char* cGooseAnalyzerDllFullPath)
{
	return m_gooseAnalyzer.Initialize(cGooseAnalyzerDllFullPath);
}
/*初始化，加载采样分析动态库*/
int CIEC61850Analyzer::InitializeSmvAnalyzer(const char* cSmvAnalyzerDllFullPath)
{
	return m_smvAnalyzer.Initialize(cSmvAnalyzerDllFullPath);
}
/*初始化，加载PTP分析动态库*/
int CIEC61850Analyzer::InitializePtpAnalyzer(const char* cSmvAnalyzerDllFullPath)
{
    //if (RES_SUCCESS == m_ptpAnaMgr.load_lib(cSmvAnalyzerDllFullPath))
    //{
        //m_ptpv2Analyzer =(CAnalyzer_I*) m_ptpAnaMgr.create_obj();
    //}
	return 0;//m_ptpAnalyzer.Initialize(cSmvAnalyzerDllFullPath);
}

/*全部卸载*/
int CIEC61850Analyzer::Uninitialize()
{
	//mms
	m_Mms2AcsiWrapper.UnInitialize();
	//goose
	m_gooseAnalyzer.UnInitialize();
	//smv
	m_smvAnalyzer.UnInitialize();
	return 0;
}
/*释放全部结构*/
int CIEC61850Analyzer::Release_Analyzed_STRUCT(void* pAnalyzed_Struct,int nStructType)
{
	if(pAnalyzed_Struct == NULL)
		return -1;

	if(nStructType == PROTOAPPTYPE_MMS)//释放mms结构
	{
		m_Mms2AcsiWrapper.I_ACSI_CLEAN_ACSI_INFO((AP_ACSI_INFO*)pAnalyzed_Struct,1);
	}
	else if(nStructType == PROTOAPPTYPE_GOOSE)//释放goose报文
	{
		m_gooseAnalyzer.I_RELEASEMESSAGEERRS((MESSAGE_ERROR_INFO_ARRAY_STRUCT*)pAnalyzed_Struct);
	}
	else if(nStructType == PROTOAPPTYPE_SMV92)//采样
	{
		m_smvAnalyzer.I_RELEASEMESSAGEERRS((MESSAGE_ERROR_INFO_ARRAY_STRUCT*)pAnalyzed_Struct);
	}
	else if(nStructType == PROTOAPPTYPE_TIME1588)//ptp
	{
//		m_ptpv2Analyzer->analyze_releasemsgerr((MESSAGE_ERROR_INFO_ARRAY_STRUCT*)pAnalyzed_Struct);
	}
	else
	{
		;
	}
	return 0;
}
