#ifndef APPBRUTE_CHUNKSDSJKALIWKSHEHJSKALDJIWOUEQIOJKLSDAL
#define APPBRUTE_CHUNKSDSJKALIWKSHEHJSKALDJIWOUEQIOJKLSDAL
#include "lib.h"

//hlavicka patternu je podmnozina hlavicky dat, ve tvaru nazev:dim123, kde 123 je pocet unikatnich hodnot
using namespace std;

//Returns upper left position of solution
//checks if the rest of the pattern is in the data
unsigned int findRest(void * * data, Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, \
	unsigned int posDimP, vector<unsigned int> indices, int posInd, vector<unsigned int> cacheInd);

vector<pair <vector<unsigned int>, unsigned int> > findFirst(void * * data, Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, unsigned int posDimP, \
	vector<unsigned int> cacheInd, unsigned int maxE);
//finds upper left position of the pattern and checks if the rest is the same by calling findRest, returns coordinates of "top left" corner
vector<vector<unsigned int> > find(void * * data, Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, unsigned int posDimP, \
	vector<unsigned int> cacheInd, unsigned int maxE);
//initializes recursion parameters
vector<vector<unsigned int> > find(Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int maxE);
//--------------------------------------------------------------------------------------------------------------------------
int charToInt(const char * n);
//only function called in main, initialize, reads data, finds solution 
void run(const char * in, const char * p, const char * err);
//--------------------------------------------------------------------------------------------------------------------------
//calls run, control of input parameters
int main(int argc, char* argv[]);
#endif
