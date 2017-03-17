#include "Cfgdll_mngr.h"
#include "XJString.h"
#include "xjlib.h"

#include <map>
using namespace std;

#ifdef OS_LINUX  
const sint8* CFGMNGR_LIB_NAME = "./lib/cfgmngr.so"; //linux 配置文件管理动态库

#else
const sint8* CFGMNGR_LIB_NAME = "./lib/cfgmngr.dll";//windows 配置文件管理动态库

#endif

CCfgdll_mngr::CCfgdll_mngr()
{
	m_CfgmngrLibHandle = NULL;//初始化加载动态库句柄
	//配置文件管理动态库初始化环境函数指针
	m_pFun_cfgmngr_init = NULL;
	
	//配置文件管理动态库卸载环境函数指针
	m_pFun_cfgmngr_uninit = NULL;
	
	//从配置文件获取指定model和指定item的值的函数指针
	m_pFun_cfgmngr_getitemval = NULL;
	
	//从配置文件获取系统模型的函数指针
	m_pFun_cfgmngr_get_sysmodel = NULL;
	m_pFun_cfgmngr_get_portlst = NULL;

	m_pFun_cfgmngr_get_ied_bysysid = NULL;
	m_pFun_cfgmngr_get_ied_byaddr = NULL;
	m_pFun_cfgmngr_get_ied_byappid = NULL;
	m_pFun_cfgmngr_get_port = NULL;
	m_pFun_cfgmngr_get_app_4y = NULL;

}

int CCfgdll_mngr::LoadCfgfile()
{
	int nRet = RES_SUCCESS;

	if (NULL == m_pFun_cfgmngr_init)
	{
		return RES_FAIL;
	}

	CXJString strFilePathName;
	char  csysmodelcfgfilepath[128];            //模型配置文件路径
	char  csysmodelcfgfilename[128];            //系统模型配置文件名称
	
	memset(&csysmodelcfgfilepath,0,128);
	memset(&csysmodelcfgfilename,0,128);
	
	strcpy(csysmodelcfgfilename,"iscanner.xml");	//字典名称
	
#ifdef OS_WINDOWS
	strcpy(csysmodelcfgfilepath,"..\\config");		//字典路径	
	strFilePathName.Format("%s\\%s", csysmodelcfgfilepath, csysmodelcfgfilename);
	
#else
	strcpy(csysmodelcfgfilepath,"../config");		//字典路径	
	strFilePathName.Format("%s/%s", csysmodelcfgfilepath, csysmodelcfgfilename);
	
#endif
	
	if (0 != m_pFun_cfgmngr_init(NULL/*strFilePathName*/))
	{
// 		m_LogFile.FormatAdd(CLogFile::error, "加载配置文件%s失败!", strFilePathName.c_str());
		printf("加载配置文件%s失败! \n", strFilePathName.c_str());
		return RES_FAIL;
	}

	return nRet;
}

