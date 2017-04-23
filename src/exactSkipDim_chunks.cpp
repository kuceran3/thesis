#include "exactSkipDim_chunks.h"

using namespace std;

//Returns upper left position of solution
bool findRest(void * * data, Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, \
	unsigned int posDimP, vector<unsigned int> indices, int posInd, vector<unsigned int> cacheInd) {

	if (posDim == cache->getDimInName()) {
		data = cache->read(cacheInd);
	}

	if (posDimP < dimP.size() && dim[posDim].getName() == dimP[posDimP].getName()) {
			if (posDim + 1 >= dim.size()) {
				for (unsigned int j = 0; j < dimP[posDimP].getSize(); ++j) {
					if (!compareItem((void * *)data[indices[posInd] + j], (void * *)dataP[j], attrH, attrHP)) {
						return false;
					}
				}
				return true;
			} else {
				for (unsigned int k = 0; k < dimP[posDimP].getSize(); ++k) {
					if (posDim < cache->getDimInName()) {
						cacheInd[posDim] = indices[posInd] + k;
						if (!findRest(NULL, cache, (void * *)dataP[k], attrH, attrHP, dim, dimP, posDim + 1, posDimP + 1, indices, posInd + 1, cacheInd)) {
							return false;
						}
					} else {
						if (!findRest((void * *)data[indices[posInd] + k], cache, (void * *)dataP[k], attrH, attrHP, dim, dimP, posDim + 1, posDimP + 1, indices, posInd + 1, cacheInd)) {
							return false;
						}
					}
				}
				return true;
			}
		return false;
	} else {
		if (posDim + 1 >= dim.size()) {
			return compareItem((void * *)data[indices[posInd]], dataP, attrH, attrHP);
		} else {
			if (posDim < cache->getDimInName()) {
				cacheInd[posDim] = indices[posInd];
				return findRest(NULL, cache, dataP, attrH, attrHP, dim, dimP, posDim + 1, posDimP, indices, posInd + 1, cacheInd);			
			} else {
				return findRest((void * *)data[indices[posInd]], cache, dataP, attrH, attrHP, dim, dimP, posDim + 1, posDimP, indices, posInd + 1, cacheInd);			
			}
		}
	}
}

vector<vector<unsigned int> > checkRest(Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, int pos, \
	int posP, vector<vector<unsigned int> > subres, vector<unsigned int> cacheInd) {

	vector<vector<unsigned int> > res;
	for (unsigned int i = 0; i < subres.size(); ++i)	{
		subres[i].insert(subres[i].begin(), (unsigned int)(pos - posP));
		if (findRest(NULL, cache, dataP, attrH, attrHP, dim, dimP, 0, 0, subres[i], 0, cacheInd)) {
			res.push_back(subres[i]);
		}
	}
	return res;
}

vector<vector<unsigned int> > find(void * * data, Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, \
	unsigned int posDimP, vector<unsigned int> cacheInd) {

	vector<vector<unsigned int> > res, returned;
	vector<unsigned int> one;
	bool isRes;

	if (posDim == cache->getDimInName()) {
		data = cache->read(cacheInd);
	}

	for (unsigned int i = 0; i < dim[posDim].getSize(); ++i) {
		//find dimensions with the same name
		if (posDimP < dimP.size() && dim[posDim].getName() == dimP[posDimP].getName()) {
			if (i + dimP[posDimP].getSize() > dim[posDim].getSize()) break;
			if (posDim + 1 >= dim.size()) {
				if (compareItem((void * *)data[i], (void * *)dataP[0], attrH, attrHP)) {
					isRes = true;
					for (unsigned int j = 1; j < dimP[posDimP].getSize(); ++j) {
						if (!compareItem((void * *)data[i + j], (void * *)dataP[j], attrH, attrHP)) {
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
				if (posDim < cache->getDimInName()) {
					cacheInd[posDim] = i;
					returned = find(NULL, cache, (void * *)dataP[0], attrH, attrHP, dim, dimP, posDim + 1, posDimP + 1, cacheInd);
				} else {
					returned = find((void * *)data[i], cache, (void * *)dataP[0], attrH, attrHP, dim, dimP, posDim + 1, posDimP + 1, cacheInd);
				}
				for (unsigned int j = 0; j < returned.size(); ++j) {
					isRes = true;
					for (unsigned int k = 1; k < dimP[posDimP].getSize(); ++k) {
						if (posDim < cache->getDimInName()) {
							cacheInd[posDim] = i + k;
							if (!findRest(NULL, cache, (void * *)dataP[k], attrH, attrHP, dim, dimP, posDim + 1, posDimP + 1, returned[j], 0, cacheInd)) {
								isRes = false;
								break;
							}
						} else {
							if (!findRest((void * *)data[i + k], cache, (void * *)dataP[k], attrH, attrHP, dim, dimP, posDim + 1, posDimP + 1, returned[j], 0, cacheInd)) {
								isRes = false;
								break;
							}
						}
					}
					if (isRes) {
						returned[j].insert(returned[j].begin(), i);
						res.push_back(returned[j]);	
					}
				}
			}
		} else {
			if (posDim + 1 >= dim.size()) {
				if (compareItem((void * *)data[i], dataP, attrH, attrHP)) {
					one.push_back(i);
					res.push_back(one);
					one.clear();
				}
			} else {
				if (posDim < cache->getDimInName()) {
					cacheInd[posDim] = i;
					returned = find(NULL, cache, dataP, attrH, attrHP, dim, dimP, posDim + 1, posDimP, cacheInd);
				} else {
					returned = find((void * *)data[i], cache, dataP, attrH, attrHP, dim, dimP, posDim + 1, posDimP, cacheInd);
				}
				for (unsigned int j = 0; j < returned.size(); ++j) {
					returned[j].insert(returned[j].begin(), i);
					res.push_back(returned[j]);
				}
			}
		}
	}
	return res;
}

vector<vector<unsigned int> > find(Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP) {
	
	vector<vector<unsigned int> > res, subres, found;
	vector<unsigned int> cacheInd;
	for (unsigned int i = 0; i < cache->getDimInName(); ++i) {
		cacheInd.push_back(0);
	}
	void * * data = NULL;
	if (cache->getDimInName() == 0) {
		data = cache->read(cacheInd);
	}

	for (unsigned int i = dimP[0].getSize() - 1; i < dim[0].getSize(); i += dimP[0].getSize()) {
		for (unsigned int j = 0; j < dimP[0].getSize(); ++j) {
			if (cache->getDimInName() == 0) {
				subres = find((void * *)data[i], cache, (void * *)dataP[j], attrH, attrHP, dim, dimP, 1, 1, cacheInd);
			} else {
				cacheInd[0] = i;
				subres = find(NULL, cache, (void * *)dataP[j], attrH, attrHP, dim, dimP, 1, 1, cacheInd);
			}
			if (!subres.empty()) {
				found = checkRest(cache, dataP, attrH, attrHP, dim, dimP, i, j, subres, cacheInd);
				res.insert(res.end(), found.begin(), found.end());
			}
		}
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

	Reader * cache = new Reader(inpFile, dim, attrHeader);
	if (!checkHeaders(dim, dimPatt, attrHeader, patternAttrHeader)) {
		cout << "Invalid pattern" << endl;
		return;
	}
	
	void * * dataPatt = readData(pattern, patternAttrHeader, dimPatt);

	cout << "Finding..." << endl;
	vector<vector<unsigned int> > res;
	
	chrono::system_clock::time_point start = chrono::system_clock::now();
	res = find(cache, dataPatt, attrHeader, patternAttrHeader, dim, dimPatt);

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
	
	delete cache;
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
