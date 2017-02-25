#include <iostream>
#include <fstream>
#include <string>
#include <vector>
//#include "object.h"
#include "dimension.h"
//hlavicka patternu je podmnozina hlavicky dat
using namespace std;

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
		if(type == "dim") {
			dim.push_back(Dimension(name));
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
	//void * res;
	if(type.length() >= 3 && type.substr(0, 3) == "int") {

		return (void *)(new int(stoi(data)));
	} else {
		return (void *)(new string(data));
	}
}

vector<vector<void*> > readData(ifstream &file, vector<pair<string, string> > attrHeader, vector<Dimension> &dim) {
	vector<vector<void*> > res;
	int pos = 0;
	string data;
	//Object ** row;
	//row = new * Object[size];
	//res.resize(size, vector<Object> (num_of_rows, initial_value))
	// data budou serazeny podle dimenzi
	while(1) {
		getline(file, data, '\n');
		if (!file.good())
			break;
		res.push_back(vector<void*>());
		for (unsigned int i = 0; i < dim.size(); ++i) {
			pos = data.find(",");
			dim[i].addVal(data.substr(0, pos));
			data = data.substr(pos + 1);
		}
		for (unsigned int i = 0; i < attrHeader.size(); ++i) {
			pos = data.find(",");
			res[res.size() - 1].push_back(readType(attrHeader[i].second, data.substr(0, pos)));
			data = data.substr(pos + 1);
		}
	}
	return res;	
}

string toString(void * data, string type) {
	if(type.length() >= 3 && type.substr(0, 3) == "int") {
		return to_string(*((int*)data));
	} else
		return *((string*)data);
}

void printData(vector<vector<void*> > data, vector<pair<string, string> > attrHeader) {
	for (unsigned int i = 0; i < attrHeader.size(); ++i)	{
		cout << attrHeader[i].first << ", ";
	}
	cout << endl;

	cout << "Data: " << endl;
	for (unsigned int i = 0; i < data.size(); ++i) {
		//cout << data[i].size() << " " << attrHeader.size() << endl;
		for (unsigned int j = 0; j < data[i].size(); ++j) {
			cout << toString(data[i][j], attrHeader[j].second) << ", ";
		}
		cout << endl;
	}

}

int main(int argc, char* argv[])
{
	if(argc < 2) {
		cout << "Usage: " << argv[0] << " <INPUTFILE>" << endl;
		return 0;
	}
	string inpFile(argv[1]);
	ifstream file;
	file.open(inpFile.c_str());
	string value, tmp;

	vector<pair<string, string> > attrHeader;
	vector<Dimension> dim;

	getline(file, value, '\n');
	attrHeader = readHeader(value, dim);
	
	//for (unsigned int i = 0; i < attrHeader.size(); ++i)	{
	//	cout << attrHeader[i].first << " " << attrHeader[i].second << endl;
	//}	
	
	vector<vector<void*> > data;
	data = readData(file, attrHeader, dim);

	printData(data, attrHeader);
	/*
	for (unsigned int i = 0; i < attrHeader.size(); ++i)	{
		cout << attrHeader[i].first << ", ";
	}
	cout << endl;
	*/
	/*for (unsigned int i = 0; i < dim.size(); ++i)	{
		for (int j = 0; j < dim[i].getSize(); ++j) {
			cout << dim[i].getOneVal(j) << ", ";
		}
		cout << endl;
	}
	cout << "Data: " << endl;
	for (unsigned int i = 0; i < data.size(); ++i) {
		for (unsigned int j = 0; j < data[i].size(); ++j) {
			cout << data[i][j] << ", ";
		}
		cout << endl;
	}*/
	/*while(file.good()) {
		getline(file, value, '\n');

		cout << value << endl;
	}*/

	file.close();
	return 0;
}
