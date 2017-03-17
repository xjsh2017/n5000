#ifndef DEAL_LOG_PATH_H
#define DEAL_LOG_PATH_H

#include "define.h"

/**
* @brief	function name:"DeleteBeginChar"
* @use		��Դ�ַ�������cSrc��ͷɾ�������ַ�cDel��������Ŀ���ַ�������cDest��
* @param 	[type] char* [name] cSrc Դ�ַ�������
* @param 	[type] char* [name] cDest Ŀ���ַ�������
* @param 	[type] char [name] cDel ��Ҫɾ�����ַ�
* @return	TRUE �ɹ��� FALSE ʧ��
*/
BOOL DeleteBeginChar(char *cSrc, char *cDest, char cDel);


/**
* @brief	function name:"DeleteEndChar"
* @use		��Դ�ַ�������cSrc��βɾ�������ַ�cDel��������Ŀ���ַ�������cDest��
* @param 	[type] char* [name] cSrc Դ�ַ�������
* @param 	[type] char* [name] cDest Ŀ���ַ�������
* @param 	[type] char [name] cDel ��Ҫɾ�����ַ�
* @return	TRUE �ɹ��� FALSE ʧ��
*/
BOOL DeleteEndChar(char *cSrc, char *cDest, char cDel);


/**
* @brief	function name:"convertPath"
* @use		����m_prun_param->cLogPath
* @param 	[type] char* [name] cSrc Դ·���ַ�
* @param 	[type] char* [name] cDest Ŀ��·���ַ�
* @return	TRUE �ɹ��� FALSE ʧ��
*/
BOOL convertPath(char *cSrc, char *cDest);





#endif//DEAL_LOG_PATH_H