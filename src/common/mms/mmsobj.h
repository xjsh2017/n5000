/*  This file created from input file '..\..\mvl\usr\scl_srvr\uca_obj.odf'
    Leaf Access Parameter (LAP) File: '..\..\mvl\usr\scl_srvr\leafmap.xml'
	Created Thu Jul 26 16:35:03 2007
*/

#ifndef FOUNDRY_OUTPUT_HEADER_INCLUDED
#define FOUNDRY_OUTPUT_HEADER_INCLUDED

#include "mvl_uca.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************/
/* MVL Type Control Information */
extern MVL_TYPE_CTRL *mvl_type_ctrl;
extern ST_INT mvl_num_types;

/************************************************************************/


/* MVL Type ID's */
#define BOOLEAN_TYPEID		0
#define INT8_TYPEID		1
#define INT16_TYPEID		2
#define INT32_TYPEID		3
#define INT128_TYPEID		4
#define INT8U_TYPEID		5
#define INT16U_TYPEID		6
#define INT32U_TYPEID		7
#define FLOAT32_TYPEID		8
#define FLOAT64_TYPEID		9
#define CODE_ENUM_TYPEID		10
#define OCTET_STRING8_TYPEID		11
#define OCTET_STRING64_TYPEID		12
#define VISIBLE_STRING255_TYPEID		13
#define VISIBLE_STRING64_TYPEID		14
#define VISIBLE_STRING65_TYPEID		15
#define UNICODE_STRING255_TYPEID		16
#define TimeStamp_TYPEID		17
#define EntryTime_TYPEID		18
#define Quality_TYPEID		19
#define EntryID_TYPEID		20
#define OPTFLDS_TYPEID		21
#define TRG_CONDITION_TYPEID		22
#define Check_TYPEID		23
#define RTYP_BOOL_TYPEID		24
#define RTYP_BTIME6_TYPEID		25
#define RTYP_BSTR6_TYPEID		26
#define RTYP_BSTR8_TYPEID		27
#define RTYP_BSTR9_TYPEID		28
#define RTYP_BVSTR6_TYPEID		29
#define RTYP_BVSTR8_TYPEID		30
#define RTYP_BVSTR10_TYPEID		31
#define RTYP_INT8U_TYPEID		32
#define RTYP_INT16U_TYPEID		33
#define RTYP_OSTR8_TYPEID		34
#define RTYP_VSTR32_TYPEID		35
#define RTYP_VSTR65_TYPEID		36
#define RTYP_INT32U_TYPEID		37


/************************************************************************/

/************************************************************************/


/*	Common Strings Index Defines	*/

#ifndef USE_RT_TYPE_2

#endif


/************************************************************************/
/* TYPEDEFS for MMS TYPES	                                              */
/************************************************************************/

/* Use ":CF", "-notypedefs" in the ODF file to not include this line */
#define USE_MMS_TYPEDEFS

#if defined (USE_MMS_TYPEDEFS)

typedef ST_BOOLEAN BOOLEAN_TDEF;

typedef ST_INT8 INT8_TDEF;

typedef ST_INT16 INT16_TDEF;

typedef ST_INT32 INT32_TDEF;

typedef ST_INT64 INT128_TDEF;

typedef ST_UINT8 INT8U_TDEF;

typedef ST_UINT16 INT16U_TDEF;

typedef ST_UINT32 INT32U_TDEF;

typedef ST_FLOAT FLOAT32_TDEF;

typedef ST_DOUBLE FLOAT64_TDEF;

typedef ST_UCHAR CODE_ENUM_TDEF[1];  /* Bitstring */

typedef struct     /* OVstring */
  {
  ST_INT16 len_1;
  ST_UCHAR data_1[8];
  }  OCTET_STRING8_TDEF;  /* Octetstring */

typedef struct     /* OVstring */
  {
  ST_INT16 len_1;
  ST_UCHAR data_1[64];
  }  OCTET_STRING64_TDEF;  /* Octetstring */

typedef ST_CHAR VISIBLE_STRING255_TDEF[256];  /* Visible String */

typedef ST_CHAR VISIBLE_STRING64_TDEF[65];  /* Visible String */

typedef ST_CHAR VISIBLE_STRING65_TDEF[66];  /* Visible String */

typedef ST_CHAR VISIBLE_STRING129_TDEF[130];  /* Visible String */

typedef ST_CHAR UNICODE_STRING255_TDEF[1021];

typedef struct
  {
  ST_UINT32 secs;  /* UTC Time is struct of three ST_UINT32 */
  ST_UINT32 fraction;
  ST_UINT32 qflags;
  } TimeStamp_TDEF;

