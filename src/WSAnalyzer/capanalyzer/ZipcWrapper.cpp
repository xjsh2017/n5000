#include "ZipcWrapper.h"
#include "xjlib.h"
CZipcWrapper *CZipcWrapper::unzip = NULL;
#ifndef WIN32  
void _splitpath(const char *path, char *drive, char *dir, char *fname, char *ext);  
  
static void _split_whole_name(const char *whole_name, char *fname, char *ext);  
#endif 
CZipcWrapper::CZipcWrapper(void)
{
	m_zipLibHandle = NULL;
	m_pFun_CreateZipc_i = NULL;
	m_pFUn_DestroyZipc_i = NULL;
	memset(m_zipfilesrcbuf,0,1024*1024*50);    //50M
	memset(m_zipfileunzipbuf,0,1024*1024*200);//200M
}


CZipcWrapper::~CZipcWrapper(void)
{
	if(m_zipLibHandle != NULL)
		Uninitialize();

}
int CZipcWrapper::Initialize(const char *cLibPathFullName)
{
	if(NULL == cLibPathFullName)
	{
		return -1;
	}

	try
	{
		//加载库
        m_zipLibHandle = xj_load_library(cLibPathFullName);
		if(NULL == m_zipLibHandle)
		{
            /*
			CString strTemp;
			strTemp.Format("加载动态库:%s失败(错误码：%d，原因：%s)", 
				cLibPathFullName, GetLastError(), strerror(errno));
			AfxMessageBox(strTemp);
            */
			return -1;
		}
		//导入库函数
        m_pFun_CreateZipc_i = (CREATEZIPC_I) xj_get_addr(m_zipLibHandle,"createZipc_I");;
        m_pFUn_DestroyZipc_i = (DESTROYZIPC_I) xj_get_addr(m_zipLibHandle,"destroyZipc_I");;

		if(m_pFun_CreateZipc_i == NULL || m_pFUn_DestroyZipc_i == NULL)
		{
            /*
			CString strTemp;
			strTemp.Format("加载动态库:%s接口函数失败",cLibPathFullName);
			AfxMessageBox(strTemp);
            */
			return -1;
		}
		//调用接口进行初始化
		LOG_STRUCT logSet;
		logSet.nLogLevel = 1;
        strcpy(logSet.cLogPath,"..\\log");
		logSet.nLogMaxLen = 10240;
		m_zipI = m_pFun_CreateZipc_i(logSet);
		if(m_zipI == NULL)
		{
            /*
			CString strTemp;
			strTemp.Format("初始化动态库:%s初始化接口函数调用失败",cLibPathFullName);

			AfxMessageBox(strTemp);
            */
            Uninitialize();
			return -1;
		}
		m_zipI->zipc_init(0);
		m_zipI->zipc_lzo_init();
	}
    catch (...)
	{
        ;
	}
	return 0;
}
int CZipcWrapper::Uninitialize()
{
	if(m_zipI != NULL &&m_pFUn_DestroyZipc_i != NULL)
	{
		m_pFUn_DestroyZipc_i(m_zipI);//释放
	}
	if(m_zipLibHandle)
	{
        xj_free_library(m_zipLibHandle);
	}
	m_zipLibHandle = NULL;
	return 0;
}
/**解压缩**/
int CZipcWrapper::UnZip_I(const char *cZipfile, const char *cDestdir)
{
    /*
	if(m_zipI == NULL)
		return -1;
//读入文件
	FILE* fp = NULL;
	fp = fopen(cZipfile,"rb");
	if(fp == NULL)
		return -1;
	memset(m_zipfilesrcbuf,0,1024*1024*50);    //25M
	memset(m_zipfileunzipbuf,0,1024*1024*200);//100M
	int nCount = 0;
	nCount = fread(m_zipfilesrcbuf,1,1024*1024*50,fp);
	fclose(fp);
	unsigned long lDesSize = 1024*1024*200;
	unsigned long lSrcSize     = (unsigned long)nCount;
//解压缩
	int nResult = m_zipI->zipc_uncomress_buffer(m_zipfileunzipbuf,&lDesSize,m_zipfilesrcbuf,lSrcSize);
//写文件
	char    chDrive[MAX_PATH], chDir[MAX_PATH*2];
    char    chFilename[MAX_PATH];
    char    chExt[6];
    string cMydes;
    _splitpath_s( cZipfile, chDrive, chDir, chFilename, chExt );
    cMydes = chDrive;
    cMydes.append(chDir);
    cMydes.append(chFilename);
    cMydes.append(".pcap");
    //cMydes.Format("%s%s%s%s",chDrive,chDir,chFilename,".pcap");
    fp = fopen(cMydes.c_str(),"wb+");
	int nWrite = fwrite(m_zipfileunzipbuf,1,lDesSize,fp);
    fclose(fp);
    */
	return 0;
}

