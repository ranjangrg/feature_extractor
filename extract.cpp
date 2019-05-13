/*
* Copyright (C) 2006-2016  Music Technology Group - Universitat Pompeu Fabra
*
* This file is part of Essentia
*
* Essentia is free software: you can redistribute it and/or modify it under
* the terms of the GNU Affero General Public License as published by the Free
* Software Foundation (FSF), either version 3 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
* details.
*
* You should have received a copy of the Affero GNU General Public License
* version 3 along with this program.  If not, see http://www.gnu.org/licenses/
*/

/*
To compile:
g++ -pipe -Wall -O2 -fPIC -I/usr/local/include/essentia/ -I/usr/local/include/essentia/scheduler/ -I/usr/local/include/essentia/streaming/  -I/usr/local/include/essentia/utils -I/usr/include/taglib -D__STDC_CONSTANT_MACROS standard_mfcc.cpp -o standard_mfcc -L/usr/local/lib -lessentia -lfftw3 -lyaml -lavcodec -lavformat -lavutil -lsamplerate -ltag -lfftw3f 
*/


#ifndef BASIC_H
	#define BASIC_H
	#include <iostream>
	#include <fstream>
#endif
#ifndef ESSENTIA_H
	#define ESSENTIA_H
	#include <essentia/algorithmfactory.h>
	#include <essentia/essentiamath.h>
	#include <essentia/pool.h>
	using namespace std;
	using namespace essentia;
	using namespace essentia::standard;
#endif
#ifndef FEAT_EXTRACTOR
	#define FEAT_EXTRACTOR
	#include "./extract.h"
#endif

//#include "credit_libav.h"


// just for debugging and printing the values of a matrix (vector of vectors)
void printMatrix(vector<vector<Real> > &matrix) {
	for (unsigned int idx = 0; idx < matrix.size(); ++idx) {
		cout << matrix[idx] << endl;
	}
}

// returns the vector at the given index within a given pool with name poolName
vector<Real> getValuesFromPoolAt(Pool &pool, unsigned long int idx, string poolName) {
	return pool.value <vector<vector<Real> > >(poolName)[idx];
}

// prints all the values from the pool with the given group name
// example: printPool (pool, "lowlevel.mfcc");
void printPool(Pool &pool, string name) {
	vector<vector<Real> > values = pool.value <vector<vector<Real> > > (name);	// a way to get values from Pool
	cout << "Pool values for \"" << name << "\"" << endl;
	for (long unsigned idx = 0; idx < values.size(); ++idx) {
		cout << getValuesFromPoolAt(pool, idx, name) << endl;
	}	
}

// returns the transpose of a given matrix; only vectors of <Real> atm
vector<vector<Real> > transpose(vector<vector<Real> > &matrix)   {
	unsigned int nColumns = matrix[0].size(); // getting number of columns
	// new matrix will have inverted size i.e. new col = original row and new row = original col
	vector<vector<Real> > transposed(nColumns, vector<Real>(matrix.size())); 
	for (unsigned int row = 0; row < matrix.size(); ++row) {
		for (unsigned int col = 0; col < nColumns; ++col) {
			transposed[col][row] = matrix[row][col];
		}
	}
	return transposed;
}

// makes all the values within the matrix (of Reals) to positive
void makeAbsolute(vector<vector<Real> > &matrix) {
	for (unsigned int row = 0; row < matrix.size(); ++row) {
		unsigned int columnSize = matrix[row].size();
		for (unsigned int col = 0; col < columnSize; ++col) {
			matrix[row][col] = fabs(matrix[row][col]);
		}
	}
}

// Adds matrix to an Pool object
void addMatrixToPool(Pool &pool, vector<vector<Real> > &matrix, string name) {
	for (unsigned int row = 0; row < matrix.size(); ++row) {
		vector<Real> currentRow = matrix[row];
		pool.add(name, currentRow);
	}
}

// Within a given pool, the method transposes data (referenced by a given poolName)
// and stores the transposed data within the same pool using a new name
// note: MUST have same number of columns throughout all rows
void addTransposedToPool(Pool &pool, string originalPoolName, string newPoolName) {
	unsigned long int nColumns = pool.value <vector<vector<Real> > >(originalPoolName)[0].size();	// it MUST have at least one vector
	vector<vector<Real> > transposedMatrix (nColumns); 
	transposedMatrix = transpose(pool.value <vector<vector<Real> > >(originalPoolName));
	addMatrixToPool(pool, transposedMatrix, newPoolName);
}

