#include "hash_chunks_rolling.h"
//hashuje jen jednou
using namespace std;

//vector<unsigned int> dataPosGlob = vector<unsigned int>(3, 0);
//vector<unsigned int> patPosGlob = vector<unsigned int>(3, 0);

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

vector<unsigned char> * hashline(void * * line, int start, int end, vector<Attribute> attrH) {
	vector<unsigned char> * res = new vector<unsigned char>(((end - start + 7) / 8) * attrH.size(), 0);
	void * * cell;
	unsigned char one;
	int pos = 128;
	int resPos = 0;
	string s;
	for (int i = 0; i < end - start; ++i) {
		cell = (void * *)line[i + start];
		for (unsigned int j = 0; j < attrH.size(); ++j) {
			one = *(unsigned char *)cell[j];
			(*res)[resPos + j] += one & pos;
		}
		if (!((i + 1) % 8)) {
			resPos++;
			pos = 128;
		} else {
			pos /= 2;
		}
	}
	return res;
}

vector<unsigned char> * hashline(void * * line, int start, int end, vector<Attribute> attrH, vector<Attribute> attrHP) {
	void * * cell;
	unsigned char one;
	int pos = 128;
	int resPos = 0;

	for (int i = start; i < end; ++i) if (line[i] == NULL) return NULL;		
	vector<unsigned char> * res = new vector<unsigned char>(((end - start + 7) / 8) * attrHP.size(), 0);

	int l = 0;
	for (int i = 0; i < end - start; ++i) {
		l = 0;
		cell = (void * *)line[i + start];
		for (unsigned int j = 0; j < attrH.size(); ++j) {
			if (attrH[j].getName() != attrHP[l].getName()) {
				continue;
			}
			one = *(unsigned char *)cell[j];
			(*res)[resPos + (l++)] += one & pos;
		}
		if (!((i + 1) % 8)) {
			resPos++;
			pos = 128;
		} else {
			pos /= 2;
		}
	}
	return res;
}

