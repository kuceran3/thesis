#ifndef BRUTE_CHUNKSDSJKALIWKSHEHJSKALDJIWOUEQIOJKLSDAL
#define BRUTE_CHUNKSDSJKALIWKSHEHJSKALDJIWOUEQIOJKLSDAL
#include "lib.h"

using namespace std;

//Returns upper left position of solution
//checks if the rest of the pattern is in the data
bool findRest(void * * data, Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, \
	unsigned int posDimP, vector<unsigned int> indices, int posInd, vector<unsigned int> cacheInd);
//finds upper left position of the pattern and checks if the rest is the same by calling findRest, returns coordinates of "top left" corner
vector<vector<unsigned int> > find(void * * data, Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, unsigned int posDimP, \
	vector<unsigned int> cacheInd);
//initializes recursion parameters
vector<vector<unsigned int> > find(Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP);
//--------------------------------------------------------------------------------------------------------------------------
//only function called in main, initialize, reads data, finds solution 
void run(const char * in, const char * p);
//--------------------------------------------------------------------------------------------------------------------------
//calls run, control of input parameters
int main(int argc, char* argv[]);
#endif
