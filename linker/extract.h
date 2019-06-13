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

	// just for debugging and printing the values of a matrix (vector of vectors)
	void printMatrix(vector<vector<Real> > &matrix);

	// Modify per will 
	void testMethod();	
	
	// testing Essentia class
	class FeatureExtractor {
		public:
			int sampleRate, frameSize, hopSize;
			int nMFCCCoeff; // no of mfcc coefficients
			
			vector<Real> audioBuffer;			// holds input signal
			vector<Real> frame, windowedFrame;		// holds framed signal (+windowed)
			vector<Real> spectrum, mfccCoeffs, mfccBands;	// holds spectrum signal, mfcc etc
			Real totalEnergy;				// holds the total energy within the signal
			
			Pool pool;		// used to store values esp output(s)
			
			// AlgorithmFactory factory;
			Algorithm* audio;	// audio algorithm factory
			Algorithm* fc;		// frame cutter algorithm factory
			Algorithm* w;		// windower algorithm factory
			Algorithm* spec;	// spectrum algorithm factory
			Algorithm* mfcc;	// mfcc algorithm factory
			Algorithm* delta;	// delta algorithm factory
			Algorithm* energy;	// energy algorithm factory
			
			// Constructors and Destructors
			FeatureExtractor();
			~FeatureExtractor();
			
			// public class methods
			void display();		// displays the content of the object
			void connectFactories();// connects relevant factories i.e. set input, output etc.
			void computeWhole();	// computes values
	};
#endif
