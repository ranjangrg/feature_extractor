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
}
