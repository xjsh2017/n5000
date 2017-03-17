#include "Keywords61850tranformer.h"

#include "./tixml/tinyxml.h"
CKeywords61850tranformer *CKeywords61850tranformer::keywords61850Instance = NULL;
CKeywords61850tranformer::CKeywords61850tranformer(void)
{
	m_bLoadFileOK = TRUE;
}


CKeywords61850tranformer::~CKeywords61850tranformer(void)
{

}
/*���������ļ����Ƴ�ʼ��ӳ���*/
/*
* @brief	Initialize				���������ļ����Ƴ�ʼ��ӳ���                 
* @param 	CString strCfgFileName  �����ļ�·��
* @return	CString
* @notes	��
* @sample	��
*/
int CKeywords61850tranformer::Initialize(string strCfgFileName)
{
	//�����ж�
    if(strCfgFileName.empty())
		return -1;
	//�����ֵ��ĵ�
	TiXmlDocument tinyxmlparser;
	TiXmlElement* pElement;
	TiXmlElement* pRootElement;

//	CUTF8ConverGB2312 utf8togb2312;

    if(!tinyxmlparser.LoadFile(strCfgFileName.c_str()))
	{
		m_bLoadFileOK = FALSE;
		return -1;
	}
	
	pRootElement = tinyxmlparser.FirstChildElement("wsdictonary");
	if(pRootElement == NULL)
		return -1;
	std::string strkey,strValue;

	//��ʼ��ȡmms����
	pElement = pRootElement->FirstChildElement("mmsdictonary");
	if(pElement != NULL)
	{
		TiXmlHandle hXmlRoot = TiXmlHandle(pElement);
		for(pElement = hXmlRoot.FirstChild().Element(); pElement; pElement = pElement->NextSiblingElement())
		{
			strkey = pElement->Attribute("keyword");
			strValue = pElement->GetText();
//			utf8togb2312.TranslateUTF_8ToGB(strValue.c_str(),strValue.size());
            m_mapMmsDictionary.insert(MAP_WSDICTIONARY::value_type(strkey, strValue));
		}
	}
	//��ʼ��ȡiec61850����
	pElement = pRootElement->FirstChildElement("iec61850dictonary");
	if(pElement != NULL)
	{
		TiXmlHandle hXmlRoot = TiXmlHandle(pElement);
		for(pElement = hXmlRoot.FirstChild().Element(); pElement; pElement = pElement->NextSiblingElement())
		{
			strkey = pElement->Attribute("keyword");
			strValue = pElement->GetText();
            m_mapIec61850Dictionary.insert(MAP_WSDICTIONARY::value_type(strkey, strValue));
		}
	}
	//��ʼ��ȡ���緭�벿��
	pElement = pRootElement->FirstChildElement("ethernetdictonary");
	if(pElement != NULL)
	{
		TiXmlHandle hXmlRoot = TiXmlHandle(pElement);
		for(pElement = hXmlRoot.FirstChild().Element(); pElement; pElement = pElement->NextSiblingElement())
		{
			strkey = pElement->Attribute("keyword");
			strValue = pElement->GetText();
            m_mapEthDictionary.insert(MAP_WSDICTIONARY::value_type(strkey, strValue));
		}
	}
	//��ʼ��ȡͨ�ò���
	pElement = pRootElement->FirstChildElement("commondictonary");
	if(pElement != NULL)
	{
		TiXmlHandle hXmlRoot = TiXmlHandle(pElement);
		for(pElement = hXmlRoot.FirstChild().Element(); pElement; pElement = pElement->NextSiblingElement())
		{
			strkey = pElement->Attribute("keyword");
			strValue = pElement->GetText();
            m_mapCommDictionary.insert(MAP_WSDICTIONARY::value_type(strkey, strValue));
		}
	}
	//��ȡmms��������
	pElement = pRootElement->FirstChildElement("mmsdatatype");
	int nkEY;
    string strTemp;
	if(pElement != NULL)
	{
		TiXmlHandle hXmlRoot = TiXmlHandle(pElement);
		for(pElement = hXmlRoot.FirstChild().Element(); pElement; pElement = pElement->NextSiblingElement())
		{
			strTemp = pElement->Attribute("keyword");
            nkEY = atoi(strTemp.c_str());
			strValue = pElement->GetText();
            m_mapEnumMmsdatatype.insert(MAP_WSENUMDICTIONARY::value_type(nkEY,strValue));
		}
	}
	//��ȡmms��������
	pElement = pRootElement->FirstChildElement("mmssrverrorclass");
	if(pElement != NULL)
	{
		TiXmlHandle hXmlRoot = TiXmlHandle(pElement);
		for(pElement = hXmlRoot.FirstChild().Element(); pElement; pElement = pElement->NextSiblingElement())
		{
			strTemp = pElement->Attribute("keyword");
            nkEY = atoi(strTemp.c_str());
			strValue = pElement->GetText();
            m_mapEnumMmssrverrorclass.insert(MAP_WSENUMDICTIONARY::value_type(nkEY,strValue));
		}
	}
	//���ݷ��ʴ�������
	pElement = pRootElement->FirstChildElement("mmsdataaccesserror");
	if(pElement != NULL)
	{
		TiXmlHandle hXmlRoot = TiXmlHandle(pElement);
		for(pElement = hXmlRoot.FirstChild().Element(); pElement; pElement = pElement->NextSiblingElement())
		{
			strTemp = pElement->Attribute("keyword");
            nkEY = atoi(strTemp.c_str());
			strValue = pElement->GetText();
            m_mapEnumMmsdataaccesserror.insert(MAP_WSENUMDICTIONARY::value_type(nkEY,strValue));
		}
	}
	//rejectpduԭ��
	pElement = pRootElement->FirstChildElement("mmsrejectpdu");
	if(pElement != NULL)
	{
		TiXmlHandle hXmlRoot = TiXmlHandle(pElement);
		for(pElement = hXmlRoot.FirstChild().Element(); pElement; pElement = pElement->NextSiblingElement())
		{
			strTemp = pElement->Attribute("keyword");
            nkEY = atoi(strTemp.c_str());
			strValue = pElement->GetText();
            m_mapEnumMmsRejectPdu.insert(MAP_WSENUMDICTIONARY::value_type(nkEY,strValue));
		}
	}
	//acsi��������
	pElement = pRootElement->FirstChildElement("acsisrvtyedictonary");
	if(pElement != NULL)
	{
		TiXmlHandle hXmlRoot = TiXmlHandle(pElement);
		for(pElement = hXmlRoot.FirstChild().Element(); pElement; pElement = pElement->NextSiblingElement())
		{
			strTemp = pElement->Attribute("keyword");
            nkEY = atoi(strTemp.c_str());
			strValue = pElement->GetText();
            m_mapEnumAcsiType.insert(MAP_WSENUMDICTIONARY::value_type(nkEY,strValue));
		}
	}
	//acsi�������
	pElement = pRootElement->FirstChildElement("acsisrverrorclass");
	if(pElement != NULL)
	{
		TiXmlHandle hXmlRoot = TiXmlHandle(pElement);
		for(pElement = hXmlRoot.FirstChild().Element(); pElement; pElement = pElement->NextSiblingElement())
		{
			strTemp = pElement->Attribute("keyword");
            nkEY = atoi(strTemp.c_str());
			strValue = pElement->GetText();
            m_mapEnumAcsiError.insert(MAP_WSENUMDICTIONARY::value_type(nkEY,strValue));
		}
	}
	//goose��������
	pElement = pRootElement->FirstChildElement("gooseerrorclass");
	if(pElement != NULL)
	{
		TiXmlHandle hXmlRoot = TiXmlHandle(pElement);
		for(pElement = hXmlRoot.FirstChild().Element(); pElement; pElement = pElement->NextSiblingElement())
		{
			strTemp = pElement->Attribute("keyword");
            nkEY = atoi(strTemp.c_str());
			strValue = pElement->GetText();
            m_mapEnumGooseError.insert(MAP_WSENUMDICTIONARY::value_type(nkEY,strValue));
		}
	}
	//������������
	pElement = pRootElement->FirstChildElement("smverrorclass");
	if(pElement != NULL)
	{
		TiXmlHandle hXmlRoot = TiXmlHandle(pElement);
		for(pElement = hXmlRoot.FirstChild().Element(); pElement; pElement = pElement->NextSiblingElement())
		{
			strTemp = pElement->Attribute("keyword");
            nkEY = atoi(strTemp.c_str());
			strValue = pElement->GetText();
            m_mapEnumSmvError.insert(MAP_WSENUMDICTIONARY::value_type(nkEY,strValue));
		}
	}
	//�˿ڸ澯
	pElement = pRootElement->FirstChildElement("portalarmclass");
	if(pElement != NULL)
	{
		TiXmlHandle hXmlRoot = TiXmlHandle(pElement);
		for(pElement = hXmlRoot.FirstChild().Element(); pElement; pElement = pElement->NextSiblingElement())
		{
			strTemp = pElement->Attribute("keyword");
            nkEY = atoi(strTemp.c_str());
			strValue = pElement->GetText();
            m_mapEnumPortAlarm.insert(MAP_WSENUMDICTIONARY::value_type(nkEY,strValue));
		}
	}
	//ptp
	pElement = pRootElement->FirstChildElement("ptpeventclass");
	if(pElement != NULL)
	{
		TiXmlHandle hXmlRoot = TiXmlHandle(pElement);
		for(pElement = hXmlRoot.FirstChild().Element(); pElement; pElement = pElement->NextSiblingElement())
		{
			strTemp = pElement->Attribute("keyword");
            nkEY = atoi(strTemp.c_str());
			strValue = pElement->GetText();
            m_mapEnumPTPEventDesc.insert(MAP_WSENUMDICTIONARY::value_type(nkEY,strValue));
		}
	}
	//ptp
	pElement = pRootElement->FirstChildElement("ptperrorclass");
	if(pElement != NULL)
	{
		TiXmlHandle hXmlRoot = TiXmlHandle(pElement);
		for(pElement = hXmlRoot.FirstChild().Element(); pElement; pElement = pElement->NextSiblingElement())
		{
			strTemp = pElement->Attribute("keyword");
            nkEY = atoi(strTemp.c_str());
			strValue = pElement->GetText();
            m_mapEnumPTPErrDesc.insert(MAP_WSENUMDICTIONARY::value_type(nkEY,strValue));
		}
	}
	//�˳�
	tinyxmlparser.Clear();
	return 0;
}
/*�ͷ���Դ*/
int CKeywords61850tranformer::Uninitialize()
{
	if(!m_bLoadFileOK)
		return 0;
	m_mapCommDictionary.clear();
	m_mapEthDictionary.clear();
	m_mapIec61850Dictionary.clear();
	m_mapMmsDictionary.clear();

	m_mapEnumMmsdatatype.clear();
	m_mapEnumMmssrverrorclass.clear();
	m_mapEnumMmsdataaccesserror.clear();
	m_mapEnumMmsRejectPdu.clear();

	m_mapEnumAcsiType.clear();
	m_mapEnumAcsiError.clear();
	m_mapEnumPortAlarm.clear();
	m_mapEnumPTPEventDesc.clear();
	return 0;
}
/*����MM�����������ͷ�������*/
/*
* @brief	GetMmsDataTypeDesc        ����MM�����������ͷ�������                   
* @param 	int nMMSDatatype
* @return	CString
* @notes	��
* @sample	��
*/
string CKeywords61850tranformer::GetMmsDataTypeDesc(int nMMSDatatype)
{
    string strDesc;
    char buf[255];
    sprintf(buf, "%d", nMMSDatatype);
    strDesc = buf;
	if(!m_bLoadFileOK)
	{
		return strDesc;
	}
	std::string strResult = m_mapEnumMmsdatatype[nMMSDatatype];
	if(strResult.size() > 0)
	{
        strDesc = strResult;
	}
	return strDesc;
}
/*����MMS������������������*/
/*
* @brief	Get_C_MmsServiceType_Chinese   ����MM�����������ͷ�������                   
* @param 	CString strMmsServiceType
* @return	CString
* @notes	��
* @sample	��
*/
string CKeywords61850tranformer::Get_C_MmsServiceType_ChineseDesc(string strMmsServiceType)
{
    string cstrResult = strMmsServiceType;
	if(!m_bLoadFileOK)
	{
		return cstrResult;
	}

    std::string strResult = m_mapMmsDictionary[strMmsServiceType];
	if(strResult.size() != 0)
	{
        cstrResult = strResult;
	}
	return cstrResult;
}
/*����PDU��������������*/
/*
* @brief	Get_PduType_ChineseDesc   ����PDU��������������                  
* @param 	CString strPduType
* @return	CString
* @notes	��
* @sample	��
*/
string CKeywords61850tranformer::Get_PduType_ChineseDesc(string strPduType)
{
    string strResult; //= strPduType;
    if(strcmp(strPduType.c_str(),"Request")==0)
	{
        strResult = "����";
	}
    else if(strcmp(strPduType.c_str(),"Response")==0)
	{
        strResult = "�ظ�";
	}
// 	else if(strstr(strPduType,"unconfirmed"))
// 	{
// 		strResult.Format("unconfirmed");
// 	}
    else if(strcmp(strPduType.c_str(),"ErrorPDU")==0)
	{
        strResult = "����";
	}
    else if(strcmp(strPduType.c_str(),"rejectPDU")==0)
	{
        strResult = "�ܾ�";
	}
	else
	{
		;
	}
	return strResult;
}
/*���ݴ���ŵõ�MMS���������ű�������ķ���*/
/*
* @brief	Get_MMS_SRV_ERROR_CLASS_ChineseDesc   ���ݴ���ŵõ�MMS���������ű�������ķ���                  
* @param 	int nErrorClassCode
* @notes	��
* @sample	��
*/
string CKeywords61850tranformer::Get_MMS_SRV_ERROR_CLASS_ChineseDesc(int nErrorClassCode)
{
    string strDesc;
    char buf[255];
    sprintf(buf,"%d",nErrorClassCode);
    strDesc = buf;
	if(!m_bLoadFileOK)
	{
		return strDesc;
	}
	std::string strResult = m_mapEnumMmssrverrorclass[nErrorClassCode];
	if(strResult.size() > 0)
	{
        strDesc = strResult;
	}
	return strDesc;
}
/*���ݴ���ŵõ�MMS���ݷ��ʺű�������ķ���*/
/*
* @brief	Get_MMS_DATA_ACCESS_ERROR_ChineseDesc   ���ݴ���ŵõ�MMS���ݷ��ʺű�������ķ���               
* @param 	int nErrorClassCode
* @notes	��
* @sample	��
*/
string CKeywords61850tranformer::Get_MMS_DATA_ACCESS_ERROR_ChineseDesc(int nErrorClassCode)
{
    string strDesc;
    strDesc.clear();
	if(!m_bLoadFileOK)
	{
		return strDesc;
	}
	std::string strResult = m_mapEnumMmsdataaccesserror[nErrorClassCode];
	if(strResult.size() > 0)
	{
        strDesc = strResult;
	}
	return strDesc;
}
/*���ݴ���ŵõ�rejectpdu�����ķ���*/
/*
* @brief	GET_REJECT_PDU_ChineseDesc   ���ݴ���ŵõ�rejectpdu�����ķ���             
* @param 	int nErrorClassCode
* @notes	��
* @sample	��
*/
string CKeywords61850tranformer::GET_REJECT_PDU_ChineseDesc(int nErrorClassCode)
{
    string strDesc;
    char buf[255];
    sprintf(buf,"%d",nErrorClassCode);
    strDesc = buf;
	if(!m_bLoadFileOK)
	{
		return strDesc;
	}
	std::string strResult = m_mapEnumMmsRejectPdu[nErrorClassCode];
	if(strResult.size() > 0)
	{
        strDesc = strResult;
	}
	return strDesc;
}
/*����acsi�����ŵõ���������*/
string CKeywords61850tranformer::Get_Acsi_Type_Desc(int nAcsiType)
{
    string strDesc;
    char buf[255];
    sprintf(buf,"%d",nAcsiType);
    strDesc = buf;
	if(!m_bLoadFileOK)
	{
		return strDesc;
	}
	std::string strResult = m_mapEnumAcsiType[nAcsiType];
	if(strResult.size() > 0)
	{
        strDesc = strResult;
	}
	return strDesc;
}
/*����acsi�����ŵõ���������*/
string CKeywords61850tranformer::Get_Acsi_Error_Desc(int nErrorCode)
{
    string strDesc;
    strDesc.clear();
	if(!m_bLoadFileOK)
	{
		return strDesc;
	}
	std::string strResult = m_mapEnumAcsiError[nErrorCode];
	if(strResult.size() > 0)
	{
        strDesc = strResult;
	}
	return strDesc;
}
/*���ݴ���ŵõ�goose����������������*/
string CKeywords61850tranformer::Get_GOOSE_Error_ChineseDesc(int nErrorClassCode)
{
    string strDesc;
    char buf[255];
    sprintf(buf,"%d",nErrorClassCode);
    strDesc = buf;
	std::string strResult = m_mapEnumGooseError[nErrorClassCode];
	if(strResult.size() > 0)
	{
        strDesc = strResult;
	}
	return strDesc;
}
/*���ݴ���ŵõ�goose����������������*/
string CKeywords61850tranformer::Get_PTP_Error_ChineseDesc(int nErrorClassCode)
{
    string strDesc;
    char buf[255];
    sprintf(buf,"%d",nErrorClassCode);
    strDesc = buf;
	std::string strResult = m_mapEnumPTPErrDesc[nErrorClassCode];
	if(strResult.size() > 0)
	{
        strDesc = strResult;
	}
	return strDesc;
}
/*���ݴ���ŵõ���������������������*/
string CKeywords61850tranformer::Get_SMV_Error_ChineseDesc(int nErrorClassCode)
{
    string strDesc;
    char buf[255];
    sprintf(buf,"%d",nErrorClassCode);
    strDesc = nErrorClassCode;
	std::string strResult = m_mapEnumSmvError[nErrorClassCode];
	if(strResult.size() > 0)
	{
        strDesc = strResult;
	}
    return strDesc;
}
/**���ݸ澯�Ż�ȡ�澯����**/
string CKeywords61850tranformer::Get_Port_Alarm_ChineseDesc(int nErrorClassCode)
{
    string strDesc;
    char buf[255];
    sprintf(buf,"%d",nErrorClassCode);
    strDesc = buf;
	std::string strResult = m_mapEnumPortAlarm[nErrorClassCode];
	if(strResult.size() > 0)
	{
        strDesc = strResult;
	}
	return strDesc;
}
/*���ݱ�ŵõ�1588�¼�����*/
string CKeywords61850tranformer::GET_PTP1588_EVENT_ChineseDesc(int nEventType)
{
    string strDesc;
    char buf[255];
    sprintf(buf,"%d",nEventType);
    strDesc = buf;
	std::string strResult = m_mapEnumPTPEventDesc[nEventType];
	if(strResult.size() > 0)
	{
        strDesc = strResult;
	}
	return strDesc;
}
