#include "appSplitDyn_chunks.h"

using namespace std;

int getNumOfParts(int k, int d, int dLen) {
	int parts = (int)pow((float)k, 1.0 / (d - 1)) + 1;
	if (parts > dLen) return dLen;
	while(dLen % parts != 0) --parts;
	return parts;
}

int getPartSize(int dLen, int count) {
	return (dLen / count);
}

bool checkRest(void * * data, Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, unsigned int posDim, vector<unsigned int> indices, vector<unsigned int> cacheInd) {

	if (posDim == cache->getDimInName()) {
		data = cache->read(cacheInd);
	}

	if (posDim + 1 >= dim.size()) {
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

	vector<vector<unsigned int> > res, returned;
	if (posDim == cache->getDimInName()) {
		data = cache->read(cacheInd);
	}	

	if (posDim + 1 >= dim.size()) {
		for (unsigned int i = 0; i < dim[posDim].getSize(); ++i) {
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
		if (posDim + 1 >= dim.size()) {
			if (compareItem((void * *)data[0], (void * *)dataP[0], attrH, attrHP)) {
				for (int j = 1; j < partSize; ++j) {	
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
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, unsigned int posDimP, \
	vector<unsigned int> dimPositions, vector<unsigned int> cacheInd, int partSize) {

	vector<vector<unsigned int> > res, returned;

	for (unsigned int i = 0; i < dimP[posDimP].getSize() - partSize + 1; i += partSize) {
		returned = checkPart(data, cache, &dataP[i], attrH, attrHP, dim, posDim, partSize, cacheInd);

		for (unsigned int k = 0; k < returned.size(); ++k) {
			returned[k][dimPositions[posDimP]] -= i;
			res.push_back(returned[k]);	
		}	
	}
	return res;
}

vector<vector<unsigned int> > findParts(void * * data, Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, unsigned int posDimP, \
	vector<unsigned int> dimPositions, vector<unsigned int> cacheInd, int partSize, int numP) {

	vector<vector<unsigned int> > res, returned;
	int slide = partSize;
	//if (numP == 1) slide = 1;
	for (unsigned int i = dimP[posDimP].getSize() - partSize; i < (dim[posDim].getSize() - partSize + 1); i += dimP[posDimP].getSize()) {
		if (i == 0) {
			if (posDim < cache->getDimInName()) {
				cacheInd[posDim] = 0;
				returned = checkParts(NULL, cache, dataP, attrH, attrHP, dim, dimP, posDim, posDimP, dimPositions, cacheInd, partSize);
			} else {
				returned = checkParts(&data[0], cache, dataP, attrH, attrHP, dim, dimP, posDim, posDimP, dimPositions, cacheInd, partSize);				
			}
			for (unsigned int k = 0; k < returned.size(); ++k) {
				res.push_back(returned[k]);	
			}	
		} else {
			for (int j = 0; j < slide; ++j)	{
				if (posDim < cache->getDimInName()) {
					cacheInd[posDim] = i - j;
					returned = checkParts(NULL, cache, dataP, attrH, attrHP, dim, dimP, posDim, posDimP, dimPositions, cacheInd, partSize);
				} else {
					returned = checkParts(&data[i - j], cache, dataP, attrH, attrHP, dim, dimP, posDim, posDimP, dimPositions, cacheInd, partSize);				
				}
				for (unsigned int k = 0; k < returned.size(); ++k) {
					returned[k][posDim] += (i - j);
					if (returned[k][posDim] + partSize < returned[k][posDim])
						returned[k][posDim] = 0;
					res.push_back(returned[k]);	
				}	
			}
		}
	}
	return res;
} 

vector<vector<unsigned int> > find(void * * data, Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, \
	unsigned int posDimP, vector<unsigned int> dimPositions, vector<unsigned int> cacheInd, int partSize, int numP) {

	vector<vector<unsigned int> > res, returned;
	vector<unsigned int> one;

	if (posDim == cache->getDimInName()) {
		data = cache->read(cacheInd);
	}

	if (posDimP < dimP.size() && dim[posDim].getName() == dimP[posDimP].getName()) {
		dimPositions.push_back(posDim);
		if (posDimP + 1 >= dimP.size()) {

			returned = findParts(data, cache, dataP, attrH, attrHP, dim, dimP, posDim, posDimP, dimPositions, cacheInd, partSize, numP);
			for (unsigned int j = 0; j < returned.size(); ++j) {
				res.push_back(returned[j]);	
			}
		} else {
			for (unsigned int i = dimP[posDimP].getSize() - 1; i < dim[posDim].getSize(); i += dimP[posDimP].getSize()) {
				for (unsigned int j = 0; j < dimP[posDimP].getSize(); ++j) {
					if (posDim < cache->getDimInName()) {
						cacheInd[posDim] = i;
						returned = find(NULL, cache, (void * *)dataP[j], attrH, attrHP, dim, dimP, posDim + 1, posDimP + 1, dimPositions, cacheInd, partSize, numP);
					} else {
						returned = find((void * *)data[i], cache, (void * *)dataP[j], attrH, attrHP, dim, dimP, posDim + 1, posDimP + 1, dimPositions, cacheInd, partSize, numP);
					}
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
				returned = find(NULL, cache, dataP, attrH, attrHP, dim, dimP, posDim + 1, posDimP, dimPositions, cacheInd, partSize, numP);
			} else {
				returned = find((void * *)data[i], cache, dataP, attrH, attrHP, dim, dimP, posDim + 1, posDimP, dimPositions, cacheInd, partSize, numP);
			}
			for (unsigned int j = 0; j < returned.size(); ++j) {
				returned[j][posDim] += i;
				res.push_back(returned[j]);
			}
		}
	}
	return res;
}

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
			if (dim[j].getName() == dimP[posDimP].getName()){
				indicesP.push_back(indices[i][j] - res[j]);
				if (posDimP++ >= dimP.size()) break;
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
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, vector<unsigned int> res, int errors) {

	int errTmp;
	int sum = 0;
	for (unsigned int i = 0; i < dimP.size(); ++i) {
		errTmp = dynDimCheck(cache, dataP, attrH, attrHP, dim, dimP, i, res, errors);
		sum += errTmp;
		if (errTmp == -1 || sum > errors) {
			return false;
		}
	}
	//check sum of errors in err, if lesser than number of errors allowed its ok
	if (sum <= errors) {
		return true;
	}

	return false;
}

vector<vector<unsigned int> > find(Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, int errors, int partSize, int numP) {

	vector<unsigned int> cacheInd;
	for (unsigned int i = 0; i < cache->getDimInName(); ++i) {
		cacheInd.push_back(0);
	}
	
	chrono::system_clock::time_point start = chrono::system_clock::now();
	vector<vector<unsigned int> > res = find(NULL, cache, dataP, attrH, attrHP, dim, dimP, 0, 0, vector<unsigned int>(), cacheInd, partSize, numP);
	chrono::duration<double> sec = chrono::system_clock::now() - start;
    cout << "Find took " << sec.count() << " seconds\n";
	
	/*cout << res.size() << endl;
	for (unsigned int i = 0; i < res.size(); ++i) {
		for (unsigned int j = 0; j < res[i].size(); ++j) {
			cout << res[i][j] << ", ";
		}
		cout << endl;
	}*/
	start = chrono::system_clock::now();
	for (vector<vector<unsigned int> >::iterator it = res.begin(); it != res.end();) {
		if (!dynCheck(cache, dataP, attrH, attrHP, dim, dimP, *it, errors)) {
			it = res.erase(it);
		} else {
			++it;
		}
		//if (!dynCheck(cache, dataP, attrH, attrHP, dim, dimP, *it, errors)) {
		//	it = res.erase(it);
		//}
		//--it;
	}
	sec = chrono::system_clock::now() - start;
    cout << "Dynamic check took " << sec.count() << " seconds\n";
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

	if (!checkHeaders(dim, dimPatt, attrHeader, patternAttrHeader)) {
		cout << "Invalid pattern" << endl;
		return;
	}

	Reader * cache = new Reader(inpFile, dim, attrHeader, dim[0].getSize());

	int errors = charToInt(err);
	int numP = getNumOfParts(errors, dimPatt.size(), dimPatt[dimPatt.size() - 1].getSize());
	int partSize = getPartSize(dimPatt[dimPatt.size() - 1].getSize(), numP);
	
	void * * dataPatt = readData(pattern, patternAttrHeader, dimPatt);

	cout << "Finding..." << endl;
	vector<vector<unsigned int> > res;

	//chrono::system_clock::time_point start = chrono::system_clock::now();
	res = find(cache, dataPatt, attrHeader, patternAttrHeader, dim, dimPatt, errors, partSize, numP);
	//chrono::duration<double> sec = chrono::system_clock::now() - start;
    //cout << "took " << sec.count() << " seconds\n";
    cout << res.size() << endl;
	/*if (res.size() == 0){
		cout << "no solutions found" << endl;
	} else {
		for (unsigned int i = 0; i < res.size(); ++i) {
			for (unsigned int j = 0; j < res[i].size(); ++j) {
				cout << res[i][j] << ", ";
			}
			cout << endl;
		}
	}*/
	
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
