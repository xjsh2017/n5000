#include "deallogpath.h"
#include "XJString.h"

/**
* @brief	function name:"DeleteBeginChar"
* @use		从源字符串数组cSrc开头删除所有字符cDel，并存入目的字符串数组cDest中
* @param 	[type] char* [name] cSrc 源字符串数组
* @param 	[type] char* [name] cDest 目的字符串数组
* @param 	[type] char [name] cDel 需要删除的字符
* @return	TRUE 成功， FALSE 失败
*/
BOOL DeleteBeginChar(char *cSrc, char *cDest, char cDel)
{
	bool bRet = TRUE;
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
BOOL DeleteEndChar(char *cSrc, char *cDest, char cDel)
{
	bool bRet = TRUE;
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
* @brief	function name:"convertPath"
* @use		设置m_prun_param->cLogPath
* @param 	[type] char* [name] cSrc 源路径字符
* @param 	[type] char* [name] cDest 目的路径字符
* @return	TRUE 成功， FALSE 失败
*/
BOOL convertPath(char *cSrc, char *cDest)
{
	//判断参数是否合法
	if (NULL==cSrc)
	{
		return FALSE;
	}

	bool bRet = TRUE;

	char cTmpLogPath[256];
	char cLogPath[256];
	CXJString strTmp="";

	memset(&cTmpLogPath,0,256);
	memset(&cLogPath,0,256);

	strTmp = cSrc;
	if (strTmp.Left(3) != "../")
	{
		//删除开头的'.'
		DeleteBeginChar(cSrc, cTmpLogPath,'.');
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
		DeleteEndChar(cSrc, cLogPath,'/');
	}
	
// 	strcat(cLogPath, "/ScanZipLog");
// 
// 	strcpy(m_prun_param->cLogPath, cLogPath);

	strcpy(cDest,cLogPath);

	return bRet;
}