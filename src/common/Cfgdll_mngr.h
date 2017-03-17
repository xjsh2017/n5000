#ifndef DLL_MNGR_H
#define DLL_MNGR_H

#include "cfgmngr.h"
#include "SYS_MODEL_STRUCT.h"
#include "SYS_CONFIGMODEL_STRUCT.h"
#include "WAVE_RECORD_STRUCT.h"
enum
{
	max_storage_len = 60
};

typedef struct _STORAGE_STRUCT
{
	int n_storage;

	char (*c_storage)[max_storage_len];

	_STORAGE_STRUCT()
	{
		n_storage = 0;
		c_storage = NULL;
	}

}STORAGE_STRUCT;


//�����ļ�����̬���ʼ����������
typedef int (*P_CFGMNGR_INIT_FUN)(const char* ccfgfilefullpath);

//�����ļ�����̬��ж�ػ�������
typedef int (*P_CFGMNGR_UNINIT_FUN)(void);

//�������ļ���ȡָ��model��ָ��item��ֵ�ĺ���
typedef CFG_ITEM* (*P_CFGMNGR_GET_ITEMVAL_FUN)(const char* c_model_name,const char* c_item_name);

//�������ļ���ȡϵͳģ�͵ĺ���
typedef SCANNER* (*P_CFGMNGR_GET_SYSMODEL_FUN)(void);


typedef SNIFFER_PORT** (*P_CFG_GET_PORTLST_FUNC)(int& nlistsize);

typedef SNIFFER_APP* (*P_CFGMNGR_GET_IED_BYSYSID) (int nsysid);
typedef SNIFFER_APP* (*P_CFGMNGR_GET_IED_BY_ADDR)(char* cNetaddr);
typedef SNIFFER_APP* (*P_CFGMNGR_GET_IED_BY_APPID)(int nappid);
typedef SNIFFER_PORT* (*P_CFGMNGR_GET_PORT)(int nsys_portseq);//
typedef APP_CHANNEL_RELATION* (*P_CFGMNGR_GET_APP_4Y)(int nsysid);//cfgmngr_get_app_4y_cfginfo

class CCfgdll_mngr
{

public:
	CCfgdll_mngr();
	virtual ~CCfgdll_mngr(){};

public:
	int LoadCfgmngrLibrary();
	int LoadCfgfile();
	int UnLoadCfgmngrLibrary();
	int UnLoadCfgfile();

public:
	CFG_ITEM* GetItemVal(const char* c_model_name,const char* c_item_name);
	SCANNER* GetSysModel(void);
	//��ȡ��־·���͵ȼ����ļ���С
	int GetLogmngr(char *cLogPath, int &nLogLevel, int &nLogSize);

	int DelCharPath(char *c_XmlLogPath, char *cResult);
	STORAGE_STRUCT * Getstoragegroup(int &nGroup);
	int releaseArrays(STORAGE_STRUCT *pstoragegroup, int nGroup);

	SNIFFER_PORT** GetSnifferPortLst(int &nList);

	SNIFFER_APP*  Get_ied_bynetidaddr(char* cNetaddr);
	SNIFFER_APP*  Get_ied_byappid(int nappid);
	SNIFFER_APP*  Get_ied_bysysid(int nsysid);
	SNIFFER_PORT* Get_port(int nsys_portseq);
	APP_CHANNEL_RELATION* get_app_4y(int nsys_id);
	SNIFFER_CARD** Get_FirstSniffer_CardLst(int &nList);


private:
	BOOL DeleteEndChar(char *cSrc, char *cDest, char cDel);
	BOOL DeleteBeginChar(char *cSrc, char *cDest, char cDel);

private:

	/*
	* @brief	���ض�̬����
	*/
	XJHANDLE m_CfgmngrLibHandle;

	//�����ļ�����̬���ʼ����������ָ��
	P_CFGMNGR_INIT_FUN m_pFun_cfgmngr_init;
	
	//�����ļ�����̬��ж�ػ�������ָ��
	P_CFGMNGR_UNINIT_FUN m_pFun_cfgmngr_uninit;
	
	//�������ļ���ȡָ��model��ָ��item��ֵ�ĺ���ָ��
	P_CFGMNGR_GET_ITEMVAL_FUN m_pFun_cfgmngr_getitemval;
	
	//�������ļ���ȡϵͳģ�͵ĺ���ָ��
	P_CFGMNGR_GET_SYSMODEL_FUN m_pFun_cfgmngr_get_sysmodel;

	P_CFG_GET_PORTLST_FUNC m_pFun_cfgmngr_get_portlst;

	P_CFGMNGR_GET_IED_BYSYSID m_pFun_cfgmngr_get_ied_bysysid;
	P_CFGMNGR_GET_IED_BY_ADDR m_pFun_cfgmngr_get_ied_byaddr;
	P_CFGMNGR_GET_IED_BY_APPID m_pFun_cfgmngr_get_ied_byappid;
	P_CFGMNGR_GET_PORT m_pFun_cfgmngr_get_port;
	P_CFGMNGR_GET_APP_4Y m_pFun_cfgmngr_get_app_4y;

};

#endif //DLL_MNGR_H