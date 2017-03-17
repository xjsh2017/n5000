/****************************************************************************/
/* Copyright (c) 2010,许继集团有限公司                                      */
/* All rights reserved.                                                     */
/*                                                                          */
/*  模块描述：                                                              */
/** 对象空间操作接口
*  @file  acsi_crscl.c                                                      
/*                                                                          */
/* 日期         作者    注释                                                */
/* 2010/07/17   ZYZ     创建文件                                            */
/****************************************************************************/
#include "acsi_dir.h"

#ifdef DEBUG_SISCO
SD_CONST static ST_CHAR *SD_CONST thisFileName = __FILE__;
#endif

#define MAX_DESC_LEN 129
static FILE * ftp = NULL;
extern ST_INT        g_ied_num;   //装置个数
extern ACSI_IED_CTRL *g_ied_ctrls;//所有装置ied

/**
* 查找ied
* ip               ipstr            
* 返回值           ied对象指针
*/
ACSI_IED_CTRL * acsi_get_ied_ip(ST_CHAR *ip)
{
	ACSI_IED_CTRL *ied = NULL;
	ST_INT i=0;

	for (i=0; i<g_ied_num; i++)
	{
		ied = &g_ied_ctrls[i];
		if (strcmp(ied->ip.ip_a, ip) == 0 || strcmp(ied->ip.ip_b, ip) == 0)
		{
			return ied;
		}
	}
	return NULL;
}

/**
* 查找报告控制块对象指针
* ied            
* rptid            rptid
* 返回值           rcb对象指针
*/
ACSI_OBJ_CTRL * acsi_get_rcb_rptid(ACSI_IED_CTRL *ied, ST_CHAR *rptid)
{
	ACSI_LD_CTRL * ld_ctrl = NULL;
	ST_INT i=0;
	ST_INT j=0;
	ST_INT k=0;
	ACSI_LN_CTRL * ln_ctrl = NULL;
	ACSI_CB_CTRL * cb_ctrl = NULL;
	ACSI_OBJ_CTRL * rcb_ctrl = NULL;

	for (i=0; i<ied->ld_num; i++)
	{
		ld_ctrl = &ied->lds[i];
		for (j=0; j<ld_ctrl->ln_num; j++)
		{
			ln_ctrl = &ld_ctrl->lns[j];
			if (strcmp(ln_ctrl->comn_obj.name, "LLN0") == 0)
			{
				break;
			}
		}
		for (j=0; j<ln_ctrl->cb_num; j++)
		{
			cb_ctrl = &ln_ctrl->cbs[j];
			if (cb_ctrl->cb_type == CB_TYPE_BR || cb_ctrl->cb_type == CB_TYPE_RP)
			{
				for (k=0; k<cb_ctrl->cb_num; k++)
				{
					rcb_ctrl = &cb_ctrl->cb_ctrls[k];
					if (strcmp(rcb_ctrl->sub_objs[0].val, rptid) == 0)
					{
						return rcb_ctrl;
					}
				}
			}
		}
	}
	return NULL;
}

/**
* 查找ied下某个ds
* ied            
* dsname           ds名称
* 返回值           ds对象指针
*/
ACSI_DS_CTRL * acsi_get_ds_dsname(ACSI_IED_CTRL *ied, ST_CHAR *ldname, ST_CHAR *dsname)
{
	ACSI_LD_CTRL * ld_ctrl = NULL;
	ST_INT i=0;
	ST_INT j=0;
	ACSI_LN_CTRL * ln_ctrl = NULL;
	ACSI_DS_CTRL * ds_ctrl = NULL;

	for (i=0; i<ied->ld_num; i++)
	{
		ld_ctrl = &ied->lds[i];
		if (0 != strcmp(ld_ctrl->comn_obj.name, ldname))
		{
			continue;
		}
		for (j=0; j<ld_ctrl->ln_num; j++)
		{
			ln_ctrl = &ld_ctrl->lns[j];
			if (strcmp(ln_ctrl->comn_obj.name, "LLN0") == 0)
			{
				break;
			}
		}
		for (j=0; j<ln_ctrl->ds_num; j++)
		{
			ds_ctrl = &ln_ctrl->data_sets[j];
			if (strcmp(ds_ctrl->comn_obj.name, dsname) == 0)
			{
				return ds_ctrl;
			}
		}
	}
	return NULL;
}

/**
* 查找ied下的ld
* ied            
* ld_name          ld名称
* 返回值           对象指针
*/
ACSI_LD_CTRL * _get_ld_name(ACSI_IED_CTRL *ied, ST_CHAR *ld_name)
{
	ST_INT i=0;
	ACSI_LD_CTRL *ld = NULL;

	for (i=0; i<ied->ld_num; i++)
	{
		ld = &ied->lds[i];
		if (strcmp(ld_name, ld->comn_obj.name) == 0)
		{
			return ld;
		}
	}
	return NULL;
}

