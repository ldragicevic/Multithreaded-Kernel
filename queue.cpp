#include "system.h"
#include "queue.h"

#include "pcb.h"
#include "ksemaph.h"
#include "kernelev.h"

class PCB;

//	Test metoda
void Queue::stampaj() {
	LOCK;
	Elem *temp = first;
	while (temp != 0) {
		temp = temp->next;
	}
	UNLOCK;
}

Elem::Elem(PCB* newPCB) {
	pcb = newPCB;
	time = 0;
	next = 0;
}

//	+ spavanje
Elem::Elem(PCB* newPCB, Time newTime) {
	pcb = newPCB;
	time = newTime;
	next = 0;
}

Queue::Queue() {
	LOCK;
	first = 0;
	len = 0;
	UNLOCK;
}

Queue::~Queue() {
	LOCK;
	Elem *temp = 0;
	while (first != 0) {
		temp = first->next;
		delete first;
		first = temp;
		len--;
	}
	UNLOCK;
}

int Queue::size() const {
	LOCK;
	int returnLen = len;
	UNLOCK;
	return returnLen;
}

PCB* Queue::get() {
	LOCK;

	if (first == 0) {
		UNLOCK;
		return NULL;
	}

	len--;
	Elem *temp = first;
	first = first->next;
	PCB *pcb = temp->pcb;
	if (temp != 0) // stavio uslov +
		delete temp;
	UNLOCK;
	return pcb;
}

void Queue::put(PCB* pcb) {
	LOCK;
	Elem 	*newElem = new Elem(pcb),
			*temp = first;

	if (temp == 0) {
		first = newElem;
		len++;
		UNLOCK
		return;
	}

	while (temp->next) {
		temp = temp->next;
	}
	temp->next = newElem;
	len++;

	UNLOCK;
}


// Drugi zadatak:

int PCBQueue::len = 0;
PCBQueue::Elem* PCBQueue::first = 0;
PCBQueue::Elem* PCBQueue::last = 0;

void PCBQueue::put(PCB* pcb) {
	Elem* newElem = new Elem(pcb);
	if (len == 0)
		first = newElem;
	else
		last->next = newElem;
	last = newElem;
	len++;
}

PCB* PCBQueue::get(ID id) {
	Elem* temp = first;

	while (temp) {
		if (temp->pcb->id == id)
			return temp->pcb;

		temp = temp->next;
	}

	return 0;
}

void PCBQueue::remove(ID id) {
	Elem* temp = first, *prev = 0;
	while (temp) {
		if (temp->pcb->id == id) {
			if (prev)
				prev->next = temp->next;
			else {
				first = temp->next;
			}

			if (temp == last)
				last = prev;

			delete temp;

			len--;

			return;
		}

		temp = temp->next;
	}
}



int KSemQueue::len = 0;
KSemQueue::Elem* KSemQueue::first = 0;
KSemQueue::Elem* KSemQueue::last = 0;

void KSemQueue::put(KernelSem* ksem) {
	Elem* newElem = new Elem(ksem);
	if (len == 0)
		first = newElem;
	else
		last->next = newElem;
	last = newElem;
	len++;
}

KernelSem* KSemQueue::get(ID id) {
	Elem* temp = first;

	while (temp) {
		if (temp->ksem->id == id)
			return temp->ksem;

		temp = temp->next;
	}

	return 0;
}

void KSemQueue::remove(ID id) {
	Elem* temp = first, *prev = 0;
	while (temp) {
		if (temp->ksem->id == id) {
			if (prev)
				prev->next = temp->next;
			else {
				first = temp->next;
			}

			if (temp == last)
				last = prev;

			delete temp;

			len--;

			return;
		}

		temp = temp->next;
	}
}



int KEvQueue::len = 0;
KEvQueue::Elem* KEvQueue::first = 0;
KEvQueue::Elem* KEvQueue::last = 0;

void KEvQueue::put(KernelEv* kev) {
	Elem* newElem = new Elem(kev);
	if (len == 0)
		first = newElem;
	else
		last->next = newElem;
	last = newElem;
	len++;
}

KernelEv* KEvQueue::get(ID id) {
	Elem* temp = first;

	while (temp) {
		if (temp->kev->id == id)
			return temp->kev;

		temp = temp->next;
	}

	return 0;
}

void KEvQueue::remove(ID id) {
	Elem* temp = first, *prev = 0;
	while (temp) {
		if (temp->kev->id == id) {
			if (prev)
				prev->next = temp->next;
			else {
				first = temp->next;
			}

			if (temp == last)
				last = prev;

			delete temp;

			len--;

			return;
		}

		temp = temp->next;
	}
}










