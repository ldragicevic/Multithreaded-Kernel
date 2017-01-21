#include "ivtentry.h"
#include "system.h"
#include "dos.h"
#include "kernelev.h"

IVTEntry* IVTEntry::entries[256];

IVTEntry::IVTEntry(IVTNo IVT_NO, InterruptRoutine NEW_ROUTINE, int CALLOLDROUTINE) {
	ivtNo = IVT_NO;
	callOldRoutine = CALLOLDROUTINE;
	entries[ivtNo] = this;
	kernelEv = 0;
	LOCK;
	oldRoutine = 0;
	#ifndef BCC_BLOCK_IGNORE
	oldRoutine = getvect(IVT_NO);
	setvect(IVT_NO, NEW_ROUTINE);
	#endif
	UNLOCK;
}

IVTEntry::~IVTEntry() {
	LOCK;
	#ifndef BCC_BLOCK_IGNORE
	setvect(ivtNo, oldRoutine);
	// send end of interrupt signal - keyboard
	asm {
		mov al, 20h;
		out 20h, al;
	}
	#endif
	entries[ivtNo] = 0;
	if (kernelEv != 0)
		kernelEv->signalOverEvent();
	UNLOCK;
}

IVTEntry* IVTEntry::getEntryAt(IVTNo ivtNo) {
	return entries[ivtNo];
}

void IVTEntry::processInterrupt() {
	if (callOldRoutine == 1)
		oldRoutine();

	if (kernelEv)
		kernelEv->signalOverEvent();

	System::dispatch();
}

