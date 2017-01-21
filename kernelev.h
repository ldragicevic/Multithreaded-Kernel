#ifndef __KERNELEV__
#define __KERNELEV__ 1

#include "event.h"

class PCB;

class KernelEv {
	friend class KEvQueue;

	public:
		KernelEv(IVTNo ivtNo, Event *event);
		~KernelEv();
		void wait();
		void signal();
		void signalOverEvent();

	protected:
		void block();
		void deblock();

	private:
		IVTNo ivtNo;
		int val;
		PCB* pcb;
		Event* event;

		enum STATES { NOT_VALID, BLOCKED, NOT_BLOCKED };
		STATES state;

		int id;
		static int tId;

};

#endif