// method to add new dataset to the pool containing Derivatives;
// note: we use a transposed MFCC to get all the Delta values 
// AND we convert all the Delta values to positive (OR else MEAN will mostly be zero overall)
void addDerivativesToPool(Pool &pool, string transposedMfccPoolName, string mfccDeltaPoolName, Algorithm* delta, int absolute){
	// Within the pool dataset defined by the name,
	// No of columns = number of frames computed; No of rows = number of MFCC coefficients
	unsigned int totalFrameCount = pool.value <vector<vector<Real> > > (transposedMfccPoolName)[0].size();
	unsigned int nCoeff = pool.value <vector<vector<Real> > > (transposedMfccPoolName).size();

	vector<vector<Real> > deltaValuesMatrix (totalFrameCount);	// holds as many vectors as the number of frames
	vector<Real> deltaValues, deltaMfcc;				// temporary cache vectors
	for (unsigned int idx = 0; idx < nCoeff; ++idx) {
		// use TRANSPOSED mfcc list; with each loop iterates through each MFCC (1st, 2nd, etc.)
		deltaValues = getValuesFromPoolAt(pool, idx, transposedMfccPoolName);	
		delta->input("signal").set(deltaValues);// setting as input, the current MFCC we are working on
		delta->output("signal").set(deltaMfcc);	// setting output
		delta->compute();			// compute Derivative for current MFCC using the input and output

		// at this point, variable 'deltaMfcc' holds the derivative of the current MFCC coefficient		
		// pushing each mfccDelta to the main Delta matrix now
		for (long unsigned int jdx = 0; jdx < deltaMfcc.size(); ++jdx) {
			if (absolute == 0) {
				deltaValuesMatrix[jdx].push_back( deltaMfcc[jdx] );	// converting to positive numbers
			} else {
				deltaValuesMatrix[jdx].push_back( fabs(deltaMfcc[jdx]) );
			}
		}
	}	
	// making all delta values positive; 
	//makeAbsolute(deltaValuesMatrix);	// if we want to converting the whole matrix to positive at the end
	addMatrixToPool(pool, deltaValuesMatrix, mfccDeltaPoolName);
}

// Modify per will
void testMethod() {
	vector<Real> row0 = {-1,-2,-3};
	vector<Real> row1 = {-4,-5.2,-6};
	vector<Real> row2 = {-7,-8,-9};
	vector<Real> row3 = {-10,-11,-12.00056};

	vector<vector<Real> > matrix = {row0, row1, row2, row3};
	cout << "Before" << endl;	
	printMatrix(matrix);
	
	vector<vector<Real> > absMatrix;
	makeAbsolute(matrix);
	cout << "After" << endl;	
	printMatrix(matrix);
}

