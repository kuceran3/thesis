#include <iostream>
#include <fstream>
#include <string>
#include <vector>
//#include "object.h"
#include "dimension.h"
//hlavicka patternu je podmnozina hlavicky dat, ve tvaru nazev:dim123, kde 123 je pocet unikatnich hodnot
using namespace std;

//File reading
vector<pair<string, string> > readHeader(string row, vector<Dimension> &dim){
	vector<pair<string, string> > res;
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
			res.push_back(pair<string, string>(name, type));
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

void * * readOneLine(string line, vector<pair<string, string> > attrHeader, vector<Dimension> &dim) {
	void * * res = new void * [attrHeader.size()];
	int pos;

	for (unsigned int i = 0; i < dim.size(); ++i) {
		pos = line.find(",");
		dim[i].addVal(line.substr(0, pos));
		line = line.substr(pos + 1);
	}

	for (unsigned int i = 0; i < attrHeader.size(); ++i) {
		pos = line.find(",");
		res[i] = readType(attrHeader[i].second, line.substr(0, pos));
		line = line.substr(pos + 1);
	}
	return res;
}

void * * readData(ifstream &file, vector<pair<string, string> > attrHeader, vector<Dimension> &dim, unsigned int posDim) {
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

//--------------------------------------------------------------------------------------------------------------------------
//Data printing
string toString(void * data, string type) {
	if(type.length() >= 3 && type.substr(0, 3) == "int") {
		return to_string(*((int*)data));
	} else
		return *((string*)data);
}

void printOneLine(void * * line, vector<pair<string, string> > attrHeader) {
	for (unsigned int i = 0; i < attrHeader.size(); ++i) {
		cout << toString(line[i], attrHeader[i].second) << ", ";
	}
	cout << endl;
}

void printData(void * * data, vector<pair<string, string> > attrHeader, vector<Dimension> &dim, unsigned int posDim) {
	for (int i = 0; i < dim[posDim].getSize(); ++i) {
		if (posDim + 1 >= dim.size()) {
			printOneLine((void * *)data[i], attrHeader);
		} else {
			printData((void * *)data[i], attrHeader, dim, posDim + 1);
		}
	}
	cout << endl;
}

void printData(void * * data, vector<pair<string, string> > attrHeader, vector<Dimension> &dim) {
	for (unsigned int i = 0; i < attrHeader.size(); ++i)	{
		cout << attrHeader[i].first << ", ";
	}

	cout << endl << "Data: " << endl;
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

void deleteOneLine(void * * line, vector<pair<string, string> > attrHeader) {
	for (unsigned int i = 0; i < attrHeader.size(); ++i) {
		deleteType(line[i], attrHeader[i].second);
	}
}

void deleteData(void * * data, vector<pair<string, string> > attrHeader, vector<Dimension> &dim, unsigned int posDim) {
	for (int i = 0; i < dim[posDim].getSize(); ++i) {
		if (posDim + 1 >= dim.size()) {
			deleteOneLine((void * *)data[i], attrHeader);
		} else {
			deleteData((void * *)data[i], attrHeader, dim, posDim + 1);
		}
		delete (void * *)data[i];
	}
}

void deleteData(void * * data, vector<pair<string, string> > attrHeader, vector<Dimension> &dim) {
	deleteData(data, attrHeader, dim, 0);
	delete data;
}

//--------------------------------------------------------------------------------------------------------------------------
//Returns upper left position of solution
int compareType(void * first, void * second, string type) {
	if(type.length() >= 3 && type.substr(0, 3) == "int") {
		return *(int *)first - *(int *)second;
	} else {
		return ((string *)first) -> compare(*(string *)second);
	}
}

bool checkOneLine(void * * data, void * * dataP, vector<pair<string, string> > attrH, vector<pair<string, string> > AttrHP) {
	int j = 0;
	for (unsigned int i = 0; i < attrH.size(); ++i) {
		if (attrH[i].first == AttrHP[j].first && attrH[i].second == AttrHP[j].second) {
			if (compareType(data[i], data[j], attrH[i].second) != 0) {
				return false;
			}
			++j;
		}
	}
	return true;
}

bool findB(void * * data, void * * dataP, vector<pair<string, string> > attrH, vector<pair<string, string> > AttrHP, \
	vector<Dimension> dim, vector<Dimension> dimP, int posDim, int posDimP) {

	for (int i = 0; i < dim[posDim].getSize(); ++i) {
		if (dim[posDim].getName() == dimP[posDimP].getName()) {
			if (dim[posDim].getOneVal(i) == dimP[posDimP].getOneVal(0)) {
				if (checkOneLine(data, dataP, attrH, AttrHP)) {
					if(findB(data, dataP, attrH, AttrHP, dim, dimP, posDim + 1, posDimP + 1)) {
						return true;
					}
				}
			}
		}
	}

	return false;
}

vector<vector<unsigned int> > find(void * * data, void * * dataP, vector<pair<string, string> > attrH, \
	vector<pair<string, string> > AttrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, unsigned int posDimP) {

	vector<vector<unsigned int> > res;
	for (int i = 0; i < dim[posDim].getSize(); ++i) {
		if (dim[posDim].getName() == dimP[posDimP].getName()) {
			if (dim[posDim].getOneVal(i) == dimP[posDimP].getOneVal(0)) {
				if (checkOneLine(data, dataP, attrH, AttrHP)) {
					if(findB(data, dataP, attrH, AttrHP, dim, dimP, posDim + 1, posDimP + 1)) {
						res[i].push_back(posDim);
						find(data, dataP, attrH, AttrHP, dim, dimP, posDim + 1, posDimP + 1);
					}
				}
			}
		} else {

			if (posDim + 1 >= dim.size()) {
				if (checkOneLine(data, dataP, attrH, AttrHP)) {
					res[i].push_back(posDim);
				}
				find(data, dataP, attrH, AttrHP, dim, dimP, posDim + 1, posDimP);
			}
		}
	}
	return res;
}



vector<vector<unsigned int> > find(void * * data, void * * dataP, vector<pair<string, string> > attrH, \
	vector<pair<string, string> > AttrHP, vector<Dimension> dim, vector<Dimension> dimP) {
	return find(data, dataP, attrH, AttrHP, dim, dimP, 0, 0);
}

void run(const char * in, const char * p) {
	string inpFile(in);
	ifstream file;
	file.open(inpFile.c_str());

	string inpPattern(p);
	ifstream pattern;
	pattern.open(inpPattern.c_str());
	
	string value, valuePatt;

	vector<pair<string, string> > attrHeader;
	vector<Dimension> dim;

	vector<pair<string, string> > patternAttrHeader;
	vector<Dimension> dimPatt;
	
	getline(file, value, '\n');
	attrHeader = readHeader(value, dim);
	
	getline(pattern, valuePatt, '\n');
	patternAttrHeader = readHeader(valuePatt, dimPatt);
	
	void * * data;
	void * * dataPatt;
	
	data = readData(file, attrHeader, dim, 0);
	dataPatt = readData(pattern, patternAttrHeader, dimPatt, 0);

	vector<vector<unsigned int> > res;
	res = find(data, dataPatt, attrHeader, patternAttrHeader, dim, dimPatt);

	for (unsigned int i = 0; i < res.size(); ++i) {
		for (unsigned int j = 0; j < res[i].size(); ++j) {
			cout << res[i][j] << ", ";
		}
	}
	cout << endl;
//	printData(data, attrHeader, dim);
//	printData(dataPatt, patternAttrHeader, dimPatt);

	deleteData(data, attrHeader, dim);
	deleteData(dataPatt, patternAttrHeader, dimPatt);

	pattern.close();
	file.close();
}

int main(int argc, char* argv[])
{
	if(argc < 3) {
		cout << "Usage: " << argv[0] << " <INPUTFILE>" << " <PATTERN>" << endl;
		return 0;
	}
	run(argv[1], argv[2]);

	return 0;
}
