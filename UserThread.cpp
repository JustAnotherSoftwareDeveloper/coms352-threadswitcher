/*
 * UserThread.cpp
 *
 *  Created on: Oct 12, 2016
 *      Author: michael
 */

#include "UserThread.h"
/*
 * Constructor. UCP is taken in as well as start time
 * Run time initialized to 0
 *
 */
UserThread::UserThread(ucontext_t *ucp,time_t st) {
	this->ucp=ucp;
	this->startTime=(time_t*)malloc(sizeof(time_t));
	this->runTime=(double*)malloc(sizeof(double));
	*startTime=st;
	*runTime=0;

}
/**
 * Deconstructor
 */
UserThread::~UserThread() {
	//free(ucp->uc_stack.ss_sp);
	//free(ucp);
	free(startTime);
	free(runTime);

}
/*
 * adds the running time in seconds (as a double) to the current running time. Also updates the start time
 */
void UserThread::updateRuntime(time_t currTime) {
	double diffTime=difftime(currTime,*startTime);
	*runTime=*runTime+diffTime;
	*startTime=currTime;
}
/*
 * Just Updates the start time
 */
void UserThread::updateStartTime(time_t newTime) {
	*startTime=newTime;
}
/*
 * getter
 */
double UserThread::getRunTime() {
	double rTime=*runTime;
	return rTime;
}
/*
 * getter
 */
time_t UserThread::getStartTime() {
	return *startTime;
}
/*
 * THIS IS REVERSED ON PURPOSE. C++ runs a maximum priority queue by default
 * by overloading the < operator backwards, that isn't an issue. Sketchy? Yes
 * Also functional
 */
bool UserThread::operator <(UserThread* t2) {
	return this->getRunTime() > t2->getRunTime();
}



