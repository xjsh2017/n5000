#include "TsScdParserWrapper.h"

CTsScdParserWrapper * CTsScdParserWrapper::tsScdInstance = NULL;
CTsScdParserWrapper::CTsScdParserWrapper(void)
{
	//���ض�̬��󷵻صľ��
	m_hDllInst                     = NULL;
	//�������ӿ�
	m_pfun_load_scdfile            = NULL;
    m_pfun_save_scdfile            = NULL;
	m_pfun_getdslist_sv_byappid    = NULL;
	m_pfun_getdslist_gse_byappid   = NULL;
	m_pfun_getdslist_bydsref       = NULL;
	m_pfun_getdslist_byrptblockref = NULL;
	m_pfun_getdoiitem_bydoiref     = NULL;
	m_pfun_free_scditemlist        = NULL;
	m_pfun_free_scditem            = NULL;
	m_pfun_free                    = NULL;
	m_pfun_getitemdesc_byref       = NULL;
	m_pfun_getdoiitem_byfcdaref    = NULL;
}
CTsScdParserWrapper::~CTsScdParserWrapper(void)
{
	UnInitialize();
}
/*��ʼ�������ض�̬�⣬�����⺯��,cLibPathFullNameΪ��̬�����·��+��̬������*/
int CTsScdParserWrapper::Initialize(const char * cLibPathName)
{
	if(NULL == cLibPathName)
	{
		return -1;
	}
	try
	{
		//���ؿ�
        m_hDllInst = xj_load_library(cLibPathName);
		if(NULL == m_hDllInst)
        {
			return -1;
		}
		//����⺯��
        m_pfun_load_scdfile =		      (LOAD_SCDFILE) xj_get_addr(m_hDllInst,"tsscdparser_loadscdfile");
        m_pfun_save_scdfile =		      (SAVE_SCDFILE) xj_get_addr(m_hDllInst,"tsscdparser_savescdfile");
        m_pfun_getdslist_sv_byappid=      (GET_DSLIST_SV_BYAPPID) xj_get_addr(m_hDllInst,"tsscdparser_getdslist_sv_byappid");
        m_pfun_getdslist_gse_byappid =    (GET_DSLIST_GSE_BYAPPID) xj_get_addr(m_hDllInst,"tsscdparser_getdslist_gse_byappid");
        m_pfun_getdslist_bydsref =        (GET_DSLIST_BYDSREF) xj_get_addr(m_hDllInst,"tsscdparser_getdslist_bydsref");
        m_pfun_getdslist_byrptblockref =  (GET_DSLIST_BYRPTBLOCKREF) xj_get_addr(m_hDllInst,"tsscdparser_getdslist_byrptblockref");
        m_pfun_getdoiitem_bydoiref =      (GET_DOIITEM_BYDOIREF) xj_get_addr(m_hDllInst,"tsscdparser_getdoitem_bydoiref");
        m_pfun_free_scditemlist =         (FREE_SCD_ITEMLIST) xj_get_addr(m_hDllInst,"tsscdparser_free_scd_itemlist");
        m_pfun_free_scditem =             (FREE_SCD_ITEM) xj_get_addr(m_hDllInst,"tsscdparser_free_scd_item");
        m_pfun_free         =             (FREE) xj_get_addr(m_hDllInst,"tsscdparser_free");
        m_pfun_getitemdesc_byref   =      (GET_ITEM_BYREF) xj_get_addr(m_hDllInst,"tsscdparser_getitemdesc_byitemref");
        m_pfun_getdoiitem_byfcdaref  =     (GET_DOIITEM_BYFCDAREF) xj_get_addr(m_hDllInst,"tsscdparser_getdoitem_byfcdaref");
		if(NULL ==  m_pfun_load_scdfile || NULL == m_pfun_save_scdfile|| NULL == m_pfun_getdslist_sv_byappid
			|| NULL == m_pfun_getdslist_gse_byappid || NULL == m_pfun_getdslist_bydsref || NULL == m_pfun_getdslist_byrptblockref
			|| NULL == m_pfun_getdoiitem_bydoiref || NULL == m_pfun_free_scditemlist || NULL == m_pfun_free_scditem 
			|| NULL == m_pfun_free || NULL == m_pfun_getitemdesc_byref)//��ʼ��ʧ��
        {
			return -1;
		}
		return 0;
	}
    catch (...)
	{
        ;
	}

	return -1;
}
/*ж�ض�̬��*/
int CTsScdParserWrapper::UnInitialize()
{
	if(m_hDllInst != NULL)
	{
		Free_I();//�ͷŶ�̬���������Դ
        xj_free_library(m_hDllInst);
		//���ض�̬��󷵻صľ��
		m_hDllInst                     = NULL;
		//�������ӿ�
		m_pfun_load_scdfile            = NULL;
		m_pfun_save_scdfile            = NULL;
		m_pfun_getdslist_sv_byappid    = NULL;
		m_pfun_getdslist_gse_byappid   = NULL;
		m_pfun_getdslist_bydsref       = NULL;
		m_pfun_getdslist_byrptblockref = NULL;
		m_pfun_getdoiitem_bydoiref     = NULL;
		m_pfun_free_scditemlist        = NULL;
		m_pfun_free_scditem            = NULL;
		m_pfun_free                    = NULL;
		m_pfun_getitemdesc_byref       = NULL;
		m_pfun_getdoiitem_byfcdaref    = NULL;
	}
	return 0;
}
/*************����scd�ļ�*************/
bool CTsScdParserWrapper::LoadscdFile_I(char* c_filepath_name)
{
	if(NULL == c_filepath_name || NULL == m_pfun_load_scdfile)
	{
		return false;
	}
	return m_pfun_load_scdfile(c_filepath_name);
}
/*************����scd�ļ�*************/
bool CTsScdParserWrapper::SaveScdFile_I(char* c_filepath_name)
{
	if(NULL == c_filepath_name || NULL == m_pfun_save_scdfile)
	{
		return false;
	}
	return m_pfun_save_scdfile(c_filepath_name);
}
/*********����APPID��ȡ��Ӧ�Ĳ������ݼ�����*********/
SCD_ITEM_LIST* CTsScdParserWrapper::Get_Dslist_sv_Byappid_I(int nappid)
{
	if(nappid <= 0 || NULL == m_pfun_getdslist_sv_byappid )
	{
		return NULL;
	}
	return m_pfun_getdslist_sv_byappid(nappid);
}
/*********����APPID��ȡ��Ӧ��GOOSE���ݼ�����*********/
SCD_ITEM_LIST* CTsScdParserWrapper::Get_Dslist_gse_Byappid_I(int nappid)
{
	if(nappid <= 0 || NULL == m_pfun_getdslist_gse_byappid )
	{
		return NULL;
	}
	return m_pfun_getdslist_gse_byappid(nappid);
}
/*********�������ݼ�������ȡ���ݼ�����*********/
SCD_ITEM_LIST* CTsScdParserWrapper::Get_Dslist_Bydsref_I(char * c_ds_ref)
{
	if( NULL == c_ds_ref || NULL == m_pfun_getdslist_bydsref )
	{
		return NULL;
	}
	return m_pfun_getdslist_bydsref(c_ds_ref);
}
/*********���ݱ�����ƿ������ȡ���ݼ�����*********/
SCD_ITEM_LIST* CTsScdParserWrapper::Get_Dslist_Byrptblockref_I(const char * c_rptblock_ref)
{
	if( NULL == c_rptblock_ref || NULL == m_pfun_getdslist_byrptblockref )
	{
		return NULL;
	}
	return m_pfun_getdslist_byrptblockref(c_rptblock_ref);
}
/********�������ݲ�����ȡ���ݶ�������**********/
SCD_ITEM* CTsScdParserWrapper::Get_DoiItem_Bydoiref_I(char * c_doi_ref)
{
	if( NULL == c_doi_ref || NULL == m_pfun_getdoiitem_bydoiref )
	{
		return NULL;
	}
	return m_pfun_getdoiitem_bydoiref(c_doi_ref);
}
/********����FCDA������ȡ���ݶ�������**********/
SCD_ITEM* CTsScdParserWrapper::Get_DoiItem_BytFcdaRef_I(char * c_fcda_ref)
{
	if(NULL == c_fcda_ref || NULL == m_pfun_getdoiitem_byfcdaref)
	{
		return NULL;
	}
	return m_pfun_getdoiitem_byfcdaref(c_fcda_ref);
}
/***********�ͷ����ݶ���***************/
int CTsScdParserWrapper::Free_Scd_ItemList_I(SCD_ITEM_LIST* p_scd_Item_List)
{
	if(NULL == p_scd_Item_List || NULL == m_pfun_free_scditemlist)
	{
		return -1;
	}
	return m_pfun_free_scditemlist(p_scd_Item_List);
}
/***********�ͷŵ������ݽṹ***************/
int CTsScdParserWrapper::Free_Scd_Item_I(SCD_ITEM* p_scd_item)
{
	if(NULL == p_scd_item || NULL == m_pfun_free_scditem)
	{
		return -1;
	}
	return m_pfun_free_scditem(p_scd_item);
}
/***********�ͷ�***************/
bool  CTsScdParserWrapper::Free_I()
{
	if(NULL == m_pfun_free)
		return false;
	return m_pfun_free();
}
/**************����iedname��ȡied����****************************/
int CTsScdParserWrapper::GetItemDesc_ByRef_I(char* c_item_name,char* c_item_desc,int n_item_type)
{
	if(NULL == m_pfun_getitemdesc_byref || NULL == c_item_name || NULL == c_item_desc)
		return -1;
	if(strlen(c_item_name) == 0)
		return -1;
	return m_pfun_getitemdesc_byref(c_item_name,c_item_desc,n_item_type);
}
