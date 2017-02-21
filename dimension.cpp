//#include <String>
//#include <vector>
#include "dimension.h"

using namespace std;

Dimension::Dimension(string n){
	name = n;
}

int Dimension::getSize() {
	return values.size();
}	

void Dimension::addVal(string val) {
	bool p = true;
	for (unsigned int i = 0; i < values.size(); ++i)	{
		if(values[i] == val) {
			p = false;
			break;
		}
	}
	if(p) {
		values.push_back(val);
	}
}

string Dimension::getOneVal(int pos) {
	return values[pos];
}

