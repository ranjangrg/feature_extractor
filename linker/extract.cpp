/*
To compile:
g++ -pipe -Wall -O2 -fPIC -I/usr/local/include/essentia/ -I/usr/local/include/essentia/scheduler/ -I/usr/local/include/essentia/streaming/  -I/usr/local/include/essentia/utils -I/usr/include/taglib -D__STDC_CONSTANT_MACROS extract.cpp -o extract -L/usr/local/lib -lessentia -lfftw3 -lyaml -lavcodec -lavformat -lavutil -lsamplerate -ltag -lfftw3f 
*/

// include Guards BEGIN
#include "./extract.h"
// include Guards END

//=========================================
//   Handle and display "Pool" methods   ||
//=========================================
// returns the vector at the given index within a given pool with name poolName
vector<Real> getValuesFromPoolAt(Pool &pool, unsigned long int idx, string poolName) {
	return pool.value <vector<vector<Real> > >(poolName)[idx];
}

// 2D-Real vectors ONLY: prints all the values from the pool with the given group name
// example: printPool2DReal (pool, "lowlevel.mfcc");
void printPool2DReal(Pool &pool, string name) {
	long unsigned size = pool.value <vector<vector<Real> > >(name).size();	// Find no of rows in the pool for that set
	cout << "Pool values for \"" << name << "\":" << endl;
	for (long unsigned idx = 0; idx < size; ++idx) {
		cout << pool.value <vector<vector<Real> > >(name)[idx] << endl;	// print the current row
	}	
}

void printPool1DReal(Pool &pool, string name) {
	cout << "Pool value for \"" << name << "\": " << pool.value<vector<Real> >(name) << endl;
}

//=========================================
// Creating and managing extractor class ||
//=========================================

// connects relevant factories i.e. set input, output etc.
void FeatureExtractor::connectFactories() {
	audioF->output("audio").set(audioBuffer);
	
	// audioBuffer -> Total energy
	energyF->input("array").set(audioBuffer);
	energyF->output("energy").set(totalEnergy);
	
	// audioBuffer -> Spectral Centroid Time
	sctF->input("array").set(audioBuffer);
	sctF->output("centroid").set(centroidSCT);
	
	// audioBuffer -> pitchYin
	pitchYF->input("signal").set(audioBuffer);
	pitchYF->output("pitch").set(pitch);
	pitchYF->output("pitchConfidence").set(pitchConfidence);
	
	// FrameCutter -> Windowing -> Spectrum
	fcF->input("signal").set(audioBuffer);
	fcF->output("frame").set(frame);
	
	wF->input("frame").set(frame);
	wF->output("frame").set(windowedFrame);
	
	specF->input("frame").set(windowedFrame);
	specF->output("spectrum").set(spectrum);
	
	// Spectrum -> MFCC
	mfccF->input("spectrum").set(spectrum);
	mfccF->output("bands").set(mfccBands);
	mfccF->output("mfcc").set(mfccCoeffs);
}

// compute Algorithms
void FeatureExtractor::computeWhole() {
	audioF->compute();	// comp[uting data from audio source (factory)
	
	energyF->compute();	// calculating total energy of the signal
	pool.add("totalEnergy", totalEnergy);
	printPool1DReal(pool, "totalEnergy");
	
	sctF->compute();		// calculating SCT of the signal
	pool.add("SCT", centroidSCT);
	printPool1DReal(pool, "SCT");
	
	/*
	// computing pitch takes a REALLY long time; hence commented out
	pitchY->compute();	// calculating pitch of the signal
	if (pitch != 0) {
		pool.add("pitch", pitch);
		pool.add("pitch-confidence", pitchConfidence);
		printPool1DReal(pool, "pitch");
		printPool1DReal(pool, "pitch-confidence");
	}
	*/
	
	long unsigned currentFrameCount = 0;	// variable to keep track of number of frames

	while (true) {
		// compute a frame
		fcF->compute();

		// if it was the last one (ie: it was empty), then we're done.
		if (!frame.size()) { break;}

		// if the frame is silent, just drop it and go on processing
		if (isSilent(frame)) continue;

		wF->compute();
		specF->compute();
		mfccF->compute();

		// "pool.add" adds mfccs of each frame to the pool
		pool.add("mfcc", mfccCoeffs);
				
		currentFrameCount++; // increment the current frame index
	}
	printPool2DReal(pool, "mfcc");	// printing mfccs from pool
}

// Constructor - init
FeatureExtractor::FeatureExtractor() {
	cout << "Initialising FeatureExtractor" << endl;
	essentia::init();
	sampleRate = 32052;
	frameSize = (int)(sampleRate/2);
	hopSize = (int)(sampleRate/4); 
	nMFCCCoeff = 12; 
	AlgorithmFactory& factory = standard::AlgorithmFactory::instance();
	audioF = factory.create(	"MonoLoader",
				"filename", "../audio/sound.flac",
				"sampleRate", sampleRate);
	fcF    = factory.create(	"FrameCutter",
				"frameSize", frameSize,
				"hopSize", hopSize,
				"startFromZero", true,
				"lastFrameToEndOfFile", true);
	wF     = factory.create(	"Windowing",
				"type", "hann");
	specF  = factory.create(	"Spectrum"); // produces a magnitude spectrum (NOT power)
	mfccF  = factory.create(	"MFCC",
				"numberCoefficients", nMFCCCoeff,
				"sampleRate", sampleRate,
				"inputSize", hopSize+1,
				"type", "magnitude"); // specify type of input spectrum				
	deltaF = factory.create(	"Derivative");
	energyF = factory.create("Energy");
	sctF = factory.create(	"SpectralCentroidTime",
				"sampleRate", sampleRate);
	pitchYF = factory.create("PitchYin",
				"sampleRate", sampleRate);
	
	connectFactories();	// connect all the factories and algorithms here
}

// Destructor - free memory-allocated while running
FeatureExtractor::~FeatureExtractor() {
	delete audioF;
	delete fcF;
	delete wF;
	delete specF;
	delete mfccF;
	delete deltaF;
	delete sctF;
	delete pitchYF;
	cout << "Destroying FeatureExtractor" << endl;
	essentia::shutdown();
}

// display contents of extractor
void FeatureExtractor::display( void ) {
	cout << "sampleRate\t: " << sampleRate << endl;
	cout << "frameSize\t: " << frameSize << endl;
	cout << "hopSize\t\t: " << hopSize << endl;
	cout << "nCoeff\t\t: " << nMFCCCoeff << endl;
}
