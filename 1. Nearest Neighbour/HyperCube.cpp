#include "HyperCube.hpp"

HyperCube::HyperCube(string m, int entrySize, int K){
	mapSize = 8;
	gfunction = new Gfunction(m, K);
	buckets = new HyperNode*[mapSize]();
	hList = new HyperH(0,0);
	metric = m;
}

int HyperCube::euclideanHashFunction(myVector *valueP){
	vector<int> hvalues;
	gfunction->getValuesOfH(&(valueP->coordinates), &hvalues);
	int hvalue = 0;
	int i = 1;
	for(vector<int>::iterator hfam_it = hvalues.begin(); hfam_it != hvalues.end(); ++hfam_it){
		int tempint;
		HyperH *tempPrev = NULL;
		HyperH *temp = hList;
		int flag = 1;
		while(temp!=NULL){
			if(temp->getKey() == (*hfam_it)){
				tempint = temp->getValue();
				flag = 0;
				break;
			}
			tempPrev = temp;
			temp = temp->getNext();
		}
		if(flag==1){
			tempint = rand();
			tempint = ((tempint % 2) + 2 )% 2;
			temp = new HyperH((*hfam_it), tempint);
			tempPrev->setNext(temp);
		}
		hvalue += i*tempint;
		i = i*2;
	}
	return hvalue;
}

int HyperCube::cosineHashFunction(myVector *valueP){
	int hvalue = 0;
	int i = 1;
	vector<int> hvalues;
	gfunction->getValuesOfH(&(valueP->coordinates), &hvalues);
	for(vector<int>::iterator hfam_it = hvalues.begin(); hfam_it != hvalues.end(); ++hfam_it){
		//end to begin doesnt work...
		hvalue += i*(*hfam_it);
		i = i*2;
	}
	return hvalue;
}

int HyperCube::calculateHashValue(myVector *valueP){
	int hashValue = 0;
	if(metric=="euclidean"){  //QUESTION: if(hi_lef == diff(#0,#1))
		hashValue = euclideanHashFunction(valueP);
	}else if(metric=="cosine"){
		hashValue = cosineHashFunction(valueP);
	}else{
		//error?
	}
	return hashValue;
}

void HyperCube::putVector(myVector *valueP){
	/* Calculate HashValue*/
	int hashValue = calculateHashValue(valueP);

	HyperNode *tempPrev = NULL;
	HyperNode *tempEntry = buckets[hashValue];
	while(tempEntry != NULL){ //if( tempEntry->getKey() != keyG ) break;
		tempPrev = tempEntry;
		tempEntry = tempEntry->getNext();
	}
	if( tempEntry == NULL ){
		tempEntry = new HyperNode(valueP);
		if( tempPrev == NULL ) { // Cell was empty
			buckets[hashValue] = tempEntry;
		} else {		 // Cell wasn't empty
			tempPrev->setNext(tempEntry);
		}
	} else {
		//tempEntry->setValue(valueP);
	}
}

void HyperCube::approxNNSearch(myVector *query, myVector **nnTemp, double *mindist){
	/* Calculate HashValue*/
	int hashValue = calculateHashValue(query);

	HyperNode *tempEntry = buckets[hashValue];

	double dist = 0;
	while(tempEntry!= NULL){
		//approx NN Search
		dist = gfunction->getDistance(&(query->coordinates),  &(tempEntry->getValue()->coordinates) );
		if( dist < (*mindist) ){
			(*mindist) = dist;
			(*nnTemp) = tempEntry->getValue();
		}
		//Parse Bucket
		tempEntry = tempEntry->getNext();
	}
}

void HyperCube::rangeSearch(myVector* query, double *R, list<myVector *> *rsTemp){
	/* Calculate HashValue*/
	int hashValue = calculateHashValue(query);

	HyperNode *tempEntry = buckets[hashValue];

	while(tempEntry!= NULL){
		double dist = 0;
		//Range Search
		dist = gfunction->getDistance(&(query->coordinates),  &(tempEntry->getValue()->coordinates) );
		if( dist < (*R))
			rsTemp->push_back( (tempEntry->getValue()) );
		//Parse Bucket
		tempEntry = tempEntry->getNext();
	}
}

void HyperCube::printHyperH(){
	cout << "printHyperH()"<< endl;
	HyperH *tem = hList;
	int i = 1;
	while(tem!=NULL){
		cout << i << ". " << tem->getKey() << "|" << tem->getValue() << endl;
		tem = tem->getNext();
	}
}
