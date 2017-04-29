#include "hash_chunks.h"

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

unsigned char hashInt(int item) {
	return (unsigned char)item;
}

unsigned char hashString(string item) {
	return (unsigned char)item[0];
}

unsigned char hashType(void * item, string type) {
	if(type.length() >= 3 && type.substr(0, 3) == "int") {
		return hashInt(*(int *)item);
	} else {
		return hashString(*(string *)item);
	}
}

vector<unsigned char> * hashline(void * * line, int start, int end, vector<Attribute> attrH) {
	vector<unsigned char> * res;
	vector<vector <int> > counts(attrH.size(), vector<int>(8, 0));
	unsigned char one;
	void * * cell;
	res = new vector<unsigned char>(attrH.size(), 0);

	for (int i = start; i < end; ++i) {
		if (line[i] != NULL) {
			cell = (void * *)line[i];
			for (unsigned int j = 0; j < attrH.size(); ++j) {
				one = hashType(cell[j], attrH[j].getType());
				if (one % 2 >= 1) ++counts[j][7];
				else --counts[j][7];
				if (one % 4 >= 2) ++counts[j][6];
				else --counts[j][6];
				if (one % 8 >= 4) ++counts[j][5];
				else --counts[j][5];
				if (one % 16 >= 8) ++counts[j][4];
				else --counts[j][4];
				if (one % 32 >= 16) ++counts[j][3];
				else --counts[j][3];
				if (one % 64 >= 32) ++counts[j][2];
				else --counts[j][2];
				if (one % 128 >= 64) ++counts[j][1];
				else --counts[j][1];
				if (one >= 128) ++counts[j][0];
				else --counts[j][0];
			}
		}
	}

	unsigned char part;
	for (unsigned int i = 0; i < attrH.size(); ++i) {		
		part = 0;
		if (counts[i][0] > 0) part += 128;
		if (counts[i][1] > 0) part += 64;
		if (counts[i][2] > 0) part += 32;
		if (counts[i][3] > 0) part += 16;
		if (counts[i][4] > 0) part += 8;
		if (counts[i][5] > 0) part += 4;
		if (counts[i][6] > 0) part += 2;
		if (counts[i][7] > 0) part += 1;
		(*res)[i] = part;
	}
	return res;
}

vector<unsigned char> * hashline(void * * line, int start, int end, vector<Attribute> attrH, vector<Attribute> attrHP, bool &valid, vector<vector <int> > &counts) {
	vector<unsigned char> * res;
	unsigned char one;
	void * * cell;
	res = new vector<unsigned char>(attrH.size(), 0);
	int l = 0;
	for (int i = start; i < end; ++i) if (line[i] == NULL) return res;		
	for (int i = start; i < end; ++i) {
		cell = (void * *)line[i];
		l = 0;
		for (unsigned int j = 0; j < attrH.size(); ++j) {
			if (attrH[j].getName() != attrHP[l].getName()) {
				continue;
			}
			one = hashType(cell[j], attrH[j].getType());
			if (one % 2 >= 1) ++counts[l][7];
			else --counts[l][7];
			if (one % 4 >= 2) ++counts[l][6];
			else --counts[l][6];
			if (one % 8 >= 4) ++counts[l][5];
			else --counts[l][5];
			if (one % 16 >= 8) ++counts[l][4];
			else --counts[l][4];
			if (one % 32 >= 16) ++counts[l][3];
			else --counts[l][3];
			if (one % 64 >= 32) ++counts[l][2];
			else --counts[l][2];
			if (one % 128 >= 64) ++counts[l][1];
			else --counts[l][1];
			if (one >= 128) ++counts[l][0];
			else --counts[l][0];
			++l;
		}
	}

	unsigned char part;
	for (unsigned int i = 0; i < attrH.size(); ++i) {		
		part = 0;
		if (counts[i][0] > 0) part += 128;
		if (counts[i][1] > 0) part += 64;
		if (counts[i][2] > 0) part += 32;
		if (counts[i][3] > 0) part += 16;
		if (counts[i][4] > 0) part += 8;
		if (counts[i][5] > 0) part += 4;
		if (counts[i][6] > 0) part += 2;
		if (counts[i][7] > 0) part += 1;
		(*res)[i] = part;
	}

	valid = true;
	return res;
}

