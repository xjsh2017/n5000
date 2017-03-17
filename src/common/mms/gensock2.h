/************************************************************************/
/* SOFTWARE MODULE HEADER ***********************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*          2002 - 2005, All Rights Reserved                            */
/*									*/
/* MODULE NAME : gensock2.h 						*/
/* PRODUCT(S)  : General Sockets Interface				*/
/*									*/
/*	    								*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 07/11/05  EJV    26     Add convertIPAddr proto.			*/
/* 06/10/05  JRB    25     Obsolete "_sockClose" function replaced with	*/
/*			   macro that calls normal "sockClose".		*/
/* 05/10/05  EJV    24     Added _sockAllocSock, _sockAddSock protos.	*/
/* 03/23/05  EJV    23     Added sockCreateWakeupSockets		*/
/*			   linux MMSEASE_MOSI:add GENSOCK_THREAD_SUPPORT*/
/* 03/16/05  JRB    22     Add GENSOCK_HUNT_DISCONNECT define.		*/
/* 02/21/05  JRB    21     Fix ioctlsocket define (deleted from tp0_sock)*/
/* 02/10/05  MDE    20     Added sockGetRemAddrInfo			*/
/* 07/22/04  EJV    19     sun: added GENSOCK_THREAD_SUPPORT		*/
/* 06/18/04  MDE    18     Include winsock2.h if needed			*/
/* 03/11/04  EJV    17     SOCK_OPTLEN,SOCK_ADDRLEN diff for each system*/
/* 02/25/04  EJV    16     _AIX: added GENSOCK_THREAD_SUPPORT		*/
/*			   Added SOCK_OPTLEN and SOCK_ADDRLEN.		*/
/* 			   Chg SOCK_NOTSOCK to be EBADF (not ENOTSOCK).	*/
/* 01/27/04  EJV    15     Added SOCK_NOTSOCK error.			*/
/* 01/14/04  EJV    14     GEN_SOCK: added sockTxQueuedGroupCnt field.	*/
/*			   GEN_SOCK_DATA: added eot field.		*/
/*		           Added sockGetTxQueueGroupCnt proto.		*/
/* 10/23/03  JRB    13     Move all system includes to sysincs.h	*/
/* 06/25/03  JRB    12     Move log macros to "sock_log.h".		*/
/* 06/23/03  EJV    11     Added 'first' param to sockTxQueueAdd.	*/
/* 06/20/03  EJV    10	   Rem param from gs_mutex_get.			*/
/* 06/20/03  EJV    09	   Added sockTxQueueDestroy.Chg gSock w/pSock.	*/
/*			   Alligned struct fields.			*/
/* 06/19/03  EJV    08     _WIN32: added GENSOCK_THREAD_SUPPORT		*/
/*			   Added comments to struct, #else, #endif	*/
/* 06/19/03  JRB    07     Make Rx/Tx names more consistent.		*/
/* 06/18/03  JRB    06     Move defines to top & add SOCKADDR*, ioctlsocket*/
/*			   Del GENSOCK_THREAD_SUPPORT, define in makefile*/
/*			   Add forward reference.			*/
/*			   Add sockTxPend to GEN_SOCK.			*/
/*			   Add GSOCK_LOG_FLOWH.				*/
/*			   Del winsock2.h (windows.h gets right winsock)*/
/*			   Use "base" slog macros.			*/
/*			   Add uSockTxBufFree to GEN_SOCK_CONFIG.	*/
/*			   Add sockTxMsg, sockTxQueue* funcs.		*/
/* 06/13/03  MDE    05     More user poll featuresAdded sockGetFds	*/
/* 06/13/03  EJV    04     Added parameter to gs_mutex_get.		*/
/* 05/14/03  MDE    03     Added sockGetFds				*/
/* 05/09/03  JRB    02     Add SOCK_* defines for !_WIN32		*/
/* 02/17/03  MDE    01     Created					*/
/************************************************************************/

#ifndef GENSOCK_INCLUDED
#define GENSOCK_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "glbtypes.h"
#include "sysincs.h"
#include "gen_list.h"
#include "glbsem.h"
#include "mem_chk.h"
#include "sock_log.h"	/* logging macros	*/

/* Windows vs. the rest of the world ... */
#ifdef _WIN32

