#include "analyseAudio.h"
#include "Wrapper.h"

// just for testing
void testArray() {
	const int ARRAY_SIZE = 16;
        double *numbers = (double *) malloc(sizeof(double) * ARRAY_SIZE);

        int idx;
        for (idx = 0; idx < ARRAY_SIZE; ++idx) {
                numbers[idx] = (idx * 20 + idx / 47) / ( (idx + 9.8) * 2);
        }

        for (idx = 0; idx < ARRAY_SIZE; ++idx) {
                printf("numbers[%d] \t: %lf\n", idx, numbers[idx]);
        }

        free(numbers);
}



int main() { 	
 	// Testing load and unload of Essentia
/*	testClass();*/
 	FeatureExtractor* fex = newFeatureExtractor();
 	displayFeatureExtractor(fex);
 	computeFeatureExtractor(fex);
 	destroyFeatureExtractor(fex);
	return 1;
}
