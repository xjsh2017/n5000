/****************************************************************************/
/* Copyright (c) 2010,��̼������޹�˾                                      */
/* All rights reserved.                                                     */
/*                                                                          */
/*  ģ��������                                                              */
/** ����scd_info��������ied����ռ�
*  @file  acsi_crscl.c                                                      
/*                                                                          */
/* ����         ����    ע��                                                */
/* 2010/07/17   ZYZ     �����ļ�                                            */
/****************************************************************************/
#include "acsi_crscl.h"

#ifdef DEBUG_SISCO
SD_CONST static ST_CHAR *SD_CONST thisFileName = __FILE__;
#endif

#define MAX_FC_NUM 17//��Լ�϶�����17��fc

typedef struct
{
	ST_CHAR  fc_str[MAX_FC_LEN+1];
	ST_INT   fcda_num;
} ACSI_FC_CTRL;

//�м�ṹ������ln��FC�����ļ���
static ACSI_FC_CTRL  fc_ctrls[MAX_FC_NUM] = {0};

ST_INT        g_ied_num;   //װ�ø���
ACSI_IED_CTRL *g_ied_ctrls;//����װ��ied

static ST_VOID _create_ln(ACSI_LN_CTRL* ln_ctrl, SCL_LN* scl_ln, SCL_INFO *data_type, ACSI_COMMON_OBJ* parent);
static ST_VOID _create_ld(ACSI_LD_CTRL* ld_ctrl, SCL_LD* scl_ld, SCL_INFO *data_type, ACSI_COMMON_OBJ* parent);
static ST_VOID _create_ied(ACSI_IED_CTRL* ied_ctrl, SCL_INFO* scl_ied, SCL_INFO *data_type);
static ST_VOID _set_netInfo_ied(ACSI_IED_CTRL* ied_ctrl, SCL_SUBNET *subnet_head_list);
static ST_VOID _set_dataset_ied(ACSI_IED_CTRL* ied_ctrl, SCL_INFO* scl_ied, SCL_INFO *data_type);
static AI_VAL_TYPE _get_val_type(ST_CHAR *type_str);

/**
* ��doTypeHead�в���ƥ���dotype
*/
static SCL_DOTYPE * _get_do_type(ST_CHAR *type_id, SCL_DOTYPE * doTypeHead)
{
	SCL_DOTYPE * do_type = NULL;

	do_type = (SCL_DOTYPE *)list_find_last((DBL_LNK *)doTypeHead);
	while(do_type)
	{
		if ( strcmp( do_type->id, type_id)== 0 )
		{
			return do_type;
		}
		do_type =  (SCL_DOTYPE *)list_find_prev( (DBL_LNK *)doTypeHead, (DBL_LNK *)do_type );
	}
	return NULL;
}

/**
* ����scd_info������ied�Ķ���ռ�
*/
ST_RET acsi_create_ied_all(SCD_INFO *scd_info)
{
	ST_RET ret = SD_FAILURE;
	ACSI_IED_CTRL* ied_ctrl = NULL;
	ST_INT i;

	if (scd_info == NULL)
	{
		return SD_FAILURE;
	}
	g_ied_num = scd_info->ied_num;
	g_ied_ctrls = chk_calloc(g_ied_num, sizeof(ACSI_IED_CTRL));
	for (i=0; i<g_ied_num; i++)
	{
		ied_ctrl = &g_ied_ctrls[i];
		//����һ��ied
		_create_ied(ied_ctrl, &scd_info->ieds[i], scd_info->data_type);
		//����ied���������ݼ�
		_set_dataset_ied(ied_ctrl, &scd_info->ieds[i], scd_info->data_type);
		//����������Ϣ
		_set_netInfo_ied(ied_ctrl, scd_info->subnet_head);
	}
	return SD_SUCCESS;
}

/**
* ����FC���м�ṹfc_ctrls
*/
static ST_VOID _save_fc_ctrl(ST_CHAR *fc)
{
	ST_INT i=0;
	for (i=0; i<MAX_FC_NUM; i++)
	{
		if (fc_ctrls[i].fcda_num == 0)
		{
			strcpy(fc_ctrls[i].fc_str, fc);
			fc_ctrls[i].fcda_num ++;
			break;
		}
		else
		{
			if (strcmp(fc_ctrls[i].fc_str, fc) == 0)
			{
				fc_ctrls[i].fcda_num ++;
				break;
			}
		}
	}
}

/**
* ����dotype��FC��Ϣ���浽�м�ṹfc_ctrls��
* doType    do����
* data_type ������������
* ����ֵ    ��
*/
static ST_VOID _count_fc_do(SCL_DOTYPE * doType, SCL_INFO *data_type)
{
	SCL_DA      * scl_da = NULL;
	SCL_DOTYPE * do_type = NULL;

	scl_da = (SCL_DA*)list_find_last((DBL_LNK *)doType->daHead);
	while(scl_da)
	{
		if (scl_da->objtype == SCL_OBJTYPE_SDO)
		{
			do_type = _get_do_type(scl_da->type, data_type->doTypeHead);
			if (do_type)
			{
				//Ƕ��
				_count_fc_do(do_type, data_type);
			}
		}
		else
		{
			//����FC��Ϣ
			_save_fc_ctrl(scl_da->fc);
		}
		scl_da = (SCL_DA*)list_find_prev((DBL_LNK *)doType->daHead, (DBL_LNK *)scl_da);
	}
}

/**
* �����м�ṹfc_ctrls����ln��fc������������BR��RP�ȿ��ƿ�FC
* lnType    ln����
* data_type ������������
* ����ֵ    ln��fc����
*/
static ST_INT _get_fc_num_ln(SCL_LNTYPE * lnType, SCL_INFO *data_type)
{
	SCL_DOTYPE * doType = NULL; 
	SCL_DO      * scl_do = NULL;
	SCL_DA      * scl_da = NULL;
	ST_INT      i = 0;
	ST_INT      fcNum_ln = 0;
	
	//FC��da�����ԣ���Ҫ������da����ȷ��������ЩFC
	scl_do = (SCL_DO *)list_find_last((DBL_LNK *)lnType->doHead);
	while ( scl_do )
	{
		doType = _get_do_type(scl_do->type, data_type->doTypeHead);
		if (doType)
		{
			//����lntype��ÿһ��do����FC��Ϣ���м�ṹfc_ctrls��
			_count_fc_do(doType, data_type);
		}			
		scl_do = (SCL_DO *)list_find_prev( (DBL_LNK *)lnType->doHead, (DBL_LNK *)scl_do );
	}

	//�м�ṹfc_ctrls�б�����fc��Ϣ
	for (i=0; i<MAX_FC_NUM; i++)
	{
		if (fc_ctrls[i].fcda_num > 0)
		{
			fcNum_ln ++;
		}
	}
	return fcNum_ln;
}

/**
* �ж�do�ڲ���fc��
* fc		   FC�ַ���
* do_type      dotype
* data_type    ������������
* ����ֵ    TRUE��FALSE
*/
static ST_BOOLEAN _fcda_do(ST_CHAR *fc, SCL_DOTYPE *do_type, SCL_INFO *data_type)
{
	SCL_DA *scl_da = NULL;
	SCL_DOTYPE *sub_do_type = NULL;

	scl_da = (SCL_DA *)list_find_last((DBL_LNK *)do_type->daHead);
	while(scl_da)
	{
		if (scl_da->objtype == SCL_OBJTYPE_SDO)
		{
			sub_do_type = _get_do_type(scl_da->type, data_type->doTypeHead);
			if (sub_do_type)
			{
				//�ݹ����
				return _fcda_do(fc, sub_do_type, data_type);
			}	
		}
		else
		{
			if (strcmp(fc, scl_da->fc) == 0)
			{
				return SD_TRUE;
			}
		}
		scl_da = (SCL_DA *)list_find_prev((DBL_LNK *)do_type->daHead, (DBL_LNK *)scl_da);
	}
	return SD_FALSE;
}

/**
* ����fc��һ���Ա����
* fc		   FC�ַ���
* doHead       ln�µ�do����
* data_type    ������������
* ����ֵ    FC��һ��do����
*/
static ST_INT _get_sub_do_num_fc(ST_CHAR *fc, SCL_DO *doHead, SCL_INFO *data_type)
{
	ST_INT do_num_fc = 0;
	SCL_DO *scl_do = NULL;
	SCL_DOTYPE *do_type = NULL;
	
	scl_do = (SCL_DO *)list_find_last((DBL_LNK *)doHead);
	while(scl_do)
	{
		do_type = _get_do_type(scl_do->type, data_type->doTypeHead);
		if (do_type)
		{
			if (_fcda_do(fc, do_type, data_type))//�ж�do�Ƿ�ƥ��FC
			{
				do_num_fc ++;
			}
		}
		scl_do = (SCL_DO *)list_find_prev((DBL_LNK *)doHead, (DBL_LNK *)scl_do);
	}
	return do_num_fc;
}
/**
* ����DO��һ���Ա������ƥ��FC
* fc		   FC�ַ���
* daHead       da����
* data_type    ������������
* ����ֵ       do��һ��ƥ��fc��do����
*/
static ST_INT _get_sub_fcd_num_do(ST_CHAR *fc, SCL_DA *daHead, SCL_INFO *data_type)
{
	ST_INT fcd_num = 0;
	SCL_DA *scl_da = NULL;
	SCL_DOTYPE *do_type = NULL;

	scl_da = (SCL_DA *)list_find_last((DBL_LNK *)daHead);
	while(scl_da)
	{
		if (scl_da->objtype == SCL_OBJTYPE_SDO)//do,�ݹ�
		{
			do_type = _get_do_type(scl_da->type, data_type->doTypeHead);
			if (do_type)
			{
				if (_fcda_do(fc, do_type, data_type))//�ж�do�Ƿ�ƥ��FC
				{
					fcd_num ++;
				}
			}
		}
		else
		{
			if (strcmp(scl_da->fc, fc) == 0)
			{
				fcd_num ++;
			}
		}
		scl_da = (SCL_DA *)list_find_prev((DBL_LNK *)daHead, (DBL_LNK *)scl_da);
	}
	return fcd_num;
}

