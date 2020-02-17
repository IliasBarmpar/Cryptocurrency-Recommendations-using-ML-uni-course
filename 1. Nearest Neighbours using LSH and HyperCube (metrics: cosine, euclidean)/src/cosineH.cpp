#include "cosineH.hpp" 

//Creates an Hmember for the cosine family by taking D ammount of normally distributed numbers
cosineH::cosineH(){
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(seed);
	normal_distribution<double> distribution(0.0,1.0);

	for(int i=0; i<D; i++){
		double temp = distribution(generator);
		_r.push_back(temp);
	}
}

int cosineH::calculateValue(vector<int> *_p){ //calculate value given a point
	double tempd = 0;
	vector< double >::iterator vecR_it = _r.begin();
	for(vector< int >::iterator vecP_it = _p->begin(); vecP_it != _p->end(); ++vecP_it, ++vecR_it)
		tempd += (*vecR_it)*(*vecP_it);
	if(tempd >= 0){
		return 1;
	}else{
		return 0;
	}
}

cosineFamily::cosineFamily(int K){
	for(int i = 0; i < K; i++){
		cosineH *temp = new cosineH();
		hfam.push_back((*temp));
	}
}

void cosineFamily::getValuesOfH(vector<int> *_p, vector<int> *hvalues){
	for(vector<cosineH>::iterator hfam_it = hfam.begin(); hfam_it != hfam.end(); ++hfam_it)
		hvalues->push_back( (*hfam_it).calculateValue(_p) );
}

int cosineFamily::getMapSize(int size){ //Needed for other metrics
	int temp = 1;
	for(int i = 0; i < hfam.size(); ++i)
		temp = temp*2;
	return temp;
}

double cosineFamily::getDistance(vector< int > *_q, vector< int > *_p){
	//Cosine Distance
	long double temp1 = 0, temp2 = 0, temp3 = 0;
	if( _q->size() != _p->size() );
		//TODO problem!
	vector<int>::iterator p_it = _p->begin();
	for( vector<int>::iterator q_it = _q->begin(); q_it != _q->end(); ++q_it, ++p_it){
		temp1 += (*q_it)*(*p_it);
		temp2 += (*q_it)*(*q_it);
		temp3 += (*p_it)*(*p_it);
	}
	temp2 = sqrt ( temp2 );
	temp3 = sqrt ( temp3 );
	temp2 = temp2*temp3;
	temp1 = temp1 / temp2;
	temp1 = 1 - temp1;
	return (double)temp1;
}
