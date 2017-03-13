#ifndef LIBNMVBAHFUGWBDSNCLALDJHSWOWJHFHFBGSAO
#define LIBNMVBAHFUGWBDSNCLALDJHSWOWJHFHFBGSAO
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "dimension.h"
#include "attribute.h"

using namespace std;

//File reading
vector<Attribute> readHeader(string row, vector<Dimension> &dim);
void * readType(string type, string data);
void * * readOneLine(string line, vector<Attribute > attrHeader, vector<Dimension> &dim);
void * * readData(ifstream &file, vector<Attribute > attrHeader, vector<Dimension> &dim, unsigned int posDim);
void * * readData(ifstream &file, vector<Attribute > attrHeader, vector<Dimension> &dim);
//--------------------------------------------------------------------------------------------------------------------------
//Data printing
string toString(void * data, string type);
void printOneLine(void * * line, vector<Attribute > attrHeader);
void printData(void * * data, vector<Attribute > attrHeader, vector<Dimension> &dim, unsigned int posDim);
void printData(void * * data, vector<Attribute > attrHeader, vector<Dimension> &dim);
//--------------------------------------------------------------------------------------------------------------------------
//Cleaning
void deleteType(void * data, string type);
void deleteOneLine(void * * line, vector<Attribute > attrHeader);
void deleteData(void * * data, vector<Attribute > attrHeader, vector<Dimension> &dim, unsigned int posDim);
void deleteData(void * * data, vector<Attribute > attrHeader, vector<Dimension> &dim);
//--------------------------------------------------------------------------------------------------------------------------
#endif