/**
* ����fcda
* fcda_ctrl    ����da
* da_type       da����
* data_type    ������������
* ����ֵ    ��
*/
static ST_VOID _create_fcda(ACSI_OBJ_CTRL* fcda_ctrl,SCL_DATYPE *da_type, SCL_INFO *data_type)
{
	SCL_DATYPE *sub_da_type = NULL;
	SCL_BDA * scl_bda = NULL;
	ST_INT  bda_num = 0;
	ST_INT i =0;
	ACSI_OBJ_CTRL* sub_fcda_ctrl = NULL;

	scl_bda = (SCL_BDA *)list_find_last((DBL_LNK *)da_type->bdaHead);
	while(scl_bda)
	{
		bda_num ++;
		scl_bda = (SCL_BDA *)list_find_prev((DBL_LNK *)da_type->bdaHead, (DBL_LNK *)scl_bda);
	}
	if (bda_num > 0)
	{
		fcda_ctrl->sub_obj_num = bda_num;
		fcda_ctrl->sub_objs = chk_calloc(fcda_ctrl->sub_obj_num, sizeof(ACSI_OBJ_CTRL));
		scl_bda = (SCL_BDA *)list_find_last((DBL_LNK *)da_type->bdaHead);
		while(scl_bda)
		{
			sub_fcda_ctrl = &fcda_ctrl->sub_objs[i];
			sub_fcda_ctrl->comn_obj.parent = &fcda_ctrl->comn_obj;
			strcpy(sub_fcda_ctrl->comn_obj.name, scl_bda->name);
			sub_fcda_ctrl->comn_obj.desc = NULL;
			sub_fcda_ctrl->comn_obj.obj_type = OBJECT_TYPE_FCDA;
			if (strlen(scl_bda->desc) > 0)
			{
				sub_fcda_ctrl->comn_obj.desc = chk_calloc(1, strlen(scl_bda->desc)+1);
				strcpy(sub_fcda_ctrl->comn_obj.desc, scl_bda->desc);
			}
			if (strcmp(scl_bda->bType, "Struct") == 0)//struct���ݹ���ã�����fcda
			{
				sub_fcda_ctrl->type = VAL_TYPE_STRUCT;
				sub_da_type = (SCL_DATYPE *)list_find_last((DBL_LNK *)data_type->daTypeHead);
				while(sub_da_type)
				{
					if (strcmp(sub_da_type->id, scl_bda->type) == 0)
					{
						break;
					}
					sub_da_type = (SCL_DATYPE *)list_find_prev((DBL_LNK *)data_type->daTypeHead, (DBL_LNK *)sub_da_type);
				}
				_create_fcda(sub_fcda_ctrl, sub_da_type, data_type);
			}
			else
			{
				sub_fcda_ctrl->sub_obj_num = 0;
				sub_fcda_ctrl->sub_objs = NULL;
				sub_fcda_ctrl->type = _get_val_type(scl_bda->type);
			}
			scl_bda = (SCL_BDA *)list_find_prev((DBL_LNK *)da_type->bdaHead, (DBL_LNK *)scl_bda);
			i++;
		}
	}
}

/**
* ����fc�µ�do
* fcdo_ctrl    ����do
* doType       do����
* data_type    ������������
* fc		   fc�ַ���
* ����ֵ    ��
*/
static ST_VOID _create_fcdo(ACSI_OBJ_CTRL* fcdo_ctrl, SCL_DOTYPE * doType, SCL_INFO *data_type, ST_CHAR *fc)
{
	ST_INT i = 0;
	ACSI_OBJ_CTRL *sub_fcd_ctrl = NULL;
	SCL_DA        *scl_da = NULL;
	SCL_DOTYPE    *sdo_type = NULL;
	SCL_DATYPE *sub_da_type = NULL;

	fcdo_ctrl->sub_obj_num = _get_sub_fcd_num_do(fc, doType->daHead, data_type);//���dotype�·���fc����һ���Ա����
	if (fcdo_ctrl->sub_obj_num > 0)
	{
		fcdo_ctrl->sub_objs = chk_calloc(fcdo_ctrl->sub_obj_num, sizeof(ACSI_OBJ_CTRL));

		scl_da = (SCL_DA *)list_find_last((DBL_LNK *)doType->daHead);
		while(scl_da)
		{
			sub_fcd_ctrl = &fcdo_ctrl->sub_objs[i];
			if (scl_da->objtype == SCL_OBJTYPE_SDO)//sdo �ݹ����_create_fcdo
			{
				sdo_type = _get_do_type(scl_da->type, data_type->doTypeHead);
				if (sdo_type)
				{
					if (_fcda_do(fc, sdo_type, data_type))//�Ƿ�ƥ��fc
					{
						sub_fcd_ctrl->comn_obj.obj_type = OBJECT_TYPE_FCD;
						sub_fcd_ctrl->comn_obj.parent = &fcdo_ctrl->comn_obj;
						strcpy(sub_fcd_ctrl->comn_obj.name, scl_da->name);
						if (strlen(scl_da->desc) == 0)
						{
							sub_fcd_ctrl->comn_obj.desc = NULL;
						}
						else
						{
							sub_fcd_ctrl->comn_obj.desc = chk_calloc(1, strlen(scl_da->desc)+1);
							strcpy(sub_fcd_ctrl->comn_obj.desc, scl_da->desc);
						}
						_create_fcdo(sub_fcd_ctrl, sdo_type, data_type, fc);//����fcdo
						i++;
					}
				}
			}
			else
			{
				if (strcmp(scl_da->fc, fc) == 0)//�Ƿ�ƥ��fc
				{
					sub_fcd_ctrl->comn_obj.parent = &fcdo_ctrl->comn_obj;
					strcpy(sub_fcd_ctrl->comn_obj.name, scl_da->name);
					sub_fcd_ctrl->comn_obj.desc = NULL;
					sub_fcd_ctrl->comn_obj.obj_type = OBJECT_TYPE_FCDA;
					if (strlen(scl_da->desc) > 0)
					{
						sub_fcd_ctrl->comn_obj.desc = chk_calloc(1, strlen(scl_da->desc)+1);
						strcpy(sub_fcd_ctrl->comn_obj.desc, scl_da->desc);
					}
					if (strcmp(scl_da->bType, "Struct") == 0)//�������͵�fcda
					{
						sub_fcd_ctrl->type = VAL_TYPE_STRUCT;
						sub_da_type = (SCL_DATYPE *)list_find_last((DBL_LNK *)data_type->daTypeHead);
						while(sub_da_type)
						{
							if (strcmp(sub_da_type->id, scl_da->type) == 0)
							{
								break;
							}
							sub_da_type = (SCL_DATYPE *)list_find_prev((DBL_LNK *)data_type->daTypeHead, (DBL_LNK *)sub_da_type);
						}
						_create_fcda(sub_fcd_ctrl, sub_da_type, data_type);//����fcda
					}
					else//������
					{
						sub_fcd_ctrl->sub_obj_num = 0;
						sub_fcd_ctrl->sub_objs = NULL;
						sub_fcd_ctrl->type = _get_val_type(scl_da->type);
					}
					i++;
				}
			}
			scl_da = (SCL_DA *)list_find_prev((DBL_LNK *)doType->daHead, (DBL_LNK *)scl_da);
		}
	}	
}

