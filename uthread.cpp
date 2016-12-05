/*
 * uthread.cpp
 *
 *  Created on: Oct 18, 2016
 *      Author: michael
 */

#include "uthread.h"
using namespace std;
//This literally just calls threadManager
void uthread_init(int numKernelThreads) {
	ThreadManager::uthread_init(numKernelThreads);
}
int uthread_create(void (* func)()) {
	return ThreadManager::uthread_create(func);
}
void uthread_yield() {
	ThreadManager::uthread_yield();
}
void uthread_exit() {
	ThreadManager::uthread_exit();
}


