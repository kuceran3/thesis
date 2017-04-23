#ifndef EXACTSKIPDIMHFHWQUIDSJAJCNKDFSKAPWDIQJSKD
#define EXACTSKIPDIMHFHWQUIDSJAJCNKDFSKAPWDIQJSKD
#include "lib.h"

using namespace std;

//first dimension of pattern and data must have the same name! otherwise not functional

//Returns upper left position of solution
//checks if the rest of the pattern is in the data
bool findRest(void * * data, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, unsigned int posDimP, vector<unsigned int> indices, int posInd);
//checks if the whole pattern is correct
vector<vector<unsigned int> > checkRest(void * * data, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, int pos, \
	int posP, vector<vector<unsigned int> > subres);
//finds upper left position of the subpattern and checks if the rest of the subpattern is the same by calling findRest, returns coordinates of "top left" corner
vector<vector<unsigned int> > find(void * * data, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, \
	unsigned int posDimP);
//skips first dimension of the pattern and data and calls function find, recursion function, to found if subpatterns can be placed in the data
vector<vector<unsigned int> > find(void * * data, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP);
//--------------------------------------------------------------------------------------------------------------------------
//only function called in main, initialize, reads data, finds solution 
void run(const char * in, const char * p);
//--------------------------------------------------------------------------------------------------------------------------
//calls run, control of input parameters
int main(int argc, char* argv[]);
#endif