/**
* ����fc��fc�³�Ա
* obj_ctrl_fc  fc�Ķ���ռ䣬��do��da������ͬ�Ľṹ
* lnType       ln����
* data_type    ������������
* parent       �������common_objָ��
* ����ֵ    ��
*/
static ST_VOID _create_fc(ACSI_OBJ_CTRL* obj_ctrl_fc, SCL_LNTYPE * lnType, SCL_INFO *data_type,  ACSI_COMMON_OBJ* parent)
{
	ST_INT i = 0;
	ACSI_OBJ_CTRL* sub_obj_ctrl = NULL;
	SCL_DO     *scl_do = NULL;
	SCL_DOTYPE *do_type = NULL;

	obj_ctrl_fc->comn_obj.obj_type = OBJECT_TYPE_FCN;
	obj_ctrl_fc->comn_obj.desc = NULL;
	obj_ctrl_fc->comn_obj.parent = parent;
	obj_ctrl_fc->sub_obj_num = _get_sub_do_num_fc(obj_ctrl_fc->comn_obj.name, lnType->doHead, data_type);//����fc��һ���Ա����
	if (obj_ctrl_fc->sub_obj_num > 0)
	{
		obj_ctrl_fc->sub_objs = chk_calloc(obj_ctrl_fc->sub_obj_num, sizeof(ACSI_OBJ_CTRL));
		scl_do = (SCL_DO *)list_find_last((DBL_LNK *)lnType->doHead);
		while(scl_do)
		{
			sub_obj_ctrl = &obj_ctrl_fc->sub_objs[i];

			do_type = _get_do_type(scl_do->type, data_type->doTypeHead);
			if (do_type)
			{
				if (_fcda_do(obj_ctrl_fc->comn_obj.name, do_type, data_type))//�ж�do�ڲ���fc��
				{
					sub_obj_ctrl->comn_obj.obj_type = OBJECT_TYPE_FCD;
					strcpy(sub_obj_ctrl->comn_obj.name, scl_do->name);
					if (strlen(scl_do->desc) == 0)
					{
						sub_obj_ctrl->comn_obj.desc = NULL;
					}
					else
					{
						sub_obj_ctrl->comn_obj.desc = chk_calloc(1, strlen(scl_do->desc)+1);
						strcpy(sub_obj_ctrl->comn_obj.desc, scl_do->desc);
					}
					sub_obj_ctrl->comn_obj.parent = &obj_ctrl_fc->comn_obj;
					_create_fcdo(sub_obj_ctrl, do_type, data_type, obj_ctrl_fc->comn_obj.name);//����do
					i++;
				}
			}
			scl_do = (SCL_DO *)list_find_prev((DBL_LNK *)lnType->doHead, (DBL_LNK *)scl_do);
		}
	}
}
//��ȡln�¿��ƿ����͸���
static ST_INT _get_cb_type_num(SCL_LN* scl_ln)
{
	ST_INT cb_num = 0;
	ST_BOOLEAN br = SD_FALSE;
	ST_BOOLEAN rp = SD_FALSE;
	SCL_RCB    *rcb = NULL;


	if (scl_ln->sgcbPtr)
	{
		cb_num ++;
	}
	if (scl_ln->lcbHead)
	{
		cb_num ++;
	}
	if (scl_ln->gcbHead)
	{
		cb_num ++;
	}
	if (scl_ln->rcbHead)
	{
		rcb = (SCL_RCB *)list_find_last((DBL_LNK *)scl_ln->rcbHead);
		while(rcb)
		{
			if (rcb->buffered)
			{
				br = SD_TRUE;
			}
			else
				rp = SD_TRUE;
			rcb = (SCL_RCB *)list_find_prev((DBL_LNK *)scl_ln->rcbHead, (DBL_LNK *)rcb);
		}
		if (br&&rp)
		{
			cb_num = cb_num + 2;
		}
		else
		{
			cb_num ++;
		}
	}
	return cb_num;
}
//��ȡln��brcb����urcb������ƿ������
static ST_INT _get_rcb_num(SCL_LN* scl_ln, ST_BOOLEAN br)
{
	ST_INT rcb_num = 0;
	SCL_RCB *rcb = NULL;

	rcb = (SCL_RCB *)list_find_last((DBL_LNK *)scl_ln->rcbHead);
	while(rcb)
	{
		if (rcb->buffered == br)
		{
			rcb_num ++;
			rcb_num = rcb_num + rcb->max_num;
		}
		rcb = (SCL_RCB *)list_find_prev((DBL_LNK *)scl_ln->rcbHead, (DBL_LNK *)rcb);
	}
	return rcb_num;
}

//����rcb���ԣ����չ�Լ�涨�������ƿ����������ԣ���Ҫ�ο�61850��Լ
static ST_VOID _create_rcb_attr(ACSI_OBJ_CTRL* rcb_ctrl, SCL_RCB *scl_rcb, ACSI_COMMON_OBJ* parent, ST_BOOLEAN br)
{
	ST_INT i=0;

	if (strlen(scl_rcb->desc) > 0 )
	{
		rcb_ctrl->comn_obj.desc = chk_calloc(1,strlen(scl_rcb->desc)+1);
		strcpy(rcb_ctrl->comn_obj.desc, scl_rcb->desc);
	}
	rcb_ctrl->comn_obj.obj_type = OBJECT_TYPE_CB;
	rcb_ctrl->comn_obj.parent = parent;
	rcb_ctrl->type = VAL_TYPE_STRUCT;
	if (br)
	{
		rcb_ctrl->sub_obj_num = 13;
	}
	else
	{
		rcb_ctrl->sub_obj_num = 11;
	}

	rcb_ctrl->sub_objs = chk_calloc(rcb_ctrl->sub_obj_num, sizeof(ACSI_OBJ_CTRL));
	for (i=0; i<rcb_ctrl->sub_obj_num; i++)
	{
		rcb_ctrl->sub_objs[i].sub_obj_num = 0;
		rcb_ctrl->sub_objs[i].sub_objs = NULL;
		rcb_ctrl->sub_objs[i].comn_obj.obj_type = OBJECT_TYPE_FCDA;
		rcb_ctrl->sub_objs[i].comn_obj.desc = NULL;
		rcb_ctrl->sub_objs[i].comn_obj.parent = &rcb_ctrl->comn_obj;
	}
	strcpy(rcb_ctrl->sub_objs[0].comn_obj.name, "RptID");
	rcb_ctrl->sub_objs[0].type = VAL_TYPE_VSTRING65;
	rcb_ctrl->sub_objs[0].val = chk_calloc(1,MAX_IDENT_LEN +1);
	if (strlen(scl_rcb->rptID) > 0)
	{
		strcpy(rcb_ctrl->sub_objs[0].val, scl_rcb->rptID);//RptID��val��Ҫ���⴦��
	}
	strcpy(rcb_ctrl->sub_objs[1].comn_obj.name, "RptEna");
	rcb_ctrl->sub_objs[1].type = VAL_TYPE_BOOLEAN;
	if (br)
	{
		strcpy(rcb_ctrl->sub_objs[2].comn_obj.name, "DataSet");
		rcb_ctrl->sub_objs[2].type = VAL_TYPE_VSTRING65;
		rcb_ctrl->sub_objs[2].val = chk_calloc(1,MAX_IDENT_LEN +1);
		if (strlen(scl_rcb->datSet) > 0)
		{
			strcpy(rcb_ctrl->sub_objs[2].val, scl_rcb->datSet);//DataSet��val��Ҫ���⴦��
		}
		strcpy(rcb_ctrl->sub_objs[3].comn_obj.name, "ConfRev");
		rcb_ctrl->sub_objs[3].type = VAL_TYPE_INT32U;
		strcpy(rcb_ctrl->sub_objs[4].comn_obj.name, "OptFlds");
		rcb_ctrl->sub_objs[4].type = VAL_TYPE_OPTFLDS;
		strcpy(rcb_ctrl->sub_objs[5].comn_obj.name, "BufTm");
		rcb_ctrl->sub_objs[5].type = VAL_TYPE_INT32U;
		strcpy(rcb_ctrl->sub_objs[6].comn_obj.name, "SqNum");
		rcb_ctrl->sub_objs[6].type = VAL_TYPE_INT16U;
		strcpy(rcb_ctrl->sub_objs[7].comn_obj.name, "TrgOps");
		rcb_ctrl->sub_objs[7].type = VAL_TYPE_TRGC;
		strcpy(rcb_ctrl->sub_objs[8].comn_obj.name, "IntgPd");
		rcb_ctrl->sub_objs[8].type = VAL_TYPE_INT32U;
		strcpy(rcb_ctrl->sub_objs[9].comn_obj.name, "GI");
		rcb_ctrl->sub_objs[9].type = VAL_TYPE_BOOLEAN;
		strcpy(rcb_ctrl->sub_objs[10].comn_obj.name, "PurgeBuf");
		rcb_ctrl->sub_objs[10].type = VAL_TYPE_BOOLEAN;
		strcpy(rcb_ctrl->sub_objs[11].comn_obj.name, "EntryID");
		rcb_ctrl->sub_objs[11].type = VAL_TYPE_ENTRYID;
		strcpy(rcb_ctrl->sub_objs[12].comn_obj.name, "TimeofEntry");
		rcb_ctrl->sub_objs[12].type = VAL_TYPE_ENTRYTIME;
	}
	else
	{
		strcpy(rcb_ctrl->sub_objs[2].comn_obj.name, "Resv");
		rcb_ctrl->sub_objs[2].type = VAL_TYPE_BOOLEAN;
		strcpy(rcb_ctrl->sub_objs[3].comn_obj.name, "DataSet");
		rcb_ctrl->sub_objs[3].type = VAL_TYPE_VSTRING65;
		rcb_ctrl->sub_objs[3].val = chk_calloc(1,MAX_IDENT_LEN +1);
		if (strlen(scl_rcb->datSet) > 0)
		{
			strcpy(rcb_ctrl->sub_objs[3].val, scl_rcb->datSet);//DataSet��val��Ҫ���⴦��
		}
		strcpy(rcb_ctrl->sub_objs[4].comn_obj.name, "ConfRev");
		rcb_ctrl->sub_objs[4].type = VAL_TYPE_INT32U;
		strcpy(rcb_ctrl->sub_objs[5].comn_obj.name, "OptFlds");
		rcb_ctrl->sub_objs[5].type = VAL_TYPE_OPTFLDS;
		strcpy(rcb_ctrl->sub_objs[6].comn_obj.name, "BufTm");
		rcb_ctrl->sub_objs[6].type = VAL_TYPE_INT32U;
		strcpy(rcb_ctrl->sub_objs[7].comn_obj.name, "SqNum");
		rcb_ctrl->sub_objs[7].type = VAL_TYPE_INT16U;
		strcpy(rcb_ctrl->sub_objs[8].comn_obj.name, "TrgOps");
		rcb_ctrl->sub_objs[8].type = VAL_TYPE_TRGC;
		strcpy(rcb_ctrl->sub_objs[9].comn_obj.name, "IntgPd");
		rcb_ctrl->sub_objs[9].type = VAL_TYPE_INT32U;
		strcpy(rcb_ctrl->sub_objs[10].comn_obj.name, "GI");
		rcb_ctrl->sub_objs[10].type = VAL_TYPE_BOOLEAN;
	}
}
//��ȡlcbnum
static ST_INT _get_lcb_num(SCL_LN* scl_ln)
{
	ST_INT lcb_num = 0;
	SCL_LCB *lcb = NULL;

	lcb = (SCL_LCB *)list_find_last((DBL_LNK *)scl_ln->lcbHead);
	while(lcb)
	{
		lcb_num ++;
		lcb = (SCL_LCB *)list_find_prev((DBL_LNK *)scl_ln->lcbHead, (DBL_LNK *)lcb);
	}
	return lcb_num;
}
//��ȡgocbnum
static ST_INT _get_gocb_num(SCL_LN* scl_ln)
{
	ST_INT gocb_num = 0;
	SCL_GCB *gocb = NULL;

	gocb = (SCL_GCB *)list_find_last((DBL_LNK *)scl_ln->gcbHead);
	while(gocb)
	{
		gocb_num ++;
		gocb = (SCL_GCB *)list_find_prev((DBL_LNK *)scl_ln->gcbHead, (DBL_LNK *)gocb);
	}
	return gocb_num;
}

