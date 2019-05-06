#ifndef cos_H
#define cos_H

#include <iostream>
#include <vector>
#include <random>
#include <chrono>

#include "Hfamily.hpp"

using namespace std;

double cosineDistOfTwoVectors(vector< double > *, vector< double > *);

class cosineH{
	vector<double> _r;
  public:
	cosineH();
	~cosineH(){}

	int calculateValue(vector<double> *);
};

class cosineFamily : public Hfamily{
  public:
	vector<cosineH> hfam;
	cosineFamily(int );
	~cosineFamily(){}

	void setPointer(Hfamily *);
	void getValuesOfH(vector<double> *, vector<int> *);
	int getMapSize(int );
	double getDistance(vector<double> *, vector< double > *);
};
#endif
