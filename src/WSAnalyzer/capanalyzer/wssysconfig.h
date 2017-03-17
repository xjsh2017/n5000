#ifndef WSSYSCONFIG_H
#define WSSYSCONFIG_H


#include "wsconstant.h"//文件解析结构
#include "SYS_MODEL_STRUCT.h"        //配置文件头文件引用
#include "SYS_CONFIGMODEL_STRUCT.h"
#include "WAVE_RECORD_STRUCT.h"
#include <map>                      //需用到map表查询
#include <string>
#include <string.h>
#include "xjlib.h"
#include "BaseDealINIFile.h"

//////////////////////////////////////////////////////////////////////
//***********************系统通用配置**********************************/
typedef struct SYS_COMMON_CONFIG
{
    char  c61850keywordtransformerpath[128];    //61850关键字配置文件路径
    char  c61850keywordtransformerfilename[128];//61850关键字配置文件名称
    char  csysmodelcfgfilepath[128];            //模型配置文件路径
    char  csysmodelcfgfilename[128];            //系统模型配置文件名称
    char  ctsananlyzercfgfilename[128];            //本程序配置文件名称
    char  cscanfiledownloadpath[128];           //scan文件下载后放置的路径
    SYS_COMMON_CONFIG()
    {
        strcpy(c61850keywordtransformerpath,"../config");
        strcpy(c61850keywordtransformerfilename,"wsdictionary.xml");//字典
        strcpy(csysmodelcfgfilepath,"../config");
        strcpy(csysmodelcfgfilename,"iscanner.xml");  //模型文件
        strcpy(cscanfiledownloadpath,"d:\\scanfiles\\");//文件存放路径
        strcpy(ctsananlyzercfgfilename,"tsanaloccfg.ini");//文件存放路径
    }
}_SYS_COMMON_CONFIG;
//***********************系统配色**********************************/
typedef struct WSSYSCOLOR
{
    int nColor;
    WSSYSCOLOR()
    {
        nColor = 0;
    }
}_WSSYSCOLOR;
//////////////////////////////////////////////////////////////////////
//***********************界面显示风格**********************************/
typedef struct WSSHOWSTYTLE
{
    int nDataViewTimeStyle;   //采样时间显示风格
                               //0：长时间格式        1970-01-01 01-02-03.123456
                               //1：短时间格式        01-02-03.123456
                               //2: 与第一帧时间差格式 123.123456
                               //3: 与上一帧时间差格式 1.123456
    int n_smpdevdelay_tus;     //采集装置报文处理延迟时间 单位为微秒 add by yinzhehong 20130327

    WSSHOWSTYTLE()
    {
        nDataViewTimeStyle = 2;//短时间格式  01-02-03.123456
    }
}_WSSHOWSTYTLE;
typedef struct IEC61850ANALYZECONFIG
{
    BOOL  breortexceptana_mms;//报告节点过滤模拟量周期报告 add by yinzhehong 20130321
    char  c_exceptcodition_mms[128][65];//需过滤数据集列表名称
    int   n_excepcodition_mms;           //需过滤数据集个数
    //GOOSE的设置参数
    int   n_tremble_T0_minpercent_goose;//GOOSET0的百分比阀值,帧间隔的绝对值 >=T0*ntremble_T0_minpercent 且 <=T0*ndelay_T0_minpercent 则判心跳报文抖动
    int   n_delay_T0_minpercent_goose;//T0的百分比阀值,帧间隔 >T0*ndelay_T0_minpercent 且 <=T0*ndrop_T0_minpercent 则判延迟
    //T0的百分比阀值,帧间隔>T0*ndrop_T0_minpercent 且 序号不连续，则判丢帧
    int   n_drop_T0_minpercent_goose;   //丢帧
    //T的百分比阀值,变位后，且此次无变位，第一/二个帧间隔的绝对值>T1*nTerr_T_minpercent，或第三个帧间隔的绝对值>2*T1*nTerr_T_minpercent，
    //或第四个帧间隔的绝对值>4*T1*nTerr_T_minpercent，判发送机制错误
    int   n_Terr_T_minpercent_goose;     //
    int   n_err_switch_setting_goose;    //判据开关配置,详见对应的分析库
    //SV的分析参数
    //帧间隔减250us的绝对值X，250us*ntremble_minpercent < X < 250us*ndelay_minpercent，(且序号连续)，则是采样值抖动
    int n_tremble_minpercent_sv;
    //帧间隔与250us的差X，250us*ndelay_minpercent <= X <250us*ndrop_minpercent，(且序号连续),则是采样值延迟
    int n_delay_minpercent_sv;
    //帧间隔减250us的绝对值，>=250us*ndrop_minpercent，且序号不连续，则是采样值丢帧
    int n_drop_minpercent_sv;
    //当前采样频率减4000帧/s的绝对值Y，Y > 4000*10%，则是采样值频率不稳定
    int n_rate_unstable_minpercent_sv;
    //采样值数据阀值
    unsigned int n_da_max_val_sv;
    //SMV计数翻转最大值
    unsigned int n_max_smpcnt_sv;
    //双AD数据变化百分比阀值
    int n_doublead_change_percent_sv;
    //通道延时
    int n_tr_delay_change_percent_sv;
    //链路中断
    int n_time_link_disconnect_sv;
    //判据开关配置
    unsigned int n_err_switch_setting_sv;//按位使能，详见对应的分析库

    //PTP中断时间-定值(s)" name="">11</cfgitem>
    unsigned int ao_ms_ptp_discon;
    //PTP同步延时时间-定值s" name="">2</cfgitem>
    unsigned int ao_ms_ptp_syncdelay;
    //delay_resp(时钟请求)延时时间-定值(s)" name="">2</cfgitem>
    unsigned int ao_ms_ptp_delayresp_delay;
    //pdelay_resp(时钟请求two-step)延时时间-定值(s)" name="">2</cfgitem>
    unsigned int ao_ms_ptp_pdelayresp_delay;
        //CF域超差-定值(ns)" "name=">400000000</cfgitem>
    unsigned int ao_ns_ptp_cf;
        //PTP同步报文中断时间-定值(s)" name="">10</cfgitem>
    unsigned int ao_ms_ptp_syn_disc;
        //PTP的delay请求响应中断时间-定值(s)" name="">10</cfgitem>
    unsigned int ao_ms_ptp_delayresp_disc;
        //TP的pdelay请求响应中断时间-定值断时间-定值(s)" name="">10</cfgitem>
    unsigned int ao_ms_ptp_pdelayresp_disc;

    IEC61850ANALYZECONFIG()
    {
        breortexceptana_mms = TRUE;
        memset(c_exceptcodition_mms,0,sizeof(c_exceptcodition_mms));
        n_excepcodition_mms = 0;
        //GOOSE的参数
        n_tremble_T0_minpercent_goose=10;
        n_delay_T0_minpercent_goose=20;
        n_drop_T0_minpercent_goose=100;
        n_Terr_T_minpercent_goose=20;
        n_err_switch_setting_goose = 0;
        //SV的参数
        n_tremble_minpercent_sv = 10;
        n_delay_minpercent_sv = 20;
        n_drop_minpercent_sv = 100;
        n_rate_unstable_minpercent_sv = 10;
        n_da_max_val_sv   = 250000000;
        n_max_smpcnt_sv   = 3999;
        n_doublead_change_percent_sv = 20;
        n_err_switch_setting_sv = 0;
        n_tr_delay_change_percent_sv = 10;
        n_time_link_disconnect_sv = 3000;
    }
}_IEC61850ANALYZECONFIG;
///////////////////////////////
//*********************第三方库配置结构*******************************/
typedef struct THIRDPART_SOURCE
{
    char  cthdpartysrcrelative_path_db[128];    //第三方动态库调试模式存放相对路径,末尾不带斜杠
    char  cthdpartysrcrelative_path_re[128];    //第三方动态库运行模式下存放相对路径,末尾不带斜杠
    char  ccaptxtparserdllname[32];             //txt文件格式解析动态库名称
    char  cfileloaderftpwrapperdllname[32];     //ftp文件下载动态库名称
    char  cmms2acsistaticdllname[32];            //mms解析到acis静态态库名称
    char  gooseanalyzedllname[32];              //goose分析动态库
    char  smvanalyzedllname[32];                //采样分析动态库
    char  ptpanalyzedllname[32];                //采样分析动态库
    char  ccfgmngrdllname[32];                  //配置文件管理
    char  cpackagecovertdllname[32];            //字符流转为采样数据结构动态库
    char  czipcdllname[32];                     //压缩解压缩动态库名
    char  cscandissetpktdllname[32];            //报文解析动态库名称
    char  stsscdparserdllname[32];              //scd中文库
    THIRDPART_SOURCE()
    {
        strcpy(cthdpartysrcrelative_path_db,"./lib");
        strcpy(cthdpartysrcrelative_path_re,"./lib");
        #ifdef OS_WINDOWS
        strcpy(ccaptxtparserdllname,"analyzetxt.dll");
        strcpy(cfileloaderftpwrapperdllname,"FileTransfer.dll");
        strcpy(cmms2acsistaticdllname,"mms2acsi.dll");
        strcpy(gooseanalyzedllname,"gooseanalyze.dll");
        strcpy(smvanalyzedllname,"smvanalyze.dll");
        strcpy(ptpanalyzedllname,"ptpanalyze.dll");
        strcpy(ccfgmngrdllname,"cfgmngr.dll");
        strcpy(cpackagecovertdllname,"packetconvert.dll");
        strcpy(czipcdllname,"zipc.dll");
        strcpy(cscandissetpktdllname,"scandissectpkt.dll");
        strcpy(stsscdparserdllname,"scdparser.dll");
        #else
        strcpy(ccaptxtparserdllname,"analyzetxt.so");
        strcpy(cfileloaderftpwrapperdllname,"FileTransfer.so");
        strcpy(cmms2acsistaticdllname,"mms2acsi.so");
        strcpy(gooseanalyzedllname,"gooseanalyze.so");
        strcpy(smvanalyzedllname,"smvanalyze.so");
        strcpy(ptpanalyzedllname,"ptpanalyze.so");
        strcpy(ccfgmngrdllname,"cfgmngr.so");
        strcpy(cpackagecovertdllname,"packetconvert.so");
        strcpy(czipcdllname,"zipc.so");
        strcpy(cscandissetpktdllname,"scandissectpkt.so");
        strcpy(stsscdparserdllname,"scdparser.so");
        #endif
    }
}_THIRDPART_SOURCE;
///////////////////////////////
//动态库数据处理函数指针定义
/*加载配置文件*/
typedef int(*CFGMNGR_INITIALIZE)(const char* ccfgfilefullpath);
/*释放*/
typedef int (*CFGMNGR_UNINITIALIZE)(void);
/*获取模型文件指针*/
typedef SCANNER* (*GET_SYSMODEL)(void);
/*根据ip地址或者mac地址查询到对应的ied*/
typedef SNIFFER_APP*(*GET_IED_BYNETIDADDR)(char* cNetaddr);
/*根据appid查询到对应的ied装置指针*/
typedef SNIFFER_APP* (*GET_IED_BYAPPID)(int nappid);
/*获取指定模块指定项配置值，输入参数区分大小写*/
typedef CFG_ITEM* (*GET_CFGITEMVALUE)(const char* c_model_name,const char* c_item_name);
/*设置指定模块指定项配置值，输入参数区分大小写*/
typedef int (*SET_CFGITEMVALUE)(const char* c_model_name,const char* c_item_name);

