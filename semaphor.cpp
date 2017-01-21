#include "semaphor.h"
#include "system.h"
#include "ksemaph.h"
#include "SCHEDULE.h"

Semaphore::Semaphore(int init) {
	LOCK;
	myImpl = new KernelSem(init);
	UNLOCK;	
}

Semaphore::~Semaphore() {
	LOCK;
	if (myImpl != 0)
		delete myImpl;
	UNLOCK;	
}
	
int Semaphore::val() const {
	return myImpl->get_val();
}

void Semaphore::wait() {
	LOCK;
	myImpl->wait();
	UNLOCK;	
}

void Semaphore::signal() {
	LOCK;
	myImpl->signal();
	UNLOCK;	
}	
