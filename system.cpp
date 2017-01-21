#include "system.h"
#include "sleepque.h"

int System::promena = 0;
unsigned System::ss, System::sp, System::bp;
unsigned* System::sysPcbStack;

// +++++++++++++  Drugi zadatak +++++++++++++++

System::Function System::currentFUNCTION = System::nothing;

// +++++++++++++  Drugi zadatak +++++++++++++++

//	test
void System::stampaj() {
	LOCK;
	cout << "Taktova: " << System::promena << endl;
	UNLOCK;
}

void System::run() {
	LOCK;
	#ifndef BCC_BLOCK_IGNORE
	Timer::old_routine = getvect(0x08);
	setvect(0x08, Timer::new_routine);
	#endif
	//PCB::running = new PCB(NULL, 0, 0); // ??
	PCB::running = new PCB(NULL, 0, 0);
	Timer::idle = new Idle();
	Timer::sleepQueue = new SleepQueue();
	sysPcbStack = new unsigned[10000];
	UNLOCK;
}

void System::stop() {
	LOCK;
	#ifndef BCC_BLOCK_IGNORE
	setvect(0x08, Timer::old_routine);
	#endif
	if (Timer::idle != 0)
		delete Timer::idle;
	if (Timer::sleepQueue != 0)
		delete Timer::sleepQueue;
	UNLOCK;
}

void System::dispatch() {
	//LOCK;
	Timer::zahtevana_promena_konteksta = 1;
	Timer::new_routine();
	//UNLOCK;
}	

void System::releaseBlocked() {
	LOCK;
	while (PCB::running->sFINISHED->val() < 0) {
		PCB::running->sFINISHED->signal();
	}
	UNLOCK;
}

int System::TESTsystemUsermain(int argc, char* argv[]) {
	LOCK;
	cout << "SystemUserMain";
	UNLOCK;
	int x; cin >> x;
	return x;
}

void interrupt System::syscall(...) {
	static System::Function function;
	static unsigned s, o;
	static void* param;
	static System::paramThread* pThread;
	static System::paramSem* pSem;
	static System::paramEvent* pEvent;

#ifndef BCC_BLOCK_IGNORE
	asm {
		mov function, si;
		mov s, cx;
		mov o, dx;
	}

	param = MK_FP(s, o);

	PCB::running->sp = _SP;
	PCB::running->bp = _BP;
	PCB::running->ss = _SS;

	ss = FP_SEG(sysPcbStack + 10000);
	sp = FP_OFF(sysPcbStack + 10000);
	bp = FP_OFF(sysPcbStack + 10000);

	_SP = sp;
	_BP = bp;
	_SS = ss;
#endif

	// obrada sistemskog poziva
	switch (function) {

	case THREAD_CONSTUCTOR:
	case THREAD_DESTRUCTOR:
	case THREAD_WAIT_TO_COMPLETE:
	case THREAD_START:
	case THREAD_DISPATCH:
	case THREAD_SLEEP:
		threadCall(function, (struct paramThread*)param);
		break;

	case SEM_CONSTRUCTOR:
	case SEM_DESTRUCTOR:
	case SEM_WAIT:
	case SEM_SIGNAL:
	case SEM_VALUE:
		semCall(function, (struct paramSem*)param);
		break;

	case EVENT_CONSTRUCTOR:
	case EVENT_DESTRUCTOR:
	case EVENT_WAIT:
	case EVENT_SIGNAL:
		eventCall(function, (struct paramEvent*)param);
		break;
	}

#ifndef BCC_BLOCK_IGNORE
	_SP = PCB::running->sp;
	_BP = PCB::running->bp;
	_SS = PCB::running->ss;
#endif

//	dispatch();

}

void System::threadCall(Function function, struct paramThread* pThread) {
	switch (function) {
	case THREAD_CONSTUCTOR: {
		PCB* myPCB = new PCB(pThread->myThreadParam, pThread->stackSize, pThread->time);
		PCBQueue::put(myPCB);
		pThread->id = myPCB->id;
	} break;
	case THREAD_DESTRUCTOR: {
		PCB* myPCB = PCBQueue::get(pThread->id);
		PCBQueue::remove(myPCB->id);
		delete myPCB;
	} break;
	case THREAD_WAIT_TO_COMPLETE:
	case THREAD_START: {
		PCB* myPCB = PCBQueue::get(pThread->id);
		myPCB->createStack();
		myPCB->state = PCB::READY;
		Scheduler::put(myPCB);
	} break;
	case THREAD_DISPATCH:
	case THREAD_SLEEP:
		break;
	}
}

void System::semCall(Function function, struct paramSem* pSem) {

}

void System::eventCall(Function function, struct paramEvent* pEvent) {

}































