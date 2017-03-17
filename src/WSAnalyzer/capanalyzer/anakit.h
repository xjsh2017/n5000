#ifndef ANAKIT_H
#define ANAKIT_H

#include "LibpCapFileMnger.h"
#include "PackageCovertWrapper.h"
#include "Keywords61850tranformer.h"
#include "CapTransformer.h"
#include "ScanDissectPacketer.h"

class AnaKit
{
public:
    AnaKit();
	~AnaKit();

    void Initialize();

    string m_capFileName;

    //pcap文件操作类
    CLibpCapFileMnger m_libpcapfilemnger;
    //用于数据包的解析
    //CScanDissectPacketer m_ScanDissectPacketer;

    //解析包数据管理类
    CapPackagesMnger m_cappackagesmnger;

    //关键字转换器,中文转英文,或者英文转中文,类似于字符表
    //CKeywords61850tranformer  m_KeyWords61850tranformer;
    //用wpcap加载文件后的转换器
    CPackageCovertWrapper*    m_pPackageCovertWrapper;
    CCapTransformer*		  m_pcatransformer;          //cap文件生成的txt文件解析器

    //初始化关键字解析器
    //int IniKeyWords61850tranformer();

    //初始化网络包解析器
    int InitScanDissectPacketer();

    //初始化SCD文件解析器
    int InitScdParser();

	//原始报文存储
    PACKET_STRUCT m_pPacket;

	//tcp及以下层次协议解析
    CPackageCovertWrapper m_pkgConvertWrp;

    //打开pcap文件并解析
    bool OpenCapFileAndParse(string fileName);
    //分析所有链接
    int AnalyzeAllConnections();
    //分析ACSI链路
    int AnalyzeAllConnections_Mms(CAPCONNECTINFO* pConnection);
    //分析Gs链路
    int AnalyzeAllConnections_Gs(CAPCONNECTINFO* pConnection);
    //分析Gs链路
    //int AnalyzeAllConnection_Ptp(CAPCONNECTINFO* pConnection);
    //分析smv链路
    int AnalyzeAllConnections_Smv(CAPCONNECTINFO* pConnection);
    //解析一帧报文，返回报文解析结果
    //nseq  该帧报文在报文包中的序号
    CAPMSGGININFO * LoadePacketMsg(int nseq, TS_PCAP_PKTHDR* pkthdr,char *pkt);

};

#endif // ANAKIT_H
