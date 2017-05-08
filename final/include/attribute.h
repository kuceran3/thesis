#ifndef ATTRIBUTEJKDSLAIEUWIOQEHFJSKAUIOEW
#define ATTRIBUTEJKDSLAIEUWIOQEHFJSKAUIOEW
#include <string>
#include <vector>
//saves attributes header
using namespace std;

class Attribute {
	string name;
	string type;
public:
	Attribute(string n, string t);
	string getName();
	string getType();
}; 
#endif
