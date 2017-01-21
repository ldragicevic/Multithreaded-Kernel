#include "system.h"

extern int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]) {
	System::run();
	int value = userMain(argc, argv);
	System::stop();
	return value;
}
