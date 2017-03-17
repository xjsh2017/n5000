#pragma once
#include <map>
#include <string>
#include "xjlib.h"
/********************************************************************
*	filename: 	CKeywords61850tranformer
*	author:		yinzhehong
*	created:	2010/12/1
*	purpose:  	61850关键字转换,英文转换成中文,创建一个map表，映射关系从配置文件中读取    
*   version:    1.0
*********************************************************************/
typedef std::map <std::string, std::string>	MAP_WSDICTIONARY;      //字符字典
typedef std::map <int, std::string>	MAP_WSENUMDICTIONARY;          //数据枚举字典
class CKeywords61850tranformer
{
private:
    static CKeywords61850tranformer *keywords61850Instance;
public:
    static CKeywords61850tranformer *getInstance()
    {
        if(keywords61850Instance == NULL)
        {
            keywords61850Instance = new CKeywords61850tranformer();
        }
        return keywords61850Instance;
    }
public:
	CKeywords61850tranformer(void);
	~CKeywords61850tranformer(void);
public:
	/*根据配置文件名称初始化映射表*/
    int Initialize(string strCfgFileName);
	/*释放资源*/
	int Uninitialize();
public:
	/*获取mms数据类型描述*/
    string GetMmsDataTypeDesc(int nMMSDatatype);
	/*获取mms服务中文描述*/
    string Get_C_MmsServiceType_ChineseDesc(string strMmsServiceType);
	/*获取pdu服务中文描述*/
    string Get_PduType_ChineseDesc(string strPduType);
	/*根据错误号得到MMS服务类错误号编码的中文翻译*/
    string Get_MMS_SRV_ERROR_CLASS_ChineseDesc(int nErrorClassCode);
	/*根据错误号得到MMS数据访问号编码的中文翻译*/
    string Get_MMS_DATA_ACCESS_ERROR_ChineseDesc(int nErrorClassCode);
	/*根据错误号得到rejectpdu编码的中文翻译*/
    string GET_REJECT_PDU_ChineseDesc(int nErrorClassCode);
	/*根据编号号得到1588事件描述*/
    string GET_PTP1588_EVENT_ChineseDesc(int nEventType);

	/*根据acsi服务编号得到服务名称*/
    string Get_Acsi_Type_Desc(int nAcsiType);
	/*根据acsi错误编号得到错误描述*/
    string Get_Acsi_Error_Desc(int nErrorCode);
public:
	/*根据goose错误号得到错误描述*/
    string Get_GOOSE_Error_ChineseDesc(int nErrorClassCode);
	/*根据采样错误号得到错误描述*/
    string Get_SMV_Error_ChineseDesc(int nErrorClassCode);
	/*根据采样错误号得到错误描述*/
    string Get_PTP_Error_ChineseDesc(int nErrorClassCode);
	
	/**根据告警号获取告警描述**/
    string Get_Port_Alarm_ChineseDesc(int nErrorClassCode);
public:
	MAP_WSDICTIONARY m_mapMmsDictionary;     //mms的数据字典
	MAP_WSDICTIONARY m_mapIec61850Dictionary;//61850的数据字典
	MAP_WSDICTIONARY m_mapEthDictionary;	 //网络报文数据字典
	MAP_WSDICTIONARY m_mapCommDictionary;	 //通用数据字典,可作为界面使用,比如表格的列名称等,暂未用

	MAP_WSENUMDICTIONARY m_mapEnumMmsdatatype;       //mms类型描述
	MAP_WSENUMDICTIONARY m_mapEnumMmssrverrorclass;  //mms错误类型
	MAP_WSENUMDICTIONARY m_mapEnumMmsdataaccesserror;//mms数据访问错误
	MAP_WSENUMDICTIONARY m_mapEnumMmsRejectPdu;      //rejectpdu返回枚举
	MAP_WSENUMDICTIONARY m_mapEnumAcsiType;          //acsi服务类型枚举
	MAP_WSENUMDICTIONARY m_mapEnumAcsiError;         //acsi错误类型枚举
	MAP_WSENUMDICTIONARY m_mapEnumGooseError;       //goose错误描述
	MAP_WSENUMDICTIONARY m_mapEnumSmvError;         //采样错误描述
	MAP_WSENUMDICTIONARY m_mapEnumPortAlarm;         //端口告警事件
	MAP_WSENUMDICTIONARY m_mapEnumPTPEventDesc;      //端口告警事件
	MAP_WSENUMDICTIONARY m_mapEnumPTPErrDesc;      //端口告警事件
private:
	BOOL m_bLoadFileOK;


};


