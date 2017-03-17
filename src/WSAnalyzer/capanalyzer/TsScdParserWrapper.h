#pragma once
/********************************************************************
*	filename: 	CTsScdParserWrapper
*	author:		yinzhehong
*	created:	2013/05/20
*	purpose:  	scd�ļ���̬������ӿ��� 
*   version:    1.0
*********************************************************************/
#include "tsscdparser_define.h"
#include "xjlib.h"
/*************����scd�ļ�*************/
typedef bool (*LOAD_SCDFILE)(char* c_filepath_name);
/*************����scd�ļ�*************/
typedef bool (*SAVE_SCDFILE)(char* c_filepath_name);
/*********����APPID��ȡ��Ӧ�Ĳ������ݼ�����*********/
typedef SCD_ITEM_LIST* (*GET_DSLIST_SV_BYAPPID)(int nappid);
/*********����APPID��ȡ��Ӧ��GOOSE���ݼ�����*********/
typedef SCD_ITEM_LIST* (*GET_DSLIST_GSE_BYAPPID)(int nappid);
/*********�������ݼ�������ȡ���ݼ�����*********/
typedef SCD_ITEM_LIST* (*GET_DSLIST_BYDSREF)(char * c_ds_ref);
/*********���ݱ�����ƿ������ȡ���ݼ�����*********/
typedef SCD_ITEM_LIST* (*GET_DSLIST_BYRPTBLOCKREF)(const char * c_rptblock_ref);
/********�������ݲ�����ȡ���ݶ�������**********/
typedef SCD_ITEM* (*GET_DOIITEM_BYDOIREF)(char * c_doi_ref);
/********����FCDA������ȡ���ݶ�������**********/
typedef SCD_ITEM* (*GET_DOIITEM_BYFCDAREF)(char * c_fcda_ref);
/***********�ͷ����ݶ���***************/
typedef int (*FREE_SCD_ITEMLIST)(SCD_ITEM_LIST* p_scd_Item_List);
/***********�ͷŵ������ݽṹ***************/
typedef int (*FREE_SCD_ITEM)(SCD_ITEM* p_scd_item);
/***********�ͷ�***************/
typedef bool (*FREE)();
  /**************����iedname��ȡied����****************************/
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
	/*��ʼ�������ض�̬�⣬�����⺯��,cLibPathFullNameΪ��̬�����·��+��̬������*/
	int Initialize(const char * cLibPathName);
	/*ж�ض�̬��*/
	int UnInitialize();
public:
	/*************����scd�ļ�*************/
	bool LoadscdFile_I(char* c_filepath_name);
	/*************����scd�ļ�*************/
	bool SaveScdFile_I(char* c_filepath_name);
	/*******��ȡ���ݼ����нӿ�*****/
	/*********����APPID��ȡ��Ӧ�Ĳ������ݼ�����*********/
	SCD_ITEM_LIST* Get_Dslist_sv_Byappid_I(int nappid);
	/*********����APPID��ȡ��Ӧ��GOOSE���ݼ�����*********/
	SCD_ITEM_LIST* Get_Dslist_gse_Byappid_I(int nappid);
	/*********�������ݼ�������ȡ���ݼ�����*********/
	SCD_ITEM_LIST* Get_Dslist_Bydsref_I(char * c_ds_ref);
	/*********���ݱ�����ƿ������ȡ���ݼ�����*********/
	SCD_ITEM_LIST* Get_Dslist_Byrptblockref_I(const char * c_rptblock_ref);
	/********�������ݲ�����ȡ���ݶ�������**********/
	SCD_ITEM* Get_DoiItem_Bydoiref_I(char * c_doi_ref);
	/********����FCDA������ȡ���ݶ�������**********/
	SCD_ITEM* Get_DoiItem_BytFcdaRef_I(char * c_fcda_ref);
	/***********�ͷ����ݶ���***************/
	int Free_Scd_ItemList_I(SCD_ITEM_LIST* p_scd_Item_List);
	/***********�ͷŵ������ݽṹ***************/
	int Free_Scd_Item_I(SCD_ITEM* p_scd_item);
	/***********�ͷ�***************/
	bool Free_I();
	/**************����iedname��ȡied����****************************/
	int GetItemDesc_ByRef_I(char* c_item_name,char* c_item_desc,int n_item_type);
private:
	//���ض�̬��󷵻صľ��
    XJHANDLE					m_hDllInst;
	//�������ӿ�
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

