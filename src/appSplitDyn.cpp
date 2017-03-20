#include "appSplitDyn.h"

//hlavicka patternu je podmnozina hlavicky dat, ve tvaru nazev:dim123, kde 123 je pocet unikatnich hodnot
using namespace std;

int getPartSize(Dimension dim) {
	return (int)sqrt(dim.getSize());
}

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

bool checkRest(void * * data, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, unsigned int posDim, vector<unsigned int> indices) {
	if (posDim >= dim.size()) {
		return checkOneLine(data, dataP, attrH, attrHP);
	} else {
		return checkRest((void * *)data[indices[posDim]], dataP, attrH, attrHP, dim, posDim + 1, indices);			
	}
}

vector<vector<unsigned int> > checkFirst(void * * data, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, unsigned int posDim) {

	vector<vector<unsigned int> > res, returned;
	if (posDim >= dim.size()) {
		if (checkOneLine(data, dataP, attrH, attrHP)) {
			res.push_back(vector<unsigned int>(dim.size(), 0));
		}
		return res;
	}

	for (int i = 0; i < dim[posDim].getSize(); ++i) {
	 	returned = checkFirst((void * *)data[i], dataP, attrH, attrHP, dim, posDim + 1);
		for (unsigned int j = 0; j < returned.size(); ++j) {
			returned[j][posDim] += i;
			res.push_back(returned[j]);
		}
	}
	return res;
}

vector<vector<unsigned int> > checkPart(void * * data, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, unsigned int posDim, int partSize) {

	vector<vector<unsigned int> > res, returned;
	bool isRes = true;

	returned = checkFirst((void * *)data[0], (void * *)dataP[0], attrH, attrHP, dim, posDim + 1);

	for (unsigned int i = 0; i < returned.size(); ++i) {
		isRes = true;
		for (int j = 1; j < partSize; ++j) {
			if (!checkRest((void * *)data[j], (void * *)dataP[j], attrH, attrHP, dim, posDim, returned[i])) {
				isRes = false;
				break;
			}
		}

		if (isRes) {
			res.push_back(returned[i]);
		}
	}
	return res;
}

vector<vector<unsigned int> > checkParts(void * * data, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, unsigned int posDimP, vector<unsigned int> dimPositions) {

	vector<vector<unsigned int> > res, returned;

	if (posDimP + 1 >= dimP.size()) {
		int partSize = getPartSize(dimP[posDimP]);

		for (int i = 0; i < dimP[posDimP].getSize() - partSize + 1; i += partSize) {
			returned = checkPart(data, &dataP[i], attrH, attrHP, dim, posDim, partSize);
			for (unsigned int k = 0; k < returned.size(); ++k) {
				returned[k][dimPositions[posDimP]] -= i;
				res.push_back(returned[k]);	
			}	
		}
	} else {
		for (int i = 0; i < dimP[posDimP].getSize(); ++i) {
			returned = checkParts(data, (void * *)dataP[i], attrH, attrHP, dim, dimP, posDim, posDimP + 1, dimPositions);
			for (unsigned int k = 0; k < returned.size(); ++k) {
				//returned[k][dimPositions[posDimP]] -= (dim[posDimP].getSize() - 1 - i);
				returned[k][dimPositions[posDimP]] -= i;
				res.push_back(returned[k]);
			}		
		}
	}
	return res;
}

//split pattern into sqrt(pattern.size()) parts
vector<vector<unsigned int> > findParts(void * * data, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, unsigned int posDimP, vector<unsigned int> dimPositions) {

	vector<vector<unsigned int> > res, returned;
	int partSize = getPartSize(dimP[posDimP]);
	for (int i = dimP[posDimP].getSize() - partSize; i < dim[posDim].getSize(); i += dimP[posDimP].getSize()) {
		for (int j = 0; j < partSize; ++j)	{
			returned = checkParts(&data[i - j], dataP, attrH, attrHP, dim, dimP, posDim, 0, dimPositions);
			for (unsigned int k = 0; k < returned.size(); ++k) {
				returned[k][posDim] += (i - j);
				res.push_back(returned[k]);	
			}	
		}
	}
	return res;
} 

vector<vector<unsigned int> > find(void * * data, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, \
	unsigned int posDimP, vector<unsigned int> dimPositions) {

	vector<vector<unsigned int> > res, returned;
	vector<unsigned int> one;

	if (posDimP < dimP.size() && dim[posDim].getName() == dimP[posDimP].getName()) {
		dimPositions.push_back(posDim);
		if (posDimP + 1 >= dimP.size()) {
			returned = findParts(data, dataP, attrH, attrHP, dim, dimP, posDim, posDimP, dimPositions);
			for (unsigned int j = 0; j < returned.size(); ++j) {
				//returned[j][posDim] += i;
				res.push_back(returned[j]);	
			}
		} else {
			for (int i = dimP[posDimP].getSize() - 1; i < dim[posDim].getSize(); i += dimP[posDimP].getSize()) {
				returned = find((void * *)data[i], dataP, attrH, attrHP, dim, dimP, posDim + 1, posDimP + 1, dimPositions);
				for (unsigned int j = 0; j < returned.size(); ++j) {
					returned[j][posDim] += i;
					res.push_back(returned[j]);	
				}
			}
		}	
	} else {
		for (int i = 0; i < dim[posDim].getSize(); ++i) {
			returned = find((void * *)data[i], dataP, attrH, attrHP, dim, dimP, posDim + 1, posDimP, dimPositions);
			for (unsigned int j = 0; j < returned.size(); ++j) {
				returned[j][posDim] += i;
				res.push_back(returned[j]);
			}
		}
	}
	return res;
}



vector<vector<unsigned int> > find(void * * data, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP) {

	vector<vector<unsigned int> > res = find(data, dataP, attrH, attrHP, dim, dimP, 0, 0, vector<unsigned int>()), sol;

	//Approximate check of the rest of the pattern

	//sol = dynCheck(data, dataP, attrH, attrHP, dim, dimP, res);

	return res;
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

	if (!checkHeaders(dim, dimPatt, attrHeader, patternAttrHeader)) {
		cout << "Invalid pattern" << endl;
		return;
	}
	
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
			//for (unsigned int j = 0; j < res[i].size(); ++j) {
			//	cout << dim[j].getOneVal(res[i][j]) << ", ";
			//}
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
