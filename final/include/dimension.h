#ifndef DIMENSION4738297483294714739
#define DIMENSION4738297483294714739
#include <string>
#include <vector>
//saves dimensions header
using namespace std;

class Dimension {
	string name;
	vector<string> values;
	unsigned int count;
public:
	Dimension(string n, unsigned int c);
	unsigned int getSize();
	void addVal(string val);
	string getOneVal(int pos);
	string getName();
}; 
#endif
