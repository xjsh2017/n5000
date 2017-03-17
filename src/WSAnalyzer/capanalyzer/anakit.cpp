#pragma execution_character_set("UTF-8")
#include "time.h"
#include "stdio.h"

#include "const_scan.h"
#include "anakit.h"
#include "CapPackageStatic.h"

#include "CapTransformer.h"
#include "ScanDissectPacketer.h"
#include "wsconstant.h"
#include "wssysconfig.h"
#include "IEC61850Analyzer.h"

CCapTransformer  *g_capTransformer = NULL;  //cap文件生成的txt文件解析器
//CZipcWrapper              m_zicpWrapper;              //解压缩动态库封装
CScanDissectPacketer   *g_ScanDissectPacketer =NULL;      //用于数据包的解析
CapPackagesMnger ex_m_cappackagesmnger;

AnaKit::AnaKit()
{


}

AnaKit::~AnaKit()
{
    //做一些释放资源的动作 todo
}
void AnaKit::Initialize()
{

     if (g_capTransformer == NULL)
    {
        string strDllPathName;
        try
        {
            //加载网络包解析器动态库
            CWSSysconfig *wsSysconfig = CWSSysconfig::getInstance();
            strDllPathName = wsSysconfig->m_wsThirdPartSrc.cthdpartysrcrelative_path_re;
            strDllPathName.append("/");
            strDllPathName.append(wsSysconfig->m_wsThirdPartSrc.ccaptxtparserdllname);
            g_capTransformer = new CCapTransformer();
            if(g_capTransformer->Initialize(strDllPathName.c_str()) != 0)//传入动态库，加载动态库，导出各类函数
            {
                return;
            }
            else
            {
                ;
            }
        }
        catch (...)
        {
           ;
        }

    }
    InitScanDissectPacketer();
}

//初始化网络包解析器
int AnaKit::InitScanDissectPacketer()
{
    string strDllPathName;
    try
    {
        //加载网络包解析器动态库
        CWSSysconfig *wsSysconfig = CWSSysconfig::getInstance();
        strDllPathName = wsSysconfig->m_wsThirdPartSrc.cthdpartysrcrelative_path_re;
        strDllPathName.append("/");
        strDllPathName.append(wsSysconfig->m_wsThirdPartSrc.cscandissetpktdllname);
        g_ScanDissectPacketer = new CScanDissectPacketer();
        if(g_ScanDissectPacketer->Initialize(strDllPathName.c_str()) != 0)//传入动态库，加载动态库，导出各类函数
        {
            return -1;
        }
        else
        {
            ;
        }
        return 0;
    }
    catch (...)
    {
       ;
    }
    return -1;
}

CAPMSGGININFO * AnaKit::LoadePacketMsg(int nseq,TS_PCAP_PKTHDR* pkthdr,char *pkt)
{
    CAPMSGGININFO * pCapPackage = new CAPMSGGININFO;
    pCapPackage->nseq = nseq;                     //序号，从1开始编写

    //记录长度
    m_pPacket.nLen = pkthdr->caplen +sizeof(TS_PCAP_PKTHDR);
    //记忆起始指针
    m_pPacket.pPacket  = pkt;

    //记录原始报文
    pCapPackage->ncap_len = pkthdr->caplen;
    pCapPackage->nlen     = pkthdr->len;
    pCapPackage->nsourceinfo_length = m_pPacket.nLen;
    pCapPackage->csourceinfo = pkt; //直接赋指针，减少一次复制

    //制作结构, TCP报文链路分析、过程层的报文(SMV,GOOSE,1588)制作
    m_pkgConvertWrp.Make61850Struct_Pack2Msg(pCapPackage,&m_pPacket);

    //时标戳
    pCapPackage->nseconds_utc_tmstamp = pkthdr->ts.GmtTime;
    pCapPackage->nus_tmstamp          = pkthdr->ts.us;
    return pCapPackage;
}


/*
 * 返回 true 成功  false 失败
 * 本函数参考CapAnalyzerDoc.cpp中LoadCapFileByLibpCapFileMnger()函数
 */
