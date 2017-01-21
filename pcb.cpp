#include "system.h"

PCB* PCB::running = NULL;
int PCB::tId = 0;

PCB::PCB(Thread* MY_THREAD, StackSize STACK_SIZE, Time TIME_SLICE) {
	LOCK;
	ss = sp = bp = 0;
	
	id = tId++;
	
	pcbStack = 0;
	myThread = MY_THREAD;
	// delimo sa 2B u bajtovima pravimo stek
	stackSize = STACK_SIZE / sizeof(unsigned);
	// ako je veca vel. steka od predvidjene, postavljamo ga na max 64kb
	if (stackSize > 65536UL)
		stackSize = 65536UL;
	timeSlice = TIME_SLICE;
	state = PCB::NEW;
	sFINISHED = new Semaphore(0);
	UNLOCK;
}	

PCB::~PCB() {
	LOCK;
	if (pcbStack != 0)
		delete [] pcbStack;
	if (sFINISHED != 0)
		delete sFINISHED;
	UNLOCK;
}	

// pcbStack pomeriti 4 mesta
// pcbStack[stackSize-1] = FP_SEG(this->parentThread);
// pcbStack[stackSize-2] = FP_OFF(this->parentThread);
// na -3 i -4 nije bitno, to je povratna adresa nakon izvrsavanja Thread::wrapper, ali posto
// se on nikad ne zavrsava nego se pomocu dispatch iskoci, to nije bitno
// mora samo da se odvoji prostor jer kompajler ne zna da je to posebna funkcija od koje nit krece
// i generise kod koji preskace povratnu adresu kad dovhata parametre sa steka

void PCB::createStack() {
	LOCK;
	pcbStack = new unsigned[stackSize];
	// flags(10) = interrupt bit
	pcbStack[stackSize-1] = 0x200;
	#ifndef BCC_BLOCK_IGNORE
	pcbStack[stackSize-2] = FP_SEG(PCB::wrapper);
	pcbStack[stackSize-3] = FP_OFF(PCB::wrapper);
	ss = FP_SEG(&pcbStack[stackSize-12]);
	sp = FP_OFF(&pcbStack[stackSize-12]);
	bp = FP_OFF(&pcbStack[stackSize-12]);
	#endif
	UNLOCK;
}

void PCB::wrapper() {
	PCB::running->myThread->run();
	PCB::running->state = PCB::ENDED;
//	LOCK;
	System::releaseBlocked();
//	UNLOCK;
	System::dispatch();	
	//	Obrnuo sam redosled unlock ,dispatch
}
