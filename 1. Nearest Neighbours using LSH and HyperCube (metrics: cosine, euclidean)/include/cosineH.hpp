#ifndef cos_H
#define cos_H

#include <iostream>
#include <vector>
#include <random>
#include <chrono>

#include "Hfamily.hpp"

using namespace std;

class cosineH{
	vector<double> _r;
  public:
	cosineH();
	~cosineH(){}

	int calculateValue(vector<int> *);
};

class cosineFamily : public Hfamily{
	vector<cosineH> hfam;
  public:
	cosineFamily(int );
	~cosineFamily(){}

	int getMapSize(int );
	void getValuesOfH(vector<int> *, vector<int> *);	
	double getDistance(vector< int > *, vector< int > *);
};
#endif
