#include "deallogpath.h"
#include "XJString.h"

/**
* @brief	function name:"DeleteBeginChar"
* @use		��Դ�ַ�������cSrc��ͷɾ�������ַ�cDel��������Ŀ���ַ�������cDest��
* @param 	[type] char* [name] cSrc Դ�ַ�������
* @param 	[type] char* [name] cDest Ŀ���ַ�������
* @param 	[type] char [name] cDel ��Ҫɾ�����ַ�
* @return	TRUE �ɹ��� FALSE ʧ��
*/
BOOL DeleteBeginChar(char *cSrc, char *cDest, char cDel)
{
	bool bRet = TRUE;
	char cTmp[10];
	CXJString strTmp="";

	//�жϲ����Ƿ�Ϸ�
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
* @use		��Դ�ַ�������cSrc��βɾ�������ַ�cDel��������Ŀ���ַ�������cDest��
* @param 	[type] char* [name] cSrc Դ�ַ�������
* @param 	[type] char* [name] cDest Ŀ���ַ�������
* @param 	[type] char [name] cDel ��Ҫɾ�����ַ�
* @return	TRUE �ɹ��� FALSE ʧ��
*/
BOOL DeleteEndChar(char *cSrc, char *cDest, char cDel)
{
	bool bRet = TRUE;
	char	*psymbol=NULL;
	char cTmp[10];

	//�жϲ����Ƿ�Ϸ�
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
			*psymbol = '\0';//ȥ���������'/'
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
* @use		����m_prun_param->cLogPath
* @param 	[type] char* [name] cSrc Դ·���ַ�
* @param 	[type] char* [name] cDest Ŀ��·���ַ�
* @return	TRUE �ɹ��� FALSE ʧ��
*/
BOOL convertPath(char *cSrc, char *cDest)
{
	//�жϲ����Ƿ�Ϸ�
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
		//ɾ����ͷ��'.'
		DeleteBeginChar(cSrc, cTmpLogPath,'.');
		//ɾ����ͷ��'/'
		DeleteBeginChar(cTmpLogPath, cLogPath,'/');
		//ɾ��ĩβ��'/'
		DeleteEndChar(cLogPath, cTmpLogPath,'/');
		strcpy(cLogPath,"./");
		strcat(cLogPath,cTmpLogPath);
		
	}
	else
	{
		//ɾ��ĩβ��'/'
		DeleteEndChar(cSrc, cLogPath,'/');
	}
	
// 	strcat(cLogPath, "/ScanZipLog");
// 
// 	strcpy(m_prun_param->cLogPath, cLogPath);

	strcpy(cDest,cLogPath);

	return bRet;
}