vector<unsigned char> * hashlineSlide(void * * next, void * * prev, vector<Attribute> attrH, vector<Attribute> attrHP, bool &valid, vector<vector <int> > &counts) {
	vector<unsigned char> * res;
	unsigned char one;
	res = new vector<unsigned char>(attrH.size(), 0);
	if (next == NULL || prev == NULL) {
		valid = false;
		return res;
	}

	int l = 0;
	for (unsigned int j = 0; j < attrH.size(); ++j) {
		if (attrH[j].getName() != attrHP[l].getName()) {
			continue;
		}
		one = hashType(next, attrH[j].getType());
		if (one % 2 >= 1) ++counts[l][7];
		else --counts[l][7];
		if (one % 4 >= 2) ++counts[l][6];
		else --counts[l][6];
		if (one % 8 >= 4) ++counts[l][5];
		else --counts[l][5];
		if (one % 16 >= 8) ++counts[l][4];
		else --counts[l][4];
		if (one % 32 >= 16) ++counts[l][3];
		else --counts[l][3];
		if (one % 64 >= 32) ++counts[l][2];
		else --counts[l][2];
		if (one % 128 >= 64) ++counts[l][1];
		else --counts[l][1];
		if (one >= 128) ++counts[l][0];
		else --counts[l][0];

		one = hashType(prev, attrH[j].getType());
		if (one % 2 >= 1) --counts[l][7];
		else ++counts[l][7];
		if (one % 4 >= 2) --counts[l][6];
		else ++counts[l][6];
		if (one % 8 >= 4) --counts[l][5];
		else ++counts[l][5];
		if (one % 16 >= 8) --counts[l][4];
		else ++counts[l][4];
		if (one % 32 >= 16) --counts[l][3];
		else ++counts[l][3];
		if (one % 64 >= 32) --counts[l][2];
		else ++counts[l][2];
		if (one % 128 >= 64) --counts[l][1];
		else ++counts[l][1];
		if (one >= 128) --counts[l][0];
		else ++counts[l][0];
		++l;
	}
	
	unsigned char part;
	for (unsigned int i = 0; i < attrH.size(); ++i) {		
		part = 0;
		if (counts[i][0] > 0) part += 128;
		if (counts[i][1] > 0) part += 64;
		if (counts[i][2] > 0) part += 32;
		if (counts[i][3] > 0) part += 16;
		if (counts[i][4] > 0) part += 8;
		if (counts[i][5] > 0) part += 4;
		if (counts[i][6] > 0) part += 2;
		if (counts[i][7] > 0) part += 1;
		(*res)[i] = part;
	}

	return res;
}

void * * hashPatternLine(void * * line, vector<Attribute> attrHeader, vector<Dimension> dim, int partSize, unsigned int posDim, int numP) {
	void * * res = new void * [numP];
	int k = 0;
	for (unsigned int i = 0; i < dim[posDim].getSize() - partSize + 1; i += partSize) {
		res[k++] = (void *)hashline(line, i, i + partSize, attrHeader);
	}
	return res;
}

void * * hashPattern(void * * pattern, vector<Attribute> attrHeader, vector<Dimension> dim, unsigned int posDim, int partSize, int numP) {
	void * * res = NULL;
	if (posDim + 1 >= dim.size()) {
		return hashPatternLine(pattern, attrHeader, dim, partSize, posDim, numP);
	} else {
		res = new void * [dim[posDim].getSize()];
		for (unsigned int i = 0; i < dim[posDim].getSize(); ++i) {
			res[i] = (void *)hashPattern((void * *)pattern[i], attrHeader, dim, posDim + 1, partSize, numP);
		}
	}	
	return res;
}