/* If winsock.h or winsock2 have not been included, use winsock2 */
#if !defined(_WINSOCKAPI_)
#include "winsock2.h"
#endif

#if !defined(GENSOCK_THREAD_SUPPORT)
#define	GENSOCK_THREAD_SUPPORT
#endif
#define SOCKET_ERRORNO  WSAGetLastError()
#define SOCK_EADDRINUSE WSAEADDRINUSE
#define SOCK_WOULDBLOCK WSAEWOULDBLOCK
#define SOCK_INTR       WSAEINTR
#define SOCK_NOBUFS     WSAENOBUFS
#define SOCK_INPROGRESS WSAEINPROGRESS
#define SOCK_TIMEDOUT   WSAETIMEDOUT
#define SOCK_NOTSOCK    WSAENOTSOCK
#define CLOSE_SOCKET(x) closesocket(x)

#define SOCK_OPTLEN     ST_INT		/* in getsockopt	*/
#define SOCK_ADDRLEN    ST_INT		/* in accept 		*/

#else	/* !_WIN32 */
#if defined(_AIX)  || defined(sun) || (defined(linux) && defined(MMSEASE_MOSI))
#if !defined(GENSOCK_THREAD_SUPPORT)
#define	GENSOCK_THREAD_SUPPORT
#endif
#endif /* defined(_AIX) */
#define SOCKET_ERRORNO errno
#define SOCK_EADDRINUSE EADDRINUSE
#define SOCK_WOULDBLOCK EWOULDBLOCK
#define SOCK_INTR       EINTR
#define SOCK_NOBUFS     ENOBUFS
#define SOCK_INPROGRESS EINPROGRESS
#define SOCK_TIMEDOUT   ETIMEDOUT
#define SOCK_NOTSOCK    EBADF
#define CLOSE_SOCKET(x) close(x)

#define SOCKET	int
#define INVALID_SOCKET	(-1)
#define SOCKADDR	struct sockaddr
#define SOCKADDR_IN	struct sockaddr_in

#if defined (__OS2__)		/* OS/2 ioctl has extra arg	*/
#define ioctlsocket(hSock,opt,valptr)	ioctl(hSock, opt, (ST_CHAR *)valptr, sizeof(u_long))
#elif defined (VXWORKS)		/* VXWORKS wants arg to be int	*/
#define ioctlsocket(hSock,opt,valptr)	ioctl(hSock, opt, (int) valptr)
#else	/* all other systems (e.g. UNIX)	*/
#define ioctlsocket	ioctl
#endif

#if defined(_AIX) || defined(sun)
#define SOCK_OPTLEN     socklen_t	/* in getsockopt	*/
#define SOCK_ADDRLEN    socklen_t	/* in accept 		*/
#else
#define SOCK_OPTLEN     ST_INT		/* in getsockopt	*/
#define SOCK_ADDRLEN    ST_INT		/* in accept 		*/
#endif
#endif  /* !_WIN32 */

/************************************************************************/
/************************************************************************/
/* Defines								*/

#define	GENSOCK_MAX_HEADER_SIZE	20


/************************************************************************/
/* Thread support */

#if defined(GENSOCK_THREAD_SUPPORT)

extern ST_BOOLEAN gs_poll_mode;
extern ST_MUTEX_SEM gSockMutex;

#define GET_SOCKET_MUTEX      {if (!gs_poll_mode)gs_mutex_get(&gSockMutex);}
#define RELEASE_SOCKET_MUTEX  {if (!gs_poll_mode)gs_mutex_free(&gSockMutex);}
#define GSOCK_MUTEX_LOCK(sem)	{if (!gs_poll_mode)gs_mutex_get(sem);}
#define GSOCK_MUTEX_UNLOCK(sem)	{if (!gs_poll_mode)gs_mutex_free(sem);}
#else  /* !GENSOCK_THREAD_SUPPORT */
#define GET_SOCKET_MUTEX
#define RELEASE_SOCKET_MUTEX
#define GSOCK_MUTEX_LOCK(a)
#define GSOCK_MUTEX_UNLOCK(a)
#endif /* !GENSOCK_THREAD_SUPPORT */

struct tag_GEN_SOCK;			/* Forward reference	*/
/************************************************************************/
/************************************************************************/

