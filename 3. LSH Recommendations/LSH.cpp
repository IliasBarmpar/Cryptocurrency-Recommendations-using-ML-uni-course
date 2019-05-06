#include "LSH.hpp"

//		Basics  	// 
HashTable::HashTable(string m, int size, int K){
	metric = m;
	gfunction = new Gfunction(m, K);
	mapSize = gfunction->getMapSize(size);
	map = new HashNode*[mapSize]();
}

HashTable::~HashTable(){}

LSH::LSH(vector<myVector> *trainSet, string m, int Lnum, int knum){
	metric = m;
	HashTable *htable;
	for(int j = 0; j < Lnum; ++j){  // for each hashtable
		htable = new HashTable(metric, trainSet->size(), knum);
		for(vector< myVector >::iterator trS_it = trainSet->begin(); trS_it != trainSet->end(); ++trS_it)
			htable->putVector(&(*trS_it));
		hashtables.push_back((*htable));
		//htable->printNstuff();
		//char ch = getchar();
		htable = NULL;
	}
}

void LSH::addTable(HashTable *ht){
	hashtables.push_back((*ht));
}

//		Hash Functions  	// 
int HashTable::euclideanHashFunction(myVector *valueP){
	vector<int> hvalues;
	gfunction->getValuesOfH(&(valueP->coordinates), &hvalues);

	int hvalue = 0, temp = 0;
	vector<int>::iterator rfam_it = _rfam->_rfamily.begin(); //TODO getter or smh
	for(vector<int>::iterator hfam_it = hvalues.begin(); hfam_it != hvalues.end(); ++hfam_it, ++rfam_it){
		temp  = (*rfam_it)*( (*hfam_it) );
		hvalue += ((temp % mapSize) + mapSize) % mapSize;
	}//TODO Some overflow prevention
	hvalue = ((hvalue % mapSize) + mapSize) % mapSize;
	return hvalue;
}

int HashTable::cosineHashFunction(myVector *valueP){
	int hvalue = 0;
	int i = 1;
	vector<int> hvalues;
	gfunction->getValuesOfH(&(valueP->coordinates), &hvalues);

	for(vector<int>::iterator hfam_it = hvalues.begin(); hfam_it != hvalues.end(); ++hfam_it){
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


//		Put Vector  	//
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
//		ApproxNN Search  	//
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

void LSH::approxNNSearch(myVector *query, myVector **nnTemp, double *mindist ){
	for(vector<HashTable>::iterator ht_it = hashtables.begin(); ht_it != hashtables.end(); ++ht_it)
		(ht_it)->approxNNSearch(query, nnTemp, mindist);
}

//		Range Search  	  //
void HashTable::rangeSearch(myVector *query, double R, vector<myVector *> *rsTemp){
	/* Calculate HashValue*/
	int hashValue = calculateHashValue(query);

	HashNode *tempEntry = map[hashValue];
	while(tempEntry!= NULL){
		double dist = 0;
		//Range Search
		dist = gfunction->getDistance(&(query->coordinates),  &(tempEntry->getValue()->coordinates) );

		if( dist < R ){
			int flag = 1;
			for(vector<myVector *>::iterator it_it = rsTemp->begin(); it_it != rsTemp->end(); ++it_it)
				if((*it_it)->id== (tempEntry->getValue())->id)
					flag = 0;
			if(flag)
				rsTemp->push_back( (tempEntry->getValue()) );
		}
		//Parse Bucket
		tempEntry = tempEntry->getNext();
	}
}

void LSH::rangeSearch(myVector* query, double rvariable, vector<myVector *> *rsTemp){
	for(vector<HashTable>::iterator ht_it = hashtables.begin(); ht_it != hashtables.end(); ++ht_it){
		(ht_it)->rangeSearch(query,rvariable, rsTemp);
	}
}


//		Range Search  	  //
void HashTable::rangeSearch2(myVector *query, vector<myVector *> *rsTemp){
	/* Calculate HashValue*/
	int hashValue = calculateHashValue(query);

	HashNode *tempEntry = map[hashValue];
	while(tempEntry!= NULL){ //Range Search
		if(query->id != (tempEntry->getValue())->id){
			int flag = 1;

			//Calculate Distance
			double dist = gfunction->getDistance(&(query->coordinates),  &(tempEntry->getValue()->coordinates) );
//			cout << "dist: " << dist << endl;
			//Check whether we already have this vector
 			for(vector<myVector *>::iterator it_it = rsTemp->begin(); it_it != rsTemp->end(); ++it_it)
				if((*it_it)->id == (tempEntry->getValue())->id)
					flag = 0;

 			// If not add it
			if(flag){
				(tempEntry->getValue())->similarity = 1 - dist;
				rsTemp->push_back( (tempEntry->getValue()) );
			}
		}

		//Parse Bucket
		tempEntry = tempEntry->getNext();
	}
}

void LSH::rangeSearch2(myVector* query, vector<myVector *> *rsTemp){
	for(vector<HashTable>::iterator ht_it = hashtables.begin(); ht_it != hashtables.end(); ++ht_it)
		(ht_it)->rangeSearch2(query, rsTemp);

}

























//		Prints		//
void LSH::printTemp(){

 	for(vector<HashTable>::iterator ht_it = hashtables.begin(); ht_it != hashtables.end(); ++ht_it){
 		cout << "____________________________________________________" << endl;
		(*ht_it).printNstuff();
		cout << "____________________________________________________" << endl;
 	}
}

void HashTable::printNstuff(){
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

void LSH::printTemp2(){
	for(vector<HashTable>::iterator ht_it = hashtables.begin(); ht_it != hashtables.end(); ++ht_it){
		cout << "____________________________________________________" << endl;
		(*ht_it).printNstuff2();
		cout << "____________________________________________________" << endl;
	}
}

void HashTable::printNstuff2(){
	for(int i = 0; i < mapSize; i++){
		HashNode *tempEntry = map[i];
		if(tempEntry==NULL){
			cout << i << "=e"<<endl;
		}else{
			cout << i << ")" << (tempEntry->getValue())->id << "|";
			tempEntry = tempEntry->getNext();
			while(tempEntry != NULL){
				cout << (tempEntry->getValue())->id << "|";
				tempEntry = tempEntry->getNext();
			}
			cout << endl;
		}
	}
}
