#ifndef SYS_CONFIGMODEL_STRUCT_INCLUDED_00000000000
#define SYS_CONFIGMODEL_STRUCT_INCLUDED_00000000000
/********************************************************************
*	filename: 	SYS_CONFIGMODEL_STRUCT.h
*	author:		yinzhehong
*	created:	2012/03/17
*	purpose:  	����ģ�Ͷ���   
*       version:        1.0
*********************************************************************/

#include "define_scan.h"

//����������
typedef struct CFG_ITEM
{
	char ccfgitemname[64]; //��Ŀ���ƣ���ģ����Ψһ
	char ccfgmodelname[64];//��Ŀ��������
	char ccfgitemvalue[64];//��Ŀֵ
	char ccfgitemdesc[256];//��Ŀ����
	CFG_ITEM()
	{
		memset(ccfgitemname,0,sizeof(ccfgitemname));
		memset(ccfgmodelname,0,sizeof(ccfgmodelname));
		memset(ccfgitemvalue,0,sizeof(ccfgitemvalue));
		memset(ccfgitemdesc,0,sizeof(ccfgitemdesc));
	}
}CFG_ITEM;
#endif //SYS_MODEL_STRUCT_INCLUDED_00000000000
