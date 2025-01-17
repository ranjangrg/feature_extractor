#ifndef __EXTRACT_H
	#define __EXTRACT_H
	#ifndef __BASIC_H
		#define __BASIC_H
		#include <iostream>
		#include <fstream>
	#endif
	#ifndef __ESSENTIA_H
		#define __ESSENTIA_H
		#include <essentia/algorithmfactory.h>
		#include <essentia/essentiamath.h>
		#include <essentia/pool.h>
		using namespace std;
		using namespace essentia;
		using namespace essentia::standard;
	#endif

	// testing Essentia class
	class FeatureExtractor {
	    public:
		int sampleRate, frameSize, hopSize;
		int nMFCCCoeff; // no of mfcc coefficients
		
		vector<Real> audioBuffer;			// holds input signal
		vector<Real> frame, windowedFrame;		// holds framed signal (+windowed)
		vector<Real> spectrum, mfccCoeffs, mfccBands;	// holds spectrum signal, mfcc etc
		Real totalEnergy;				// holds the total energy within the signal
		Real centroidSCT;				// holds the spectral centroid time of the signal
		Real pitch, pitchConfidence;			// holds fundamental pitch and its confidence with the value
		
		Pool pool;		// used to store values esp output(s)
		
		// AlgorithmFactory factory;
		Algorithm* audioF;	// audio algorithm factory
		Algorithm* fcF;		// frame cutter algorithm factory
		Algorithm* wF;		// windower algorithm factory
		Algorithm* specF;	// spectrum algorithm factory
		Algorithm* mfccF;	// mfcc algorithm factory
		Algorithm* deltaF;	// delta algorithm factory
		Algorithm* energyF;	// energy algorithm factory
		Algorithm* sctF;	// spectral centroid time algortihm factory
		Algorithm* pitchYF;	// PitchYin algorithm factory; NOTE: takes quite a while to process
		
		// Constructors and Destructors
		FeatureExtractor();
		~FeatureExtractor();
		
		// public class methods
		void display();		// displays the content of the object
		void connectFactories();// connects relevant factories i.e. set input, output etc.
		void computeWhole();	// computes values
	};
#endif