vector<vector<unsigned int> > getAllIndices(vector<Dimension> dim, unsigned int pos, unsigned int posDim) {
	vector<vector<unsigned int> > indices, ret;
	if (posDim >= dim.size()) {
		indices.push_back(vector<unsigned int>(dim.size() - pos));
		return indices;
	} 

	if (posDim == pos) {
		indices = getAllIndices(dim, pos, posDim + 1);
		for (unsigned int i = 0; i < indices.size(); ++i) {
			indices[i][0] = 0;
		}
	} else {
		ret = getAllIndices(dim, pos, posDim + 1);
		for (unsigned int i = 0; i < dim[posDim].getSize(); ++i) {
			for (unsigned int j = 0; j < ret.size(); ++j) {
				ret[j][0] = i;
				indices.push_back(ret[j]);
			}
		}
	} 
	
	return indices;
}

bool compareHash(vector<unsigned char> * hash, vector<unsigned char> * hashP) {
	for (unsigned int i = 0; i < hash -> size(); ++i) {
		if ((*hash)[i] != (*hashP)[i]) return false;
	}
	return true;
}

void * * makeVoid(vector<void * *> column) {
	void * * res = new void * [column.size()];
	for (unsigned int i = 0; i < column.size(); ++i) {
		res[i] = (void *)column[i];
	}
	return res;
}

vector<vector<unsigned int> > checkPart(void * * data, Reader * cache, vector<unsigned char> * hashP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, unsigned int posDim, int partSize, vector<unsigned int> cacheInd, \
	vector<vector <int> > * countsSlide, vector<vector<unsigned int> > &indices, bool * valid) {

	vector<vector<unsigned int> > res(0);
	vector<void * *> col;
	void * * next, * * prev;
	vector<unsigned char> * hash;
	vector<unsigned int> one(dim.size(), 0);

	if (posDim < cache->getDimInName()) {
		for (unsigned int i = 0; i < indices.size(); ++i) {
			indices[i][0] = cacheInd[posDim];
			if (!valid[i]) {
				col = getDim(cache, posDim, partSize, indices[i], posDim);
				hash = hashline(makeVoid(col), 0, partSize, attrH, attrHP, valid[i], countsSlide[i]);
				if (valid[i] && compareHash(hash, hashP)) {
					for (unsigned int j = 0; j < indices[i].size(); ++j) {
						one[j + posDim] = indices[i][j];
					}
					res.push_back(one);
				}
				delete hash;
			} else {
				next = getItem(cache, indices[i]);
				indices[i][0] += partSize;
				prev = getItem(cache, indices[i]);
				hash = hashlineSlide(next, prev, attrH, attrHP, valid[i], countsSlide[i]);
				if (valid[i] && compareHash(hash, hashP)) {
					for (unsigned int j = 0; j < indices[i].size(); ++j) {
						one[j + posDim] = indices[i][j];
					}
					res.push_back(one);
				}
				delete hash;
			}
		}
	} else {
		if (posDim + 1 >= dim.size()) {
			if (!valid[0]) {
				hash = hashline(data, 0, partSize, attrH, attrHP, valid[0], countsSlide[0]);
				if (valid[0] && compareHash(hash, hashP)) {
					delete hash;
					return vector<vector<unsigned int> >(1, vector<unsigned int>(dim.size(), 0));
				}
				delete hash;
			} else {
				hash = hashlineSlide((void * *)data[0], (void * *)data[partSize], attrH, attrHP, valid[0], countsSlide[0]);

				if (valid[0] && compareHash(hash, hashP)) {
					delete hash;
					return vector<vector<unsigned int> >(1, vector<unsigned int>(dim.size(), 0));
				}
				delete hash;
			}
		} else {
			for (unsigned int i = 0; i < indices.size(); ++i) {
				indices[i][0] = 0;
				if (!valid[i]) {
					col = getDim(data, posDim, partSize, indices[i], posDim);
					hash = hashline(makeVoid(col), 0, partSize, attrH, attrHP, valid[i], countsSlide[i]);
					if (valid[i] && compareHash(hash, hashP)) {
						for (unsigned int j = 0; j < indices[i].size(); ++j) {
							one[j + posDim] = indices[i][j];
						}
						res.push_back(one);
					}
					delete hash;
				} else {
					next = getItem(data, indices[i], posDim);
					indices[i][0] += partSize;
					prev = getItem(data, indices[i], posDim);
					hash = hashlineSlide(next, prev, attrH, attrHP, valid[i], countsSlide[i]);
					if (valid[i] && compareHash(hash, hashP)) {
						for (unsigned int j = 0; j < indices[i].size(); ++j) {
							one[j + posDim] = indices[i][j];
						}
						res.push_back(one);
					}
					delete hash;
				}
			}
		}
	}
	return res;
}

