#include "LSH.hpp"

HashTable::HashTable(string m, int size, int K){
	metric = m;
	gfunction = new Gfunction(m, K);
	mapSize = gfunction->getMapSize(size);
	map = new HashNode*[mapSize]();
}

HashTable::~HashTable(){}

int HashTable::euclideanHashFunction(myVector *valueP){
	vector<int> hvalues;
	gfunction->getValuesOfH(&(valueP->coordinates), &hvalues);
	int hvalue = 0, temp = 0;
	vector<int>::iterator rfam_it = _rfam->_rfamily.begin(); //TODO getter or smh
	for(vector<int>::iterator hfam_it = hvalues.begin(); hfam_it != hvalues.end(); ++hfam_it, ++rfam_it){
		temp  = (*rfam_it)*( (*hfam_it) );
		hvalue += ((temp % mapSize) + mapSize) % mapSize;
	} //TODO Some overflow prevention
	hvalue = ((hvalue % mapSize) + mapSize) % mapSize;
	return hvalue;
}

int HashTable::cosineHashFunction(myVector *valueP){
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

int HashTable::calculateHashValue(myVector *valueP){
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

void HashTable::putVector(myVector *valueP){
	/* Calculate HashValue*/
	int hashValue = calculateHashValue(valueP);

	HashNode *tempPrev = NULL;
	HashNode *tempEntry = map[hashValue];

	while(tempEntry != NULL){
		tempPrev = tempEntry;
		tempEntry = tempEntry->getNext();
	}

	if( tempEntry == NULL ){
		tempEntry = new HashNode(valueP);
		if( tempPrev == NULL ) { // Cell was empty
			map[hashValue] = tempEntry;
		} else {		 // Cell wasn't empty
			tempPrev->setNext(tempEntry);
		}
	} else {
		//error
	}
}

void HashTable::approxNNSearch(myVector *query,  myVector **nnTemp, double *mindist){
	/* Calculate HashValue*/
	int hashValue = calculateHashValue(query);

	HashNode *tempEntry = map[hashValue];
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

void HashTable::rangeSearch(myVector *query, double *R, list<myVector *> *rsTemp){
	/* Calculate HashValue*/
	int hashValue = calculateHashValue(query);

	HashNode *tempEntry = map[hashValue];

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

void HashTable::printer(){
	for(int i = 0; i < mapSize; i++){
		HashNode *tempEntry = map[i];
		if(tempEntry==NULL){
			cout << i << "=e"<<endl;
		}else{
			cout << i << "|><|";
			tempEntry = tempEntry->getNext();
			while(tempEntry != NULL){
				cout << "><|";
				tempEntry = tempEntry->getNext();
			}
			cout << endl;
		}
	}
}

void LSH::addTable(HashTable *ht){
	hashtables.push_back((*ht));
}

void LSH::rangeSearch(myVector* query, double * rvariable, list<myVector *> *rsTemp){
	for(vector<HashTable>::iterator ht_it = hashtables.begin(); ht_it != hashtables.end(); ++ht_it)
		(ht_it)->rangeSearch(query,rvariable, rsTemp);
}

void LSH::approxNNSearch(myVector *query, myVector **nnTemp, double *mindist ){
	for(vector<HashTable>::iterator ht_it = hashtables.begin(); ht_it != hashtables.end(); ++ht_it)
		(ht_it)->approxNNSearch(query, nnTemp, mindist);
}