typedef struct tag_GEN_SOCK_DATA
  {
/* User Send/Receive Queueing		*/
  DBL_LNK   	l;		

/* For user management of send/receive buffers */
  ST_UCHAR     *usrBufBase; 	/* Data buffer 				*/ 
  ST_INT  	usrBufLen;	/* Total send buffer size		*/

  ST_VOID      *usr1;		/* Misc. user use */
  ST_VOID      *usr2;		/* Misc. user use */

/* Data to be sent/Received Data */	
  ST_UCHAR     *data;		/* Recv Data, Send Data			*/
  ST_INT  	dataLen;    	/* Length of data recvd, or to be sent	*/

  ST_INT 	result;		/* For receive, SD_SUCCESS or error 	*/
  ST_BOOLEAN	eot;		/* SD_TRUE - if last msg in a group	*/
  } GEN_SOCK_DATA;



/************************************************************************/

typedef struct
  {
  time_t   createdTime; 	
  time_t   activeTime;			/* connected or listening	*/
  ST_LONG  numSend;			/* num of RFC1006 msgs sent	*/
  ST_LONG  numRecv;			/* num of RFC1006 msgs received	*/
  } GEN_SOCK_STATS;

/************************************************************************/

typedef struct tag_GEN_SOCK_CONFIG
  {
/* Config parameters */
  ST_INT            hdrSize;		/* Default sizeof (ST_INT)	*/

/* setsockopt parameters */
  ST_BOOLEAN        setSockOpts;
  ST_INT            noDelay;
  ST_INT            keepAlive;
  ST_INT            reuseAddr;
  ST_INT            rcvBufSize; 
  ST_INT            sndBufSize;

/* User can set to disable receive */
  ST_BOOLEAN        pauseRecv;

/* Listen backlog */
  ST_INT            listenBacklog;

/* User callbacks */
  ST_RET  (*uSockConnect) (struct tag_GEN_SOCK *pSock);
  ST_VOID (*uSockDisconnect) (struct tag_GEN_SOCK *pSock);
  ST_VOID (*uSockHunt) (struct tag_GEN_SOCK *pSock, ST_INT *huntStateIo,
		            ST_CHAR *buf, ST_INT bufCount, ST_INT *lenOut);
  ST_VOID (*uSockRx) (struct tag_GEN_SOCK *pSock, GEN_SOCK_DATA *sockData);
  ST_VOID (*uSockRxBufAlloc) (struct tag_GEN_SOCK *pSock, ST_INT dataLen, GEN_SOCK_DATA **sockData);
  ST_VOID (*uSockTxBufFree) (struct tag_GEN_SOCK *pSock, GEN_SOCK_DATA *sockData);
  ST_VOID (*uSockWritable) (struct tag_GEN_SOCK *pSock);
	    
/* User fields */
  ST_EVENT_SEM     recvEvent;		
  ST_VOID	   *usr1;
  ST_VOID	   *usr2;
  ST_VOID	   *usr3;
  ST_VOID	   *usr4;

/* Internal Security Hooks: Set to NULL unless using security plug-in */ 
  ST_BOOLEAN secEnable;
  ST_RET  (*secConnectProc) (struct tag_GEN_SOCK *pSock);
  ST_VOID (*secDisconnectProc) (struct tag_GEN_SOCK *pSock);
  ST_RET  (*secRxProc) (struct tag_GEN_SOCK *pSock, ST_CHAR *dest, ST_INT maxRx, 
		     ST_INT flags, ST_INT *rxCountOut, ST_BOOLEAN *secDataLeftOut);
  ST_RET  (*secTxProc) (struct tag_GEN_SOCK *pSock, GEN_SOCK_DATA *sockData, ST_INT *numSentOut);
  ST_VOID (*secWritable) (struct tag_GEN_SOCK *pSock);
  ST_VOID (*secSockFree) (struct tag_GEN_SOCK *pSock);

  ST_VOID          *secCtrl;

/* Set to enable write select */
  ST_BOOLEAN        chkWritable;	
  } GEN_SOCK_CONFIG;

/************************************************************************/

/* State defines for GEN_SOCK */
#define GS_STATE_LISTENING  		1 
#define GS_STATE_STOPPING_LISTEN	2 
#define GS_STATE_FAILED_LISTEN		3 
#define GS_STATE_CONNECTING 		4 
#define GS_STATE_CONNECTED  		5	
#define GS_STATE_CLOSED  		6	

