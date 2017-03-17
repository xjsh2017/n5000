#ifndef DEAL_LOG_PATH_H
#define DEAL_LOG_PATH_H

#include "define.h"

/**
* @brief	function name:"DeleteBeginChar"
* @use		从源字符串数组cSrc开头删除所有字符cDel，并存入目的字符串数组cDest中
* @param 	[type] char* [name] cSrc 源字符串数组
* @param 	[type] char* [name] cDest 目的字符串数组
* @param 	[type] char [name] cDel 需要删除的字符
* @return	TRUE 成功， FALSE 失败
*/
BOOL DeleteBeginChar(char *cSrc, char *cDest, char cDel);


/**
* @brief	function name:"DeleteEndChar"
* @use		从源字符串数组cSrc结尾删除所有字符cDel，并存入目的字符串数组cDest中
* @param 	[type] char* [name] cSrc 源字符串数组
* @param 	[type] char* [name] cDest 目的字符串数组
* @param 	[type] char [name] cDel 需要删除的字符
* @return	TRUE 成功， FALSE 失败
*/
BOOL DeleteEndChar(char *cSrc, char *cDest, char cDel);


/**
* @brief	function name:"convertPath"
* @use		设置m_prun_param->cLogPath
* @param 	[type] char* [name] cSrc 源路径字符
* @param 	[type] char* [name] cDest 目的路径字符
* @return	TRUE 成功， FALSE 失败
*/
BOOL convertPath(char *cSrc, char *cDest);





#endif//DEAL_LOG_PATH_H