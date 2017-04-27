#include "reader.h"

using namespace std;

Reader::Reader(string name, vector<Dimension> dims, vector<Attribute> attrs) {
	dir = "./";
	meta = dir + name;
	cacheSize = 50;
	
	int pos = name.find("_meta.csv");
	fileTemp = dir + name.substr(0, pos);

	dim = dims;
	attr = attrs;
	readDimNum();
}

Reader::~Reader() {
	for (unsigned int i = 0; i < cache.size(); ++i) {
		if (cache[i] != NULL) {
			deleteData(cache[i], dimInName);
			delete [] cache[i];
		}
		delete [] fileMasks[i];
	}
}

unsigned int Reader::getDimInName() {
	return dimInName;
}

void Reader::print() {
	cout << "Meta: " << meta << endl;
	cout << "fileTemp: " << fileTemp << endl;
	cout << "dimInName: " << dimInName << endl;
}

void Reader::readDimNum() {
	ifstream file;
	file.open(meta.c_str());
	string line1, line2;

	getline(file, line1, '\n');
	getline(file, line2, '\n');

	int pos = 8; //dimName:_ 
	string num = line2.substr(pos, line2.length() - 1);
	dimInName = stoi(num);
	
	initCache();
	file.close();
}

void Reader::initCache() {
	int count = 1, rest = 1;
	unsigned long maskSize = 1;
	string maskF = fileTemp + "_mask.bin";

	ifstream file;
	file.open(maskF.c_str(), ios::binary|ios::in);

	for (unsigned int i = 0; i < dim.size(); ++i) {
		maskSize *= dim[i].getSize();
	}
	maskSize /= 8;

	unsigned char * buffer = new unsigned char[maskSize + 1];

	file.read((char *)buffer, maskSize + 1);

	for (unsigned int i = 0; i < dimInName; ++i) {
		count *= dim[i].getSize();
	}
	for (unsigned int i = dimInName; i < dim.size(); ++i) {
		rest *= dim[i].getSize();
	}

	int byte = 0;
	int bitPos = 0;

	for (int i = 0; i < count; ++i)	{
		cache.push_back(NULL);
		fileMasks.push_back(new bool[rest]);
		for (int j = 0; j < rest; ++j) {
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
		//cout << "read " << indices[0] << " " << indices[1] << endl;
		return readFile(indices);
	}
	return res;
}

void * * Reader::findInCache(vector<unsigned int> indices) {
	int pos = 0;
	if (indices.size() > 0) {
		pos = indices[0];
		for (unsigned int i = 1; i < indices.size(); ++i) {
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
		for (unsigned int i = 0; i < attr.size(); ++i) {
			cellMask.push(buffer[byte + i] & bit);
		}
		bit /= 2;
	}
	byte += attr.size();
}

void * * Reader::readCell(unsigned char * buffer, unsigned int &byte, queue<bool> &cellMask) {
	void * * cell = new void * [attr.size()];
	if (cellMask.size() == 0) {
		readMask(buffer, byte, cellMask);
	}

	for (unsigned int i = 0; i < attr.size(); ++i) {
		if (cellMask.front()) {
			cell[i] = readInt(buffer, byte);
		} else {
			cell[i] = NULL;
		}
		cellMask.pop();
	}
	return cell;
}

void * * Reader::readDim(unsigned char * buffer, unsigned int posDim, unsigned int &byte, int pos, unsigned int &posInMask, queue<bool> &cellMask) {
	void * * res = new void * [dim[posDim].getSize()];
	if (posDim + 1 >= dim.size()) {
		for (unsigned int i = 0; i < dim[posDim].getSize(); ++i) {
			if (fileMasks[pos][posInMask++]) {
				res[i] = (void *)readCell(buffer, byte, cellMask);
			} else {
				res[i] = NULL;
			}
		}
	} else {	
		for (unsigned int i = 0; i < dim[posDim].getSize(); ++i) {
			res[i] = (void *)readDim(buffer, posDim + 1, byte, pos, posInMask, cellMask);
		}
	}
	return res;
}

void * * Reader::readFile2(ifstream &file, int pos) {
	unsigned long s = 1;
	for (unsigned int i = dimInName; i < dim.size(); ++i) {
		s *= dim[i].getSize();
	}
	s *= (3 * attr.size());
	unsigned char * buffer = new unsigned char[s];
	file.read((char *)buffer, s);

	queue<bool> cellMask;
	unsigned int posInMask = 0, byte = 0;
	void * * res = readDim(buffer, dimInName, byte, pos, posInMask, cellMask);
	delete [] buffer;
	return res;
}

void * * Reader::readFile(vector<unsigned int> indices) {
	string fileName = fileTemp;
	for (unsigned int i = 0; i < indices.size(); ++i) {
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
		for (unsigned int i = 1; i < indices.size(); ++i) {
			pos = pos * dim[i].getSize() + indices[i];
		}
	}
	files.push(pos);
	cache[pos] = readFile2(file, pos);
	file.close();
	return cache[pos];
}

void Reader::deleteOneLine(void * * line) {
	for (unsigned int i = 0; i < attr.size(); ++i) {
		if (line[i] != NULL)
			delete (int *)line[i];
	}
}

void Reader::deleteData(void * * data, unsigned int posDim) {
	for (unsigned int i = 0; i < dim[posDim].getSize(); ++i) {
		if (data[i] != NULL) {
			if (posDim + 1 >= dim.size()) {
					deleteOneLine((void * *)data[i]);
			} else {
					deleteData((void * *)data[i], posDim + 1);
			}
		}
		delete [] (void * *)data[i];
	}
}
