// UTF8ConverGB2312.cpp: implementation of the CUTF8ConverGB2312 class.
//
//////////////////////////////////////////////////////////////////////

#include "UTF8ConverGB2312.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUTF8ConverGB2312::CUTF8ConverGB2312()
{

}

CUTF8ConverGB2312::~CUTF8ConverGB2312()
{

}

CString CUTF8ConverGB2312::HexToBin(CString string)
{

	if( string == "0") return "0000";
	if( string == "1") return "0001";
	if( string == "2") return "0010";
	if( string == "3") return "0011";
	if( string == "4") return "0100";
	if( string == "5") return "0101";
	if( string == "6") return "0110";
	if( string == "7") return "0111";
	if( string == "8") return "1000";
	if( string == "9") return "1001";
	if( string == "a") return "1010";
	if( string == "b") return "1011";
	if( string == "c") return "1100";
	if( string == "d") return "1101";
	if( string == "e") return "1110";
	if( string == "f") return "1111";
	return "";
}

CString CUTF8ConverGB2312::BinToHex(CString BinString)
{

	if( BinString == "0000") return "0";
	if( BinString == "0001") return "1";
	if( BinString == "0010") return "2";
	if( BinString == "0011") return "3";
	if( BinString == "0100") return "4";
	if( BinString == "0101") return "5";
	if( BinString == "0110") return "6";
	if( BinString == "0111") return "7";
	if( BinString == "1000") return "8";
	if( BinString == "1001") return "9";
	if( BinString == "1010") return "a";
	if( BinString == "1011") return "b";
	if( BinString == "1100") return "c";
	if( BinString == "1101") return "d";
	if( BinString == "1110") return "e";
	if( BinString == "1111") return "f";
	return "";
}

int CUTF8ConverGB2312::BinToInt(CString string)
{
	int len =0;
	int tempInt = 0;
	int strInt = 0;
	for(int i =0 ;i < string.GetLength() ;i ++)
	{
		tempInt = 1;
		strInt = (int)string.GetAt(i)-48;
		for(int k =0 ;k < 7-i ; k++)
		{
			tempInt = 2*tempInt;
		}
		len += tempInt*strInt;
	}
	return len;	
}

WCHAR* CUTF8ConverGB2312::UTF_8ToUnicode(char *ustart)
{
//	char char_one;
//	char char_two;
//	char char_three;
//	char uchar[2];
//	WCHAR *unicode;
//	
//	char_one = ustart[0];
//	char_two = ustart[1];
//	char_three = ustart[2];
//	
//	uchar[1] = ((char_one & 0x0F) << 4) + ((char_two >> 2) & 0x0F);
//	uchar[0] = (char_two & 0x03) << 6 + (char_three & 0x3F);
//	
//	unicode = (WCHAR *)uchar;
//	return unicode;

	char char_one;
	char char_two;
	char char_three;
	int Hchar;
	int Lchar;
	char uchar[2];
	WCHAR *unicode;
	CString string_one;
	CString string_two;
	CString string_three;
	CString combiString;
	char_one = *ustart;
	char_two = *(ustart+1);
	char_three = *(ustart+2);
	string_one.Format("%x",char_one);
	string_two.Format("%x",char_two);
	string_three.Format("%x",char_three);
	string_three = string_three.Right(2);
	string_two = string_two.Right(2);
	string_one = string_one.Right(2);
	string_three = HexToBin(string_three.Left(1))+HexToBin(string_three.Right(1));
	string_two = HexToBin(string_two.Left(1))+HexToBin(string_two.Right(1));
	string_one = HexToBin(string_one.Left(1))+HexToBin(string_one.Right(1));
	combiString = string_one +string_two +string_three;
	combiString = combiString.Right(20);
	combiString.Delete(4,2);
	combiString.Delete(10,2);
	Hchar = BinToInt(combiString.Left(8));
	Lchar = BinToInt(combiString.Right(8));
	uchar[1] = (char)Hchar;
	uchar[0] = (char)Lchar;
	unicode = (WCHAR *)uchar;
	return unicode;
}

char *CUTF8ConverGB2312::UnicodeToUTF_8(WCHAR *UniChar)
{
	
	char *buffer;
	CString strOne;
	CString strTwo;
	CString strThree;
	CString strFour;
	CString strAnd;
	buffer = new char[3];
	int hInt,lInt;
	hInt = (int)((*UniChar)/256);
	lInt = (*UniChar)%256;
	CString string ;
	string.Format("%x",hInt);
	strTwo = HexToBin(string.Right(1));
	string = string.Left(string.GetLength() - 1);
	strOne = HexToBin(string.Right(1));
	string.Format("%x",lInt);
	strFour = HexToBin(string.Right(1));
	string = string.Left(string.GetLength() -1);
	strThree = HexToBin(string.Right(1));
	strAnd = strOne +strTwo + strThree + strFour;
	strAnd.Insert(0,"1110");
	strAnd.Insert(8,"10");
	strAnd.Insert(16,"10");
	strOne = strAnd.Left(8);
	strAnd = strAnd.Right(16);
	strTwo = strAnd.Left(8);
	strThree = strAnd.Right(8);
	*buffer = (char)BinToInt(strOne);
	buffer[1] = (char)BinToInt(strTwo);
	buffer[2] = (char)BinToInt(strThree);
	return buffer;
}

