#ifndef hc_H
#define hc_H

#include <iostream>
#include <string>
#include <list>
#include "gfunction.hpp"

using namespace std;

class HyperH{
	int key;
	int value;
	HyperH *next;
public:
	HyperH(int key, int value) :
		key(key), value(value), next(NULL) {}

	~HyperH(){}
	int getKey(){ return key; }
	int getValue() { return value; }
	HyperH *getNext() { return next; }
	void setNext(HyperH *n){ next = n; }
};

class HyperNode{
	myVector *value;
	HyperNode *next;
public:
	HyperNode(myVector *value) :
		value(value), next(NULL) {}

	~HyperNode(){}

	myVector *getValue() { return value; }
	void setValue(myVector *val){ value = val; }
	HyperNode *getNext() { return next; }
	void setNext(HyperNode *n){ next = n; }
};

class HyperCube{
	HyperNode **buckets;
	Gfunction *gfunction;
	int mapSize;
	int M;
	int probes;
	string metric;
	HyperH *hList;
  public:
	HyperCube(string , vector<myVector> *, int, int);
	~HyperCube(){}

	int euclideanHashFunction(myVector *);
	int cosineHashFunction(myVector *);
	int calculateHashValue(myVector *);
	void setHyperCube(vector<myVector> *);
	void putVector(myVector *);
	void approxNNSearch(myVector *, myVector **, double *);
	void rangeSearch(myVector*, double *, vector<myVector *> *);
	void printHyperH();
	void printNstuff();
};

#endif
