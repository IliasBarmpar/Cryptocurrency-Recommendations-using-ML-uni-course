#ifndef myV_H
#define myV_H

#include <iostream>
#include <vector>

using namespace std;

struct myVector{
	int id;
	vector<double> coordinates;
	double similarity;

	~myVector(){
		coordinates.clear();
		vector<double>().swap(coordinates);
	}
};
#endif
