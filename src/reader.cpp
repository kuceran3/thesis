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
	string maskF = fileTemp + "_mask.bin";

	ifstream file;
	file.open(maskF.c_str(), ios::binary|ios::in);

	for (unsigned int i = 0; i < dim.size(); ++i) {
		maskSize *= dim[i].getSize();
	}
	maskSize /= 8;
	//cout << "maskSize: " << maskSize << endl;

	unsigned char * buffer = new unsigned char[maskSize + 1];

	file.read((char *)buffer, maskSize + 1);
	//cout << "maskfile eof: " << file.eof() << " readable: " << file.good() << endl;

	for (unsigned int i = 0; i < dimInName; ++i) {
		count *= dim[i].getSize();
	}
	for (unsigned int i = dimInName; i < dim.size(); ++i) {
		rest *= dim[i].getSize();
	}
	//cout << "Count: " << count << " Rest: " << rest << endl;

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
	/*for (unsigned int i = 0; i < indices.size(); ++i) {
		cout << indices[i] << ", ";
	}*/
		
	if (res == NULL) {
		//cout << "not using cache: " << endl;
		//for (unsigned int i = 0; i < indices.size(); ++i) {
		//	cout << indices[i] << ", ";
		//}
		return readFile(indices);
	}
	//cout << "using cache: " << indices.size() << " ";
	//for (unsigned int i = 0; i < indices.size(); ++i) {
	//	cout << indices[i] << ", ";
	//}
	//cout << endl;
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

void * Reader::readCell(unsigned char * buffer, unsigned int &byte, queue<bool> &cellMask) {
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
	return (void *)cell;
}

void * * Reader::readDim(unsigned char * buffer, unsigned int posDim, unsigned int &byte, int pos, unsigned int &posInMask, queue<bool> &cellMask) {
	void * * res = new void * [dim[posDim].getSize()];
	if (posDim + 1 >= dim.size()) {
		for (unsigned int i = 0; i < dim[posDim].getSize(); ++i) {
			if (fileMasks[pos][posInMask++]) {
				res[i] = readCell(buffer, byte, cellMask);
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
	//cout << "File size in bits " << s << endl;

	unsigned char * buffer = new unsigned char[s];
	file.read((char *)buffer, s);

	//cout << "datafile eof: " << file.eof() << " readable: " << file.good() << endl;

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
	//cout << "Filename: " << fileName << endl;
	
	if (files.size() >= cacheSize) {
	//	cout << "Full cache" << endl;
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
	//cout << "Reading Data for pos: " << pos << endl;
	files.push(pos);
	cache[pos] = readFile2(file, pos);
	//printData(cache[pos], attr, dim, dimInName);
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
