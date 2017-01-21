#ifndef __PCB__
#define __PCB__ 1

#include "thread.h"
#include "queue.h"
#include "semaphor.h"

class Queue;
class Semaphore;

class PCB {
	
	public:
		PCB(Thread* myThreadParam, StackSize stackSize, Time timeSlice);
		~PCB();
		void createStack();
		Thread *myThread;
		Time timeSlice;
		StackSize stackSize;

		static PCB* running;
		static void wrapper();
		
		enum STATES { NEW, READY, BLOCKED, ENDED };
		STATES state;

		int id;
		static int tId;

		unsigned sp, ss, bp;
		unsigned *pcbStack;

		Semaphore* sFINISHED;		
};		

#endif
