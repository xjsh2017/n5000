#ifndef _XJLIB_H_
#define _XJLIB_H_

#include <string>
#include "define_scan.h"

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BOOL

typedef int BOOL;

#endif

#ifndef TRUE

#define TRUE 1

#endif

#ifndef FALSE

#define FALSE 0

#endif

#define XJLIB_MAX_CHAR_BUFF 256

#ifdef OS_WINDOWS

        #include <direct.h>

        /**
        * @brief	‰^
        */
        #define OUTPUTAPI __stdcall

        /**
        * @name		XJ_THREAD_ROUTINE
        * @brief	
        */
        typedef unsigned int(OUTPUTAPI *XJ_THREAD_ROUTINE)(void *);

        typedef SYSTEMTIME xj_systime;

#endif

#ifdef OS_LINUX

        /**
        * @brief	‰^
        */
        #define OUTPUTAPI

        /**
        * @name		XJ_THREAD_ROUTINE
        * @brief	
        */
        typedef void *(*XJ_THREAD_ROUTINE)(void *);

        /**
        * @brief	‰^
        */
        typedef struct
        {
                unsigned short wYear;
                unsigned short wMonth;
                unsigned short wDayOfWeek;
                unsigned short wDay;
                unsigned short wHour;
                unsigned short wMinute;
                unsigned short wSecond;
                unsigned short wMilliseconds;
        }xj_systime;

#endif /*OS_LINUX*/

//thread

/**
* @name		xj_thread_create
* @brief	(join)(detach)
* @param	phandle		
* @param	pid			id
* @param	routine		
* @param	param		
* @return	0
* @note
*/
int xj_thread_create(THREAD_HANDLE *phandle, THREAD_ID *pid, XJ_THREAD_ROUTINE routine, void *param);

/**
* @name		xj_thread_detach
* @brief	§Ù
* @param	phandle	
* @return	0
* @note
*/
int xj_thread_detach(THREAD_HANDLE phandle);

/**
* @name		xj_thread_join
* @brief	
* @param	phandle		
* @param	*pretcode	
* @return	0
* @note
*/
int xj_thread_join(THREAD_HANDLE phandle, void **pretcode);

/**
* @name		xj_thread_exit
* @brief	§³
* @param  retval	
* @return	
* @note
*/
void xj_thread_exit(unsigned int retval);

int pro_create_thread(THREAD_HANDLE *phandle, THREAD_ID *pid, XJ_THREAD_ROUTINE routine, void *param);
//

/**
* @name		xj_mutex_init
* @brief	
* @param	mutex	
* @return	0
* @note		
*/
int xj_mutex_init(MUTEX *mutex);

/**
* @name		xj_mutex_lock
* @brief	
* @param	mutex	
* @return	0
* @note
*/
int xj_mutex_lock(MUTEX *mutex);

/**
* @name		xj_mutex_unlock
* @brief	
* @param	mutex	
* @return	0
* @note
*/
int xj_mutex_unlock(MUTEX *mutex);

/**
* @name		xj_mutex_destroy
* @brief	
* @param	mutex	
* @return	0
* @note		¨²
*/
int xj_mutex_destroy(MUTEX *mutex);

//

/**
* @name		xj_cond_init
* @brief	
* @param	cond	
* @return	0
* @note		
*/
int xj_cond_init(HCOND *cond);

/**
* @name		xj_cond_wait
* @brief	milliseconds
* @param	mutex			
* @param	cond			
* @param	milliseconds	-1
* @return	0
* @note
*/
int xj_cond_wait(MUTEX *mutex, HCOND *cond, int milliseconds);

/**
* @name		xj_cond_signal
* @brief	§Ø
                        
* @param	cond	
* @return	0
* @note
*/
int xj_cond_signal(HCOND *cond);

/**
* @name		xj_cond_broadcast
* @brief	§Òcond§»¦Î
                        §³
* @param	cond	
* @return	0
* @note
*/
int xj_cond_broadcast(HCOND *cond);

/**
* @name		xj_cond_destroy
* @brief	
* @param	cond	
* @return	0
* @note		¨²
*/
int xj_cond_destroy(HCOND *cond);

//

/**
* @name		xj_noname_sem_init
* @brief	value
                        
* @param	sem		
* @param	value	
* @return	0
* @note		
*/
int xj_noname_sem_init(HNSEM *sem, unsigned int value);

/**
* @name		xj_noname_sem_init
* @brief	
* @param	sem		
* @return	0
* @note		¨²
*/
int xj_noname_sem_destroy(HNSEM *sem);

/**
* @name		xj_noname_sem_wait
* @brief	<=0>0
* @param	sem		
* @return	0
* @note
*/
int xj_noname_sem_wait(HNSEM *sem);

/**
* @name		xj_noname_sem_post
* @brief	
                        §Ö
* @param	sem		
* @return	0
* @note
*/
int xj_noname_sem_post(HNSEM *sem);

