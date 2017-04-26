#ifndef HASHCHUNKSHASLKFHALFHWOIFHFPIQJFIPQFJ
#define HASHCHUNKSHASLKFHALFHWOIFHFPIQJFIPQFJ
#include "lib.h"

using namespace std;

//Returns a size of parts that pattern will be split to
int getNumOfParts(int k, int d, int dLen);
int getPartSize(int dLen, int count);
//--------------------------------------------------------------------------------------------------------------------------
//framework for creating, comparing, ... of hashes
void * * makeVoid(vector<void * *> column);
unsigned char hashInt(int item);
unsigned char hashString(string item);
unsigned char hashType(void * item, string type);
vector<unsigned char> * hashline(void * * line, int start, int end, vector<Attribute> attrH);
vector<unsigned char> * hashline(void * * line, int start, int end, vector<Attribute> attrH, vector<Attribute> attrHP, bool &valid, vector<vector <int> > &counts);
vector<unsigned char> * hashlineSlide(void * * next, void * * prev, vector<Attribute> attrH, vector<Attribute> attrHP, bool &valid, vector<vector <int> > &counts);

void * * hashPatternLine(void * * line, vector<Attribute> attrHeader, vector<Dimension> dim, int partSize, unsigned int posDim, int numP);
void * * hashPattern(void * * pattern, vector<Attribute> attrHeader, vector<Dimension> dim, unsigned int posDim, int partSize, int numP);
vector<vector<unsigned int> > getAllIndices(vector<Dimension> dim, unsigned int pos, unsigned int posDim);
bool compareHash(vector<unsigned char> * hash, vector<unsigned char> * hashP);
//--------------------------------------------------------------------------------------------------------------------------
//functions checking partial solutions
vector<vector<unsigned int> > checkPart(void * * data, Reader * cache, vector<unsigned char> * hashP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, unsigned int posDim, int partSize, vector<unsigned int> cacheInd, \
	vector<vector <int> > * countsSlide, vector<vector<unsigned int> > &indices, bool * valid);
vector<vector<unsigned int> > checkParts(void * * data, Reader * cache, void * * hashP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, unsigned int posDimP, \
	vector<unsigned int> dimPositions, vector<unsigned int> cacheInd, int partSize, int numP, vector<vector <int> > * countsSlide, \
	vector<vector<unsigned int> > &indices, bool * valid);
//--------------------------------------------------------------------------------------------------------------------------
vector<vector<unsigned int> > findParts(void * * data, Reader * cache, void * * hashP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, unsigned int posDimP, \
	vector<unsigned int> dimPositions, vector<unsigned int> cacheInd, int partSize, int numP);
//finds upper left position of the subpattern and checks if the rest of the subpattern is the same by calling findRest, returns coordinates of "top left" corner
vector<vector<unsigned int> > find(void * * data, Reader * cache, void * * hashP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, \
	unsigned int posDimP, vector<unsigned int> dimPositions, vector<unsigned int> cacheInd, int partSize, int numP);
//finds upper left corners of candidate solutions
vector<vector<unsigned int> > find(Reader * cache, void * * hashP, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, int errors, int partSize, int numP);
//--------------------------------------------------------------------------------------------------------------------------
//dynamic check of the solutions found by find function
int dynDimCheck(Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int pos, vector<unsigned int> res, int errors);
bool dynCheck(Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, vector<unsigned int> res, int errors);
//--------------------------------------------------------------------------------------------------------------------------
//preverification of the solutions found by find function
void preverifCheck(void * * data, Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, \
	unsigned int posDimP, vector<unsigned int> indices, int posInd, vector<unsigned int> cacheInd, int errors, int count, int &res, int &done);
bool preverif(Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, vector<unsigned int> res, int errors);
//--------------------------------------------------------------------------------------------------------------------------
int charToInt(const char * n);
//only function called in main, initialize, reads data, finds solution 
void run(const char * in, const char * p, const char * err);
//--------------------------------------------------------------------------------------------------------------------------
//calls run, control of input parameters
int main(int argc, char* argv[]);
#endif

