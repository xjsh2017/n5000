#include "Bind_CPU.h"

#ifdef OS_LINUX

#define _GNU_SOURCE 
#include <unistd.h>
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>
#include <sched.h>
#include <sys/types.h> 

#endif /*OS_LINUX*/


//测试用，找出线程被绑定到哪个CPU
int find_thread_bind_cpu(unsigned long l_threadID)
{
	int ret = 0;

#ifdef OS_LINUX

	cpu_set_t mask ;
	CPU_ZERO(&mask);

	ret =pthread_getaffinity_np(l_threadID,sizeof(mask),(cpu_set_t*)&mask );
	if(ret < 0)
	{
		printf("pthread_getaffinity_np err \n");
		return  NULL;
	}
	int j;
	
	for( j = 0;j < CPU_SETSIZE;j++)
	{
		if(CPU_ISSET(j,&mask))
			printf("///////////////////////thread[%lu] bind cpu[%d]\n", l_threadID, j);
	} 

#endif /*OS_LINUX*/

	return ret;

}

int bind_thread_2_cpu(unsigned long l_threadID, int nCpu)
{
	int nRet = 0;
	
#ifdef OS_LINUX
	
	cpu_set_t mask ;
	CPU_ZERO(&mask);
	CPU_SET(nCpu,&mask);
	
	int ret = 0;
	
	ret = pthread_setaffinity_np(l_threadID, sizeof(mask), (const cpu_set_t*)&mask );
	if(ret < 0)
	{
		printf("pthread_setaffinity_np err \n");
		return ret;
	}
	printf("//////////////////////////////////////////bound threadID:%lu to CPU%d success \n\n", l_threadID, nCpu);
	
#endif /*OS_LINXU*/
	
	return 0;
}