bool AnaKit::OpenCapFileAndParse(string fileName)
{
    m_capFileName.append(fileName);

    TS_PCAP_PKTHDR pktheader;//报文头
    char           *pkt_data;//含报文头
    int nseq = 1;   //顺序编号
    CAPMSGGININFO  * pCapPackage = NULL;
    CCapPackageStatic    cappackagestic; //链路统计

    double   fFirstPackageT = 0.0f; //第一帧时间
    double   fPrePackageT = 0.0f;   //上一帧时间
    double   fPackageT =0.0f;

    char errbuf[500];
    int fileSize = m_libpcapfilemnger.Libpcap_open_offline(fileName.c_str(), errbuf);
    if(fileSize == 0)
        return false;

    unsigned int npktoffset = 0; //
    unsigned int nmmscount = 0;//MMS报文级数

    //开始逐帧读取数据
    while((pkt_data = m_libpcapfilemnger.Libpcap_next_cap(&pktheader,npktoffset))!= NULL)
    {
        //制作报文，TCP报文链路分析、过程层的报文(SMV,GOOSE,1588)制作
        pCapPackage = LoadePacketMsg(nseq, &pktheader, pkt_data);
        pCapPackage->npkt_offset_incapfile = npktoffset;

        //加入到总的报文队列
        m_cappackagesmnger.AddPacket2MnGrList(pCapPackage);

        //报文采样时间获取
        fPackageT = pktheader.ts.GmtTime + pktheader.ts.us/1000000.0;
        if(nseq == 1)//第一帧
        {
            fFirstPackageT = fPackageT;
            fPrePackageT   = fPackageT;//0.0f;
        }
        pCapPackage->ftime_delta = fPackageT - fPrePackageT;     //与上一帧的时间差
        pCapPackage->ftime_relative = fPackageT - fFirstPackageT;//与第一帧报文的时间差

        //制作界面显示用的结构-，只解析102端口报文,且心跳报文不解析
        if(pCapPackage->napptype == ETHER_TYPE_TCP  && (pCapPackage->ndst_port == 102 || pCapPackage->nsrc_port == 102)
            /*&& (pCapPackage->ncap_len > 90)*/)//关闭文件请求仅86字节，应答仅82字节 //66仅心跳 +7 COPT+TPKT的头
        {
            nmmscount ++;
            g_ScanDissectPacketer->I_XJ_DISSECT_MMS_PACKET(pCapPackage,nmmscount);
            //MMS报文分析
            if (pCapPackage->napptype == IEC61850_ETHER_TYPE_MMS || pCapPackage->napptype == ETHER_TYPE_COTP || pCapPackage->napptype == ETHER_TYPE_TPKT)//设定为MMS报文
            {
                g_capTransformer->I_XJ_PKT_STRUCT_MAKE_MMS_INFO_STRUCT(pCapPackage,TRUE);//制作MMS报文,格式错误报文不显示
                //释放资源
                //pApp->m_pcatransformer->I_ReleaseMMSInfoStruct((MMS_INFO_STRUCT *)pCapPackage->pparserdstruct);
                //pCapPackage->pparserdstruct = NULL;
            }
            g_ScanDissectPacketer->I_XJ_CLEANUP_PACKET(pCapPackage->pxj_dissect_pkt);//释放资源
            pCapPackage->pxj_dissect_pkt = NULL;
        }
        //统计一次
        cappackagestic.StaticPackageLink(&m_cappackagesmnger.m_capparserninfo,pCapPackage,&m_cappackagesmnger.m_mapcapconnectionfo);
        nseq++;//序号+1
        fPrePackageT = fPackageT;//记录上一帧时间
    }
    m_cappackagesmnger.m_capparserninfo.napppackages = nseq-1;//总帧数

    //文件的开始时间和结束时间
    int nsize = m_cappackagesmnger.m_pcapconnectinfoTotal.map_capmsginfo.size();
    if(nsize  > 1)
    {
        time_to_string(m_cappackagesmnger.m_pcapconnectinfoTotal.map_capmsginfo[0]->nseconds_utc_tmstamp,
                m_cappackagesmnger.m_pcapconnectinfoTotal.map_capmsginfo[0]->nus_tmstamp,
                m_cappackagesmnger.m_capparserninfo.cstarttimestamp);
        time_to_string(m_cappackagesmnger.m_pcapconnectinfoTotal.map_capmsginfo[nsize -1]->nseconds_utc_tmstamp,
                m_cappackagesmnger.m_pcapconnectinfoTotal.map_capmsginfo[nsize -1]->nus_tmstamp,
                m_cappackagesmnger.m_capparserninfo.cendtimestamp);
    }
    AnalyzeAllConnections();
    ex_m_cappackagesmnger = m_cappackagesmnger;
    return true;
}

