#pragma once
#include "wssysconfig.h"

float g_fAo_DblAd_U,g_fAo_DblAd_I;
#define ACSI_ROPORT_FILTER_CONFIG_SIZE 9 //ACSI 报告过滤节点个数

CWSSysconfig * CWSSysconfig::wsInstance = NULL;

CWSSysconfig::CWSSysconfig(void)
{
    m_pcfgmngr_initialize     = NULL;
    m_pcfgmngr_uninitialize   = NULL;
    m_pget_sysmodel           = NULL;
    m_pget_ied_bynetidaddr    = NULL;
    m_pget_ied_byappid        = NULL;
    m_pget_cfgitemvalue       = NULL;
    m_pset_cfgitemvalue       = NULL;
    m_pget_sniffer            = NULL;
    m_pget_sniffercard       = NULL;
    m_pget_snifferport       = NULL;
    m_pget_snifferportlist   = NULL;
    m_pget_app_4y_cfginfo   = NULL;
    m_pget_scdied_byname    = NULL;
    m_pget_scdiedlist       = NULL;
    m_pbuild_scdied_exref_relation = NULL;
    m_pget_cfg_rec_channel_byscdref = NULL;
    //加载动态库后返回的句柄
    m_hDllInst                = NULL;
    m_bCfgFileLoaded          = FALSE;
}


CWSSysconfig::~CWSSysconfig(void)
{
    if(m_bCfgFileLoaded)
    {
        UnInitialize();//释放动态库
    }
}