int CCfgdll_mngr::LoadCfgmngrLibrary()
{
	int nRet = RES_SUCCESS;

	m_CfgmngrLibHandle = xj_load_library(CFGMNGR_LIB_NAME);
	if (NULL == m_CfgmngrLibHandle)
	{
		printf("加载动态库: %s 失败! \n", CFGMNGR_LIB_NAME);
		printf("%s \n", strerror(errno));
		return RES_FAIL;
	}
	
	m_pFun_cfgmngr_init = (P_CFGMNGR_INIT_FUN)(xj_get_addr(m_CfgmngrLibHandle,"cfgmngr_initialize"));
	if (NULL == m_pFun_cfgmngr_init)
	{
		printf("获取%s函数cfgmngr_initialize的地址失败! \n", CFGMNGR_LIB_NAME);
		return RES_FAIL;
	}
	
	m_pFun_cfgmngr_uninit = (P_CFGMNGR_UNINIT_FUN)(xj_get_addr(m_CfgmngrLibHandle,"cfgmngr_uninitialize"));
	if (NULL == m_pFun_cfgmngr_uninit)
	{
		printf("获取%s函数cfgmngr_uninitialize的地址失败! \n", CFGMNGR_LIB_NAME);
		return RES_FAIL;
	}
	
	m_pFun_cfgmngr_getitemval = (P_CFGMNGR_GET_ITEMVAL_FUN)(xj_get_addr(m_CfgmngrLibHandle,"cfgmngr_get_cfgitemvalue"));
	if (NULL == m_pFun_cfgmngr_getitemval)
	{
		printf("获取%s函数cfgmngr_get_cfgitemvalue的地址失败! \n", CFGMNGR_LIB_NAME);
		return RES_FAIL;
	}
	
	m_pFun_cfgmngr_get_sysmodel = (P_CFGMNGR_GET_SYSMODEL_FUN)(xj_get_addr(m_CfgmngrLibHandle,"cfgmngr_get_sysmodel"));
	if (NULL == m_pFun_cfgmngr_get_sysmodel)
	{
		printf("获取%s函数cfgmngr_get_sysmodel的地址失败! \n", CFGMNGR_LIB_NAME);
		return RES_FAIL;
	}

	m_pFun_cfgmngr_get_portlst = (P_CFG_GET_PORTLST_FUNC)(xj_get_addr(m_CfgmngrLibHandle,"cfgmnfr_get_snifferportlist"));
	if (NULL == m_pFun_cfgmngr_get_portlst)
	{
		printf("获取%s函数cfgmnfr_get_snifferportlist的地址失败! \n", CFGMNGR_LIB_NAME);
		return RES_FAIL;
	}

	m_pFun_cfgmngr_get_ied_byaddr = (P_CFGMNGR_GET_IED_BY_ADDR)(xj_get_addr(m_CfgmngrLibHandle,"cfgmngr_get_ied_bynetidaddr"));
	if (NULL == m_pFun_cfgmngr_get_ied_byaddr)
	{
		printf("获取%s函数cfgmngr_get_ied_bynetidaddr的地址失败! \n", CFGMNGR_LIB_NAME);
		return RES_FAIL;
	}

	m_pFun_cfgmngr_get_ied_byappid = (P_CFGMNGR_GET_IED_BY_APPID)(xj_get_addr(m_CfgmngrLibHandle,"cfgmngr_get_ied_byappid"));
	if (NULL == m_pFun_cfgmngr_get_ied_byappid)
	{
		printf("获取%s函数cfgmngr_get_ied_byappid的地址失败! \n", CFGMNGR_LIB_NAME);
		return RES_FAIL;
	}

	m_pFun_cfgmngr_get_ied_bysysid = (P_CFGMNGR_GET_IED_BYSYSID)(xj_get_addr(m_CfgmngrLibHandle,"cfgmngr_get_ied_bysysid"));
	if (NULL == m_pFun_cfgmngr_get_ied_bysysid)
	{
		printf("获取%s函数 cfgmngr_get_ied_bysysid的地址失败! \n", CFGMNGR_LIB_NAME);
		return RES_FAIL;
	}
	
	m_pFun_cfgmngr_get_port  = (P_CFGMNGR_GET_PORT)(xj_get_addr(m_CfgmngrLibHandle,"cfgmngr_get_snifferport"));
	if (NULL == m_pFun_cfgmngr_get_port)
	{
		printf("获取%s函数 cfgmngr_get_snifferport的地址失败! \n", CFGMNGR_LIB_NAME);
		return RES_FAIL;
	}

	m_pFun_cfgmngr_get_app_4y = (P_CFGMNGR_GET_APP_4Y)(xj_get_addr(m_CfgmngrLibHandle,"cfgmngr_get_app_4y_cfginfo"));
	if (NULL == m_pFun_cfgmngr_get_app_4y)
	{
		printf("获取%s函数 cfgmngr_get_app_4y_cfginfo的地址失败! \n", CFGMNGR_LIB_NAME);
		return RES_FAIL;
	}

	return nRet;
}

int CCfgdll_mngr::UnLoadCfgfile()
{
	int nRet = RES_SUCCESS;

	if (NULL == m_pFun_cfgmngr_uninit)
	{
		return RES_FAIL;
	}
	
	if (0 != m_pFun_cfgmngr_uninit())
	{
		return RES_FAIL;
	}
	
	printf("卸载配置文件成功! \n");
	
	return nRet;
}