/* Role defines */
#define GS_ROLE_CALLING	    	1
#define GS_ROLE_LISTENING  	2
#define GS_ROLE_CALLED	    	3

/* disconnectReason defines */
#define GS_DISCONNECT_UNKNOWN		0
#define GS_DISCONNECT_CONNECT_FAILED	1
#define GS_DISCONNECT_USR_REFUSED	2
#define GS_DISCONNECT_SEND_FAILURE	3
#define GS_DISCONNECT_RECV_FAILED	4
#define GS_DISCONNECT_ACCEPT_FAILED	5
#define GS_DISCONNECT_CLOSED		6
#define GS_DISCONNECT_TERMINATING	7

/* Hunt state defines */
#define GENSOCK_HUNT_UNDERWAY	1
#define GENSOCK_HUNT_DONE	2
#define GENSOCK_HUNT_RESET	3
#define GENSOCK_HUNT_DISCONNECT	4	/* stop hunting and disconnect	*/

#define RECV_STATE_HUNT	1
#define RECV_STATE_DATA	2

typedef struct tag_GEN_SOCK
  {
  DBL_LNK              l;			/* internal use */

/* Socket state */
  ST_INT               sockState;
  ST_INT               role;

/* Valid in user socket disconnet call */
  ST_INT               disconnectReason;

/* Config parameters */
  GEN_SOCK_CONFIG      sockCfg;

  GEN_SOCK_STATS       sockStats;

/* For Listen socket only */
  ST_UINT              maxActive;
  ST_UINT              numActive;

/* For called socket, reference the listen socket */
  struct tag_GEN_SOCK *listenSocket;

  ST_INT               callingAddrLen;
  SOCKADDR_IN          callingAddr;

/* Internal */
  SOCKET               hSock;		
  ST_BOOLEAN  	       readyToFree;

  ST_INT               recvState;
  ST_CHAR             *recvBuf;
  ST_INT               recvDoneCount;
  ST_INT               recvCurrCount;
  ST_CHAR              hdrBuf[GENSOCK_MAX_HEADER_SIZE];
  GEN_SOCK_DATA       *sockData;

  struct sockaddr_in   sockAddrIn;
  ST_BOOLEAN           usrCloseCalled;
  ST_BOOLEAN           recvUsing;

  struct tag_GEN_SOCK_CTRL *serviceCtrl;

  GEN_SOCK_DATA       *sockTxPend;	     /* Queue of data to be sent.	*/
  ST_UINT              sockTxQueueGroupCnt;  /* num of queued msgs w/eot to tx	*/
  } GEN_SOCK;

/************************************************************************/

/* Allow one for wakeup socket */
#define GS_MAX_SOCK_PER_SERVICE   FD_SETSIZE - 1

typedef struct tag_GEN_SOCK_CTRL
  {
  DBL_LNK          l;
  ST_INT           numSock;
  GEN_SOCK        *sockList;

#if defined(GENSOCK_THREAD_SUPPORT)
  ST_EVENT_SEM     serviceEvent;		
  ST_THREAD_HANDLE thService;
  ST_THREAD_ID     tIdService;

  ST_UINT16        wakeupPort;
  SOCKET           xCalledSock;
  SOCKET           xCallingSock;
#endif  /* GENSOCK_THREAD_SUPPORT */
  } GEN_SOCK_CTRL;

/************************************************************************/
/************************************************************************/

ST_RET sockStart (ST_VOID);
ST_RET sockEnd (ST_VOID);

ST_RET sockInitCalling (GEN_SOCK_CONFIG *sockCfg, ST_UINT16 portNo, 
  			ST_CHAR *ipAddr, GEN_SOCK **pSockOut);

ST_RET sockInitListen (GEN_SOCK_CONFIG *sockCfg, ST_UINT16 portNo,
                       ST_INT maxActive, GEN_SOCK **pListenSockOut);

