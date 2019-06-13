#ifndef __WRAPPER_H
	#define __WRAPPER_H

	#ifdef __cplusplus
		extern "C" {
	#endif
	
	// FeatureExtractor methods for C-code
	typedef struct FeatureExtractor FeatureExtractor;
	FeatureExtractor* newFeatureExtractor();		// Create new FeatureExtractor object
	void displayFeatureExtractor(FeatureExtractor* fex);	// display info about FeatureExtractor object
	void computeFeatureExtractor(FeatureExtractor* fex);	// computes all the factories and algorithms set in FeatureExtractor
	void destroyFeatureExtractor(FeatureExtractor* fex);	// destroys FeatureExtractor and manages memory afterthat
	
	// Test methods
	void testInC();
	void testClass();

	#ifdef __cplusplus
		}
	#endif
#endif
