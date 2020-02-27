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

	virtual int getMapSize(int size){}
	
	virtual void getValuesOfH(vector<int> *_p, vector<int> *hvalues){}

	virtual double getDistance(vector<int> *_q, vector<int> *_p){}
};

#endif
