#include "attribute.h"

using namespace std;

Attribute::Attribute(string n, string t) {
	name = n;
	type = t;
}	

string Attribute::getName() {
	return name;
}

string Attribute::getType() {
	return type;
}
