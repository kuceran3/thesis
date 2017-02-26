#ifndef DIMENSION4738297483294714739
#define DIMENSION4738297483294714739
#include <string>
#include <vector>

using namespace std;

class Dimension {
	string name;
	vector<string> values;
	int count;
public:
	Dimension(string n, int c);
	int getSize();
	void addVal(string val);
	string getOneVal(int pos);
}; 
#endif
