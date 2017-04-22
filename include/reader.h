#ifndef READERJKLNMHJKSDABNMFKWKQJSNFJFKD
#define READERJKLNMHJKSDABNMFKWKQJSNFJFKD
//#include "lib.h"
#include "dimension.h"
#include "attribute.h"
#include <queue>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Reader {
	
	string fileTemp, meta, dir;
	unsigned int dimInName, cacheSize;
	vector<Dimension> dim;
	vector<Attribute> attr;
	vector<void * *> cache;
	vector<bool *> fileMasks;
	queue<int> files;

public:
	Reader(string name, vector<Dimension> dims, vector<Attribute> attrs);
	~Reader();
	void * * read(vector<unsigned int> indices);
	unsigned int getDimInName();
	void print();
	
private:
	void readDimNum();
	void initCache();
	void * * findInCache(vector<unsigned int> indices);
	void * readInt(unsigned char * buffer, unsigned int &byte);
	void readMask(unsigned char * buffer, unsigned int &byte, queue<bool> &cellMask);
	void * * readCell(unsigned char * buffer, unsigned int &byte, queue<bool> &cellMask);
	void * * readDim(unsigned char * buffer, unsigned int posDim, unsigned int &byte, int maskPos, unsigned int &pos, queue<bool> &cellMask);
	void * * readFile2(ifstream &file, int pos);
	void * * readFile(vector<unsigned int> indices);
	void deleteOneLine(void * * line);
	void deleteData(void * * data, unsigned int posDim);

}; 
#endif
