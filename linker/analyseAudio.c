#include "analyseAudio.h"
#include "Wrapper.h"

int main() { 	
 	// Testing load and unload of Essentia
 	FeatureExtractor* fex = newFeatureExtractor();
 	displayFeatureExtractor(fex);

 	computeFeatureExtractor(fex);
 	
 	destroyFeatureExtractor(fex);
 	
	return 1;
}