/*根据采样端口系统编号获取对应的采样装置指针*/
typedef SNIFFER* (*GET_SNIFFER)(int nsys_portseq);
/*根据采样端口系统编号获取对应的采样板卡指针*/
typedef SNIFFER_CARD* (*GET_SNIFFERCAR)(int nsys_portseq);
/*根据采样端口系统编号获取对应的采样端口指针*/
typedef SNIFFER_PORT* (*GET_SNIFFERPORT)(int nsys_portseq);
/*获取采集装置端口队列*/
typedef SNIFFER_PORT** (*GET_SNIFFERPORT_LIST)(int& nlistsize);
/*根据节点编号获取对应的遥测遥信数据*/
typedef APP_CHANNEL_RELATION* (*GET_APP_4Y_CFGINFO)(int nsysid);

//根据ied的name获取对象
typedef TS_IED*   (*GET_SCDIED_BYNAME)(char* cName);
//获取IED队列
typedef TS_IED**  (*GET_SCDIEDLSIT)(int& nlistsize);
//创建IED队列
typedef int      (*BUILD_SCDIED_EXREF_RELATION)(TS_IED* ptsIed);
//获取通道队列
typedef CFG_REC_CHANNEL*  (*GET_CFG_REC_CHANNEL_BYSCDREF)(char* cscdref);
class CWSSysconfig
{
private:
    static CWSSysconfig *wsInstance;
public:
    static CWSSysconfig *getInstance()
    {
        if(wsInstance == NULL)
        {
            wsInstance = new CWSSysconfig();
        }
        return wsInstance;
    }
public:
    CWSSysconfig(void);
    ~CWSSysconfig(void);
public:
    WSSYSCOLOR         m_wsSysColor;
    WSSHOWSTYTLE       m_wsShowStyle;
//	CAPFILEPARSERMODE  m_wsCapfileParserMode;
    THIRDPART_SOURCE   m_wsThirdPartSrc;
    SYS_COMMON_CONFIG  m_wsSysCommcfg;
    IEC61850ANALYZECONFIG m_wsIec61850AnaCfg;//61850分析配置项
public:
    /*初始化，加载动态库，导出库函数,cLibPathFullName为动态库相对路径+动态库名称*/
    int Initialize(const char * cLibPathName);
    /*卸载动态库*/
    int UnInitialize();
public:
    /*加载配置文件*/
    int I_CFGMNGR_INITIALIZE(const char* ccfgfilefullpath);
    /*释放*/
    int I_CFGMNGR_UNINITIALIZE(void);
    /*获取模型文件指针*/
    SCANNER* I_GET_SYSMODEL(void);
    /*根据ip地址或者mac地址查询到对应的ied*/
    SNIFFER_APP* I_GET_IED_BYNETIDADDR(char* cNetaddr);
    /*根据appid查询到对应的ied装置指针*/
    SNIFFER_APP* I_GET_IED_BYAPPID(int nappid);
    /*获取指定模块指定项配置值，输入参数区分大小写*/
    CFG_ITEM* I_GET_CFGITEMVALUE(const char* c_model_name,const char* c_item_name);
    /*设置指定模块指定项配置值，输入参数区分大小写*/
    int I_SET_CFGITEMVALUE(const char* c_model_name,const char* c_item_name);

