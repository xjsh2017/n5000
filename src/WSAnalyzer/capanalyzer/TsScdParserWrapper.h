#pragma once
/********************************************************************
*	filename: 	CTsScdParserWrapper
*	author:		yinzhehong
*	created:	2013/05/20
*	purpose:  	scd文件动态库解析接口类 
*   version:    1.0
*********************************************************************/
#include "tsscdparser_define.h"
#include "xjlib.h"
/*************加载scd文件*************/
typedef bool (*LOAD_SCDFILE)(char* c_filepath_name);
/*************保存scd文件*************/
typedef bool (*SAVE_SCDFILE)(char* c_filepath_name);
/*********根据APPID获取对应的采样数据集队列*********/
typedef SCD_ITEM_LIST* (*GET_DSLIST_SV_BYAPPID)(int nappid);
/*********根据APPID获取对应的GOOSE数据集队列*********/
typedef SCD_ITEM_LIST* (*GET_DSLIST_GSE_BYAPPID)(int nappid);
/*********根据数据集参引获取数据集队列*********/
typedef SCD_ITEM_LIST* (*GET_DSLIST_BYDSREF)(char * c_ds_ref);
/*********根据报告控制块参引获取数据集队列*********/
typedef SCD_ITEM_LIST* (*GET_DSLIST_BYRPTBLOCKREF)(const char * c_rptblock_ref);
/********根据数据参引获取数据对象内容**********/
typedef SCD_ITEM* (*GET_DOIITEM_BYDOIREF)(char * c_doi_ref);
/********根据FCDA参引获取数据对象内容**********/
typedef SCD_ITEM* (*GET_DOIITEM_BYFCDAREF)(char * c_fcda_ref);
/***********释放数据队列***************/
typedef int (*FREE_SCD_ITEMLIST)(SCD_ITEM_LIST* p_scd_Item_List);
/***********释放单个数据结构***************/
typedef int (*FREE_SCD_ITEM)(SCD_ITEM* p_scd_item);
/***********释放***************/
typedef bool (*FREE)();
  /**************根据iedname获取ied描述****************************/
typedef int (*GET_ITEM_BYREF)(char* c_item_ref,char* c_item_desc,int n_item_type);
class CTsScdParserWrapper
{
private:
    static CTsScdParserWrapper *tsScdInstance;
public:
    static CTsScdParserWrapper *getInstance()
    {
        if(tsScdInstance == NULL)
        {
            tsScdInstance = new CTsScdParserWrapper();
        }
        return tsScdInstance;
    }
public:
	CTsScdParserWrapper(void);
	~CTsScdParserWrapper(void);
public:
	/*初始化，加载动态库，导出库函数,cLibPathFullName为动态库相对路径+动态库名称*/
	int Initialize(const char * cLibPathName);
	/*卸载动态库*/
	int UnInitialize();
public:
	/*************加载scd文件*************/
	bool LoadscdFile_I(char* c_filepath_name);
	/*************保存scd文件*************/
	bool SaveScdFile_I(char* c_filepath_name);
	/*******获取数据集队列接口*****/
	/*********根据APPID获取对应的采样数据集队列*********/
	SCD_ITEM_LIST* Get_Dslist_sv_Byappid_I(int nappid);
	/*********根据APPID获取对应的GOOSE数据集队列*********/
	SCD_ITEM_LIST* Get_Dslist_gse_Byappid_I(int nappid);
	/*********根据数据集参引获取数据集队列*********/
	SCD_ITEM_LIST* Get_Dslist_Bydsref_I(char * c_ds_ref);
	/*********根据报告控制块参引获取数据集队列*********/
	SCD_ITEM_LIST* Get_Dslist_Byrptblockref_I(const char * c_rptblock_ref);
	/********根据数据参引获取数据对象内容**********/
	SCD_ITEM* Get_DoiItem_Bydoiref_I(char * c_doi_ref);
	/********根据FCDA参引获取数据对象内容**********/
	SCD_ITEM* Get_DoiItem_BytFcdaRef_I(char * c_fcda_ref);
	/***********释放数据队列***************/
	int Free_Scd_ItemList_I(SCD_ITEM_LIST* p_scd_Item_List);
	/***********释放单个数据结构***************/
	int Free_Scd_Item_I(SCD_ITEM* p_scd_item);
	/***********释放***************/
	bool Free_I();
	/**************根据iedname获取ied描述****************************/
	int GetItemDesc_ByRef_I(char* c_item_name,char* c_item_desc,int n_item_type);
private:
	//加载动态库后返回的句柄
    XJHANDLE					m_hDllInst;
	//各函数接口
	LOAD_SCDFILE				m_pfun_load_scdfile;
	SAVE_SCDFILE                m_pfun_save_scdfile;
	GET_DSLIST_SV_BYAPPID       m_pfun_getdslist_sv_byappid;
	GET_DSLIST_GSE_BYAPPID      m_pfun_getdslist_gse_byappid;
	GET_DSLIST_BYDSREF          m_pfun_getdslist_bydsref;
	GET_DSLIST_BYRPTBLOCKREF    m_pfun_getdslist_byrptblockref;
	GET_DOIITEM_BYDOIREF        m_pfun_getdoiitem_bydoiref;
	GET_DOIITEM_BYFCDAREF       m_pfun_getdoiitem_byfcdaref;
	FREE_SCD_ITEMLIST           m_pfun_free_scditemlist;
	FREE_SCD_ITEM               m_pfun_free_scditem;
	FREE                        m_pfun_free;
	GET_ITEM_BYREF              m_pfun_getitemdesc_byref;    
};

