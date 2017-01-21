#ifndef __SYSTEM__
#define __SYSTEM__ 1

#define LOCK
#define UNLOCK

#ifndef BCC_BLOCK_IGNORE
#undef LOCK
#undef UNLOCK
#define LOCK	{ asm { pushf; cli; } }
#define UNLOCK 	{ asm { popf; } }
#endif

#include "idle.h"
#include "timer.h"
#include "thread.h"
#include "pcb.h"
#include "SCHEDULE.h"
#include "kernelev.h"
#include <iostream.h>
#include <dos.h>
#include <stdlib.h>

class System {

	public:
		static int promena;
		static void stampaj();
		static void run();
		static void stop();
		static void dispatch();
		static void releaseBlocked();
		int TESTsystemUsermain(int argc, char* argv[]);

	friend class Thread;
	friend class PCB;
	friend class DisplayThread;
	friend class Scheduler;
	friend class Timer;
	friend class KernelSem;
	friend class KernelEv;

	// +++++++++++++  Drugi zadatak +++++++++++++++

	// Lista funkcija
	enum Function 	{
					nothing,
					THREAD_CONSTUCTOR,
					THREAD_DESTRUCTOR,
					THREAD_WAIT_TO_COMPLETE,
					THREAD_START,
					THREAD_DISPATCH, 	// ?
					THREAD_SLEEP,

					SEM_CONSTRUCTOR,
					SEM_DESTRUCTOR,
					SEM_WAIT,
					SEM_SIGNAL,
					SEM_VALUE,

					EVENT_CONSTRUCTOR,
					EVENT_DESTRUCTOR,
					EVENT_WAIT,
					EVENT_SIGNAL
					};

	static Function currentFUNCTION;


	struct paramThread {
		ID id;
		Thread* myThreadParam;
		Time time;
		StackSize stackSize;
	};

	struct paramSem {
		ID id;
	};

	struct paramEvent {
		ID id;
	};

	static void interrupt syscall(...);

private:
	static unsigned sp, ss, bp;
	static unsigned *sysPcbStack;

	static void threadCall(Function function, struct paramThread* pThread);
	static void semCall(Function function, struct paramSem* pSem);
	static void eventCall(Function function, struct paramEvent* pEvent);

};

#endif
