#ifndef __TIMER__
#define __TIMER__ 1

#include "system.h"

class Idle;
class Scheduler;
class SleepQueue;

typedef void interrupt (*InterruptRoutine)(...);

class Timer {
	
	public:

		static int zahtevana_promena_konteksta;
		static int proteklo_vreme;
		
		static InterruptRoutine old_routine;
		static void interrupt new_routine(...);
		
		static Idle* idle;
		static SleepQueue* sleepQueue;

	friend class System;
	friend class PCB;
};

#endif

//void interrupt (*Timer::old_routine)(...) = 0;
