#include "reader.h"

using namespace std;

Reader::Reader(string name, int sizePatt, vector<Dimension> dims, vector<Attribute> attrs) {
	orig = name;
	dir = "./";
	cacheSize = 50;
	
	int pos = name.find(".csv");
	fileTemp = dir + name.substr(0, pos);

	meta = fileName + "_meta.csv";
	dim = dims;
	attr = attrs;
	readDimNum();
}

Reader::~Reader() {
	for (int i = 0; i < cache.size(); ++i) {
		if (cache[i] != NULL) {
			deleteData(cache[i], dimInName);
			delete [] cache[i];
		}
		delete [] fileMasks[i];
	}
}

void Reader::readDimNum() {
	ifstream file;
	file.open(meta.c_str());
	string line1, line2;

	getline(meta, line1);
	getline(meta, line2);

	int pos = 8; //dimName:_ 
	string num = line2.substr(pos, line2.length() - 1);
	dimInName = stoi(num);
	
	//readDim(dimInName, line1);
	initCache();
	file.close();
}

/*void Reader::readDim(int num, string line) {
	int pos1, pos2, len2;
	string name, type;
	
	for (int i = 0; i < num; ++i) {
		pos1 = row.find(":dim");
		pos2 = row.find(",");

		len2 = pos2 - pos1 - 1;

		name = row.substr(0, pos1);
		type = row.substr(pos1 + 1, len2);

		dim.push_back(Dimension(name, (unsigned int)stoi(type.substr(3))));
		row = row.substr(pos2 + 1);
	}
}*/

void Reader::initCache() {
	int count = 1, rest = 1;
	unsigned long maskSize = 1;
	string maskF = fileName + "_mask.bin";

	ifstream file;
	file.open(maskF.c_str(), ios::binary|ios::in);

	for (int i = 0; i < dim.size(); ++i) {
		maskSize *= dim[i].getSize();
	}
	maskSize /= 8;

	unsigned char * buffer = new unsigned char[maskSize + 1];

	file.read(buffer, maskSize + 1);

	for (int i = 0; i < dimInName; ++i) {
		count *= dim[i].getSize();
	}
	for (int i = dimInName; i < dim.size(); ++i) {
		rest *= dim[i].getSize();
	}

	int byte = 0;
	int bitPos = 0;

	for (int i = 0; i < count; ++i)	{
		cache.push_back(NULL);
		fileMasks.push_back(new bool[rest])
		for (int j = 0; j < rest; ++j) {
			if (buffer[byte] > )
			fileMasks[i][j] = buffer[byte] > 127;
			buffer[byte] <<= 1;
			if (++bitPos >= 8) {
				bitPos = 0;
				byte++;
			}
		}
	}
	delete [] buffer;
}

void * * Reader::read(vector<unsigned int> indices) {
	void * * res = findInCache(indices);
	if (res == NULL) {
		return readFile(indices);
	}
	return res;
}

void * * Reader::findInCache(vector<unsigned int> indices) {
	int pos = 0;
	if (indices.size() > 0) {
		pos = indices[0];
		for (int i = 1; i < indices.size(); ++i) {
			pos = pos * dim[i].getSize() + indices[i];
		}
	}
	return cache[pos];
}

void * Reader::readInt(unsigned char * buffer, unsigned int &byte) {
	int * val = new int();
	*val = (buffer[byte + 1] << 8 | buffer[byte]);
	byte += 2;
	return (void *)val;
}

void Reader::readMask(unsigned char * buffer, unsigned int &byte, queue<bool> &cellMask) {
	unsigned char bit = 128;
	for (int j = 0; j < 8; ++j) {
		for (int i = 0; i < attr.size(); ++i) {
			cellMask.push(buffer[byte + i] & bit);
		}
		bit /= 2;
	}
	byte += attr.size();
}

void * * Reader::readCell(unsigned char * buffer, unsigned int &byte, queue<bool> &cellMask) {
	void * * cell = new void * [attr.size()];

	if (cellMask.size() == 0) {
		readMask(buffer, byte, cellMask)
	}

	for (int i = 0; i < attr.size(); ++i) {
		if (cellMask.front()) {
			cell[i] = readInt(buffer, byte)
		} else {
			cell[i] = NULL;
		}
		cellMask.pop();
	}
	return res;
}

void * * Reader::readDim(unsigned char * buffer, unsigned int posDim, unsigned int &byte, int maskPos, unsigned int &pos, queue<bool> &cellMask) {
	void * * res = new void * [dim[posDim].getSize()];
 
	if (posDim + 1 >= dim.size()) {
		for (int i = 0; i < dim[posDim].getSize(); ++i) {
			if (fileMasks[maskPos][pos++]) {
				res[i] = (void *)readCell(buffer, byte, cellMask);
			} else {
				res[i] = NULL;
			}
		}
	} else {
		for (int i = 0; i < dim[posDim].getSize(); ++i) {
			res[i] = (void *)readDim(buffer, posDim + 1, byte, maskPos, pos, cellMask);
		}
	}
	return res;
}

void * * Reader::read(ifstream &file, int pos) {
	unsigned long s = 1;
	for (int i = dimInName; i < dim.size(); ++i) {
		s *= dim[i].getSize();
	}
	s *= (3 * attr.size());

	unsigned char * buffer = new unsigned char[s];
	file.read(buffer, s);

	queue<bool> cellMask;
	void * * res = readDim(buffer, indices.size(), 0, pos, 0, cellMask);
	delete [] buffer;
	return res;
}

void * * Reader::readFile(vector<unsigned int> indices) {
	string fileName = fileTemp;
	for (int i = 0; i < indices.size(); ++i) {
		fileName += '_' + to_string(indices[i]);
	}
	fileName += ".bin";

	ifstream file;
	file.open(fileName.c_str(), ios::binary);
	
	if (files.size() >= cacheSize) {
		deleteData(cache[files.front()], indices.size());
		delete [] cache[files.front()];
		cache[files.front()] = NULL;
		files.pop();
	}

	int pos = 0;
	if (indices.size() > 0) {
		pos = indices[0];
		for (int i = 1; i < indices.size(); ++i) {
			pos = pos * dim[i].getSize() + indices[i];
		}
	}
	files.push(pos);
	cache[pos] = read(file, pos);
	file.close();
	return cache[pos];
}

void Reader::deleteOneLine(void * * line) {
	for (unsigned int i = 0; i < attr.size(); ++i) {
		delete (int *)line[i];
	}
}

void Reader::deleteData(void * * data, unsigned int posDim) {
	for (unsigned int i = 0; i < dim[posDim].getSize(); ++i) {
		if (posDim + 1 >= dim.size()) {
			deleteOneLine((void * *)data[i]);
		} else {
			deleteData((void * *)data[i], posDim + 1);
		}
		delete [] (void * *)data[i];
	}
}





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
		delete (void * *)data[i];
	}
}

void deleteData(void * * data, vector<Attribute> attrHeader, vector<Dimension> &dim) {
	if (dim.size() <= 0)
		deleteOneLine(data, attrHeader);
	else 
		deleteData(data, attrHeader, dim, 0);
	delete data;
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