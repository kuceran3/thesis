#include "appSplitDyn_chunks.h"

using namespace std;
//static int calls = 0;
//static int callsCP = 0;
//static int callsParts = 0;
//static int callsFind = 0;
//static int compares1 = 0;
//static int compares2 = 0;

int getPartSize(Dimension dim) {
	//return 1;
	return (int)sqrt(dim.getSize());
}

bool checkRest(void * * data, Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, unsigned int posDim, vector<unsigned int> indices, vector<unsigned int> cacheInd) {
	//++calls;

	if (posDim == cache->getDimInName()) {
		data = cache->read(cacheInd);
	}

	if (posDim + 1 >= dim.size()) {
		//++compares2;
		return compareItem((void * *)data[indices[posDim]], dataP, attrH, attrHP);
	} else {
		if (posDim < cache->getDimInName()) {
			cacheInd[posDim] = indices[posDim];
	 		return checkRest(NULL, cache, dataP, attrH, attrHP, dim, posDim + 1, indices, cacheInd);			
	 	} else {
	 		return checkRest((void * *)data[indices[posDim]], cache, dataP, attrH, attrHP, dim, posDim + 1, indices, cacheInd);			
	 	}
	}
}

vector<vector<unsigned int> > checkFirst(void * * data, Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, unsigned int posDim, vector<unsigned int> cacheInd) {
	//++calls;

	vector<vector<unsigned int> > res, returned;
	if (posDim == cache->getDimInName()) {
		data = cache->read(cacheInd);
	}	

	if (posDim + 1 >= dim.size()) {
		for (unsigned int i = 0; i < dim[posDim].getSize(); ++i) {
			//++compares1;
		 	if (compareItem((void * *)data[i], dataP, attrH, attrHP)) {
				res.push_back(vector<unsigned int>(dim.size() - 1, 0));
				res[res.size() - 1].push_back(i);
			}
		}
	} else {
		for (unsigned int i = 0; i < dim[posDim].getSize(); ++i) {
			if (posDim < cache->getDimInName()) {
				cacheInd[posDim] = i;
		 		returned = checkFirst(NULL, cache, dataP, attrH, attrHP, dim, posDim + 1, cacheInd);
		 	} else {
		 		returned = checkFirst((void * *)data[i], cache, dataP, attrH, attrHP, dim, posDim + 1, cacheInd);
		 	}
			for (unsigned int j = 0; j < returned.size(); ++j) {
				returned[j][posDim] += i;
				res.push_back(returned[j]);
			}
		}
	}
	return res;
}

vector<vector<unsigned int> > checkPart(void * * data, Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, unsigned int posDim, int partSize, vector<unsigned int> cacheInd) {
	//++calls;

	vector<vector<unsigned int> > res, returned;
	bool isRes = true;
	int start = 0;
	if (posDim < cache->getDimInName()) {
		start = cacheInd[posDim];
		returned = checkFirst(NULL, cache, (void * *)dataP[0], attrH, attrHP, dim, posDim + 1, cacheInd);
		for (unsigned int i = 0; i < returned.size(); ++i) {
			isRes = true;
			for (int j = 1; j < partSize; ++j) {
				cacheInd[posDim] = start + j;
				if (!checkRest(NULL, cache, (void * *)dataP[j], attrH, attrHP, dim, posDim + 1, returned[i], cacheInd)) {
					isRes = false;
					break;
				}
			}

			if (isRes) {
				res.push_back(returned[i]);
			}
		}
	} else {
		/*if (posDim == cache->getDimInName()) {
			data = cache->read(cacheInd);
			//start = cacheInd[posDim];
		}*/
		if (posDim + 1 >= dim.size()) {
			//++compares1;
			if (compareItem((void * *)data[0], (void * *)dataP[0], attrH, attrHP)) {
				for (int j = 1; j < partSize; ++j) {	
				//++compares2;
					if (!compareItem((void * *)data[j], (void * *)dataP[j], attrH, attrHP)) {
						return res;
					}
				}
				res.push_back(vector<unsigned int>(dim.size(), 0));
				return res;
			}
		} else {
			returned = checkFirst((void * *)data[0], cache, (void * *)dataP[0], attrH, attrHP, dim, posDim + 1, cacheInd);
			for (unsigned int i = 0; i < returned.size(); ++i) {
				isRes = true;
				for (int j = 1; j < partSize; ++j) {
					if (!checkRest((void * *)data[j], cache, (void * *)dataP[j], attrH, attrHP, dim, posDim + 1, returned[i], cacheInd)) {
						isRes = false;
						break;
					}
				}

				if (isRes) {
					res.push_back(returned[i]);
				}
			}
		}
	}
	return res;
}

