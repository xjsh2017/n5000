#include "glbtypes.h"
#include "sysincs.h"
#include "scl.h"
#include "acsi_crscl.h"
#include "acsi_dir.h"
#include "mmsstruct.h"

//#include <stdio.h>
//#include <windows.h>

#include <winbase.h>
//#include <wtypes.h>

HMODULE hDllLib;
//FARPROC fpFun1;
//FARPROC fpFun2;
//FARPROC fpFun3;

///////////////////////////
//AnalyzeTxt内容结构
typedef struct AT_ITEM_STRUCT{
	char c_des[127];
	char c_content[255];	//实际内容

}_AT_ITEM_STRUCT;

//AnalyzeTxt结果结构
typedef struct AT_STRUCT{
	int n_item_num;	//节点个数
	_AT_ITEM_STRUCT *p_item;	//节点结构首地址

	int n_child_num;		//子节点个数
	struct AT_STRUCT *p_child;	//子节点结构首地址
	struct AT_STRUCT *p_parent;	//父节点指针

}_AT_STRUCT;

//单步解析结果结构,针对单一帧报文
typedef struct AT_STRUCT_STEP{

	//数据类型 0: 不解析类型 1: mms 2: goose 3: smv
	int n_type;

	//本帧报文解析结果
	_AT_STRUCT *p_at_struct;

}_AT_STRUCT_STEP;

_AT_STRUCT* at_struct1 = NULL;
_AT_STRUCT* at_struct2 = NULL;
_AT_STRUCT* at_struct3 = NULL;
_AT_STRUCT* packet = NULL;
MMS_INFO_STRUCT* mms_struct = NULL;

typedef _AT_STRUCT * (*ANALYZE_TXT_FUNC)(const char * c_file_name);
typedef _AT_STRUCT *(*GET_STRUCT_FUNC)(_AT_STRUCT *p_layer_struct,const char *c_key_des);
typedef MMS_INFO_STRUCT *(*MAKE_MMS_FUNC)(_AT_STRUCT *p_at_struct);
typedef _AT_STRUCT *(*GET_PACKET_FUNC)(_AT_STRUCT *p_at_struct,int n_frame_id);

ANALYZE_TXT_FUNC fpFun1 = NULL;
GET_STRUCT_FUNC fpFun2 = NULL;
MAKE_MMS_FUNC fpFun3 = NULL;
GET_PACKET_FUNC fpFuncGetPacket = NULL;

AP_ACSI_INFO mms_structs[1];
AP_ACSI_INFO* acsi_info = NULL;
///////////////////////////////////////////
main()
{
	SCD_INFO                scd_info = {0};
	ACSI_IED_CTRL *ied = NULL;
	ACSI_COMMON_OBJ * obj = NULL;
	ST_CHAR ref[MAX_IDENT_LEN +1] = {0};
	ST_DOUBLE t0, t1,t2,t3;

	t0 = sGetMsTime();
	ai_parse_scd("scl.SCD", &scd_info);
	t1 = sGetMsTime();
	acsi_create_ied_all(&scd_info);
	t2 = sGetMsTime();
	ai_cleanup_scd_info(&scd_info);
	t3 = sGetMsTime();
	
	ied = acsi_get_ied_ip("10.0.50.103");
//	obj = acsi_get_obj_ref(ied, "E1Q1SB1C1/LLN0$ST$Mod$stVal");

//	acsi_get_ref_obj((ACSI_COMMON_OBJ *)obj, ref);
	acsi_ied_output_all();
//	acsi_ied_destroy_all();
	printf("t1-t0:%f,.....t2-t1:%f....,t3-t2:%f",t1-t0, t2-t1, t3-t2);


	hDllLib = LoadLibrary("AnalyzeTxt.dll");	 
	if (hDllLib==NULL)
	{
		return;
	}

	 fpFun1 = (ANALYZE_TXT_FUNC)GetProcAddress(hDllLib,"AnalyzeTxt");
	if (NULL==fpFun1)
	{
		FreeLibrary(hDllLib);
		acsi_ied_destroy_all();
		return;
	}
	
	at_struct1 = (*fpFun1)("D:\\D146_GetAllDataValues");
	if (NULL == at_struct1)
	{
		FreeLibrary(hDllLib);
		acsi_ied_destroy_all();
		return;
	}
	
	fpFuncGetPacket = (GET_PACKET_FUNC)GetProcAddress(hDllLib,"GetPacketByFrameID");
	if (NULL == fpFuncGetPacket)
	{
		FreeLibrary(hDllLib);
		acsi_ied_destroy_all();
		return;
	}
	packet = (fpFuncGetPacket)(at_struct1, 74);
	//D146_getflist_reject, 81 initiate-RequestPDU, 83 initiate-ResponsePDU, 184reject

	//D146fileservice, 89 getfilelist-req(COMTRADE), 90 getfilelist-resp(COMTRADE), 
						//115 getfilelist-req(comtrade/),116 getfilelist-error(comtrade/)
						//440 fileOpen-req, 441 fileOpen-error
	//.........................................................................................................................................D146_getflist_error, 139getfilelist_req,140getfilelist_error
	//D146_getfile，fileOpen_req 496，fileOpen_resp 497，
					//fileRead _req 498, fileRead_resp510
					//fileClose_req 511, fileClose_resp 514
					//532 informationReport

	//D146_GetDataValues_FC, 154 GetFCValues_req, 156 GetFCValues_resp(clean时特殊处理)

	//D146_BRCB,234 SetBRCBValues-req,147 write-resp, 27 GetBRCBValues-req, 29 read-resp(clean时特殊处理)

	//D146_GetSGCB_GetDs,159 GetSGCBValues-req, 166 GetSGCBValues-resp(failure),
						//208 GetDataSetValues-req,210 GetDataSetValues_resp

	//D146_setsgv,758 getsgv-req，760 getsgv-resp
					//764 cnfsg-req, 768 cnfsg-resp
					//761 setsgv-req，762 setsgv-resp
					//834 editsg-req，844editsg-resp

	//D146_SelectActSG,318 是selectActSG-req, 918 selectActSG-resp

	//D146_operate, 261 operate-req, 370 operate-resp

	//D146_GetAllDataValues, 74read-req, 77 read-resp

	//D146_DirectoryService, 148 getNameList, 102 getVariableAccessAttributes ,589 getNamedVariableListAttributes 
	
	if (NULL == packet)
	{
		FreeLibrary(hDllLib);
		acsi_ied_destroy_all();
		return;
	}

	 fpFun2 = (GET_STRUCT_FUNC)GetProcAddress(hDllLib,"GetStructByKeyDes");
	if (NULL==fpFun2)
	{
		FreeLibrary(hDllLib);
		acsi_ied_destroy_all();
		return;
	}
	at_struct2 = (*fpFun2)(packet, "MMS");
	if (NULL == at_struct2)
	{
		FreeLibrary(hDllLib);
		acsi_ied_destroy_all();
		return;
	}

	fpFun3 = (MAKE_MMS_FUNC)GetProcAddress(hDllLib,"MakeMMSInfoStruct");
	if (NULL==fpFun3)
	{
		FreeLibrary(hDllLib);
		acsi_ied_destroy_all();
		return;
	}
	mms_struct = (*fpFun3)(at_struct2);
	if (NULL == mms_struct)
	{
		FreeLibrary(hDllLib);
		acsi_ied_destroy_all();
		return;
	}
	acsi_info = acsi_cre_info_mmsInfo(mms_struct, 1);

	acsi_clean_acsi_info(acsi_info, 1);

	FreeLibrary(hDllLib);
	acsi_ied_destroy_all();
	return;
}