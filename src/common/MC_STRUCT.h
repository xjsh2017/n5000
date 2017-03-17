// mc_struct.h: interface for the XJMsgCenterAPI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MC_STRUCT_H__6E9918EE_8193_4913_9BE0_570E03AAFF49__INCLUDED_)
#define AFX_MC_STRUCT_H__6E9918EE_8193_4913_9BE0_570E03AAFF49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000　
#include <memory.h>
#include <stdio.h>


/*消息中心启动模式枚举*/
/*add by yzh 20120323新接口使用*/
enum MCServiceMode
{
	MC_PROXY_S_SERVICE = 0x01,		//接收服务代理
		MC_PROXY_D_SERVICE,			    //双向通信代理
		MC_SWITCH_SERVICE,				//纯交换服务
		MC_MIXTURE_S_SERVICE,			//混合服务(单方向)
		MC_MIXTURE_D_SERVICE,			//混合服务(双方向)
};
/*消息中心启动接口*/
/*add by yzh 20120323新接口使用*/
typedef struct mc_config_struct
{
	unsigned int    nidentity;		  //应用程序的唯一标识号(必须大于0)
	MCServiceMode   serviceType;       //启动类型
	char			cprogname[256];	  //应用程序的名称描述
	char			cversion[256];	  //应用程序的版本号(V1.0之类)
	char            clocalname[30];    //本系统作为消息中心交换服务时，应具有的系统唯一标识[作交换服务时使用]
	char            cremotename[15][30];//本系统作为消息中心代理服务时，所连交换服务的系统唯一标识组[作代理服务时使用],最多15个
	mc_config_struct()
	{
		nidentity = 10000;
		serviceType = MC_SWITCH_SERVICE;
		memset (cprogname,0,256);
		memset (cversion,0,256);
		memset (clocalname,0,30);
		memset (cremotename,0,sizeof(cremotename));
	}
}MC_CONFIG_STRUCT;

//******************************通用信息传输结构**************************************//
//*****************用来传输事件******************//
/**	\brief	FIELD_STRUCT
	域结构定义
*/
#define FIELD_STRUCT_CONTENT_LEN 120
#define FIELD_STRUCT_NAME_LEN 20
typedef struct field_struct{
	
	char c_field_name[FIELD_STRUCT_NAME_LEN];      //域名称
	char c_field_content[FIELD_STRUCT_CONTENT_LEN];  //域定义
	
	field_struct()
	{
		memset (c_field_name,0,FIELD_STRUCT_NAME_LEN);
		memset(c_field_content,0,FIELD_STRUCT_CONTENT_LEN);
	}
}FIELD_STRUCT;

/**	\brief	The _Message struct
	用户自定义传送到消息中心库报文数据
*/
typedef struct t_message
{
	int n_topic_type;                         //主题类型(通常于结构生产者按应用类型填写),此处按照数据表类型标识填写
	int n_msgtype;                             //报文类型：0：报告 1：请求 2：回复 event 填为0，yk填为1
	int n_unique;                              //唯一标识 可用作节点编号
	int n_event_yk_id;                         //事件或者控制命令的点号
	int n_result;                              //处理结果 0:成功  1:失败,错误码待定
	int n_reserved;                            //保留
	unsigned int n_utctime;                             //事件发送时间 UTC格式

	char c_sender_name[25];                    //报文发送者,由消息中心填写，使用者无需填写
	char c_suffix[120];                        //后缀，可用于填写带路径的文件名
	
	FIELD_STRUCT *p_field_struct;	           //字段内容数组首地址
	int n_field_num;						   //字段个数
	char c_invokeid[40];                       //控制信号下发时带的控制命令编号
	unsigned int n_timeu;
	t_message()
	{
		n_topic_type    = 0;
		n_msgtype       = 0;
		n_unique        = 0;
		n_event_yk_id   = 0;
		n_reserved      = 0;
		n_result        = 1;
		n_utctime       = 1;
		n_timeu         = 0;
		memset(c_sender_name,0,25);
		memset(c_suffix,0,120);
		p_field_struct = NULL;
		n_field_num = 0;
		memset(c_invokeid,0,40);
	}
}T_MESSAGE;
//******************************连接状态传输结构********************************//
/*连接状态描述结构*/
typedef struct mcprogramnode
{
	unsigned int	nIdentity;		//应用程序的唯一标识号(必须大于0)
	char			cProgName[256];	//应用程序的名称描述
	char			cVersion[256];	//应用程序的版本号(V1.0之类)
}MCPROGRAMNODE;