/**
* @name		xj_noname_sem_getvalue
* @brief	
* @param	sem		
* @return	>=0
* @note
*/
int xj_noname_sem_getvalue(HNSEM *sem);

//

/**
* @name		xj_load_library
* @brief	³libmytest.so¡¤
* @param	filename	
* @return	NULLNULL
* @note		¨´
*/
XJHANDLE xj_load_library(const char *filename);

/**
* @name		*xj_get_addr
* @brief	ï…´¿
* @param	handle	xj_load_library()
* @param	module	
* @return	NULLNULL
* @note
*/
void *xj_get_addr(XJHANDLE handle, const char *module);

/**
* @name		xj_free_library
* @brief	
* @param	handle	
* @return	0
* @note		¨´
*/
int xj_free_library(XJHANDLE handle);

/**
* @name		*xj_get_liberror
* @brief	¨°
* @return	
* @note
*/
char *xj_get_liberror();

/**
* @name		xj_sleep
* @brief	
* @param	milliseconds	>=0
* @return	0
* @note
*/
int xj_sleep(unsigned int milliseconds);

/**
* @name		xj_getlocaltime
* @brief	‰
* @param	psystime	¨
* @return	0
* @note
*/
int xj_getlocaltime(xj_systime *psystime);

/**
* @name		xj_setlocaltime
* @brief	‰
* @param	psystime	¨
* @return	0
* @note
*/
int xj_setlocaltime(const xj_systime *psystime);

/**
* @name		xj_pathfile_exist
* @brief	§Ø
* @param	pathfile	
* @return	00
* @note
*/
int xj_pathfile_exist(const char *pathfile);

/**
* @name		xj_pathfile_exec
* @brief	§Ø
* @param	pathfile	
* @return	0§µ0
* @note
*/
int xj_pathfile_exec(const char *pathfile);

/**
* @name		xj_pathfile_rw
* @brief	§Ø§Ø§Õ
* @param	pathfile	
* @return	0§µ0
* @note
*/
int xj_pathfile_rw(const char *pathfile);

/**
* @name		xj_get_file_size
* @brief	§³¦Ëbyte
* @param	file_name	
* @return	>=0§³-1
* @note
*/
int xj_get_file_size(const char *file_name);

/**
* @name		xj_create_local_dir
* @brief	¡¤
* @param	szPath	¡¤¡¤¡¤¡¤¡¤'.'
* @return	TRUEFALSE
* @note		¡¤linux'/'Windows'\'
*/
BOOL xj_create_local_dir(char *szPath);

/**
* @name		isCommenceApi
* @brief	§Øsum;//
*			
* @param	pszBuff	
* @return	TRUE FALSE 
* @note
*/
BOOL isCommenceApi(char *pszBuff);

/**
* @name		GetPrivateProfileStr
* @brief	§³sectionpszKeyName
* @param	pszSectorName	section
* @param	pszKeyName		KeyName
* @param	pszDefaultStr	
* @param	pszReturnedStr	
* @param	uSize			§³
* @param	pszFileName		
* @return	
* @note		
*/
unsigned int GetPrivateProfileStr(const char *pszSectorName,
                                                const char *pszKeyName,
                                                const char *pszDefaultStr,
                                                const char *pszReturnedStr,
                                                unsigned int  uSize,
                        const char	*pszFileName
                                                );

/**
* @name		GetPrivateProfileint
* @brief	§³sectionpszKeyName
* @param	pszSectorName	section
* @param	pszKeyName		KeyName
* @param	nDefaultValue	
* @param	pszFileName		
* @return	
* @note		
*/
int GetPrivateProfileint(const char *pszSectorName,
                                                 const char *pszKeyName,
                                                 int	nDefaultValue,
                         const char	*pszFileName
                                                 );

/**
* @name		GetPrivateProfileEqualint
* @brief	§µpszKeyName
* @param	pszKeyName		KeyName
* @param	nDefaultValue	
* @param	pszFileName		
* @return	
* @note		
*/
int GetPrivateProfileEqualint(
                                                 char	*pszKeyName,
                                                 int	nDefaultValue,
                                                 char	*pszFileName
                                                 );

/**
* @name		xj_modify_time
* @brief	§Ü
* @param	psystime		NULL
* @param	wMilliseconds	0
* @param	op				'+''-''+'
* @return	00
* @note		190011000
*/
int xj_modify_time(xj_systime *psystime, unsigned short wMilliseconds, char op);

/**
* @name		xjtime_to_timet
* @brief	
* @param	psystime		NULL
* @return	
* @note		
*/
time_t xjtime_to_timet(xj_systime *psystime);


/**
 * @name   time_to_string
 * @brief  197011  str ›¥27
 * @param  sec  since 1970-1-1 0:0:0
 * @param  us
 * @param  str
 */
void time_to_string(int sec, int us, char *str);

#ifdef __cplusplus
}
#endif

#endif/*_XJLIB_H_*/
