#pragma once
//#include "../wsconfig/WSSysconfig.h"
//#include "../wsconstant/wsconstant.h"
#include "capanalyzerstruct.h"
#include <map>
#include <string>
/**
 * @brief       class name: CCapPackageStatic
 * @use			pcap���ݰ���·������ͳ��
					 1��ȫ���ݰ�����ģʽ����ȡ���������ݰ���ͳһ����ͳ��
					 2�������ͳ��ģʽ��  ��ȡһ֡ͳ��һ֡
 * @author      �����
 * @version     ver1.0
 * @date        2010/8/23
 * example
 * @code
 *
 *
 *
 * @endcode
 */
typedef std::map <std::string, CAPCONNECTINFO*>	MAP_MAC_CAPCONNECTINFO;      //MAC��ַ��·map��
class CCapPackageStatic
{
public:
	CCapPackageStatic(void);
	~CCapPackageStatic(void);
public:
	
	/*****ȫ����ͳ����·��Ϣ*****/
	//int StaticAllPakcageLinks(CAPPARSERINFO*  pcapparserninfo, MAP_CAPMSGINFO*  pmapcapmsginfo,MAP_CAPCONNECTINFO* pmapcapconnectionfo);
	/*****��֡������·ͳ��*****/
	CAPCONNECTINFO* StaticPackageLink(CAPPARSERINFO*  pcapparserninfo,CAPMSGGININFO * pCapMsgGinInfo,MAP_CAPCONNECTINFO* pmapcapconnectionfo);
	/*****ͳ�Ʊ������͸�������Ե�֡����*****/
    int StaticPakcageAppNum(CAPPARSERINFO*  pcapparserninfo, CAPMSGGININFO * pCapMsgGinInfo);

private:
	/*****��ǰ�����Ƿ����*****/
	CAPCONNECTINFO* PackageLinkExist(CAPMSGGININFO * pCapMsgGinInfo);//const char* csource1,const char* csource2,int nappid,int nethertype);
	/*****����������*****/
	CAPCONNECTINFO* CreatNewLink(int nIndex,CAPMSGGININFO* pCapMsgGinInfo);
	/*****��ǰ�����Ƿ����******/
	int StaticExistPackage(CAPMSGGININFO* pCapMsgGinInfo,CAPCONNECTINFO * pCapconnectinfo);
private:
	CAPPARSERINFO*		m_pcapparserninfo;			//cap�ļ��ܽṹ
	MAP_CAPMSGINFO*		m_pmapcapmsginfo;			//������Ϣmap��ָ��
	MAP_CAPCONNECTINFO* m_pmapcapconnectionfo;		//��������map��ָ��
//	MAP_CAPCONNECTINFO  m_mapappid_capconnectionfo; //appid�����ӵ�ӳ��
	MAP_MAC_CAPCONNECTINFO m_mapappid_mac_capconnectionfo; //appid�����ӵ�ӳ��
	MAP_MAC_CAPCONNECTINFO m_map_mac_capconnectionfo; //appid�����ӵ�ӳ��
	MAP_MAC_CAPCONNECTINFO m_map_mac_reverse_capconnectionfo;//����mac��ַ��

};

