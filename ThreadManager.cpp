/*
 * uthread.cpp
 *
 *  Created on: Oct 12, 2016
 *      Author: michael
 */

#include "ThreadManager.h"

#include <semaphore.h>
#include <pthread.h>
#ifndef STDIO_H_
#include <stdio.h>
#include <iostream>
#endif

namespace std {
/*
 * Initializes all the variables and allocates memory when needed
 */

int ThreadManager::maxThreads;
int ThreadManager::currentThreads;
pthread_mutex_t ThreadManager::mapMutex;
pthread_mutex_t ThreadManager::qMutex;
pthread_mutex_t ThreadManager::threadMutex;
map<int,UserThread*>* ThreadManager::threadMap;
priority_queue<UserThread*,vector<UserThread*>,UserThreadCompare>* ThreadManager::threadQueue;

void ThreadManager::uthread_init(int numKernelThreads) {
	/* Initialize all this stuff */
	pthread_mutex_t tester;
	pthread_mutex_init(&tester,NULL);
	ThreadManager::maxThreads=numKernelThreads;
	ThreadManager::currentThreads=1; //thread that called uthread create
	pthread_mutex_init(&threadMutex,NULL);
	pthread_mutex_init(&qMutex,NULL);
	pthread_mutex_init(&mapMutex,NULL);
	threadQueue= new priority_queue<UserThread*,vector<UserThread*>,UserThreadCompare>;
	threadMap = new map<int,UserThread*>;
	/* Insert Initial Thread Thread */
	ucontext_t* ucp=(ucontext_t*)malloc(sizeof(ucontext_t));
	ucp->uc_stack.ss_sp=malloc(16384);
	ucp->uc_stack.ss_size=16384;
	int threadID=syscall(SYS_gettid);
	UserThread* initThread=new UserThread(ucp,time(NULL));
	(*threadMap)[threadID]=initThread;


}
int ThreadManager::uthread_create(void (* func)( )) {
	//Create ucontext to be used in in
	ucontext_t* ucp=(ucontext_t*)malloc(sizeof(ucontext_t));
	getcontext(ucp);
	ucp->uc_stack.ss_sp=(void*)malloc(16384);
	ucp->uc_stack.ss_size=16384;
	makecontext(ucp, func, 0); //make the context for a thread running func
	//Create UserThread
	time_t currTime;
	time(&currTime);
	UserThread* newThread=new UserThread(ucp,currTime);
	//Thread Creation Logic
	pthread_mutex_lock(&threadMutex);
	if (currentThreads>=maxThreads) {
		pthread_mutex_unlock(&threadMutex);
		pthread_mutex_lock(&qMutex);
		threadQueue->push(newThread);
		pthread_mutex_unlock(&qMutex);
		return 0;
	}
	else {
		//This casting is so arcane jk rowling is including it in the next harry potter book
		int (*execute)(void *)= (int (*)(void *)) func;
		int tid=clone(execute,ucp->uc_stack.ss_sp,CLONE_VM|CLONE_FILES,NULL);
		if (tid==-1) { //clone failed
			pthread_mutex_unlock(&threadMutex);
			return -1;
		}
		currentThreads++;
		pthread_mutex_unlock(&threadMutex);
		/*
		 * Map tid -> UserThread in thread map
		 */
		(*threadMap)[tid]=newThread;
		pthread_mutex_unlock(&mapMutex);
		return 0;
	}
	//Something went wrong
	return -1;
}
void ThreadManager::uthread_exit() {
	/*
	 * Get the corresponding UserThread object from the map
	 */
	int threadID=syscall(SYS_gettid) ;
	pthread_mutex_lock(&mapMutex);

	UserThread* currThread= (*threadMap)[threadID];
	pthread_mutex_unlock(&mapMutex);
	pthread_mutex_lock(&qMutex);

	if (threadQueue->empty()) { //No items on queue, delete memory references and exit
		//printf("Queue is Empty");
		pthread_mutex_unlock(&qMutex);
		pthread_mutex_lock(&mapMutex);
		delete currThread;
		/* For some reason removing the reference from the map gives me an error sometimes.
		/ it has something to do with the fact that the threadMap is on the stack
		 *
		 */
		pthread_mutex_unlock(&mapMutex);
		pthread_mutex_lock(&threadMutex);
		//Decrement Threads
		currentThreads--;
		pthread_mutex_unlock(&threadMutex);
		exit(0);
	}
	else { //Remove and delete memory reference to old thread, set context to new thread
		UserThread* newThread=threadQueue->top();
		threadQueue->pop();
		pthread_mutex_unlock(&qMutex);
		pthread_mutex_lock(&mapMutex);
		(*threadMap)[threadID]=newThread; //Update Map
		pthread_mutex_unlock(&mapMutex);
		delete currThread;
		setcontext(newThread->ucp);
	}
}
void ThreadManager::uthread_yield() {
	int threadID=syscall(SYS_gettid) ;
	pthread_mutex_lock(&mapMutex);
	UserThread* currThread= (*threadMap)[threadID];
	pthread_mutex_lock(&qMutex);
	if (threadQueue->empty()) { //Current Thread keeps running
		pthread_mutex_unlock(&qMutex);
		pthread_mutex_unlock(&mapMutex);
		return;
	}
	else {
		currThread->updateRuntime(time(NULL)); //updates run time account for time it's been on thread
		UserThread* highestPriority=threadQueue->top();
		if (highestPriority->getRunTime()>currThread->getRunTime()) { //highestPriority is lower priority than currently running thread

			pthread_mutex_unlock(&qMutex);
			return;
		}
		else {
			/*
			 * Pop() update the startTime of highestPriority(), pop it, and push currThread
			 */
			highestPriority->updateStartTime(time(NULL));
			threadQueue->pop();
			threadQueue->push(currThread);
			pthread_mutex_unlock(&qMutex);
			pthread_mutex_lock(&mapMutex);
			(*threadMap)[threadID]=highestPriority; //Update Map reference
			pthread_mutex_unlock(&mapMutex);
			//Swaps contexts
			getcontext(currThread->ucp); //don't know if needed
			swapcontext(currThread->ucp,highestPriority->ucp);
		}

	}

}

}

