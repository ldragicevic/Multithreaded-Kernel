#include "ksemaph.h"
#include "system.h"
#include "SCHEDULE.h"

int KernelSem::tId = 0;

KernelSem::KernelSem(int initValue) {
	LOCK;
	val = initValue;
	blocked = new Queue();
	id = tId++;
	UNLOCK;
}

KernelSem::~KernelSem() {
	LOCK;
	if (blocked != 0) {
		//	Pri brisanju, oslobadjamo sve zakocene na njemu
		while (val < 0)
			signal();
		delete blocked;
	}
	UNLOCK;
}

int KernelSem::get_val() {
	return val;
}

void KernelSem::wait() {
	LOCK;
	if (--val < 0) {
		// Blokiranje jednog
		block();
	}
	UNLOCK;
}

void KernelSem:: signal() {
	LOCK;
	if (val++ < 0) {
		// Budjenje jednog blokiranog
		deblock();
	}
	UNLOCK;
}

void KernelSem::block() {
	PCB::running->state = PCB::BLOCKED;
	blocked->put(PCB::running);
	System::dispatch();
}

void KernelSem::deblock() {
	PCB* pcb = blocked->get();
	pcb->state = PCB::READY;
	Scheduler::put(pcb);
}
