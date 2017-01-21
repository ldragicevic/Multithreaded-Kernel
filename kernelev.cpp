#include "kernelev.h"
#include "pcb.h"
#include "schedule.h"
#include "system.h"

int KernelEv::tId = 0;

KernelEv::KernelEv(IVTNo IVT_NO, Event *EVENT) {
	ivtNo = IVT_NO;
	event = EVENT;
	pcb = PCB::running;
	val = 0;

	// registrovanje sa ivtentry objektom na ulazu broj ivtno
	IVTEntry *entry = IVTEntry::getEntryAt(IVT_NO);
	if (entry->kernelEv != 0) {
		state = NOT_VALID;
	}
	else {
		state = NOT_BLOCKED;
		entry->kernelEv = this;
	}

	id = tId++;
}

KernelEv::~KernelEv() {
	if (state != NOT_VALID) {
		IVTEntry *entry = IVTEntry::getEntryAt(ivtNo);
		entry->kernelEv = 0;
	}
}

void KernelEv::wait() {
	//	ne moze druga nit sem one koja je napravila da poziva wait()
	if (PCB::running != this->pcb)
		return;
	if (state == NOT_VALID)
		return;
	//	blokiranje
	if (state == NOT_BLOCKED) {
		state = BLOCKED;
		block();
	}
}

void KernelEv::signal() {
	if (state == NOT_VALID)
		return;

	if (state == BLOCKED) {
		state = NOT_BLOCKED;
		deblock();
	}
}

void KernelEv::signalOverEvent() {
	if (state == NOT_VALID)
		return;
	event->signal();
}

void KernelEv::block() {
	pcb->state = PCB::BLOCKED;
	System::dispatch();
}

void KernelEv::deblock() {
	pcb->state = PCB::READY;
	Scheduler::put(pcb);
}



