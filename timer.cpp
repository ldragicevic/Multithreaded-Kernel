#include "system.h"
#include "sleepque.h"

InterruptRoutine Timer::old_routine = 0;
Idle* Timer::idle = 0;
SleepQueue* Timer::sleepQueue = 0;
int Timer::zahtevana_promena_konteksta = 0;
int Timer::proteklo_vreme = 0;
void tick();

int coutflag = 0;

void interrupt Timer::new_routine(...) {

	System::promena++;

	if (Timer::zahtevana_promena_konteksta == 0) {
		old_routine();
		proteklo_vreme++;
		tick();
		sleepQueue->tickFirst();
		while (sleepQueue->awakeFirst());
	}

	if (coutflag)
		return;

	if ((Timer::zahtevana_promena_konteksta == 1) || ((PCB::running->timeSlice > 0) && (PCB::running->timeSlice <= proteklo_vreme))) {

		#ifndef BCC_BLOCK_IGNORE
		PCB::running->sp = _SP;
		PCB::running->bp = _BP;
		PCB::running->ss = _SS;
		#endif

		if ((PCB::running->state == PCB::READY) && (PCB::running != idle->getPCB())) {
			Scheduler::put(PCB::running);
		}

		PCB::running = Scheduler::get();

		if (PCB::running == 0)
			PCB::running = idle->getPCB();

		#ifndef BCC_BLOCK_IGNORE
		_SP = PCB::running->sp;
		_BP = PCB::running->bp;
		_SS = PCB::running->ss;
		#endif

		Timer::zahtevana_promena_konteksta = 0;
		Timer::proteklo_vreme = 0;

	}

}
