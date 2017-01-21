#ifndef __QUEUE__
#define __QUEUE__ 1

#include "SCHEDULE.H"
#include "system.h"

class PCB;

class Elem {
	
	public:
		PCB* pcb;
		Elem* next;
		Time time;
		Elem(PCB*);
		Elem(PCB*, Time);
};

class Queue {
	
	public:
		Queue();
		virtual ~Queue();

		int size() const;
		virtual void put(PCB* pcb);
		virtual PCB* get();

		void stampaj();

	protected:
		int len;
		Elem* first;

};

// Drugi zadatak:

class PCB;
class KernelSem;
class KernelEv;

class PCBQueue {

public:
	static void put(PCB* pcb);
	static PCB* get(ID id);
	static void remove(ID id);

private:
	PCBQueue() {}

	class Elem {
	public:
		Elem(PCB* pcb) { this->pcb = pcb; this->next = 0; }

		PCB* pcb;
		Elem* next;
	};

	static int len;
	static Elem* first;
	static Elem* last;

};


class KSemQueue {

public:
	static void put(KernelSem* ksem);
	static KernelSem* get(ID id);
	static void remove(ID id);

private:
	KSemQueue() {}

	class Elem {
	public:
		Elem(KernelSem* ksem) { this->ksem = ksem; this->next = 0; }

		KernelSem* ksem;
		Elem* next;
	};

	static int len;
	static Elem* first;
	static Elem* last;

};


class KEvQueue {

public:
	static void put(KernelEv* kev);
	static KernelEv* get(ID id);
	static void remove(ID id);

private:
	KEvQueue() {}

	class Elem {
	public:
		Elem(KernelEv* kev) { this->kev = kev; this->next = 0; }

		KernelEv* kev;
		Elem* next;
	};

	static int len;
	static Elem* first;
	static Elem* last;

};

#endif