/**
 * @brief AnaKit::AnalyzeAllConnections 分析所有链接
 * @return true 成功  false 失败
 */
int AnaKit::AnalyzeAllConnections()
{
    std::map <int, CAPCONNECTINFO* >::iterator iter;
    CAPCONNECTINFO* pConnection;
    for (iter = m_cappackagesmnger.m_mapcapconnectionfo.begin(); iter != m_cappackagesmnger.m_mapcapconnectionfo.end(); iter ++ )
    {
        pConnection = iter->second;
        if(pConnection == NULL)
            continue;
        if(pConnection->nconnectapptype == PROTOAPPTYPE_SMV92)
        {
            AnalyzeAllConnections_Smv(pConnection);
        }
        else if(pConnection->nconnectapptype == PROTOAPPTYPE_GOOSE)
        {
            AnalyzeAllConnections_Gs(pConnection);
        }
        else if(pConnection->nconnectapptype == PROTOAPPTYPE_MMS)//mms
        {
            //COPT list分析 add by yinzhehong 20120319
            m_pPackageCovertWrapper->COTP_LIST_ANALYZE(pConnection);
            //上下文分析
            AnalyzeAllConnections_Mms(pConnection);
        }

        //else if(pConnection->nconnectapptype == PROTOAPPTYPE_TIME1588)
        //{
            //AnalyzeAllConnection_Ptp(pConnection);
        //}

        m_cappackagesmnger.m_pcapconnectinfoTotal.nerrpackages += pConnection->nerrpackages;//总的错误包
    }
    return 0;
}

/**
 * @brief AnaKit::AnalyzeAllConnections_Smv 分析smv链路
 * @param pConnection
 * @return
 */