/**
* 查找ld下的ln
* ld            
* ln_name          ln名称
* 返回值           对象指针
*/
ACSI_LN_CTRL * _get_ln_name(ACSI_LD_CTRL *ld, ST_CHAR *ln_name)
{
	ST_INT i=0;
	ACSI_LN_CTRL *ln = NULL;

	for (i=0; i<ld->ln_num; i++)
	{
		ln = &ld->lns[i];
		if (strcmp(ln_name, ln->comn_obj.name) == 0)
		{
			return ln;
		}
	}
	return NULL;
}

/**
* 查找ln下的fc
* ln            
* name          fc名称
* 返回值        对象指针
*/
ACSI_OBJ_CTRL * _get_sub_name_ln(ACSI_LN_CTRL *ln, ST_CHAR *name)
{
	ST_INT i=0;
	ACSI_OBJ_CTRL *obj = NULL;

	for (i=0; i<ln->sub_obj_num; i++)
	{
		obj = &ln->sub_objs[i];
		if (strcmp(name, obj->comn_obj.name) == 0)
		{
			return obj;
		}
	}
	return NULL;
}

/**
* 查找ln下的cb
* ln            
* name          cb名称
* 返回值        cb对象指针
*/
ACSI_CB_CTRL * _get_cb_name(ACSI_LN_CTRL *ln, ST_CHAR *name)
{
	ST_INT i=0;
	ACSI_CB_CTRL *cb = NULL;

	for (i=0; i<ln->cb_num; i++)
	{
		cb = &ln->cbs[i];
		if (strcmp(name, cb->comn_obj.name) == 0)
		{
			return cb;
		}
	}
	return NULL;
}

/**
* 查找ln下的ds
* ln            
* name          ds名称
* 返回值        ds对象指针
*/
ACSI_DS_CTRL * _get_ds_name(ACSI_LN_CTRL *ln, ST_CHAR *name)
{
	ST_INT i=0;
	ACSI_DS_CTRL *ds = NULL;

	for (i=0; i<ln->ds_num; i++)
	{
		ds = &ln->data_sets[i];
		if (strcmp(name, ds->comn_obj.name) == 0)
		{
			return ds;
		}
	}
	return NULL;
}

/**
* 查找objname对应的obj
* parent        父obj
* name          对象名称
* 返回值        对象指针
*/
ACSI_COMMON_OBJ * _find_obj_name(ACSI_COMMON_OBJ *parent, ST_CHAR *name)
{
	ST_INT i=0;
	ACSI_OBJ_CTRL *obj_ctrl = NULL;
	ACSI_OBJ_CTRL *sub_obj_ctrl = NULL;

	obj_ctrl = (ACSI_OBJ_CTRL *)parent;
	for (i=0; i<obj_ctrl->sub_obj_num; i++)
	{
		sub_obj_ctrl = &obj_ctrl->sub_objs[i];
		if (strcmp(sub_obj_ctrl->comn_obj.name, name) == 0)
		{
			return (ACSI_COMMON_OBJ *)sub_obj_ctrl;
		}
	}
	return NULL;
}

/**
* 递归查找obj
* obj           
* ref           对象参引
* 返回值        对象指针
*/
ACSI_COMMON_OBJ * _get_obj_ref(ACSI_COMMON_OBJ *obj, ST_CHAR *ref)
{	
	ST_CHAR  obj_name[MAX_NAME_LEN +1] = {0};
	ST_CHAR  obj_ref[MAX_NAME_LEN +1] = {0};
	ST_CHAR  *ptr = NULL;
	ACSI_COMMON_OBJ * sub_obj = NULL;

	ptr = strstr(ref, "$");
	if (ptr == NULL)
	{
		strcpy(obj_name, ref);
	}
	else
		strncpy(obj_name, ref, ptr - ref);


	if (ptr == NULL)
	{
		return _find_obj_name(obj, obj_name);//查找objname对应的obj
	}	
	else
	{
		sub_obj = _find_obj_name(obj, obj_name);
		if (sub_obj == NULL)
		{
			return NULL;
		}
		ptr ++;
		strcpy(obj_ref, ptr);
		return _get_obj_ref(sub_obj, obj_ref);//递归调用
	}
}

