#include "HyperCube.hpp"

HyperCube::HyperCube(string m, vector<myVector> *trainSet, int tm, int tp){
	M = tm;
	probes = tp;
	metric = m;
	double res = log2(trainSet->size());
	int ress = res;
	
	gfunction = new Gfunction(m, ress);
	
	mapSize = 1;
	for(int i=0; i < ress; i++)
		mapSize = mapSize*2;
	
	buckets = new HyperNode*[mapSize]();
	hList = new HyperH(0,0);
	
	setHyperCube(trainSet);
}

int HyperCube::euclideanHashFunction(myVector *valueP){
	vector<int> hvalues;
	gfunction->getValuesOfH(&(valueP->coordinates), &hvalues);

	int hvalue = 0, i = 1;
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

void HyperCube::setHyperCube(vector<myVector> *trainSet){
	for(vector< myVector >::iterator l_it = trainSet->begin(); l_it != trainSet->end(); ++l_it)
			putVector(&(*l_it));
}

void HyperCube::putVector(myVector *valueP){
	/* Calculate HashValue*/
	int hashValue = calculateHashValue(valueP);

	/* Get to our bucket */
	HyperNode *tempPrev = NULL;
	HyperNode *tempEntry = buckets[hashValue];

	/* Get to the last item in the bucket */
	while(tempEntry != NULL){
		tempPrev = tempEntry;
		tempEntry = tempEntry->getNext();
	}

	/* Place our item*/
	if( tempEntry == NULL ){
		tempEntry = new HyperNode(valueP);
		if( tempPrev == NULL ) { // Cell was empty
			buckets[hashValue] = tempEntry;
		} else {		 		 // Cell wasn't empty
			tempPrev->setNext(tempEntry);
		}
	} else {
		cout << "Undefined Error" << endl;
	}
}
void HyperCube::approxNNSearch(myVector *query, myVector **nnTemp, double *mindist){
	/* Calculate HashValue*/
	int hashValue = calculateHashValue(query);

	HyperNode *tempEntry = buckets[hashValue];
	double dist = 0;
	int itemCount = 0;
	int probeCount = 0;
	do{
		while(tempEntry!= NULL){
			//approx NN Search	// for vectors q and p find distance (for this g function with these h values)
			dist = gfunction->getDistance(&(query->coordinates), &(tempEntry->getValue()->coordinates) );
			if( dist < (*mindist) ){
				(*mindist) = dist;
				(*nnTemp) = tempEntry->getValue();
			}
			//Parse Bucket
			itemCount++;
			if(itemCount >= M)
				break;
			tempEntry = tempEntry->getNext();
		}
		probeCount++;
		if(itemCount >= M)
			break;
		if(probeCount >= probes)
			break;
		if(hashValue%2==0)
			hashValue++; //Guaranteed to have a hamming distance of 1
		else
			hashValue--; //Guaranteed to have a hamming distance of 1
		tempEntry = buckets[hashValue];
	}while(1);
		
}

void HyperCube::rangeSearch(myVector* query, double *R, vector<myVector *> *rsTemp){
	/* Calculate HashValue*/
	int hashValue = calculateHashValue(query);

	HyperNode *tempEntry = buckets[hashValue];
	int itemCount = 0;
	int probeCount = 0;
	do{
		while(tempEntry!= NULL){
			double dist = 0;
			//Range Search
			dist = gfunction->getDistance(&(query->coordinates),  &(tempEntry->getValue()->coordinates) );
			if( dist < (*R))
				rsTemp->push_back( (tempEntry->getValue()) );
			//Parse Bucket
			itemCount++;
			if(itemCount >= M)
				break;
			tempEntry = tempEntry->getNext();
		}
		probeCount++;
		if(itemCount >= M)
			break;
		if(probeCount >= probes)
			break;
		if(hashValue%2==0)
			hashValue++; //Guaranteed to have a hamming distance of 1
		else
			hashValue--; //Guaranteed to have a hamming distance of 1
		tempEntry = buckets[hashValue];
	}while(1);
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

void HyperCube::printNstuff(){
	for(int i = 0; i < mapSize; i++){
		HyperNode *tempEntry = buckets[i];
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
