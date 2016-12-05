/*
 * uthread.h
 *
 *  Created on: Oct 12, 2016
 *      Author: michael
 */

#ifndef THREADMANAGER_H_
#define THREADMANAGER_H_
#ifndef SEMAPHORE_H_
#include <semaphore.h>
#endif
#ifndef QUEUE
#include <queue>
#endif
#ifndef MAP
#include <map>
#endif
#ifndef SCHED_H_
#include <sched.h>
#endif
#ifndef UNISTD_H_
#include <unistd.h>
#endif
#ifndef SYSCALL_H_
#include <sys/syscall.h>
#endif
#ifndef USERTHREAD_H_
#include "UserThread.h"
#endif
namespace std {
class ThreadManager {
public:

	static void uthread_init(int numKernelThreads);
	static int uthread_create(void(* func)());
	static void uthread_yield();
	static void uthread_exit();
	//Class Variables. All Static due to nature of class
	static int maxThreads; //The maximum amount of threads
	static int currentThreads; //The number of current threads
	static pthread_mutex_t threadMutex,qMutex,mapMutex; //mutexes, used instead of semaphores due to g++ options
	static priority_queue<UserThread*,vector<UserThread*>,UserThreadCompare>* threadQueue; //Priority Queue. See UserThread
	static map<int,UserThread*>* threadMap; //Map of kernel thread IDs -> UserThread object . Initialized on stack so i can use []
private:
	ThreadManager();
	virtual ~ThreadManager();
};
}

#endif /* THREADMANAGER_H_ */
