#include "lib.h"

using namespace std;

//File reading
vector<Attribute> readHeader(string row, vector<Dimension> &dim){
	vector<Attribute> res;
	int pos1, pos2, len2;
	string name, type;
	
	while(1) {
		pos1 = row.find(":");
		pos2 = row.find(",");
		if (pos2 == -1)
			pos2 = (int)row.length();
		len2 = pos2 - pos1 - 1;
		name = row.substr(0, pos1);
		type = row.substr(pos1 + 1, len2);
		if(type.find("dim") == 0) {
			dim.push_back(Dimension(name, stoi(type.substr(3))));
		} else {
			res.push_back(Attribute(name, type));
		}
		if (pos2 == (int)row.length())
			break;
		row = row.substr(pos2 + 1);
	}

	return res;
}

void * readType(string type, string data) {
	if(type.length() >= 3 && type.substr(0, 3) == "int") {
		return (void *)(new int(stoi(data)));
	} else {
		return (void *)(new string(data));
	}
}

void * * readOneLine(string line, vector<Attribute> attrHeader, vector<Dimension> &dim) {
	void * * res = new void * [attrHeader.size()];
	int pos;

	for (unsigned int i = 0; i < dim.size(); ++i) {
		pos = line.find(",");
		dim[i].addVal(line.substr(0, pos));
		line = line.substr(pos + 1);
	}

	for (unsigned int i = 0; i < attrHeader.size(); ++i) {
		pos = line.find(",");
		res[i] = readType(attrHeader[i].getType(), line.substr(0, pos));
		line = line.substr(pos + 1);
	}
	return res;
}

void * * readData(ifstream &file, vector<Attribute> attrHeader, vector<Dimension> &dim, unsigned int posDim) {
	void * * res;
	string data;
	
	res = new void * [dim[posDim].getSize()];

	for (int i = 0; i < dim[posDim].getSize(); ++i) {
		if (posDim + 1 >= dim.size()) {
			getline(file, data, '\n');
			res[i] = (void *)readOneLine(data, attrHeader, dim);
		} else {
			res[i] = (void *)readData(file, attrHeader, dim, posDim + 1);
		}	
	}
	return res;	
}

void * * readData(ifstream &file, vector<Attribute> attrHeader, vector<Dimension> &dim) {
	if (dim.size() <= 0) {
		string line;
		getline(file, line, '\n');
		return readOneLine(line, attrHeader, dim);
	}
	else 
		return readData(file, attrHeader, dim, 0);
}

//--------------------------------------------------------------------------------------------------------------------------
//Data printing
string toString(void * data, string type) {
	if(type.length() >= 3 && type.substr(0, 3) == "int") {
		return to_string(*((int*)data));
	} else
		return *((string*)data);
}

void printOneLine(void * * line, vector<Attribute> attrHeader) {
	for (unsigned int i = 0; i < attrHeader.size(); ++i) {
		cout << toString(line[i], attrHeader[i].getType()) << ", ";
	}
	cout << endl;
}

void printData(void * * data, vector<Attribute> attrHeader, vector<Dimension> &dim, unsigned int posDim) {
	for (int i = 0; i < dim[posDim].getSize(); ++i) {
		if (posDim + 1 >= dim.size()) {
			printOneLine((void * *)data[i], attrHeader);
		} else {
			printData((void * *)data[i], attrHeader, dim, posDim + 1);
		}
	}
	cout << endl;
}

void printData(void * * data, vector<Attribute> attrHeader, vector<Dimension> &dim) {
	for (unsigned int i = 0; i < attrHeader.size(); ++i)	{
		cout << attrHeader[i].getName() << ", ";
	}

	cout << endl << "Data: " << endl;
	if (dim.size() <= 0)
		printOneLine(data, attrHeader);
	else 
		printData(data, attrHeader, dim, 0);
}

//--------------------------------------------------------------------------------------------------------------------------
//Cleaning
void deleteType(void * data, string type) {
	if(type.length() >= 3 && type.substr(0, 3) == "int") {
		delete (int *)data;
	} else {
		delete (string *)data;
	}
}

void deleteOneLine(void * * line, vector<Attribute> attrHeader) {
	for (unsigned int i = 0; i < attrHeader.size(); ++i) {
		deleteType(line[i], attrHeader[i].getType());
	}
}

void deleteData(void * * data, vector<Attribute> attrHeader, vector<Dimension> &dim, unsigned int posDim) {
	for (int i = 0; i < dim[posDim].getSize(); ++i) {
		if (posDim + 1 >= dim.size()) {
			deleteOneLine((void * *)data[i], attrHeader);
		} else {
			deleteData((void * *)data[i], attrHeader, dim, posDim + 1);
		}
		delete (void * *)data[i];
	}
}

void deleteData(void * * data, vector<Attribute> attrHeader, vector<Dimension> &dim) {
	if (dim.size() <= 0)
		deleteOneLine(data, attrHeader);
	else 
		deleteData(data, attrHeader, dim, 0);
	delete data;
}

//--------------------------------------------------------------------------------------------------------------------------
//Checks header
bool checkHeaders(vector<Dimension> dim, vector<Dimension> dimPatt, vector<Attribute> attrHeader, vector<Attribute> patternAttrHeader) {
	unsigned int j = 0;
	for (unsigned int i = 0; i < attrHeader.size(); ++i) {
		if (patternAttrHeader[j].getName() == attrHeader[i].getName() && patternAttrHeader[j].getType() == attrHeader[i].getType()) {
				j++;
		}
		if (j >= patternAttrHeader.size())
			break;
	}
	if (j != patternAttrHeader.size())
		return false;

	j = 0;
	for (unsigned int i = 0; i < dim.size(); ++i) {
		if (dimPatt[j].getName() == dim[i].getName() && dimPatt[j].getSize() <= dim[i].getSize()) {
				j++;
		}
		if (j >= dimPatt.size())
			break;
	}
	if (j != dimPatt.size())
		return false;

	return true;
}