int AnaKit::AnalyzeAllConnections_Smv(CAPCONNECTINFO* pConnection)
{
    CWSSysconfig *wsSysconfig = CWSSysconfig::getInstance();
    CIEC61850Analyzer * iecAnalyzer = CIEC61850Analyzer::getInstance();
    std::map <int, CAPMSGGININFO* >::iterator iter;
    CAPMSGGININFO* pCapMsgGinInfo;
    MESSAGE_ERROR_INFO_ARRAY_STRUCT* pArrayStruct;
    int nAnaCount = 0;
    SMV_INFO_STRUCT* gStructTemp;
    for(iter = pConnection->map_capmsginfo.begin(); iter != pConnection->map_capmsginfo.end(); iter ++ )
    {
        pCapMsgGinInfo = iter->second;
        if(pCapMsgGinInfo == NULL)
            continue;
        if(pCapMsgGinInfo->pparserdstruct == NULL)//尚未解析出来
            continue;
        if(pCapMsgGinInfo->napptype != PROTOAPPTYPE_SMV92)//不是采样报文
            continue;
        if(pCapMsgGinInfo->berroranalyzedsmv)//已经进行过错误分析了
            continue;
        gStructTemp = (SMV_INFO_STRUCT*) pCapMsgGinInfo->pparserdstruct;//错误报文
        //if((gStructTemp->u_result & DISSECT_SMV_RESULT_OK)!=DISSECT_SMV_RESULT_OK)
        if((gStructTemp->u_result & DISSECT_SMV_PKTLEN_ERROR))
            continue;
        if(nAnaCount == 0)//重置链路
        {
            SNIFFER_APP* pSnifferApp = wsSysconfig->I_GET_IED_BYAPPID(pCapMsgGinInfo->napp_id);
            if(pSnifferApp == NULL || pSnifferApp->napptype!=PROTOAPPTYPE_SMV92)
            {
                SNIFFER_APP* pSnifferApp_Temp = new SNIFFER_APP;
                pSnifferApp_Temp->nappid = pCapMsgGinInfo->napp_id;
                memcpy(pSnifferApp_Temp->cmacaddress,pCapMsgGinInfo->csrc_mac,strlen(pCapMsgGinInfo->csrc_mac));
                gStructTemp = (SMV_INFO_STRUCT*) pCapMsgGinInfo->pparserdstruct;
                pSnifferApp_Temp->nappid = gStructTemp->n_app_id;//app_id
                //memcpy(pSnifferApp_Temp->cipaddress,pCapMsgGinInfo->csrc_ip,strlen(pCapMsgGinInfo->csrc_ip));
                if (gStructTemp->p_asdu_info_struct)
                {
                    memcpy(pSnifferApp_Temp->csvid,gStructTemp->p_asdu_info_struct->c_svID,strlen(gStructTemp->p_asdu_info_struct->c_svID));
                    memcpy(pSnifferApp_Temp->cdsname,gStructTemp->p_asdu_info_struct->c_dataset,strlen(gStructTemp->p_asdu_info_struct->c_dataset));
                    pSnifferApp_Temp->nconfrev = gStructTemp->p_asdu_info_struct->n_confRev;
                    pSnifferApp_Temp->ndssize = gStructTemp->p_asdu_info_struct->n_data_num;
                }

                iecAnalyzer->m_smvAnalyzer.I_SET_SNIFFER_APP(pSnifferApp_Temp);//
                delete pSnifferApp_Temp;
            }
            else
            {
                iecAnalyzer->m_smvAnalyzer.I_SET_SNIFFER_APP(pSnifferApp);
            }
        }
        pCapMsgGinInfo->pap_analyzed_info = (void*)iecAnalyzer->m_smvAnalyzer.I_SMVANALYZE((SMV_INFO_STRUCT*) pCapMsgGinInfo->pparserdstruct,1);
        pCapMsgGinInfo->berroranalyzedsmv = true;//置位
        if(pCapMsgGinInfo->pap_analyzed_info != NULL)
        {
            pArrayStruct = (MESSAGE_ERROR_INFO_ARRAY_STRUCT*) pCapMsgGinInfo->pap_analyzed_info;
            if(pArrayStruct->n_msg_err >= 1 && pCapMsgGinInfo->nAppConetentGood)//有错误内容
            {
                //将错误报文增加到队列中
                pConnection->map_capmsginfo_error.insert(std::map <int, CAPMSGGININFO*> :: value_type(pConnection->nerrpackages, pCapMsgGinInfo));
                pCapMsgGinInfo->nAppConetentGood = 0;//错误报文
                pConnection->nerrpackages ++;
            }
        }
        nAnaCount ++;
    }
    return 0;
}

/**
 * @brief AnaKit::AnalyzeAllConnections_Gs 分析Gs链路
 * @param pConnection
 * @return
 */
