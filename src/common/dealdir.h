#ifndef DEAL_DIR_H
#define DEAL_DIR_H

#ifdef OS_WINDOWS

bool DeleteFolder(char * lpPath);
bool DeleteDir(const char *pDir);

#endif//OS_WINDOWS





#ifdef OS_WINDOWS
#include <direct.h>
#include <io.h>
#elif OS_LINUX
#include <stdarg.h>
#include <sys/stat.h>
#include <unistd.h>//access
#endif

#ifdef OS_WINDOWS
#define ACCESS _access
#define MKDIR(a) _mkdir((a))
#elif OS_LINUX
#define ACCESS access
#define MKDIR(a) mkdir((a),0755)
#endif

#ifdef OS_LINUX
#include <stddef.h>//NULL
#include <string.h>//memset
#endif


bool CreateZipDir(const char *cZfname);






#endif//DEAL_DIR_H