# coms352-threadswitcher
ComS 352: Operating Systems project

NOTE: This only worked in g++ v6.0.2 10062016 . Probably an unintended behavior bug, but I was lucky enough to have a professor who grades on that machine. 
I ended up with 100, but that doesn't mean this code is good. Description is Below

Com S 352 Fall 2016 Project 1: A User-level Thread Library
Due:	
 Wednesday,	
 October	
 26,	
 11:59pm	
  
	
  
In this project, you are required to develop a user-level thread library called uthread. The thread library
should create user-level threads (upon request from some user application code), map the user-level threads
to multiple kernel threads following the many-to-many mapping model, schedule the mapping based on the
amount of time each user-level thread has been mapped to a kernel thread (i.e., when a kernel thread
becomes available, the user-level thread that has been mapped to a kernel thread for the least amount of
time should be mapped to the kernel thread; when there are multiple user-level threads that have been
mapped to kernel threads for the same amount of time, one of these threads that arrives at the ready queue
the earliest should be selected for mapping).
Thread Library Interface
The user-level thread library that you develop should provide the following functions:
void uthread_init(int numKernelThreads)
This function has to be called before any other functions of the uthread library can be called. It initializes
the uthread system and specifies the maximum number of kernel threads to be argument
numKernelThreads. For example, it may establish and initialize a priority ready queue of user-level
threads (with the amount of time each user-level thread has been mapped to kernel threads as priority
number) and other data structures.
int uthread_create(void (* func)( ))
The calling thread requests the thread library to create a new user-level thread that runs the function func(),
which is specified as the argument of this function. At the time when this function is called, if less than
numKernelThreads kernel threads have been active, a new kernel thread is created to execute function
func(); otherwise, a context of a new user-level thread should be properly created and stored on the priority
ready queue. This function returns 0 if succeeds, or -1 otherwise.
void uthread_yield()
The calling thread requests to yield the kernel thread to another user-level thread with the same or higher
priority (note: the priority is based on the time a thread has been mapped to kernel threads). If each ready
thread has lower priority than this calling thread, the calling thread will continue its running; otherwise, the
kernel thread is yielded to a ready thread with the highest priority.
void uthread_exit()
This function is called when the calling user-level thread terminates its execution. In response to this call, if
no ready user-level thread in the system, the whole process terminates; otherwise, a ready user thread with
the highest priority should be mapped to the kernel thread to run.
