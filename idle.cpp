#include "system.h"
#include "idle.h"

Idle::Idle(): Thread(256, 1) {
	LOCK;
	myPCB->state = PCB::READY;
	myPCB->createStack();
	UNLOCK;
}

void Idle::run() {
	while(1);
}

PCB* Idle::getPCB() {
	return myPCB;
}