int CCfgdll_mngr::UnLoadCfgmngrLibrary()
{
	int nRet = RES_SUCCESS;

	int nFreeCfgmngrLib = 0;
	
	if (NULL != m_CfgmngrLibHandle)
	{
		nFreeCfgmngrLib = xj_free_library(m_CfgmngrLibHandle);
		if (0 != nFreeCfgmngrLib)
		{
			printf("卸载动态库 %s 失败! \n", CFGMNGR_LIB_NAME);
			return RES_FAIL;
		}
		else
		{
			printf("卸载动态库 %s 成功! \n", CFGMNGR_LIB_NAME);
		}
	}
	
	return nRet;
}

int CCfgdll_mngr::GetLogmngr(char *cLogPath, int &nLogLevel, int &nLogSize)
{
	int nRet = RES_SUCCESS;
	CFG_ITEM *pCfgitem = NULL;

	if (NULL == cLogPath)
	{
		return RES_FAIL;
	}
	if (NULL == m_pFun_cfgmngr_getitemval)
	{
		return RES_FAIL;
	}

	//日志文件路径
	pCfgitem = m_pFun_cfgmngr_getitemval("logfile","log_path");
	if (NULL == pCfgitem)
	{
		printf("cfgmngr_getitemval失败!，modelname = logfile, itemname = log_path \n");
		return RES_FAIL;
	}
	strcpy(cLogPath, pCfgitem->ccfgitemvalue);
	
	//日志文件最大值
	pCfgitem = m_pFun_cfgmngr_getitemval("logfile","log_length");
	if (NULL == pCfgitem)
	{
		printf("cfgmngr_getitemval失败!，modelname = logfile, itemname = log_length \n");
		return RES_FAIL;
	}
	nLogSize = atoi(pCfgitem->ccfgitemvalue);
	
	//日志文件级别
	pCfgitem = m_pFun_cfgmngr_getitemval("logfile","log_level");
	if (NULL == pCfgitem)
	{
		printf("modelname = logfile, itemname = log_level \n");
		return RES_FAIL;
	}
	nLogLevel = atoi(pCfgitem->ccfgitemvalue);

	return nRet;
}

/**
* @brief	function name:"DeleteBeginChar"
* @use		从源字符串数组cSrc开头删除所有字符cDel，并存入目的字符串数组cDest中
* @param 	[type] char* [name] cSrc 源字符串数组
* @param 	[type] char* [name] cDest 目的字符串数组
* @param 	[type] char [name] cDel 需要删除的字符
* @return	TRUE 成功， FALSE 失败
*/
BOOL CCfgdll_mngr::DeleteBeginChar(char *cSrc, char *cDest, char cDel)
{
	BOOL bRet = TRUE;
	char cTmp[10];
	CXJString strTmp="";

	//判断参数是否合法
	if (NULL==cSrc || NULL==cDest)
	{
		return FALSE;
	}

	memset(&cTmp,0,10);
	cTmp[0] = cDel;
	strcpy(cDest, cSrc);

	strTmp = cDest;
	while (strTmp.Left(1) == cTmp)
	{
		strTmp = cDest;
		strTmp = strTmp.Right(strTmp.length()-1);
		strcpy(cDest,(char*)strTmp.c_str());
	}

	return bRet;
}

/**
* @brief	function name:"DeleteEndChar"
* @use		从源字符串数组cSrc结尾删除所有字符cDel，并存入目的字符串数组cDest中
* @param 	[type] char* [name] cSrc 源字符串数组
* @param 	[type] char* [name] cDest 目的字符串数组
* @param 	[type] char [name] cDel 需要删除的字符
* @return	TRUE 成功， FALSE 失败
*/
BOOL CCfgdll_mngr::DeleteEndChar(char *cSrc, char *cDest, char cDel)
{
	BOOL bRet = TRUE;
	char	*psymbol=NULL;
	char cTmp[10];

	//判断参数是否合法
	if (NULL==cSrc || NULL==cDest)
	{
		return FALSE;
	}

	memset(&cTmp,0,10);
	cTmp[0] = cDel;
	strcpy(cDest, cSrc);
	
	psymbol = strrchr(cDest, cDel);
	while (psymbol)
	{
		if (strcmp(psymbol, cTmp)==0)
		{
			*psymbol = '\0';//去掉最后的面的'/'
		}
		else
		{
			break;
		}

		psymbol = strrchr(cDest, cDel);
	}

	return bRet;
}

