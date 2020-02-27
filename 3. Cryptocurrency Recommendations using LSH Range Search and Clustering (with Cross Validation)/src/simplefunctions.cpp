#include "simplefunctions.hpp"

void printUserCrypto(vector<myVector> *trainSet, int size){
	vector<myVector>::iterator v = trainSet->begin();
	for(int i = 0; i < size; ++v, i++){
		cout << (*v).id << ' ';
		for(vector<double>::iterator d = (*v).coordinates.begin(); d != (*v).coordinates.end(); ++d){
			cout << *d << ' ';
		}
		cout << endl;
	}
}

void functionVec( vector<myVector> *trainSet, int size, int k){
	myVector tempvec;
	tempvec.id = k;
	for(int i = 0; i < size; ++i)
		tempvec.coordinates.push_back(0.0);
	trainSet->push_back(tempvec);
}

void setVectors(vector<myVector> *trainSet, int sizei, int sizej){
	for(int i = 0; i < sizei; ++i)
		functionVec(trainSet, sizej, i);
}

bool searchInLexiconA(string word, vector<myLexiconAMember> *lexiconA, double *score){
	for(vector<myLexiconAMember>::iterator l_it = lexiconA->begin(); l_it != lexiconA->end(); ++l_it ){
		if(word == (*l_it).word){
			*score = (*l_it).sentscore;
			return 1;
		}
	}
	*score = 0.0;
	return 0;
}

bool searchInLexiconK(string word, vector<myLexiconKMember> *lexiconK, int *pos){
	int i = 0;
	for(vector<myLexiconKMember>::iterator k_it = lexiconK->begin(); k_it != lexiconK->end(); ++k_it, ++i ){
		for(vector<string>::iterator str_it = (*k_it).words.begin(); str_it != (*k_it).words.end(); ++str_it){
			if(word == (*str_it)){
				*pos = i;
				return 1;
			}
		}
	}
	*pos = -1;
	return 0;
}

bool checkIfZeroVector(double *vector, int size){
	for(int i = 0; i < size; i++)
		if(vector[i]!=0)
			return 0;
	return 1;
}

bool compareByDist(const myVector * a, const myVector * b){
	return a->similarity > b->similarity;
}

int findInTweets(vector<myTweet> *tweets, int pos){
	int i = 0, flag = 0;
	for(vector<myTweet>::iterator tw_it = tweets->begin(); tw_it != tweets->end(); ++tw_it, ++i){
		if( (*tw_it).id == pos )
			return i;
	}
	if(flag){
		cout << "Couldn't find such id in tweets_dataset." << endl;
		return -1;
	}
}

void shuffleArray(int* array,int size) {
  int n = size;
  while (n > 1){
    // 0 <= k < n.
    int k = rand()%n;

    // n is now the last pertinent index;
    n--;

    // swap array[n] with array[k]
    int temp = array[n];
    array[n] = array[k];
    array[k] = temp;
  }
}

int* kfold( int size, int k ){
  int* indices = new int[ size ];

  for (int i = 0; i < size; i++ )
    indices[ i ] = i%k;

  shuffleArray( indices, size );

  return indices;
}

void setUserCryptLoc(vector<uc> *ucloc, vector<vector<bool>> *userCryptoReferences){
	int ii = 0, count = 0;
	for(vector<vector<bool>>::iterator bool_it = userCryptoReferences->begin(); bool_it != userCryptoReferences->end(); ++bool_it, ++ii){
		int c = 0;
		for(vector<bool>::iterator b_it = (*bool_it).begin(); b_it != (*bool_it).end(); ++b_it, ++c){
			if((*b_it)){
				count++;
				uc temp;
				temp.user	= ii;
				temp.crypto	= c;
				ucloc->push_back(temp);
			}
		}
	}
}