vector<vector<unsigned int> > checkParts(void * * data, Reader * cache, void * * hashP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, unsigned int posDimP, \
	vector<unsigned int> dimPositions, vector<unsigned int> cacheInd, int partSize, int numP, vector<vector <int> > * countsSlide, \
	vector<vector<unsigned int> > &indices, bool * valid) {

	vector<vector<unsigned int> > res(0), returned(0);
	for (int i = 0; i < numP; i++) {
		returned.clear();
		returned = checkPart(data, cache, (vector<unsigned char> *)hashP[i], attrH, attrHP, dim, posDim, partSize, cacheInd, countsSlide, indices, valid);

		for (unsigned int k = 0; k < returned.size(); ++k) {
			returned[k][dimPositions[posDimP]] -= (i * partSize);
		}	
		res.insert(res.end(), returned.begin(), returned.end());	
	}
	return res;
}

vector<vector<unsigned int> > findParts(void * * data, Reader * cache, void * * hashP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, unsigned int posDimP, \
	vector<unsigned int> dimPositions, vector<unsigned int> cacheInd, int partSize, int numP) {

	vector<vector<unsigned int> > res(0), returned(0);
	vector<vector <int> > * countsSlide;
	bool * valid;
	vector<vector<unsigned int> > indices = getAllIndices(dim, posDim, posDim);
	int slide = partSize;
	if (numP == 1) slide = 1;
	countsSlide = new vector<vector<int> >[indices.size()];
	valid = new bool[indices.size()];
		
	for (unsigned int i = dimP[posDimP].getSize() - partSize; i < (dim[posDim].getSize() - partSize + 1); i += dimP[posDimP].getSize()) {
		for (unsigned int j = 0; j < indices.size(); ++j) {
			countsSlide[j] = vector<vector<int> >(attrH.size(), vector<int>(8, 0));
			valid[j] = false;
		}
		for (int j = 0; j < slide; ++j)	{
			returned.clear();
			if (posDim < cache->getDimInName()) {
				cacheInd[posDim] = i - j;
				returned = checkParts(NULL, cache, hashP, attrH, attrHP, dim, dimP, posDim, posDimP, dimPositions, cacheInd, partSize, numP, countsSlide, indices, valid);
			} else {
				returned = checkParts(&(data[i - j]), cache, hashP, attrH, attrHP, dim, dimP, posDim, posDimP, dimPositions, cacheInd, partSize, numP, countsSlide, indices, valid);
			}
			for (unsigned int k = 0; k < returned.size(); ++k) {
				returned[k][posDim] += (i - j);
				if (returned[k][posDim] + partSize < returned[k][posDim]) {
					returned[k][posDim] = 0;
				}
			}
			res.insert(res.end(), returned.begin(), returned.end());	
		}
	}
	delete [] countsSlide;
	delete [] valid;
	
	return res;
} 

