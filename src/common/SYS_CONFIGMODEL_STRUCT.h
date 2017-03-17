#ifndef SYS_CONFIGMODEL_STRUCT_INCLUDED_00000000000
#define SYS_CONFIGMODEL_STRUCT_INCLUDED_00000000000
/********************************************************************
*	filename: 	SYS_CONFIGMODEL_STRUCT.h
*	author:		yinzhehong
*	created:	2012/03/17
*	purpose:  	配置模型定义   
*       version:        1.0
*********************************************************************/

#include "define_scan.h"

//配置项内容
typedef struct CFG_ITEM
{
	char ccfgitemname[64]; //条目名称，在模块中唯一
	char ccfgmodelname[64];//条目所属名称
	char ccfgitemvalue[64];//条目值
	char ccfgitemdesc[256];//条目描述
	CFG_ITEM()
	{
		memset(ccfgitemname,0,sizeof(ccfgitemname));
		memset(ccfgmodelname,0,sizeof(ccfgmodelname));
		memset(ccfgitemvalue,0,sizeof(ccfgitemvalue));
		memset(ccfgitemdesc,0,sizeof(ccfgitemdesc));
	}
}CFG_ITEM;
#endif //SYS_MODEL_STRUCT_INCLUDED_00000000000
