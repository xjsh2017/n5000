// UTF8ConverGB2312.h: interface for the CUTF8ConverGB2312 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UTF8CONVERGB2312_H__A74391D8_D705_4269_B8CF_B788CA5C157A__INCLUDED_)
#define AFX_UTF8CONVERGB2312_H__A74391D8_D705_4269_B8CF_B788CA5C157A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// #include <afxwin.h>         // MFC core and standard components
// #include <afxext.h>         // MFC extensions
// #include <afxdisp.h>        // MFC Automation classes
// #include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//#include "glbtypes.h"


class CUTF8ConverGB2312  
{
public:
	CUTF8ConverGB2312();
	virtual ~CUTF8ConverGB2312();
public:
	/*将16进制数转换成2进制*/
	CString HexToBin(CString string);
	/*将2进制数转换成16进制*/
	CString BinToHex(CString BinString);
	/*2进制字符数据转换成10进制整型*/
	int BinToInt(CString string);
	/*把UTF-8转换成Unicode*/
	WCHAR* UTF_8ToUnicode(char *ustart);
	/*把Unicode 转换成 GB2312*/
	char *  UnicodeToGB2312(unsigned short uData);
	/*GB2312 转换成　Unicode*/
	WCHAR*  Gb2312ToUnicode(char *gbBuffer);
	/*Unicode 转换成UTF-8*/
	char *  UnicodeToUTF_8(WCHAR *UniChar);
	/*将GB2312转换成UTF-8的调用*/
	char *  translateCharToUTF_8(char *xmlStream, int len);
	/*将UTF-8转换成GB2312的调用*/
	char *  TranslateUTF_8ToGB(char *xmlStream, int len);
};
signed int GB2312ToUnicode(const char* szGBString, wchar_t*pBuffer);
char* UnicodeToGB2312(const wchar_t* szUnicodeString);
#endif // !defined(AFX_UTF8CONVERGB2312_H__A74391D8_D705_4269_B8CF_B788CA5C157A__INCLUDED_)