//初始化，加载动态库，导出库函数
/**
* @brief	Initialize         初始化，加载动态库，导出库函数,需被外部显性调用
* @param 	const char * cLibPathName  动态库所在路径,需相对路径+动态库名称
* @return	int 0表示成功
* @notes	无
* @sample	无
*/
int CWSSysconfig::Initialize(const char * cLibPathName)
{
   try
   {
        loadconfig();    //从ini文件中加载所有配置
        if(cLibPathName == NULL)
        {
            string strDllPathName;
            strDllPathName = m_wsThirdPartSrc.cthdpartysrcrelative_path_re;
            strDllPathName.append("/");
            strDllPathName.append(m_wsThirdPartSrc.ccfgmngrdllname);
            m_hDllInst =  xj_load_library(strDllPathName.c_str());
        }
        else//外部传入了参数
        {
            //加载库
            m_hDllInst = xj_load_library(cLibPathName);
        }
        if(NULL == m_hDllInst)
        {
            return -1;
        }
        //加载函数
        m_pcfgmngr_initialize   = (CFGMNGR_INITIALIZE)   xj_get_addr(m_hDllInst,"cfgmngr_initialize");
        m_pcfgmngr_uninitialize = (CFGMNGR_UNINITIALIZE) xj_get_addr(m_hDllInst,"cfgmngr_uninitialize");
        m_pget_sysmodel         = (GET_SYSMODEL)         xj_get_addr(m_hDllInst,"cfgmngr_get_sysmodel");
        m_pget_ied_bynetidaddr  = (GET_IED_BYNETIDADDR)  xj_get_addr(m_hDllInst,"cfgmngr_get_ied_bynetidaddr");
        m_pget_ied_byappid      = (GET_IED_BYAPPID)      xj_get_addr(m_hDllInst,"cfgmngr_get_ied_byappid");
        m_pget_cfgitemvalue     = (GET_CFGITEMVALUE)     xj_get_addr(m_hDllInst,"cfgmngr_get_cfgitemvalue");
        m_pset_cfgitemvalue     = (SET_CFGITEMVALUE)     xj_get_addr(m_hDllInst,"cfgmngr_set_cfgitemvalue");
        m_pget_sniffer          = (GET_SNIFFER)          xj_get_addr(m_hDllInst,"cfgmngr_get_sniffer");
        m_pget_sniffercard      = (GET_SNIFFERCAR)       xj_get_addr(m_hDllInst,"cfgmngr_get_sniffercard");
        m_pget_snifferport      = (GET_SNIFFERPORT)      xj_get_addr(m_hDllInst,"cfgmngr_get_snifferport");
        m_pget_snifferportlist  = (GET_SNIFFERPORT_LIST) xj_get_addr(m_hDllInst,"cfgmnfr_get_snifferportlist");
        m_pget_app_4y_cfginfo   = (GET_APP_4Y_CFGINFO)   xj_get_addr(m_hDllInst,"cfgmngr_get_app_4y_cfginfo");

        m_pget_scdied_byname    = (GET_SCDIED_BYNAME)  xj_get_addr(m_hDllInst,"cfgmngr_get_scdied_byname");;
        m_pget_scdiedlist       = (GET_SCDIEDLSIT) xj_get_addr(m_hDllInst,"cfgmngr_get_scdiedlist");;
        m_pbuild_scdied_exref_relation = (BUILD_SCDIED_EXREF_RELATION) xj_get_addr(m_hDllInst,"cfgmngr_BuildScdIedExRefRelation");;
        m_pget_cfg_rec_channel_byscdref = (GET_CFG_REC_CHANNEL_BYSCDREF) xj_get_addr(m_hDllInst,"cfgmngr_get_cfgrecchannel_byscdref");;
        //成功判断
        if(m_pcfgmngr_initialize == NULL || m_pcfgmngr_uninitialize == NULL || m_pget_sysmodel == NULL
            || m_pget_ied_bynetidaddr == NULL || m_pget_ied_byappid == NULL || m_pget_cfgitemvalue == NULL ||
            m_pset_cfgitemvalue == NULL || m_pget_sniffer == NULL || m_pget_sniffercard == NULL || m_pget_snifferport == NULL ||
            m_pget_snifferportlist == NULL || m_pget_app_4y_cfginfo == NULL)
        {
            return -1;
        }
        else//加载配置文件
        {
            //设置配置文件名
            string strFilePathName;
            strFilePathName = m_wsSysCommcfg.csysmodelcfgfilepath;
            strFilePathName.append("/");
            strFilePathName.append(m_wsSysCommcfg.csysmodelcfgfilename);
            int ss =  I_CFGMNGR_INITIALIZE(strFilePathName.c_str());
            if(I_CFGMNGR_INITIALIZE(strFilePathName.c_str()) == 0)//加载配置文件成功
            {
                m_bCfgFileLoaded = TRUE;
                //将文件配置映射到内存配置
                return 0;
            }
            else//失败
            {
                return -1;
            }
        }
    }/*
    catch (CMemoryException* e)
    {
        e->Delete();
    }
    catch (CFileException* e)
    {
        e->Delete();
    }
    catch (CException* e)
    {
        e->Delete();
    }*/
    catch (...)
    {

    }
    return -1;
}
//释放动态库
/**
* @brief	UnInitialize         释放动态库
* @param
* @return	int 0表示成功
* @notes	无
* @sample	无
*/
int CWSSysconfig::UnInitialize()
{
    if(m_bCfgFileLoaded)
    {
        I_CFGMNGR_UNINITIALIZE();//释放资源
        m_bCfgFileLoaded = FALSE;
    }
    if(m_hDllInst != NULL)
    {
        xj_free_library(m_hDllInst);
        m_hDllInst = NULL;
        m_pcfgmngr_initialize     = NULL;
        m_pcfgmngr_uninitialize   = NULL;
        m_pget_sysmodel           = NULL;
        m_pget_ied_bynetidaddr    = NULL;
        m_pget_ied_byappid        = NULL;
        m_pget_cfgitemvalue       = NULL;
        m_pset_cfgitemvalue       = NULL;
    }
    return 0;
}
/*加载配置文件*/
int CWSSysconfig::I_CFGMNGR_INITIALIZE(const char* ccfgfilefullpath)
{
    if(m_pcfgmngr_initialize == NULL)
        return -1;

    return m_pcfgmngr_initialize(ccfgfilefullpath);
}
/*释放*/
int CWSSysconfig::I_CFGMNGR_UNINITIALIZE(void)
{
    if(m_pcfgmngr_uninitialize == NULL)
        return -1;
    return m_pcfgmngr_uninitialize();
}
/*获取模型文件指针*/
SCANNER* CWSSysconfig::I_GET_SYSMODEL(void)
{
    if(m_pget_sysmodel == NULL)
        return NULL;
    return m_pget_sysmodel();
}
/*根据ip地址或者mac地址查询到对应的ied*/
SNIFFER_APP* CWSSysconfig::I_GET_IED_BYNETIDADDR(char* cNetaddr)
{
    if(m_pget_ied_bynetidaddr == NULL)
        return NULL;
    return m_pget_ied_bynetidaddr(cNetaddr);
}
/*根据appid查询到对应的ied装置指针*/
SNIFFER_APP* CWSSysconfig::I_GET_IED_BYAPPID(int nappid)
{
    if(m_pget_ied_byappid == NULL)
        return NULL;

    return m_pget_ied_byappid(nappid);
}
/*获取指定模块指定项配置值，输入参数区分大小写*/
CFG_ITEM* CWSSysconfig::I_GET_CFGITEMVALUE(const char* c_model_name,const char* c_item_name)
{
    if(m_pget_cfgitemvalue == NULL)
        return NULL;

    return m_pget_cfgitemvalue(c_model_name,c_item_name);
}
/*设置指定模块指定项配置值，输入参数区分大小写*/
int CWSSysconfig::I_SET_CFGITEMVALUE(const char* c_model_name,const char* c_item_name)
{
    if(m_pset_cfgitemvalue == NULL)
        return NULL;

    return m_pset_cfgitemvalue(c_model_name,c_item_name);
}
/*根据采样端口系统编号获取对应的采样装置指针*/
SNIFFER* CWSSysconfig::I_GET_SNIFFER(int nsys_portseq)
{
    if(m_pget_sniffer == NULL)
        return NULL;
    return m_pget_sniffer(nsys_portseq);
}
/*根据采样端口系统编号获取对应的采样板卡指针*/
SNIFFER_CARD* CWSSysconfig::I_GET_SNIFFERCAR(int nsys_portseq)
{
    if(m_pget_sniffercard == NULL)
        return NULL;
    return m_pget_sniffercard(nsys_portseq);
}
/*根据采样端口系统编号获取对应的采样端口指针*/
SNIFFER_PORT* CWSSysconfig::I_GET_SNIFFERPORT(int nsys_portseq)
{
    if(m_pget_snifferport == NULL)
        return NULL;
    return m_pget_snifferport(nsys_portseq);
}
/*获取采集装置接口队列*/
SNIFFER_PORT** CWSSysconfig::I_GET_SNIFFERPORT_LIST(int& nlistsize)
{
    if(m_pget_snifferportlist == NULL)
        return NULL;
    return m_pget_snifferportlist(nlistsize);
}
//根据系统编号查找详细信息
APP_CHANNEL_RELATION* CWSSysconfig::I_GET_APP_4Y_CFGINFO(int nsysid)
{
    if(m_pget_app_4y_cfginfo == NULL)
        return NULL;
    return m_pget_app_4y_cfginfo(nsysid);
}
/*将文件中的内容读取到配置项中*/
int CWSSysconfig::loadconfig()
{
    loadconfig_common();
    loadconfig_mms();
    loadconfig_goose();
    loadconfig_sv();
    loadconfig_ptp();
    return 0;
}
/***********读取文本方式的配置文件*************/
int CWSSysconfig::loadconfig_common()
{
    //GetPrivateProfileString  GetPrivateProfileint
    string strFileName;
    strFileName = m_wsSysCommcfg.csysmodelcfgfilepath;
    strFileName.append("/");
    strFileName.append(m_wsSysCommcfg.ctsananlyzercfgfilename);
    //报文显示风格
    m_wsShowStyle.nDataViewTimeStyle = GetPrivateProfileint("SYSSETING","dataviewtimestyle",2,strFileName.c_str());
    //文件下载后存放的路径
    GetPrivateProfileStr("SYSSETING","downloadfilesavepath","c:\\scanfiles\\",m_wsSysCommcfg.cscanfiledownloadpath,128,strFileName.c_str());
    return 0;
}
int CWSSysconfig::loadconfig_mms()
{
    string strFileName;
    strFileName = m_wsSysCommcfg.csysmodelcfgfilepath;
    strFileName.append("/");
    strFileName.append(m_wsSysCommcfg.ctsananlyzercfgfilename);
    //MMS报告
    if(GetPrivateProfileint("MMS_SETTING","ReportExceptEnable",1,strFileName.c_str()) > 0)
    {
        m_wsIec61850AnaCfg.breortexceptana_mms = TRUE;
    }
    else
    {
        m_wsIec61850AnaCfg.breortexceptana_mms = FALSE;
    }
    m_wsIec61850AnaCfg.n_excepcodition_mms = 1;
    char dsexceptlsit[1024];
    memset(dsexceptlsit,0,1024);
    GetPrivateProfileStr("MMS_SETTING","ExceptDslist","c:\\scanfiles\\",dsexceptlsit,1024,strFileName.c_str());
    int ntemp = 0;
    int ncfgitemvaluelength = strlen(dsexceptlsit);
    for(int i = 0; i < ncfgitemvaluelength;i++)
    {
        if(dsexceptlsit[i] != '$')
        {
            memcpy(m_wsIec61850AnaCfg.c_exceptcodition_mms[m_wsIec61850AnaCfg.n_excepcodition_mms-1]+ntemp,dsexceptlsit+i,1);
            ntemp ++;
            if(ntemp == 65)//越限后强制到下一节点
            {
                ntemp = 0;
                m_wsIec61850AnaCfg.n_excepcodition_mms ++;//条件增加
                break;
            }
        }
        else
        {
            ntemp = 0;
            m_wsIec61850AnaCfg.n_excepcodition_mms ++;//条件增加
            if(m_wsIec61850AnaCfg.n_excepcodition_mms > 128)
                break;
        }
    }
    return 0;
}