/**
* @brief	function name:"DelCharPath"
* @use		设置m_prun_param->cLogPath
** @param 	[type] char* [name] c_XmlLogPaht XML配置文件中的日志文件路径
* @return	RES_SUCCESS 成功， RES_FAIL 失败
*/
int CCfgdll_mngr::DelCharPath(char *c_XmlLogPath, char *cResult)
{
	//判断参数是否合法
	if (NULL==c_XmlLogPath || NULL==cResult)
	{
		return RES_FAIL;
	}

	int nRet = RES_SUCCESS;

	char cTmpLogPath[256];
	char cLogPath[256];
	CXJString strTmp="";

	memset(&cTmpLogPath,0,256);
	memset(&cLogPath,0,256);

	strTmp = c_XmlLogPath;
	if (strTmp.Left(3) != "../")
	{
		//删除开头的'.'
		DeleteBeginChar(c_XmlLogPath, cTmpLogPath,'.');
		//删除开头的'/'
		DeleteBeginChar(cTmpLogPath, cLogPath,'/');
		//删除末尾的'/'
		DeleteEndChar(cLogPath, cTmpLogPath,'/');
		strcpy(cLogPath,"./");
		strcat(cLogPath,cTmpLogPath);
		
	}
	else
	{
		//删除末尾的'/'
		DeleteEndChar(c_XmlLogPath, cLogPath,'/');
	}
	
	strcpy(cResult, cLogPath);

	return nRet;
}

STORAGE_STRUCT * CCfgdll_mngr::Getstoragegroup(int &nGroup)
{
	STORAGE_STRUCT *pstoragegroup = NULL;
	int i,j;
	char cmodel[max_storage_len];
	char cNum[10];
	int nstorage = 0;
	CFG_ITEM *pCfgitem = NULL;
	char cItemname[max_storage_len];
	char cStoragename[max_storage_len];

	map<int, vector<string> > map_group_storage;
	map<int, vector<string> >::iterator Ite;

	if (NULL == m_pFun_cfgmngr_getitemval)
	{
		return NULL;
	}

	for (i=0; i<5; i++)
	{
		memset(cNum, 0, sizeof(cNum));
		sprintf(cNum, "%d", i+1);

		memset(cmodel, 0, sizeof(cmodel));
		strcpy(cmodel, "storagegroup_");
		strcat(cmodel, cNum);

		pCfgitem = m_pFun_cfgmngr_getitemval(cmodel, "num");
		if (NULL != pCfgitem)
		{
			nstorage = atoi(pCfgitem->ccfgitemvalue);
			nGroup++;
		}

		for (j=0; j<nstorage; j++)
		{
			memset(cNum, 0, sizeof(cNum));
			sprintf(cNum, "%d", j);
			memset(cItemname,0,sizeof(cItemname));
			strcpy(cItemname, "snifferdata_store_");
			strcat(cItemname, cNum);

			pCfgitem = m_pFun_cfgmngr_getitemval(cmodel, cItemname);
			if (NULL != pCfgitem)
			{
				memset(cStoragename, 0, sizeof(cStoragename));
				strcpy(cStoragename, pCfgitem->ccfgitemvalue);
				strcat(cStoragename, "/");
				map_group_storage[nGroup-1].push_back(cStoragename);
			}
		}
		

		
	}

	i = 0;
	pstoragegroup = new STORAGE_STRUCT[nGroup];
 	Ite = map_group_storage.begin();
 	while (Ite != map_group_storage.end())
 	{
		pstoragegroup[i].n_storage = Ite->second.size();
		pstoragegroup[i].c_storage = new char[pstoragegroup[i].n_storage][max_storage_len];
		
		for (j=0; j<pstoragegroup[i].n_storage; j++)
		{
			strcpy(pstoragegroup[i].c_storage[j], (char*)Ite->second[j].c_str());
		}
 
 		Ite++;
		i++;
 	}
	
	map_group_storage.clear();

	return pstoragegroup;
}

