#ifndef __IVTENTRY__
#define __IVTENTRY__ 1

typedef unsigned char IVTNo;
typedef void interrupt (*InterruptRoutine)(...);

class KernelEv;

class IVTEntry {

	public:
		IVTEntry(IVTNo ivtNo, InterruptRoutine newRoutine, int callOldRoutine);
		~IVTEntry();

		static IVTEntry* getEntryAt(IVTNo ivtNo);

		void processInterrupt();

	private:
		static IVTEntry* entries[256];
		IVTNo ivtNo;
		InterruptRoutine oldRoutine;
		int callOldRoutine;

		KernelEv* kernelEv;

	friend class KernelEv;
};

#endif
