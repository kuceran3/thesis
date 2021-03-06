#include "brute.h"

using namespace std;

//Returns upper left position of solution
bool findRest(void * * data, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, unsigned int posDimP, vector<unsigned int> indices, int posInd) {

	if (posDimP < dimP.size() && dim[posDim].getName() == dimP[posDimP].getName()) {
		if (dim[posDim].getOneVal(indices[posInd]) == dimP[posDimP].getOneVal(0)) {
			if (posDim + 1 >= dim.size()) {
				for (unsigned int j = 0; j < dimP[posDimP].getSize(); ++j) {
					if (!compareItem((void * *)data[indices[posInd] + j], (void * *)dataP[j], attrH, attrHP)) {
						return false;
					}
				}
				return true;
			} else {
				for (unsigned int k = 0; k < dimP[posDimP].getSize(); ++k) {
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
			return compareItem((void * *)data[indices[posInd]], dataP, attrH, attrHP);
		} else {
			return findRest((void * *)data[indices[posInd]], dataP, attrH, attrHP, dim, dimP, posDim + 1, posDimP, indices, posInd + 1);			
		}
	}
}

vector<vector<unsigned int> > find(void * * data, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, unsigned int posDimP) {

	vector<vector<unsigned int> > res, returned;
	vector<unsigned int> one;
	bool isRes;
	for (unsigned int i = 0; i < dim[posDim].getSize(); ++i) {
		//find dimensions with the same name
		if (posDimP < dimP.size() && dim[posDim].getName() == dimP[posDimP].getName()) {
			if (i + dimP[posDimP].getSize() > dim[posDim].getSize()) break;
			//find same value in the dimension
			if (dim[posDim].getOneVal(i) == dimP[posDimP].getOneVal(0)) {
				if (posDim + 1 >= dim.size()) {
					if (compareItem((void * *)data[i], (void * *)dataP[0], attrH, attrHP)) {
						isRes = true;
						for (unsigned int j = 1; j < dimP[posDimP].getSize(); ++j) {
							if (dim[posDim].getOneVal(i + j) != dimP[posDimP].getOneVal(j)) {
								isRes = false;
								break;
							}
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
					returned = find((void * *)data[i], (void * *)dataP[0], attrH, attrHP, dim, dimP, posDim + 1, posDimP + 1);
					for (unsigned int j = 0; j < returned.size(); ++j) {
						isRes = true;
						for (unsigned int k = 1; k < dimP[posDimP].getSize(); ++k) {
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
				if (compareItem((void * *)data[i], dataP, attrH, attrHP)) {
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
	
	chrono::system_clock::time_point start = chrono::system_clock::now();
	res = find(data, dataPatt, attrHeader, patternAttrHeader, dim, dimPatt);
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
