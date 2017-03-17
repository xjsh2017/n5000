/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*	(c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*	1997, All Rights Reserved					*/
/*									*/
/*			PROPRIETARY AND CONFIDENTIAL			*/
/*									*/
/* MODULE NAME : copp_log.h						*/
/* PRODUCT(S)  : MOSI Stack (over TP4)					*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*		This header file contains logging macros for COPP.	*/
/*	Note:	To compile logging functions use the DEBUG_SISCO define	*/
/*		in the project make file.				*/
/*									*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 09/13/99  MDE    04     Added SD_CONST modifiers			*/
/* 10/08/98  MDE    03     Migrated to updated SLOG interface		*/
/* 05/27/97  JRB  7.00     MMSEASE 7.0 release.				*/
/* 03/19/97  JRB    02     General cleanup.				*/
/* 02/20/97  EJV    01     Created					*/
/************************************************************************/

#ifndef COPP_LOG_INCLUDED
#define COPP_LOG_INCLUDED

#include "slog.h"

#ifdef __cplusplus
extern "C" {
#endif

/* NOTE: COPP log masks and acse_debug_sel are defined in acse2usr.h */

#if defined(DEBUG_SISCO)

extern SD_CONST ST_CHAR *SD_CONST _copp_err_logstr;
extern SD_CONST ST_CHAR *SD_CONST _copp_dec_logstr;
extern SD_CONST ST_CHAR *SD_CONST _copp_dec_hex_logstr;
extern SD_CONST ST_CHAR *SD_CONST _copp_enc_logstr;
extern SD_CONST ST_CHAR *SD_CONST _copp_enc_hex_logstr;

  /*--------------------------------------------*/
  /* hard errors logging			*/
  /*--------------------------------------------*/

  #define COPP_LOG_ERR0(a) {\
	if (acse_debug_sel & COPP_LOG_ERR)\
	  _slog (sLogCtrl,_copp_err_logstr,thisFileName,__LINE__,a);\
	}
  #define COPP_LOG_ERR1(a,b) {\
	if (acse_debug_sel & COPP_LOG_ERR)\
	  _slog (sLogCtrl,_copp_err_logstr,thisFileName,__LINE__,a,b);\
	}
  #define COPP_LOG_ERR2(a,b,c) {\
	if (acse_debug_sel & COPP_LOG_ERR)\
	  _slog (sLogCtrl,_copp_err_logstr,thisFileName,__LINE__,a,b,c);\
	}
  #define COPP_LOG_ERR3(a,b,c,d) {\
	if (acse_debug_sel & COPP_LOG_ERR)\
	  _slog (sLogCtrl,_copp_err_logstr,thisFileName,__LINE__,a,b,c,d);\
	}

  /*--------------------------------------------*/
  /* PS-user req and rsp logging		*/
  /*--------------------------------------------*/

  #define COPP_LOG_ENC0(a) {\
	if (acse_debug_sel & COPP_LOG_ENC)\
	  _slog (sLogCtrl,_copp_enc_logstr,thisFileName,__LINE__,a);\
	}
  #define COPP_LOG_ENC1(a,b) {\
	if (acse_debug_sel & COPP_LOG_ENC)\
	  _slog (sLogCtrl,_copp_enc_logstr,thisFileName,__LINE__,a,b);\
	}
  #define COPP_LOG_ENC2(a,b,c) {\
	if (acse_debug_sel & COPP_LOG_ENC)\
	  _slog (sLogCtrl,_copp_enc_logstr,thisFileName,__LINE__,a,b,c);\
	}
  #define COPP_LOG_ENC3(a,b,c,d) {\
	if (acse_debug_sel & COPP_LOG_ENC)\
	  _slog (sLogCtrl,_copp_enc_logstr,thisFileName,__LINE__,a,b,c,d);\
	}
  #define COPP_LOG_ENC4(a,b,c,d,e) {\
	if (acse_debug_sel & COPP_LOG_ENC)\
	  _slog (sLogCtrl,_copp_enc_logstr,thisFileName,__LINE__,a,b,c,d,e);\
	}
  /* continuation macros */
  #define COPP_LOG_ENCC0(a) {\
	if (acse_debug_sel & COPP_LOG_ENC)\
	  _slogc (sLogCtrl,a);\
	}
  #define COPP_LOG_ENCC1(a,b) {\
	if (acse_debug_sel & COPP_LOG_ENC)\
	  _slogc (sLogCtrl,a,b);\
	}
  /* hex logging */
  #define COPP_LOG_ENCH(a,b) {\
	if (acse_debug_sel & COPP_LOG_ENC_HEX)\
	  _slogHex (sLogCtrl,a,b);\
	}

  /*--------------------------------------------*/
  /* PS-user ind and cnf logging		*/
  /*--------------------------------------------*/

  #define COPP_LOG_DEC0(a) {\
	if (acse_debug_sel & COPP_LOG_DEC)\
	  _slog (sLogCtrl,_copp_dec_logstr,thisFileName,__LINE__,a);\
	}
  #define COPP_LOG_DEC1(a,b) {\
	if (acse_debug_sel & COPP_LOG_DEC)\
	  _slog (sLogCtrl,_copp_dec_logstr,thisFileName,__LINE__,a,b);\
	}
  #define COPP_LOG_DEC2(a,b,c) {\
	if (acse_debug_sel & COPP_LOG_DEC)\
	  _slog (sLogCtrl,_copp_dec_logstr,thisFileName,__LINE__,a,b,c);\
	}
  #define COPP_LOG_DEC3(a,b,c,d) {\
	if (acse_debug_sel & COPP_LOG_DEC)\
	  _slog (sLogCtrl,_copp_dec_logstr,thisFileName,__LINE__,a,b,c,d);\
	}
  #define COPP_LOG_DEC4(a,b,c,d,e) {\
	if (acse_debug_sel & COPP_LOG_DEC)\
	  _slog (sLogCtrl,_copp_dec_logstr,thisFileName,__LINE__,a,b,c,d,e);\
	}
  /* continuation macros */
  #define COPP_LOG_DECC0(a) {\
	if (acse_debug_sel & COPP_LOG_DEC)\
	  _slogc (sLogCtrl,a);\
	}
  #define COPP_LOG_DECC1(a,b) {\
	if (acse_debug_sel & COPP_LOG_DEC)\
	  _slogc (sLogCtrl,a,b);\
	}
  /* hex logging */
  #define COPP_LOG_DECH(a,b) {\
	if (acse_debug_sel & COPP_LOG_DEC_HEX)\
	  _slogHex (sLogCtrl,a,b);\
	}
