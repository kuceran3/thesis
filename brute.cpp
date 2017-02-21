#include <iostream>
#include <fstream>
#include <String>
#include <vector>
//#include "object.h"
#include "dimension.h"

using namespace std;

vector<pair<string, string> > readHeader(string row, vector<Dimension> &dim){
	vector<pair<string, string> > res;
	int pos1, pos2, len2;
	string name, type;
	
	while(1) {
		pos1 = row.find(":");
		pos2 = row.find(",");
		if (pos2 == -1)
			pos2 = int(row.length());
		len2 = pos2 - pos1 - 1;
		name = row.substr(0, pos1);
		type = row.substr(pos1 + 1, len2);
		if(type == "dim") {
			dim.push_back(Dimension(name));
		}

		res.push_back(pair<string, string>(name, type));
		if (pos2 == int(row.length()))
			break;
		row = row.substr(pos2 + 1);
	}

	return res;
}

vector<vector<string> > readData(ifstream &file, vector<pair<string, string> > header, vector<Dimension> &dim) {
	vector<vector<string> > res;
	int pos = 0;
	string tmp, data;
	//Object ** row;
	//row = new * Object[size];
	//res.resize(size, vector<Object> (num_of_rows, initial_value))
	// data budou serazeny podle dimenzi
	while(1) {
		getline(file, tmp, '\n');
		if (!file.good())
			break;
		res.push_back(vector<string>());
		for (unsigned int i = 0; i < header.size(); ++i)	{
			pos = tmp.find(",");
			data = tmp.substr(0, pos);
			tmp = tmp.substr(pos + 1);
			if(header[i].second == "dim"){
				dim[i].addVal(data);
			} else {
				res[res.size() - 1].push_back(data);
			}
		}
	}
	return res;	
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

	vector<pair<string, string> > header;
	vector<Dimension> dim;

	getline(file, value, '\n');
	header = readHeader(value, dim);
	
	//for (unsigned int i = 0; i < header.size(); ++i)	{
	//	cout << header[i].first << " " << header[i].second << endl;
	//}	
	
	vector<vector<string> > data;
	data = readData(file, header, dim);
	for (unsigned int i = 0; i < header.size(); ++i)	{
		cout << header[i].first << ", ";
	}
	cout << endl;

	for (unsigned int i = 0; i < dim.size(); ++i)	{
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
	}
	/*while(file.good()) {
		getline(file, value, '\n');

		cout << value << endl;
	}*/

	file.close();
	return 0;
}
