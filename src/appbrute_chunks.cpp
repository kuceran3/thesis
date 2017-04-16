#include "appbrute_chunks.h"

using namespace std;

//Returns upper left position of solution
unsigned int findRest(void * * data, Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, \
	unsigned int posDimP, vector<unsigned int> indices, int posInd, vector<unsigned int> cacheInd) {

	int sumE = 0;

	if (posDim == cache->getDimInName()) {
		data = cache->read(cacheInd);
	}
	if (posDimP < dimP.size() && dim[posDim].getName() == dimP[posDimP].getName()) {
		if (posDim + 1 >= dim.size()) {
			for (unsigned int j = 0; j < dimP[posDimP].getSize(); ++j) {
				if (!compareItem((void * *)data[indices[posInd] + j], (void * *)dataP[j], attrH, attrHP)) {
					sumE++;
				}
			}
			//return true;
		} else {
			for (unsigned int k = 0; k < dimP[posDimP].getSize(); ++k) {
				if (posDim < cache->getDimInName()) {
					cacheInd[posDim] = indices[posInd] + k;
					sumE += findRest(NULL, cache, (void * *)dataP[k], attrH, attrHP, dim, dimP, posDim + 1, posDimP + 1, indices, posInd + 1, cacheInd);
				} else {
					sumE += findRest((void * *)data[indices[posInd] + k], cache, (void * *)dataP[k], attrH, attrHP, dim, dimP, posDim + 1, posDimP + 1, indices, posInd + 1, cacheInd);
				}
			}
		}
			//return true;
		//return false;
	} else {
		if (posDim + 1 >= dim.size()) {
			if (!compareItem((void * *)data[indices[posInd]], dataP, attrH, attrHP))
				sumE++;
		} else {
			if (posDim < cache->getDimInName()) {
				cacheInd[posDim] = indices[posInd];
				sumE += findRest(NULL, cache, dataP, attrH, attrHP, dim, dimP, posDim + 1, posDimP, indices, posInd + 1, cacheInd);
			} else {
				sumE += findRest((void * *)data[indices[posInd]], cache, dataP, attrH, attrHP, dim, dimP, posDim + 1, posDimP, indices, posInd + 1, cacheInd);
			}
		}
	}
	return sumE;
}

vector<pair <vector<unsigned int>, unsigned int> > findFirst(void * * data, Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, unsigned int posDimP, \
	vector<unsigned int> cacheInd, unsigned int maxE) {

	vector<pair <vector<unsigned int>, unsigned int> > res, returned;
	vector<unsigned int> one, errors;
	bool isRes;
	unsigned int rowE = 0;

	if (posDim == cache->getDimInName()) {
		data = cache->read(cacheInd);
	}


	for (unsigned int i = 0; i < dim[posDim].getSize(); ++i) {
		//find dimensions with the same name
		if (posDimP < dimP.size() && dim[posDim].getName() == dimP[posDimP].getName()) {
			if (i + dimP[posDimP].getSize() > dim[posDim].getSize()) break;
			if (posDim + 1 >= dim.size()) {
				rowE = 0;
				for (unsigned int j = 0; j < dimP[posDimP].getSize(); ++j) {
					if (!compareItem((void * *)data[i + j], (void * *)dataP[j], attrH, attrHP)) {
						rowE++;
					}
				}
				if (rowE <= maxE) {
					one.push_back(i);
					res.push_back(pair<vector<unsigned int>, unsigned int> (one, rowE));
					one.clear();
				}
			} else {
				if (posDim < cache->getDimInName()) {
					cacheInd[posDim] = i;
					returned = findFirst(NULL, cache, (void * *)dataP[0], attrH, attrHP, dim, dimP, posDim + 1, posDimP + 1, cacheInd, maxE);
				} else {
					returned = findFirst((void * *)data[i], cache, (void * *)dataP[0], attrH, attrHP, dim, dimP, posDim + 1, posDimP + 1, cacheInd, maxE);
				}

				for (unsigned int j = 0; j < returned.size(); ++j) {
					isRes = true;
					for (unsigned int k = 1; k < dimP[posDimP].getSize(); ++k) {
						if (posDim < cache->getDimInName()) {
							cacheInd[posDim] = i + k;
							returned[j].second += findRest(NULL, cache, (void * *)dataP[k], attrH, attrHP, dim, dimP, posDim + 1, posDimP + 1, returned[j].first, 0, cacheInd);
						} else {
							returned[j].second += findRest((void * *)data[i + k], cache, (void * *)dataP[k], attrH, attrHP, dim, dimP, posDim + 1, posDimP + 1, returned[j].first, 0, cacheInd);
						}
						if (returned[j].second > maxE) {
							isRes = false;
							break;
						}
					}
					if (isRes) {
						returned[j].first.insert(returned[j].first.begin(), i);
						res.push_back(returned[j]);	
					}
				}
			}
		} else {
			if (posDim + 1 >= dim.size()) {
				one.push_back(i);
				if (compareItem((void * *)data[i], dataP, attrH, attrHP)) {
					res.push_back(pair<vector<unsigned int>, unsigned int> (one, 0));
				} else {
					res.push_back(pair<vector<unsigned int>, unsigned int> (one, 1));
				}
				one.clear();

			} else {
				if (posDim < cache->getDimInName()) {
					cacheInd[posDim] = i;
					returned = findFirst(NULL, cache, dataP, attrH, attrHP, dim, dimP, posDim + 1, posDimP, cacheInd, maxE);
				} else {
					returned = findFirst((void * *)data[i], cache, dataP, attrH, attrHP, dim, dimP, posDim + 1, posDimP, cacheInd, maxE);
				}
				for (unsigned int j = 0; j < returned.size(); ++j) {
					returned[j].first.insert(returned[j].first.begin(), i);
					res.push_back(returned[j]);
				}
			}
		}
	}
	return res;
}

