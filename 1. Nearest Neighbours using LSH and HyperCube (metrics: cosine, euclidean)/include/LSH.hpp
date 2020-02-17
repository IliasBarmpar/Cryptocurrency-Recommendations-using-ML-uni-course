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

	void rangeSearch(myVector *, double *, list<myVector *> *);

	//Temp function
	void printNstuff();
};


class LSH{
	vector<HashTable> hashtables;
public:
	LSH(){}
	~LSH(){}

	void addTable(HashTable *);

	// Range Search (r, c) we assume c = 1
	void rangeSearch(myVector*, double *, list<myVector *> *);

	void approxNNSearch(myVector *, myVector **, double *);
};

#endif
