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

#include <iostream>
#include <fstream>
#include <essentia/algorithmfactory.h>
#include <essentia/essentiamath.h>
#include <essentia/pool.h>
#include <typeinfo> // for finding data-types
//#include "credit_libav.h"

using namespace std;
using namespace essentia;
using namespace essentia::standard;

void transpose(vector<vector<Real>> &vect)   {
	vect.at(0).at(0) = -99.56;
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		cout << "Error: incorrect number of arguments." << endl;
		cout << "Usage: " << argv[0] << " audio_input yaml_output" << endl;
		//    creditLibAV();
		exit(1);
	}

	string audioFilename = argv[1];
	string outputFilename = argv[2];

	// register the algorithms in the factory(ies)
	essentia::init();

	Pool pool;

	/////// PARAMS //////////////
	int sampleRate = 32052;
	int frameSize = (int)(sampleRate/2);
	int hopSize = (int)(sampleRate/4);
	int nCoeff = 12;

	// we want to compute the MFCC of a file: we need the create the following:
	// audioloader -> framecutter -> windowing -> FFT -> MFCC

	AlgorithmFactory& factory = standard::AlgorithmFactory::instance();

	Algorithm* audio = factory.create("MonoLoader",
					"filename", audioFilename,
					"sampleRate", sampleRate);
	Algorithm* fc    = factory.create("FrameCutter",
					"frameSize", frameSize,
					"hopSize", hopSize,
					"startFromZero", true,
					"lastFrameToEndOfFile", true);

	Algorithm* w     = factory.create("Windowing",
					"type", "hann");

	Algorithm* spec  = factory.create("Spectrum");
	Algorithm* mfcc  = factory.create("MFCC",
					"numberCoefficients", nCoeff,
					"sampleRate", sampleRate,
					"inputSize", hopSize+1);
					
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

	// Testing values of audioBuffer: creating a sinusoid with main freq of 752Hz
	// by this time, audio data probably is already loaded in the variable
	//	for (long unsigned int t = 0; t < audioBuffer.size(); ++t) {
	//		double ampl = 2.3;
	//		double mainFreq = 752;
	//		audioBuffer.at(t) = ampl * sin(2 * 3.141592653 * mainFreq * t / sampleRate);
	//		cout << audioBuffer.at(t) << endl;
	//	}
	
	// creating a 2d vector to hold the mfccs (transposed)
	vector<Real> mfcc0, mfcc1, mfcc2, mfcc3, mfcc4, mfcc5, mfcc6, mfcc7, mfcc8, mfcc9, mfcc10, mfcc11; // one vector of reals for each mfcc
	vector<vector<Real>> mfccTranMatrix = {mfcc0, mfcc1, mfcc2, mfcc3, mfcc4, mfcc5, mfcc6, mfcc7, mfcc8, mfcc9, mfcc10, mfcc11}; // holds nCoeff number of vectors
	long unsigned currentFrameIdx = 0;

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
		pool.add("lowlevel.mfcc", mfccCoeffs);
		
		// pushing each mfcc to the main MFCC matrix now
		for (long unsigned int i = 0; i < mfccCoeffs.size(); ++i) {
			mfccTranMatrix[i].push_back(mfccCoeffs[i]);
		}
				
		currentFrameIdx++; // increment the current frame index
	}
	
	// testing pool vector
//	for (int idx = 0; idx < currentFrameIdx; ++idx) {
//		vector<Real> poolMfcc = pool.value <vector <vector <Real>>> ("lowlevel.mfcc")[idx]; // this is how we get value from pool
//		cout << "poolMfcc" << endl;
//		cout << poolMfcc << endl;		
////		for (long unsigned jdx = 0; jdx < deltaMfcc.size(); ++jdx) {
////			cout << std::scientific << deltaMfcc[jdx] << " ";
////		}
//	}
	
	// Derivative of each MFCC coefficient
	vector<vector<Real>> deltaValueList (currentFrameIdx); // holds nCoeff number of vectors
	vector<Real> deltaValues, deltaMfcc;
	// testing derivative function
	for (int idx = 0; idx < nCoeff; ++idx) {
		deltaValues = mfccTranMatrix[idx];
		delta->input("signal").set(deltaValues);
		delta->output("signal").set(deltaMfcc);
		delta->compute();
		// pushing each mfccDelta to the main Delta matrix now
		for (long unsigned int jdx = 0; jdx < deltaMfcc.size(); ++jdx) {
			deltaValueList[jdx].push_back(deltaMfcc[jdx]);
		}
	}
	
	for (long unsigned int idx = 0; idx < deltaValueList.size(); ++idx) {
		pool.add("lowlevel.mfccDelta", deltaValueList[idx]);
	}
	
	// testing pool vector
//	for (int idx = 0; idx < nCoeff; ++idx) {
//		vector<Real> poolMfccDelta = pool.value <vector <vector <Real>>> ("lowlevel.mfccDelta")[idx]; // this is how we get value from pool
//		cout << "MfccDelta" << idx << endl;
//		cout << poolMfccDelta << endl;		
////		for (long unsigned jdx = 0; jdx < deltaMfcc.size(); ++jdx) {
////			cout << std::scientific << deltaMfcc[jdx] << " ";
////		}
//	}
	
//	delta->output("frame").set(windowedFrame);
//	delta->input("signal").set(windowedFrame);
	
	// Testing transpose of vector here  
//	cout << "Final MFCCs ..." << endl;
//	for (long unsigned int i = 0; i < mfccTranMatrix.size(); ++i) {
//		cout << "MFCC" << i << endl;
//		for (long unsigned int j = 0; j < mfccTranMatrix[i].size(); ++j) {
//			cout << std::scientific << mfccTranMatrix[i][j] << " ";
//			if ( (j + 1) % 4 == 0 ) {cout << endl;}
//		}
//		cout << endl;
//		// adding this collection of mfccs to the pool instead
//		//pool.add("lowlevel.mfcc", mfccTranMatrix[i]);
//	}
//	cout << "Number of frames is " << currentFrameIdx + 1 << endl;

	// DELETE ME: just finding out data types
	//cout << typeid(mfcc).name() << endl;

	// aggregate the results
	Pool aggrPool; // the pool with the aggregated MFCC values
//	const char* stats[] = { "mean", "var", "stdev" };
	const char* stats[] = { "mean" };
//	const char* stats[] = { "copy" };

	Algorithm* aggr = AlgorithmFactory::create("PoolAggregator",
					"defaultStats", arrayToVector<string>(stats));

	aggr->input("input").set(pool);
	aggr->output("output").set(aggrPool);
	aggr->compute();

	// write results to file
	cout << "-------- writing results to file " << outputFilename << " ---------" << endl;

	Algorithm* output = AlgorithmFactory::create("YamlOutput",
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
