/*
 * UserThread.h
 *
 *  Created on: Oct 12, 2016
 *      Author: michael
 */

#ifndef USERTHREAD_H_
#define USERTHREAD_H_
#ifndef UCONTEXT_H_
#include <ucontext.h>
#endif
#ifndef TIME_H_
#include <time.h>
#endif
#ifndef THREAD
#include <thread>
#endif
class UserThread {
public:
	UserThread(ucontext_t *ucp,time_t startTime);
	virtual ~UserThread();
	//Used in PriorityQueue
	bool operator<(UserThread*);
	ucontext_t *ucp;
	//Getters and Setters
	time_t getStartTime();
	void updateStartTime(time_t newTime);
	double getRunTime();
	void updateRuntime(time_t currTime);

private:
	//Pointers to avoid stack smashing
	time_t* startTime;
	double* runTime;

};
/*
 * Struct to compare UserThread* in the priority queue
 */
struct UserThreadCompare
{
    bool operator()(UserThread* lhs, UserThread* rhs) {
    	return lhs->getRunTime() > rhs->getRunTime();
    }
};


#endif /* USERTHREAD_H_ */
