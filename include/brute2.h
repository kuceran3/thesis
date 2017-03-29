#ifndef BRUTEJSKALDJIWOUEQIOJKLSDAL
#define BRUTEJSKALDJIWOUEQIOJKLSDAL
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "dimension.h"
#include "attribute.h"
#include "lib.h"

//hlavicka patternu je podmnozina hlavicky dat, ve tvaru nazev:dim123, kde 123 je pocet unikatnich hodnot
using namespace std;

//Returns upper left position of solution
int compareType(void * first, void * second, string type);
//compares one item in the data
bool checkOneLine(void * * data, void * * dataP, vector<Attribute > attrH, vector<Attribute > attrHP);
//checks if the rest of the pattern is in the data
bool findRest(void * * data, void * * dataP, vector<Attribute > attrH, \
	vector<Attribute > attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, unsigned int posDimP, vector<unsigned int> indices, int posInd);
//finds upper left position of the pattern and checks if the rest is the same by calling findRest, returns coordinates of "top left" corner
vector<vector<unsigned int> > find(void * * data, void * * dataP, vector<Attribute > attrH, \
	vector<Attribute > attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, unsigned int posDimP);
//initializes recursion parameters
vector<vector<unsigned int> > find(void * * data, void * * dataP, vector<Attribute > attrH, \
	vector<Attribute > attrHP, vector<Dimension> dim, vector<Dimension> dimP);
//--------------------------------------------------------------------------------------------------------------------------
//only function called in main, initialize, reads data, finds solution 
void run(const char * in, const char * p);
//--------------------------------------------------------------------------------------------------------------------------
//calls run, control of input parameters
int main(int argc, char* argv[]);
#endif
