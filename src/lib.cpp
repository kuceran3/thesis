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
			dim.push_back(Dimension(name, (unsigned int)stoi(type.substr(3))));
		} else {
			res.push_back(Attribute(name, type));
		}
		if (pos2 == (int)row.length())
			break;
		row = row.substr(pos2 + 1);
	}

	return res;
}

int readDimNum(string row) {
	int pos = 8; //dimName:_ 
	cout << row << endl;
	string num = row.substr(pos, row.length() - 1);
	return stoi(num);
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

	for (unsigned int i = 0; i < dim[posDim].getSize(); ++i) {
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
	for (unsigned int i = 0; i < dim[posDim].getSize(); ++i) {
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
	for (unsigned int i = 0; i < dim[posDim].getSize(); ++i) {
		if (posDim + 1 >= dim.size()) {
			deleteOneLine((void * *)data[i], attrHeader);
		} else {
			deleteData((void * *)data[i], attrHeader, dim, posDim + 1);
		}
		delete [] (void * *)data[i];
	}
}

void deleteData(void * * data, vector<Attribute> attrHeader, vector<Dimension> &dim) {
	if (dim.size() <= 0)
		deleteOneLine(data, attrHeader);
	else 
		deleteData(data, attrHeader, dim, 0);
	delete [] data;
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

int compareType(void * first, void * second, string type) {
	if(type.length() >= 3 && type.substr(0, 3) == "int") {
		return *(int *)first - *(int *)second;
	} else {
		return ((string *)first) -> compare(*(string *)second);
	}
}

bool compareItem(void * * data, void * * dataP, vector<Attribute> attrH, vector<Attribute> attrHP) {
	unsigned int j = 0;

	if (data == NULL)
		return false;

	for (unsigned int i = 0; i < attrH.size(); ++i) {
		if (j >= attrHP.size()) break;
		if (attrH[i].getName() == attrHP[j].getName() && attrH[i].getType() == attrHP[j].getType()) {
			//cout << *(int *)data << endl;
			//cout << *(int *)dataP[j] << endl;
			if (compareType(data[i], dataP[j], attrH[i].getType()) != 0) {
				return false;
			}
			++j;
		}
	}
	if (j < attrHP.size())
		return false;
	return true;
}
//--------------------------------------------------------------------------------------------------------------------------
//Framework for computing distances
void * * getItem(void * * data, vector<unsigned int> indices, unsigned int posDim) {
	if (posDim >= indices.size()) {
		return data;
	} else {
		return getItem((void * *)data[indices[posDim]], indices, posDim + 1);
	}
}

void * * getItem(Reader * cache, vector<unsigned int> indices) {
	void * * data = NULL;		
	vector<unsigned int> dataIndices;
	for (unsigned int i = 0; i < cache->getDimInName(); ++i) {
		dataIndices.push_back(indices[i]);
	}
	data = cache->read(dataIndices);
	return getItem((void * *)data[indices[cache->getDimInName()]], indices, cache->getDimInName() + 1);
}

vector<void * *> getDim(void * * data, unsigned int dimInd, unsigned int length, vector<unsigned int> indices, unsigned int posDim) {
	vector<void * *> res;

	if (posDim == dimInd) {
		for (unsigned int i = indices[posDim]; i < indices[posDim] + length; ++i) {
			res.push_back(getItem((void * *)data[i], indices, posDim + 1));
		}
	} else {
		res = getDim((void * *)data[indices[posDim]], dimInd, length, indices, posDim + 1);
	}

	return res;
}

vector<void * *> getDim(Reader * cache, unsigned int dimInd, unsigned int length, vector<unsigned int> indices, unsigned int posDim) {
	vector<void * *> res;

	if (posDim < cache->getDimInName()) {
		if (posDim == dimInd) {
			for (unsigned int i = 0; i < length; ++i) {
				res.push_back(getItem(cache, indices));
				indices[posDim] += 1;
			}
		} else {
			res = getDim(cache, dimInd, length, indices, cache->getDimInName());
		}
	} else {
		vector<unsigned int> dataIndices;
		void * * data = NULL;
		
		if (posDim == cache->getDimInName()) {
			for (unsigned int i = 0; i < posDim; ++i) {
				dataIndices.push_back(indices[i]);
			}
			data = cache->read(dataIndices);
		}
		if (posDim == dimInd) {
			for (unsigned int i = indices[posDim]; i < indices[posDim] + length; ++i) {
				res.push_back(getItem((void * *)data[i], indices, posDim + 1));
			}
		} else {
			res = getDim((void * *)data[indices[posDim]], dimInd, length, indices, posDim + 1);
		}
	}
	
	return res;
}

vector<vector<unsigned int> > getIndices(vector<Dimension> dim, vector<Dimension> dimP, unsigned int pos, vector<unsigned int> res, unsigned int posDim, unsigned int posDimP) {

	vector<vector<unsigned int> > indices, ret;

	unsigned int max;
	if (posDimP >= dimP.size()) {
		indices.push_back(vector<unsigned int>(res.begin() + posDim, res.end()));
		return indices;
	} 

	if (posDimP == pos) {
		indices = getIndices(dim, dimP, pos, res, posDim + 1, posDimP + 1);
		//cout << indices.size() << " " << res[posDim] <<  endl;
		for (unsigned int i = 0; i < indices.size(); ++i) {
			indices[i].insert(indices[i].begin(), res[posDim]);
		}
	} else if (dim[posDim].getName() != dimP[posDimP].getName()) {
		indices = getIndices(dim, dimP, pos, res, posDim + 1, posDimP);
		for (unsigned int i = 0; i < indices.size(); ++i) {
			indices[i].insert(indices[i].begin(), res[posDim]);
		}
	} else {
		ret = getIndices(dim, dimP, pos, res, posDim + 1, posDimP + 1);
		for (unsigned int i = 0; i < ret.size(); ++i) {
			ret[i].insert(ret[i].begin(), 0);
		}
		max = (res[posDim] + dimP[posDimP].getSize() > dim[posDim].getSize()) ? dim[posDim].getSize() : (res[posDim] + dimP[posDimP].getSize());
		for (unsigned int i = res[posDim]; i < max; ++i) {
			for (unsigned int j = 0; j < ret.size(); ++j) {
				ret[j][0] = i;
				indices.push_back(ret[j]);
			}
		}
	} 
	
	return indices;
}
//--------------------------------------------------------------------------------------------------------------------------
//Edit distance
int editDistance(vector<void * *> col, vector<void * *> colP, vector<Attribute> attrH, vector<Attribute> attrHP) {

	int l1 = col.size() + 1;
	int l2 = colP.size() + 1;
	int res = 0;
	int * * table = new int*[l1];

	for (int i = 0; i < l1; ++i) {
		table[i] = new int[l2];
		table[i][0] = i;
	}
	for (int i = 0; i < l2; ++i) {
		table[0][i] = i;
	}

	for (int i = 1; i < l1; ++i) {
		for (int j = 1; j < l2; ++j) {
			if (compareItem(col[i - 1], colP[j - 1], attrH, attrHP)) {
				table[i][j] = table[i - 1][j - 1];
			} else {
				table[i][j] = table[i - 1][j] + 1;
				if (table[i][j - 1] + 1 < table[i][j]) {
					table[i][j] = table[i][j - 1] + 1;
				}
				if (table[i - 1][j - 1] + 1 < table[i][j]) {
					table[i][j] = table[i - 1][j - 1] + 1;
				}
			}
		}
	}

	res = table[l1 - 1][l2 - 1];
	for (int i = 0; i < l1; ++i) {
		delete table[i];
	}
	delete table;
	return res;
}
