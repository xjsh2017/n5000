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
	/*��ʼ��������mms��̬��*/
	int InitializeMms2AcsiAnalyzer(const char* cMms2AcsiAnalyzerDllFullPath);
	/*��ʼ��������goose��̬��*/
	int InitializeGooseAnalyzer(const char* cGooseAnalyzerDllFullPath);
	/*��ʼ�������ز���������̬��*/
	int InitializeSmvAnalyzer(const char* cSmvAnalyzerDllFullPath);
	/*��ʼ��������PTP������̬��*/
	int InitializePtpAnalyzer(const char* cSmvAnalyzerDllFullPath);

	/*ȫ��ж��*/
	int Uninitialize();
public:
	CMms2AcsiWrapper   m_Mms2AcsiWrapper; //mms2acsi������
	CGooseAnalyzer     m_gooseAnalyzer;   //goose������
	CSmvAnalyzer       m_smvAnalyzer;     //����������
    //CAnalyzer_I     *m_ptpv2Analyzer;   //goose������
    //static CAnalyzer_mngr  m_ptpAnaMgr;
	//CPtpAnalyzer     m_ptpAnalyzer;   //goose������
public:
	/*�ͷŷ����ṹ*/
	int Release_Analyzed_STRUCT(void* pAnalyzed_Struct,int nStructType);
	
};

