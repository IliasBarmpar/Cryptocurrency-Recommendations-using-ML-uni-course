/*
 * euclideanH.cpp
 *
 *  Created on: Oct 30, 2018
 *      Author: effy
 */
#include "euclideanH.hpp"

Rfamily *_rfam = NULL; //todo dhlwsh sto cpp extern sto hpp

Rfamily::Rfamily(){
	for(int i = 0; i < D; ++i){
		int temp = rand()% 200;
		_rfamily.push_back(temp);
	}
}
//Creates an Hmember for the cosine family by taking D ammount of normally distributed numbers plus a uniform real number
euclideanH::euclideanH(){
	//_v = new VectorV;
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(seed);
	normal_distribution<double> distribution(0.0,1.0);

	for(int i=0; i<D; i++){
		double temp = distribution(generator);
		_v.push_back(temp);
	}

	//_t = new ShiftT;
	uniform_real_distribution<> dis(0.0,W);
	do{
		_t = dis(generator);
	}while(_t==W); // this addition ensures that we get a T in [0,w)!
}

// Calculates the value using the v's and the t when its given a vector
int euclideanH::calculateValue(vector<int> *_p){ //calculate value given a point
	//cout << "initiating eclideanH->calculateValue EUCLIDEAN" << endl;
	double tempd = 0;
	vector< double >::iterator vecV_it = _v.begin();
	for(vector< int >::iterator vecP_it = _p->begin(); vecP_it != _p->end(); ++vecP_it, ++vecV_it)
		tempd += (*vecV_it)*(*vecP_it);	//v*p

	tempd += _t; 		// v*p + t
	tempd = tempd / W;	//(v*p + t)/w
	int temp = tempd;	//keep only the integer part
	return temp;
}

///////////////////////////////////////////////////////////////////////////////////////

euclideanFamily::euclideanFamily(int K){
	TableSize = 2000; //temp
	if(_rfam == NULL)
		_rfam = new Rfamily();
	for(int i = 0; i < K; i++){ //TODO random if need be.
		euclideanH *temp = new euclideanH();
		hfam.push_back(*temp);
	}
}


int euclideanFamily::getMapSize(int size){
	return size/4;
}


// simply returns the family of H members
void euclideanFamily::getValuesOfH(vector<int> *_p, vector<int> *hvalues){
	for(vector<euclideanH>::iterator hfam_it = hfam.begin(); hfam_it != hfam.end(); ++hfam_it)
		hvalues->push_back( (*hfam_it).calculateValue(_p) );
}


double euclideanFamily::getDistance(vector< int > *_q, vector< int > *_p){
	/*int hflag = 0;
	//Check if theyve got the same g functions
	cout << "hfamSize.size()=" << hfam.size() << endl;
	for(vector<euclideanH>::iterator h_it = hfam.begin(); h_it != hfam.end(); ++h_it){
		int temp1 = (*h_it).calculateValue(_q);
		int temp2 = (*h_it).calculateValue(_p);
		cout << "(*b_it)=" << temp1 << " | " << "(*c_it)=" << temp2 << endl;
		if( temp1 != temp2 ){
			hflag = 1;
			//break;
		}
	}
	if(hflag==1){
		return numeric_limits<double>::max();
	}else{*/
	//Euclidean Distance
	long double temp = 0;
	vector<int>::iterator p_it = _p->begin();
	for( vector<int>::iterator q_it = _q->begin(); q_it != _q->end(); ++q_it, ++p_it)
		temp += ( (*q_it) - (*p_it) )*( (*q_it) - (*p_it) );
	temp = sqrt( temp );
	return (double)temp;
	//}
}