/**
* 获取ld及ld以下成员对象空间中指针
* ied           ied对象
* ref           对象参引
* 返回值        对象指针
*/
ACSI_COMMON_OBJ * acsi_get_obj_ref(ACSI_IED_CTRL *ied, ST_CHAR *ref)
{
	ST_INT i=0;
	ST_INT j=0;
	ST_CHAR  ld_name[MAX_NAME_LEN +1] = {0};
	ST_CHAR  ln_name[MAX_NAME_LEN +1] = {0};
	ST_CHAR  obj_name[MAX_NAME_LEN +1] = {0};
	ST_CHAR  leaf[MAX_NAME_LEN +1] = {0};
	ST_CHAR *ptr = NULL;
	ST_CHAR *ptr_ln = NULL;
	ST_CHAR *ptr_obj = NULL;
	ACSI_LD_CTRL *ld = NULL;
	ACSI_LN_CTRL *ln = NULL;
	ACSI_OBJ_CTRL *obj = NULL;
	ACSI_CB_CTRL *cb = NULL;
	ACSI_DS_CTRL *ds = NULL;
	ST_BOOLEAN   end = SD_FALSE;
	ST_INT str_len = 0;

	if (ied == NULL || ref == NULL)
	{
		return NULL;
	}
	str_len = strlen(ref);
	if (str_len == 0)
	{
		return NULL;
	}
	if (ref[str_len-1] == '$' || ref[str_len-1] == '/')
	{
		return NULL;
	}

	ptr = strstr(ref, "/");
	if (ptr)
	{
		strncpy(ld_name, ref, ptr - ref);
	}
	else
	{
		if (strlen(ref))//只有ld
		{ 
			return (ACSI_COMMON_OBJ *)_get_ld_name(ied, ref);
		}
		else
			return NULL;
	}
	ld = _get_ld_name(ied, ld_name);
	if (ld == NULL)
	{
		return NULL;
	}

	ptr ++ ;
	ptr_ln = strstr(ptr, "$");
	if (ptr_ln)
	{
		strncpy(ln_name, ptr, ptr_ln - ptr);
	}
	else
	{
		if (strlen(ptr))//ld/ln
		{
			return (ACSI_COMMON_OBJ *)_get_ln_name(ld, ptr);
		}
		else
			return NULL;
	}
	ln = _get_ln_name(ld, ln_name);
	if (ln == NULL)
	{
		return NULL;
	}

	ptr_ln ++;
	ptr_obj = strstr(ptr_ln, "$");
	if (ptr_obj)
	{
		strncpy(obj_name, ptr_ln, ptr_obj - ptr_ln);
		ptr_obj ++;
		strcpy(leaf, ptr_obj);
	}
	else
	{
		strcpy(obj_name, ptr_ln);
	}
	
	obj = _get_sub_name_ln(ln, obj_name);//在ln下的subobj中查找
	if (obj)//找到ln下的FC，在FC中查找
	{
		if (strlen(leaf) == 0)//最后一层
		{
			return (ACSI_COMMON_OBJ *)obj;
		}
		return _get_obj_ref((ACSI_COMMON_OBJ * )obj, leaf);
	}
	else//在ds中查找
	{
		ds = _get_ds_name(ln, obj_name);
		if(ds)
		{
			return (ACSI_COMMON_OBJ *)ds;
		}
		else//在cb中查找
		{
			cb = _get_cb_name(ln, obj_name);
			if(cb)
			{
				if (strlen(leaf) == 0)//最后一层
				{
					return (ACSI_COMMON_OBJ *)cb;
				}
				return  _get_obj_ref((ACSI_COMMON_OBJ * )cb, leaf);//递归查找obj
			}
			else
			{
				return NULL;
			}
		}
	}
	return NULL;
}

/**
* 输出obj对象
*/
static ST_VOID _output_sub_objs(ACSI_OBJ_CTRL* sub_objs, ST_INT sub_obj_num)
{
	ST_INT i;
	ACSI_OBJ_CTRL* sub_obj = NULL;
	ST_CHAR   str[MAX_DESC_LEN +1] = {0};

	if (sub_obj_num > 0 && sub_objs)
	{
		for (i=0; i<sub_obj_num; i++)
		{
			sub_obj = &sub_objs[i];
			strcpy(str, sub_obj->comn_obj.name);
			if (sub_obj->comn_obj.desc)
			{
				strcat(str, "\t");
				strcat(str, sub_obj->comn_obj.desc);
			}
			strcat(str, "\n");
			fwrite(str, 1, strlen(str), ftp);
			_output_sub_objs(sub_obj->sub_objs, sub_obj->sub_obj_num);
		}
	}
}

/**
* 输出控制块
*/
static ST_VOID _output_cbs(ACSI_CB_CTRL* cbs, ST_INT cb_num)
{
	ST_INT i;
	ACSI_CB_CTRL* cb = NULL;
	ST_CHAR   str[MAX_DESC_LEN +1] = {0};

	if (cb_num > 0 && cbs)
	{
		for (i=0; i<cb_num; i++)
		{
			cb = &cbs[i];
			strcpy(str, cb->comn_obj.name);
			if (cb->comn_obj.desc)
			{
				strcat(str, "\t");
				strcat(str, cb->comn_obj.desc);
			}
			strcat(str, "\n");
			fwrite(str, 1, strlen(str), ftp);
			_output_sub_objs(cb->cb_ctrls, cb->cb_num);
		}
	}
}