/**
* ����������ƿ�
* cb_ctrl   cb����ռ�
* scl_ln    �����ļ�����ln
* parent    �������common_objָ��
* br        �Ƿ�br
* ����ֵ    ��
*/
static ST_VOID _create_rcb(ACSI_CB_CTRL* cb_ctrl, SCL_LN* scl_ln, ACSI_COMMON_OBJ* parent, ST_BOOLEAN br)
{
	ST_INT i=0;
	ST_INT j=0;
	ACSI_OBJ_CTRL *acsi_rcb = NULL;
	SCL_RCB *scl_rcb = NULL;
	ST_CHAR num_str[3] = {0};

	if (br)
	{
		strcpy(cb_ctrl->comn_obj.name, "BR");
		cb_ctrl->cb_type = CB_TYPE_BR;
	}
	else
	{
		strcpy(cb_ctrl->comn_obj.name, "RP");
		cb_ctrl->cb_type = CB_TYPE_RP;
	}
	cb_ctrl->comn_obj.desc = NULL;
	cb_ctrl->comn_obj.obj_type = OBJECT_TYPE_FCN;
	cb_ctrl->comn_obj.parent = parent;
	
	cb_ctrl->cb_num = _get_rcb_num(scl_ln, br);//��ȡbr����rp������ƿ����
	cb_ctrl->cb_ctrls = chk_calloc(cb_ctrl->cb_num, sizeof(ACSI_OBJ_CTRL));

	scl_rcb = (SCL_RCB *)list_find_last((DBL_LNK *)scl_ln->rcbHead);
	while(scl_rcb)
	{
		if (scl_rcb->buffered == br)
		{
			acsi_rcb = &cb_ctrl->cb_ctrls[i];
			strcpy(acsi_rcb->comn_obj.name, scl_rcb->name);
			_create_rcb_attr(acsi_rcb, scl_rcb, &cb_ctrl->comn_obj,br);//��������ʵ���ŵı�����ƿ�����
			i++;
			//������ʵ���ŵı�����ƿ�����
			for (j=1; j<scl_rcb->max_num+1; j++)
			{
				acsi_rcb = &cb_ctrl->cb_ctrls[i];
				memset(num_str, 0, 3);
				sprintf(num_str, "%02d", j);
				strcpy(acsi_rcb->comn_obj.name, scl_rcb->name);
				strcat(acsi_rcb->comn_obj.name, num_str);
				_create_rcb_attr(acsi_rcb, scl_rcb, &cb_ctrl->comn_obj,br);//����rcb����
				i++;
			}
		}
		scl_rcb = (SCL_RCB *)list_find_prev((DBL_LNK *)scl_ln->rcbHead, (DBL_LNK *)scl_rcb);
	}
}
//��rcb��cb���øú�������
static ST_VOID _create_other_cb(ACSI_CB_CTRL* cb_ctrl, SCL_LN* scl_ln, ACSI_COMMON_OBJ* parent, ACSI_CB_TYPE type)
{
	ST_INT i=0;
	ST_INT j=0;
	ACSI_OBJ_CTRL *sgcb = NULL;
	ACSI_OBJ_CTRL *sgcb_attr = NULL;
	ACSI_OBJ_CTRL *lcb_attr = NULL;
	ACSI_OBJ_CTRL *gcb_attr = NULL;
	SCL_LCB       *lcb = NULL;
	SCL_GCB       *gcb = NULL;
	ACSI_OBJ_CTRL *obj_lcb = NULL;
	ACSI_OBJ_CTRL *obj_gcb = NULL;

	cb_ctrl->comn_obj.desc = NULL;
	cb_ctrl->comn_obj.obj_type = OBJECT_TYPE_FCN;
	cb_ctrl->comn_obj.parent = parent;
	switch(type)
	{
	case CB_TYPE_SG:
		strcpy(cb_ctrl->comn_obj.name, "SP");//����sgcb����Ҫ�ο�61850��Լ
		cb_ctrl->cb_type = CB_TYPE_SG;
		cb_ctrl->cb_num = 1;
		cb_ctrl->cb_ctrls = chk_calloc(cb_ctrl->cb_num, sizeof(ACSI_OBJ_CTRL));
		sgcb = &cb_ctrl->cb_ctrls[0];
		sgcb->comn_obj.obj_type = OBJECT_TYPE_CB;
		sgcb->comn_obj.parent = &cb_ctrl->comn_obj;
		sgcb->comn_obj.desc = NULL;
		if (strlen(scl_ln->sgcbPtr->desc) > 0)
		{
			sgcb->comn_obj.desc = chk_calloc(1,strlen(scl_ln->sgcbPtr->desc)+1);
			strcpy(sgcb->comn_obj.desc, scl_ln->sgcbPtr->desc);
		}
		strcpy(sgcb->comn_obj.name, "SGCB");
		sgcb->type = VAL_TYPE_STRUCT;
		sgcb->sub_obj_num = 5;
		sgcb->sub_objs = chk_calloc(5, sizeof(ACSI_OBJ_CTRL));
		for (i=0; i<5; i++)
		{
			sgcb_attr = &sgcb->sub_objs[i];
			sgcb_attr->sub_obj_num = 0;
			sgcb_attr->sub_objs = NULL;
			sgcb_attr->comn_obj.obj_type = OBJECT_TYPE_FCDA;
			sgcb_attr->comn_obj.desc = NULL;
			sgcb_attr->comn_obj.parent = &(sgcb->comn_obj);
		}
		strcpy(sgcb->sub_objs[0].comn_obj.name, "NumOfSG");
		sgcb->sub_objs[0].type = VAL_TYPE_INT8U;
		strcpy(sgcb->sub_objs[1].comn_obj.name, "ActSG");
		sgcb->sub_objs[1].type = VAL_TYPE_INT8U;
		strcpy(sgcb->sub_objs[2].comn_obj.name, "EditSG");
		sgcb->sub_objs[2].type = VAL_TYPE_INT8U;
		strcpy(sgcb->sub_objs[3].comn_obj.name, "CnfEdit");
		sgcb->sub_objs[3].type = VAL_TYPE_BOOLEAN;
		strcpy(sgcb->sub_objs[4].comn_obj.name, "LActTm");
		sgcb->sub_objs[4].type = VAL_TYPE_TIMESTAMP;
		break;
	case CB_TYPE_LOG://������־���ƿ飬��Ҫ�ο�61850��Լ
		strcpy(cb_ctrl->comn_obj.name, "LG");
		cb_ctrl->cb_type = CB_TYPE_LOG;

		cb_ctrl->cb_num = _get_lcb_num(scl_ln);
		cb_ctrl->cb_ctrls = chk_calloc(cb_ctrl->cb_num, sizeof(ACSI_OBJ_CTRL)); 
		lcb = (SCL_LCB *)list_find_last((DBL_LNK *)scl_ln->lcbHead);
		i = 0;
		while(lcb)
		{
			obj_lcb = &cb_ctrl->cb_ctrls[i];
			obj_lcb->type = VAL_TYPE_STRUCT;
			strcpy(obj_lcb->comn_obj.name, lcb->name);
			if (strlen(lcb->desc) > 0)
			{
				obj_lcb->comn_obj.desc = chk_calloc(1,strlen(lcb->desc) + 1);
				strcpy(obj_lcb->comn_obj.desc, lcb->desc);
			}
			obj_lcb->comn_obj.obj_type = OBJECT_TYPE_CB;
			obj_lcb->comn_obj.parent = &cb_ctrl->comn_obj;
			obj_lcb->sub_obj_num = 9;
			obj_lcb->sub_objs = chk_calloc(obj_lcb->sub_obj_num, sizeof(ACSI_OBJ_CTRL));
			for (j=0; j<9; j++)
			{
				lcb_attr = &obj_lcb->sub_objs[j];
				lcb_attr->sub_obj_num = 0;
				lcb_attr->sub_objs = NULL;
				lcb_attr->comn_obj.obj_type = OBJECT_TYPE_FCDA;
				lcb_attr->comn_obj.desc = NULL;
				lcb_attr->comn_obj.parent = &(obj_lcb->comn_obj);
			}
			strcpy(obj_lcb->sub_objs[0].comn_obj.name, "LogEna");
			obj_lcb->sub_objs[0].type = VAL_TYPE_BOOLEAN;
			strcpy(obj_lcb->sub_objs[1].comn_obj.name, "LogRef");
			obj_lcb->sub_objs[1].type = VAL_TYPE_VSTRING65;
			strcpy(obj_lcb->sub_objs[2].comn_obj.name, "DataSet");
			obj_lcb->sub_objs[2].type = VAL_TYPE_VSTRING65;
			obj_lcb->sub_objs[2].val = chk_calloc(1,MAX_IDENT_LEN +1);
			if (strlen(lcb->datSet) > 0)
			{
				strcpy(obj_lcb->sub_objs[2].val, lcb->datSet);//���⴦��
			}
			strcpy(obj_lcb->sub_objs[3].comn_obj.name, "OldEntrTim");
			obj_lcb->sub_objs[3].type = VAL_TYPE_TIMESTAMP;
			strcpy(obj_lcb->sub_objs[4].comn_obj.name, "NewEntrTim");
			obj_lcb->sub_objs[4].type = VAL_TYPE_TIMESTAMP;
			strcpy(obj_lcb->sub_objs[5].comn_obj.name, "OldEntr");
			obj_lcb->sub_objs[5].type = VAL_TYPE_ENTRYID;
			strcpy(obj_lcb->sub_objs[6].comn_obj.name, "NewEntr");
			obj_lcb->sub_objs[6].type = VAL_TYPE_ENTRYID;
			strcpy(obj_lcb->sub_objs[7].comn_obj.name, "TrgOps");
			obj_lcb->sub_objs[7].type = VAL_TYPE_TRGC;
			strcpy(obj_lcb->sub_objs[8].comn_obj.name, "IntgPd");
			obj_lcb->sub_objs[8].type = VAL_TYPE_INT32U;
			i++;
			lcb = (SCL_LCB *)list_find_prev((DBL_LNK *)scl_ln->lcbHead, (DBL_LNK *)lcb);
		}
		break;
	case CB_TYPE_GO://����goose���ƿ飬��Ҫ�ο�61850��Լ
		strcpy(cb_ctrl->comn_obj.name, "GO");
		cb_ctrl->cb_type = CB_TYPE_GO;

		cb_ctrl->cb_num = _get_gocb_num(scl_ln);
		cb_ctrl->cb_ctrls = chk_calloc(cb_ctrl->cb_num, sizeof(ACSI_OBJ_CTRL)); 
		gcb = (SCL_GCB *)list_find_last((DBL_LNK *)scl_ln->gcbHead);
		while(gcb)
		{
			obj_gcb = &cb_ctrl->cb_ctrls[i];
			obj_gcb->type = VAL_TYPE_STRUCT;
			strcpy(obj_gcb->comn_obj.name, gcb->name);
			if (strlen(gcb->desc) > 0)
			{
				obj_gcb->comn_obj.desc = chk_calloc(1,strlen(gcb->desc) + 1);
				strcpy(obj_gcb->comn_obj.desc, gcb->desc);
			}
			obj_gcb->comn_obj.obj_type = OBJECT_TYPE_CB;
			obj_gcb->comn_obj.parent = &cb_ctrl->comn_obj;
			obj_gcb->sub_obj_num = 6;
			obj_gcb->sub_objs = chk_calloc(obj_gcb->sub_obj_num, sizeof(ACSI_OBJ_CTRL));
			for (j=0; j<5; j++)
			{
				gcb_attr = &obj_gcb->sub_objs[j];
				gcb_attr->sub_obj_num = 0;
				gcb_attr->sub_objs = NULL;
				gcb_attr->comn_obj.obj_type = OBJECT_TYPE_FCDA;
				gcb_attr->comn_obj.desc = NULL;
				gcb_attr->comn_obj.parent = &(obj_gcb->comn_obj);
			}
			strcpy(obj_gcb->sub_objs[0].comn_obj.name, "GoEna");
			obj_gcb->sub_objs[0].type = VAL_TYPE_BOOLEAN;
			strcpy(obj_gcb->sub_objs[1].comn_obj.name, "GoID");
			obj_gcb->sub_objs[1].type = VAL_TYPE_VSTRING65;
			obj_gcb->sub_objs[1].val = chk_calloc(1,MAX_IDENT_LEN +1);
			if (strlen(gcb->appID) > 0)
			{
				strcpy(obj_gcb->sub_objs[1].val, gcb->appID);//���⴦��
			}
			strcpy(obj_gcb->sub_objs[2].comn_obj.name, "DataSet");
			obj_gcb->sub_objs[2].type = VAL_TYPE_VSTRING65;
			obj_gcb->sub_objs[2].val = chk_calloc(1,MAX_IDENT_LEN +1);
			if (strlen(gcb->datSet) > 0)
			{
				strcpy(obj_gcb->sub_objs[2].val, gcb->datSet);//���⴦��
			}
			strcpy(obj_gcb->sub_objs[3].comn_obj.name, "ConfRev");
			obj_gcb->sub_objs[3].type = VAL_TYPE_INT32U;
			strcpy(obj_gcb->sub_objs[4].comn_obj.name, "NdsCom");
			obj_gcb->sub_objs[4].type = VAL_TYPE_BOOLEAN;
			strcpy(obj_gcb->sub_objs[5].comn_obj.name, "DstAddress");
			obj_gcb->sub_objs[5].type = VAL_TYPE_STRUCT;

			gcb_attr = &obj_gcb->sub_objs[5];
			gcb_attr->sub_obj_num = 4;
			gcb_attr->comn_obj.obj_type = OBJECT_TYPE_FCD;
			gcb_attr->comn_obj.desc = NULL;
			gcb_attr->comn_obj.parent = &(obj_gcb->comn_obj);
			gcb_attr->sub_objs = chk_calloc(gcb_attr->sub_obj_num, sizeof(ACSI_OBJ_CTRL));
			for (j=0; j<4; j++)
			{
				gcb_attr->sub_objs[j].sub_obj_num = 0;
				gcb_attr->sub_objs[j].sub_objs = NULL;
				gcb_attr->sub_objs[j].comn_obj.desc = NULL;
				gcb_attr->sub_objs[j].comn_obj.obj_type = OBJECT_TYPE_FCDA;
				gcb_attr->sub_objs[j].comn_obj.parent = &gcb_attr->comn_obj;
			}
			strcpy(gcb_attr->sub_objs[0].comn_obj.name, "Addr");
			gcb_attr->sub_objs[0].type = VAL_TYPE_OSTRING6;
			strcpy(gcb_attr->sub_objs[1].comn_obj.name, "PRIORITY");
			gcb_attr->sub_objs[1].type = VAL_TYPE_INT8U;
			strcpy(gcb_attr->sub_objs[2].comn_obj.name, "VID");
			gcb_attr->sub_objs[2].type = VAL_TYPE_INT16U;
			strcpy(gcb_attr->sub_objs[3].comn_obj.name, "APPID");
			gcb_attr->sub_objs[3].type = VAL_TYPE_INT16U;

			i++;
			gcb = (SCL_GCB *)list_find_prev((DBL_LNK *)scl_ln->gcbHead, (DBL_LNK *)gcb);
		}	 
		break;
	default:
		return;
	}
}
/**
* �������ƿ�
* cb_ctrl   cb����ռ�
* scl_ln    �����ļ�����ln
* parent    �������common_objָ��
* type      ���ƿ�����
* ����ֵ    ��
*/
static ST_VOID _create_cb_type(ACSI_CB_CTRL* cb_ctrl, SCL_LN* scl_ln, ACSI_COMMON_OBJ* parent, ACSI_CB_TYPE type)
{
	switch(type)
	{
	case CB_TYPE_BR:
			_create_rcb(cb_ctrl, scl_ln, parent, SD_TRUE);
			 break;
	case CB_TYPE_RP:
			_create_rcb(cb_ctrl, scl_ln, parent, SD_FALSE);
			 break;
	default:
		_create_other_cb(cb_ctrl, scl_ln, parent, type);//����cb���øú���
		break;
	}
}

