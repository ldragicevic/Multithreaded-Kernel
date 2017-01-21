#ifndef __THREAD__
#define __THREAD__ 1

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;

typedef unsigned int Time;
const Time defaultTimeSlice = 2;

typedef int ID;
class PCB;

class Thread {
	
	public:
		virtual ~Thread();
		void start();
		void waitToComplete();
		static void sleep(Time timeToSleep);
		
	protected:
		Thread(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
		virtual void run() {};
		
	protected:
		PCB* myPCB;
		ID myID;
		
	friend class PCB;
	friend class System;

};

void dispatch();

#endif
