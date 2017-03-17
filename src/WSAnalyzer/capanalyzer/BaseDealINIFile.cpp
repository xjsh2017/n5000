// DealINIFile.cpp: implementation of the CDealINIFile class.
//
//////////////////////////////////////////////////////////////////////

#include "BaseDealINIFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealINIFile::CDealINIFile()
{
	m_line_no   =0;   
	m_max_alloc_line=0;
	for (int i = 0 ; i < MAX_MEMORY_LINE; i++)
	{
		m_MemLine[i] = NULL;
	}
}

CDealINIFile::~CDealINIFile()
{
	m_line_no   =0;   
	m_max_alloc_line=0;
}

int CDealINIFile::Split(char* source,char* dest[])   
{   
	char* str = source;   
	int cnt  =  0;   
	if (*str == '#')   
	{   
		return   -1;   
	}   
	
	char* tmp =  new  char[MAX_STRING_LEN];   
	dest[cnt] = tmp;   
	bzero(dest[cnt],MAX_STRING_LEN);     
	while (*str != '\0')   
	{   
		if(*str != '=')   
		{   
			*tmp = *str;   
			tmp ++;   
			str ++;   
		}   
		else   
		{   
			str ++;   
			cnt ++;   
			tmp = new char[MAX_STRING_LEN];   
			dest[cnt] = tmp;   
			bzero(dest[cnt], MAX_STRING_LEN);   
			strcpy(dest[cnt], str);     
			break;     
		}   
	}   
	if (cnt == 0)   
	{   
		delete dest[0];   
	}
	
	return cnt;   
}   

char* CDealINIFile::LTrim(char* source)   
{   
	char* tmp = source;   
	while(*tmp == ' ')   
	{   
		tmp ++;   
	}
	
	return   tmp;   
}   

char* CDealINIFile::RTrim(char* source)   
{   
	int  nLen = strlen(source);   
	char* tmp = source;   
	char* lp = tmp + nLen - 1;   
	int  n = 0;   
    
	while (*lp == ' ' || *lp == '\n' || *lp =='\0' || *lp == 0x0D)   
	{   
		n++;   
		lp--;   
	} 
	
	nLen = nLen - n;   
	tmp[nLen] = '\n';   
	tmp[nLen+1] = '\0';
	
	return tmp;   
}   

bool CDealINIFile::OpenFile()   
{   
	FILE* fp;   
	if ((fp = fopen(m_szFilename, "a+")) == NULL)   
	{   
		m_errno  =  errno;   
		return   false;   
	} 
	
	m_fp   =   fp;   
	rewind(m_fp);   
	return   true;   
}   

bool CDealINIFile::CloseFile()   
{   
	if(m_fp == NULL)   
	{   
		return   false;   
	}   
	else   
	{   
		if (fclose(m_fp) != 0)   
		{   
			m_errno  =  errno;   
			return  false;   
		}   
	} 
	
	return true;   
}   

void  CDealINIFile::ReadFileToMemory()   
{   
	char  lpszbuf[MAX_STRING_LEN];     
	m_max_alloc_line = 0;
	
	while (fgets(lpszbuf,MAX_STRING_LEN,m_fp)   !=   NULL)   
	{   
		if (IsSection(lpszbuf)||IsKeyValue(lpszbuf))   
		{
			m_MemLine[m_max_alloc_line]  = new char[MAX_STRING_LEN];   
			strcpy(m_MemLine[m_max_alloc_line], LTrim(RTrim(lpszbuf)));   
			m_max_alloc_line ++;   
		}   
	}  
	
	CloseFile();   
	
}   

void   CDealINIFile::WriteMemoryToFile()   
{   
	int i = 0;  
	FILE *fp = NULL;
	if ((fp = fopen (m_szFilename,"w+")) == NULL)   
	{   
		m_errno   =   errno;   
		return;   
	}
	
	m_fp = fp;   
	
	while(i < m_max_alloc_line)   
	{   
		fputs(m_MemLine[i],m_fp);   
		i++;   
	}
	
	fclose(m_fp);
	
	for(i = 0; i < m_max_alloc_line ; i++)   
	{   
		delete m_MemLine[i];   
	}   
}   

void   CDealINIFile::AddLine(int   lineno)   
{   
	int   lineend   =   m_max_alloc_line-1;     
	m_MemLine[m_max_alloc_line++] = new char[MAX_STRING_LEN];   
	while   (lineend >= lineno)   
	{   
		bzero(m_MemLine[lineend+1],MAX_STRING_LEN);   
		strcpy(m_MemLine[lineend+1],m_MemLine[lineend]);   
		lineend   --;   
	} 
	
	bzero(m_MemLine[lineno], MAX_STRING_LEN);   
}   

