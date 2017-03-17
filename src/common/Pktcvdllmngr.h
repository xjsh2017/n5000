#ifndef PKT_CONVERT_DLL_MNGR_H_INCLUDE

#include "Packet2Message_I.h"

typedef CPacketConvert_I* (*_FUN_CREATE_PKTCV)();
typedef void (*_FUN_DESTROY_PKTCV)(CPacketConvert_I* theInterface);

class CPktcvdllmngr
{
public:
	CPktcvdllmngr();
	~CPktcvdllmngr(){};

public:
	int load_pktcv_lib();
	int unload_pktcv_lib();
	CPacketConvert_I* create_pktcv();
	int destroy_pktcv(CPacketConvert_I* theInterface);
	
private:
	_FUN_CREATE_PKTCV m_pFunCrePktcv;
	_FUN_DESTROY_PKTCV m_pFunDestroyPktcv;
	XJHANDLE m_hLib;

};

#define PKT_CONVERT_DLL_MNGR_H_INCLUDE
#endif /*PKT_CONVERT_DLL_MNGR_H_INCLUDE*/