vector<vector<unsigned int> > checkParts(void * * data, Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, unsigned int posDimP, vector<unsigned int> dimPositions, vector<unsigned int> cacheInd) {

	//++callsCP;
	vector<vector<unsigned int> > res, returned;

	//if (posDimP + 1 >= dimP.size()) {
		int partSize = getPartSize(dimP[posDimP]);

		for (unsigned int i = 0; i < dimP[posDimP].getSize() - partSize + 1; i += partSize) {
			returned = checkPart(data, cache, &dataP[i], attrH, attrHP, dim, posDim, partSize, cacheInd);

			for (unsigned int k = 0; k < returned.size(); ++k) {
				//if (i > returned[k][dimPositions[posDimP]]) 
				//	returned[k][dimPositions[posDimP]] = 0;
				//else
				returned[k][dimPositions[posDimP]] -= i;
				res.push_back(returned[k]);	
			}	
		}
	/*} else {
		for (unsigned int i = 0; i < dimP[posDimP].getSize(); ++i) {
			returned = checkParts(data, (void * *)dataP[i], attrH, attrHP, dim, dimP, posDim, posDimP + 1, dimPositions);
			for (unsigned int k = 0; k < returned.size(); ++k) {
				//returned[k][dimPositions[posDimP]] -= (dim[posDimP].getSize() - 1 - i);
				returned[k][dimPositions[posDimP]] -= i;
				res.push_back(returned[k]);
			}		
		}
	}*/
	return res;
}

