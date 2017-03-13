//#include <iostream>
//#include <fstream>
//#include <string>
//#include <vector>
//#include "object.h"
#include "dimension.h"
#include "brute.h"
#include "attribute.h"

//hlavicka patternu je podmnozina hlavicky dat, ve tvaru nazev:dim123, kde 123 je pocet unikatnich hodnot
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
//Returns upper left position of solution
int compareType(void * first, void * second, string type) {
	if(type.length() >= 3 && type.substr(0, 3) == "int") {
		return *(int *)first - *(int *)second;
	} else {
		return ((string *)first) -> compare(*(string *)second);
	}
}

bool checkOneLine(void * * data, void * * dataP, vector<Attribute> attrH, vector<Attribute> attrHP) {
	unsigned int j = 0;
	for (unsigned int i = 0; i < attrH.size(); ++i) {
		if (j >= attrHP.size()) break;
		if (attrH[i].getName() == attrHP[j].getName() && attrH[i].getType() == attrHP[j].getType()) {
			//cout << "Comparing ";
			//cout << i << " " << j << endl;
			//cout << *((string*)data) << " first " << attrH[i].getType() << endl;
			if (compareType(data[i], dataP[j], attrH[i].getType()) != 0) {
				//cout << "compare false" << endl;
				return false;
			}
			++j;
		}
	}
	if (j < attrHP.size())
		return false;
	//cout << "compare true" << endl;
	return true;
}

bool findRest(void * * data, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, unsigned int posDimP, vector<unsigned int> indices, int posInd) {

	if (posDimP < dimP.size() && dim[posDim].getName() == dimP[posDimP].getName()) {
		if (dim[posDim].getOneVal(indices[posInd]) == dimP[posDimP].getOneVal(0)) {
			if (posDim + 1 >= dim.size()) {
				for (int j = 0; j < dimP[posDimP].getSize(); ++j) {
					if (!checkOneLine((void * *)data[indices[posInd] + j], (void * *)dataP[j], attrH, attrHP)) {
						return false;
					}
				}
				return true;
			} else {
				for (int k = 0; k < dimP[posDimP].getSize(); ++k) {
					if (!findRest((void * *)data[indices[posInd] + k], (void * *)dataP[k], attrH, attrHP, dim, dimP, posDim + 1, posDimP + 1, indices, posInd + 1)) {
						return false;
					}
				}
				return true;
			}
		}
		return false;
	} else {
		if (posDim + 1 >= dim.size()) {
			return checkOneLine((void * *)data[indices[posInd]], dataP, attrH, attrHP);
		} else {
			return findRest((void * *)data[indices[posInd]], dataP, attrH, attrHP, dim, dimP, posDim + 1, posDimP, indices, posInd + 1);			
		}
	}
	//return true;
}

vector<vector<unsigned int> > find(void * * data, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, unsigned int posDimP) {

	vector<vector<unsigned int> > res, returned;
	vector<unsigned int> one;
	bool isRes;
	//cout << "Dim: " << dim[posDim].getName() << endl;
	for (int i = 0; i < dim[posDim].getSize(); ++i) {
		//find dimensions with the same name
		if (posDimP < dimP.size() && dim[posDim].getName() == dimP[posDimP].getName()) {
			if (i + dimP[posDimP].getSize() > dim[posDim].getSize()) break;
			//find same value in the dimension
			if (dim[posDim].getOneVal(i) == dimP[posDimP].getOneVal(0)) {
				if (posDim + 1 >= dim.size()) {
					if (checkOneLine((void * *)data[i], (void * *)dataP[0], attrH, attrHP)) {
						isRes = true;
						for (int j = 1; j < dimP[posDimP].getSize(); ++j) {
							if (dim[posDim].getOneVal(i + j) != dimP[posDimP].getOneVal(j)) {
								isRes = false;
								break;
							}
							if (!checkOneLine((void * *)data[i + j], (void * *)dataP[j], attrH, attrHP)) {
								isRes = false;
								break;
							}
								
						}
						if (isRes) {
							one.push_back(i);
							res.push_back(one);
							one.clear();
						}
					}
				} else {
					returned = find((void * *)data[i], (void * *)dataP[0], attrH, attrHP, dim, dimP, posDim + 1, posDimP + 1);
					for (unsigned int j = 0; j < returned.size(); ++j) {
						isRes = true;
						for (int k = 1; k < dimP[posDimP].getSize(); ++k) {
							if (dim[posDim].getOneVal(i + k) != dimP[posDimP].getOneVal(k)) {
								isRes = false;
								break;
							}
							if (!findRest((void * *)data[i + k], (void * *)dataP[k], attrH, attrHP, dim, dimP, posDim + 1, posDimP + 1, returned[j], 0)) {
								isRes = false;
								break;
							}
						}
						if (isRes) {
							returned[j].insert(returned[j].begin(), i);
							res.push_back(returned[j]);	
						}
					}
				}
				break;
			}
		} else {
			if (posDim + 1 >= dim.size()) {
				if (checkOneLine((void * *)data[i], dataP, attrH, attrHP)) {
					one.push_back(i);
					res.push_back(one);
					one.clear();
				}
			} else {
				returned = find((void * *)data[i], dataP, attrH, attrHP, dim, dimP, posDim + 1, posDimP);
				for (unsigned int j = 0; j < returned.size(); ++j) {
					returned[j].insert(returned[j].begin(), i);
					res.push_back(returned[j]);
				}
			}
		}
	}
	return res;
}

vector<vector<unsigned int> > find(void * * data, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP) {
	return find(data, dataP, attrH, attrHP, dim, dimP, 0, 0);
}

void run(const char * in, const char * p) {
	string inpFile(in);
	ifstream file;
	file.open(inpFile.c_str());

	string inpPattern(p);
	ifstream pattern;
	pattern.open(inpPattern.c_str());
	
	string value, valuePatt;

	vector<Attribute> attrHeader;
	vector<Dimension> dim;

	vector<Attribute> patternAttrHeader;
	vector<Dimension> dimPatt;
	
	getline(file, value, '\n');
	attrHeader = readHeader(value, dim);
	
	getline(pattern, valuePatt, '\n');
	patternAttrHeader = readHeader(valuePatt, dimPatt);
	
	void * * data;
	void * * dataPatt;
	
	data = readData(file, attrHeader, dim);
	dataPatt = readData(pattern, patternAttrHeader, dimPatt);
	cout << " Find" << endl;
	vector<vector<unsigned int> > res;
	res = find(data, dataPatt, attrHeader, patternAttrHeader, dim, dimPatt);
	if (res.size() == 0){
		cout << "no solutions found" << endl;
	} else {
		for (unsigned int i = 0; i < res.size(); ++i) {
			for (unsigned int j = 0; j < res[i].size(); ++j) {
				cout << res[i][j] << ", ";
			}
			cout << " ... ";
			for (unsigned int j = 0; j < res[i].size(); ++j) {
				cout << dim[j].getOneVal(res[i][j]) << ", ";
			}
			cout << endl;
		}
	}
	//printData(data, attrHeader, dim);
	//printData(dataPatt, patternAttrHeader, dimPatt);

	deleteData(data, attrHeader, dim);
	deleteData(dataPatt, patternAttrHeader, dimPatt);

	pattern.close();
	file.close();
}

int main(int argc, char* argv[]) {
	if(argc < 3) {
		cout << "Usage: " << argv[0] << " <INPUTFILE>" << " <PATTERN>" << endl;
		return 0;
	}
	run(argv[1], argv[2]);
	return 0;
}
