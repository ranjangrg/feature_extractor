#ifndef FEAT_EXTRACTOR
	#define FEAT_EXTRACTOR
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

	// just for debugging and printing the values of a matrix (vector of vectors)
	void printMatrix(vector<vector<Real> > &matrix);

	// returns the vector at the given index within a given pool with name poolName
	vector<Real> getValuesFromPoolAt(Pool &pool, unsigned long int idx, string poolName);

	// prints all the values from the pool with the given group name
	// example: printPool (pool, "lowlevel.mfcc");
	void printPool(Pool &pool, string name);

	// returns the transpose of a given matrix; only vectors of <Real> atm
	vector<vector<Real> > transpose(vector<vector<Real> > &matrix);

	// makes all the values within the matrix (of Reals) to positive
	void makeAbsolute(vector<vector<Real> > &matrix);

	// Adds matrix to an Pool object
	void addMatrixToPool(Pool &pool, vector<vector<Real> > &matrix, string name);

	// Within a given pool, the method transposes data (referenced by a given poolName)
	// and stores the transposed data within the same pool using a new name
	// note: MUST have same number of columns throughout all rows
	void addTransposedToPool(Pool &pool, string originalPoolName, string newPoolName);

	// method to add new dataset to the pool containing Derivatives;
	// note: we use a transposed MFCC to get all the Delta values 
	// AND we convert all the Delta values to positive (OR else MEAN will mostly be zero overall)
	void addDerivativesToPool(Pool &pool, string transposedMfccPoolName, string mfccDeltaPoolName, Algorithm* delta, int absolute);

	// Modify per will
	void testMethod();

#endif
