#ifndef READERJKLNMHJKSDABNMFKWKQJSNFJFKD
#define READERJKLNMHJKSDABNMFKWKQJSNFJFKD
#include <string>
#include <vector>

using namespace std;

class Reader {
	
	string orig, fileTemp, meta, dir;
	int dimInName, cacheSize;
	vector<Dimension> dim;
	vector<Attribute> attr;
	vector<void * *> cache;
	vector<bool *> fileMasks;
	queue<int> files;

public:
	Reader(int dimNum, string name);
	void * * read(indices);
	void * * findInCache();
	void * * readFile();

}; 
#endif
