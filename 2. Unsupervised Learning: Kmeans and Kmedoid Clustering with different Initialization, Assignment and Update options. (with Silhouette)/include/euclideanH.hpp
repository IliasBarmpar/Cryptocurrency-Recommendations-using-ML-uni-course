#ifndef euc_H
#define euc_H

#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <math.h>
#include "Hfamily.hpp"

using namespace std;

static double W = 2.0;

class Rfamily{
  public:
	Rfamily();
	~Rfamily(){}

	vector<int> _rfamily;
};
extern Rfamily *_rfam;

double euclideanDistOfTwoVectors(vector< double > *, vector< double > *);

class euclideanH{
	vector<double> _v;
	double _t;
  public:
	euclideanH();
	~euclideanH(){};

	int calculateValue(vector<double> *_p);
};

class euclideanFamily : public Hfamily{
	int TableSize;
	vector<euclideanH> hfam;
  public:

	euclideanFamily(int K);
	~euclideanFamily(){}
	
	void getValuesOfH(vector<double> *, vector<int> *);
	void setPointer(Hfamily *);
	int getMapSize(int );
	double getDistance(vector<double> *, vector< double > *);
};

#endif








