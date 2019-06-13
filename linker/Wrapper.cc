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
	
	void testClass() {
		FeatureExtractor* fex = new FeatureExtractor(); // dynamic object; need to delete after use
		fex->display();
		fex->computeWhole();
		delete fex;
	}
	
	// Create new FeatureExtractor object
	FeatureExtractor* newFeatureExtractor(){
		return new FeatureExtractor();
	}
	
	// display info about FeatureExtractor object
	void displayFeatureExtractor(FeatureExtractor* fex){
		fex->display();
	}
	
	// computes all the factories and algorithms set in FeatureExtractor
	void computeFeatureExtractor(FeatureExtractor* fex) {
		fex->computeWhole();
	}
	
	// destroys FeatureExtractor and manages memory afterthat
	void destroyFeatureExtractor(FeatureExtractor* fex) {
		delete fex;
	}
}
