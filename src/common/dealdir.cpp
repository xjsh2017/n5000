
#include "const_scan.h"
#include "dealdir.h"

#ifdef OS_WINDOWS

#include <Windows.h>
#include <sys/types.h> 
#include <sys/stat.h>
/*#include "deletedir.h"*/

bool DeleteFolder(char * lpPath) 
{     
	char szFind[MAX_PATH];     
	WIN32_FIND_DATA FindFileData; 
    strcpy(szFind,lpPath);     
	strcat(szFind,"\\*.*");
    HANDLE hFind=::FindFirstFile(szFind,&FindFileData);     
	if(INVALID_HANDLE_VALUE == hFind)//有趣的返回 
	{  
		return false;
	}         
	while(TRUE) {         
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)         
		{  //判断是否为文件夹        
			if(FindFileData.cFileName[0]!='.')             
			{   
				strcpy(szFind,lpPath);                 
				strcat(szFind,"\\");                 
				strcat(szFind,FindFileData.cFileName);                 
				if(!DeleteFolder(szFind))
					return false;
			}         
		}         
		else      
		{  
			char buf[500];
			memset(buf,0,500);
			strncpy(buf,lpPath,strlen(lpPath));
			strcat(buf,"\\");
			strcat(buf,FindFileData.cFileName);
			if(!DeleteFile(buf))//嵌套搜索删除
				return false;
		}         
		if(!FindNextFile(hFind,&FindFileData))    
			break;     
	}     
	FindClose(hFind);
	RemoveDirectory(lpPath);
	return true;
} 

bool DeleteDir(const char *pDir)
{
	if (pDir == NULL) 
		return false;
	//判断是文件还是文件夹
	struct _stat sfile;
    if(_stat( pDir, &sfile)==0)
	{
		if (_S_IFREG&sfile.st_mode)
		{
			if(DeleteFile(pDir))
				return true;
			else return false;
		}
		if (_S_IFDIR&sfile.st_mode)
		{
			
			if(DeleteFolder((char *)pDir))
				return true;
		}
	}
	return true;
}

#endif //OS_WINDOWS





bool CreateZipDir(const char *cZfname)
{
	bool bRet = true;
	char cZipdir[256];
	char *ptmp = NULL;

	memset(cZipdir,0,sizeof(cZipdir));
	if (NULL==cZfname)
	{
		return false;
	}


	ptmp = strrchr((char*)cZfname, PCHAR_PATH);
	if (NULL==ptmp)
	{
		return false;
	}
	strncpy(cZipdir, cZfname, strlen(cZfname)-strlen(ptmp));

	int i = 0;
	int iRet;
	int iLen = strlen(cZipdir);

	//在末尾加/
	if (cZipdir[iLen - 1] != PCHAR_PATH)
	{
		cZipdir[iLen] = PCHAR_PATH;
		cZipdir[iLen + 1] = '\0';
	}

	// 创建目录
	int index = 0;
#ifdef OS_WINDOWS
	index = 0;
#else
	index = 1;
#endif

	for (i = index;i<iLen+1;i ++)
	{
		if (cZipdir[i] == PCHAR_PATH)
		{ 
			cZipdir[i] = '\0';

			//如果不存在,创建
			iRet = ACCESS(cZipdir,0);
			if (iRet == -1)
			{
				iRet = MKDIR(cZipdir);
				if (iRet != 0)
				{
					return false;
				} 
			}

			cZipdir[i] = PCHAR_PATH;
		} 
	}


	return bRet;
}