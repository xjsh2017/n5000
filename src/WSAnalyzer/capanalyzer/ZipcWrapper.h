#pragma once
#include "zipc_I.h"
	/*
	* @brief ��̬������ӿں���
	*/
typedef CZIP_I* (*CREATEZIPC_I)(LOG_STRUCT logInfo);
typedef void (*DESTROYZIPC_I)(CZIP_I* theInterface);
class CZipcWrapper
{
private:
    static CZipcWrapper *unzip;
public:
    static CZipcWrapper *getInstance()
    {
        if(unzip == NULL)
        {
            unzip = new CZipcWrapper();
        }
        return unzip;
    }
public:
	CZipcWrapper(void);
	~CZipcWrapper(void);
public:
	int Initialize(const char *cLibPathFullName);
	int Uninitialize();
public:
	/**��ѹ��**/
	int UnZip_I(const char *cZipfile, const char *cDestdir);
	/**��ѹ�� minilzo�㷨**/
	int UnZip_MINILZO_I(const char *cZipfile, const char *cDestdir,BOOL bSafe=TRUE);
	/**ѹ��**/
	int Zip_I(const char *cZipfile, const char *cSrcfiles,const char* cPassword,int nZlevel);
private:
	/*
	* @brief	���ض�̬��zipc.dll���
	*/
    XJHANDLE m_zipLibHandle;
	CREATEZIPC_I m_pFun_CreateZipc_i;
	DESTROYZIPC_I m_pFUn_DestroyZipc_i;
	CZIP_I*  m_zipI;
	unsigned char m_zipfilesrcbuf[1024*1024*50];    //25M
	unsigned char m_zipfileunzipbuf[1024*1024*200];//100M
};