void * * hashPatternLine(void * * line, vector<Attribute> attrHeader, vector<Dimension> dim, int partSize, unsigned int posDim, int numP) {
	void * * res = new void * [numP];
	int k = 0;
	//vector<unsigned char> * hash;
	for (unsigned int i = 0; i < dim[posDim].getSize() - partSize + 1; i += partSize) {
		res[k++] = (void *)hashline(line, i, i + partSize, attrHeader);
		//hash = hashline(line, i, i + partSize, attrHeader);
		//if (hash == NULL)
		//	res[k++] = NULL;
		//else
		//for (int i = 0; i < hash -> size(); ++i) {
		//	cout << (int)(*hash)[i] << " " ;
		//}
		//cout << endl;
		//res[k++] = (void *)hash;
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

void * * hashDataParts(void * * data, vector<Attribute> attrH, vector<Attribute> attrHP, vector<Dimension> dim, \
		unsigned int posDim, int partSize, vector<vector<unsigned int> > indices) {
	
	vector<void * *> col;
	vector<unsigned int> ind(dim.size(), 0);
	void * * res = new void * [indices.size()];
	void * * voidArr;
	for (unsigned int i = 0; i < indices.size(); ++i) {
		indices[i][0] = 0;
		for (unsigned int j = posDim; j < dim.size(); ++j)	{
			ind[j] = indices[i][j - posDim];
		}
		col = getDim(data, posDim, partSize, ind, posDim);
		voidArr = makeVoid(col);
		res[i] = (void *)hashline(voidArr, 0, partSize, attrH, attrHP);
		delete [] voidArr;
	}
	return res;
}

void * * hashDataLine(void * * data, vector<Attribute> attrH, vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, \
		unsigned int posDim, unsigned int posDimP, int partSize, int numP) {
	vector<vector<unsigned int> > indices = getAllIndices(dim, posDim, posDim);

	unsigned int slide = partSize;
	//if (numP == 1) slide = 1;
	int c = (dim[posDim].getSize()  / dimP[posDimP].getSize()) + 1;
	void * * res = new void * [slide * c];
	c = 0;
	for (unsigned int i = dimP[posDimP].getSize() - partSize; i < (dim[posDim].getSize() - partSize + 1); i += dimP[posDimP].getSize()) {
			
		if (i == 0) res[0] = (void *)hashDataParts(&(data[0]), attrH, attrHP, dim, posDim, partSize, indices);
		else {
			for (unsigned int j = 0; j < slide; ++j) {
				res[c + j] = (void *)hashDataParts(&(data[i - j]), attrH, attrHP, dim, posDim, partSize, indices);
			}
		}
		c += slide;
	}
	return res;
}

void * * hashData(void * * data, vector<Attribute> attrH, vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, \
		unsigned int posDim, unsigned int posDimP, int partSize, int numP) {
	void * * res = NULL;
	if (posDimP < dimP.size() && dim[posDim].getName() == dimP[posDimP].getName()) {
		if (posDimP + 1 >= dimP.size()) {
			return hashDataLine(data, attrH, attrHP, dim, dimP, posDim, posDimP, partSize, numP);
		}
		res = new void * [dim[posDim].getSize()];
		//for (unsigned int i = 0; i < dim[posDim].getSize(); ++i) res[i] = NULL;
		for (unsigned int i = dimP[posDimP].getSize() - 1; i < dim[posDim].getSize(); i += dimP[posDimP].getSize()) {
			res[i] = (void *)hashData((void * *)data[i], attrH, attrHP, dim, dimP, posDim + 1, posDimP + 1, partSize, numP);
		}
	} else {
		res = new void * [dim[posDim].getSize()];
		for (unsigned int i = 0; i < dim[posDim].getSize(); ++i) {
			res[i] = (void *)hashData((void * *)data[i], attrH, attrHP, dim, dimP, posDim + 1, posDimP, partSize, numP);
		}
	}
	return res;
}

void deleteHashDataParts(void * * data, unsigned int size) {
	//del++;
	for (unsigned int i = 0; i < size; ++i) {
		delete (vector<unsigned char> *) data[i];
	}
	delete [] data;
}

void deleteHashDataLine(void * * data, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, unsigned int posDimP, int partSize, int numP) {
	vector<vector<unsigned int> > indices = getAllIndices(dim, posDim, posDim);
	unsigned int slide = partSize;
	int c = 0;
	for (unsigned int i = dimP[posDimP].getSize() - partSize; i < (dim[posDim].getSize() - partSize + 1); i += dimP[posDimP].getSize()) {
		if (i == 0) {
			deleteHashDataParts((void * *)data[0], indices.size());
		}
		else {
			for (unsigned int j = 0; j < slide; ++j) {
				deleteHashDataParts((void * *)data[c+j], indices.size());
			}
		}
		c += slide;
	}

	delete [] data;
}

void deleteHashData(void * * data, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, unsigned int posDimP, int partSize, int numP) {
	if (posDimP < dimP.size() && dim[posDim].getName() == dimP[posDimP].getName()) {
		if (posDimP + 1 >= dimP.size()) {
			deleteHashDataLine(data, dim, dimP, posDim, posDimP, partSize, numP);
			//delete [] data;
			return;
		}
		//for (unsigned int i = 0; i < dim[posDim].getSize(); ++i) {
		for (unsigned int i = dimP[posDimP].getSize() - 1; i < dim[posDim].getSize(); i += dimP[posDimP].getSize()) {
			//if ((void * *)data[i] != NULL) deleteHashData((void * *)data[i], dim, dimP, posDim + 1, posDimP + 1, partSize, numP);
			deleteHashData((void * *)data[i], dim, dimP, posDim + 1, posDimP + 1, partSize, numP);
		}

	} else {
		for (unsigned int i = 0; i < dim[posDim].getSize(); ++i) {
			deleteHashData((void * *)data[i], dim, dimP, posDim + 1, posDimP, partSize, numP);
		}
	}
	delete [] data;
}

/*
void deleteHashDataLine(void * * data, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, unsigned int posDimP, int partSize, int numP) {
	vector<vector<unsigned int> > indices = getAllIndices(dim, posDim, posDim);
	
	unsigned int slide = partSize;
	//if (numP == 1) slide = 1;
	int c = 0;
	for (unsigned int i = dimP[posDimP].getSize() - partSize; i < (dim[posDim].getSize() - partSize + 1); i += dimP[posDimP].getSize()) {
		if (i == 0) {
			for (unsigned int k = 0; k < indices.size(); ++k) {
				delete ((vector<unsigned char> *)((void * *)data[c])[k]);
			} 
			delete [] (void * *)data[c];
		} else {
			for (unsigned int j = 0; j < slide; ++j)	{
				for (unsigned int k = 0; k < indices.size(); ++k) {
					delete ((vector<unsigned char> *)((void * *)data[c+j])[k]);
				} 
				delete [] (void * *)data[c+j];
			}
		}
		c += slide;
	}
}

void deleteHashData(void * * data, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, unsigned int posDimP, int partSize, int numP) {
	if (posDimP < dimP.size() && dim[posDim].getName() == dimP[posDimP].getName()) {
		if (posDimP + 1 >= dimP.size()) {
			deleteHashDataLine(data, dim, dimP, posDim, posDimP, partSize, numP);
			return;
		}
		for (unsigned int i = dimP[posDimP].getSize() - 1; i < dim[posDim].getSize(); i += dimP[posDimP].getSize()) {
			deleteHashData((void * *)data[i], dim, dimP, posDim + 1, posDimP + 1, partSize, numP);
		}
	} else {
		for (unsigned int i = 0; i < dim[posDim].getSize(); ++i) {
			deleteHashData((void * *)data[i],dim, dimP, posDim + 1, posDimP, partSize, numP);
		}
	}
	delete [] data;
}
*/
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

vector<vector<unsigned int> *> checkPart(void * * data, Reader * cache, vector<unsigned char> * hashP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, unsigned int posDim, int partSize, vector<unsigned int> cacheInd, \
	vector<vector<unsigned int> > &indices) {

	vector<vector<unsigned int> *> res(0);
	vector<void * *> col;
	vector<unsigned char> * hash;
	vector<unsigned int> *one = NULL;
	void * * voidArr;
	//vector<unsigned int> one(dim.size(), 0);

	if (posDim < cache->getDimInName()) {
		for (unsigned int i = 0; i < indices.size(); ++i) {
			indices[i][0] = cacheInd[posDim];
			col = getDim(cache, posDim, partSize, indices[i], posDim);
			voidArr = makeVoid(col);
			hash = hashline(voidArr, 0, partSize, attrH, attrHP);
			if (compareHash(hash, hashP)) {
				one = new vector<unsigned int>(dim.size());
				for (unsigned int j = 0; j < indices[i].size(); ++j) {
					(*one)[j + posDim] = indices[i][j];
				}
				res.push_back(one);
			}
			delete [] voidArr;
			delete hash;
		}
	} else {
		if (posDim + 1 >= dim.size()) {
			//hash = hashline(data, 0, partSize, attrH, attrHP);
			if (data[0] != NULL && compareHash((vector<unsigned char> *)data[0], hashP)) {
				return vector<vector<unsigned int> *>(1, new vector<unsigned int>(dim.size(), 0));
			}
		} else {
			for (unsigned int i = 0; i < indices.size(); ++i) {
				indices[i][0] = 0;
				//col = getDim(data, posDim, partSize, indices[i], posDim);
				//hash = hashline(makeVoid(col), 0, partSize, attrH, attrHP);
				if (data[i] != NULL && compareHash((vector<unsigned char> *)data[i], hashP)) {
					one = new vector<unsigned int>(dim.size());
					for (unsigned int j = 0; j < indices[i].size(); ++j) {
						(*one)[j + posDim] = indices[i][j];
					}
					res.push_back(one);
				}
			}
		}
	}
	return res;
}

vector<vector<unsigned int> *> checkParts(void * * data, Reader * cache, void * * hashP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, unsigned int posDimP, \
	vector<unsigned int> dimPositions, vector<unsigned int> cacheInd, int partSize, int numP, vector<vector<unsigned int> > &indices) {

	vector<vector<unsigned int> *> res(0), returned;
	for (int i = 0; i < numP; i++) {
		//patPosGlob[posDimP] = i;
		returned.clear();
		returned = checkPart(data, cache, (vector<unsigned char> *)hashP[i], attrH, attrHP, dim, posDim, partSize, cacheInd, indices);
		for (unsigned int k = 0; k < returned.size(); ++k) {
			(*returned[k])[dimPositions[posDimP]] -= (i * partSize);
		}	
		res.insert(res.end(), returned.begin(), returned.end());	
	}
	return res;
}

vector<vector<unsigned int> *> findParts(void * * data, Reader * cache, void * * hashP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, unsigned int posDimP, \
	vector<unsigned int> dimPositions, vector<unsigned int> cacheInd, int partSize, int numP) {

	vector<vector<unsigned int> *> res(0), returned;
	vector<vector<unsigned int> > indices = getAllIndices(dim, posDim, posDim);
	int slide = partSize;
	//if (numP == 1) slide = 1;
	int c = 0;
	for (unsigned int i = dimP[posDimP].getSize() - partSize; i < (dim[posDim].getSize() - partSize + 1); i += dimP[posDimP].getSize()) {
		//dataPosGlob[posDim] = i;
		if (i == 0) {
			returned.clear();
			if (posDim < cache->getDimInName()) {
				cacheInd[posDim] = 0;
				returned = checkParts(NULL, cache, hashP, attrH, attrHP, dim, dimP, posDim, posDimP, dimPositions, cacheInd, partSize, numP, indices);
			} else {
				returned = checkParts((void * *)(data[c]), cache, hashP, attrH, attrHP, dim, dimP, posDim, posDimP, dimPositions, cacheInd, partSize, numP, indices);
			}

			res.insert(res.end(), returned.begin(), returned.end());	
			//dataPosGlob[posDim] -= 1;
		} else {
			for (int j = 0; j < slide; ++j)	{
				returned.clear();
				if (posDim < cache->getDimInName()) {
					cacheInd[posDim] = i - j;
					returned = checkParts(NULL, cache, hashP, attrH, attrHP, dim, dimP, posDim, posDimP, dimPositions, cacheInd, partSize, numP, indices);
				} else {
					returned = checkParts((void * *)(data[c + j]), cache, hashP, attrH, attrHP, dim, dimP, posDim, posDimP, dimPositions, cacheInd, partSize, numP, indices);
				}

				for (unsigned int k = 0; k < returned.size(); ++k) {
					(*returned[k])[posDim] += (i - j);
					if ((*returned[k])[posDim] + partSize < (*returned[k])[posDim]) {
						(*returned[k])[posDim] = 0;
					}
				}

				res.insert(res.end(), returned.begin(), returned.end());	
				//dataPosGlob[posDim] -= 1;
			}
		}
		c += slide;
	}
	return res;
} 

vector<vector<unsigned int> *> find(void * * data, Reader * cache, void * * hashP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, unsigned int posDim, \
	unsigned int posDimP, vector<unsigned int> dimPositions, vector<unsigned int> cacheInd, int partSize, int numP) {

	vector<vector<unsigned int> *> res, returned;
	vector<unsigned int> one;

	if (posDim == cache->getDimInName()) {
		data = cache->read(cacheInd);
		data = hashData(data, attrH, attrHP, dim, dimP, posDim, posDimP, partSize, numP);
	}

	if (posDimP < dimP.size() && dim[posDim].getName() == dimP[posDimP].getName()) {
		dimPositions.push_back(posDim);
		if (posDimP + 1 >= dimP.size()) {
			res = findParts(data, cache, hashP, attrH, attrHP, dim, dimP, posDim, posDimP, dimPositions, cacheInd, partSize, numP);
		} else {
			for (unsigned int i = dimP[posDimP].getSize() - 1; i < dim[posDim].getSize(); i += dimP[posDimP].getSize()) {
				//dataPosGlob[posDim] = i;
				for (unsigned int j = 0; j < dimP[posDimP].getSize(); ++j) {
					//patPosGlob[posDim] = j;
				
					returned.clear();
					if (posDim < cache->getDimInName()) {
						cacheInd[posDim] = i;
						returned = find(NULL, cache, (void * *)hashP[j], attrH, attrHP, dim, dimP, posDim + 1, posDimP + 1, dimPositions, cacheInd, partSize, numP);
					} else {
						returned = find((void * *)data[i], cache, (void * *)hashP[j], attrH, attrHP, dim, dimP, posDim + 1, posDimP + 1, dimPositions, cacheInd, partSize, numP);
					}

					for (unsigned int k = 0; k < returned.size(); ++k) {
						(*returned[k])[posDim] += (i - j);
						//res.push_back(returned[k]);	
					}
					res.insert(res.end(), returned.begin(), returned.end());	
				}
			}
		}	
	} else {
		for (unsigned int i = 0; i < dim[posDim].getSize(); ++i) {
			returned.clear();
			if (posDim < cache->getDimInName()) {
				cacheInd[posDim] = i;
				returned = find(NULL, cache, hashP, attrH, attrHP, dim, dimP, posDim + 1, posDimP, dimPositions, cacheInd, partSize, numP);
			} else {
				returned = find((void * *)data[i], cache, hashP, attrH, attrHP, dim, dimP, posDim + 1, posDimP, dimPositions, cacheInd, partSize, numP);
			}
			for (unsigned int j = 0; j < returned.size(); ++j) {
				(*returned[j])[posDim] += i;
				//res.push_back(returned[j]);
			}
			res.insert(res.end(), returned.begin(), returned.end());	
		}
	}
	if (posDim == cache->getDimInName()) {
		deleteHashData(data, dim, dimP, posDim, posDimP, partSize, numP);
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
		if (sum > errors || errTmp == -1) {
			return false;
		}
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

vector<vector<unsigned int> *> find(Reader * cache, void * * hashP, void * * dataP, vector<Attribute> attrH, \
	vector<Attribute> attrHP, vector<Dimension> dim, vector<Dimension> dimP, int errors, int partSize, int numP) {

	vector<unsigned int> cacheInd;

	for (unsigned int i = 0; i < cache->getDimInName(); ++i) {
		cacheInd.push_back(0);
	}

	chrono::system_clock::time_point start = chrono::system_clock::now();
	vector<vector<unsigned int> *> res = find(NULL, cache, hashP, attrH, attrHP, dim, dimP, 0, 0, vector<unsigned int>(), cacheInd, partSize, numP);
	chrono::duration<double> sec = chrono::system_clock::now() - start;
    cout << "Find took " << sec.count() << " seconds\n";
	//Preverification
	cout << res.size() << endl;
	
	start = chrono::system_clock::now();
	for (vector<vector<unsigned int> *>::iterator it = res.begin(); it != res.end();) {
		if (!preverif(cache, dataP, attrH, attrHP, dim, dimP, **it, errors)) {
			it = res.erase(it);
		} else {
			++it;
		}
	}

	sec = chrono::system_clock::now() - start;
    cout << "Preverification took " << sec.count() << " seconds\n";
    /*for (unsigned int i = 0; i < res.size(); ++i) {
		for (unsigned int j = 0; j < res[i] -> size(); ++j) {
			cout << (*res[i])[j] << ", ";
		}
		cout << endl;
	}*/
	//Approximate check of the rest of the pattern
	start = chrono::system_clock::now();
	for (vector<vector<unsigned int> *>::iterator it = res.end() - 1; it != res.begin() - 1;) {
		if (!dynCheck(cache, dataP, attrH, attrHP, dim, dimP, **it, errors)) {
			it = res.erase(it);
		}
		--it;
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
	cout << "Num of parts: " << numP << endl;
	cout << "Part size: " << partSize << endl;

	void * * dataPatt = readData(pattern, patternAttrHeader, dimPatt);
	void * * hashP = hashPattern(dataPatt, patternAttrHeader, dimPatt, 0, partSize, numP);
	
	cout << "Finding..." << endl;
	vector<vector<unsigned int> *> res;

	//chrono::system_clock::time_point start = chrono::system_clock::now();
	res = find(cache, hashP, dataPatt, attrHeader, patternAttrHeader, dim, dimPatt, errors, partSize, numP);
	//chrono::duration<double> sec = chrono::system_clock::now() - start;
    //cout << "took " << sec.count() << " seconds\n";
    cout << res.size() << endl;
	/*if (res.size() == 0){
		cout << "no solutions found" << endl;
	} else {
		for (unsigned int i = 0; i < res.size(); ++i) {
			for (unsigned int j = 0; j < res[i] -> size(); ++j) {
				cout << (*res[i])[j] << ", ";
			}
			cout << endl;
		}
	}*/
	delete cache;
	deleteHashP(hashP, dimPatt, numP);
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
