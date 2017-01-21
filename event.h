#ifndef __EVENT__
#define __EVENT__ 1

#include "ivtentry.h"

// macro
#define PREPAREENTRY(ivtNo, callOldRoutine)							\
	extern IVTEntry entry##ivtNo;									\
	void interrupt newRoutine##ivtNo(...) {							\
		entry##ivtNo.processInterrupt();							\
	}																\
	IVTEntry entry##ivtNo(ivtNo, newRoutine##ivtNo, callOldRoutine);

typedef unsigned char IVTNo;

class KernelEv;

class Event {

	public:
		Event(IVTNo ivtNo);
		~Event();
		void wait();

	protected:
		friend class KernelEv;
		void signal();

	private:
		KernelEv* myImpl;

};

#endif
