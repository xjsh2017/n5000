#include "cappackagesmnger.h"
#include "wsconstant.h"
#include "define_scan.h"
#include "acsi_const.h"

CapPackagesMnger::CapPackagesMnger()
{
    m_pcapconnectinfoTotal.nusetype = 1;
    sprintf(m_pcapconnetionfoTotal_MMS.csrc1_ip,"MMS");
    m_pcapconnetionfoTotal_MMS.nusetype = 2;
    sprintf(m_pcapconnetionfoTotal_TCP.csrc1_ip,"TCP");
    m_pcapconnetionfoTotal_TCP.nusetype = 2;
    sprintf(m_pcapconnetionfoTotal_SV.csrc1_ip,"SV");
    m_pcapconnetionfoTotal_SV.nusetype = 2;
    sprintf(m_pcapconnetionfoTotal_GOOSE.csrc1_ip,"GOOSE");
    m_pcapconnetionfoTotal_GOOSE.nusetype = 2;
    sprintf(m_pcapconnetionfoTotal_UDP.csrc1_ip,"UDP");//UDP报文统计纳入到ARP中
    m_pcapconnetionfoTotal_UDP.nusetype = 2;
    sprintf(m_pcapconnetionfoTotal_ARP.csrc1_ip,"ARPUDP");
    m_pcapconnetionfoTotal_ARP.nusetype = 2;
    sprintf(m_pcapconnetionfoTotal_PTP.csrc1_ip,"PTP");
    m_pcapconnetionfoTotal_PTP.nusetype = 2;
    sprintf(m_pcapconnetionfoTotal_OTHER.csrc1_ip,"OTHER");
    m_pcapconnetionfoTotal_OTHER.nusetype = 2;
    //增加两个默认的链接
    //加入总map表--arp和UDP预先加好
    /*CH1717*/
    CAPCONNECTINFO* pconnection = new CAPCONNECTINFO;
    pconnection->nconnectapptype = ETHER_TYPE_ARP;
    m_mapcapconnectionfo.insert(std::map <int, CAPCONNECTINFO*> :: value_type(0, pconnection));

}

CapPackagesMnger::~CapPackagesMnger()
{

}

CAPCONNECTINFO *CapPackagesMnger::GetPcapconnetInfo(int npcapconnectType)
{
    CAPCONNECTINFO* pcapconnetion = NULL;
    switch (npcapconnectType)
    {
    case PROTOAPPTYPE_TOTAL:
        pcapconnetion = & m_pcapconnectinfoTotal;
        break;
    case IEC61850_ETHER_TYPE_SMV:
        pcapconnetion = & m_pcapconnetionfoTotal_SV;
        break;
    case IEC61850_ETHER_TYPE_GOOSE:
        pcapconnetion = & m_pcapconnetionfoTotal_GOOSE;
        break;
    case ETHER_TYPE_TCP:
        pcapconnetion = & m_pcapconnetionfoTotal_TCP;
        break;
    case IEC61850_ETHER_TYPE_MMS:
        pcapconnetion = & m_pcapconnetionfoTotal_MMS;
        break;
    case IEC61850_ETHER_TYPE_PTP_1588:
        pcapconnetion = & m_pcapconnetionfoTotal_PTP;
        break;
    case ETHER_TYPE_UDP:
        pcapconnetion = & m_pcapconnetionfoTotal_UDP;
        break;
    case ETHER_TYPE_ARP:
        pcapconnetion = & m_pcapconnetionfoTotal_ARP;
        break;
    case  PROTOAPPTYPE_OTHER:
        pcapconnetion = &m_pcapconnetionfoTotal_OTHER;
        break;
    default:
        break;
    }
    return pcapconnetion;
}