typedef struct
  {
  ST_INT32 btime6_ms;  /* Btime6 is two ST_INT32 */
  ST_INT32 btime6_days;
  } EntryTime_TDEF;

typedef struct     /* BVstring */
  {
  ST_INT16 len_1;
  ST_UCHAR data_1[2];
  }  Quality_TDEF;  /* Bitstring */

typedef ST_UCHAR EntryID_TDEF[8];  /* Octetstring */

typedef struct     /* BVstring */
  {
  ST_INT16 len_1;
  ST_UCHAR data_1[2];
  }  OPTFLDS_TDEF;  /* Bitstring */

typedef struct     /* BVstring */
  {
  ST_INT16 len_1;
  ST_UCHAR data_1[1];
  }  TRG_CONDITION_TDEF;  /* Bitstring */

typedef struct     /* BVstring */
  {
  ST_INT16 len_1;
  ST_UCHAR data_1[1];
  }  Check_TDEF;  /* Bitstring */

typedef ST_BOOLEAN RTYP_BOOL_TDEF;

typedef struct
  {
  ST_INT32 btime6_ms;  /* Btime6 is two ST_INT32 */
  ST_INT32 btime6_days;
  } RTYP_BTIME6_TDEF;

typedef ST_UCHAR RTYP_BSTR6_TDEF[1];  /* Bitstring */

typedef ST_UCHAR RTYP_BSTR8_TDEF[1];  /* Bitstring */

typedef ST_UCHAR RTYP_BSTR9_TDEF[2];  /* Bitstring */

typedef struct     /* BVstring */
  {
  ST_INT16 len_1;
  ST_UCHAR data_1[1];
  }  RTYP_BVSTR6_TDEF;  /* Bitstring */

typedef struct     /* BVstring */
  {
  ST_INT16 len_1;
  ST_UCHAR data_1[1];
  }  RTYP_BVSTR8_TDEF;  /* Bitstring */

typedef struct     /* BVstring */
  {
  ST_INT16 len_1;
  ST_UCHAR data_1[2];
  }  RTYP_BVSTR10_TDEF;  /* Bitstring */

typedef ST_UINT8 RTYP_INT8U_TDEF;

typedef ST_UINT16 RTYP_INT16U_TDEF;

typedef ST_UCHAR RTYP_OSTR8_TDEF[8];  /* Octetstring */

typedef ST_CHAR RTYP_VSTR32_TDEF[33];  /* Visible String */

typedef ST_CHAR RTYP_VSTR65_TDEF[66];  /* Visible String */

typedef ST_UINT32 RTYP_INT32U_TDEF;

/************************************************************************/
#endif /* if defined (USE_MMS_TYPEDEFS) */
/************************************************************************/

/************************************************************************/
/* UCA Runtime Type Function Indexes                                    */
/* These are generated by Foundry and should not be changed             */

#define U_CUSTOM_RD_IND_INDEX	0
#define U_CTL_SBO_RD_IND_INDEX	1
#define MVL61850_BEH_STVAL_RD_IND_INDEX	2
#define MVLU_RPTENA_RD_IND_FUN_INDEX	3
#define MVLU_RPTID_RD_IND_FUN_INDEX	4
#define MVLU_DATSETNA_RD_IND_FUN_INDEX	5
#define MVLU_CONFREV_RD_IND_INDEX	6
#define MVLU_OPTFLDS_RD_IND_FUN_INDEX	7
#define MVLU_BUFTIM_RD_IND_FUN_INDEX	8
#define MVLU_SQNUM_RD_IND_FUN_INDEX	9
#define MVLU_SQNUM_INT16U_RD_IND_FUN_INDEX	10
#define MVLU_TRGOPS_RD_IND_FUN_INDEX	11
#define MVLU_INTGPD_RD_IND_FUN_INDEX	12
#define MVLU_GI_RD_IND_INDEX	13
#define MVLU_PURGEBUF_RD_IND_INDEX	14
#define MVLU_ENTRYID_RD_IND_INDEX	15
#define MVLU_TIMEOFENTRY_RD_IND_INDEX	16
#define MVLU_RESV_RD_IND_FUN_INDEX	17