//split pattern into sqrt(pattern.size()) parts
vector<vector<unsigned int> > findParts(void * * data, Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, unsigned int posDimP, vector<unsigned int> dimPositions, vector<unsigned int> cacheInd) {
	//++callsParts;

	vector<vector<unsigned int> > res, returned;
	int partSize = getPartSize(dimP[posDimP]);
	for (unsigned int i = dimP[posDimP].getSize() - partSize; i < (dim[posDim].getSize() - partSize + 1); i += dimP[posDimP].getSize()) {
		for (int j = 0; j < partSize; ++j)	{
			if (posDim < cache->getDimInName()) {
				cacheInd[posDim] = i - j;
				returned = checkParts(NULL, cache, dataP, attrH, attrHP, dim, dimP, posDim, posDimP, dimPositions, cacheInd);
			} else {
				returned = checkParts(&data[i - j], cache, dataP, attrH, attrHP, dim, dimP, posDim, posDimP, dimPositions, cacheInd);				
			}
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

vector<vector<unsigned int> > find(void * * data, Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, \
	unsigned int posDimP, vector<unsigned int> dimPositions, vector<unsigned int> cacheInd) {

	//++callsFind;
	vector<vector<unsigned int> > res, returned;
	vector<unsigned int> one;

	if (posDim == cache->getDimInName()) {
		data = cache->read(cacheInd);
	}

	if (posDimP < dimP.size() && dim[posDim].getName() == dimP[posDimP].getName()) {
		dimPositions.push_back(posDim);
		if (posDimP + 1 >= dimP.size()) {
			returned = findParts(data, cache, dataP, attrH, attrHP, dim, dimP, posDim, posDimP, dimPositions, cacheInd);
			for (unsigned int j = 0; j < returned.size(); ++j) {
				//returned[j][posDim] += i;
				res.push_back(returned[j]);	
			}
		} else {
			for (unsigned int i = dimP[posDimP].getSize() - 1; i < dim[posDim].getSize(); i += dimP[posDimP].getSize()) {
				for (unsigned int j = 0; j < dimP[0].getSize(); ++j) {
					if (posDim < cache->getDimInName()) {
						cacheInd[posDim] = i;
						returned = find(NULL, cache, (void * *)dataP[j], attrH, attrHP, dim, dimP, posDim + 1, posDimP + 1, dimPositions, cacheInd);
					} else {
						returned = find((void * *)data[i], cache, (void * *)dataP[j], attrH, attrHP, dim, dimP, posDim + 1, posDimP + 1, dimPositions, cacheInd);
					}
					//if (returned.size() > 0) {
					//}
					for (unsigned int k = 0; k < returned.size(); ++k) {
						returned[k][posDim] += (i - j);
						res.push_back(returned[k]);	
					}
				}
			}
		}	
	} else {
		for (unsigned int i = 0; i < dim[posDim].getSize(); ++i) {
			if (posDim < cache->getDimInName()) {
				cacheInd[posDim] = i;
				returned = find(NULL, cache, dataP, attrH, attrHP, dim, dimP, posDim + 1, posDimP, dimPositions, cacheInd);
			} else {
				returned = find((void * *)data[i], cache, dataP, attrH, attrHP, dim, dimP, posDim + 1, posDimP, dimPositions, cacheInd);
			}
			for (unsigned int j = 0; j < returned.size(); ++j) {
				returned[j][posDim] += i;
				res.push_back(returned[j]);
			}
		}
	}
	return res;
}

//returns sum of errors in 1 dimension
int dynDimCheck(Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int pos, vector<unsigned int> res, int errors) {

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


	unsigned int posDimP, length;
	for (unsigned int i = 0; i < indices.size(); ++i) {
		indicesP.clear();
		posDimP = 0;
		for (unsigned int j = 0; j < indices[i].size(); ++j) {
			if (j < dimP.size() && dim[j].getName() == dimP[posDimP].getName()){
				indicesP.push_back(indices[i][j] - res[j]);
				posDimP++;
				if (posDimP >= dimP.size()) break;
			}
		}
		length = (indices[i][pos2] + dimP[pos].getSize() > dim[pos2].getSize()) ? dim[pos2].getSize() - indices[i][pos2] : dimP[pos].getSize();
		dataCol = getDim(cache, pos2, length, indices[i], 0);
		dataPCol = getDim(dataP, pos, dimP[pos].getSize(), indicesP, 0);
		sum += editDistance(dataCol, dataPCol, attrH, attrHP);
		if (sum > errors) {
			return -1;
		}
	}

	return sum;
}

// max error d*m^(d)
bool dynCheck(Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, vector<unsigned int> res, int errors){

	vector<unsigned int> indices;
	//vector<int> err;
	int errTmp;
	int sum = 0;
	for (unsigned int i = 0; i < dimP.size(); ++i) {
		errTmp = dynDimCheck(cache, dataP, attrH, attrHP, dim, dimP, i, res, errors);
		sum += errTmp;
		if (sum > errors || errTmp == -1) {
			return false;
		}
		//err.push_back(errTmp);
	}
	//check sum of errors in err, if lesser than number of errors allowed its ok
	if (sum <= errors) {
		return true;
	}

	return false;
}

vector<vector<unsigned int> > find(Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, int errors) {

	vector<unsigned int> cacheInd;
	for (unsigned int i = 0; i < cache->getDimInName(); ++i) {
		cacheInd.push_back(0);
	}
	vector<vector<unsigned int> > res = find(NULL, cache, dataP, attrH, attrHP, dim, dimP, 0, 0, vector<unsigned int>(), cacheInd);
	//cout << "Calls: " << calls << endl;
	//cout << "CallsFind: " << callsFind << endl;
	//cout << "CallsParts: " << callsParts << endl;
	//cout << "CallsCP: " << callsCP << endl;

	//cout << "Compares1: " << compares1 << endl; 
	//cout << "Compares2: " << compares2 << endl; 
	//Approximate check of the rest of the pattern
	/*for (unsigned int i = 0; i < res.size(); ++i) {
		for (unsigned int j = 0; j < res[i].size(); ++j) {
			cout << res[i][j] << " ";
		}
		cout << endl;
	}*/
	for (vector<vector<unsigned int> >::iterator it = res.end() - 1; it != res.begin() - 1;) {
		if (!dynCheck(cache, dataP, attrH, attrHP, dim, dimP, *it, errors)) {
			it = res.erase(it);
		}
		--it;
	}
	return res;
}

int charToInt(const char * n) {
	string i(n);
	return stoi(i.c_str());
}

void run(const char * in, const char * p, const char * err) {
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

	Reader * cache = new Reader(inpFile, dim, attrHeader);
	
	if (!checkHeaders(dim, dimPatt, attrHeader, patternAttrHeader)) {
		cout << "Invalid pattern" << endl;
		return;
	}

	int errors = charToInt(err);
	
	//void * * data = readData(file, attrHeader, dim);
	void * * dataPatt = readData(pattern, patternAttrHeader, dimPatt);

	cout << "Finding..." << endl;
	vector<vector<unsigned int> > res;

	chrono::system_clock::time_point start = chrono::system_clock::now();
	res = find(cache, dataPatt, attrHeader, patternAttrHeader, dim, dimPatt, errors);
	chrono::duration<double> sec = chrono::system_clock::now() - start;
    cout << "took " << sec.count() << " seconds\n";

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
	
	delete cache;
	deleteData(dataPatt, patternAttrHeader, dimPatt);

	pattern.close();
	file.close();
}

int main(int argc, char* argv[]) {
	if(argc < 4) {
		cout << "Usage: " << argv[0] << " <INPUTFILE>" << " <PATTERN>" << " <NUMBER_OF_ERRORS>" << endl;
		return 0;
	}
	run(argv[1], argv[2], argv[3]);
	
	return 0;
}