/**
* �������п��ƿ�FC
* ln_ctrl  ln����ռ�
* scl_ln   �����ļ�����ln
* data_type ������������
* parent    �������common_objָ��
* ����ֵ    ��
*/
static ST_VOID _create_cb_fc(ACSI_LN_CTRL* ln_ctrl, SCL_LN* scl_ln, SCL_INFO *data_type, ACSI_COMMON_OBJ* parent)
{
	ST_INT i=0;
	ACSI_CB_CTRL * cb_ctrl = NULL;
	ST_BOOLEAN br = SD_FALSE;
	ST_BOOLEAN rp = SD_FALSE;
	SCL_RCB    *rcb = NULL;


	ln_ctrl->cb_num = _get_cb_type_num(scl_ln);//��ȡln�¿��ƿ����͸���,BR,RPΪ��ͬ����
	if (ln_ctrl->cb_num > 0)
	{
		ln_ctrl->cbs = chk_calloc(ln_ctrl->cb_num, sizeof(ACSI_CB_CTRL));
		if (scl_ln->sgcbPtr)
		{
			cb_ctrl = &ln_ctrl->cbs[i];
			//����SGCB
			_create_cb_type(cb_ctrl, scl_ln, &ln_ctrl->comn_obj, CB_TYPE_SG);
			i++;
		}
		if (scl_ln->rcbHead)
		{
			rcb = (SCL_RCB *)list_find_last((DBL_LNK *)scl_ln->rcbHead);
			while(rcb)
			{
				if (rcb->buffered)
				{
					br = SD_TRUE;
				}
				else
					rp = SD_TRUE;
				rcb = (SCL_RCB *)list_find_prev((DBL_LNK *)scl_ln->rcbHead, (DBL_LNK *)rcb);
			}
			if (br)
			{
				cb_ctrl = &ln_ctrl->cbs[i];
				//�������汨����ƿ�BR
				_create_cb_type(cb_ctrl, scl_ln, &ln_ctrl->comn_obj, CB_TYPE_BR);
				i++;
			}
			if (rp)
			{
				cb_ctrl = &ln_ctrl->cbs[i];
				//�����ǻ��汨����ƿ�RP
				_create_cb_type(cb_ctrl, scl_ln, &ln_ctrl->comn_obj, CB_TYPE_RP);
				i++;
			}
		}
		if (scl_ln->lcbHead)
		{
			//������־���ƿ�
			cb_ctrl = &ln_ctrl->cbs[i];
			_create_cb_type(cb_ctrl, scl_ln, &ln_ctrl->comn_obj, CB_TYPE_LOG);
			i++;
		}
		if (scl_ln->gcbHead)
		{
			//����goose���ƿ�
			cb_ctrl = &ln_ctrl->cbs[i];
			_create_cb_type(cb_ctrl, scl_ln, &ln_ctrl->comn_obj, CB_TYPE_GO);
			i++;
		}
	}
}
/**
* ����һ��ln�Ķ���ռ�
* ln_ctrl  ln����ռ�
* scl_ln   �����ļ�����ln
* data_type ������������
* parent    �������common_objָ��
* ����ֵ    ��
*/
static ST_VOID _create_ln(ACSI_LN_CTRL* ln_ctrl, SCL_LN* scl_ln, SCL_INFO *data_type, ACSI_COMMON_OBJ* parent)
{
	ST_INT i=0;
	SCL_LNTYPE * lnType = NULL; 

	ln_ctrl->comn_obj.obj_type = OBJECT_TYPE_LN;
	ln_ctrl->comn_obj.parent = parent;
	strcpy(ln_ctrl->comn_obj.name, scl_ln->varName);
	if (strlen(scl_ln->desc) == 0)
	{
		ln_ctrl->comn_obj.desc = NULL;
	}
	else
	{
		ln_ctrl->comn_obj.desc = chk_calloc(1, strlen(scl_ln->desc)+1);
		strcpy(ln_ctrl->comn_obj.desc, scl_ln->desc);
	}

	lnType = (SCL_LNTYPE *)list_find_last( (DBL_LNK *)data_type->lnTypeHead);
	while ( lnType )
	{
		if ( strcmp( lnType->id, scl_ln->lnType ) == 0 )
		{
			break;
		}
		lnType = (SCL_LNTYPE *)list_find_prev( (DBL_LNK *)data_type->lnTypeHead, (DBL_LNK *)lnType );
	}

	ln_ctrl->sub_obj_num = _get_fc_num_ln(lnType, data_type);//ln��fc���������������ƿ��fc
	if (ln_ctrl->sub_obj_num > 0)
	{
		ln_ctrl->sub_objs = chk_calloc(ln_ctrl->sub_obj_num, sizeof(ACSI_OBJ_CTRL));
		for (i=0; i<ln_ctrl->sub_obj_num; i++)
		{
			strcpy(ln_ctrl->sub_objs[i].comn_obj.name, fc_ctrls[i].fc_str);
			_create_fc(&ln_ctrl->sub_objs[i],lnType, data_type, &ln_ctrl->comn_obj);//����fc��fc�³�Ա
		}
	}
	memset(fc_ctrls, 0, sizeof(ACSI_FC_CTRL) * MAX_FC_NUM);
	if (strcmp(lnType->lnClass, "LLN0") == 0)
	{
		_create_cb_fc(ln_ctrl, scl_ln, data_type, &ln_ctrl->comn_obj);//�������ƿ�FC
	}
}
/**
* ����һ��ld�Ķ���ռ�
* ld_ctrl  ld����ռ�
* scl_ld   �����ļ�����ld
* data_type ������������ָ��
* parent    �������common_objָ��
* ����ֵ    ��
*/
static ST_VOID _create_ld(ACSI_LD_CTRL* ld_ctrl, SCL_LD* scl_ld, SCL_INFO *data_type, ACSI_COMMON_OBJ* parent)
{
	ST_INT ln_num = 0;
	SCL_LN *scl_ln = NULL;
	ACSI_LN_CTRL *ln_ctrl = NULL;
	ST_INT i;

	ld_ctrl->comn_obj.obj_type = OBJECT_TYPE_LD;
	ld_ctrl->comn_obj.parent = parent;
	strcpy(ld_ctrl->comn_obj.name, scl_ld->domName);
	if (strlen(scl_ld->desc) == 0)
	{
		ld_ctrl->comn_obj.desc = NULL;
	}
	else
	{
		ld_ctrl->comn_obj.desc = chk_calloc(1, strlen(scl_ld->desc)+1);
		strcpy(ld_ctrl->comn_obj.desc, scl_ld->desc);
	}
	
	scl_ln = (SCL_LN *)list_find_last((DBL_LNK *)scl_ld->lnHead);
	while(scl_ln)//����ln����
	{
		ln_num ++;
		scl_ln = (SCL_LN *)list_find_prev((DBL_LNK *)scl_ld->lnHead, (DBL_LNK *)scl_ln);
	}
	ld_ctrl->ln_num = ln_num;
	if (ln_num>0)
	{
		ld_ctrl->lns = chk_calloc(ln_num, sizeof(ACSI_LN_CTRL));

		scl_ln = (SCL_LN *)list_find_last((DBL_LNK *)scl_ld->lnHead);
		for (i=0; i<ld_ctrl->ln_num; i++)
		{
			ln_ctrl = &ld_ctrl->lns[i];
			_create_ln(ln_ctrl, scl_ln, data_type, &ld_ctrl->comn_obj);//����ln
			scl_ln = (SCL_LN *)list_find_prev((DBL_LNK *)scl_ld->lnHead, (DBL_LNK *)scl_ln);
		}
	}
	
}

