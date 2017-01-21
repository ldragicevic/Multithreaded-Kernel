#ifndef __SLEEPQUE__
#define __SLEEPQUE__ 1

#include "queue.h"

class SleepQueue : public Queue {

	public:
		SleepQueue();
		virtual ~SleepQueue();

		virtual void putSleep(PCB *pcb, Time time);
		virtual void tickFirst();
		virtual int awakeFirst();

	friend class Timer;

};

#endif
