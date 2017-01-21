#include "event.h"
#include "system.h"
#include "kernelev.h"

Event::Event(IVTNo ivtNo) {
	LOCK;
	myImpl = new KernelEv(ivtNo, this);
	UNLOCK;
}

Event::~Event() {
	LOCK;
	if (myImpl != 0)
		delete myImpl;
	UNLOCK;
}

void Event::wait() {
	LOCK;
	myImpl->wait();
	UNLOCK;
}

void Event::signal() {
	LOCK;
	myImpl->signal();
	UNLOCK;
}

