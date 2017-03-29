#ifndef APPSPLITDYNASHFLHAFOEFBQOHQ
#define APPSPLITDYNASHFLHAFOEFBQOHQ
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <time.h>
//#include <boost/chrono.hpp>

#include "dimension.h"
#include "attribute.h"
#include "lib.h"

using namespace std;

//Returns sqrt of dimensions count as a size of parts that pattern will be split to
int getPartSize(Dimension dim);
//--------------------------------------------------------------------------------------------------------------------------
//functions checking partial solutions
vector<vector<unsigned int> > checkRest(void * * data, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, int pos, \
	int posP, vector<vector<unsigned int> > subres);
bool checkRest(void * * data, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, unsigned int posDim, vector<unsigned int> indices);
vector<vector<unsigned int> > checkFirst(void * * data, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, unsigned int posDim);
vector<vector<unsigned int> > checkPart(void * * data, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, unsigned int posDim, int partSize);
vector<vector<unsigned int> > checkParts(void * * data, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, unsigned int posDimP, vector<unsigned int> dimPositions);
//--------------------------------------------------------------------------------------------------------------------------
//split pattern into sqrt(pattern.size()) parts
vector<vector<unsigned int> > findParts(void * * data, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, unsigned int posDimP, vector<unsigned int> dimPositions);
//finds upper left position of the subpattern and checks if the rest of the subpattern is the same by calling findRest, returns coordinates of "top left" corner
vector<vector<unsigned int> > find(void * * data, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, \
	unsigned int posDimP, vector<unsigned int> dimPositions);
//skips first dimension of the pattern and data and calls function find, recursion function, to found if subpatterns can be placed in the data
//returns vector of solutions, where every item has less than (k) errors
vector<vector<unsigned int> > find(void * * data, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP);
//--------------------------------------------------------------------------------------------------------------------------
//only function called in main, initialize, reads data, finds solution 
void run(const char * in, const char * p);
//--------------------------------------------------------------------------------------------------------------------------
//calls run, control of input parameters
int main(int argc, char* argv[]);
#endif
