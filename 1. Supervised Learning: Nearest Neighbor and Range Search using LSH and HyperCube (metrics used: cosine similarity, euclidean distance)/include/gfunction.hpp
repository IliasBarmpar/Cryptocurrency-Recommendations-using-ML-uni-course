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

	int getMapSize(int );
	Hfamily* getHfamilyRef();
	void getValuesOfH(vector<int> *, vector<int> *);
	double getDistance(vector<int> *, vector<int> *);
};


#endif