/**
* ����һ��ied�Ķ���ռ�
* ied_ctrl  ied����ռ�
* scl_ied   �����ļ�����ied
* data_type ��������
* ����ֵ    ��
*/
static ST_VOID _create_ied(ACSI_IED_CTRL* ied_ctrl, SCL_INFO* scl_ied, SCL_INFO *data_type)
{
	ST_INT ld_num = 0;
	SCL_LD *scl_ld = NULL;
	ACSI_LD_CTRL *ld_ctrl = NULL;
	ST_INT i;

	ied_ctrl->comn_obj.obj_type = OBJECT_TYPE_IED;
	ied_ctrl->comn_obj.parent = NULL;
	ied_ctrl->comn_obj.desc = NULL;
	strcpy(ied_ctrl->comn_obj.name, scl_ied->iedName);

	scl_ld = (SCL_LD *)list_find_last((DBL_LNK *)scl_ied->ldHead);
	while(scl_ld)//����ld����
	{
		ld_num ++;
		scl_ld = (SCL_LD *)list_find_prev((DBL_LNK *)scl_ied->ldHead, (DBL_LNK *)scl_ld);
	}
	ied_ctrl->ld_num = ld_num;//ld����

	if (ld_num > 0)
	{
		ied_ctrl->lds = chk_calloc(ld_num, sizeof(ACSI_LD_CTRL));

		scl_ld = (SCL_LD *)list_find_last((DBL_LNK *)scl_ied->ldHead);
		for (i=0; i<ied_ctrl->ld_num; i++)
		{
			ld_ctrl = &ied_ctrl->lds[i];

			_create_ld(ld_ctrl, scl_ld, data_type, &ied_ctrl->comn_obj);//����ld

			scl_ld = (SCL_LD *)list_find_prev((DBL_LNK *)scl_ied->ldHead, (DBL_LNK *)scl_ld);
		}
	}
}

