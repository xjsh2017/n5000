#ifndef ZIP_C_H_I_INCLUDE
#define ZIP_C_H_I_INCLUDE

#include "share_scan.h"

#ifdef OS_WINDOWS

#ifdef DLL_FILE_ZIP_C
class __declspec(dllexport) CZIP_I //导出类

#else 
class __declspec(dllimport) CZIP_I //导入类

#endif /*DLL_FILE_ZIP_C*/
#endif /*OS_WINDOWS*/


#ifdef OS_LINUX
class CZIP_I
#endif /*OS_LINUX*/

{

protected:
	CZIP_I(){};
	
public:

	/*	初始化，打开日志文件，调用该库其他接口前必须先执行调用接口
	参数：
		无

	返回：
		0：成功
		1：失败
	*/
	virtual int zipc_init(int para)=0;

	/******************************************************zlib算法***********************************************************/

	/*	zlib算法 压缩一个或多个文件和目录
	参数：
		cZipfile：压缩后生成的zip文件（包括全路径，不能为NULL；如：/home/nbpt/test.zip
		cSrcfiles：将要被压缩的文件或路径列表，不能为NULL，多个文件或路径用“|”间隔；如：/home/nbpt/zipfiles/test.txt|/home/nbpt/zipfiles/test1.txt
		cPassword：压缩密码，不能为NULL，但可以为空字符串""
		nZlevel：压缩等级
		bDel：是否删除源文件(或源目录)

	返回：
		0：成功
		1：失败
	*/
	virtual int zipc_zip(const char *cZipfile, const char *cSrcfiles,const char* cPassword,int nZlevel,bool bDel)=0;


	/*	zlib算法 解压缩
	参数：
		cZipfile：将要被解压的压缩文件（包括全路径）；
		cDestdir：解压的目的路径；如果该参数为空，则解压到系统的当前路径；
		cPassword：解压密码，可以为NULL
		bDel：是否删除源文件
		
	返回：
		0：成功
		1：失败
	*/
	virtual int zipc_unzip(const char *cZipfile, const char *cDestdir,const char* cPassword,bool bDel)=0;

	/*	zlib算法 压缩内存
	参数：
		dest：保存压缩结果的缓存
		destLen：压缩结果的长度
		source：待压缩缓存
		sourceLen：待压缩缓存长度
	
	返回：
		0：成功
		1：失败
	*/
	virtual int zipc_compress_buffer(unsigned char *dest,unsigned long *destLen,const unsigned char *source, unsigned long sourceLen)=0;

	/* zlib算法压缩内存
	参数：
		dest：保存压缩结果的缓存
		destLen：压缩结果的长度
		source：待压缩缓存
		sourceLen：待压缩缓存长度
		level：压缩等级，压缩等级越高，压缩比越大，但压缩速度越低。等级为1至9
	
	返回：
		0：成功
		1：失败
	*/
	virtual int zipc_compress2_buffer(unsigned char *dest, unsigned long *destLen,const unsigned char *source, unsigned long sourceLen,int level)=0;

	/* zlib算法 根据待压缩内存长度获取压缩目的内存建议的长度
	参数：
		sourceLen：待压缩缓存的长度
	
	返回：sourceLen + (sourceLen >> 12) + (sourceLen >> 14) + 11
	*/
	virtual unsigned long zipc_compress_Bound(unsigned long sourceLen)=0;

	/* zlib算法 解压缩内存
	参数：
		dest：保存解压缩结果的缓存
		destLen：解压缩结果的长度
		source：待解压缩缓存
		sourceLen：待解压缩缓存长度
	
	返回：
		0：成功
		1：失败
	*/
	virtual int zipc_uncomress_buffer(unsigned char *dest, unsigned long *destLen,const unsigned char *source, unsigned long sourceLen)=0;

	/* zlib算法 解压缩内存，并保存到文件，压缩等级为6
	参数：
		dest_fname：保存解压缩结果的文件名
		source：待解压缩缓存
		sourceLen：待解压缩缓存长度
	
	返回：
		0：成功
		1：失败
	*/
	virtual int zipc_compress_buf2file(const char*dest_fname,const unsigned char *source, unsigned long sourceLen)=0;

	/* zlib算法 解压缩内存，并保存到文件
	参数：
		dest_fname：保存解压缩结果的文件名
		source：待解压缩缓存
		sourceLen：待解压缩缓存长度
		level：压缩等级，1至9，压缩等级越高，压缩速度越慢
	
	返回：
		0：成功
		1：失败
	*/
	virtual int zipc_compress2_buf2file(const char*dest_fname,const unsigned char *source, unsigned long sourceLen,int level)=0;

	/***********************************************************minilzo算法*******************************************************************************************/
	/*	minilzo算法初始化，调用minilzo算法的接口前必须先调用该接口
	参数：
		无

	返回：
		0：成功
		1：失败
	*/	
	virtual int zipc_lzo_init()=0;

	/* minilzo算法 根据待压缩内存长度获取压缩目的内存建议的长度
	参数：
		sourceLen：待压缩缓存的长度
	
	返回：sourceLen + sourceLen / 16 + 64 + 3
	*/
	virtual unsigned long zipc_lzo1x_compress_Bound(unsigned long sourceLen)=0;

	/*	minilzo算法 压缩内存
	参数：
		src：待压缩缓存
		src_len：待压缩缓存长度
		dst：保存压缩结果的缓存
		dst_len：压缩结果的长度
		wrkmem：压缩空间，必须非NULL
	
	返回：
		0：成功
		1：失败
	*/
	virtual int zipc_lzo1x_1_compress(const unsigned char* src, unsigned long src_len, unsigned char* dst, unsigned long* dst_len, void* wrkmem)=0;

	/*	minilzo算法 解压内存（不带格式安全验证）
	参数：
		src：待解压缩缓存
		src_len：待解压缩缓存长度
		dst：保存解压缩结果的缓存
		dst_len：解压缩结果的长度
		wrkmem：解压缩空间，该参数没有用，传入NULL即可
	
	返回：
		0：成功
		1：失败
	*/
	virtual int zipc_lzo1x_decompress(const unsigned char* src, unsigned long src_len, unsigned char* dst, unsigned long* dst_len, void* wrkmem=NULL/* NOT USED */)=0;
	
	/*	minilzo算法 解压内存（带格式安全验证）
	参数：
		src：待解压缩缓存
		src_len：待解压缩缓存长度
		dst：保存解压缩结果的缓存
		dst_len：解压缩结果的长度
		wrkmem：解压缩空间，该参数没有用，传入NULL即可
	
	返回：
		0：成功
		1：失败
	*/	
	virtual int zipc_lzo1x_decompress_safe(const unsigned char* src, unsigned long src_len, unsigned char* dst, unsigned long* dst_len, void* wrkmem=NULL/* NOT USED */)=0;


	int m_para;
};


#ifdef OS_WINDOWS

#ifdef DLL_FILE_ZIP_C

//导出函数
extern "C"  __declspec( dllexport ) CZIP_I * createZipc_I(LOG_STRUCT logInfo);
extern "C"  __declspec( dllexport ) void destroyZipc_I(CZIP_I* theInterface);

#else 

//导入函数
extern "C"  __declspec( dllimport ) CZIP_I * createZipc_I(LOG_STRUCT logInfo);
extern "C"  __declspec( dllimport ) void destroyZipc_I(CZIP_I* theInterface);


#endif /*DLL_FILE_ZIP_C*/
#endif /*OS_WINDOWS*/

#ifdef OS_LINUX
#ifdef __cplusplus
extern "C"
{
#endif
	
	CZIP_I * createZipc_I(LOG_STRUCT logInfo);
	void destroyZipc_I(CZIP_I* theInterface);
	
#ifdef __cplusplus
}
#endif

#endif /*OS_LINUX*/



#endif /*ZIP_C_H_I_INCLUDE*/