int  CDealINIFile::FindKeyInSection()   
{   
	int lineno = FindSection();   
	if (lineno != -1)   
	{
		lineno ++;
		while (m_MemLine[lineno] != NULL && (*m_MemLine[lineno] != '['))   
		{   
			char* tmp[2];   
			if (Split(m_MemLine[lineno],tmp)>0)   
			{   
				if (strcmp(m_szKey,tmp[0])==0)   
				{   
					bzero(m_MemLine[lineno],MAX_STRING_LEN);   
					delete   tmp[0];   
					delete   tmp[1];   
					return   lineno;   
				}   
			}   
			delete   tmp[0];   
			delete   tmp[1];   
			if ( m_MemLine[lineno+1] == NULL)   
			{   
				lineno ++;
				break;   
			}
			
			lineno ++;
		}   
	}   
	else   
	{   
		m_errno   =   FILENAME_SECTION_NOT_FOUND;   
		return   -1;   
	} 
	
	AddLine(lineno);   
	return   lineno;   
}   

void   CDealINIFile::AddKeyValue(int   lineno)   
{   
	if (m_MemLine[lineno] != NULL)   
	{   
		bzero(m_MemLine[lineno],MAX_STRING_LEN);   
	}   
	else   
	{   
		m_MemLine[lineno] = new char[MAX_STRING_LEN];   
	}
	
	sprintf(m_MemLine[lineno],"%s=%s\n",m_szKey,m_szValue);   
}   

void CDealINIFile::AddSection()   
{   
	m_MemLine[m_max_alloc_line] = new char[MAX_STRING_LEN];   
	sprintf(m_MemLine[m_max_alloc_line], "[%s]\n", m_szSection);   
	m_max_alloc_line ++;   
	AddKeyValue(m_max_alloc_line);   
	m_max_alloc_line++;   
}   

int   CDealINIFile::FindSection()   
{   
	int   i;   
	char   szSec[MAX_STRING_LEN];   
	bzero(szSec,MAX_STRING_LEN);   
	sprintf(szSec,"[%s]\n",m_szSection);
	
	for (i = 0; i < m_max_alloc_line; i++)   
	{      
		if (strcmp(szSec,m_MemLine[i]) == 0)   
		{   
			return i;   
		}   
	}   
	return -1;   
}   

bool   CDealINIFile::WritePrivateProfile(const char*   szSection,   const char*   szKey,   const char*   szValue,   const char*   szFilename)
{   
	if (szFilename== NULL || szSection == NULL || szKey == NULL || szValue == NULL)   
	{   
		m_errno = FILENAME_SECTION_KEY_NULL;   
		return false;   
	}   
    
	if (strcmp(szFilename,"") == 0 || strcmp(szSection,"") == 0 || strcmp(szKey,"") == 0)   
	{   
		m_errno   =   FILENAME_SECTION_KEY_EMPTY;   
		return   false;   
	}   
    m_szFilename   = new char[LEN((char*)szFilename) + 1];
	m_szSection = new char[LEN(szSection) + 1];   
	m_szKey   = new char[LEN(szKey) + 1];   
	m_szValue  = new char[LEN(szValue) + 1];   
    
	strcpy(m_szFilename,szFilename);   
	strcpy(m_szSection,szSection);   
	strcpy(m_szKey,szKey);   
	strcpy(m_szValue,szValue);   
    
	if   (!OpenFile())   
	{   
		FreeMemory();   
		return   false;   
	}   

	ReadFileToMemory();   

	if (FindSection() == -1)   
	{   
		AddSection();   
	}   
	else   
	{   
		int nRet = FindKeyInSection();   
		if (nRet == -1)   
		{   
			return false;   
		}   
		else   
		{   
			AddKeyValue(nRet);   
		}   
		
	}   
	WriteMemoryToFile();   
	FreeMemory();   
	return   true;   
}   

void   CDealINIFile::FreeMemory()   
{   
	if (m_szFilename != NULL)   
	{   
		delete   m_szFilename;   
	}   
	if   (m_szSection != NULL)   
	{   
		delete   m_szSection;   
	}   
	if   (m_szKey != NULL)   
	{   
		delete   m_szKey;   
	}   
	if   (m_szValue != NULL)   
	{   
		delete m_szValue;   
	}   
}   

bool   CDealINIFile::IsSection(char*   source)   
{   
	int   len   =   LEN(LTrim(RTrim(source)));   
	char* lpstart  = LTrim(RTrim(source));   
	char* lpend   =   lpstart   +   len  - 2;   
	if   (*lpstart=='['   &&   *lpend ==']')   
	{   
		return   true;   
	}   
	return   false;   
}   

bool   CDealINIFile::IsKeyValue(char*   source)   
{   
	char*   lp = source;   
	while (*lp != '=')   
	{   
		if (*lp == '\0' || *lp == '\n')   
		{   
			return   false;   
		}   
		lp++;   
	}   
	return   true;   
}   

int CDealINIFile::LEN(const char *pstrLen)
{
	int Len = 0;
	Len = strlen(pstrLen);
	return Len;
}

void CDealINIFile::GetKeyValue(char *pValue, int nlineno, int size)
{

}
