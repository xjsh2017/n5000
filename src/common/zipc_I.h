#ifndef ZIP_C_H_I_INCLUDE
#define ZIP_C_H_I_INCLUDE

#include "share_scan.h"

#ifdef OS_WINDOWS

#ifdef DLL_FILE_ZIP_C
class __declspec(dllexport) CZIP_I //������

#else 
class __declspec(dllimport) CZIP_I //������

#endif /*DLL_FILE_ZIP_C*/
#endif /*OS_WINDOWS*/


#ifdef OS_LINUX
class CZIP_I
#endif /*OS_LINUX*/

{

protected:
	CZIP_I(){};
	
public:

	/*	��ʼ��������־�ļ������øÿ������ӿ�ǰ������ִ�е��ýӿ�
	������
		��

	���أ�
		0���ɹ�
		1��ʧ��
	*/
	virtual int zipc_init(int para)=0;

	/******************************************************zlib�㷨***********************************************************/

	/*	zlib�㷨 ѹ��һ�������ļ���Ŀ¼
	������
		cZipfile��ѹ�������ɵ�zip�ļ�������ȫ·��������ΪNULL���磺/home/nbpt/test.zip
		cSrcfiles����Ҫ��ѹ�����ļ���·���б�����ΪNULL������ļ���·���á�|��������磺/home/nbpt/zipfiles/test.txt|/home/nbpt/zipfiles/test1.txt
		cPassword��ѹ�����룬����ΪNULL��������Ϊ���ַ���""
		nZlevel��ѹ���ȼ�
		bDel���Ƿ�ɾ��Դ�ļ�(��ԴĿ¼)

	���أ�
		0���ɹ�
		1��ʧ��
	*/
	virtual int zipc_zip(const char *cZipfile, const char *cSrcfiles,const char* cPassword,int nZlevel,bool bDel)=0;


	/*	zlib�㷨 ��ѹ��
	������
		cZipfile����Ҫ����ѹ��ѹ���ļ�������ȫ·������
		cDestdir����ѹ��Ŀ��·��������ò���Ϊ�գ����ѹ��ϵͳ�ĵ�ǰ·����
		cPassword����ѹ���룬����ΪNULL
		bDel���Ƿ�ɾ��Դ�ļ�
		
	���أ�
		0���ɹ�
		1��ʧ��
	*/
	virtual int zipc_unzip(const char *cZipfile, const char *cDestdir,const char* cPassword,bool bDel)=0;

	/*	zlib�㷨 ѹ���ڴ�
	������
		dest������ѹ������Ļ���
		destLen��ѹ������ĳ���
		source����ѹ������
		sourceLen����ѹ�����泤��
	
	���أ�
		0���ɹ�
		1��ʧ��
	*/
	virtual int zipc_compress_buffer(unsigned char *dest,unsigned long *destLen,const unsigned char *source, unsigned long sourceLen)=0;

	/* zlib�㷨ѹ���ڴ�
	������
		dest������ѹ������Ļ���
		destLen��ѹ������ĳ���
		source����ѹ������
		sourceLen����ѹ�����泤��
		level��ѹ���ȼ���ѹ���ȼ�Խ�ߣ�ѹ����Խ�󣬵�ѹ���ٶ�Խ�͡��ȼ�Ϊ1��9
	
	���أ�
		0���ɹ�
		1��ʧ��
	*/
	virtual int zipc_compress2_buffer(unsigned char *dest, unsigned long *destLen,const unsigned char *source, unsigned long sourceLen,int level)=0;

	/* zlib�㷨 ���ݴ�ѹ���ڴ泤�Ȼ�ȡѹ��Ŀ���ڴ潨��ĳ���
	������
		sourceLen����ѹ������ĳ���
	
	���أ�sourceLen + (sourceLen >> 12) + (sourceLen >> 14) + 11
	*/
	virtual unsigned long zipc_compress_Bound(unsigned long sourceLen)=0;

	/* zlib�㷨 ��ѹ���ڴ�
	������
		dest�������ѹ������Ļ���
		destLen����ѹ������ĳ���
		source������ѹ������
		sourceLen������ѹ�����泤��
	
	���أ�
		0���ɹ�
		1��ʧ��
	*/
	virtual int zipc_uncomress_buffer(unsigned char *dest, unsigned long *destLen,const unsigned char *source, unsigned long sourceLen)=0;

	/* zlib�㷨 ��ѹ���ڴ棬�����浽�ļ���ѹ���ȼ�Ϊ6
	������
		dest_fname�������ѹ��������ļ���
		source������ѹ������
		sourceLen������ѹ�����泤��
	
	���أ�
		0���ɹ�
		1��ʧ��
	*/
	virtual int zipc_compress_buf2file(const char*dest_fname,const unsigned char *source, unsigned long sourceLen)=0;

	/* zlib�㷨 ��ѹ���ڴ棬�����浽�ļ�
	������
		dest_fname�������ѹ��������ļ���
		source������ѹ������
		sourceLen������ѹ�����泤��
		level��ѹ���ȼ���1��9��ѹ���ȼ�Խ�ߣ�ѹ���ٶ�Խ��
	
	���أ�
		0���ɹ�
		1��ʧ��
	*/
	virtual int zipc_compress2_buf2file(const char*dest_fname,const unsigned char *source, unsigned long sourceLen,int level)=0;

	/***********************************************************minilzo�㷨*******************************************************************************************/
	/*	minilzo�㷨��ʼ��������minilzo�㷨�Ľӿ�ǰ�����ȵ��øýӿ�
	������
		��

	���أ�
		0���ɹ�
		1��ʧ��
	*/	
	virtual int zipc_lzo_init()=0;

	/* minilzo�㷨 ���ݴ�ѹ���ڴ泤�Ȼ�ȡѹ��Ŀ���ڴ潨��ĳ���
	������
		sourceLen����ѹ������ĳ���
	
	���أ�sourceLen + sourceLen / 16 + 64 + 3
	*/
	virtual unsigned long zipc_lzo1x_compress_Bound(unsigned long sourceLen)=0;

	/*	minilzo�㷨 ѹ���ڴ�
	������
		src����ѹ������
		src_len����ѹ�����泤��
		dst������ѹ������Ļ���
		dst_len��ѹ������ĳ���
		wrkmem��ѹ���ռ䣬�����NULL
	
	���أ�
		0���ɹ�
		1��ʧ��
	*/
	virtual int zipc_lzo1x_1_compress(const unsigned char* src, unsigned long src_len, unsigned char* dst, unsigned long* dst_len, void* wrkmem)=0;

	/*	minilzo�㷨 ��ѹ�ڴ棨������ʽ��ȫ��֤��
	������
		src������ѹ������
		src_len������ѹ�����泤��
		dst�������ѹ������Ļ���
		dst_len����ѹ������ĳ���
		wrkmem����ѹ���ռ䣬�ò���û���ã�����NULL����
	
	���أ�
		0���ɹ�
		1��ʧ��
	*/
	virtual int zipc_lzo1x_decompress(const unsigned char* src, unsigned long src_len, unsigned char* dst, unsigned long* dst_len, void* wrkmem=NULL/* NOT USED */)=0;
	
	/*	minilzo�㷨 ��ѹ�ڴ棨����ʽ��ȫ��֤��
	������
		src������ѹ������
		src_len������ѹ�����泤��
		dst�������ѹ������Ļ���
		dst_len����ѹ������ĳ���
		wrkmem����ѹ���ռ䣬�ò���û���ã�����NULL����
	
	���أ�
		0���ɹ�
		1��ʧ��
	*/	
	virtual int zipc_lzo1x_decompress_safe(const unsigned char* src, unsigned long src_len, unsigned char* dst, unsigned long* dst_len, void* wrkmem=NULL/* NOT USED */)=0;


	int m_para;
};


#ifdef OS_WINDOWS

#ifdef DLL_FILE_ZIP_C

//��������
extern "C"  __declspec( dllexport ) CZIP_I * createZipc_I(LOG_STRUCT logInfo);
extern "C"  __declspec( dllexport ) void destroyZipc_I(CZIP_I* theInterface);

#else 

//���뺯��
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
