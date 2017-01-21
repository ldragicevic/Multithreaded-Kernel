#include "sleepque.h"

//	Konstruktor sleepQueue
SleepQueue::SleepQueue() : Queue() {}

//	Destruktor sleepQueue
SleepQueue::~SleepQueue() {
	while (first != 0) {
		//	Ako ima zablokiranih, vracaju se u scheduler
		Elem *temp = first->next;
		first->pcb->state = PCB::READY;
		Scheduler::put(first->pcb);
		len--;
		delete first;
		first = temp;
	}
}

//	Stavljanje PCB u red za spavanje
void SleepQueue::putSleep(PCB *pcb, Time time) {
	LOCK;
	//	Ne treba da spava
	if (time == 0)
		return;
	Elem 	*newElem = new Elem(pcb, time),
			*temp = first,
			*prev = 0;
	//	Krecemo se po listi, trazimo mesto za stavljanje
	//	usput smanjujemo pcb koji stavljamo
	//	vreme spavanja za vreme spavanja prethodne niti (temp)
	//	na kraju sledeci smanjimo za umetnuti
	while ((temp != 0) && (temp->time <= newElem->time)) {
		newElem->time -= temp->time;
		prev = temp;
		temp = temp->next;
	}
	if (temp != 0) {
		temp->time -= newElem->time;
	}
	if (prev != 0)
		prev->next = newElem;
	else
		first = newElem;
	newElem->next = temp;
	len++;
	UNLOCK;
}

//	Smanjivanje na otkucaj prvom spavanje za jedan
void SleepQueue::tickFirst() {
	LOCK;
	if (first != 0)
		first->time--;
	UNLOCK;
}

//	Ako treba da se probudi prvi (vracamo 1 i vracamo ga u scheduler), inace 0
int SleepQueue::awakeFirst() {
	LOCK;
	if ((first != 0) && (first->time == 0)) {
		PCB *pcb = first->pcb;
		pcb->state = PCB::READY;
		Scheduler::put(pcb);
		Elem *temp = first->next;
		delete first;
		first = temp;
		UNLOCK;
		return 1;
	}
	else {
		UNLOCK;
		return 0;
	}
}