int CWSSysconfig::loadconfig_ptp()
{

    string strFileName;
    strFileName = m_wsSysCommcfg.csysmodelcfgfilepath;
    strFileName.append("/");
    strFileName.append(m_wsSysCommcfg.ctsananlyzercfgfilename);


    //PTP中断时间-定值(s)" name="">11</cfgitem>
    m_wsIec61850AnaCfg.ao_ms_ptp_discon = GetPrivateProfileint("PTP_SETTING","ao_ms_ptp_discon",11,strFileName.c_str());
    //PTP同步延时时间-定值s" name="">2</cfgitem>
    m_wsIec61850AnaCfg.ao_ms_ptp_syncdelay = GetPrivateProfileint("PTP_SETTING","ao_ms_ptp_syncdelay",2,strFileName.c_str());
    //delay_resp(时钟请求)延时时间-定值(s)" name="">2</cfgitem>
    m_wsIec61850AnaCfg.ao_ms_ptp_delayresp_delay = GetPrivateProfileint("PTP_SETTING","ao_ms_ptp_delayresp_delay",2,strFileName.c_str());
    //pdelay_resp(时钟请求two-step)延时时间-定值(s)" name="">2</cfgitem>
    m_wsIec61850AnaCfg.ao_ms_ptp_pdelayresp_delay = GetPrivateProfileint("PTP_SETTING","ao_ms_ptp_pdelayresp_delay",2,strFileName.c_str());
    //CF域超差-定值(ns)" "name=">400000000</cfgitem>
    m_wsIec61850AnaCfg.ao_ns_ptp_cf = GetPrivateProfileint("ao_ns_ptp_cf","ao_ns_ptp_cf",400000000,strFileName.c_str());
    //PTP同步报文中断时间-定值(s)" name="">10</cfgitem>
    m_wsIec61850AnaCfg.ao_ms_ptp_syn_disc = GetPrivateProfileint("PTP_SETTING","ao_ms_ptp_syn_disc",10,strFileName.c_str());
    //PTP的delay请求响应中断时间-定值(s)" name="">10</cfgitem>
    m_wsIec61850AnaCfg.ao_ms_ptp_delayresp_disc = GetPrivateProfileint("PTP_SETTING","ao_ms_ptp_delayresp_disc",10,strFileName.c_str());
    //TP的pdelay请求响应中断时间-定值断时间-定值(s)" name="">10</cfgitem>
    m_wsIec61850AnaCfg.ao_ms_ptp_pdelayresp_disc = GetPrivateProfileint("PTP_SETTING","ao_ms_ptp_pdelayresp_disc",10,strFileName.c_str());

    return 0;
}

