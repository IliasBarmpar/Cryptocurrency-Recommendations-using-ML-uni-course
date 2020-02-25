#include "gfunction.hpp"

Gfunction::Gfunction(string m, int K){
	if(m=="euclidean")
		_h = new euclideanFamily(K);
	else
		_h = new cosineFamily(K);
}

int Gfunction::getMapSize(int size){
	return _h->getMapSize(size);
}

Hfamily* Gfunction::getHfamilyRef(){
	return _h;
}

void Gfunction::getValuesOfH(vector<int> *_p, vector<int> *hvalues){
	_h->getValuesOfH(_p, hvalues);
}

double Gfunction::getDistance(vector<int> *_q, vector<int> *_p){
	return _h->getDistance(_q,_p);
}
