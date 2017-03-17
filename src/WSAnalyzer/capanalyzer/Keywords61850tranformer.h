#pragma once
#include <map>
#include <string>
#include "xjlib.h"
/********************************************************************
*	filename: 	CKeywords61850tranformer
*	author:		yinzhehong
*	created:	2010/12/1
*	purpose:  	61850�ؼ���ת��,Ӣ��ת��������,����һ��map��ӳ���ϵ�������ļ��ж�ȡ    
*   version:    1.0
*********************************************************************/
typedef std::map <std::string, std::string>	MAP_WSDICTIONARY;      //�ַ��ֵ�
typedef std::map <int, std::string>	MAP_WSENUMDICTIONARY;          //����ö���ֵ�
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
	/*���������ļ����Ƴ�ʼ��ӳ���*/
    int Initialize(string strCfgFileName);
	/*�ͷ���Դ*/
	int Uninitialize();
public:
	/*��ȡmms������������*/
    string GetMmsDataTypeDesc(int nMMSDatatype);
	/*��ȡmms������������*/
    string Get_C_MmsServiceType_ChineseDesc(string strMmsServiceType);
	/*��ȡpdu������������*/
    string Get_PduType_ChineseDesc(string strPduType);
	/*���ݴ���ŵõ�MMS���������ű�������ķ���*/
    string Get_MMS_SRV_ERROR_CLASS_ChineseDesc(int nErrorClassCode);
	/*���ݴ���ŵõ�MMS���ݷ��ʺű�������ķ���*/
    string Get_MMS_DATA_ACCESS_ERROR_ChineseDesc(int nErrorClassCode);
	/*���ݴ���ŵõ�rejectpdu��������ķ���*/
    string GET_REJECT_PDU_ChineseDesc(int nErrorClassCode);
	/*���ݱ�źŵõ�1588�¼�����*/
    string GET_PTP1588_EVENT_ChineseDesc(int nEventType);

	/*����acsi�����ŵõ���������*/
    string Get_Acsi_Type_Desc(int nAcsiType);
	/*����acsi�����ŵõ���������*/
    string Get_Acsi_Error_Desc(int nErrorCode);
public:
	/*����goose����ŵõ���������*/
    string Get_GOOSE_Error_ChineseDesc(int nErrorClassCode);
	/*���ݲ�������ŵõ���������*/
    string Get_SMV_Error_ChineseDesc(int nErrorClassCode);
	/*���ݲ�������ŵõ���������*/
    string Get_PTP_Error_ChineseDesc(int nErrorClassCode);
	
	/**���ݸ澯�Ż�ȡ�澯����**/
    string Get_Port_Alarm_ChineseDesc(int nErrorClassCode);
public:
	MAP_WSDICTIONARY m_mapMmsDictionary;     //mms�������ֵ�
	MAP_WSDICTIONARY m_mapIec61850Dictionary;//61850�������ֵ�
	MAP_WSDICTIONARY m_mapEthDictionary;	 //���籨�������ֵ�
	MAP_WSDICTIONARY m_mapCommDictionary;	 //ͨ�������ֵ�,����Ϊ����ʹ��,������������Ƶ�,��δ��

	MAP_WSENUMDICTIONARY m_mapEnumMmsdatatype;       //mms��������
	MAP_WSENUMDICTIONARY m_mapEnumMmssrverrorclass;  //mms��������
	MAP_WSENUMDICTIONARY m_mapEnumMmsdataaccesserror;//mms���ݷ��ʴ���
	MAP_WSENUMDICTIONARY m_mapEnumMmsRejectPdu;      //rejectpdu����ö��
	MAP_WSENUMDICTIONARY m_mapEnumAcsiType;          //acsi��������ö��
	MAP_WSENUMDICTIONARY m_mapEnumAcsiError;         //acsi��������ö��
	MAP_WSENUMDICTIONARY m_mapEnumGooseError;       //goose��������
	MAP_WSENUMDICTIONARY m_mapEnumSmvError;         //������������
	MAP_WSENUMDICTIONARY m_mapEnumPortAlarm;         //�˿ڸ澯�¼�
	MAP_WSENUMDICTIONARY m_mapEnumPTPEventDesc;      //�˿ڸ澯�¼�
	MAP_WSENUMDICTIONARY m_mapEnumPTPErrDesc;      //�˿ڸ澯�¼�
private:
	BOOL m_bLoadFileOK;


};