vector<vector<unsigned int> > find(void * * data, Reader * cache, void * * hashP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, \
	unsigned int posDimP, vector<unsigned int> dimPositions, vector<unsigned int> cacheInd, int partSize, int numP) {

	vector<vector<unsigned int> > res(0), returned(0);
	vector<unsigned int> one;

	if (posDim == cache->getDimInName()) {
		data = cache->read(cacheInd);
	}

	if (posDimP < dimP.size() && dim[posDim].getName() == dimP[posDimP].getName()) {
		dimPositions.push_back(posDim);
		if (posDimP + 1 >= dimP.size()) {
			return findParts(data, cache, hashP, attrH, attrHP, dim, dimP, posDim, posDimP, dimPositions, cacheInd, partSize, numP);
		} else {
			for (unsigned int i = dimP[posDimP].getSize() - 1; i < dim[posDim].getSize(); i += dimP[posDimP].getSize()) {
				for (unsigned int j = 0; j < dimP[posDimP].getSize(); ++j) {
					returned.clear();
					if (posDim < cache->getDimInName()) {
						cacheInd[posDim] = i;
						returned = find(NULL, cache, (void * *)hashP[j], attrH, attrHP, dim, dimP, posDim + 1, posDimP + 1, dimPositions, cacheInd, partSize, numP);
					} else {
						returned = find((void * *)data[i], cache, (void * *)hashP[j], attrH, attrHP, dim, dimP, posDim + 1, posDimP + 1, dimPositions, cacheInd, partSize, numP);
					}

					for (unsigned int k = 0; k < returned.size(); ++k) {
						returned[k][posDim] += (i - j);
						//res.push_back(returned[k]);	
					}
					res.insert(res.end(), returned.begin(), returned.end());	
				}
			}
			return res;
		}	
	} else {
		for (unsigned int i = 0; i < dim[posDim].getSize(); ++i) {
			if (posDim < cache->getDimInName()) {
				cacheInd[posDim] = i;
				returned = find(NULL, cache, hashP, attrH, attrHP, dim, dimP, posDim + 1, posDimP, dimPositions, cacheInd, partSize, numP);
			} else {
				returned = find((void * *)data[i], cache, hashP, attrH, attrHP, dim, dimP, posDim + 1, posDimP, dimPositions, cacheInd, partSize, numP);
			}
			for (unsigned int j = 0; j < returned.size(); ++j) {
				returned[j][posDim] += i;
				res.push_back(returned[j]);
			}
		}
		return res;
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
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, vector<unsigned int> res, int errors) {

	vector<unsigned int> indices;
	vector<int> err;
	int errTmp;
	int sum = 0;
	for (unsigned int i = 0; i < dimP.size(); ++i) {
		errTmp = dynDimCheck(cache, dataP, attrH, attrHP, dim, dimP, i, res, errors);
		sum += errTmp;
		if (sum > errors || errTmp == -1) {
			return false;
		}
		err.push_back(errTmp);
	}
	//check sum of errors in err, if lesser than number of errors allowed its ok
	if (sum <= errors) {
		return true;
	}

	return false;
}

void preverifCheck(void * * data, Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, \
	unsigned int posDimP, vector<unsigned int> indices, int posInd, vector<unsigned int> cacheInd, int errors, int count, int &res, int &done) {

	if (posDim == cache->getDimInName()) {
		data = cache->read(cacheInd);
	}
	
	if (count - res < errors) 
		return;
	if (done - res > errors)
		return;

	unsigned int max;

	if (posDimP < dimP.size() && dim[posDim].getName() == dimP[posDimP].getName()) {
		if (posDim + 1 >= dim.size()) {
			max = (indices[posInd] + dimP[posDimP].getSize() > dim[posDim].getSize() ? dim[posDim].getSize() - indices[posInd] : dimP[posDimP].getSize());
			for (unsigned int j = 0; j < max; ++j) {
				++done;
				if (compareItem((void * *)data[indices[posInd] + j], (void * *)dataP[j], attrH, attrHP)) {
					++res;
				}
			}
		} else {
			max = (indices[posInd] + dimP[posDimP].getSize() > dim[posDim].getSize() ? dim[posDim].getSize() - indices[posInd] : dimP[posDimP].getSize());
			if (posDim < cache->getDimInName()) {
				for (unsigned int k = 0; k < max; ++k) {
					cacheInd[posDim] = indices[posInd] + k;
					preverifCheck(NULL, cache, (void * *)dataP[k], attrH, attrHP, dim, dimP, posDim + 1, posDimP + 1, indices, posInd + 1, cacheInd, errors, count, res, done);
				}
			} else {
				for (unsigned int k = 0; k < max; ++k) {
					preverifCheck((void * *)data[indices[posInd] + k], cache, (void * *)dataP[k], attrH, attrHP, dim, dimP, posDim + 1, posDimP + 1, indices, posInd + 1, cacheInd, errors, count, res, done);
				}
			}
		}
	} else {
		if (posDim + 1 >= dim.size()) {
			++done;
			if(compareItem((void * *)data[indices[posInd]], dataP, attrH, attrHP)) {
				++res;
			}
		} else {
			if (posDim < cache->getDimInName()) {
				cacheInd[posDim] = indices[posInd];
				preverifCheck(NULL, cache, dataP, attrH, attrHP, dim, dimP, posDim + 1, posDimP, indices, posInd + 1, cacheInd, errors, count, res, done);			
			} else {
				preverifCheck((void * *)data[indices[posInd]], cache, dataP, attrH, attrHP, dim, dimP, posDim + 1, posDimP, indices, posInd + 1, cacheInd, errors, count, res, done);			
			}
		}
	}
}

bool preverif(Reader * cache, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, vector<unsigned int> res, int errors) {
	vector<unsigned int> indices;
	int corr = 0, done = 0;
	int sum = 1;
	for (unsigned int i = 0; i < cache->getDimInName(); ++i) {
		indices.push_back(0);
	}
	for (unsigned int i = 0; i < dimP.size(); ++i) {
		sum *= dimP[i].getSize();
	}
	preverifCheck(NULL, cache, dataP, attrH, attrHP, dim, dimP, 0, 0, res, 0, indices, errors, sum, corr, done);
	//check sum of errors in err, if lesser than number of errors allowed its ok
	if (corr >= sum - errors) {
		return true;
	}
	return false;
}

vector<vector<unsigned int> > find(Reader * cache, void * * hashP, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, int errors, int partSize, int numP) {

	vector<unsigned int> cacheInd;

	for (unsigned int i = 0; i < cache->getDimInName(); ++i) {
		cacheInd.push_back(0);
	}
	vector<vector<unsigned int> > res = find(NULL, cache, hashP, attrH, attrHP, dim, dimP, 0, 0, vector<unsigned int>(), cacheInd, partSize, numP);
	//Preverification
	for (vector<vector<unsigned int> >::iterator it = res.end() - 1; it != res.begin() - 1;) {
		if (!preverif(cache, dataP, attrH, attrHP, dim, dimP, *it, errors)) {
			it = res.erase(it);
		}
		--it;
	}
	//Approximate check of the rest of the pattern
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
	int numP = getNumOfParts(errors, dimPatt.size(), dimPatt[dimPatt.size() - 1].getSize());
	int partSize = getPartSize(dimPatt[dimPatt.size() - 1].getSize(), numP);
	cout << "Num of parts: " << numP << endl;
	cout << "Part size: " << partSize << endl;

	void * * dataPatt = readData(pattern, patternAttrHeader, dimPatt);
	void * * hashP = hashPattern(dataPatt, patternAttrHeader, dimPatt, 0, partSize, numP);
	
	cout << "Finding..." << endl;
	vector<vector<unsigned int> > res;

	chrono::system_clock::time_point start = chrono::system_clock::now();
	res = find(cache, hashP, dataPatt, attrHeader, patternAttrHeader, dim, dimPatt, errors, partSize, numP);
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
	deleteHashP(hashP, dimPatt);
	
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