vector<vector<unsigned int> > find(void * * data, Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, unsigned int posDimP, \
	vector<unsigned int> cacheInd, unsigned int maxE) {

	vector<vector<unsigned int> > res, returned;
	vector<pair <vector<unsigned int>, unsigned int> > ret;
	vector<unsigned int> one;

	if (dim[posDim].getName() == dimP[posDimP].getName()) {
		ret = findFirst(data, cache, dataP, attrH, attrHP, dim, dimP, posDim, posDimP, cacheInd, maxE);
		for (unsigned int i = 0; i < ret.size(); ++i) {
			res.push_back(ret[i].first);
		}
		return res;
	}

	if (posDim == cache->getDimInName()) {
		data = cache->read(cacheInd);
	}

	for (unsigned int i = 0; i < dim[posDim].getSize(); ++i) {
		//find dimensions with the same name
		if (posDim < cache->getDimInName()) {
			cacheInd[posDim] = i;
			returned = find(NULL, cache, dataP, attrH, attrHP, dim, dimP, posDim + 1, posDimP, cacheInd, maxE);
		} else {
			returned = find((void * *)data[i], cache, dataP, attrH, attrHP, dim, dimP, posDim + 1, posDimP, cacheInd, maxE);
		}
		for (unsigned int j = 0; j < returned.size(); ++j) {
			returned[j].insert(returned[j].begin(), i);
			res.push_back(returned[j]);
		}
	}
	return res;
}

vector<vector<unsigned int> > find(Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int maxE) {
	
	vector<unsigned int> cacheInd;
	for (unsigned int i = 0; i < cache->getDimInName(); ++i) {
		cacheInd.push_back(0);
	}
	return find(NULL, cache, dataP, attrH, attrHP, dim, dimP, 0, 0, cacheInd, maxE);
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
	//int dimInName = 0;
	
	vector<Attribute> patternAttrHeader;
	vector<Dimension> dimPatt;
	//int dimInNamePatt = 0;
	
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
	//void * * data;
	//data = readData(file, attrHeader, dim);
	
	void * * dataPatt = readData(pattern, patternAttrHeader, dimPatt);
	vector<vector<unsigned int> > res;

	cout << "Finding..." << endl;
	
	chrono::system_clock::time_point start = chrono::system_clock::now();
	res = find(cache, dataPatt, attrHeader, patternAttrHeader, dim, dimPatt, errors);
	chrono::duration<double> sec = chrono::system_clock::now() - start;
    cout << "took " << sec.count() << " seconds\n";

	if (res.size() == 0){
		cout << "No solutions found" << endl;
	} else {
		for (unsigned int i = 0; i < res.size(); ++i) {
			for (unsigned int j = 0; j < res[i].size(); ++j) {
				cout << res[i][j] << ", ";
			}
			cout << endl;
		}
	}
	//deleteData(data, attrHeader, dim);
	delete cache;
	deleteData(dataPatt, patternAttrHeader, dimPatt);

	pattern.close();
	file.close();
	//closedir(dp);
}

int main(int argc, char* argv[]) {
	if(argc < 4) {
		cout << "Usage: " << argv[0] << " <INPUTFILE>" << " <PATTERN>" << " <NUMBER_OF_ERRORS>" << endl;
		return 0;
	}
	run(argv[1], argv[2], argv[3]);
	return 0;
}
