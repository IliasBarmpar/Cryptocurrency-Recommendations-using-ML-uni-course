#ifndef lsh_H
#define lsh_H

#include <iostream>
#include <string>
#include <list>
#include "gfunction.hpp"

using namespace std;

class HashNode{ 		//bucket
	myVector *value;	//a single vector
	HashNode *next;
  public:
	HashNode(myVector *value) :
		value(value), next(NULL) {}
	~HashNode(){}

	myVector *getValue() { return value; }
	void setValue(myVector *val){ value = val; }
	HashNode *getNext() { return next; }
	void setNext(HashNode *n){ next = n; }
};

class HashTable{
	HashNode **map;
	Gfunction *gfunction;
	int mapSize;
	string metric;
  public:
	HashTable(string , int , int );
	~HashTable();

	int euclideanHashFunction(myVector *);
	int cosineHashFunction(myVector *);
	int calculateHashValue(myVector *);
	void putVector(myVector *);
	void approxNNSearch(myVector *,  myVector **, double *);
	void rangeSearch(myVector *, double , vector<myVector *> *);
	void rangeSearch2(myVector *, vector<myVector *> *);
	void printNstuff();
	void printNstuff2();
};

class LSH{
	vector<HashTable> hashtables;
	string metric;
public:
	LSH(vector<myVector> *, string, int, int);
	~LSH(){}

	void addTable(HashTable *);
	void rangeSearch(myVector*, double , vector<myVector *> *);
	void rangeSearch2(myVector*, vector<myVector *> *);
	void approxNNSearch(myVector *, myVector **, double *);
	void printTemp();
	void printTemp2();
};

#endif
