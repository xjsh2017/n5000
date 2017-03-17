// XJMsgCenterAPI.h: interface for the XJMsgCenterAPI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XJMSGCENTERAPI_H__6E9918EE_8193_4913_9BE0_570E03AAFF49__INCLUDED_)
#define AFX_XJMSGCENTERAPI_H__6E9918EE_8193_4913_9BE0_570E03AAFF49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/**	
   消息中心启动模式,本宏定义由老接口使用
*/
#define MC_PROXY_MODE 0                /*代理单向模式*/
#define MC_SWITCH_MODE 1               /*路由模式*/
#define MC_MIXTURE_MODE 2              /*混合单向模式*/
#define MC_PROXY_D_MODE 3              /*代理双向模式-新增*/
#define MC_MIXTURE_D_MODE 4            /*混合双向模式-新增*/

/*消息优先级*/
/*add by yzh 20120323新接口使用*/
enum  MCMessagePri
{
	MC_MESSAGE_PRI_HIGHEST = 0x0a,  //最高优先级
	MC_MESSAGE_PRI_NORMAL,			//普通优先级
	MC_MESSAGE_PRI_LOWEST			//最低优先级
};
/*各应用程序的编号，全系统统一*/
enum MCApp_Indentity
{
	MC_APP_SNIFFER_SWITCH = 0X01,    //装置消息中心
	MC_APP_SNIFFER_DATA_SAMPLE_NET,  //基于千兆口的数据采集
	MC_APP_SNIFFER_DATA_SAMPLE_CPCI, //基于CPCI的数据采集
	MC_APP_SNIFFER_NETANALYZE,       //装置网络分析进程——流量统计
	MC_APP_SNIFFER_DATAANALYZE,      //装置报文分析进程——在线分析
	MC_APP_SNIFFER_WAVERECORD,       //录波分析模块
	MC_APP_SNIFFER_FILESYS_MONITOR,  //文件监视系统
	MC_APP_SNIFFER_HISTORY_STORE,    //历史数据存储
	MC_APP_SNIFFER_TIME_ADJUST,      //对时模块
	MC_APP_SNIFFER_DISK_CLEANER,     //磁盘管理
	MC_APP_SNIFFER_IEC61850_SERVER,   //IEC61850服务端  
	MC_APP_SNIFFER_DATAANALYZE_SMV,		//SMV在线分析进程
	MC_APP_SNIFFER_DATAANALYZE_MMS		//SMV在线分析进程
};
/**	\brief	PmMyMessageStruct enum
	结构类型注册(分配用户段类型)（深圳分部）报文数据,需从0x1000开始
*/
enum PmMyMessageStruct
{
	REFVALUE_SCAN_STRUCT 	   = 0x1000+0x100,  //通用数据传输结构
	REFVALUE_SCAN_DATA_STRUCT  = 0x1000+0x101,  //点数据传输结构
	REFVALUE_SCAN_FRAME_STRUCT = 0x1000+0x102, //原始帧数据传输结构
};
/**	\brief MC TOPIC 消息中心主题
	结构类型注册(分配用户段类型)（深圳分部）报文数据,需从0x1000开始
*/
//基本部分
/**********************************以下为新版本 add by yinzhehong 20120327***************************************************/
#define MC_MCENTER_DEFAULT                              0x0001

#define MC_TOPIC_USER_APP_NO_RECEIVE                    (0x1000 + 0x999)//本主题空闲，只发送不接收消息的客户端使用
#define MC_EVENT_MESSAGE_REQ                            (0x1000 + 0x109)//事件消息_控制命令下发，本主题用于采集装置只发送数据，并支持命令
#define MC_EVENT_MESSAGE_NETANALYZE_REPORT              (0x1000 + 0x100)//事件消息_网络分析
#define MC_EVENT_MESSAGE_DATAANALYZE_REPORT             (0x1000 + 0x102)//事件消息_数据分析
#define MC_EVENT_MESSAGE_FAULTRECORD_REPORT             (0x1000 + 0x103)//事件消息_故障录波
#define MC_EVENT_MESSAGE_DATAANALYZE_REPORT_NEW_SMV		(0x1000 + 0x104)//事件消息_数据分析_SMV节点突增加
#define MC_EVENT_MESSAGE_DATAANALYZE_REPORT_NEW_GOOSE	(0x1000 + 0x105)//事件消息_数据分析_GOOSE节点突增加
#define MC_EVENT_MESSAGE_DATAANALYZE_REPORT_MMS         (0x1000 + 0x106)//事件消息_MMS报文告警
#define MC_EVENT_MESSAGE_DATAANALYZE_REPORT_PTP         (0x1000 + 0x107)//事件消息_PTP报文告警
//#define MC_EVENT_MESSAGE_REQ                            (0x1000 + 0x109)//占位

