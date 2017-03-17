// DealINIFile.h: interface for the CDealINIFile class.
//
//////////////////////////////////////////////////////////////////////
// 
// #if !defined(AFX_DEALINIFILE_H__23C8F8F8_3887_43D6_93BB_6FF598B12C6B__INCLUDED_)
// #define AFX_DEALINIFILE_H__23C8F8F8_3887_43D6_93BB_6FF598B12C6B__INCLUDED_
// 
// #if _MSC_VER > 1000
// #pragma once
// #endif // _MSC_VER > 1000
#pragma once
// #include "share.h"
 #include <stdio.h>  
 #include <stdlib.h>  
// #include <time.h>
// #include <signal.h>
 #include <errno.h>
 #include <string.h>  
// #include <ctype.h>
// #include <stddef.h>
// #include <stdarg.h>
#define bzero(ptr,n) memset(ptr,0,n)
const int MAX_STRING_LEN = 256;
const int FILENAME_SECTION_KEY_EMPTY = 1;
const int FILENAME_SECTION_NOT_FOUND = 2;
const int FILENAME_SECTION_KEY_NULL = 3;
const int MAX_MEMORY_LINE = 2048;
//

class CDealINIFile
{
	
public:
	CDealINIFile();
	virtual ~CDealINIFile();
    bool WritePrivateProfile(const char *szSection,   const char *szKey,   const char *szValue,   const char*   szFilename);

private:
	int Split(char* source,char* dest[]);
	char* LTrim(char* source);
	char* RTrim(char* source);
	bool OpenFile();
	bool CloseFile(); 
	void ReadFileToMemory();
	void WriteMemoryToFile();
	void AddLine(int lineno);
	int FindKeyInSection();
	void AddKeyValue(int lineno);
	void AddSection();
	int FindSection();
	void FreeMemory();
	bool IsSection(char* source);
	bool IsKeyValue(char* source);
    int LEN(const char *pstrLen);
	
private:
	void GetKeyValue(char *pValue, int nlineno, int size);
	int m_errno;
	int m_line_no;   
	int m_max_alloc_line;
	FILE *m_fp;
	char *m_MemLine[MAX_MEMORY_LINE];
	char *m_szSection;
	char *m_szFilename;
	char *m_szKey;
	char *m_szValue;
};

//#endif // !defined(AFX_DEALINIFILE_H__23C8F8F8_3887_43D6_93BB_6FF598B12C6B__INCLUDED_)
