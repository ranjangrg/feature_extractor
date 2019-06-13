#ifndef __EXTRACT_H
	#include "./extract.h"
#endif
#include "./Wrapper.h"

extern "C" {	
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