ST_RET sockTx (GEN_SOCK *pSock, GEN_SOCK_DATA *sockData, ST_INT *numSentOut);
ST_RET sockTxMsg (GEN_SOCK *pSock, GEN_SOCK_DATA *sockData);
ST_RET sockTxQueueAdd (GEN_SOCK *pSock, GEN_SOCK_DATA *sockData, ST_BOOLEAN first);
ST_RET sockTxQueueDestroy (GEN_SOCK *pSock);
ST_RET sockTxQueueProc (GEN_SOCK *pSock);
ST_UINT sockTxQueueGroupCntGet (GEN_SOCK *pSock);
ST_RET sockStopRecv (GEN_SOCK *pSock);
ST_RET sockClose (GEN_SOCK *pSock);
ST_RET sockFree (GEN_SOCK *pSock);

/* Polling support */

/* Get socket descriptors */
typedef struct
  {
  ST_INT   totalfds;
  ST_UINT  selectnfds;

  ST_INT   numReadfds;
  fd_set   readfds;

  ST_INT   numWritefds;
  fd_set   writefds;

  ST_INT   numExceptfds;
  fd_set   exceptfds;
  } GENSOCK_FD_SET;

ST_VOID sockGetFds (GEN_SOCK_CTRL *serviceCtrl, GENSOCK_FD_SET *sockFds);

/* Return values for sockCtrlService, sockSelectResultProcess */
#define GS_SELECT_NO_ACTIVE_SOCK	1
#define GS_SELECT_TIMEOUT		2
#define GS_SELECT_ERROR			3
#define GS_SELECT_ACTIVE		4
#define GS_SELECT_TERMINATED		5

ST_VOID sockServiceAll (ST_LONG timeOut);
ST_INT sockCtrlService (GEN_SOCK_CTRL *serviceCtrl, ST_LONG timeOut);

ST_VOID sockInitAllFds (GENSOCK_FD_SET *sockFds);
ST_VOID sockInitFds (GEN_SOCK_CTRL *serviceCtrl, GENSOCK_FD_SET *sockFds);
ST_VOID sockAddReadFds (GENSOCK_FD_SET *sockFds, SOCKET hSock);
ST_VOID sockAddWriteFds (GENSOCK_FD_SET *sockFds, SOCKET hSock);
ST_VOID sockAddExceptFds (GENSOCK_FD_SET *sockFds, SOCKET hSock);
ST_INT  sockSelectResultProcess (GEN_SOCK_CTRL *serviceCtrl, 
				int nfds, GENSOCK_FD_SET *sockFds);

ST_RET sockServiceFreeList (ST_VOID);

/* Thread support */
ST_RET sockCreateWakeupSockets (ST_UINT16 basePort, ST_UINT portRange,
                                ST_UINT16 *usedPort,
                                SOCKET *callingSock, SOCKET *calledSock);
ST_VOID sockServiceWakeAll (ST_VOID);
ST_VOID sockServiceWake (GEN_SOCK_CTRL *serviceCtrl);

/* Logging */
ST_VOID sockLogState (ST_VOID);
ST_VOID sockLogSockState (GEN_SOCK *pSock, ST_CHAR *prefix);

/* Socket info */
ST_RET sockGetRemAddrInfo (GEN_SOCK *pSock, SOCKADDR_IN *remSockAddrDest, 
			   ST_CHAR **remAddrTxtOut, ST_INT *portOut);

/************************************************************************/
/************************************************************************/
/* INTERNAL                                                             */
/************************************************************************/

GEN_SOCK *_sockAllocSock (ST_INT role, ST_INT sockState, SOCKET hSock,
                        GEN_SOCK_CONFIG *sockCfg);
ST_VOID  _sockAddSock (GEN_SOCK *pSock);

ST_RET _sockRecv (GEN_SOCK *pSock, ST_CHAR *dest, 
                         ST_INT maxRx, ST_INT flags, ST_INT *rxCountOut);

ST_RET _sockTx (GEN_SOCK *pSock, GEN_SOCK_DATA *sockData, ST_INT *numSentOut);

/* OBSOLETE: For backward compatibility only. New code should use "sockClose".*/
#define _sockClose	sockClose


		/* --------------------------------------------	*/
		/*	Misc socket related functions		*/
		/* --------------------------------------------	*/

ST_ULONG convertIPAddr (ST_CHAR *ipAddrStr, ST_BOOLEAN useGetHostByName);


/************************************************************************/

#ifdef __cplusplus
}
#endif

#endif  /* GENSOCK_INCLUDED */






