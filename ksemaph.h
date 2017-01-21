#ifndef __KSEMP__
#define __KSEMP__ 1

#include "SCHEDULE.h"
#include "queue.h"
#include "semaphor.h"

class KernelSem {
	friend class KSemQueue;
	
	public:
		KernelSem(int init = 1);
		~KernelSem();
		
		void wait();
		void signal();
		
		int get_val();
		
	protected:
		void block();
		void deblock();
	
	private:
		int val;
		Queue* blocked;
		
		int id;
		static int tId;

};


#endif 
