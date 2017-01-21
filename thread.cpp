#include "system.h"
#include "pcb.h"
#include "thread.h"
#include "SCHEDULE.h"
#include "sleepque.h"
#include <dos.h>
#include <stdlib.h>

extern int coutflag;

Thread::Thread(StackSize STACK_SIZE, Time TIME_SLICE) {
	LOCK;
//	myPCB = new PCB(this, STACK_SIZE, TIME_SLICE);
	System::paramThread param;
	param.myThreadParam = this;
	param.stackSize = STACK_SIZE;
	param.time = TIME_SLICE;
	System::Function function = System::THREAD_CONSTUCTOR;
#ifndef BCC_BLOCK_IGNORE
	unsigned s = FP_SEG(&param);
	unsigned o = FP_OFF(&param);
	asm{
		mov si, function;
		mov cx, s;
		mov dx, o;
	}
#endif

	System::syscall();

	myID = param.id;
	UNLOCK;
}

Thread::~Thread() {
	waitToComplete();
	LOCK;
	System::paramThread param;
	param.id = myID;
	System::Function function = System::THREAD_DESTRUCTOR;
	#ifndef BCC_BLOCK_IGNORE
		unsigned s = FP_SEG(&param);
		unsigned o = FP_OFF(&param);
		asm{
			mov si, function;
			mov cx, s;
			mov dx, o;
		}
	#endif

	System::syscall();
	UNLOCK;
}

void Thread::sleep(Time TIME_TO_SLEEP) {
	if (TIME_TO_SLEEP > 0) {
		PCB::running->state = PCB::BLOCKED;
		Timer::sleepQueue->putSleep(PCB::running, TIME_TO_SLEEP);
		System::dispatch();
	}
}

void Thread::start() {
	LOCK;
	System::paramThread param;
	param.id = myID;
	System::Function function = System::THREAD_START;
	#ifndef BCC_BLOCK_IGNORE
		unsigned s = FP_SEG(&param);
		unsigned o = FP_OFF(&param);
		asm{
			mov si, function;
			mov cx, s;
			mov dx, o;
		}
	#endif

	System::syscall();

	UNLOCK;
}

void Thread::waitToComplete() {
	// ended || sam sebe || idle || new
	if((myPCB->state == PCB::ENDED) || (myPCB == PCB::running) || (myPCB == Timer::idle->getPCB()) || (myPCB->state == PCB::NEW))
		return;
	myPCB->sFINISHED->wait();
	dispatch(); // dispatch +
}

void dispatch() {
	LOCK;
	System::dispatch();
	UNLOCK;
}