int CCfgdll_mngr::releaseArrays(STORAGE_STRUCT *pstoragegroup, int nGroup)
{
	int nRet = RES_SUCCESS;
	int i;

	if (NULL == pstoragegroup)
	{
		return RES_FAIL;
	}

	for (i=0; i<nGroup; i++)
	{
		delete[] pstoragegroup[i].c_storage;
		pstoragegroup[i].c_storage = NULL;
	}

	delete[] pstoragegroup;
	pstoragegroup = NULL;
	
	return nRet;
}

CFG_ITEM* CCfgdll_mngr::GetItemVal(const char* c_model_name, const char* c_item_name)
{
	CFG_ITEM *pCfgItem = NULL;
	if (NULL==c_model_name || NULL==c_item_name)
	{
		return NULL;
	}
	if (NULL == m_pFun_cfgmngr_getitemval)
	{
		return NULL;
	}

	pCfgItem = m_pFun_cfgmngr_getitemval(c_model_name, c_item_name);
	
	return pCfgItem;

}

SCANNER* CCfgdll_mngr::GetSysModel(void)
{
	SCANNER *pModel = NULL;
	if (NULL == m_pFun_cfgmngr_get_sysmodel)
	{
		return NULL;
	}

	pModel = m_pFun_cfgmngr_get_sysmodel();

	return pModel;

}

SNIFFER_PORT** CCfgdll_mngr::GetSnifferPortLst(int &nList)
{

	SNIFFER_PORT** pPortLst = NULL;
	if (NULL == m_pFun_cfgmngr_get_portlst)
	{
		return NULL;
	}

	pPortLst = m_pFun_cfgmngr_get_portlst(nList);
	if (NULL == pPortLst)
	{
		return NULL;
	}

	return pPortLst;
}

SNIFFER_APP*  CCfgdll_mngr::Get_ied_bynetidaddr(char* cNetaddr)
{
	if (NULL==cNetaddr)
		return NULL;

	if (NULL==m_pFun_cfgmngr_get_ied_byaddr)
	{
		return NULL;
	}

	return m_pFun_cfgmngr_get_ied_byaddr(cNetaddr);

}

SNIFFER_APP*  CCfgdll_mngr::Get_ied_byappid(int nappid)
{
	if (NULL==m_pFun_cfgmngr_get_ied_byappid)
	{
		return NULL;
	}
	
	return m_pFun_cfgmngr_get_ied_byappid(nappid);
}

SNIFFER_APP*  CCfgdll_mngr::Get_ied_bysysid(int nsysid)
{
	if (NULL==m_pFun_cfgmngr_get_ied_bysysid)
	{
		return NULL;
	}
	
	return m_pFun_cfgmngr_get_ied_bysysid(nsysid);
}

SNIFFER_PORT* CCfgdll_mngr::Get_port(int nsys_portseq)
{
	if (NULL==m_pFun_cfgmngr_get_port)
	{
		return NULL;
	}

	return m_pFun_cfgmngr_get_port(nsys_portseq);
}

APP_CHANNEL_RELATION* CCfgdll_mngr::get_app_4y(int nsys_id)
{
	if (NULL==m_pFun_cfgmngr_get_app_4y)
	{
		return NULL;
	}

	return m_pFun_cfgmngr_get_app_4y(nsys_id);
}

SNIFFER_CARD** CCfgdll_mngr::Get_FirstSniffer_CardLst(int &nList)
{
	if (NULL==m_pFun_cfgmngr_get_sysmodel)
	{
		return NULL;
	}

//	int i;
	SNIFFER_CARD** pSniffer_card = NULL;
	SNIFFER** pSnifferLst = NULL;
	SCANNER* pScanner= m_pFun_cfgmngr_get_sysmodel();
	if (NULL==pScanner)
	{
		return NULL;
	}

	pSnifferLst = pScanner->plisthead_sniffer;//第一台采集装置
	if (pSnifferLst != NULL)
	{
		nList += pSnifferLst[0]->ncardnum;
		pSniffer_card = pSnifferLst[0]->plisthead_sniffercard;
	}	

	return pSniffer_card;
}