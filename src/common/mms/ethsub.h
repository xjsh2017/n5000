#ifndef ETHSUB_INCLUDED
#define ETHSUB_INCLUDED
/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*	(c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*	2000-2000 All Rights Reserved					*/
/*									*/
/* MODULE NAME : ethsub.h						*/
/* PRODUCT(S)  : Lean-T Stack for Windows 95/98/NT			*/
/*									*/
/* MODULE DESCRIPTION :							*/
/*	Definitions to allow Subnetwork interface to work with OSI LLC	*/
/*	Ethernet driver.						*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev     Comments					*/
/* --------  ---  ------   -------------------------------------------  */
/* 11/20/02  ASK    02     Add ethdown_etype proto			*/
/* 04/06/00  JRB    01     Created					*/
/************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************/
/*		Include files required to interface with	*/
/*		OSI LLC NDIS Ethernet Driver			*/
/*                                                              */
/****************************************************************/

#include "conpack.h"
#include "osillc.h"

#include "glbtypes.h"
#include "slog.h"
#include "stime.h"
#include "clnp_usr.h"	/* clnp_sne.h needs this	*/
#include "clnp_log.h"
#include "clnp_sne.h"	/* for SN_UNITDATA, etc.	*/


/****************************************************************/
/* Base Definitions						*/
/****************************************************************/
typedef unsigned char X;
typedef char * A;

/****************************************************************/
/* Definitions copied from "llcprctx"				*/
/****************************************************************/
#define LLC_MAC_ADD_LEN	6

/* NOTE: this struct copied and then unused params removed.	*/
typedef struct
{
/** ----- Following fields are specific to this Access Method */
 HANDLE hFile;		/* LLC device driver descriptor */

 unsigned long seq;	/* sequence number to expect */
 void *readlst[10];	/* list of pointers to out of sequence packet blocks */

 /* ----- Following fields are required for 95/98 */
 struct readT *pWriteEvents;	/* list of write event structures */
 struct readT *pReadEvents;	/* list of read event structures */
} llcxcbT,*llcxcbTp;

/****************************************************************/
/* Definitions copied from "llcupcon"				*/
/****************************************************************/
#define LLC_MAX_LSDU		1514	/* max. LLC frame length	*/

#define LLC_HEAD_LEN		3 	/* DSAP + SSAP + Control	*/
#define MAC_HEAD_LEN		((LLC_MAC_ADD_LEN * 2) + 2)	/* DADDR + SADDR + LENGTH	*/

#define LLC_ISO_SAP		0xFE	/* ISO LLC SAP			*/
#define LLC_UI			0x03	/* control field (always UI)	*/

#define LLC_DEBUG_UP		0x01
#define LLC_DEBUG_DOWN		0x02

/****************************************************************/
/* Definitions copied from "llcuptyp"				*/
/****************************************************************/

/************************************************************************/
/* MAC Frame structure							*/
/************************************************************************/

typedef struct
  {
  X dstadr[LLC_MAC_ADD_LEN];	/* destination MAC Address	*/
  X srcadr[LLC_MAC_ADD_LEN];	/* source MAC Address		*/
  X length[2];			/* LLC data length		*/
  X data[LLC_MAX_LSDU - MAC_HEAD_LEN];	/* MAC data		*/
  } frameT, *frameTp;

/************************************************************************/
/* OSI Event structure - Modified for MMS_LITE				*/
/************************************************************************/

typedef struct
  {
  void * nxevt;
  SN_UNITDATA sn_req;	/* event data in format needed by LEANT CLNP.	*/
  } osievtT, *osievtTp;

typedef struct readT
  {
  int inuse;
  HANDLE hEvent;
  OVERLAPPED *ovlp;
  frameT *framep;
  void *pb;
  llcxcbTp gccp;
  } readT, *readTp;


typedef struct
  {
  int DummyStructureElement;
  } *intcbTp;	/* Dummy type. Should never be used for MMS_LITE.	*/

/************************************************************************/
/* MMS_LITE function to Queue received packet for Lite CLNP.		*/
/************************************************************************/
void QueueRxPacket (frameTp framep);

/************************************************************************/
/* Functions that may be called from CLNP.				*/
/************************************************************************/
void ethdown (SN_UNITDATA *sn_req);
void ethdown_etype (SN_UNITDATA *sn_req);
void ethsapoff (void);
void ethsapon (A drvname);
int llcsetup (void);

BOOL QueryOid (HANDLE hDriver, ULONG ulOid, PBYTE pBuffer, ULONG ulLength);
BOOL SetOid   (HANDLE hDriver, ULONG ulOid, PBYTE pBuffer, ULONG ulLength);

void osifreeevt (osievtTp evt);
osievtTp osiallocevt ();
void osiputevt (osievtTp evt);
osievtTp osigetevt ();

void OsiReadThread (LPDWORD lpdwParam);
void CALLBACK WriteCompletion (DWORD error, DWORD length, OVERLAPPED *ovlp);
void CALLBACK ReadCompletion (DWORD error, DWORD length, OVERLAPPED *ovlp);
void ProcessReadBlock (OVERLAPPED *ovlp, readT *readBlock);

/************************************************************************/
/* Global variables.							*/
/************************************************************************/
extern llcxcbT GCC;		/* General Current Context */
extern llcxcbTp GCCP;		/* General Current Context Pointer */
extern int      GSAP ;       /* General SAP                     */

/****************************************************************/
/*              Global LLC AM variables                    	*/
/*                                                              */
extern int MaxWriteOut;
extern int MaxReadOut;
extern int FramesPerBlock;
extern int ReceptionMode;
extern int DriverID;
extern int NumWriteOut;
extern CRITICAL_SECTION csOsiList;   /* Critical Section object for OSI Event List */

/************************************************************************/
/* Dummy macros to disable Marben logging.				*/
/************************************************************************/
#define bmshdr(a,b,c)
#define osibxhdr()
#define osibcstr(a,b)
#define osibflush()

#ifdef __cplusplus
}
#endif

#endif	/* !ETHSUB_INCLUDED	*/

