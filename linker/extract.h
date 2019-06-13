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

#endif