#define U_CUSTOM_WR_IND_INDEX	0
#define U_NO_WRITE_ALLOWED_INDEX	1
#define U_CTL_SBOW_COMP_WR_IND_INDEX	2
#define U_CTL_OPER_CTLVAL_WR_IND_INDEX	3
#define U_CTL_OPER_OTHER_WR_IND_INDEX	4
#define U_CANCEL_COMP_WR_IND_INDEX	5
#define MVLU_RPTENA_WR_IND_FUN_INDEX	6
#define MVLU_RPTID_WR_IND_FUN_INDEX	7
#define MVLU_OPTFLDS_WR_IND_FUN_INDEX	8
#define MVLU_BUFTIM_WR_IND_FUN_INDEX	9
#define MVLU_TRGOPS_WR_IND_FUN_INDEX	10
#define MVLU_INTGPD_WR_IND_FUN_INDEX	11
#define MVLU_GI_WR_IND_INDEX	12
#define MVLU_PURGEBUF_WR_IND_INDEX	13
#define MVLU_ENTRYID_WR_IND_INDEX	14
#define MVLU_RESV_WR_IND_FUN_INDEX	15

/************************************************************************/
/* UCA Read/Write Indication function external declarations */
/* These functions must be provided externally              */

ST_VOID u_custom_rd_ind (MVLU_RD_VA_CTRL *mvluRdVaCtrl);
ST_VOID u_ctl_sbo_rd_ind (MVLU_RD_VA_CTRL *mvluRdVaCtrl);
ST_VOID mvl61850_beh_stval_rd_ind (MVLU_RD_VA_CTRL *mvluRdVaCtrl);
ST_VOID mvlu_rptena_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl);
ST_VOID mvlu_rptid_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl);
ST_VOID mvlu_datsetna_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl);
ST_VOID mvlu_confrev_rd_ind (MVLU_RD_VA_CTRL *mvluRdVaCtrl);
ST_VOID mvlu_optflds_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl);
ST_VOID mvlu_buftim_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl);
ST_VOID mvlu_sqnum_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl);
ST_VOID mvlu_sqnum_int16u_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl);
ST_VOID mvlu_trgops_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl);
ST_VOID mvlu_intgpd_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl);
ST_VOID mvlu_gi_rd_ind (MVLU_RD_VA_CTRL *mvluRdVaCtrl);
ST_VOID mvlu_purgebuf_rd_ind (MVLU_RD_VA_CTRL *mvluRdVaCtrl);
ST_VOID mvlu_entryid_rd_ind (MVLU_RD_VA_CTRL *mvluRdVaCtrl);
ST_VOID mvlu_timeofentry_rd_ind (MVLU_RD_VA_CTRL *mvluRdVaCtrl);
ST_VOID mvlu_resv_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl);

ST_VOID u_custom_wr_ind (MVLU_WR_VA_CTRL *mvluWrVaCtrl);
ST_VOID u_no_write_allowed (MVLU_WR_VA_CTRL *mvluWrVaCtrl);
ST_VOID u_ctl_sbow_comp_wr_ind (MVLU_WR_VA_CTRL *mvluWrVaCtrl);
ST_VOID u_ctl_oper_ctlval_wr_ind (MVLU_WR_VA_CTRL *mvluWrVaCtrl);
ST_VOID u_ctl_oper_other_wr_ind (MVLU_WR_VA_CTRL *mvluWrVaCtrl);
ST_VOID u_cancel_comp_wr_ind (MVLU_WR_VA_CTRL *mvluWrVaCtrl);
ST_VOID mvlu_rptena_wr_ind_fun (MVLU_WR_VA_CTRL *mvluWrVaCtrl);
ST_VOID mvlu_rptid_wr_ind_fun (MVLU_WR_VA_CTRL *mvluWrVaCtrl);
ST_VOID mvlu_optflds_wr_ind_fun (MVLU_WR_VA_CTRL *mvluWrVaCtrl);
ST_VOID mvlu_buftim_wr_ind_fun (MVLU_WR_VA_CTRL *mvluWrVaCtrl);
ST_VOID mvlu_trgops_wr_ind_fun (MVLU_WR_VA_CTRL *mvluWrVaCtrl);
ST_VOID mvlu_intgpd_wr_ind_fun (MVLU_WR_VA_CTRL *mvluWrVaCtrl);
ST_VOID mvlu_gi_wr_ind (MVLU_WR_VA_CTRL *mvluWrVaCtrl);
ST_VOID mvlu_purgebuf_wr_ind (MVLU_WR_VA_CTRL *mvluWrVaCtrl);
ST_VOID mvlu_entryid_wr_ind (MVLU_WR_VA_CTRL *mvluWrVaCtrl);
ST_VOID mvlu_resv_wr_ind_fun (MVLU_WR_VA_CTRL *mvluWrVaCtrl);
ST_VOID mvl_uninit_mms_objs();

#ifdef __cplusplus
}
#endif

#endif /* #ifndef FOUNDRY_OUTPUT_HEADER_INCLUDED */
