#ifndef BOUND_CPU_H_INCLUDE
#define BOUND_CPU_H_INCLUDE

int bind_thread_2_cpu(unsigned long l_threadID, int nCpu);
int find_thread_bind_cpu(unsigned long l_threadID);


#endif /*BOUND_CPU_H_INCLUDE*/