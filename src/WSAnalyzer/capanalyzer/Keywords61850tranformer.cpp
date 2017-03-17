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
/*根据配置文件名称初始化映射表*/
/*
* @brief	Initialize				根据配置文件名称初始化映射表                 
* @param 	CString strCfgFileName  配置文件路径
* @return	CString
* @notes	无
* @sample	无
*/
int CKeywords61850tranformer::Initialize(string strCfgFileName)
{
	//参数判断
    if(strCfgFileName.empty())
		return -1;
	//加载字典文档
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

	//开始读取mms部分
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
	//开始读取iec61850部分
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
	//开始读取网络翻译部分
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
	//开始读取通用部分
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
	//读取mms数据类型
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
	//读取mms错误类型
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
	//数据访问错误描述
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
	//rejectpdu原因
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
	//acsi服务类型
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
	//acsi错误服务
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
	//goose错误描述
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
	//采样错误描述
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
	//端口告警
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
	//退出
	tinyxmlparser.Clear();
	return 0;
}
/*释放资源*/
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
/*根据MM数据类型类型反查描述*/
/*
* @brief	GetMmsDataTypeDesc        根据MM数据类型类型反查描述                   
* @param 	int nMMSDatatype
* @return	CString
* @notes	无
* @sample	无
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
/*翻译MMS服务类型描述到中文*/
/*
* @brief	Get_C_MmsServiceType_Chinese   根据MM数据类型类型反查描述                   
* @param 	CString strMmsServiceType
* @return	CString
* @notes	无
* @sample	无
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
/*翻译PDU类型描述到中文*/
/*
* @brief	Get_PduType_ChineseDesc   翻译PDU类型描述到中文                  
* @param 	CString strPduType
* @return	CString
* @notes	无
* @sample	无
*/
string CKeywords61850tranformer::Get_PduType_ChineseDesc(string strPduType)
{
    string strResult; //= strPduType;
    if(strcmp(strPduType.c_str(),"Request")==0)
	{
        strResult = "请求";
	}
    else if(strcmp(strPduType.c_str(),"Response")==0)
	{
        strResult = "回复";
	}
// 	else if(strstr(strPduType,"unconfirmed"))
// 	{
// 		strResult.Format("unconfirmed");
// 	}
    else if(strcmp(strPduType.c_str(),"ErrorPDU")==0)
	{
        strResult = "错误";
	}
    else if(strcmp(strPduType.c_str(),"rejectPDU")==0)
	{
        strResult = "拒绝";
	}
	else
	{
		;
	}
	return strResult;
}
/*根据错误号得到MMS服务类错误号编码的中文翻译*/
/*
* @brief	Get_MMS_SRV_ERROR_CLASS_ChineseDesc   根据错误号得到MMS服务类错误号编码的中文翻译                  
* @param 	int nErrorClassCode
* @notes	无
* @sample	无
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
/*根据错误号得到MMS数据访问号编码的中文翻译*/
/*
* @brief	Get_MMS_DATA_ACCESS_ERROR_ChineseDesc   根据错误号得到MMS数据访问号编码的中文翻译               
* @param 	int nErrorClassCode
* @notes	无
* @sample	无
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
/*根据错误号得到rejectpdu的中文翻译*/
/*
* @brief	GET_REJECT_PDU_ChineseDesc   根据错误号得到rejectpdu的中文翻译             
* @param 	int nErrorClassCode
* @notes	无
* @sample	无
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
/*根据acsi服务编号得到服务名称*/
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
/*根据acsi错误编号得到错误描述*/
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
/*根据错误号得到goose分析错误中文描述*/
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
/*根据错误号得到goose分析错误中文描述*/
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
/*根据错误号得到采样分析错误中文描述*/
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
/**根据告警号获取告警描述**/
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
/*根据编号得到1588事件描述*/
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
