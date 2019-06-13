#ifndef __EXTRACT_H
	#include "./extract.h"
#endif
#include "./Wrapper.h"

extern "C" {
	void testInC() {
		int value = 900;
		printf("Running CPP code now\n");
		testMethod();
		printf("Ending CPP code now\n");
	}
}
