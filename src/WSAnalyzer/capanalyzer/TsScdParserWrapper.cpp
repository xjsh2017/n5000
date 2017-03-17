#include "TsScdParserWrapper.h"

CTsScdParserWrapper * CTsScdParserWrapper::tsScdInstance = NULL;
CTsScdParserWrapper::CTsScdParserWrapper(void)
{
	//加载动态库后返回的句柄
	m_hDllInst                     = NULL;
	//各函数接口
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
/*初始化，加载动态库，导出库函数,cLibPathFullName为动态库相对路径+动态库名称*/
int CTsScdParserWrapper::Initialize(const char * cLibPathName)
{
	if(NULL == cLibPathName)
	{
		return -1;
	}
	try
	{
		//加载库
        m_hDllInst = xj_load_library(cLibPathName);
		if(NULL == m_hDllInst)
        {
			return -1;
		}
		//导入库函数
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
			|| NULL == m_pfun_free || NULL == m_pfun_getitemdesc_byref)//初始化失败
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
/*卸载动态库*/
int CTsScdParserWrapper::UnInitialize()
{
	if(m_hDllInst != NULL)
	{
		Free_I();//释放动态库申请的资源
        xj_free_library(m_hDllInst);
		//加载动态库后返回的句柄
		m_hDllInst                     = NULL;
		//各函数接口
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
/*************加载scd文件*************/
bool CTsScdParserWrapper::LoadscdFile_I(char* c_filepath_name)
{
	if(NULL == c_filepath_name || NULL == m_pfun_load_scdfile)
	{
		return false;
	}
	return m_pfun_load_scdfile(c_filepath_name);
}
/*************保存scd文件*************/
bool CTsScdParserWrapper::SaveScdFile_I(char* c_filepath_name)
{
	if(NULL == c_filepath_name || NULL == m_pfun_save_scdfile)
	{
		return false;
	}
	return m_pfun_save_scdfile(c_filepath_name);
}
/*********根据APPID获取对应的采样数据集队列*********/
SCD_ITEM_LIST* CTsScdParserWrapper::Get_Dslist_sv_Byappid_I(int nappid)
{
	if(nappid <= 0 || NULL == m_pfun_getdslist_sv_byappid )
	{
		return NULL;
	}
	return m_pfun_getdslist_sv_byappid(nappid);
}
/*********根据APPID获取对应的GOOSE数据集队列*********/
SCD_ITEM_LIST* CTsScdParserWrapper::Get_Dslist_gse_Byappid_I(int nappid)
{
	if(nappid <= 0 || NULL == m_pfun_getdslist_gse_byappid )
	{
		return NULL;
	}
	return m_pfun_getdslist_gse_byappid(nappid);
}
/*********根据数据集参引获取数据集队列*********/
SCD_ITEM_LIST* CTsScdParserWrapper::Get_Dslist_Bydsref_I(char * c_ds_ref)
{
	if( NULL == c_ds_ref || NULL == m_pfun_getdslist_bydsref )
	{
		return NULL;
	}
	return m_pfun_getdslist_bydsref(c_ds_ref);
}
/*********根据报告控制块参引获取数据集队列*********/
SCD_ITEM_LIST* CTsScdParserWrapper::Get_Dslist_Byrptblockref_I(const char * c_rptblock_ref)
{
	if( NULL == c_rptblock_ref || NULL == m_pfun_getdslist_byrptblockref )
	{
		return NULL;
	}
	return m_pfun_getdslist_byrptblockref(c_rptblock_ref);
}
/********根据数据参引获取数据对象内容**********/
SCD_ITEM* CTsScdParserWrapper::Get_DoiItem_Bydoiref_I(char * c_doi_ref)
{
	if( NULL == c_doi_ref || NULL == m_pfun_getdoiitem_bydoiref )
	{
		return NULL;
	}
	return m_pfun_getdoiitem_bydoiref(c_doi_ref);
}
/********根据FCDA参引获取数据对象内容**********/
SCD_ITEM* CTsScdParserWrapper::Get_DoiItem_BytFcdaRef_I(char * c_fcda_ref)
{
	if(NULL == c_fcda_ref || NULL == m_pfun_getdoiitem_byfcdaref)
	{
		return NULL;
	}
	return m_pfun_getdoiitem_byfcdaref(c_fcda_ref);
}
/***********释放数据队列***************/
int CTsScdParserWrapper::Free_Scd_ItemList_I(SCD_ITEM_LIST* p_scd_Item_List)
{
	if(NULL == p_scd_Item_List || NULL == m_pfun_free_scditemlist)
	{
		return -1;
	}
	return m_pfun_free_scditemlist(p_scd_Item_List);
}
/***********释放单个数据结构***************/
int CTsScdParserWrapper::Free_Scd_Item_I(SCD_ITEM* p_scd_item)
{
	if(NULL == p_scd_item || NULL == m_pfun_free_scditem)
	{
		return -1;
	}
	return m_pfun_free_scditem(p_scd_item);
}
/***********释放***************/
bool  CTsScdParserWrapper::Free_I()
{
	if(NULL == m_pfun_free)
		return false;
	return m_pfun_free();
}
/**************根据iedname获取ied描述****************************/
int CTsScdParserWrapper::GetItemDesc_ByRef_I(char* c_item_name,char* c_item_desc,int n_item_type)
{
	if(NULL == m_pfun_getitemdesc_byref || NULL == c_item_name || NULL == c_item_desc)
		return -1;
	if(strlen(c_item_name) == 0)
		return -1;
	return m_pfun_getitemdesc_byref(c_item_name,c_item_desc,n_item_type);
}