int main(int argc, char* argv[]) {
	// uncomment below to run testMethod(); exits after completion
	//testMethod(); return 1;
	
	if (argc != 3) {
		cout << "Error: incorrect number of arguments." << endl;
		cout << "Usage: " << argv[0] << " audio_input yaml_output(file)" << endl;
		//creditLibAV();
		exit(1);
	}

	string audioFilename = argv[1];
	string outputFilename = argv[2];

	// register the algorithms in the factory(ies)
	essentia::init();

	// this pool will hold values
	Pool pool;

	/////// PARAMS //////////////
	int sampleRate = 32052;				// sample rate of the audio signal
	int frameSize = (int)(sampleRate/2);		// size of a frame for FrameCutter
	int hopSize = (int)(sampleRate/4);		// hop size for FrameCutter
	int nCoeff = 12;				// number of MFCC coefficients
	string mfccInputSpectrumType = "magnitude"; 	// normally spectrum() produces a magnitude spectrum
	
	// Following are the names given to the dataset within the pool; same will be used on output
	string mfccPoolName = "lowlevel.mfcc";
	string transposedMfccPoolName = "temp.transposedMFCC";
	string mfccDeltaPoolName = "lowlevel.mfccDelta";

	// we want to compute the MFCC of a file: we need the create the following:
	// audioloader -> framecutter -> windowing -> FFT -> MFCC

	AlgorithmFactory& factory = standard::AlgorithmFactory::instance();

	Algorithm* audio = factory.create(	"MonoLoader",
						"filename", audioFilename,
						"sampleRate", sampleRate);
										
	Algorithm* fc    = factory.create(	"FrameCutter",
						"frameSize", frameSize,
						"hopSize", hopSize,
						"startFromZero", true,
						"lastFrameToEndOfFile", true);

	Algorithm* w     = factory.create(	"Windowing",
						"type", "hann");

	Algorithm* spec  = factory.create(	"Spectrum"); // produces a magnitude spectrum (NOT power)
	Algorithm* mfcc  = factory.create(	"MFCC",
						"numberCoefficients", nCoeff,
						"sampleRate", sampleRate,
						"inputSize", hopSize+1,
						"type", mfccInputSpectrumType); // specify type of input spectrum
					
	Algorithm* delta = factory.create("Derivative");

	/////////// CONNECTING THE ALGORITHMS ////////////////
	cout << "-------- connecting algos ---------" << endl;

	// Audio -> FrameCutter
	vector<Real> audioBuffer;

	audio->output("audio").set(audioBuffer);
	fc->input("signal").set(audioBuffer);

	// FrameCutter -> Windowing -> Spectrum
	vector<Real> frame, windowedFrame;

	fc->output("frame").set(frame);
	w->input("frame").set(frame);

	w->output("frame").set(windowedFrame);
	spec->input("frame").set(windowedFrame);

	// Spectrum -> MFCC
	vector<Real> spectrum, mfccCoeffs, mfccBands;

	spec->output("spectrum").set(spectrum);
	mfcc->input("spectrum").set(spectrum);

	mfcc->output("bands").set(mfccBands);
	mfcc->output("mfcc").set(mfccCoeffs);

	/////////// STARTING THE ALGORITHMS //////////////////
	cout << "-------- start processing " << audioFilename << " --------" << endl;

	audio->compute();
	
	long unsigned currentFrameCount = 0;	// variable to keep track of number of frames

	while (true) {
		// compute a frame
		fc->compute();

		// if it was the last one (ie: it was empty), then we're done.
		if (!frame.size()) {
			break;
		}

		// if the frame is silent, just drop it and go on processing
		if (isSilent(frame)) continue;

		w->compute();
		spec->compute();
		mfcc->compute();

		// "pool.add" adds mfccs of each frame to the pool
		pool.add(mfccPoolName, mfccCoeffs);
				
		currentFrameCount++; // increment the current frame index
	}
	
	// Uncomment below, if totalFrameCount is needed; although it can be calculated, its sometimes easier to
	// have it stored (to avoid recalculating)
	//long unsigned totalFrameCount = currentFrameCount; // this variable now holds number of frames processed;
	
	// transpose the MFCC data within the pool and store as a new dataset
	addTransposedToPool(pool, mfccPoolName, transposedMfccPoolName);

	// Uncomment below to print pool values	
	//printPool (pool, mfccPoolName);
	//printPool (pool, transposedMfccPoolName);

	/////////// Calculating Derivative of each MFCC coefficient //////////////////
	
	// Computing the Derivative of each MFCC (note: using transposed MFCC list)
	// It is so because, if we are calculating means of MFCCs, we want mean of the
	// matching MFCCs (i.e. mean of 1st MFCCs, mean of 2nd MFCCs .. NOT mean of 
	// MFCCs from the first frame, mean of the MFCCs from the second frame etc.)
	addDerivativesToPool(pool, transposedMfccPoolName, mfccDeltaPoolName, delta, 1);	// use absolute values
	
	pool.remove(transposedMfccPoolName);	// deleting temporary dataset within pool; we don't want its aggregate in final output

	// aggregate the results
	Pool aggrPool;	// the pool with the aggregated MFCC values
	const char* stats[] = { "mean", "stdev" };
	//const char* stats[] = { "copy" };

	Algorithm* aggr = AlgorithmFactory::create(	"PoolAggregator",
							"defaultStats", arrayToVector<string>(stats));

	aggr->input("input").set(pool);
	aggr->output("output").set(aggrPool);
	aggr->compute();

	// write results to file
	cout << "-------- writing results to file " << outputFilename << " ---------" << endl;

	Algorithm* output = AlgorithmFactory::create(	"YamlOutput",
							"filename", outputFilename);
	output->input("pool").set(aggrPool);
	output->compute();

	delete audio;
	delete fc;
	delete w;
	delete spec;
	delete mfcc;
	delete aggr;
	delete output;

	essentia::shutdown();

	return 0;
}
