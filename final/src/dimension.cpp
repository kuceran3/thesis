#include "dimension.h"

using namespace std;

Dimension::Dimension(string n, unsigned int c){
	name = n;
	count = c;
}

unsigned int Dimension::getSize() {
	return count;
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

string Dimension::getName() {
	return name;
}
