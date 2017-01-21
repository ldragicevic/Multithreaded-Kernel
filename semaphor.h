#ifndef __SEMAPHOR__
#define __SEMAPHOR__ 1

#include "SCHEDULE.h"

class KernelSem;

class Semaphore {
	
	public:
		Semaphore(int init = 1);
		virtual ~Semaphore();
		
		virtual void wait();
		virtual void signal();
		
		int val() const;
		
	private:
		KernelSem* myImpl;
};


#endif