int AnaKit::AnalyzeAllConnections_Gs(CAPCONNECTINFO* pConnection)
{
    CWSSysconfig *wsSysconfig = CWSSysconfig::getInstance();
    CIEC61850Analyzer * iecAnalyzer = CIEC61850Analyzer::getInstance();
    std::map <int, CAPMSGGININFO* >::iterator iter;
    CAPMSGGININFO* pCapMsgGinInfo;
    MESSAGE_ERROR_INFO_ARRAY_STRUCT* pArrayStruct;
    int nAnaCount = 0;
    GOOSE_INFO_STRUCT* gStructTemp;
    for(iter = pConnection->map_capmsginfo.begin(); iter != pConnection->map_capmsginfo.end(); iter ++ )
    {
        pCapMsgGinInfo = iter->second;
        if(pCapMsgGinInfo == NULL)
            continue;
        if(pCapMsgGinInfo->pparserdstruct == NULL)//尚未解析出来
            continue;
        if(pCapMsgGinInfo->napptype != PROTOAPPTYPE_GOOSE)//不是GOOSE报文
            continue;
        if(pCapMsgGinInfo->berroranalyzedgoose)//已经进行过错误分析了
            continue;
        gStructTemp = (GOOSE_INFO_STRUCT*) pCapMsgGinInfo->pparserdstruct;
        //if((gStructTemp->u_result  &DISSECT_GOOSE_RESULT_OK)!=DISSECT_GOOSE_RESULT_OK)//格式异常 add by yzh 20130331
        if(gStructTemp->u_result&DISSECT_GOOSE_PKTLEN_ERROR)
            continue;
        if(nAnaCount == 0)//重置链路
        {
            SNIFFER_APP* pSnifferApp = wsSysconfig->I_GET_IED_BYAPPID(pCapMsgGinInfo->napp_id);
            if(pSnifferApp == NULL || pSnifferApp->napptype!=PROTOAPPTYPE_GOOSE)
            {
                SNIFFER_APP* pSnifferApp_Temp = new SNIFFER_APP;
                gStructTemp = (GOOSE_INFO_STRUCT*) pCapMsgGinInfo->pparserdstruct;
                pSnifferApp_Temp->nappid = gStructTemp->n_app_id;
                memcpy(pSnifferApp_Temp->cmacaddress,pCapMsgGinInfo->cdst_mac,strlen(pCapMsgGinInfo->cdst_mac));//gStructTemp->c_dest_mac,strlen(gStructTemp->c_dest_mac));
                memcpy(pSnifferApp_Temp->cgoid,gStructTemp->c_goID,strlen(gStructTemp->c_goID));
                memcpy(pSnifferApp_Temp->cdsname,gStructTemp->c_dataSet,strlen(gStructTemp->c_dataSet));
                memcpy(pSnifferApp_Temp->ccbname,gStructTemp->c_gocbRef,strlen(gStructTemp->c_gocbRef));//cbname
                pSnifferApp_Temp->nconfrev = gStructTemp->n_confRev;
                pSnifferApp_Temp->ndssize = gStructTemp->n_numDatSetEntries;
                iecAnalyzer->m_gooseAnalyzer.I_SET_SNIFFER_APP(pSnifferApp_Temp);
                delete pSnifferApp_Temp;
            }
            else
            {
                iecAnalyzer->m_gooseAnalyzer.I_SET_SNIFFER_APP(pSnifferApp);
            }
        }
        pCapMsgGinInfo->pap_analyzed_info = (void*)iecAnalyzer->m_gooseAnalyzer.I_GOOSEANALYZE((GOOSE_INFO_STRUCT*) pCapMsgGinInfo->pparserdstruct,1);
        pCapMsgGinInfo->berroranalyzedgoose = true;//置位
        if(pCapMsgGinInfo->pap_analyzed_info != NULL)
        {
            pArrayStruct = (MESSAGE_ERROR_INFO_ARRAY_STRUCT*) pCapMsgGinInfo->pap_analyzed_info;
            if(pArrayStruct->n_msg_err >= 1 && pCapMsgGinInfo->nAppConetentGood )//有分析错误事件（且未出现格式错误，最初格式错误后不进行分析，所以不会产生新的n_msg_err,现在允许格式错误继续分析，所以避免重复加入）
            {
                pCapMsgGinInfo->nAppConetentGood = 0;//错误报文
                //事件报文
                BOOL bEventPackage = FALSE;
                for(int i = 0; i< pArrayStruct->n_msg_err; i++)
                {
                    for(int j = 0; j < pArrayStruct->p_msg_err[i].n_num_asduerr; j++)//统计asdu中的错误
                    {
                        for(int k = 0; k < pArrayStruct->p_msg_err[i].p_asduerr[j].n_num_errcode; k++)
                        {
                            if(pArrayStruct->p_msg_err[i].p_asduerr[j].p_errcode[k] == 24)//GOOSE变位事件下文特殊处理
                            {
                                bEventPackage = TRUE;
                                pCapMsgGinInfo->beventanalyzedgoose = true;//goose变位事件
                                 break;
                            }
                        }
                    }
                }
                if(!bEventPackage)
                {
                    //将错误报文增加到队列中
                    pConnection->map_capmsginfo_error.insert(std::map <int, CAPMSGGININFO*> :: value_type(pConnection->nerrpackages, pCapMsgGinInfo));
                    pConnection->nerrpackages ++;//错误报文增加
                }
                else
                {
                    pConnection->map_capmsginfo_event.insert(std::map <int, CAPMSGGININFO*> :: value_type(pConnection->neventpackages, pCapMsgGinInfo));
                    pConnection->neventpackages ++;	//事件报文增加)
                }
            }
        }
        nAnaCount ++;
    }
    return 0;
}