/************把报文放入对应队列*****************/
int  CapPackagesMnger::AddPacket2MnGrList(CAPMSGGININFO * pCapPackage)
{
    //加入到总的报文队列
    m_pcapconnectinfoTotal.map_capmsginfo.insert(std::map <int, CAPMSGGININFO*> :: value_type(pCapPackage->nseq-1, pCapPackage));
    //根据报文类型分别加入对应类型
    switch (pCapPackage->napptype)
    {
    case IEC61850_ETHER_TYPE_SMV://
        m_pcapconnetionfoTotal_SV.map_capmsginfo.insert(std::map <int, CAPMSGGININFO*> :: value_type(m_pcapconnetionfoTotal_SV.map_capmsginfo.size(), pCapPackage));
        break;
    case IEC61850_ETHER_TYPE_GOOSE://
        m_pcapconnetionfoTotal_GOOSE.map_capmsginfo.insert(std::map <int, CAPMSGGININFO*> :: value_type(m_pcapconnetionfoTotal_GOOSE.map_capmsginfo.size(), pCapPackage));
        break;
    case ETHER_TYPE_TCP://
        if(pCapPackage->nsrc_port == 102 || pCapPackage->ndst_port == 102)
        {
            m_pcapconnetionfoTotal_MMS.map_capmsginfo.insert(std::map <int, CAPMSGGININFO*> :: value_type(m_pcapconnetionfoTotal_MMS.map_capmsginfo.size(), pCapPackage));
        }
        else
        {
            m_pcapconnetionfoTotal_TCP.map_capmsginfo.insert(std::map <int, CAPMSGGININFO*> :: value_type(m_pcapconnetionfoTotal_TCP.map_capmsginfo.size(), pCapPackage));
        }
        break;
    case IEC61850_ETHER_TYPE_MMS://
        m_pcapconnetionfoTotal_MMS.map_capmsginfo.insert(std::map <int, CAPMSGGININFO*> :: value_type(m_pcapconnetionfoTotal_MMS.map_capmsginfo.size(), pCapPackage));
        break;
    case ETHER_TYPE_COTP://
        m_pcapconnetionfoTotal_MMS.map_capmsginfo.insert(std::map <int, CAPMSGGININFO*> :: value_type(m_pcapconnetionfoTotal_MMS.map_capmsginfo.size(), pCapPackage));
        break;
    case ETHER_TYPE_TPKT://
        m_pcapconnetionfoTotal_MMS.map_capmsginfo.insert(std::map <int, CAPMSGGININFO*> :: value_type(m_pcapconnetionfoTotal_MMS.map_capmsginfo.size(), pCapPackage));
        break;
    case IEC61850_ETHER_TYPE_PTP_1588://
        m_pcapconnetionfoTotal_PTP.map_capmsginfo.insert(std::map <int, CAPMSGGININFO*> :: value_type(m_pcapconnetionfoTotal_PTP.map_capmsginfo.size(), pCapPackage));
        break;
    case ETHER_TYPE_UDP:
        m_pcapconnetionfoTotal_UDP.map_capmsginfo.insert(std::map <int, CAPMSGGININFO*> :: value_type(m_pcapconnetionfoTotal_UDP.map_capmsginfo.size(), pCapPackage));
        break;
    case ETHER_TYPE_ARP:
        m_pcapconnetionfoTotal_ARP.map_capmsginfo.insert(std::map <int, CAPMSGGININFO*> :: value_type(m_pcapconnetionfoTotal_ARP.map_capmsginfo.size(), pCapPackage));
        break;
    default:
        m_pcapconnetionfoTotal_OTHER.map_capmsginfo.insert(std::map <int, CAPMSGGININFO*> :: value_type(m_pcapconnetionfoTotal_OTHER.map_capmsginfo.size(), pCapPackage));
        break;
    }
    return 0;
}

