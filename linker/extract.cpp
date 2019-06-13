/*
To compile:
g++ -pipe -Wall -O2 -fPIC -I/usr/local/include/essentia/ -I/usr/local/include/essentia/scheduler/ -I/usr/local/include/essentia/streaming/  -I/usr/local/include/essentia/utils -I/usr/include/taglib -D__STDC_CONSTANT_MACROS extract.cpp -o extract -L/usr/local/lib -lessentia -lfftw3 -lyaml -lavcodec -lavformat -lavutil -lsamplerate -ltag -lfftw3f 
*/

// include Guards BEGIN
#include "./extract.h"
//#include "credit_libav.h"
// include Guards END

// just for debugging and printing the values of a matrix (vector of vectors)
void printMatrix(vector<vector<Real> > &matrix) {
	for (unsigned int idx = 0; idx < matrix.size(); ++idx) {
		cout << matrix[idx] << endl;
	}
}

// Modify per will
void testMethod() {
	/*
	vector<Real> row0 = {-1,-2,-3};
	vector<Real> row1 = {-4,-5.2,-6};
	vector<Real> row2 = {-7,-8,-9};
	vector<Real> row3 = {-10,-11,-12.00056};

	vector<vector<Real> > matrix = {row0, row1, row2, row3};
	cout << "Before" << endl;	
	printMatrix(matrix);
	
	vector<vector<Real> > absMatrix;
	//makeAbsolute(matrix);
	cout << "After" << endl;	
	printMatrix(matrix);
	*/
}

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
	audio->output("audio").set(audioBuffer);
	
	// audioBuffer -> Total energy
	energy->input("array").set(audioBuffer);
	energy->output("energy").set(totalEnergy);
	
	// audioBuffer -> Spectral Centroid Time
	sct->input("array").set(audioBuffer);
	sct->output("centroid").set(centroidSCT);
	
	// audioBuffer -> pitchYin
	pitchY->input("signal").set(audioBuffer);
	pitchY->output("pitch").set(pitch);
	pitchY->output("pitchConfidence").set(pitchConfidence);
	
	// FrameCutter -> Windowing -> Spectrum
	fc->input("signal").set(audioBuffer);
	fc->output("frame").set(frame);
	
	w->input("frame").set(frame);
	w->output("frame").set(windowedFrame);
	
	spec->input("frame").set(windowedFrame);
	spec->output("spectrum").set(spectrum);
	
	// Spectrum -> MFCC
	mfcc->input("spectrum").set(spectrum);
	mfcc->output("bands").set(mfccBands);
	mfcc->output("mfcc").set(mfccCoeffs);
}

// compute Algorithms
void FeatureExtractor::computeWhole() {
	audio->compute();
	
	energy->compute();	// calculating total energy of the signal
	pool.add("totalEnergy", totalEnergy);
	printPool1DReal(pool, "totalEnergy");
	
	sct->compute();		// calculating SCT of the signal
	pool.add("SCT", centroidSCT);
	printPool1DReal(pool, "SCT");
	
//	pitchY->compute();	// calculating pitch of the signal
//	if (pitch != 0) {
//		pool.add("pitch", pitch);
//		pool.add("pitch-confidence", pitchConfidence);
//		printPool1DReal(pool, "pitch");
//		printPool1DReal(pool, "pitch-confidence");
//	}
	
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
		pool.add("mfcc", mfccCoeffs);
				
		currentFrameCount++; // increment the current frame index
	}
	printPool2DReal(pool, "mfcc");
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
	audio = factory.create(	"MonoLoader",
				"filename", "../audio/sound.flac",
				"sampleRate", sampleRate);
	fc    = factory.create(	"FrameCutter",
				"frameSize", frameSize,
				"hopSize", hopSize,
				"startFromZero", true,
				"lastFrameToEndOfFile", true);
	w     = factory.create(	"Windowing",
				"type", "hann");
	spec  = factory.create(	"Spectrum"); // produces a magnitude spectrum (NOT power)
	mfcc  = factory.create(	"MFCC",
				"numberCoefficients", nMFCCCoeff,
				"sampleRate", sampleRate,
				"inputSize", hopSize+1,
				"type", "magnitude"); // specify type of input spectrum				
	delta = factory.create(	"Derivative");
	energy = factory.create("Energy");
	sct = factory.create(	"SpectralCentroidTime",
				"sampleRate", sampleRate);
	pitchY = factory.create("PitchYin",
				"sampleRate", sampleRate);
	
	connectFactories();	// connect all the factories and algorithms here
}

// Destructor - free memory-allocated while running
FeatureExtractor::~FeatureExtractor() {
	delete audio;
	delete fc;
	delete w;
	delete spec;
	delete mfcc;
	delete delta;
	delete sct;
	delete pitchY;
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
