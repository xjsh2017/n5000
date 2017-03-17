#ifndef WAVE_RECORD_STRUCT_INCLUDED_00000000000
#define WAVE_RECORD_STRUCT_INCLUDED_00000000000
/********************************************************************
*	filename: 	SYS_MODEL_STRUCT.h
*	author:		yinzhehong
*	created:	2012/03/16
*	purpose:  	录波通道相关   
*   version:    1.0
*********************************************************************/

#include "define_scan.h"
#include"SYS_MODEL_STRUCT.h"
//SCD中的定义
typedef struct SCDSRC
{                     
	char napp_id;   //应用编号
	char cref[MAX_DESCRIBE_LEN];         //参引
	char cdsname[MAX_DESCRIBE_LEN];      //数据集名称
	int  ndsseq;                         //在数据集中的序号，从0开始编
	char ciedname[MAX_DESCRIBE_LEN];     //ied名称
	SCDSRC()
	{
		memset(cref,0,sizeof(cref));
		memset(cdsname,0,sizeof(cdsname));
		ndsseq = 0;
		napp_id =0;
		memset(ciedname,0,sizeof(ciedname));
	}
}_SCDSRC;

// 通通道结构定义typedef struct CFG_REC_CHANNEL
// {
// 	int  nsys_channelseq;                 //通道编号，由本系统分配，全系统唯一编号，所有的通道需顺序编号，从1开始
// 	char cname[MAX_DESCRIBE_LEN];         //通道名称
// 	char cdesc[MAX_DESCRIBE_LEN];         //说明
// 	int  nsnifferport_sys_id;             //数据采集口对应的sys_id
// 	bool bsynthetic;                     //该通道是否为逻辑合成通道,暂时无用
// 	int  nchaneltype;                     //通道类别  0：模拟量通道 1：开关量通道
// 	float fa;                            //通道增益系数
// 	float fb;                            //偏移量
// 	char  cunits[12];                     //单位
// 	float fratio;                        //变比ct或者pt
// 	char ccbm[64];                       //被监视的设备
// 	char cphase[4];                      //相别 A、B、C、N
// 	float fmin;                          //最小值
// 	float fmax;                          //最大值
// 	float fskew;                         //采样延时
// 	int  nstatus;                        //状态值 通道正常状态，常开：1 常闭：0  开关量通道时常用
// 	SCDSRC  struct_scdsrc;               //scd关联的配置
// 	int  n61850_data_type;               //61850数据类型
// 	//初始化
// 	CFG_REC_CHANNEL()
// 	{
// 		nsys_channelseq = 1;
// 		nsnifferport_sys_id = 0;
// 		bsynthetic      = false;
// 		fa = 1.0;
// 		fb = 0.0;
// 		fratio =1.0;
// 		memset(cname,0,sizeof(cname));
// 		memset(cunits,0,sizeof(cunits));
// 		memset(ccbm,0,sizeof(ccbm));
// 		memset(cphase,0,sizeof(cphase));
// 		fmin = 0.0;
// 		fmax = 0.0;
// 		fskew = 0.0;//单位为微秒
// 		nchaneltype = 0;
// 		nstatus = 0;
// 		n61850_data_type = 999;
// 	}
// }_CFG_REC_CHANNEL;//通道
// 
// 应用节点通道定义
// typedef struct _APP_CHANNEL_RELATION
// {
// 	int n_sniffer_app_sys_id;
// 	int n_yc_channel_num;
// 	int n_yx_channel_num;
// 	CFG_REC_CHANNEL** pyc_channel_list;//遥测通道队列
// 	CFG_REC_CHANNEL** pyx_channel_list;//遥信通道队列
// 
// 	int n_sys_yc_channel_num;
// 	int n_sys_yx_channel_num;
// 
// 	CFG_REC_CHANNEL** pyc_channel_list;//遥测通道队列
// 	CFG_REC_CHANNEL** pyx_channel_list;//遥信通道队列
// 
// 	_APP_CHANNEL_RELATION()
// 	{
// 		n_sniffer_app_sys_id = 0;
// 		n_yc_channel_num     = 0;
// 		n_yx_channel_num     = 0;
// 		pyc_channel_list     = 0;
// 		pyx_channel_list     = 0;
// 	}
// }APP_CHANNEL_RELATION;

#endif //WAVE_RECORD_STRUCT_INCLUDED_00000000000
