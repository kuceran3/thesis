#include "appSplitDyn.h"

using namespace std;

int getPartSize(Dimension dim) {
	return (int)sqrt(dim.getSize());
}

bool checkRest(void * * data, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, unsigned int posDim, vector<unsigned int> indices) {
	if (posDim >= dim.size()) {
		return compareItem(data, dataP, attrH, attrHP);
	} else {
		return checkRest((void * *)data[indices[posDim]], dataP, attrH, attrHP, dim, posDim + 1, indices);			
	}
}

vector<vector<unsigned int> > checkFirst(void * * data, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, unsigned int posDim) {

	vector<vector<unsigned int> > res, returned;
	if (posDim >= dim.size()) {
		if (compareItem(data, dataP, attrH, attrHP)) {
			res.push_back(vector<unsigned int>(dim.size(), 0));
		}
		return res;
	}
	for (unsigned int i = 0; i < dim[posDim].getSize(); ++i) {
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
			if (!checkRest((void * *)data[j], (void * *)dataP[j], attrH, attrHP, dim, posDim + 1, returned[i])) {
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

		for (unsigned int i = 0; i < dimP[posDimP].getSize() - partSize + 1; i += partSize) {
			returned = checkPart(data, &dataP[i], attrH, attrHP, dim, posDim, partSize);
			for (unsigned int k = 0; k < returned.size(); ++k) {
				//if (i > returned[k][dimPositions[posDimP]]) 
				//	returned[k][dimPositions[posDimP]] = 0;
				//else
				returned[k][dimPositions[posDimP]] -= i;
				res.push_back(returned[k]);	
			}	
		}
	} else {
		for (unsigned int i = 0; i < dimP[posDimP].getSize(); ++i) {
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
	for (unsigned int i = dimP[posDimP].getSize() - partSize; i < dim[posDim].getSize(); i += dimP[posDimP].getSize()) {
		for (int j = 0; j < partSize; ++j)	{
			returned = checkParts(&data[i - j], dataP, attrH, attrHP, dim, dimP, posDim, 0, dimPositions);
			for (unsigned int k = 0; k < returned.size(); ++k) {
				returned[k][posDim] += (i - j);
				if (returned[k][posDim] + partSize < returned[k][posDim])
					returned[k][posDim] = 0;
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
			for (unsigned int i = dimP[posDimP].getSize() - 1; i < dim[posDim].getSize(); i += dimP[posDimP].getSize()) {
				returned = find((void * *)data[i], dataP, attrH, attrHP, dim, dimP, posDim + 1, posDimP + 1, dimPositions);
				for (unsigned int j = 0; j < returned.size(); ++j) {
					returned[j][posDim] += i;
					res.push_back(returned[j]);	
				}
			}
		}	
	} else {
		for (unsigned int i = 0; i < dim[posDim].getSize(); ++i) {
			returned = find((void * *)data[i], dataP, attrH, attrHP, dim, dimP, posDim + 1, posDimP, dimPositions);
			for (unsigned int j = 0; j < returned.size(); ++j) {
				returned[j][posDim] += i;
				res.push_back(returned[j]);
			}
		}
	}
	return res;
}

//returns sum of errors in 1 dimension
int dynDimCheck(void * * data, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int pos, vector<unsigned int> res) {

	vector<void * *> dataCol, dataPCol;
	vector<vector<unsigned int> > indices;
	vector<unsigned int> indicesP;
	unsigned int pos2 = 0;
	int sum = 0;

	for (unsigned int i = 0; i < dim.size(); ++i) {
		if (dimP[pos].getName() == dim[i].getName()) {
			pos2 = i;
			break;
		}
	}

	indices = getIndices(dim, dimP, pos, res, 0, 0);

	unsigned int posDimP;
	for (unsigned int i = 0; i < indices.size(); ++i) {
		indicesP.clear();
		posDimP = 0;
		for (unsigned int j = 0; j < indices[i].size(); ++j) {
			if (dim[j].getName() == dimP[posDimP].getName()){
				indicesP.push_back(indices[i][j] - res[j]);
				posDimP++;
				if (posDimP >= dimP.size()) break;
			}
		}
		dataCol = getDim(data, pos2, dimP[pos].getSize(), indices[i], 0);
		dataPCol = getDim(dataP, pos, dimP[pos].getSize(), indicesP, 0);
		sum += editDistance(dataCol, dataPCol, attrH, attrHP);
	}

	return sum;
}

// max error d*m^(d)
bool dynCheck(void * * data, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, vector<unsigned int> res){

	vector<unsigned int> indices;
	vector<int> err;

	for (unsigned int i = 0; i < dimP.size(); ++i) {
		err.push_back(dynDimCheck(data, dataP, attrH, attrHP, dim, dimP, i, res));
	}

	//check suma chyb v err, mensi nez ... ok
	int sum = 0;
	for (unsigned int i = 0; i < err.size(); ++i) {
		sum += err[i];
	}
	if (sum < 2) {
		return true;
	}

	return false;
}

vector<vector<unsigned int> > find(void * * data, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP) {

	vector<vector<unsigned int> > res = find(data, dataP, attrH, attrHP, dim, dimP, 0, 0, vector<unsigned int>());
	//Approximate check of the rest of the pattern
	for (vector<vector<unsigned int> >::iterator it = res.begin(); it != res.end();) {
		if (!dynCheck(data, dataP, attrH, attrHP, dim, dimP, *it))
			it = res.erase(it);
		else
			++it;
	}
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
	cout << "Finding..." << endl;
	vector<vector<unsigned int> > res;
	res = find(data, dataPatt, attrHeader, patternAttrHeader, dim, dimPatt);
	if (res.size() == 0){
		cout << "no solutions found" << endl;
	} else {
		for (unsigned int i = 0; i < res.size(); ++i) {
			for (unsigned int j = 0; j < res[i].size(); ++j) {
				cout << res[i][j] << ", ";
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
	const clock_t begin_time = clock();
	run(argv[1], argv[2]);
	cout << float( clock () - begin_time ) /  CLOCKS_PER_SEC;
	return 0;
}
