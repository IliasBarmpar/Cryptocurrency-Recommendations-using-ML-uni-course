#ifndef hfam_H
#define hfam_H

#include <iostream>
#include <vector>
#include "myVector.hpp"

using namespace std;

static int D = 128;

class Hfamily{
  public:
	virtual ~Hfamily(){}

	virtual void setPointer(Hfamily *ptr){}
	virtual void getValuesOfH(vector<double> *_p, vector<int> *hvalues){}
	virtual int getMapSize(int size){ return -1;}
	virtual double getDistance(vector<double> *_q, vector<double> *_p){ return -1; }
};

#endif