/**
* 输出对象空间每个成员，测试用
*/
ST_VOID acsi_ied_output_all()
{
	ST_INT i=0;
	ST_INT j=0;
	ST_INT k=0;
	ACSI_IED_CTRL *ied_ctrl = NULL;
	ACSI_LD_CTRL *ld_ctrl = NULL;
	ACSI_LN_CTRL *ln_ctrl = NULL;
	ST_CHAR   str[MAX_DESC_LEN +1] = {0};


	ftp = fopen("out.txt", "wt+");
	if (g_ied_num == 0 || g_ied_ctrls == NULL)
	{
		return;
	}
	for (i=0; i<g_ied_num; i++)
	{
		ied_ctrl = &g_ied_ctrls[i];
		strcpy(str, ied_ctrl->comn_obj.name);
		strcat(str, "\n");
		fwrite(str, 1, strlen(str), ftp);
		if (ied_ctrl->ld_num == 0 || ied_ctrl->lds == NULL)
		{
			continue;
		}
		for (j=0; j<ied_ctrl->ld_num; j++)
		{
			ld_ctrl = &ied_ctrl->lds[j];
			strcpy(str, ld_ctrl->comn_obj.name);
			if (ld_ctrl->comn_obj.desc)
			{
				strcat(str, "\t");
				strcat(str, ld_ctrl->comn_obj.desc);
			}
			strcat(str, "\n");
			fwrite(str, 1, strlen(str), ftp);

			if (ld_ctrl->ln_num == 0 || ld_ctrl->lns == NULL)
			{
				continue;
			}
			for (k=0; k<ld_ctrl->ln_num; k++)
			{
				ln_ctrl = &ld_ctrl->lns[k];
				strcpy(str, ln_ctrl->comn_obj.name);
				if (ln_ctrl->comn_obj.desc)
				{
					strcat(str, "\t");
					strcat(str, ln_ctrl->comn_obj.desc);
				}
				strcat(str, "\n");
				fwrite(str, 1, strlen(str), ftp);
				_output_sub_objs(ln_ctrl->sub_objs, ln_ctrl->sub_obj_num);
				_output_cbs(ln_ctrl->cbs, ln_ctrl->cb_num);
			}
		}
	}
	fclose(ftp);
}

/**
* 获取对象的61850参引
* obj           对象
* ref           对象61850参引
* 返回值        对象指针
*/
ST_RET acsi_get_ref_obj(ACSI_COMMON_OBJ *obj, ST_CHAR *ref)
{
	ST_CHAR fc_name[MAX_NAME_LEN] = {0};
	ACSI_COMMON_OBJ * obj_ptr = NULL;
	ACSI_COMMON_OBJ ** obj_ptrs = NULL;
	ACSI_COMMON_OBJ * parent_ptr = NULL;
	ST_INT layer_num = 1;
	ST_INT i=1;

	if (obj == NULL||ref == NULL)
	{
		return SD_FAILURE;
	}
	parent_ptr = obj->parent;
	while(parent_ptr)
	{
		layer_num ++;
		parent_ptr = parent_ptr->parent;
	}

	obj_ptrs = chk_calloc(layer_num, sizeof(ACSI_COMMON_OBJ *));

	obj_ptrs[0] = obj;
	parent_ptr = obj->parent;
	while(parent_ptr)
	{
		obj_ptrs[i] = parent_ptr;
		i ++;
		parent_ptr = parent_ptr->parent;
	}

	for (i=layer_num-1; i>=0; i--)
	{
		obj_ptr = obj_ptrs[i];
		if (obj_ptr->obj_type == OBJECT_TYPE_IED)
		{
			continue;
		}
		if (obj_ptr->obj_type == OBJECT_TYPE_FCN)
		{
			strcpy(fc_name, "[");
			strcat(fc_name, obj_ptr->name);
			strcat(fc_name, "]");
			continue;
		}
		if (obj_ptr->obj_type == OBJECT_TYPE_LD)
		{
			strcpy(ref, obj_ptr->name);
		}
		else
		{
			strcat(ref, obj_ptr->name);
		}
		if (i != 0)
		{
			if (obj_ptr->obj_type == OBJECT_TYPE_LD)
			{
				strcat(ref, "/");
			}
			else			
				strcat(ref, ".");
		}
	}
	if (strlen(fc_name))
	{
		strcat(ref, fc_name);
	}
	chk_free(obj_ptrs);
	return SD_SUCCESS;
}