//����ied�µ�net��Ϣ
static ST_VOID _set_netInfo_ied(ACSI_IED_CTRL* ied_ctrl, SCL_SUBNET *subnet_head_list)
{
	SCL_SUBNET *subnet = NULL;
	SCL_NETAP *netap = NULL;
	SCL_ADDR *Address = NULL;
	SCL_P * ip = NULL;

	subnet = (SCL_SUBNET *)list_find_last((DBL_LNK *)subnet_head_list);
	while(subnet)
	{
		netap = (SCL_NETAP *)list_find_last((DBL_LNK *)subnet->netap_head);
		while(netap)
		{
			if (strcmp(netap->ied_name, ied_ctrl->comn_obj.name) == 0)
				break;
			netap = (SCL_NETAP *)list_find_prev((DBL_LNK *)subnet->netap_head, (DBL_LNK *)netap);
		}
		if (netap)
		{
			if (netap->pAddress)
			{
				ip = (SCL_P *)list_find_last((DBL_LNK *)netap->pAddress->P_head);
				while(ip)
				{
					if (strcmp(ip->type, "IP") == 0)
						break;
					ip =(SCL_P *)list_find_prev((DBL_LNK *)netap->pAddress->P_head, (DBL_LNK *)ip);
				}
				if (ip)
				{
					if (strlen(ied_ctrl->ip.ip_b )== 0)
					{
						strcpy(ied_ctrl->ip.ip_b, ip->p);
					}
					else
					{
						strcpy(ied_ctrl->ip.ip_a, ip->p);
					}
				}
			}
		}
		subnet = (SCL_SUBNET *)list_find_prev((DBL_LNK *)subnet_head_list, (DBL_LNK *)subnet);
	}
}

//��ȡln��ds����
static _get_ds_num(SCL_LN *scl_ln)
{
	ST_INT ds_num = 0;
	SCL_DATASET *data_set = NULL;

	data_set = (SCL_DATASET *)list_find_last((DBL_LNK *)scl_ln->datasetHead);
	while(data_set)
	{
		ds_num ++;
		data_set = (SCL_DATASET *)list_find_prev((DBL_LNK *)scl_ln->datasetHead, (DBL_LNK *)data_set);
	}
	return ds_num;
}

//ds��Ա����
static _get_ds_member_num(SCL_FCDA *fcdaHead)
{
	ST_INT ds_member_num = 0;
	SCL_FCDA *ds_member = NULL;

	ds_member = (SCL_FCDA *)list_find_last((DBL_LNK *)fcdaHead);
	while(ds_member)
	{
		ds_member_num ++;
		ds_member = (SCL_FCDA *)list_find_prev((DBL_LNK *)fcdaHead, (DBL_LNK *)ds_member);
	}
	return ds_member_num;
}

//�ݹ���ò���do��Ӧ��objָ��
static ACSI_OBJ_CTRL* _find_fcda_ctrl(ST_CHAR *fcda_name,  ACSI_OBJ_CTRL *do_ctrl)
{
	ST_INT i=0;
	ST_CHAR *fcda_ptr = NULL;
	ST_CHAR sdo_name[MAX_IDENT_LEN +1] = {0};
	ACSI_OBJ_CTRL *sdo_ctrl = NULL;

	fcda_ptr = strstr(fcda_name,"$");
	if (fcda_ptr)
	{
		strncpy(sdo_name, fcda_name, fcda_ptr - fcda_name);
	}
	else
	{
		strcpy(sdo_name, fcda_name);
	}

	for (i=0; i<do_ctrl->sub_obj_num; i++)
	{
		sdo_ctrl = &do_ctrl->sub_objs[i];
		if (strcmp(sdo_ctrl->comn_obj.name, sdo_name) == 0)
		{
			break;
		}
	}
	if (i==do_ctrl->sub_obj_num)
	{
		return NULL;
	}
	if (fcda_ptr)
	{
		return _find_fcda_ctrl(fcda_ptr+1, sdo_ctrl);
	}
	else
	{
		return sdo_ctrl;
	}
}

/**
* �ڶ���ռ��в���dsmember��Ӧ��objָ��
* ds_member     ���ݼ���Ա
* ied_ctrl      ied����
* ����ֵ        ����ָ��
*/
static ACSI_OBJ_CTRL* _find_obj_ctrl(SCL_FCDA *ds_member,  ACSI_IED_CTRL* ied_ctrl)
{
	ST_INT i=0;
	ST_CHAR ln_name[MAX_IDENT_LEN +1] = {0};
	ACSI_LD_CTRL *ld_ctrl = NULL;
	ACSI_LN_CTRL *ln_ctrl = NULL;
	ACSI_OBJ_CTRL *fc_ctrl = NULL;
	ACSI_OBJ_CTRL *do_ctrl = NULL;

	strcpy(ln_name,ds_member->prefix);
	strcat(ln_name, ds_member->lnClass);
	strcat(ln_name, ds_member->lnInst);
	for (i=0; i<ied_ctrl->ld_num; i++)
	{
		ld_ctrl = &ied_ctrl->lds[i];
		if (strcmp(ld_ctrl->comn_obj.name, ds_member->domName) == 0)
		{
			break;
		}
	}
	if (i==ied_ctrl->ld_num)
	{
		return NULL;
	}
	for (i=0; i<ld_ctrl->ln_num; i++)
	{
		ln_ctrl = &ld_ctrl->lns[i];
		if (strcmp(ln_ctrl->comn_obj.name, ln_name) == 0)
		{
			break;
		}
	}
	if (i==ld_ctrl->ln_num)
	{
		return NULL;
	}
	for (i=0; i<ln_ctrl->sub_obj_num; i++)
	{
		fc_ctrl = &ln_ctrl->sub_objs[i];
		if (strcmp(fc_ctrl->comn_obj.name, ds_member->fc) == 0)
		{
			break;
		}
	}
	if (i==ln_ctrl->sub_obj_num)
	{
		return NULL;
	}
	for (i=0; i<fc_ctrl->sub_obj_num; i++)
	{
		do_ctrl = &fc_ctrl->sub_objs[i];
		if (strcmp(do_ctrl->comn_obj.name, ds_member->doName) == 0)
		{
			break;
		}
	}
	if (i==fc_ctrl->sub_obj_num)
	{
		return NULL;
	}
	if (strlen(ds_member->daName) == 0)
	{
		return do_ctrl;
	}
	else
	{
		return _find_fcda_ctrl(ds_member->daName, do_ctrl);
	}
}

/**
* �������ݼ�
* ds_ctrl     ���ݼ�����
* ied_ctrl    ied����
* scl_ds      �����ļ�����һ��ds��Ϣ
* parent      lnָ��
* ����ֵ    ��
*/
static ST_VOID _create_ds(ACSI_DS_CTRL *ds_ctrl, ACSI_IED_CTRL* ied_ctrl, SCL_DATASET * scl_ds, ACSI_COMMON_OBJ* parent)
{
	ST_INT i=0;
	SCL_FCDA *ds_member = NULL;

	ds_ctrl->comn_obj.obj_type = OBJECT_TYPE_DS;
	ds_ctrl->comn_obj.parent = parent;
	strcpy(ds_ctrl->comn_obj.name, scl_ds->name);
	if (strlen(scl_ds->desc) > 0)
	{
		ds_ctrl->comn_obj.desc = chk_calloc(1, strlen(scl_ds->desc) +1);
		strcpy(ds_ctrl->comn_obj.desc, scl_ds->desc);
	}
	ds_ctrl->member_num = _get_ds_member_num(scl_ds->fcdaHead);//ds��Ա����
	if (ds_ctrl->member_num > 0)
	{
		ds_ctrl->ds_members = chk_calloc(ds_ctrl->member_num, sizeof(ACSI_OBJ_CTRL*));
		ds_member = (SCL_FCDA *)list_find_last((DBL_LNK *)scl_ds->fcdaHead);
		//�ڶ���ռ��в���dsmember��Ӧ��objָ�븳��ds_membersָ������
		while(ds_member)
		{
			ds_ctrl->ds_members[i] = _find_obj_ctrl(ds_member, ied_ctrl);
			ds_member = (SCL_FCDA *)list_find_prev((DBL_LNK *)scl_ds->fcdaHead, (DBL_LNK *)ds_member);
			i++;
		}
	}
	//if (strlen(scl_ds->desc) > 0)
	//{
	//	ds_ctrl->comn_obj.desc = chk_calloc(1, strlen(scl_ds->desc) +1);
	//	strcpy(ds_ctrl->comn_obj.desc, scl_ds->desc);
	//	ds_ctrl->member_num = _get_ds_member_num(scl_ds->fcdaHead);//ds��Ա����
	//	if (ds_ctrl->member_num > 0)
	//	{
	//		ds_ctrl->ds_members = chk_calloc(ds_ctrl->member_num, sizeof(ACSI_OBJ_CTRL*));
	//		ds_member = (SCL_FCDA *)list_find_last((DBL_LNK *)scl_ds->fcdaHead);
	//		//�ڶ���ռ��в���dsmember��Ӧ��objָ�븳��ds_membersָ������
	//		while(ds_member)
	//		{
	//			ds_ctrl->ds_members[i] = _find_obj_ctrl(ds_member, ied_ctrl);
	//			ds_member = (SCL_FCDA *)list_find_prev((DBL_LNK *)scl_ds->fcdaHead, (DBL_LNK *)ds_member);
	//			i++;
	//		}
	//	}
	//}
}