/**
 * @brief CapAnalyzerDoc::AnalyzeAllConnections_Mms 分析MMS链路
 * @param pConnection
 * @return
 */
int AnaKit::AnalyzeAllConnections_Mms(CAPCONNECTINFO* pConnection)
{
    std::map <int, CAPMSGGININFO* >::iterator iter,iter2;
    CAPMSGGININFO* pCapMsgGinInfo;
    CAPMSGGININFO* pCapMsgGinInfo_Connect;
    int nUpdateView = 0;
    MMS_INFO_STRUCT* pMMs_Info_struct,*pMMs_Info_struct_connect;
    for(iter = pConnection->map_capmsginfo.begin(); iter != pConnection->map_capmsginfo.end(); iter ++ )
    {
        pCapMsgGinInfo = iter->second;
        nUpdateView++;
        if(pCapMsgGinInfo == NULL)
            continue;
        if(pCapMsgGinInfo->berroranalyzedmms)//是否已经分析过
            continue;
        if(pCapMsgGinInfo->pparserdstruct == NULL)//尚未解析出来
            continue;
        if(pCapMsgGinInfo->napptype != PROTOAPPTYPE_MMS)//非MMS报文
            continue;
        pCapMsgGinInfo->berroranalyzedmms = true;

        pMMs_Info_struct = (MMS_INFO_STRUCT*) pCapMsgGinInfo->pparserdstruct;

        if(strstr(pMMs_Info_struct->c_pdu_type,"Response") > 0)//回复，需要找关联的报文
        {
            pMMs_Info_struct_connect = NULL;
            for(int i=nUpdateView -2;i>=0; i--)//如果是结果，需根据invokeid追溯MMS报文服务类型
            {
                pCapMsgGinInfo_Connect = pConnection->map_capmsginfo[i];
                if(pCapMsgGinInfo_Connect == NULL)
                    continue;
                if(pCapMsgGinInfo_Connect->pparserdstruct == NULL)
                    continue;
                pMMs_Info_struct_connect = (MMS_INFO_STRUCT*)pCapMsgGinInfo_Connect->pparserdstruct;
                if (pMMs_Info_struct_connect->c_service_type)
                {
                }
                if(strstr(pMMs_Info_struct_connect->c_pdu_type,"Response") >0)//重复报文关联排除
                {
                    continue;
                }
                if(pMMs_Info_struct_connect->n_invoke_id == pMMs_Info_struct->n_invoke_id)
                {
                    pCapMsgGinInfo->pap_anlyzed_info_connect = pCapMsgGinInfo_Connect;
                    pCapMsgGinInfo_Connect->pap_anlyzed_info_connect = pCapMsgGinInfo;
                    //pCapMsgGinInfo->napptpye_type = pCapMsgGinInfo_Connect->napptpye_type +1;//最初invokeid相同，消息类型假设应答==请求+1。

                    strcpy(pCapMsgGinInfo->ccontent,m_pcatransformer->Get_MMS_PKT_DESC(pMMs_Info_struct,pCapMsgGinInfo->nAppConetentGood,pCapMsgGinInfo->napptpye_type).c_str());

                    break;
                }
            }

        }
    }
    return 0;
}
