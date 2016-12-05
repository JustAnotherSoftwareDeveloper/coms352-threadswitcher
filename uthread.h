#ifndef UTHREAD_H_
#define UTHREAD_H_

#ifndef THREADMANAGER_H_
#include "ThreadManager.h"
#endif
/*
 * Thread to make sure project conforms with testing standards
 */
void uthread_init(int numKernelThreads);
int uthread_create(void (* func)());
void uthread_yield();
void uthread_exit();

#endif