/**
* ����ied���������ݼ�
* ied_ctrl    ied����
* scl_ied     �����ļ�����ied��Ϣ
* data_type   ������������
* ����ֵ    ��
*/
static ST_VOID _set_dataset_ied(ACSI_IED_CTRL* ied_ctrl, SCL_INFO* scl_ied, SCL_INFO *data_type)
{
	SCL_LD *scl_ld = NULL;
	SCL_LN *scl_ln = NULL;
	SCL_DATASET * scl_ds = NULL;
	ACSI_LD_CTRL* ld_ctrl = NULL;
	ACSI_LN_CTRL* ln_ctrl = NULL;
	ACSI_DS_CTRL* ds_ctrl = NULL;
	ST_INT i = 0;
	ST_INT j = 0;
	ST_INT k = 0;

	scl_ld = (SCL_LD *)list_find_last((DBL_LNK *)scl_ied->ldHead);
	while(scl_ld)
	{
		ld_ctrl = &ied_ctrl->lds[i];
		j=0;
		scl_ln = (SCL_LN *)list_find_last((DBL_LNK *)scl_ld->lnHead);
		while(scl_ln)
		{
			ln_ctrl = &ld_ctrl->lns[j];
			if (strcmp(ln_ctrl->comn_obj.name, "LLN0") == 0)
			{
				ln_ctrl->ds_num = _get_ds_num(scl_ln);//����lln0��ds����
				ln_ctrl->data_sets = chk_calloc(ln_ctrl->ds_num, sizeof(ACSI_DS_CTRL));
				scl_ds = (SCL_DATASET *)list_find_last((DBL_LNK *)scl_ln->datasetHead);
				k = 0;
				while(scl_ds)
				{
					ds_ctrl = &ln_ctrl->data_sets[k];
					_create_ds(ds_ctrl,ied_ctrl, scl_ds, &ln_ctrl->comn_obj);//�������ݼ�
					scl_ds = (SCL_DATASET *)list_find_prev((DBL_LNK *)scl_ln->datasetHead, (DBL_LNK *)scl_ds);
					k++;
				}
				break;
			}
			j++;
			scl_ln = (SCL_LN *)list_find_prev((DBL_LNK *)scl_ld->lnHead, (DBL_LNK *)scl_ln);
		}

		i++;
		scl_ld = (SCL_LD *)list_find_prev((DBL_LNK *)scl_ied->ldHead, (DBL_LNK *)scl_ld);
	}
}
/**
* �ͷ�obj���ݹ����
*/
static ST_VOID _clean_sub_objs(ACSI_OBJ_CTRL* sub_objs, ST_INT sub_obj_num)
{
	ST_INT i;
	ACSI_OBJ_CTRL* sub_obj = NULL;

	if (sub_obj_num > 0 && sub_objs)
	{
		for (i=0; i<sub_obj_num; i++)
		{
			sub_obj = &sub_objs[i];
			if (sub_obj->val)
			{
				//��Ը�������Ա�������⴦���ڴ�������ռ�ʱ�������ڴ棬�������ͷ�
				if (strcmp(sub_obj->comn_obj.name, "DataSet") == 0 
					|| strcmp(sub_obj->comn_obj.name, "GoID") == 0 
					|| strcmp(sub_obj->comn_obj.name, "RptID") == 0 )
				{
					chk_free(sub_obj->val);
				}
			}
			
			if (sub_obj->comn_obj.desc)
			{
				chk_free(sub_obj->comn_obj.desc);
			}
			_clean_sub_objs(sub_obj->sub_objs, sub_obj->sub_obj_num);
		}
		chk_free(sub_objs);
	}
}

/**
* �ͷſ��ƿ��ڴ�ռ�
*/
static ST_VOID _clean_cbs(ACSI_CB_CTRL* cbs, ST_INT cb_num)
{
	ST_INT i;
	ACSI_CB_CTRL* cb = NULL;

	if (cb_num > 0 && cbs)
	{
		for (i=0; i<cb_num; i++)
		{
			cb = &cbs[i];
			if (cb->comn_obj.desc)
			{
				chk_free(cb->comn_obj.desc);
			}
			_clean_sub_objs(cb->cb_ctrls, cb->cb_num);
		}
		chk_free(cbs);
	}
}
/**
* �ͷ����ݼ��ڴ�ռ�
*/
static ST_VOID _clean_data_sets(ACSI_DS_CTRL* data_sets, ST_INT ds_num)
{
	ST_INT i;
	ACSI_DS_CTRL* ds_ctrl = NULL;

	if (ds_num > 0 && data_sets)
	{
		for (i=0; i<ds_num; i++)
		{
			ds_ctrl = &data_sets[i];
			if (ds_ctrl->comn_obj.desc)
			{
				chk_free(ds_ctrl->comn_obj.desc);
			}
			if (ds_ctrl->ds_members)
			{
				chk_free(ds_ctrl->ds_members);
			}
		}
		chk_free(data_sets);
	}
}

/**
* �ͷŶ���ռ�
*/
ST_VOID acsi_ied_destroy_all()
{
	ST_INT i=0;
	ST_INT j=0;
	ST_INT k=0;
	ACSI_IED_CTRL *ied_ctrl = NULL;
	ACSI_LD_CTRL *ld_ctrl = NULL;
	ACSI_LN_CTRL *ln_ctrl = NULL;


	if (g_ied_num == 0 || g_ied_ctrls == NULL)
	{
		return;
	}
	for (i=0; i<g_ied_num; i++)
	{
		ied_ctrl = &g_ied_ctrls[i];
		if (ied_ctrl->comn_obj.desc)
		{
			chk_free(ied_ctrl->comn_obj.desc);
		}
		if (ied_ctrl->ld_num == 0 || ied_ctrl->lds == NULL)
		{
			continue;
		}
		for (j=0; j<ied_ctrl->ld_num; j++)
		{
			ld_ctrl = &ied_ctrl->lds[j];
			if (ld_ctrl->comn_obj.desc)
			{
				chk_free(ld_ctrl->comn_obj.desc);
			}
			if (ld_ctrl->ln_num == 0 || ld_ctrl->lns == NULL)
			{
				continue;
			}
			for (k=0; k<ld_ctrl->ln_num; k++)
			{
				ln_ctrl = &ld_ctrl->lns[k];
				if (ln_ctrl->comn_obj.desc)
				{
					chk_free(ln_ctrl->comn_obj.desc);
				}
				_clean_sub_objs(ln_ctrl->sub_objs, ln_ctrl->sub_obj_num);
				_clean_cbs(ln_ctrl->cbs, ln_ctrl->cb_num);
				_clean_data_sets(ln_ctrl->data_sets, ln_ctrl->ds_num);
			}
			chk_free(ld_ctrl->lns);
		}
		chk_free(ied_ctrl->lds);
	}
	chk_free(g_ied_ctrls);
}

/**
* ת�������������ַ���ΪAI_VAL_TYPE
*/
static AI_VAL_TYPE _get_val_type(ST_CHAR *type_str)
{
	if (strcmp(type_str, "BOOLEAN") == 0)
	{
		return VAL_TYPE_BOOLEAN;
	}
	if (strcmp(type_str, "INT8") == 0)
	{
		return VAL_TYPE_INT8;
	}
	if (strcmp(type_str, "INT16") == 0)
	{
		return VAL_TYPE_INT16;
	}
	if (strcmp(type_str, "INT32") == 0)
	{
		return VAL_TYPE_INT32;
	}
	if (strcmp(type_str, "INT128") == 0)
	{
		return VAL_TYPE_INT128;
	}
	if (strcmp(type_str, "INT8U") == 0)
	{
		return VAL_TYPE_INT8U;
	}
	if (strcmp(type_str, "INT16U") == 0)
	{
		return VAL_TYPE_INT16U;
	}
	if (strcmp(type_str, "INT32U") == 0)
	{
		return VAL_TYPE_INT32U;
	}
	if (strcmp(type_str, "FLOAT32") == 0)
	{
		return VAL_TYPE_FLOAT32;
	}
	if (strcmp(type_str, "FLOAT64") == 0)
	{
		return VAL_TYPE_FLOAT64;
	}
	if (strcmp(type_str, "Quality") == 0)
	{
		return VAL_TYPE_QUALITY;
	}
	if (strcmp(type_str, "Timestamp") == 0)
	{
		return VAL_TYPE_TIMESTAMP;
	}
	if (strcmp(type_str, "VisString64") == 0)
	{
		return VAL_TYPE_VSTRING64;
	}
	if (strcmp(type_str, "VisString65") == 0)
	{
		return VAL_TYPE_VSTRING65;
	}
	if (strcmp(type_str, "VisString129") == 0)
	{
		return VAL_TYPE_VSTRING129;
	}
	if (strcmp(type_str, "VisString255") == 0)
	{
		return VAL_TYPE_VSTRING255;
	}
	if (strcmp(type_str, "Unicode255") == 0)
	{
		return VAL_TYPE_UNISTRING255;
	}
	if (strcmp(type_str, "Octet64") == 0)
	{
		return VAL_TYPE_OSTRING64;
	}
	if (strcmp(type_str, "Octet8") == 0)
	{
		return VAL_TYPE_OSTRING8;
	}
	if (strcmp(type_str, "Check") == 0)
	{
		return VAL_TYPE_CHECK;
	}
	if (strcmp(type_str, "Struct") == 0)
	{
		return VAL_TYPE_STRUCT;
	}
	if (strcmp(type_str, "Dbpos") == 0)
	{
		return VAL_TYPE_CODEDENUM;
	}
	if (strcmp(type_str, "Enum") == 0)
	{
		return VAL_TYPE_ENUM;
	}
	return VAL_TYPE_UNKNOWN;
}