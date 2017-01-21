#ifndef __IDLE__
#define __IDLE__ 1

#include "thread.h"

class Idle : public Thread {
	
	public:
		Idle();
		void run();
		PCB* getPCB();
		
};

#endif
