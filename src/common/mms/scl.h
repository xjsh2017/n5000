#ifndef SCL_INCLUDED
#define SCL_INCLUDED
/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*	(c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*	2004-2004 All Rights Reserved					*/
/*									*/
/* MODULE NAME : scl.h							*/
/* PRODUCT(S)  : MMS-EASE-LITE						*/
/*									*/
/* MODULE DESCRIPTION :							*/
/*	SCL main header file.						*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev     Comments					*/
/* --------  ---  ------   -------------------------------------------  */
/* 07/25/05  JRB    12     scl2_ld_create_all: Add brcb_bufsize arg.	*/
/*			   SCL_INFO: add brcb_bufsize element.		*/
/* 06/28/05  JRB    11     Del scl2_ld_destroy_all. Use mvl_vmd_destroy.*/
/* 06/24/05  JRB    10     Chg "ord" from unsigned to signed value.	*/
/* 05/27/05  CRM    09	   Add scl_info_destroy & scl2_ld_destroy_all.	*/
/* 03/22/05  JRB    08     Add ifdef __cplusplus to work with C++ code.	*/
/* 02/15/05  JRB    07     Add iedName to SCL_INFO.			*/
/*			   Add domName to SCL_LD & SCL_FCDA.		*/
/*			   Add varName to SCL_LN.			*/
/*			   Define SCL_HEADER struct & add it to SCL_INFO.*/
/*			   Add scl_header_save.				*/
/* 08/06/04  JRB    06     Add scl_parse.				*/
/*			   Move mapping functions to user header.	*/
/* 07/19/04  JRB    05     Add out_filename arg to datamap_cfg_read.	*/
/*			   Add any_cfg_count proto.			*/
/* 07/15/04  JRB    04     Clean up SCL_GCB, scl_gcb_add.		*/
/* 07/09/04  JRB    03     scl2_ld_create_all: add reportScanRate arg.	*/
/* 07/02/04  JRB    02     Add SCL_LCB, SCL_GCB, scl_lcb_add, scl_gcb_add.*/
/* 06/08/04  JRB    01     Initial Revision.				*/
/************************************************************************/
#include "gen_list.h"
#include "mms_def2.h"		/* need MAX_IDENT_LEN	*/
#include "mvl_defs.h"		/* need MVL_VAR_ASSOC, etc.	*/

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_VALKIND_LEN	4	/* Spec, Conf, RO, or Set		*/
#define MAX_CDC_LEN	5	/* SPS, DPS, etc. (CURVE is longest)	*/
#define MAX_FC_LEN	2	/* ST, MX, etc.				*/

/* These defines used in SCL_DA struct to differentiate between structs	*/
/* containing DA info and structs containing SDO info.			*/
#define SCL_OBJTYPE_DA	0
#define SCL_OBJTYPE_SDO	1

/* This def used for flattened leaf names (longer to allow array indices)*/
#define MAX_FLAT_LEN	(MAX_IDENT_LEN*2)
#define MAX_OBJECT_NAME_LEN  32 /*对象名称最大字符长度 */

/*所要解析的P子节点的父节点标识 */
#define P_ADDRESS      0   
#define P_GSE          1
#define P_SMV          2
#define P_PHYSCONN     3  
	
#define MAX_AR_NAME     64
#define MAX_AP_TITLE    80
#define MAX_AE_QUAL     32

/************************************************************************/
/* Structures to contain information from "Header" section of SCL.	*/
/************************************************************************/
typedef struct
  {
  /* NOTE: only required elements included here. Add optional elements as needed.*/
  ST_CHAR id            [MAX_IDENT_LEN+1];

  /* Defined values for "nameStructure" attribute	*/
  #define SCL_NAMESTRUCTURE_IEDNAME	0	/* value="IEDName"	*/
  #define SCL_NAMESTRUCTURE_FUNCNAME	1	/* value="FuncName"	*/
  ST_INT nameStructure;
  } SCL_HEADER;

/************************************************************************/
/* Structures to contain information from "AccessPoint" section of SCL.*/
/************************************************************************/

/* "scl_fcda_add" allocates this struct, fills it in,		*/
/* and adds it to the linked list "fcdaHead" in SCL_DATASET.		*/
typedef struct
  {
  DBL_LNK l;
  ST_CHAR domName[MAX_IDENT_LEN+1];	/* domain name (constructed)	*/
  ST_CHAR ldInst  [MAX_IDENT_LEN+1];
  ST_CHAR prefix  [MAX_IDENT_LEN+1];
  ST_CHAR lnInst  [MAX_IDENT_LEN+1];
  ST_CHAR lnClass [MAX_IDENT_LEN+1];
  ST_CHAR doName  [MAX_IDENT_LEN+1];
  ST_CHAR daName  [MAX_IDENT_LEN+1];
  ST_CHAR fc      [MAX_FC_LEN+1];	/* ST, MX, etc.			*/
  } SCL_FCDA;

/* "scl_dai_add" allocates this struct, fills it in,			*/
/* and adds it to the linked list "daiHead" in SCL_LN.			*/
/* The "flattened" name must be constructed from the "name" & "ix"	*/
/* attribute of the DOI and DAI and possibly the intervening SDI,	*/
/* where "ix" is an array index (we'll need some new flattened name	*/
/* syntax to handle the array index).					*/
/* The "accessControl" attr of DOI is ignored (don't know what it means).*/
/* The "desc" attr of DOI, SDI, & DAI are ignored (not useful).		*/
typedef struct
  {
  DBL_LNK l;
  ST_CHAR flattened[MAX_IDENT_LEN+1];	/* flattened attribute name	*/
					/* constructed from "name" & "ix"*/
					/* from DOI, SDI, & DAI		*/
  ST_CHAR desc[MAX_IDENT_LEN+1]; //保存DOI的desc属性
  ST_CHAR *Val;				/* attribute value text		*/
					/* allocate appropriate size buffer*/
  ST_CHAR sAddr[MAX_IDENT_LEN+1];	/* from DAI			*/
  ST_CHAR valKind[MAX_VALKIND_LEN+1];	/* from DAI			*/
  } SCL_DAI;

/* "scl_dataset_add" allocates this struct,			*/
/* and adds it to the linked list "datasetHead" in SCL_LN.	*/
typedef struct
  {
  DBL_LNK l;
  ST_CHAR name[MAX_IDENT_LEN+1];	/* dataset name		*/
  ST_CHAR desc[MAX_IDENT_LEN+1];	/* description (optional)*/
					/* TRUNCATED if longer than buffer*/
  SCL_FCDA *fcdaHead;			/* head of list of FCDA	*/
  } SCL_DATASET;

/* "scl_rcb_add" allocates this struct, fills it in,		*/
/* and adds it to the linked list "rcbHead" in SCL_LN.		*/
typedef struct
  {
  DBL_LNK l;
  ST_CHAR name[MAX_IDENT_LEN+1];
  ST_CHAR desc[MAX_IDENT_LEN+1];	/* description (optional)*/
					/* TRUNCATED if longer than buffer*/
  ST_CHAR datSet[MAX_IDENT_LEN+1];
  ST_UINT intgPd;
  ST_CHAR rptID[MAX_IDENT_LEN+1];
  ST_UINT confRev;
  ST_BOOLEAN buffered;		/* TRUE if this is buffered RCB	*/
  ST_UINT bufTime;
  ST_UINT8 TrgOps [1];		/* 8-bit bitstring			*/
				/* Boolean vals from SCL file		*/
				/* (dchg, qchg, dupd, & period)		*/
				/* used to set bits in TrgOps bitstring	*/
  ST_UINT8 OptFlds [2];		/* 9-bit bitstring			*/
				/* Boolean vals from SCL file		*/
				/* (seqNum, timeStamp, dataSet,		*/
				/* reasonCode, dataRef, bufOvfl,	*/
				/* entryID, configRef)			*/
				/* segmentation boolean is ignored	*/
				/* used to set bits in OptFlds bitstring*/

  /* NOTE: the RptEnabled element of the SCL file is ignored. SISCO software*/
  /* does not need to know which clients may access the RCB.		*/
  ST_INT max_num;//实例化最大个数
} SCL_RCB;			/* Report Control Block	*/

typedef struct
  {
  /* Counters of different types of BRCB.	*/
  ST_UINT brcb_count_complex;
  ST_UINT brcb_count_ST;
  ST_UINT brcb_count_MX;
  ST_UINT brcb_count_CO;
  ST_UINT brcb_count_SP;
  ST_UINT brcb_count_SG;
  ST_UINT brcb_count_SE;
  ST_UINT brcb_count_SV;
  ST_UINT brcb_count_CF;
  ST_UINT brcb_count_DC;
  ST_UINT brcb_count_EX;

  /* Counters of different types of URCB.	*/
  ST_UINT urcb_count_complex;
  ST_UINT urcb_count_ST;
  ST_UINT urcb_count_MX;
  ST_UINT urcb_count_CO;
  ST_UINT urcb_count_SP;
  ST_UINT urcb_count_SG;
  ST_UINT urcb_count_SE;
  ST_UINT urcb_count_SV;
  ST_UINT urcb_count_CF;
  ST_UINT urcb_count_DC;
  ST_UINT urcb_count_EX;
  } SCL_RCB_COUNTERS;

/************************************************************************/
/*			LOG Control Block				*/
/* "scl_lcb_add" allocates this struct, fills it in,			*/
/* and adds it to the linked list "lcbHead" in SCL_LN.			*/
typedef struct
  {
  DBL_LNK l;
  ST_CHAR name[MAX_IDENT_LEN+1];
  ST_CHAR desc[MAX_IDENT_LEN+1];	/* description (optional)*/
					/* TRUNCATED if longer than buffer*/
  ST_CHAR datSet[MAX_IDENT_LEN+1];
  ST_UINT intgPd;
  ST_CHAR logName[MAX_IDENT_LEN+1];
  ST_BOOLEAN logEna;
  ST_BOOLEAN reasonCode;
  ST_UINT8 TrgOps [1];		/* 8-bit bitstring			*/
				/* Boolean vals from SCL file		*/
				/* (dchg, qchg, dupd, & period)		*/
				/* used to set bits in TrgOps bitstring	*/
  } SCL_LCB;

/************************************************************************/
/* Added By djf 2007-08-16 */
/*		Setting Group Control Block			*/
/* "scl_sgcb_add" allocates this struct, fills it in,		*/
/*and adds it to the linked list "sgcbHead" in SCL_LN		*/
typedef struct
{
	ST_CHAR *desc;	/* description (optional)*/
	/* TRUNCATED if longer than buffer*/
	ST_UINT numOfSG;			/* Number of SG	*/
	ST_UINT actSG;              /* ActSG */
} SCL_SGCB;

/************************************************************************/
/*			GOOSE Control Block				*/
/* "scl_gcb_add" allocates this struct, fills it in,			*/
/* and adds it to the linked list "gcbHead" in SCL_LN.			*/
typedef struct
  {
  DBL_LNK l;
  ST_CHAR name[MAX_IDENT_LEN+1];
  ST_CHAR desc[MAX_IDENT_LEN+1];	/* description (optional)*/
					/* TRUNCATED if longer than buffer*/
  ST_CHAR datSet[MAX_IDENT_LEN+1];	/* for GOOSE only	*/
  ST_UINT confRev;			/* for GOOSE only	*/
  ST_BOOLEAN isGoose;	/* SD_TRUE if "GOOSE", SD_FALSE if "GSSE"*/
  ST_CHAR appID[MAX_IDENT_LEN+1];	/* for GOOSE only	*/
  /* The SCL file may also contain one or more "IEDName" elements to	*/
  /* indicate IEDs that should subscribe for GOOSE data. We have no	*/
  /* way to use this information, so it is ignored.			*/
  } SCL_GCB;

/* added for do desc 20080617 djf */
typedef struct 
{
	DBL_LNK l;
	ST_CHAR doName[MAX_IDENT_LEN+1];
	ST_CHAR desc[MAX_IDENT_LEN+1];
}SCL_DOI;

/* "scl_ln_add" allocates this struct, fills it in,		*/
/* and adds it to the linked list "lnHead" in SCL_LD.		*/
typedef struct
  {
  DBL_LNK l;
  ST_CHAR varName[MAX_IDENT_LEN+1];	/* variable name (constructed)	*/
  ST_CHAR desc[MAX_IDENT_LEN+1];	/* description (optional)*/
					/* TRUNCATED if longer than buffer*/
  ST_CHAR lnType[MAX_IDENT_LEN+1];	/* LN Type name		*/
  ST_CHAR lnClass[MAX_IDENT_LEN+1];	/* LN Class name	*/
					/* for LN0, must be "LLN0"	*/
  ST_CHAR inst[MAX_IDENT_LEN+1];	/* LN inst name			*/
					/* for LN0, must be "" (empty string)*/
  ST_CHAR prefix[MAX_IDENT_LEN+1];	/* LN prefix name	*/
					/* for LNO, ignored	*/
  SCL_DAI     *daiHead;		/* head of list of DAI	*/
  SCL_DATASET *datasetHead;	/* head of list of DataSet	*/
  SCL_RCB     *rcbHead;		/* head of list of RCB (Report Control)	*/
  SCL_LCB     *lcbHead;		/* head of list of LCB (Log Control)	*/
  SCL_GCB     *gcbHead;		/* head of list of GCB (GOOSE Control)	*/
  /* NOTE: In LN or LN0: Inputs ignored		*/
  /* NOTE: In LN0: SampledValueControl, SettingControl, SCLControl ignored*/

  /* Added By djf 2007-08-16 */	
  SCL_SGCB     *sgcbPtr;  /* the ptr of SGCB  only for LLN0 */
  /* added djf 20080617 */
  SCL_DOI      *doiHead;  /* 保存doi中的desc */

  MVL_VAR_ASSOC *mvl_var_assoc;	/* MVL Variable Association created from LN info*/
  } SCL_LN;			/* Logical Node (LN or LN0 in SCL)	*/

/* "scl_ld_create" allocates this struct			*/
/* and adds it to the linked list "ldHead" in SCL_INFO.		*/
typedef struct
  {
  DBL_LNK l;
  ST_CHAR domName[MAX_IDENT_LEN+1];	/* domain name (constructed)	*/
  ST_CHAR desc[MAX_IDENT_LEN+1];	/* description (optional)*/
					/* TRUNCATED if longer than buffer*/
  ST_CHAR inst[MAX_IDENT_LEN+1];	/* LD inst name		*/
  SCL_LN *lnHead;			/* head of list of LN	*/
  /* NOTE: AccessControl in LDevice is ignored	*/
  } SCL_LD;			/* Logical Device (LDevice in SCL)*/

/************************************************************************/
/* Structures to contain information from "DataTypeTemplates" section of SCL.*/
/************************************************************************/
/* This structure should be allocated and filled in by the function	*/
/* "scl_lntype_add_do".							*/
typedef struct scl_do
  {
  /* CRITICAL: First 2 parameters used to add this struct to linked	*/
  /* lists using list_add_last, etc.					*/
  struct scl_do *next;			/* CRITICAL: DON'T MOVE.	*/
  struct scl_do *prev;			/* CRITICAL: DON'T MOVE.	*/
  ST_CHAR name[MAX_IDENT_LEN+1];	/* data object name		*/
  ST_CHAR desc[MAX_IDENT_LEN+1];    //描述
  ST_CHAR type[MAX_IDENT_LEN+1];	/* data object type		*/
  } SCL_DO;

typedef struct scl_lntype
  {
  /* CRITICAL: First 2 parameters used to add this struct to linked	*/
  /* lists using list_add_last, etc.					*/
  struct scl_lntype *next;		/* CRITICAL: DON'T MOVE.	*/
  struct scl_lntype *prev;		/* CRITICAL: DON'T MOVE.	*/
  ST_CHAR id[MAX_IDENT_LEN+1];			/* name used to reference this LN Type*/
  ST_CHAR lnClass[MAX_IDENT_LEN+1];		/* logical node class		*/
  SCL_DO *doHead;		/* head of list of DO	*/
				/* scl_lntype_add_do adds to list	*/
  } SCL_LNTYPE;


/* This structure should be allocated and filled in by the function	*/
/* "scl_dotype_add_da" OR "scl_dotype_add_sdo", and possibly modified by the optional	*/
/* function "scl_dotype_add_da_val".					*/
/* NOTE: the same structure must be used for DA or SDO because each must	*/
/* be put on the same linked list in the order they are read from the SCL file.*/
/* Most of the parameters are relevant only for DA elements. They are	*/
/* ignored if this is an SDO (i.e. objtype=SCL_OBJTYPE_SDO).		*/
typedef struct
  {
  DBL_LNK l;
  ST_CHAR name[MAX_IDENT_LEN+1];	/* DA or SDO name		*/
  ST_INT objtype;			/* SCL_OBJTYPE_DA or SCL_OBJTYPE_SDO	*/
  ST_CHAR desc[MAX_IDENT_LEN+1];	/* DA or SDO description (optional)*/
					/* TRUNCATED if longer than buffer	*/
  ST_CHAR sAddr[MAX_IDENT_LEN+1];	/* for DA only: DA sAddr	*/
  ST_CHAR bType[MAX_IDENT_LEN+1];	/* for DA only: DA bType	*/
  ST_CHAR valKind[MAX_VALKIND_LEN+1];	/* for DA only: Spec, Conf, RO, or Set	*/
  ST_CHAR type[MAX_IDENT_LEN+1];	/* for DA: needed if bType="Struct" or "Enum"*/
					/* for SDO: required		*/
  ST_UINT count;			/* for DA only: num array entries*/
  ST_CHAR fc[MAX_FC_LEN+1];		/* for DA only: functional constraint	*/
  ST_BOOLEAN dchg;			/* for DA only: TrgOp (data change)	*/
  ST_BOOLEAN qchg;			/* for DA only: TrgOp (quality change)	*/
  ST_BOOLEAN dupd;			/* for DA only: TrgOp (data update)	*/

  /* The "Val" and "sGroup" parameters are only set if the SCL file contains the
   * optional "Val" element, in which case "scl_dotype_add_da_val" is called.
   */
  ST_CHAR *Val;				/* for DA only: attribute value text	*/
					/* allocate appropriate size buffer*/
  ST_UINT sGroup;			/* for DA only: optional Setting Group Number*/
  } SCL_DA;

typedef struct scl_dotype
  {
  /* CRITICAL: First 2 parameters used to add this struct to linked	*/
  /* lists using list_add_last, etc.					*/
  struct scl_dotype *next;		/* CRITICAL: DON'T MOVE.	*/
  struct scl_dotype *prev;		/* CRITICAL: DON'T MOVE.	*/
  ST_CHAR id[MAX_IDENT_LEN+1];	/* name used to reference this DO Type	*/
  ST_CHAR cdc[MAX_CDC_LEN+1];	/* CDC name				*/
  SCL_DA *daHead;		/* head of list of DA or SDO		*/
				/* scl_dotype_add_da OR			*/
				/* scl_dotype_add_sdo adds to list	*/
  } SCL_DOTYPE;

/* This structure should be allocated and filled in by the function	*/
/* "scl_datype_add_bda".						*/
typedef struct
  {
  DBL_LNK l;
  ST_CHAR name[MAX_IDENT_LEN+1];		/* data attribute name		*/
  ST_CHAR desc[MAX_IDENT_LEN+1];		/* DA or SDO description (optional)*/
					/* TRUNCATED if longer than buffer	*/
  ST_CHAR sAddr[MAX_IDENT_LEN+1];		/* for DA only: DA sAddr	*/
  ST_CHAR bType[MAX_IDENT_LEN+1];		/* data attribute type		*/
  ST_CHAR valKind[MAX_VALKIND_LEN+1];		/* Spec, Conf, RO, or Set	*/
  ST_CHAR type[MAX_IDENT_LEN+1];		/* only used if btype="Struct" or "Enum"*/
  ST_UINT count;	/* for DA only: num array entries*/

  /* The "Val" and "sGroup" parameters are only set if the SCL file contains the
   * optional "Val" element, in which case "scl_datype_add_bda_val" is called.
   */
  ST_CHAR *Val;				/* attribute value text		*/
					/* allocate appropriate size buffer*/
  ST_UINT sGroup;			/* optional Setting Group Number*/
  } SCL_BDA;			/* Basic Data Attribute		*/
typedef struct
  {
  DBL_LNK l;
  ST_CHAR id[MAX_IDENT_LEN+1];			/* name used to reference this DA Type*/
  SCL_BDA *bdaHead;		/* head of list of BDA	*/
				/* scl_datype_add_bda adds to list	*/
  } SCL_DATYPE;


/* This structure should be allocated and filled in by the function	*/
/* "scl_enumtype_add_enumval".						*/
typedef struct
  {
  DBL_LNK l;
  ST_INT ord;				/* ord attribute	*/
  ST_CHAR EnumVal[MAX_IDENT_LEN+1];	/* EnumVal element		*/
					/* TRUNCATED if longer than buffer	*/
  } SCL_ENUMVAL;
typedef struct
  {
  DBL_LNK l;
  ST_CHAR id[MAX_IDENT_LEN+1];			/* name used to reference this DA Type*/
  SCL_ENUMVAL *enumvalHead;	/* head of list of EnumVal	*/
				/* scl_enumtype_add_enumval adds to list*/
  } SCL_ENUMTYPE;

/************************************************************************/
/*			SCL_INFO					*/
/* This structure contains all information extracted from the SCL file	*/
/* to be used for MMS-EASE Lite initialization.				*/
/************************************************************************/
typedef struct
  {
  ST_CHAR iedName [MAX_IDENT_LEN+1];	/* passed to scl_parse		*/

  SCL_HEADER Header;		/* Info from "Header" section of SCL file*/

  /* Logical Node Type definitions (from DataTypeTemplates section)	*/
  SCL_LNTYPE *lnTypeHead;	/* head of list	of LNodeType defs	*/
  SCL_DOTYPE *doTypeHead;	/* head of list of DOType defs		*/
  SCL_DATYPE *daTypeHead;	/* head of list of DAType defs		*/
  SCL_ENUMTYPE *enumTypeHead;	/* head of list of EnumType defs	*/

  /* Logical Device (MMS Domain) definitions (from AccessPoint section)	*/
  SCL_LD *ldHead;		/* head of list of LDevice defs		*/

  ST_INT brcb_bufsize;		/* BRCB buffer size			*/
				/* scl2_ld_create_all uses arg to set it*/
  } SCL_INFO;


/* Communication 节点部分结构定义*/

/*   P   */
typedef struct
{
	DBL_LNK l;
	ST_CHAR *type;	/* type (required)*/
	ST_CHAR *p;		/* address */
} SCL_P;

/*   Address   */
typedef struct
{
	SCL_P *P_head;	/* Head of P */
} SCL_ADDR;

/*   GSE   */
typedef struct
{
	DBL_LNK l;
	ST_CHAR *ld_inst;		/* LD inst		requred*/
	ST_CHAR *cb_name;		/* control block name		requred*/
	ST_CHAR *desc;			/* description (optional)*/
	/* option  sub node */
	SCL_ADDR *pAddress;		/* Address Node */
	ST_CHAR *min_time_unit;	/* MinTime (requred) fix:s*/
	ST_CHAR *min_time_multiplier;	/* MinTime (optional) fix:m*/
	ST_UINT min_time;		/* MinTime value*/
	ST_CHAR *max_time_unit;	/* MaxTime (requred) fix:s*/
	ST_CHAR *max_time_multiplier;	/* MaxTime (optional) fix:m*/
	ST_UINT max_time;		/* MaxTime value*/
} SCL_GSE;

/*   SMV   */
typedef struct
{
	DBL_LNK l;
	ST_CHAR *ld_inst;	/* LD inst		requred*/
	ST_CHAR *cb_name;	/* control block name		requred*/
	ST_CHAR *desc;		/* description (optional)*/
	/* option  sub node */
	SCL_ADDR *pAddress;	/* Address Node */
} SCL_SMV;

/*   PhysConn   */
typedef struct
{
	DBL_LNK l;
	ST_CHAR *type;	/* type (requred)*/
	SCL_P *P_head;	/* Head of P */
} SCL_PHYS;

/*   connectedAP   */
typedef struct
{
	DBL_LNK l;
	ST_CHAR *ied_name;	/* IED name		requred*/
	ST_CHAR *desc;		/* description (optional)*/
	ST_CHAR *connectedAP_name;	/* connectedAP name		requred*/
	/* option  sub node */
	SCL_ADDR *pAddress;	/* Address Node */
	SCL_GSE  *GSE_head;	/* Head of GSE */
	SCL_SMV  *SMV_head;	/* Head of SMV */
	SCL_PHYS *PhysConn_head;	/* Head of PhysConn */
} SCL_NETAP;

/*   SubNetwork   */
typedef struct
{
	DBL_LNK l;
	ST_CHAR *name;	/* SubNetwork name		*/
	ST_CHAR *desc;	/* description (optional)*/
	ST_CHAR *type;	/* type (optional)*/
	/* option  sub node BitRate*/
	ST_CHAR *unit;	/* unit (if BitRate then requred) Fix:b/s */
	ST_CHAR *multiplier;	/* multiplier (optional) Fix:M */
	ST_UINT bitrate;		/* BitRate */
	SCL_NETAP *netap_head;		/* Head of connectedAP */
} SCL_SUBNET;

/**
存储scd文件中所有节点信息的数据结构
*/
typedef struct 
{
	SCL_SUBNET *subnet_head;	 /**<< SubNetwork节点链表*/
    SCL_INFO *ieds;  /**<< 所有ied设备节点数组*/
	ST_INT   ied_num;  /**<< ied设备的个数*/
	SCL_INFO *data_type;  /**<< scd文件数据模版节点*/
}SCD_INFO;

/************************************************************************/
/************************************************************************/
/* FUNCTIONS to store SCL info in "SCL_INFO" structure.			*/
/************************************************************************/
/************************************************************************/

/************************************************************************/
/*added by djf 2007/08/16				*/
/* scl_sgcb_add															*/
/* Add a Setting Group Control Block (SGCB).							*/
/* Allocates a SCL_SGCB struct, fills it in,							*/
/* and evaluate it to the pointer "sgcbPtr" in SCL_LN.					*/
/************************************************************************/
ST_RET scl_sgcb_add(SCL_INFO *scl_info,	/* main struct where all SCL info stored*/
					ST_CHAR *desc,		/* SGCB description (optional)		*/
					/* TRUNCATED if longer than buffer	*/
					ST_UINT numOfSG,
					ST_UINT actSG);


/************************************************************************/
/*			scl_header_save					*/
/* Save "Header" information from SCL file.				*/
/* RETURNS:	SD_SUCCESS or error code				*/
/************************************************************************/
ST_RET scl_header_save (SCL_INFO *sclInfo, ST_CHAR *id, ST_CHAR *nameStructure);

/************************************************************************/
/*			scl_lntype_create				*/
/* Begin creation of a Logical Node Type (LNodeType).			*/
/* RETURNS:	SD_SUCCESS or error code				*/
/************************************************************************/
ST_RET scl_lntype_create (
	SCL_INFO *sclInfo,	/* main struct where all SCL info stored*/
	ST_CHAR *id,		/* name used to reference this LN Type*/
	ST_CHAR *lnClass);	/* logical node class		*/

/************************************************************************/
/*			scl_lntype_add_do				*/
/* Add a Data Object (DO) to a Logical Node Type (LNodeType).		*/
/* Adds to lntype created by most recent call to "scl_lntype_create".	*/
/* RETURNS:	SD_SUCCESS or error code				*/
/************************************************************************/
ST_RET scl_lntype_add_do (
	SCL_INFO *sclInfo,	/* main struct where all SCL info stored*/
	ST_CHAR *name,		/* data object name		*/
	ST_CHAR *type,		/* data object type		*/
	ST_CHAR *desc);		//描述

/************************************************************************/
/*			scl_dotype_create				*/
/* Begin creation of a Data Object Type (DOType).			*/
/* RETURNS:	SD_SUCCESS or error code				*/
/************************************************************************/
ST_RET scl_dotype_create (
	SCL_INFO *sclInfo,	/* main struct where all SCL info stored*/
	ST_CHAR *id,		/* name used to reference this DO Type*/
	ST_CHAR *cdc);		/* CDC name		*/

/************************************************************************/
/*			scl_dotype_add_da				*/
/* Add a Data Attribute (DA) to a Data Object Type (DOType).		*/
/* RETURNS:	SD_SUCCESS or error code				*/
/************************************************************************/
ST_RET scl_dotype_add_da (
	SCL_INFO *sclInfo,	/* main struct where all SCL info stored*/
	ST_CHAR *name,		/* DA name		*/
	ST_CHAR *desc,		/* DA description (optional)*/
	ST_CHAR *sAddr,		/* DA sAddr	*/
	ST_CHAR *bType,		/* DA bType	*/
	ST_CHAR *valKind,	/* Spec, Conf, RO, or Set	*/
	ST_CHAR *type,		/* needed if bType="Struct" or "Enum"*/
	ST_UINT count,		/* num array entries*/
	ST_CHAR *fc,		/* functional constraint	*/
	ST_BOOLEAN dchg,	/* TrgOp (data change)	*/
	ST_BOOLEAN qchg,	/* TrgOp (quality change)	*/
	ST_BOOLEAN dupd);	/* TrgOp (data update)	*/

/************************************************************************/
/*			scl_dotype_add_sdo				*/
/* Add a (SDO) to a Data Object Type (DOType).				*/
/* RETURNS:	SD_SUCCESS or error code				*/
/************************************************************************/
ST_RET scl_dotype_add_sdo (
	SCL_INFO *sclInfo,	/* main struct where all SCL info stored*/
	ST_CHAR *name,		/* DA or SDO name		*/
	ST_CHAR *desc,		/* DA or SDO description (optional)*/
	ST_CHAR *type);		/* SDO type (ref to lower DOType)*/

/************************************************************************/
/*			scl_dotype_add_da_val				*/
/* Add a Val element to a DA which is being added to a DOType.		*/
/* Add to DA created by most recent call to scl_dotype_add_da.		*/
/* RETURNS:	SD_SUCCESS or error code				*/
/************************************************************************/
ST_RET scl_dotype_add_da_val (
	SCL_INFO *sclInfo,	/* main struct where all SCL info stored*/
	ST_CHAR *Val,		/* Val to add			*/
	ST_UINT sGroup);	/* optional Setting Group Number*/

/************************************************************************/
/*			scl_datype_create				*/
/* Begin creation of a Data Attribute Type (DAType).			*/
/* RETURNS:	SD_SUCCESS or error code				*/
/************************************************************************/
ST_RET scl_datype_create (
	SCL_INFO *sclInfo,	/* main struct where all SCL info stored*/
	ST_CHAR *id);		/* name used to reference this DA Type*/
/************************************************************************/
/*			scl_datype_add_bda				*/
/* Add a Basic Data Attribute (BDA) to a Data Attribute Type (DAType).	*/
/* RETURNS:	SD_SUCCESS or error code				*/
/************************************************************************/
ST_RET scl_datype_add_bda (
	SCL_INFO *sclInfo,	/* main struct where all SCL info stored*/
	ST_CHAR *name,		/* DA name		*/
	ST_CHAR *desc,		/* DA description (optional)*/
	ST_CHAR *sAddr,
	ST_CHAR *bType,		/* data attribute type		*/
	ST_CHAR *valKind,	/* Spec, Conf, RO, or Set	*/
	ST_CHAR *type,		/* only used if bType="Struct" or "Enum"*/
	ST_UINT count);	/* num array entries	*/

/************************************************************************/
/*			scl_datype_add_bda_val				*/
/* Add a Val element to a BDA which is being added to a DAType.		*/
/* Add to BDA created by most recent call to scl_datype_add_bda.	*/
/* RETURNS:	SD_SUCCESS or error code				*/
/************************************************************************/
ST_RET scl_datype_add_bda_val (
	SCL_INFO *sclInfo,	/* main struct where all SCL info stored*/
	ST_CHAR *Val,		/* Val to add			*/
	ST_UINT sGroup);	/* optional Setting Group Number*/

/************************************************************************/
/*			scl_enumtype_create				*/
/* Begin creation of an EnumType.					*/
/* RETURNS:	SD_SUCCESS or error code				*/
/************************************************************************/
ST_RET scl_enumtype_create (
	SCL_INFO *sclInfo,	/* main struct where all SCL info stored*/
	ST_CHAR *id);		/* name used to reference this EnumType*/

/************************************************************************/
/*			scl_enumtype_add_enumval			*/
/* Add an EnumVal to an EnumType.					*/
/* Add to EnumType created by most recent call to scl_enumtype_create.	*/
/* RETURNS:	SD_SUCCESS or error code				*/
/************************************************************************/
ST_RET scl_enumtype_add_enumval (
	SCL_INFO *sclInfo,	/* main struct where all SCL info stored*/
		ST_INT ord,		/* ord attribute		*/
		ST_CHAR *EnumVal);	/* EnumVal element		*/

/************************************************************************/
/*			scl_fcda_add				*/
/* Allocates a SCL_FCDA struct, fills it in,				*/
/* and adds it to the linked list "fcdaHead" in SCL_DATASET.		*/
/************************************************************************/
ST_RET scl_fcda_add (
	SCL_INFO *sclInfo,	/* main struct where all SCL info stored*/
	ST_CHAR *ldInst,
	ST_CHAR *prefix,
	ST_CHAR *lnInst,
	ST_CHAR *lnClass,
	ST_CHAR *doName,
	ST_CHAR *daName,
	ST_CHAR *fc);

/************************************************************************/
/*			scl_dai_add					*/
/* Allocates a SCL_DAI struct, fills it in,				*/
/* and adds it to the linked list "daiHead" in SCL_LN.			*/
/************************************************************************/
ST_RET scl_dai_add (
	SCL_INFO *sclInfo,	/* main struct where all SCL info stored*/
	ST_CHAR *flattened,	/* flattened attribute name		*/
				/* constructed from "name" & "ix"	*/
				/* from DOI, SDI, & DAI			*/
	ST_CHAR *Val,		/* attribute value text from DAI	*/
				/* allocate appropriate size buffer	*/
	ST_CHAR *sAddr,		/* from DAI				*/
	ST_CHAR *valKind, /* from DAI				*/
	ST_CHAR *desc);	/* from DAI				*/

/************************************************************************/
/*			scl_dataset_add				*/
/* Allocates a SCL_DATASET struct, fills it in,				*/
/* and adds it to the linked list "datasetHead" in SCL_LN.		*/
/************************************************************************/
ST_RET scl_dataset_add (
	SCL_INFO *sclInfo,	/* main struct where all SCL info stored*/
	ST_CHAR *name,		/* DataSet name				*/
	ST_CHAR *desc);		/* DataSet description (optional)	*/
				/* TRUNCATED if longer than buffer	*/

/************************************************************************/
/*			scl_rcb_add					*/
/* Allocates a SCL_RCB struct, fills it in,				*/
/* and adds it to the linked list "rcbHead" in SCL_LN.			*/
/* NOTE: the RptEnabled element of the SCL file is ignored. SISCO software*/
/*	does not need to know which clients may access the RCB.		*/
/************************************************************************/
ST_RET scl_rcb_add (
	SCL_INFO *sclInfo,	/* main struct where all SCL info stored*/
	ST_CHAR *name,		/* RCB name				*/
	ST_CHAR *desc,		/* RCB description (optional)		*/
				/* TRUNCATED if longer than buffer	*/
	ST_CHAR *datSet,
	ST_UINT intgPd,
	ST_CHAR *rptID,
	ST_UINT confRev,
	ST_BOOLEAN buffered,	/* TRUE if this is buffered RCB		*/
	ST_UINT bufTime,
	ST_UINT8 *TrgOps,	/* 8-bit bitstring			*/
				/* Pointer to 1-byte array		*/
				/* Boolean vals from SCL file		*/
				/* (dchg, qchg, dupd, & period)		*/
				/* used to set bits in TrgOps bitstring	*/
	ST_UINT8 *OptFlds,	/* 9-bit bitstring			*/
				/* Pointer to 2-byte array		*/
				/* Boolean vals from SCL file		*/
				/* (seqNum, timeStamp, dataSet,		*/
				/* reasonCode, dataRef, bufOvfl,	*/
				/* entryID, configRef)			*/
				/* segmentation boolean is ignored	*/
				/* used to set bits in OptFlds bitstring*/
	ST_INT max_num);//实例化最大个数
/************************************************************************/
/*			scl_lcb_add					*/
/* Add a LOG Control Block (LCB).					*/
/* Allocates a SCL_LCB struct, fills it in,				*/
/* and adds it to the linked list "lcbHead" in SCL_LN.			*/
/************************************************************************/
ST_RET scl_lcb_add (
	SCL_INFO *scl_info,	/* main struct where all SCL info stored*/
	ST_CHAR *name,		/* LCB name				*/
	ST_CHAR *desc,		/* LCB description (optional)		*/
				/* TRUNCATED if longer than buffer	*/
	ST_CHAR *datSet,
	ST_UINT intgPd,
	ST_CHAR *logName,
	ST_BOOLEAN logEna,
	ST_BOOLEAN reasonCode,
	ST_UINT8 *TrgOps);	/* 8-bit bitstring			*/
				/* Pointer to 1-byte array		*/
				/* Boolean vals from SCL file		*/
				/* (dchg, qchg, dupd, & period)		*/
				/* used to set bits in TrgOps bitstring	*/

/************************************************************************/
/*			scl_gcb_add					*/
/* Add a GOOSE Control Block (GCB).					*/
/* Allocates a SCL_GCB struct, fills it in,				*/
/* and adds it to the linked list "gcbHead" in SCL_LN.			*/
/* NOTE: The SCL file may also contain one or more "IEDName" elements to*/
/*       indicate IEDs that should subscribe for GOOSE data. We have no	*/
/*       way to use this information, so it is ignored.			*/
/************************************************************************/
ST_RET scl_gcb_add (
	SCL_INFO *scl_info,	/* main struct where all SCL info stored*/
	ST_CHAR *name,		/* GCB name				*/
	ST_CHAR *desc,		/* GCB description (optional)		*/
				/* TRUNCATED if longer than buffer	*/
	ST_CHAR *datSet,
	ST_UINT confRev,
	ST_BOOLEAN isGoose,	/* SD_TRUE if "GOOSE", SD_FALSE if "GSSE"*/
	ST_CHAR *appID);

/************************************************************************/
/*			scl_ln_add					*/
/* Allocates a SCL_LN struct, fills it in,				*/
/* and adds it to the linked list "lnHead" in SCL_LD.			*/
/************************************************************************/
ST_RET scl_ln_add (
	SCL_INFO *sclInfo,	/* main struct where all SCL info stored*/
	ST_CHAR *desc,		/* LN description (optional)		*/
				/* TRUNCATED if longer than buffer	*/
	ST_CHAR *lnType,	/* LN lnType				*/
	ST_CHAR *lnClass,	/* LN lnClass				*/
	ST_CHAR *inst,		/* LN inst				*/
	ST_CHAR *prefix);	/* LN prefix				*/

/************************************************************************/
/*			scl_parse					*/
/* Parses SCL file and stores extracted info in SCL_INFO structure.	*/
/************************************************************************/
ST_RET scl_parse (ST_CHAR *xmlFileName, ST_CHAR *iedName, 
		  ST_CHAR *accessPointName, SCL_INFO *sclInfo);
ST_VOID scl_log_all (SCL_INFO *scl_info);

/* Add by tyj 2007/07/11  	*/
/*			scd文件解析		*/
ST_RET ai_parse_scd(ST_CHAR *scd_name, SCD_INFO *scd_info);

/************************************************************************/
/*			scl_info_destroy				*/
/* Destroy all info stored in the SCL_INFO structure by "scl_parse".	*/
/************************************************************************/
ST_VOID scl_info_destroy (SCL_INFO *scl_info);

/* Add by tyj 2007/07/11  	*/
/*		释放scd结构空间	    */
ST_VOID ai_cleanup_scd_info(SCD_INFO *scd_info);

/************************************************************************/
/*			scl_ld_create					*/
/* Allocates SCL_LD struct						*/
/* and adds it to the linked list "ldHead" in SCL_INFO.			*/
/************************************************************************/
ST_RET scl_ld_create (
	SCL_INFO *sclInfo,	/* main struct where all SCL info stored*/
	ST_CHAR *desc,		/* LD description (optional)		*/
				/* TRUNCATED if longer than buffer	*/
	ST_CHAR *inst);		/* LD inst				*/

/************************************************************************/
/*			scl2_datatype_create_all			*/
/* Create MMS Data types for all Logical Node Types (LNodeType)		*/
/* defined in SCL.							*/
/* This function needs a user buffer for constructing TDL strings.	*/
/* RETURNS:	SD_SUCCESS or error code				*/
/************************************************************************/
ST_RET scl2_datatype_create_all (
	SCL_INFO *sclInfo,	/* main struct where all SCL info stored*/
	ST_CHAR *tdlbuf,	/* buffer to use for constructing TDL	*/
				/* reused for each LNType in list	*/
	size_t tdlbuflen);	/* len of tdlbuf buffer in bytes	*/

/************************************************************************/
/*			scl_create_type_id				*/
/*			Add by tyj 2007/07/12 				*/
/* Create MMS Data type for one Logical Node Type (LNodeType)		*/
/* defined in SCL.							*/
/* This function needs a user buffer for constructing TDL strings.	*/
/* TDL is generated by calling "tdladd_*" functions. A ptr to a		*/
/* TDLADD_CTXT structure is passed to each "tdladd_*" function.		*/
/* The structure is used to keep track of the current ptr in the TDL	*/
/* buffer and the space remaining in the buffer.			*/
/* RETURNS:	SD_SUCCESS or error code				*/
/************************************************************************/
ST_RET scl_create_type_id(
	SCL_INFO *scl_info,	/* main struct where all SCL info stored*/
	SCL_LNTYPE *scl_lntype,	/* info for this LNType			*/
	ST_CHAR *tdl_buf,	/* buffer to use for constructing TDL	*/
				/* reused for each LNType in list	*/
	size_t tdl_buf_len,	/* len of tdlbuf buffer in bytes	*/
	SCL_LD *scl_ld_special,	/* LD containing LN with "special stuff"*/
				/* (i.e. ReportControl, LogControl, etc)*/
				/* NULL if nothing special in LN.	*/
	SCL_LN *scl_ln_special);	/* LN with "special stuff"		*/
				/* NULL if nothing special in LN.	*/

/************************************************************************/
/*			scl2_ld_create_all			*/
/* Create all Logical Devices from SCL info saved in "sclInfo".		*/
/* RETURNS:	SD_SUCCESS or error code				*/
/************************************************************************/
ST_RET scl2_ld_create_all (
	SCL_INFO *sclInfo,	/* main struct where all SCL info stored*/
	ST_UINT reportScanRate,	/* report scan rate (millisec)	*/
	ST_INT brcb_bufsize);	/* BRCB buffer size			*/

/************************************************************************/
/*			u_set_all_leaf_functions			*/
/* User customizable function to set all leaf functions for a data type.*/
/************************************************************************/
ST_RET u_set_all_leaf_functions (RUNTIME_CTRL *rt_ctrl);
//add doi
ST_RET scl_doi_add(SCL_INFO *scl_info, ST_CHAR *do_name, ST_CHAR *desc);

#ifdef __cplusplus
}
#endif

#endif	/* !SCL_INCLUDED	*/