char *CUTF8ConverGB2312::UnicodeToGB2312(unsigned short uData)
{
	char *buffer ;
	buffer = new char[sizeof(WCHAR)*100];
	memset(buffer,0,sizeof(WCHAR)*100);
	WideCharToMultiByte(CP_ACP,NULL,(LPCWSTR)&uData,1,buffer,sizeof(WCHAR)*100,NULL,NULL);
	return buffer;
}

WCHAR  *CUTF8ConverGB2312::Gb2312ToUnicode(char *gbBuffer)
{
	WCHAR *uniChar;
	uniChar = new WCHAR[1];
	MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,gbBuffer,2,uniChar,1);
	return uniChar;
}
/*将GB2312转换成UTF-8的调用*/
char *CUTF8ConverGB2312::translateCharToUTF_8(char*xmlStream,int len)
{
	if (NULL==xmlStream || len<=0)
	{
		return NULL;
	}

	int newCharLen =0 ;
	int oldCharLen = 0;
	int revCharLen = len;
	char* newCharBuffer;
	char* finalCharBuffer;
	char *buffer ;
	CString string;
	WCHAR *pbuffer = NULL;
	newCharBuffer = new char[int(1.5*revCharLen)];//设置最大的一个缓冲区
	while(oldCharLen < revCharLen)
	{
		if( *(xmlStream + oldCharLen) >= 0)
		{
			*(newCharBuffer+newCharLen) = *(xmlStream +oldCharLen);
			newCharLen ++;
			oldCharLen ++;
		}//如果是英文直接复制就可以
		else
		{
			pbuffer = this->Gb2312ToUnicode(xmlStream+oldCharLen);
			buffer = this->UnicodeToUTF_8(pbuffer);
			*(newCharBuffer+newCharLen) = *buffer;
			*(newCharBuffer +newCharLen +1) = *(buffer + 1);
			*(newCharBuffer +newCharLen +2) = *(buffer + 2);
			delete pbuffer;
			delete buffer;
			newCharLen += 3;
			oldCharLen += 2;
		}
		
	}
	newCharBuffer[oldCharLen] = '\0';
	CString string1 ;
	string1.Format("%s",newCharBuffer);
	finalCharBuffer = new char[newCharLen+1];
	memcpy(finalCharBuffer,newCharBuffer,newCharLen+1);
	delete [] newCharBuffer;
	return finalCharBuffer;
}

/*将UTF-8转换成GB2312的调用*/
char *CUTF8ConverGB2312::TranslateUTF_8ToGB(char *xmlStream,int len)
{
	if (NULL==xmlStream || len<=0)
	{
		return NULL;
	}
	
	char * newCharBuffer = new char[len]; 
    int index =0; 
    int nCBIndex = 0; 
    while(index < len) 
    { 
        if(xmlStream[index] > 0)     // 如果是GB2312的字符 
        { 
            newCharBuffer[nCBIndex] = xmlStream[index];    //直接复制 
            index += 1;              //源字符串偏移量1 
            nCBIndex += 1;           //目标字符串偏移量1 
        } 
        else                 //如果是UTF-8的字符 
        { 
            WCHAR * Wtemp = this->UTF_8ToUnicode(xmlStream + index);  //先把UTF-8转成Unicode 
            char * Ctemp  = this->UnicodeToGB2312(*Wtemp);//再把Unicode 转成 GB2312 
                    newCharBuffer[nCBIndex] = * Ctemp;         // 复制 
            newCharBuffer[nCBIndex + 1] = *(Ctemp + 1);
			delete Ctemp;

            index += 3;     //源字符串偏移量3 
            nCBIndex += 2;   //目标字符串偏移量2  因为一个中文UTF-8占3个字节，GB2312占两个字节 
        } 
    } 
    newCharBuffer[nCBIndex] = 0; //结束符 
    return newCharBuffer;
}


char * UnicodeToGB2312(const wchar_t* szUnicodeString)
{
	UINT nCodePage = 936; //GB2312
	int nLength=WideCharToMultiByte(nCodePage,0,szUnicodeString,-1,NULL,0,NULL,NULL);
	char* pBuffer=new char[nLength+1];
	WideCharToMultiByte(nCodePage,0,szUnicodeString,-1,pBuffer,nLength,NULL,NULL);
	pBuffer[nLength]=0;
	return pBuffer;
}

signed int GB2312ToUnicode(const char* szGBString, wchar_t*pBuffer)
{
	signed int ret =0;
	UINT nCodePage = 936; //GB2312
	int nLength=MultiByteToWideChar(nCodePage,0,szGBString,-1,NULL,0);
	ret = MultiByteToWideChar(nCodePage,0,szGBString,-1,pBuffer,nLength);
	pBuffer[nLength]=0;
	return ret;
}