    /*根据采样端口系统编号获取对应的采样装置指针*/
    SNIFFER* I_GET_SNIFFER(int nsys_portseq);
    /*根据采样端口系统编号获取对应的采样板卡指针*/
    SNIFFER_CARD* I_GET_SNIFFERCAR(int nsys_portseq);
    /*根据采样端口系统编号获取对应的采样端口指针*/
    SNIFFER_PORT* I_GET_SNIFFERPORT(int nsys_portseq);
    /*根据采样端口系统编号获取对应的采集端口队列*/
    SNIFFER_PORT** I_GET_SNIFFERPORT_LIST(int& nlistsize);
    /*根据节点编号获取对应的遥测遥信数据*/
    APP_CHANNEL_RELATION* I_GET_APP_4Y_CFGINFO(int nsysid);

public:
    //将文件中的内容读取到配置项中
    int loadconfig();
    int loadconfig_common();
    int loadconfig_mms();
    int loadconfig_ptp();
    int loadconfig_goose();
    int loadconfig_sv();
    //将配置写入到配置文件
    int saveconfig();
    int saveconfig_common();
    int saveconfig_mms();
    int saveconfig_sv();
    int saveconfig_goose();
private:
    CFGMNGR_INITIALIZE         m_pcfgmngr_initialize;
    CFGMNGR_UNINITIALIZE       m_pcfgmngr_uninitialize;
    GET_SYSMODEL               m_pget_sysmodel;
    GET_IED_BYNETIDADDR        m_pget_ied_bynetidaddr;
    GET_IED_BYAPPID            m_pget_ied_byappid;
    GET_CFGITEMVALUE           m_pget_cfgitemvalue;
    SET_CFGITEMVALUE           m_pset_cfgitemvalue;
    GET_SNIFFER                m_pget_sniffer;
    GET_SNIFFERCAR             m_pget_sniffercard;
    GET_SNIFFERPORT            m_pget_snifferport;
    GET_SNIFFERPORT_LIST       m_pget_snifferportlist;
    GET_APP_4Y_CFGINFO         m_pget_app_4y_cfginfo;

    GET_SCDIED_BYNAME              m_pget_scdied_byname;
    GET_SCDIEDLSIT                 m_pget_scdiedlist;
    BUILD_SCDIED_EXREF_RELATION    m_pbuild_scdied_exref_relation;
    GET_CFG_REC_CHANNEL_BYSCDREF   m_pget_cfg_rec_channel_byscdref;
    //加载动态库后返回的句柄
    XJHANDLE                   m_hDllInst;
    BOOL                       m_bCfgFileLoaded;
    CDealINIFile sysconfigFile;
};

#endif // WSSYSCONFIG_H