#ifndef WIN32  
void _splitpath(const char *path, char *drive, char *dir, char *fname, char *ext)  
{  
  char *p_whole_name;  
  
  drive[0] = '\0';  
  if (NULL == path)  
  {  
    dir[0] = '\0';  
    fname[0] = '\0';  
    ext[0] = '\0';  
    return;  
  }  
  
  if ('/' == path[strlen(path)])  
  {  
    strcpy(dir, path);  
    fname[0] = '\0';  
    ext[0] = '\0';  
    return;  
  }  
  
  p_whole_name = (char *)rindex(path, '/');  
  if (NULL != p_whole_name)  
  {  
    p_whole_name++;  
    _split_whole_name(p_whole_name, fname, ext);  
  
    snprintf(dir, p_whole_name - path +1, "%s", path);
  }  
  else  
  {  
    _split_whole_name(path, fname, ext);  
    dir[0] = '\0';  
  }  
}  
  
static void _split_whole_name(const char *whole_name, char *fname, char *ext)  
{  
  char *p_ext;  
  
  p_ext = (char *)rindex(whole_name, '.');  
  if (NULL != p_ext)  
  {  
    strcpy(ext, p_ext);  
    snprintf(fname, p_ext - whole_name + 1, "%s", whole_name);  
  }  
  else  
  {  
    ext[0] = '\0';  
    strcpy(fname, whole_name);  
  }  
}  
  
#endif  
//#include "bzlib.h"
/**minilzo解压缩**/
//BOOL bSafe 解压缩时是否进行格式验证
//磁盘写入的时候为与页面对齐写入，读出文件前4个字节长度，然后进行压缩
int CZipcWrapper::UnZip_MINILZO_I(const char *cZipfile, const char *cDestdir,BOOL bSafe)
{
	if(m_zipI == NULL)
		return -1;
	//读入文件
	FILE* fp = NULL;
	fp = fopen(cZipfile,"rb");
	if(fp == NULL)
		return -1;
	bool bVer_Zip = 0 == strcmp(cZipfile+(strlen(cZipfile)-4),".zip");//20130919之前压缩文件头部不含文件长度，整个是压缩数据，之后改为tzip格式，前面增加文件长度，后面可能含有无效乱码
	bool bVer_Zipx = 0 == strcmp(cZipfile+(strlen(cZipfile)-5),".zipx");
	unsigned long nzipsize_real = 0;
	memset(m_zipfilesrcbuf,0,1024*1024*50);    //50M
	memset(m_zipfileunzipbuf,0,1024*1024*150);//150M
	unsigned long nCount = 0;
	nCount = fread(m_zipfilesrcbuf,1,1024*1024*50,fp);
	memcpy(&nzipsize_real,m_zipfilesrcbuf,4);//读入长度
	fclose(fp);
	if (bVer_Zip)
	{
		nzipsize_real = nCount;
	}
	unsigned long lDesSize = 0;//1024*1024*50;
	unsigned long lSrcSize     = nCount;
	/*src：待解压缩缓存
	src_len：待解压缩缓存长度
	dst：保存解压缩结果的缓存
	dst_len：解压缩结果的长度
	bSafe:是否带格式验证*/
	int nResult = 0;
//  	if(bSafe)
//  		nResult = m_zipI->zipc_lzo1x_decompress_safe(m_zipfilesrcbuf,nCount,m_zipfileunzipbuf,&lDesSize,NULL);//带格式验证
//  	else
//		nResult = m_zipI->zipc_lzo1x_decompress(m_zipfilesrcbuf,nCount,m_zipfileunzipbuf,&lDesSize,NULL);     //不带格式验证
	if (bVer_Zip)
	{
		nResult = m_zipI->zipc_lzo1x_decompress(m_zipfilesrcbuf,nzipsize_real,m_zipfileunzipbuf,&lDesSize,NULL);     //不带格式验证
	}
	else if (bVer_Zipx)
	{
		lDesSize=sizeof(m_zipfileunzipbuf);//m_zipI->zipc_compress_Bound(nzipsize_real);
		nResult= m_zipI->zipc_uncomress_buffer(m_zipfileunzipbuf,&lDesSize,m_zipfilesrcbuf+4,nzipsize_real);
		if (nResult!=0)
		{
			nResult=1;
		}
	}
	else
	{
		nResult = m_zipI->zipc_lzo1x_decompress(m_zipfilesrcbuf+4,nzipsize_real,m_zipfileunzipbuf,&lDesSize,NULL);     //不带格式验证
	}
	
	if(nResult == 1)//失败
		return 1;
	//写文件
	char    chDrive[260], chDir[260*2];
	char    chFilename[260]; 
	char    chExt[6];
    string cMydes;
	_splitpath( cZipfile, chDrive, chDir, chFilename, chExt );
    cMydes = chDrive;
    cMydes.append(chDir);
    cMydes.append(chFilename);
    cMydes.append(".pcap");
    //cMydes.Format("%s%s%s%s",chDrive,chDir,chFilename,".pcap");
    fp = fopen(cMydes.c_str(),"wb+");
	int nWrite = fwrite(m_zipfileunzipbuf,1,lDesSize,fp);
	fclose(fp);
	return 0;
}
/**压缩**/
int CZipcWrapper::Zip_I(const char *cZipfile, const char *cSrcfiles,const char* cPassword,int nZlevel)
{
	if(m_zipI)
		return m_zipI->zipc_zip(cZipfile,cSrcfiles,NULL,1,false);
	return -1;
}
