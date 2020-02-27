#ifndef euc_H
#define euc_H

#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <math.h>
#include "Hfamily.hpp"

using namespace std;

static int W = 4;

class Rfamily{
  public:
	Rfamily();
	~Rfamily(){}

	vector<int> _rfamily;
};
extern Rfamily *_rfam;

class euclideanH{
	vector<double> _v;
	double _t;
  public:
	euclideanH();
	~euclideanH(){};

	int calculateValue(vector<int> *_p);
};

class euclideanFamily : public Hfamily{
	vector<euclideanH> hfam;
	int TableSize;
  public:
	euclideanFamily(int K);
	~euclideanFamily(){}

	int getMapSize(int );
	void getValuesOfH(vector<int> *, vector<int> *);	
	double getDistance(vector< int > *, vector< int > *);
};

#endif






