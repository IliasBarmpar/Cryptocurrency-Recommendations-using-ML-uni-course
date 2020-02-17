#ifndef gfun_H
#define gfun_H

#include <iostream>
#include <string>
#include <vector>
#include <list>

#include "euclideanH.hpp"
#include "cosineH.hpp"

using namespace std;

class Gfunction{ // G basically sets its chosen h combination with a single point p
	Hfamily *_h;
  public:
	Gfunction(string , int );
	~Gfunction(){}

	Hfamily* getHfamilyRef();
	void getValuesOfH(vector<double> *, vector<int> *);
	int getMapSize(int );
	double getDistance(vector<double> *, vector<double> *);
};


#endif
