#ifndef LIBNMVBAHFUGWBDSNCLALDJHSWOWJHFHFBGSAO
#define LIBNMVBAHFUGWBDSNCLALDJHSWOWJHFHFBGSAO
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <chrono>
#include "dimension.h"
#include "attribute.h"
#include "reader.h"

using namespace std;

//File reading
vector<Attribute> readHeader(string row, vector<Dimension> &dim);
int readDimNum(string row);
void * readType(string type, string data);
void * * readOneLine(string line, vector<Attribute > attrHeader, vector<Dimension> &dim);
void * * readData(ifstream &file, vector<Attribute > attrHeader, vector<Dimension> &dim, unsigned int posDim);
void * * readData(ifstream &file, vector<Attribute > attrHeader, vector<Dimension> &dim);
//--------------------------------------------------------------------------------------------------------------------------
//Data printing
string toString(void * data, string type);
void printOneLine(void * * line, vector<Attribute > attrHeader);
void printData2(void * * data, vector<Attribute > attrHeader, vector<Dimension> &dim, unsigned int posDim);
void printData(void * * data, vector<Attribute > attrHeader, vector<Dimension> &dim);
//--------------------------------------------------------------------------------------------------------------------------
//Cleaning
void deleteType(void * data, string type);
void deleteOneLine(void * * line, vector<Attribute > attrHeader);
void deleteData(void * * data, vector<Attribute > attrHeader, vector<Dimension> &dim, unsigned int posDim);
void deleteData(void * * data, vector<Attribute > attrHeader, vector<Dimension> &dim);
void deleteOneHash(void * * hashP, int numP);
void deleteHashP(void * * hashP, vector<Dimension> dimP, unsigned int posDimP, int numP);
void deleteHashP(void * * hashP, vector<Dimension> dimP, int numP);
//--------------------------------------------------------------------------------------------------------------------------
bool checkHeaders(vector<Dimension> dim, vector<Dimension> dimPatt, vector<Attribute> attrHeader, vector<Attribute> patternAttrHeader);
int compareType(void * first, void * second, string type);
bool compareItem(void * * data, void * * dataP, vector<Attribute> attrH, vector<Attribute> attrHP);
//--------------------------------------------------------------------------------------------------------------------------
//Framework for computing distances
void * * getItem(void * * data, vector<unsigned int> &indices, unsigned int posDim);
void * * getItem(Reader * cache, vector<unsigned int> &indices);
vector<void * *> getDim(void * * data, unsigned int dimInd, unsigned int length, vector<unsigned int> &indices, unsigned int posDim);
vector<void * *> getDim(Reader * cache, unsigned int dimInd, unsigned int length, vector<unsigned int> &indices, unsigned int posDim);
vector<vector<unsigned int> > getIndices(vector<Dimension> dim, vector<Dimension> dimP, unsigned int pos, vector<unsigned int> res, unsigned int posDim, unsigned int posDimP);
//--------------------------------------------------------------------------------------------------------------------------
//Edit distance
int editDistance(vector<void * *> col, vector<void * *> colP, vector<Attribute> attrH, vector<Attribute> attrHP);
#endif