/********GOOSE报文分析***********/
int CWSSysconfig::loadconfig_goose()
{
    string strFileName;
    strFileName = m_wsSysCommcfg.csysmodelcfgfilepath;
    strFileName.append("/");
    strFileName.append(m_wsSysCommcfg.ctsananlyzercfgfilename);
    //GOOSE的设置参数
    //GOOSET0的百分比阀值,帧间隔的绝对值 >=T0*ntremble_T0_minpercent 且 <=T0*ntremble_T0_minpercent 则判心跳报文抖动
    m_wsIec61850AnaCfg.n_tremble_T0_minpercent_goose = GetPrivateProfileint("GOOSE_SETTING","tremble_T0_minpercent",10,strFileName.c_str());
   //T0的百分比阀值,帧间隔 >T0*ndelay_T0_minpercent 且 <=T0*ndrop_T0_minpercent 则判延迟
    m_wsIec61850AnaCfg.n_delay_T0_minpercent_goose = GetPrivateProfileint("GOOSE_SETTING","delay_T0_minpercent",20,strFileName.c_str());
    //T0的百分比阀值,帧间隔>T0*ndrop_T0_minpercent 且 序号不连续，则判丢帧
    m_wsIec61850AnaCfg.n_drop_T0_minpercent_goose = GetPrivateProfileint("GOOSE_SETTING","drop_T0_minpercent",100,strFileName.c_str());   //丢帧
    //T的百分比阀值,变位后，且此次无变位，第一/二个帧间隔的绝对值>T1*nTerr_T_minpercent，或第三个帧间隔的绝对值>2*T1*nTerr_T_minpercent，
    //或第四个帧间隔的绝对值>4*T1*nTerr_T_minpercent，判发送机制错误
    m_wsIec61850AnaCfg.n_Terr_T_minpercent_goose = GetPrivateProfileint("GOOSE_SETTING","Terr_T1_minpercent",20,strFileName.c_str());
    //以下为判据开关配置
    int nswitch = 0;
    //sqNum跳变
    nswitch = GetPrivateProfileint("GOOSE_SETTING","sqNum_jump",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_goose |= SETTING_GOOSE_SQNUM_JUMP;//sqNum跳变
    //sqNum逆转
    nswitch = GetPrivateProfileint("GOOSE_SETTING","sqNum_reverse",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_goose |= SETTING_GOOSE_SQNUM_REVERSE;//sqNum逆转
    //stNum跳变
    nswitch = GetPrivateProfileint("GOOSE_SETTING","stNum_jump",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_goose |= SETTING_GOOSE_STNUM_JUMP;//stNum跳变
    //stNum逆转
    nswitch = GetPrivateProfileint("GOOSE_SETTING","stNum_reverse",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_goose |= SETTING_GOOSE_STNUM_REVERSE;//stNum逆转
    //GOOSE新事件,sqNum未复归
    nswitch = GetPrivateProfileint("GOOSE_SETTING","sqNum_unrese",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_goose |= SETTING_GOOSE_SQNUM_UNRESET;//stNum变化,sqNum未复归
    //重复帧
    nswitch = GetPrivateProfileint("GOOSE_SETTING","goose_double",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_goose |= SETTING_GOOSE_GOOSE_DOUBLE; //重复帧
    //内容完全重复的GOOSE
    nswitch = GetPrivateProfileint("GOOSE_SETTING","goose_repeat",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_goose |= SETTING_GOOSE_GOOSE_REPEAT;//内容完全重复
    //stNum虚变
    nswitch = GetPrivateProfileint("GOOSE_SETTING","stNumChange_fake",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_goose |= SETTING_GOOSE_STNUMCHANGE_FAKE;//stNum虚变
    //GOOSE延迟
    nswitch = GetPrivateProfileint("GOOSE_SETTING","goose_delay",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_goose |= SETTING_GOOSE_GOOSE_DELAY;//报文延迟
    //TAL为零
    nswitch = GetPrivateProfileint("GOOSE_SETTING","tal_zero",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_goose  |= SETTING_GOOSE_TAL_ZERO;//TAL为零
    //APPID为零
    nswitch = GetPrivateProfileint("GOOSE_SETTING","appid_zero",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_goose |= SETTING_GOOSE_APPID_ZERO;//APPID为零
    //GOOSE控制块需要配置
    nswitch = GetPrivateProfileint("GOOSE_SETTING","ndscomm_true",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_goose |= SETTING_GOOSE_NDSCOMM_TRUE;//控制块需要配置
    //GOOSE发送机制错误
    nswitch = GetPrivateProfileint("GOOSE_SETTING","T0_T3_err",0,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_goose |= SETTING_GOOSE_T0_T3_ERR;//发送机制错误
    //GOOSE报文中通道个数与配置不一致
    nswitch = GetPrivateProfileint("GOOSE_SETTING","daNum_differ_dsmembernum",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_goose |= SETTING_GOOSE_DANUM_DIFFER_DSMEMBERNUM;//通道个数与配置不一致
    //gocbref与配置不一致
    nswitch = GetPrivateProfileint("GOOSE_SETTING","gocbref_differ_cfg",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_goose |= SETTING_GOOSE_GOCBREF_DIFFER_CFG;//gocbref与配置不一致
    //goid与配置不一致
    nswitch = GetPrivateProfileint("GOOSE_SETTING","goid_differ_cfg",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_goose |= SETTING_GOOSE_GOID_DIFFER_CFG;//goid与配置不一致
    //dsname与配置不一致
    nswitch = GetPrivateProfileint("GOOSE_SETTING","ds_differ_cfg",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_goose |= SETTING_GOOSE_DS_DIFFER_CFG;//dsname与配置不一致
    //版本号与配置不一致
    nswitch = GetPrivateProfileint("GOOSE_SETTING","confrev_differ_cfg",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_goose |= SETTING_GOOSE_CONFREV_DIFFER_CFG;//版本号与配置不一致
    //实际数据个数与danum不一致
    nswitch = GetPrivateProfileint("GOOSE_SETTING","daEntries_differ_danum",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_goose |= SETTING_GOOSE_DAENTRIES_DIFFER_DANUM;//numDataEntries与实际数据个数不一致
    //目的MAC地址错误
    nswitch = GetPrivateProfileint("GOOSE_SETTING","destmac_err",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_goose |= SETTING_GOOSE_DESTMAC_ERR;//目的MAC地址错误
    //GOOSE链路重启
    nswitch = GetPrivateProfileint("GOOSE_SETTING","goose_restart",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_goose |= SETTING_GOOSE_GOOSE_RESTART;//GOOSE链路重启
    //GOOSE变位
    nswitch = GetPrivateProfileint("GOOSE_SETTING","stNumChange_right",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_goose |= SETTING_GOOSE_STNUMCHANGE_RIGHT;//GOOSE变位
    //置检修标志
    nswitch = GetPrivateProfileint("GOOSE_SETTING","test_true",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_goose |= SETTING_GOOSE_TEST_TRUE;//置检修标志
    //格式错误
    nswitch = GetPrivateProfileint("GOOSE_SETTING","format_err",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_goose |= SETTING_GOOSE_FORMAT_ERR;//格式错误
    //目的MAC地址与配置不一致
    nswitch = GetPrivateProfileint("GOOSE_SETTING","mac_differ_cfg",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_goose |= SETTING_GOOSE_MAC_DIFFER_CFG;//目的MAC地址与配置不一致
    //数据类型与配置文件不一致
    nswitch = GetPrivateProfileint("GOOSE_SETTING","datype_differ_cfg",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_goose |= SETTING_GOOSE_DATYPE_DIFFER_CFG;//数据类型与配置文件不一致
    //丢帧
    nswitch = GetPrivateProfileint("GOOSE_SETTING","goose_drop",0,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_goose |= SETTING_GOOSE_GOOSE_DROP;//丢帧
    //心跳报文抖动
    nswitch = GetPrivateProfileint("GOOSE_SETTING","goose_tremble",0,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_goose |= SETTING_GOOSE_GOOSE_THEMBLE;//心跳报文抖动

    return 0;
}
int CWSSysconfig::loadconfig_sv()
{
    string strFileName;
    strFileName = m_wsSysCommcfg.csysmodelcfgfilepath;
    strFileName.append("/");
    strFileName.append(m_wsSysCommcfg.ctsananlyzercfgfilename);
    //帧间隔减250us的绝对值X，250us*ntremble_minpercent < X < 250us*ndelay_minpercent，(且序号连续)，则是采样值抖动
    m_wsIec61850AnaCfg.n_tremble_minpercent_sv = GetPrivateProfileint("SV_SETTING","tremble_minpercent",10,strFileName.c_str());
    //帧间隔与250us的差X，250us*ndelay_minpercent <= X <250us*ndrop_minpercent，(且序号连续),则是采样值延迟
    m_wsIec61850AnaCfg.n_delay_minpercent_sv = GetPrivateProfileint("SV_SETTING","delay_inpercent",20,strFileName.c_str());
    //帧间隔减250us的绝对值，>=250us*ndrop_minpercent，且序号不连续，则是采样值丢帧
    m_wsIec61850AnaCfg.n_drop_minpercent_sv = GetPrivateProfileint("SV_SETTING","drop_minpercent_sv",100,strFileName.c_str());
    //当前采样频率减4000帧/s的绝对值Y，Y > 4000*10%，则是采样值频率不稳定
    m_wsIec61850AnaCfg.n_rate_unstable_minpercent_sv = GetPrivateProfileint("SV_SETTING","rate_unstable_minpercent",10,strFileName.c_str());
    //采样值数据阀值
    m_wsIec61850AnaCfg.n_da_max_val_sv = GetPrivateProfileint("SV_SETTING","smvda_max_val",25000000,strFileName.c_str());
    //SMV计数翻转最大值
    m_wsIec61850AnaCfg.n_max_smpcnt_sv = GetPrivateProfileint("SV_SETTING","max_smp_cnt",3999,strFileName.c_str());
    //双AD数据变化百分比阀值
    m_wsIec61850AnaCfg.n_doublead_change_percent_sv = GetPrivateProfileint("SV_SETTING","doublead_change_percent",20,strFileName.c_str());
    //通道延时变化
    m_wsIec61850AnaCfg.n_tr_delay_change_percent_sv = GetPrivateProfileint("SV_SETTING","tr_delay_change_percent",10,strFileName.c_str());
    //链路中断
    m_wsIec61850AnaCfg.n_time_link_disconnect_sv = GetPrivateProfileint("SV_SETTING","time_link_disconnect",3000,strFileName.c_str());

    //以下为判据开关配置
    int nswitch = 0;
    //采样延迟
    nswitch = GetPrivateProfileint("SV_SETTING","smv_delay",0,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_sv |= SETTING_SMV_DELAY;
    //APPID为零
    nswitch = GetPrivateProfileint("SV_SETTING","appid_zero",0,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_sv |= SETTING_SMV_APPID_ZERO;
    //pdu长度为零
    nswitch = GetPrivateProfileint("SV_SETTING","pdulen_zero",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_sv |= SETTING_SMV_PDULEN_ZERO;
    //目的MAC地址错误
    nswitch = GetPrivateProfileint("SV_SETTING","destmac_err",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_sv |= SETTING_SMV_DESTMAC_ERR;
    //MAC地址与配置不一致
    nswitch = GetPrivateProfileint("SV_SETTING","mac_differ_cfg",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_sv |= SETTING_SMV_MAC_DIFFER_CFG;
    //格式错误
    nswitch = GetPrivateProfileint("SV_SETTING","format_err",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_sv |= SETTING_SMV_FORMAT_ERR;
    //ASDU个数与配置不一致
    nswitch = GetPrivateProfileint("SV_SETTING","asdu_differ_cfg",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_sv |= SETTING_SMV_ASDU_DIFFER_CFG;
    //数据帧抖动
    nswitch = GetPrivateProfileint("SV_SETTING","tremble",0,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_sv |= SETTING_SMV_TREMBLE;
    //频率不稳定
    nswitch = GetPrivateProfileint("SV_SETTING","rate_unstable",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_sv |= SETTING_SMV_RATE_UNSTABLE;
    //双SMV
    nswitch = GetPrivateProfileint("SV_SETTING","smv_double",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_sv |= SETTING_SMV_DOUBLE;
    //序号跳变
    nswitch = GetPrivateProfileint("SV_SETTING","smpcnt_jump",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_sv |= SETTING_SMV_SMPCNT_JUMP;
    //序号逆转
    nswitch = GetPrivateProfileint("SV_SETTING","smpcnt_reverse",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_sv |= SETTING_SMV_SMPCNT_REVERSE;
    //丢失同步信号
    nswitch = GetPrivateProfileint("SV_SETTING","asynchronism",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_sv |= SETTING_SMV_ASYNCHRONISM;
    //通道个数与配置不一致
    nswitch = GetPrivateProfileint("SV_SETTING","danum_differ_dsmembernum",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_sv |= SETTING_SMV_DANUM_DIFFER_DSMEMBERNUM;
    //ds与配置不一致
    nswitch = GetPrivateProfileint("SV_SETTING","ds_differ_cfg",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_sv |= SETTING_SMV_DS_DIFFER_CFG;
    //svid与配置不一致
    nswitch = GetPrivateProfileint("SV_SETTING","svid_differ_cfg",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_sv |= SETTING_SMV_SVID_DIFFER_CFG;
    //版本号与配置不一致
    nswitch = GetPrivateProfileint("SV_SETTING","confrev_differ_cfg",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_sv |= SETTING_SMV_CONFREV_DIFFER_CFG;
    //丢帧
    nswitch = GetPrivateProfileint("SV_SETTING","drop_pkt",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_sv |= SETTING_SMV_DROP_PKT;
    //通道延时变化
    nswitch = GetPrivateProfileint("SV_SETTING","delay_tr_tg_change",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_sv |= SETTING_SMV_DELAY_TR_TG_CHANGE;
    //品质改变
    nswitch = GetPrivateProfileint("SV_SETTING","quality_change",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_sv |= SETTING_SMV_QUALITY_CHANGE;
    //数值异常
    nswitch = GetPrivateProfileint("SV_SETTING","data_overflow",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_sv |= SETTING_SMV_DA_ERR;
    //双AD数据不一致
    nswitch = GetPrivateProfileint("SV_SETTING","double_ad_data_differ",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_sv |= SETTING_SMV_DOUBLE_AD_DATA_DIFFER;
    //置检修标志
    nswitch = GetPrivateProfileint("SV_SETTING","smv_test",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_sv |= SETTING_SMV_TEST;
    nswitch = GetPrivateProfileint("SV_SETTING","smv_invalid_field_val",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_sv |= SETTING_SMV_INVALID_FIELD_VAL;
    nswitch = GetPrivateProfileint("SV_SETTING","resynchron",1,strFileName.c_str());
    if(nswitch)
        m_wsIec61850AnaCfg.n_err_switch_setting_sv |= SETTING_SMV_RESYNCHRON;

    char buf[25];

    GetPrivateProfileStr("SV_SETTING","ao_lmt_dbl_ad_diff_u","1000",buf,25,strFileName.c_str());
    g_fAo_DblAd_U= atof(buf) ;
    GetPrivateProfileStr("SV_SETTING","ao_lmt_dbl_ad_diff_i","20",buf,25,strFileName.c_str());
    g_fAo_DblAd_I= atof(buf);
    return 0;
}
/***********保存文本方式的配置文件*************/
int CWSSysconfig::saveconfig()
{
    saveconfig_common();
    saveconfig_mms();
    saveconfig_sv();
    saveconfig_goose();
    return 0;
}
/******************/
int CWSSysconfig::saveconfig_common()
{
    string strFileName;
    char strValue[255];
    strFileName = m_wsSysCommcfg.csysmodelcfgfilepath;
    strFileName.append("/");
    strFileName.append(m_wsSysCommcfg.ctsananlyzercfgfilename);
    //报文显示风格
    sprintf(strValue, "%d", m_wsShowStyle.nDataViewTimeStyle);
    sysconfigFile.WritePrivateProfile("SYSSETING", "dataviewtimestyle",strValue,strFileName.c_str());

    //文件下载后存放的路径
    sysconfigFile.WritePrivateProfile("SYSSETING", "downloadfilesavepath",m_wsSysCommcfg.cscanfiledownloadpath,strFileName.c_str());
    return 0;
}
/***********保存mms的设置配置***********/
int CWSSysconfig::saveconfig_mms()
{
    string strFileName,strvalue,strtemp;
    strFileName = m_wsSysCommcfg.csysmodelcfgfilepath;
    strFileName.append("/");
    strFileName.append(m_wsSysCommcfg.ctsananlyzercfgfilename);
    if(m_wsIec61850AnaCfg.breortexceptana_mms)
    {
        sysconfigFile.WritePrivateProfile("MMS_SETTING", "ReportExceptEnable","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("MMS_SETTING", "ReportExceptEnable","0",strFileName.c_str());
    }
    strvalue.clear();
    for(int i = 0; i < m_wsIec61850AnaCfg.n_excepcodition_mms; i++)
    {
        strtemp = m_wsIec61850AnaCfg.c_exceptcodition_mms[i];
        if(strvalue.empty())
        {
            strvalue = strtemp;
        }
        else
        {
            strvalue = strvalue + "$" + strtemp;
        }
    }
    sysconfigFile.WritePrivateProfile("MMS_SETTING", "ExceptDslist",(char *)strvalue.c_str(),strFileName.c_str());
    return 0;
}
/**********保存sv的设置配置****************/
int CWSSysconfig::saveconfig_sv()
{
    string strFileName;
    strFileName = m_wsSysCommcfg.csysmodelcfgfilepath;
    strFileName.append("/");
    strFileName.append(m_wsSysCommcfg.ctsananlyzercfgfilename);
    //帧间隔减250us的绝对值X，250us*ntremble_minpercent < X < 250us*ndelay_minpercent，(且序号连续)，则是采样值抖动
    char strValue[255];
    sprintf(strValue,"%d",m_wsIec61850AnaCfg.n_tremble_minpercent_sv);
    sysconfigFile.WritePrivateProfile("SV_SETTING", "tremble_minpercent",strValue,strFileName.c_str());
    //帧间隔与250us的差X，250us*ndelay_minpercent <= X <250us*ndrop_minpercent，(且序号连续),则是采样值延迟
    sprintf(strValue,"%d",m_wsIec61850AnaCfg.n_delay_minpercent_sv);
    sysconfigFile.WritePrivateProfile("SV_SETTING", "delay_inpercent",strValue,strFileName.c_str());
    //帧间隔减250us的绝对值，>=250us*ndrop_minpercent，且序号不连续，则是采样值丢帧
    sprintf(strValue,"%d",m_wsIec61850AnaCfg.n_drop_minpercent_sv);
    sysconfigFile.WritePrivateProfile("SV_SETTING", "drop_minpercent_sv",strValue,strFileName.c_str());
    //当前采样频率减4000帧/s的绝对值Y，Y > 4000*10%，则是采样值频率不稳定
    sprintf(strValue,"%d",m_wsIec61850AnaCfg.n_rate_unstable_minpercent_sv);
    sysconfigFile.WritePrivateProfile("SV_SETTING", "rate_unstable_minpercent",strValue,strFileName.c_str());
    //采样值数据阀值
    sprintf(strValue,"%d",m_wsIec61850AnaCfg.n_da_max_val_sv);
    sysconfigFile.WritePrivateProfile("SV_SETTING", "smvda_max_val",strValue,strFileName.c_str());
    //SMV计数翻转最大值
    sprintf(strValue,"%d",m_wsIec61850AnaCfg.n_max_smpcnt_sv,strValue);
    sysconfigFile.WritePrivateProfile("SV_SETTING", "max_smp_cnt",strValue,strFileName.c_str());
    //双AD数据变化百分比阀值
    sprintf(strValue,"%d",m_wsIec61850AnaCfg.n_doublead_change_percent_sv,strValue);
    sysconfigFile.WritePrivateProfile("SV_SETTING", "doublead_change_percent",strValue,strFileName.c_str());
    //通道延时
    sprintf(strValue,"%d",m_wsIec61850AnaCfg.n_tr_delay_change_percent_sv,strValue);
    sysconfigFile.WritePrivateProfile("SV_SETTING", "tr_delay_change_percent",strValue,strFileName.c_str());
    //链路中断
    sprintf(strValue,"%d",m_wsIec61850AnaCfg.n_time_link_disconnect_sv,strValue);
    sysconfigFile.WritePrivateProfile("SV_SETTING", "time_link_disconnect",strValue,strFileName.c_str());
    //以下为判据开关配置
    //采样延迟
    if( (m_wsIec61850AnaCfg.n_err_switch_setting_sv & SETTING_SMV_DELAY) == SETTING_SMV_DELAY)
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "smv_delay","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "smv_delay","0",strFileName.c_str());
    }
    //APPID为零
    if( (m_wsIec61850AnaCfg.n_err_switch_setting_sv & SETTING_SMV_APPID_ZERO) == SETTING_SMV_APPID_ZERO)
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "appid_zero","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "appid_zero","0",strFileName.c_str());
    }
    //pdu长度为零
    if((m_wsIec61850AnaCfg.n_err_switch_setting_sv & SETTING_SMV_PDULEN_ZERO) == SETTING_SMV_PDULEN_ZERO)
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "pdulen_zero","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "pdulen_zero","0",strFileName.c_str());
    }
    //目的MAC地址错误
    if((m_wsIec61850AnaCfg.n_err_switch_setting_sv & SETTING_SMV_DESTMAC_ERR) == SETTING_SMV_DESTMAC_ERR)
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "destmac_err","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "destmac_err","0",strFileName.c_str());
    }
    //MAC地址与配置不一致
    if((m_wsIec61850AnaCfg.n_err_switch_setting_sv & SETTING_SMV_MAC_DIFFER_CFG) == SETTING_SMV_MAC_DIFFER_CFG)
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "mac_differ_cfg","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "mac_differ_cfg","0",strFileName.c_str());
    }
    //格式错误
    if((m_wsIec61850AnaCfg.n_err_switch_setting_sv & SETTING_SMV_FORMAT_ERR) == SETTING_SMV_FORMAT_ERR)
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "format_err","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "format_err","0",strFileName.c_str());
    }
    //ASDU个数与配置不一致
    if((m_wsIec61850AnaCfg.n_err_switch_setting_sv & SETTING_SMV_ASDU_DIFFER_CFG) == SETTING_SMV_ASDU_DIFFER_CFG)
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "asdu_differ_cfg","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "asdu_differ_cfg","0",strFileName.c_str());
    }
    //数据帧抖动
    if((m_wsIec61850AnaCfg.n_err_switch_setting_sv & SETTING_SMV_TREMBLE) == SETTING_SMV_TREMBLE)
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "tremble","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "tremble","0",strFileName.c_str());
    }
    //频率不稳定
    if((m_wsIec61850AnaCfg.n_err_switch_setting_sv & SETTING_SMV_RATE_UNSTABLE) == SETTING_SMV_RATE_UNSTABLE)
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "rate_unstable","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "rate_unstable","0",strFileName.c_str());
    }
    //双SMV
    if((m_wsIec61850AnaCfg.n_err_switch_setting_sv & SETTING_SMV_DOUBLE) == SETTING_SMV_DOUBLE)
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "smv_double","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "smv_double","0",strFileName.c_str());
    }
    //序号跳变
    if((m_wsIec61850AnaCfg.n_err_switch_setting_sv & SETTING_SMV_SMPCNT_JUMP) == SETTING_SMV_SMPCNT_JUMP)
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "smpcnt_jump","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "smpcnt_jump","0",strFileName.c_str());
    }
    //序号逆转
    if((m_wsIec61850AnaCfg.n_err_switch_setting_sv & SETTING_SMV_SMPCNT_REVERSE) == SETTING_SMV_SMPCNT_REVERSE)
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "smpcnt_reverse","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "smpcnt_reverse","0",strFileName.c_str());
    }
    //丢失同步信号
    if((m_wsIec61850AnaCfg.n_err_switch_setting_sv & SETTING_SMV_ASYNCHRONISM) == SETTING_SMV_ASYNCHRONISM)
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "asynchronism","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "asynchronism","0",strFileName.c_str());
    }
    //通道个数与配置不一致
    if((m_wsIec61850AnaCfg.n_err_switch_setting_sv & SETTING_SMV_DANUM_DIFFER_DSMEMBERNUM) == SETTING_SMV_DANUM_DIFFER_DSMEMBERNUM)
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "danum_differ_dsmembernum","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "danum_differ_dsmembernum","0",strFileName.c_str());
    }
    //ds与配置不一致
    if((m_wsIec61850AnaCfg.n_err_switch_setting_sv & SETTING_SMV_DS_DIFFER_CFG) == SETTING_SMV_DS_DIFFER_CFG)
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "ds_differ_cfg","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "ds_differ_cfg","0",strFileName.c_str());
    }
    //svid与配置不一致
    if((m_wsIec61850AnaCfg.n_err_switch_setting_sv & SETTING_SMV_SVID_DIFFER_CFG) == SETTING_SMV_SVID_DIFFER_CFG)
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "svid_differ_cfg","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "svid_differ_cfg","0",strFileName.c_str());
    }
    //版本号与配置不一致
    if((m_wsIec61850AnaCfg.n_err_switch_setting_sv & SETTING_SMV_CONFREV_DIFFER_CFG) == SETTING_SMV_CONFREV_DIFFER_CFG)
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "confrev_differ_cfg","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "confrev_differ_cfg","0",strFileName.c_str());
    }
    //丢帧
    if((m_wsIec61850AnaCfg.n_err_switch_setting_sv & SETTING_SMV_DROP_PKT) == SETTING_SMV_DROP_PKT)
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "drop_pkt","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "drop_pkt","0",strFileName.c_str());
    }
    //通道延时变化
    if((m_wsIec61850AnaCfg.n_err_switch_setting_sv & SETTING_SMV_DELAY_TR_TG_CHANGE) == SETTING_SMV_DELAY_TR_TG_CHANGE)
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "delay_tr_tg_change","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "delay_tr_tg_change","0",strFileName.c_str());
    }
    //品质改变
    if((m_wsIec61850AnaCfg.n_err_switch_setting_sv & SETTING_SMV_QUALITY_CHANGE) == SETTING_SMV_QUALITY_CHANGE)
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "quality_change","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "quality_change","0",strFileName.c_str());
    }
    //数值异常
    if((m_wsIec61850AnaCfg.n_err_switch_setting_sv & SETTING_SMV_DA_ERR) == SETTING_SMV_DA_ERR)
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "data_overflow","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "data_overflow","0",strFileName.c_str());
    }
    //双AD数据不一致
    if((m_wsIec61850AnaCfg.n_err_switch_setting_sv & SETTING_SMV_DOUBLE_AD_DATA_DIFFER) == SETTING_SMV_DOUBLE_AD_DATA_DIFFER)
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "double_ad_data_differ","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "double_ad_data_differ","0",strFileName.c_str());
    }
    //置检修标志
    if((m_wsIec61850AnaCfg.n_err_switch_setting_sv & SETTING_SMV_TEST) == SETTING_SMV_TEST)
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "smv_test","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "smv_test","0",strFileName.c_str());
    }
    //置检修标志
    if((m_wsIec61850AnaCfg.n_err_switch_setting_sv & SETTING_SMV_INVALID_FIELD_VAL) == SETTING_SMV_INVALID_FIELD_VAL)
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "smv_invalid_field_val","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "smv_invalid_field_val","0",strFileName.c_str());
    }
    //置检修标志
    if((m_wsIec61850AnaCfg.n_err_switch_setting_sv & SETTING_SMV_RESYNCHRON) == SETTING_SMV_RESYNCHRON)
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "resynchron","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("SV_SETTING", "resynchron","0",strFileName.c_str());
    }
    char szVal[23];
    sprintf(szVal,"%f",g_fAo_DblAd_U);
    sysconfigFile.WritePrivateProfile("SV_SETTING", "ao_lmt_dbl_ad_diff_u",szVal,strFileName.c_str());
    sprintf(szVal,"%f",g_fAo_DblAd_I);
    sysconfigFile.WritePrivateProfile("SV_SETTING", "ao_lmt_dbl_ad_diff_i",szVal,strFileName.c_str());

    return 0;
}
/********保存GOOSE的设置配置***********/
int CWSSysconfig::saveconfig_goose()
{
    string strFileName;
    strFileName = m_wsSysCommcfg.csysmodelcfgfilepath;
    strFileName.append("/");
    strFileName.append(m_wsSysCommcfg.ctsananlyzercfgfilename);
    char strvalue[255];
    sprintf(strvalue,"%d",m_wsIec61850AnaCfg.n_tremble_T0_minpercent_goose);
    sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "tremble_T0_minpercent",strvalue,strFileName.c_str());

    sprintf(strvalue,"%d",m_wsIec61850AnaCfg.n_delay_T0_minpercent_goose);
    sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "delay_T0_minpercent",strvalue,strFileName.c_str());

    sprintf(strvalue,"%d",m_wsIec61850AnaCfg.n_drop_T0_minpercent_goose);
    sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "drop_T0_minpercent",strvalue,strFileName.c_str());

    sprintf(strvalue,"%d",m_wsIec61850AnaCfg.n_Terr_T_minpercent_goose);
    sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "Terr_T1_minpercent",strvalue,strFileName.c_str());
    //以下为判据开关配置
    //sqNum跳变
    if( (m_wsIec61850AnaCfg.n_err_switch_setting_goose & SETTING_GOOSE_SQNUM_JUMP) == SETTING_GOOSE_SQNUM_JUMP)
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "sqNum_jump","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "sqNum_jump","0",strFileName.c_str());
    }
    //sqNum逆转
    if( (m_wsIec61850AnaCfg.n_err_switch_setting_goose & SETTING_GOOSE_SQNUM_REVERSE) == SETTING_GOOSE_SQNUM_REVERSE)
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING","sqNum_reverse","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "sqNum_reverse","0",strFileName.c_str());
    }
    //stNum跳变
    if( (m_wsIec61850AnaCfg.n_err_switch_setting_goose & SETTING_GOOSE_STNUM_JUMP) == SETTING_GOOSE_STNUM_JUMP)
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING","stNum_jump","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "stNum_jump","0",strFileName.c_str());
    }
    //stNum逆转
    if( (m_wsIec61850AnaCfg.n_err_switch_setting_goose & SETTING_GOOSE_STNUM_REVERSE) == SETTING_GOOSE_STNUM_REVERSE)
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING","stNum_reverse","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "stNum_reverse","0",strFileName.c_str());
    }
    //GOOSE新事件,sqNum未复归
    if( (m_wsIec61850AnaCfg.n_err_switch_setting_goose & SETTING_GOOSE_SQNUM_UNRESET) == SETTING_GOOSE_SQNUM_UNRESET)
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING","sqNum_unrese","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "sqNum_unrese","0",strFileName.c_str());
    }
    //重复帧
    if( (m_wsIec61850AnaCfg.n_err_switch_setting_goose & SETTING_GOOSE_GOOSE_DOUBLE) == SETTING_GOOSE_GOOSE_DOUBLE)
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING","goose_double","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "goose_double","0",strFileName.c_str());
    }
    //内容完全重复的GOOSE
    if( (m_wsIec61850AnaCfg.n_err_switch_setting_goose & SETTING_GOOSE_GOOSE_REPEAT) == SETTING_GOOSE_GOOSE_REPEAT)
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING","goose_repeat","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "goose_repeat","0",strFileName.c_str());
    }
    //stNum虚变
    if( (m_wsIec61850AnaCfg.n_err_switch_setting_goose & SETTING_GOOSE_STNUMCHANGE_FAKE) == SETTING_GOOSE_STNUMCHANGE_FAKE)
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING","stNumChange_fake","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "stNumChange_fake","0",strFileName.c_str());
    }
    //GOOSE延迟
    if( (m_wsIec61850AnaCfg.n_err_switch_setting_goose & SETTING_GOOSE_GOOSE_DELAY) == SETTING_GOOSE_GOOSE_DELAY)
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING","goose_delay","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "goose_delay","0",strFileName.c_str());
    }
    //TAL为零
    if( (m_wsIec61850AnaCfg.n_err_switch_setting_goose & SETTING_GOOSE_TAL_ZERO) == SETTING_GOOSE_TAL_ZERO)
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING","tal_zero","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "tal_zero","0",strFileName.c_str());
    }
    //APPID为零
    if( (m_wsIec61850AnaCfg.n_err_switch_setting_goose & SETTING_GOOSE_APPID_ZERO) == SETTING_GOOSE_APPID_ZERO)
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING","appid_zero","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "appid_zero","0",strFileName.c_str());
    }
    //GOOSE控制块需要配置
    if( (m_wsIec61850AnaCfg.n_err_switch_setting_goose & SETTING_GOOSE_NDSCOMM_TRUE) == SETTING_GOOSE_NDSCOMM_TRUE)
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING","ndscomm_true","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "ndscomm_true","0",strFileName.c_str());
    }
    //GOOSE发送机制错误
    if( (m_wsIec61850AnaCfg.n_err_switch_setting_goose & SETTING_GOOSE_T0_T3_ERR) == SETTING_GOOSE_T0_T3_ERR)
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING","T0_T3_err","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "T0_T3_err","0",strFileName.c_str());
    }
    //GOOSE报文中通道个数与配置不一致
    if( (m_wsIec61850AnaCfg.n_err_switch_setting_goose & SETTING_GOOSE_DANUM_DIFFER_DSMEMBERNUM) == SETTING_GOOSE_DANUM_DIFFER_DSMEMBERNUM)
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING","daNum_differ_dsmembernum","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "daNum_differ_dsmembernum","0",strFileName.c_str());
    }
    //gocbref与配置不一致
    if( (m_wsIec61850AnaCfg.n_err_switch_setting_goose & SETTING_GOOSE_GOCBREF_DIFFER_CFG) == SETTING_GOOSE_GOCBREF_DIFFER_CFG)
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING","gocbref_differ_cfg","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "gocbref_differ_cfg","0",strFileName.c_str());
    }
    //goid与配置不一致
    if( (m_wsIec61850AnaCfg.n_err_switch_setting_goose & SETTING_GOOSE_GOID_DIFFER_CFG) == SETTING_GOOSE_GOID_DIFFER_CFG)
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING","goid_differ_cfg","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "goid_differ_cfg","0",strFileName.c_str());
    }
    //dsname与配置不一致
    if( (m_wsIec61850AnaCfg.n_err_switch_setting_goose & SETTING_GOOSE_DS_DIFFER_CFG) == SETTING_GOOSE_DS_DIFFER_CFG)
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING","ds_differ_cfg","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "ds_differ_cfg","0",strFileName.c_str());
    }
    //版本号与配置不一致
    if( (m_wsIec61850AnaCfg.n_err_switch_setting_goose & SETTING_GOOSE_CONFREV_DIFFER_CFG) == SETTING_GOOSE_CONFREV_DIFFER_CFG)
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING","confrev_differ_cfg","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "confrev_differ_cfg","0",strFileName.c_str());
    }
    //实际数据个数与danum不一致
    if((m_wsIec61850AnaCfg.n_err_switch_setting_goose & SETTING_GOOSE_DAENTRIES_DIFFER_DANUM) == SETTING_GOOSE_DAENTRIES_DIFFER_DANUM)
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING","daEntries_differ_danum","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "daEntries_differ_danum","0",strFileName.c_str());
    }
    //目的MAC地址错误
    if((m_wsIec61850AnaCfg.n_err_switch_setting_goose & SETTING_GOOSE_DESTMAC_ERR) == SETTING_GOOSE_DESTMAC_ERR)
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING","destmac_err","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "destmac_err","0",strFileName.c_str());
    }
    //GOOSE链路重启
    if((m_wsIec61850AnaCfg.n_err_switch_setting_goose & SETTING_GOOSE_GOOSE_RESTART) == SETTING_GOOSE_GOOSE_RESTART)
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING","goose_restart","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "goose_restart","0",strFileName.c_str());
    }
    //GOOSE变位
    if((m_wsIec61850AnaCfg.n_err_switch_setting_goose & SETTING_GOOSE_STNUMCHANGE_RIGHT) == SETTING_GOOSE_STNUMCHANGE_RIGHT)
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING","stNumChange_right","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "stNumChange_right","0",strFileName.c_str());
    }
    //置检修标志
    if((m_wsIec61850AnaCfg.n_err_switch_setting_goose & SETTING_GOOSE_TEST_TRUE) == SETTING_GOOSE_TEST_TRUE)
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING","test_true","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "test_true","0",strFileName.c_str());
    }
    //格式错误
    if((m_wsIec61850AnaCfg.n_err_switch_setting_goose & SETTING_GOOSE_FORMAT_ERR) == SETTING_GOOSE_FORMAT_ERR)
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING","format_err","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "format_err","0",strFileName.c_str());
    }
    //目的MAC地址与配置不一致
    if((m_wsIec61850AnaCfg.n_err_switch_setting_goose & SETTING_GOOSE_MAC_DIFFER_CFG) == SETTING_GOOSE_MAC_DIFFER_CFG)
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING","mac_differ_cfg","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "mac_differ_cfg","0",strFileName.c_str());
    }
    //数据类型与配置文件不一致
    if((m_wsIec61850AnaCfg.n_err_switch_setting_goose & SETTING_GOOSE_DATYPE_DIFFER_CFG) == SETTING_GOOSE_DATYPE_DIFFER_CFG)
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING","datype_differ_cfg","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "datype_differ_cfg","0",strFileName.c_str());
    }
    //丢帧
    if((m_wsIec61850AnaCfg.n_err_switch_setting_goose & SETTING_GOOSE_GOOSE_DROP) == SETTING_GOOSE_GOOSE_DROP)
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING","goose_drop","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "goose_drop","0",strFileName.c_str());
    }
    //心跳报文抖动
    if((m_wsIec61850AnaCfg.n_err_switch_setting_goose & SETTING_GOOSE_GOOSE_THEMBLE) == SETTING_GOOSE_GOOSE_THEMBLE)
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING","goose_tremble","1",strFileName.c_str());
    }
    else
    {
        sysconfigFile.WritePrivateProfile("GOOSE_SETTING", "goose_tremble","0",strFileName.c_str());
    }
    return 0;
}