/************根据报文源端或者目的获取总列表*****************/
CAPCONNECTINFO* CapPackagesMnger::GetPcapconnetInfo_bySrc1_Src2(char* src1_addr,char* src2_addr)
{
    if( src1_addr == NULL || src2_addr == NULL)
        return NULL;
    std::map <int, CAPCONNECTINFO* >::iterator iter;
    CAPCONNECTINFO*  pconnection = NULL;
    for (iter = m_mapcapconnectionfo.begin(); iter != m_mapcapconnectionfo.end(); iter ++ )
    {
        if((strstr(iter->second->csrc1_ip,src1_addr)!= NULL) && strstr(iter->second->csrc2_ip,src2_addr)!= NULL)
        {
            pconnection = iter->second;
            break;
        }
        if((strstr(iter->second->csrc2_ip,src1_addr)!= NULL) && strstr(iter->second->csrc1_ip,src2_addr)!= NULL)
        {
            pconnection = iter->second;
            break;
        }
        if((strstr(iter->second->csrc1_mac,src1_addr)!= NULL) && strstr(iter->second->csrc2_mac,src2_addr)!= NULL)
        {
            pconnection = iter->second;
            break;
        }
        if((strstr(iter->second->csrc2_mac,src1_addr)!= NULL) && strstr(iter->second->csrc1_mac,src2_addr)!= NULL)
        {
            pconnection = iter->second;
            break;
        }
    }
    return pconnection;
}
/************根据appid*****************/
CAPCONNECTINFO* CapPackagesMnger::GetPcapconnetInfo_byAppID(int appid)
{
    if(appid <= 0 )
        return NULL;
    std::map <int, CAPCONNECTINFO* >::iterator iter;
    CAPCONNECTINFO*  pconnection = NULL;
    for (iter = m_mapcapconnectionfo.begin(); iter != m_mapcapconnectionfo.end(); iter ++ )
    {
            if(iter->second->ncapp_id == appid)
            {
                pconnection = iter->second;
                break;
            }
    }
    return pconnection;
}
/************根据报文查询条件从队列中查询出对应报文****************/
//查询类别
//          0: 根据发送IP地址模糊查询
//          1: 根据目的IP地址模糊查询
//          2: 根据发送端和目的IP地址模糊查询
//          3：根据发送MAC地址模糊查询
//          4: 根据目的MAC地址模糊查询
//          5: 根据发送以及目的MAC地址模糊查询
//          6：根据MMS报文的应用类型查询，比如：报告，关联，等等
int CapPackagesMnger::GetNewFilterConnectionByCondition(MAP_CAPMSGINFO map_capmsginfo_src,MAP_CAPMSGINFO* pmap_capmsginfo_dst,PACKETQUER_FILTER *pQuery_Filter)
{
    if(map_capmsginfo_src.size() == 0 || pmap_capmsginfo_dst == NULL || pQuery_Filter == NULL)
        return RES_FAIL;
    pmap_capmsginfo_dst->clear();//先清空查询结果
    //根据查询条件进行查询
    std::map <int,  CAPMSGGININFO*>::iterator iter;
    CAPMSGGININFO* pPkt;
    //条件分隔
    char c_codition[128][129];
    memset(c_codition,0,sizeof(c_codition));
    int nconditonsize = 1;
    int ntemp = 0;
    BOOL bMatch = FALSE;
    for(int  i = 0; i < strlen(pQuery_Filter->c_filter_appword); i++)
    {
        if(pQuery_Filter->c_filter_appword[i] != '$')
        {
            memcpy(c_codition[nconditonsize-1]+ntemp,pQuery_Filter->c_filter_appword+i,1);
            ntemp ++;
        }
        else
        {
            ntemp = 0;
            nconditonsize ++;//条件增加
            if(nconditonsize > 128)
                break;
        }
    }
    for (iter = map_capmsginfo_src.begin(); iter != map_capmsginfo_src.end(); iter ++ )
    {
        pPkt = iter->second;
        switch(pQuery_Filter->nfilter_type)
        {
        case 0://根据发送IP地址模糊查询
            {
                bMatch = FALSE;
                for( int nn = 0; nn < nconditonsize; nn++)
                {
                    if((strlen(c_codition[nn]) > 0) && strstr(pPkt->csrc_ip,c_codition[nn]) != NULL)
                    {
                        pmap_capmsginfo_dst->insert(std::map <int, CAPMSGGININFO*> :: value_type(pmap_capmsginfo_dst->size(), pPkt));
                        bMatch = TRUE;
                    }
                    if(bMatch)
                        break;
                }
                break;
            }
        case 1://根据目的IP地址模糊查询
            {
                bMatch = FALSE;
                for( int nn = 0; nn < nconditonsize; nn++)
                {
                    if((strlen(c_codition[nn]) > 0) && strstr(pPkt->cdst_ip,c_codition[nn]) != NULL)
                    {
                        pmap_capmsginfo_dst->insert(std::map <int, CAPMSGGININFO*> :: value_type(pmap_capmsginfo_dst->size(), pPkt));
                        bMatch = TRUE;
                    }
                    if(bMatch)
                        break;
                }
                break;
            }
        case 2://根据发送端和目的IP地址模糊查询
            {
                if(nconditonsize >=2)
                {
                    if((strlen(c_codition[0]) > 0)&& (strlen(c_codition[1])> 0) &&
                        (strstr(pPkt->csrc_ip,c_codition[0]) != NULL) && (strstr(pPkt->cdst_ip,c_codition[1]) != NULL))
                    {
                        pmap_capmsginfo_dst->insert(std::map <int, CAPMSGGININFO*> :: value_type(pmap_capmsginfo_dst->size(), pPkt));
                        bMatch = TRUE;
                    }
                }
                break;
            }
        case 3://根据发送MAC地址模糊查询
            {
                bMatch = FALSE;
                for( int nn = 0; nn < nconditonsize; nn++)
                {
                    if(strlen(c_codition[nn]) > 0 && strstr(pPkt->csrc_mac,c_codition[nn]) != NULL)
                    {
                        pmap_capmsginfo_dst->insert(std::map <int, CAPMSGGININFO*> :: value_type(pmap_capmsginfo_dst->size(), pPkt));
                        bMatch = TRUE;
                    }
                    if(bMatch)
                        break;
                }
                break;
            }
        case 4://根据目的MAC地址模糊查询
            {
                bMatch = FALSE;
                for( int nn = 0; nn < nconditonsize; nn++)
                {
                    if(strlen(c_codition[nn]) > 0 && strstr(pPkt->cdst_mac,c_codition[nn]) != NULL)
                    {
                        pmap_capmsginfo_dst->insert(std::map <int, CAPMSGGININFO*> :: value_type(pmap_capmsginfo_dst->size(), pPkt));
                        bMatch = TRUE;
                    }
                    if(bMatch)
                        break;
                }
                break;
            }
        case 5://根据发送以及目的MAC地址模糊查询
            {
                bMatch = FALSE;
                for( int nn = 0; nn < nconditonsize; nn++)
                {
                    if(strlen(c_codition[nn]) > 0 &&
                        (strstr(pPkt->csrc_mac,c_codition[nn]) != NULL|| strstr(pPkt->cdst_mac,c_codition[nn]) != NULL))
                    {
                        pmap_capmsginfo_dst->insert(std::map <int, CAPMSGGININFO*> :: value_type(pmap_capmsginfo_dst->size(), pPkt));
                        bMatch = TRUE;
                    }
                    if(bMatch)
                        break;
                }
                break;
            }
        case 6://根据MMS报文的应用类型查询，比如：报告，关联，等等
            {
                if(pPkt->napptype == IEC61850_ETHER_TYPE_MMS && pPkt->napptpye_type == pQuery_Filter->nfliter_apptype)
                {
                    if(pPkt->napptpye_type == ACSI_DETAIL_SRVC_TYPE_REPORT )//报告
                    {
                        if(pPkt->beventanalyzedgoose == true)
                            pmap_capmsginfo_dst->insert(std::map <int, CAPMSGGININFO*> :: value_type(pmap_capmsginfo_dst->size(), pPkt));
                    }
                    else
                    {
                        pmap_capmsginfo_dst->insert(std::map <int, CAPMSGGININFO*> :: value_type(pmap_capmsginfo_dst->size(), pPkt));
                    }
                }
                break;
            }
        default:
            break;;
        }
    }
    return 0;
}