/*连接状态描述*/
typedef struct mc_connetmsg
{
	//消息中心Proxy和Switch之间连接关系发生的时间
	unsigned int 	year;
	unsigned int    month;
	unsigned int    day;
	unsigned int    hour;
	unsigned int    minute;
	unsigned int    second;
	unsigned int    millisecond;
	//消息中心Proxy和Switch之间连接关系的状态(true:连接正常/false:连接异常)
	bool 			state;	
	//本地地址信息(IP:PORT, 比如:10.123.16.88:7064)			
	char			local[25];
    //远程地址信息(IP:PORT, 比如:10.123.16.88:7636)		
	char            remote[25];  
	//本地(对端)应用进程的相关信息(唯一编号/进程名称描述/进程版本等)
	MCPROGRAMNODE   localnode;	
	//远程(对端)应用进程的相关信息(唯一编号/进程名称描述/进程版本等)
	MCPROGRAMNODE   remotenode;
}MC_CONNECTMSG;

//******************************模拟量状态量控制量上传传输结构**************************************//
/**	\brief	FIELD_STRUCT
	域结构定义
*/
typedef struct data_field_struct
{
	unsigned int n_nodeid;//节点编号
	unsigned int n_unique;//点号
	char         c_type;  //数据类型   1:表示状态量  2：表示模拟量
	double       f_value; //值
	char         c_flag;  //数据标识   1:有效  0:无效 2::测试
	unsigned int n_additional; //附加数据，根据实际需求使用
	char         c_additional; //附加数据，根据实际需求使用
	int          n_reserved;    //备用
	data_field_struct()
	{
		n_nodeid = 0;
		n_unique  = 0;
		f_value   = 0.0f;
		c_type   = 1;
		c_flag   = 1;
		n_additional = 0;
		c_additional = 0;
		n_reserved    = 0;
	}
}DATA_FIELD_STRUCT;

/**	\brief	The DATA_MESSAGE struct
	模拟量 状态量 上送数据
*/
typedef struct data_message
{
	int   n_topic_type;                          //主题类型(通常于结构生产者按应用类型填写),此处按照数据表类型标识填写
	char  c_sender_name[25];                     //报文发送者,由消息中心填写，使用者无需填写,返回时使用
	unsigned int   n_utctime;                             //发送时间 UTC格式
	DATA_FIELD_STRUCT *p_data_field_struct;	     //字段内容数组首地址
	int n_field_num;						     //字段个数
	char c_invokeid[40];                       //控制信号下发时带的控制命令编号
	data_message()
	{
		n_topic_type = 0;
		n_utctime    = 1;
		memset(c_sender_name,0,25);
		p_data_field_struct = NULL;
		n_field_num = 0;
		memset(c_invokeid,0,40);
	}
}DATA_MESSAGE;
//******************************原始报文上传传输结构************************//
typedef struct smv_frame_field_struct{
	
	unsigned int n_frameseq;   //帧序号
	unsigned int n_framelen;   //帧长度
    char         c_frame[400]; //内容
	smv_frame_field_struct()
	{
		n_frameseq   = 0;
		n_framelen   = 0;
		memset(c_frame,0,400);
	}
}SMV_FRAME_FIELD_STRUCT;

typedef struct frame_message
{
	int   n_topic_type;                               //主题类型(通常于结构生产者按应用类型填写),此处按照数据表类型标识填写
	int   n_msgtype;                                  //报文类型：0：报告 1：请求 2：回复，本属性预留使用
	char  c_dataype;                                   //数据类型  1:表示SMV
	int   n_unique;                                    //SMV节点全系统节点编号
	int   n_appid;                                     //SMV的APPID号
	unsigned int   n_utctime;                          //发送的UTC时间
	char  c_sender_name[25];                           //报文发送者,由消息中心填写，使用者无需填写,返回时使用
	SMV_FRAME_FIELD_STRUCT* p_smv_frame_field_struct;  //SMV报文首地址
	int n_field_num;						            //字段个数

	frame_message()
	{
		n_topic_type = 0x1000 + 0x305;           //默认为采样值数据传输
		n_msgtype    = 0;
		c_dataype    = 1;
		n_unique     = 1;
		n_appid      = 0;
		n_utctime    = 1;
		memset(c_sender_name,0,25);
		p_smv_frame_field_struct = NULL;
		n_field_num = 0;
	}
}FRAME_MESSAGE;

//******************************************************************************//
//******************************连接状态传输结构********************************//
/** @brief 提供给用户回调函数类型-通用消息收取回调*/
typedef int (*PFUNPUTMESSAGE) (T_MESSAGE*);

/** @brief 提供给用户回调函数类型-节点状态信息回调*/
typedef int (*PFUNMCCONNETMESSAGE) (MC_CONNECTMSG);

/** @brief 提供给用户回调函数类型-数据消息回调*/
typedef int (*PFUNMCDATAMESSAGE) (DATA_MESSAGE*);

/** @brief 提供给用户回调函数类型-数据消息回调*/
typedef int (*PFUNMCFRMEMESSAGE) (FRAME_MESSAGE*);

#endif