#define MC_DATA_MESSAGE_NET_FLOW_REPORT                   (0x1000 + 0x110)//数据事件_流量报告
#define MC_DATA_MESSAGE_NET_SAMPLE_DATA_REPORT            (0x1000 + 0x111)//数据事件_通过网络节点采样上送数据
#define MC_DATA_MESSAGE_FAULTRECORD_SAMPLE_DATA_REPORT    (0x1000 + 0x112)//数据事件_通过录波节点采样上送的数据
#define MC_DATA_MESSAGE_COMMON_DATA_REPORT                (0x1000 + 0x113)//数据事件_通用数据-其它类型的数据，比如状态量变位，节点状态等
#define MC_DATA_MESSAGE_NET_SAMPLE_DATA_GOOSE_SNAP_REPORT (0x1000 + 0x114)//数据事件_通过网络节点采样上送GOOSE快照数据
#define MC_DATA_MESSAGE_NET_SAMPLE_DATA_SMV_SNAP_REPORT   (0x1000 + 0x115)//数据事件_通过网络节点采样上送SMV快照数据
#define MC_DATA_MESSAGE_NET_SAMPLE_DATA_SMV_SAMP_REPORT   (0x1000 + 0x116)//数据事件_通过网络节点采样上送SMV采样数据
#define MC_DATA_MESSAGE_NET_SAMPLE_DATA_MULTI_SMV_SNAP_REPORT   (0x1000 + 0x117)//数据事件_通过网络节点采样上送同一时刻多个SMV快照数据

#define MC_FRAME_MESSAGE_REPORT                           (0x1000 + 0x120)//原始报文事件

#define MC_DATA_MESSAGE_MMS_DATA_REPORT                (0x1000 + 0x118)//数据事件_MMS类型的数据，节点状态
#define MC_DATA_MESSAGE_SMV_DISPERSION_REPORT                (0x1000 + 0x119)//数据事件_YC离散度


/**************************************以下为老版本****************************************************************/
//网络记录仪部分
#define MC_TOPIC_USER_APP_GOOSE_ANALYZE_REPORT          (0x1000 + 0x301) //GOOSE分析结果报告
#define MC_TOPIC_USER_APP_GOOSE_STATE_REPORT            (0x1000 + 0x302) //GOOSE原始数据报告
#define MC_TOPIC_USER_APP_GOOSE_STATE_CHANGE_REPORT     (0x1000 + 0x303) //GOOSE状态变位报告
#define MC_TOPIC_USER_APP_SMV_ANALYZE_REPORT            (0x1000 + 0x304) //SMV分析结果报告
#define MC_TOPIC_USER_APP_SMV_VALUE_REPORT              (0x1000 + 0x305) //SMV原始数据内容报告
#define MC_TOPIC_USER_APP_MMS_ANALYZE_REPORT            (0x1000 + 0x306) //MMS分析结果报告
#define MC_TOPIC_USER_APP_GOOSE_STATE_REQ               (0x1000 + 0x307) //GOOSE报文发送/取消发送请求
#define MC_TOPIC_USER_APP_SMV_STATE_REQ                 (0x1000 + 0x308) //SMV报文发送/取消发送请求

#define MC_TOPIC_USER_APP_SCANFILE_REPORT               (0x1000 + 0x320) //记录文件生成报告
#define MC_TOPIC_USER_APP_SCANFILE_GET_REQ              (0x1000 + 0x321) //记录文件调取请求
#define MC_TOPIC_USER_APP_SCANFILE_MAKE_REQ             (0x1000 + 0x322) //生成记录文件要求

#define MC_TOPIC_USER_APP_SNIFFER_FLOW_REPORT           (0x1000 + 0x352) //采集装置流量报告
#define MC_TOPIC_USER_APP_SNIFFER_FLOW_REQ              (0x1000 + 0x353) //采集装置流量查询请求
#define MC_TOPIC_USER_APP_NETWORK_NODE_FLOWALARM_REPORT (0x1000 + 0x354) //网络节点流量越限报告
#define MC_TOPIC_USER_APP_NETWORK_NODE_CMMUSTATE_REPORT (0x1000 + 0x355) //网络节点通断状态报告
#define MC_TOPIC_USER_APP_NETWORK_NODE_CMMUSTATE_REQ    (0x1000 + 0x356) //网络节点通断状态查询请求

//录波器部分
#define MC_TOPIC_USER_APP_COMTRADEFILE_REPORT            (0x1000 + 0x401) //生成录波文件报告
#define MC_TOPIC_USER_APP_COMTRADEFILE_MAKE_REQ          (0x1000 + 0x402) //生成录波文件要求

//共通部分
#define MC_TOPIC_USER_APP_COMMAND_REQ_RESULT            (0x1000 + 0x801) //命令执行结果反馈
#define MC_TOPIC_USER_APP_SYS_STATE_REPORT              (0x1000 + 0x802) //系统运行状态报告
#define MC_TOPIC_USER_APP_SYS_RESTART_REQ               (0x1000 + 0x803) //进程重起请求
#define MC_TOPIC_USER_APP_SYS_RELOADCFG_REQ             (0x1000 + 0x804) //重新加载配置文件请求(在修改配置文件以后使用)

#define MC_TOPIC_USER_APP_SYS_TIME_REPORT               (0x1000 + 0x810) //系统时间报告
#define MC_TOPIC_USER_APP_SYS_TIME_REQ                  (0x1000 + 0x811) //系统时间查询要求
#define MC_TOPIC_USER_APP_SYS_TIME_MODIFY_REQ           (0x1000 + 0x812) //系统时间修改要求

#endif // !defined(AFX_XJMSGCENTERAPI_H__6E9918EE_8193_4913_9BE0_570E03AAFF49__INCLUDED_)