#else
  /*--------------------------------------------*/
  /* hard errors logging			*/
  /*--------------------------------------------*/

  #define COPP_LOG_ERR0(a)
  #define COPP_LOG_ERR1(a,b)
  #define COPP_LOG_ERR2(a,b,c)
  #define COPP_LOG_ERR3(a,b,c,d)

  /*--------------------------------------------*/
  /* PS-user req and rsp logging		*/
  /*--------------------------------------------*/

  #define COPP_LOG_ENC0(a)
  #define COPP_LOG_ENC1(a,b)
  #define COPP_LOG_ENC2(a,b,c)
  #define COPP_LOG_ENC3(a,b,c,d)
  #define COPP_LOG_ENC4(a,b,c,d,e)
  #define COPP_LOG_ENCC0(a)
  #define COPP_LOG_ENCC1(a,b)
  #define COPP_LOG_ENCH(a,b)

  /*--------------------------------------------*/
  /* PS-user ind and cnf logging		*/
  /*--------------------------------------------*/

  #define COPP_LOG_DEC0(a)
  #define COPP_LOG_DEC1(a,b)
  #define COPP_LOG_DEC2(a,b,c)
  #define COPP_LOG_DEC3(a,b,c,d)
  #define COPP_LOG_DEC4(a,b,c,d,e)
  #define COPP_LOG_DECC0(a)
  #define COPP_LOG_DECC1(a,b)
  #define COPP_LOG_DECH(a,b)

#endif


#ifdef __cplusplus
}
#endif

#endif  /* end of 'already included' 	*/
