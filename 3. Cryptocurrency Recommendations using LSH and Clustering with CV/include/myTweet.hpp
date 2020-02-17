#ifndef myT_H
#define myT_H

#include <iostream>
#include <vector>
#include "myVector.hpp"

using namespace std;

struct myTweet{
	int userid;
	int id;
	vector<string> tweet;
	double sentscore;

	~myTweet(){
		tweet.clear();
		vector<string>().swap(tweet);
	}
};

struct myUser{
	int id;
	vector<int> tweetIDs;
	double meanRating;

	~myUser(){
		tweetIDs.clear();
		vector<int>().swap(tweetIDs);
	}
};

#endif
/*

#include <vector>
#include <math.h>
#include <algorithm>
#include <random>
#include <chrono>

#include "myTweet.hpp"
#include "myLexicon.hpp"
#include "readFileInput.hpp"
#include "readCmdRec.hpp"
#include "readLexicon.hpp"
#include "LSH.hpp"
#include "kmeans.hpp"
#include "readFileCsv.hpp"

static int clusterCount = 50;

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
// int k = 0;
//	for(vector<myUser>::iterator u_it = users->begin(); u_it != users->end(); ++u_it, ++k){
	for(int i = 0; i < sizei; ++i)
		functionVec(trainSet, sizej, i);
}

bool searchInLexiconA(string word, vector<myLexiconAMember> *lexiconA, double *score){
	for(vector<myLexiconAMember>::iterator l_it = lexiconA->begin(); l_it != lexiconA->end(); ++l_it ){
		if(word == (*l_it).word){
//			cout << ">" << l_it->word << ":" << l_it->sentscore << endl;
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
//				cout << word << "-'" << i << "'" << ' ';
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

struct myKD{
	int loc;
	double distance;
};


int main(int argc, char* argv[]){
	srand(time(nullptr));
	string inputfile2 = "twitter_dataset_small_v2.csv", inputfile, lexiconAfile = "vader_lexicon.csv", lexiconKfile = "coins_queries.csv";
	string outputfile;
	vector<myUser> users;
	vector<myTweet> tweets;
	vector<myLexiconAMember> lexiconA;
	vector<myLexiconKMember> lexiconK;
	vector<myVector> trainSet, trainSet2, trainSetPrj2;
	int P = 5, a = 15;
	bool val;

	if( readCmdRec(argc, argv, &inputfile, &outputfile, &val) ) return -1;

	if( readInputFile(inputfile, &users, &tweets) ) return -1;

	if( readLexiconA(lexiconAfile, &lexiconA) ) return -1;

	if( readLexiconK(lexiconKfile, &lexiconK) ) return -1;

	if( readFileCsv(inputfile2, &trainSetPrj2) ) return -1;

	setVectors(&trainSet, users.size(), lexiconK.size());

	setVectors(&trainSet2, clusterCount, lexiconK.size());

	myKmeans kMeansForPrj2( &trainSetPrj2, "euclidean", clusterCount, trainSetPrj2[0].coordinates.size() );
	kMeansForPrj2.runKmeans(1, 1, 1);
	vector<int> *clAssignPrj2 = kMeansForPrj2.getClustAssigned();

	/////////////////////////////////
	// Sentiment Score calculation //
	/////////////////////////////////
	vector<myTweet>::iterator id_it = tweets.begin();
	for(vector<myUser>::iterator u_it = users.begin(); u_it != users.end(); ++u_it){
		for(vector<myTweet>::iterator tw_it = (*u_it).tweets.begin(); tw_it != (*u_it).tweets.end(); ++tw_it, ++id_it){
			double totalscore = 0.0;
			for(vector<string>::iterator wo_it = (*tw_it).tweet.begin(); wo_it != (*tw_it).tweet.end(); ++wo_it){
				double score = 0.0;
				searchInLexiconA(*wo_it, &lexiconA, &score);
				totalscore += score;
			}
			totalscore = totalscore / sqrt(totalscore*totalscore + a);
			(*tw_it).sentscore = totalscore;
			(*id_it).sentscore = totalscore;
		}
	}

	/////////////////////////////////////////
	// User-Cryptocurrency Vector creation //
	/////////////////////////////////////////
	vector<vector<bool>> abc(users.size(),vector<bool>(lexiconK.size()));
	for(int j = 0; j < users.size(); ++j) for(int k = 0; k < lexiconK.size(); ++k) abc[j][k] = 0;

	int i = 0, pos = -1;
	vector<myUser>::iterator u_it = users.begin();
	for(vector<myVector>::iterator tS_it = trainSet.begin(); tS_it != trainSet.end(); ++u_it, ++tS_it, ++i){
		for(vector<myTweet>::iterator tw_it = (*u_it).tweets.begin(); tw_it != (*u_it).tweets.end(); ++tw_it){
			//TODO me advance wste na einai sumvato kai me ta duo

			bool tempbool[lexiconK.size()] = {0};
 			for(vector<string>::iterator wo_it = (*tw_it).tweet.begin(); wo_it != (*tw_it).tweet.end(); ++wo_it){
				if(searchInLexiconK((*wo_it), &lexiconK, &pos)){
					if(!tempbool[pos]){
						(*tS_it).coordinates[pos] += (*tw_it).sentscore;
						abc[i][pos]   = 1;
						tempbool[pos] = 1;
					}
				}
 			}
		}
	}



	/////////////////////////////////
	// Remove users with 0 vectors //
	/////////////////////////////////
	u_it = users.begin();
	vector<vector<bool>>::iterator bool_it = abc.begin();
	for(vector<myVector>::iterator tS_it = trainSet.begin(); tS_it != trainSet.end(); ){
		int flag = 1;
		for(vector<double>::iterator d_it = (*tS_it).coordinates.begin(); d_it != (*tS_it).coordinates.end(); ++d_it){
			if((*d_it) != 0.0){
				flag = 0;
				break;
			}
		}
		if(flag){
			tS_it	= trainSet.erase(tS_it);
			u_it	= users.erase(u_it);
			bool_it = abc.erase(bool_it);
		}else{
			++u_it;
			++tS_it;
			++bool_it;
		}
	}

	/////////////////
	// Change ID's //
	/////////////////
	int l = 1;
	for(vector<myVector>::iterator tS_it = trainSet.begin(); tS_it != trainSet.end(); ++tS_it, ++l)
		tS_it->id = l;




	/////////////////////////////////////////
	// Calculate rating mean for each user //
	/////////////////////////////////////////
	u_it = users.begin();
	for( vector<myVector>::iterator tS_it = trainSet.begin(); tS_it != trainSet.end(); ++u_it, ++tS_it){
		double sum = 0.0; int counter = 0, c = 0;
		for(vector<double>::iterator d_it = (*tS_it).coordinates.begin(); d_it != (*tS_it).coordinates.end(); ++d_it, ++c){ //Parse User-Crypto Vector
			if(abc[(*tS_it).id-1][c]){
				sum += (*d_it);
				counter++;
			}
		}
		(*u_it).meanRating = sum/counter;
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	////////////////////////////////////////////
	// Cluster-Cryptocurrency Vector creation //
	////////////////////////////////////////////
	vector<vector<bool>> cba( clusterCount, vector<bool>(lexiconK.size()) );
	for(int j = 0; j < clusterCount; ++j) for(int k = 0; k < lexiconK.size(); ++k) cba[j][k] = 0;

	int i = 0, pos = -1;
	for(vector<myVector>::iterator tS_it = trainSet2.begin(); tS_it != trainSet2.end(); ++tS_it, ++i){ //for every user
		vector<int> clusterPositions;
		int ii = 0;
		for(vector<int>::iterator c_it = clAssignPrj2->begin(); c_it != clAssignPrj2->end(); c_it++, ii++){ //find tweets
			if( *c_it== i ){
				clusterPositions.push_back( findInTweets(&tweets, ii+1) );
			}
		}

		ii = 0;
		for(vector<int>::iterator cl_it = clusterPositions.begin(); cl_it != clusterPositions.end(); cl_it++, ii++){ //for every tweet
			bool tempbool[lexiconK.size()] = {0};
			for(vector<string>::iterator wo_it = tweets[*cl_it].tweet.begin(); wo_it != tweets[*cl_it].tweet.end(); ++wo_it){ //for every word
				if(searchInLexiconK((*wo_it), &lexiconK, &pos)){ //search in lexicon K
					if(!tempbool[pos]){
						(*tS_it).coordinates[pos] += tweets[*cl_it].sentscore;
						cba[i][pos]   = 1;

						tempbool[pos] = 1;
					}
				}
			}
		}
	}


	///////////////////////////////////////
	// Remove pseudousers with 0 vectors //
	///////////////////////////////////////
	vector<vector<bool>>::iterator bool_it = cba.begin();
	for(vector<myVector>::iterator tS_it = trainSet2.begin(); tS_it != trainSet2.end();){
		int flag = 1;
		for(vector<double>::iterator d_it = (*tS_it).coordinates.begin(); d_it != (*tS_it).coordinates.end(); ++d_it){
			if((*d_it) != 0.0){
				flag = 0;
				break;
			}
		}
		if(flag){
			tS_it	= trainSet2.erase(tS_it);
			bool_it = cba.erase(bool_it);
		}else{
			++tS_it;
			++bool_it;
		}
	}

	///////////////////////
	// Change pseudoID's //
	///////////////////////
	int l = 1;
	for(vector<myVector>::iterator tS_it = trainSet2.begin(); tS_it != trainSet2.end(); ++tS_it, ++l)
		tS_it->id = l;


	/////////////////////////////////////////
	// Calculate rating mean for each user //
	/////////////////////////////////////////
	double arrayy[trainSet2.size()] = {0.0}; int b = 0;
	for(vector<myVector>::iterator tS_it = trainSet2.begin(); tS_it != trainSet2.end(); ++tS_it, ++b){ //Parse Users
		double sum = 0.0; int counter = 0, c = 0;
		for(vector<double>::iterator d_it = (*tS_it).coordinates.begin(); d_it != (*tS_it).coordinates.end(); ++d_it, ++c){ //Parse User-Crypto Vector
			if(cba[(*tS_it).id-1][c]){
				sum += (*d_it);
				counter++;
			}
		}
		arrayy[b] = sum/counter;
	}











































	{

	//////////////////////
	// pseudoCLUSTERING //
	//////////////////////
	cout << "Clustering" << endl;
	myKmeans kMeans (&trainSet2, "euclidean", 5, trainSet2[0].coordinates.size());
	kMeans.runKmeans(2,1,1);
	vector<int> *clAssign = kMeans.getClustAssigned();

	i = 0;
	for(vector<myVector>::iterator tS_it = trainSet2.begin(); tS_it != trainSet2.end(); ++tS_it, ++i){
		vector<myVector *> rsTemp;

		//Find Neighbors, no sort necessary
		for(vector<myVector>::iterator tS2_it = trainSet2.begin(); tS2_it != trainSet2.end(); ++tS2_it)
			if( (*clAssign)[tS_it->id-1] == (*clAssign)[tS2_it->id-1])
				if( tS_it->id != tS2_it->id )
					rsTemp.push_back( &(*tS2_it) );

		//Find distance
		for(vector<myVector *>::iterator tt_it = rsTemp.begin(); tt_it != rsTemp.end(); ++tt_it)
			(*tt_it)->similarity =   euclideanDistOfTwoVectors(&((*tS_it).coordinates) ,&((*tt_it)->coordinates));

		//Fill in vectors
		int c = 0;
		for(vector<double>::iterator d_it = (*tS_it).coordinates.begin(); d_it != (*tS_it).coordinates.end(); ++d_it, ++c){
			double result = 0.0;
			if(!cba[i][c]){
				//P Neighbors
				double suma = 0.0, sumb = 0.0;
				if(rsTemp.size() != 0){
					for(vector<myVector *>::iterator tt_it = rsTemp.begin(); tt_it != rsTemp.end(); ++tt_it){
						if((*tt_it)->similarity < 0)
							suma += (-1)*(*tt_it)->similarity;
						else
							suma += (*tt_it)->similarity;
						sumb += (*tt_it)->similarity*( (*tt_it)->coordinates[c] - arrayy[ (*tt_it)->id - 1 ] );
					}
					result = (1/suma)*sumb;
				}
			}else{
				result = (*d_it) - arrayy[i];
			}
			(*d_it) = result;
		}

		//Recommend
		cout << tS_it->id << ' ';
		for(int i = 0; i < 5; i++){
			int pos = -1, c = 0;
			double max = numeric_limits<double>::lowest();
			for(vector<double>::iterator d_it = (*tS_it).coordinates.begin(); d_it != (*tS_it).coordinates.end(); ++d_it, ++c){
				if(!cba[i][c]){
					//Find max
					if ((*d_it) > max){
						max = (*d_it);
						pos = c;
					}
				}//else{} //Skip
			}
			if(pos==-1){
				break;
			}else{
				cba[i][pos] = 1;
				if(lexiconK[pos].words.size()>4)
					cout << lexiconK[pos].words[4] << " ";
				else
					cout << lexiconK[pos].words[0] << " ";
			}
		}
		cout << endl;
	}
	return 0;


	/////////////////////////////////////
	// pseudo Cosine LSHRecommendation //
	/////////////////////////////////////
	cout << "Cosine LSH" << endl;
	LSH lsh(&trainSet2, "cosine", 4, 4);
	i = 0;
	for(vector<myVector>::iterator tS_it = trainSet2.begin(); tS_it != trainSet2.end(); ++tS_it, ++i){
		P = 5;
		vector<myVector *> rsTemp;
		//Find Neighbors
		lsh.rangeSearch2( &(*tS_it), &rsTemp);

		//Sort
		sort(rsTemp.begin(), rsTemp.end(), compareByDist);

		//Fill in vectors
		int c = 0;
		for(vector<double>::iterator d_it = (*tS_it).coordinates.begin(); d_it != (*tS_it).coordinates.end(); ++d_it, ++c){
			double result = 0.0;
			if(rsTemp.size() < P)
				P = rsTemp.size();
			if(!cba[i][c]){
				//P Neighbors
				double suma = 0.0, sumb = 0.0;
				vector<myVector *>::iterator tt_it = rsTemp.begin();
				for(int jj = 0; jj < P; jj++, tt_it++){
					if((*tt_it)->similarity < 0)
						suma += (-1)*(*tt_it)->similarity;
					else
						suma += (*tt_it)->similarity;
					sumb += (*tt_it)->similarity*( (*tt_it)->coordinates[c] - users[(*tt_it)->id - 1 ].meanRating);
				}
				result += (1/suma)*sumb;
			}else{
				result = (*d_it) - arrayy[i];
			}
			(*d_it) = result;
		}

		//Recommend
		cout << i << ' ';
		for(int ii = 0; ii < 5; ii++){
			int pos = -1, c = 0;
			double max = numeric_limits<double>::lowest();
			for(vector<double>::iterator d_it = (*tS_it).coordinates.begin(); d_it != (*tS_it).coordinates.end(); ++d_it, ++c){
				if(!cba[i][c]){
					//find max
					if ((*d_it) > max){
						max = (*d_it);
						pos = c;
					}
				}else{} //skip
			}
			if(pos==-1){
				cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
				break;
			}else{
				cba[i][pos] = 1;
				if(lexiconK[pos].words.size()>4)
					cout << lexiconK[pos].words[4] << " ";
				else
					cout << lexiconK[pos].words[0] << " ";
			}
		}
		cout << endl << endl;
	}

	}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





	////////////////
	// CLUSTERING //
	////////////////
	cout << "Clustering" << endl;
	myKmeans kMeans (&trainSet, "euclidean", clusterCount, trainSet[0].coordinates.size());
	kMeans.runKmeans(2,1,1);
	vector<int> *clAssign = kMeans.getClustAssigned();
//	int ij = 0;
//	for(vector<int>::iterator c_it = clAssign->begin(); c_it != clAssign->end(); c_it++, ij++)
//		cout << ij << ": " << *c_it << endl;

	i = 0;
	vector<myUser>::iterator u1_it = users.begin();
	for(vector<myVector>::iterator tS_it = trainSet.begin(); tS_it != trainSet.end(); ++tS_it, ++u1_it, ++i){
//		cout << (*tS_it).id << ' ' << (*clAssign)[tS_it->id-1] << endl << endl;
		vector<myVector *> rsTemp;

		//Find Neighbors, no sort necessary
		for(vector<myVector>::iterator tS2_it = trainSet.begin(); tS2_it != trainSet.end(); ++tS2_it)
			if( (*clAssign)[tS_it->id-1] == (*clAssign)[tS2_it->id-1] )
				if( tS_it->id!=tS2_it->id )
					rsTemp.push_back( &(*tS2_it) );

		//Find distance
		for(vector<myVector *>::iterator tt_it = rsTemp.begin(); tt_it != rsTemp.end(); ++tt_it)
			(*tt_it)->similarity =   euclideanDistOfTwoVectors(&((*tS_it).coordinates) ,&((*tt_it)->coordinates));

		//PrintDistance
//		for(vector<myVector *>::iterator tt_it = rsTemp.begin(); tt_it != rsTemp.end(); ++tt_it)
//			cout << (*tt_it)->id << " " << (*tt_it)->similarity << endl;

		//Fill in vectors
		int c = 0;
//		cout << tS_it->id << ": ";
		for(vector<double>::iterator d_it = (*tS_it).coordinates.begin(); d_it != (*tS_it).coordinates.end(); ++d_it, ++c){
			double result = 0.0;
			if(!abc[(*tS_it).id-1][c]){
				//P Neighbors
				double suma = 0.0, sumb = 0.0;
				vector<myVector *>::iterator tt_it = rsTemp.begin();
				if(rsTemp.size() != 0){
					for(vector<myVector *>::iterator tt_it = rsTemp.begin(); tt_it != rsTemp.end(); ++tt_it){
						if((*tt_it)->similarity < 0)
							suma += (-1)*(*tt_it)->similarity;
						else
							suma += (*tt_it)->similarity;
						sumb += (*tt_it)->similarity*( (*tt_it)->coordinates[c] - users[(*tt_it)->id - 1 ].meanRating);
					}
					result = (1/suma)*sumb;
				}
			}else{
				result = (*d_it) - (*u1_it).meanRating;
			}
			(*d_it) = result;
		}

		//Recommend
		cout << tS_it->id << ' ';
		for(int i = 0; i < 5; i++){
			int pos = -1, c = 0;
			double max = numeric_limits<double>::lowest();
			for(vector<double>::iterator d_it = (*tS_it).coordinates.begin(); d_it != (*tS_it).coordinates.end(); ++d_it, ++c){
				if(!abc[(*tS_it).id-1][c]){
					//find max
					if ((*d_it) > max){
						max = (*d_it);
						pos = c;
					}
				}else{} //skip
			}
			if(pos==-1){
				cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
				cout << endl << endl << endl << endl;
				getchar();
				break;
			}else{
				abc[(*tS_it).id-1][pos] = 1;
				if(lexiconK[pos].words.size()>4)
					cout << lexiconK[pos].words[4] << " ";
				else
					cout << lexiconK[pos].words[0] << " ";
			}
		}
		cout << endl << endl << endl;

	}

	cout << endl;
	cout << endl;


	///////////////////////////////
	// Cosine LSH Recommendation //
	///////////////////////////////
	cout << "Cosine LSH" << endl;
	LSH lsh(&trainSet, "cosine", 4, 4);

	u_it = users.begin();
	i = 0;
	for(vector<myVector>::iterator tS_it = trainSet.begin(); tS_it != trainSet.end(); ++tS_it, ++u_it, ++i){
		vector<myVector *> rsTemp;
		//Find Neighbors
		lsh.rangeSearch2( &(*tS_it), &rsTemp);

		//Sort
		sort(rsTemp.begin(), rsTemp.end(), compareByDist);

		//Fill in vectors
		int c = 0;
		for(vector<double>::iterator d_it = (*tS_it).coordinates.begin(); d_it != (*tS_it).coordinates.end(); ++d_it, ++c){
			double result = 0.0;
			if(!abc[(*tS_it).id-1][c]){
				//P Neighbors
				double suma = 0.0, sumb = 0.0;
				vector<myVector *>::iterator tt_it = rsTemp.begin();
				if(rsTemp.size() != 0){
					for(int i = 0; i < P; i++, tt_it++){
						if((*tt_it)->similarity < 0)
							suma += (-1)*(*tt_it)->similarity;
						else
							suma += (*tt_it)->similarity;
						sumb += (*tt_it)->similarity*( (*tt_it)->coordinates[c] - users[(*tt_it)->id - 1].meanRating);
					}
					result = (1/suma)*sumb;
				}
			}else{
				result = (*d_it) - (*u_it).meanRating;
			}
			(*d_it) = result;
		}


		//Recommend
		cout << tS_it->id << ' ';
		for(int i = 0; i < 5; i++){
			int pos = -1, c = 0;
			double max = numeric_limits<double>::lowest();
			for(vector<double>::iterator d_it = (*tS_it).coordinates.begin(); d_it != (*tS_it).coordinates.end(); ++d_it, ++c){
				if(!abc[(*tS_it).id-1][c]){
					//find max
					if ((*d_it) > max){
						max = (*d_it);
						pos = c;
					}
				}else{} //skip
			}
			if(pos==-1){
				cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
				break;
			}else{
				abc[(*tS_it).id-1][pos] = 1;
				if(lexiconK[pos].words.size()>4)
					cout << lexiconK[pos].words[4] << " ";
				else
					cout << lexiconK[pos].words[0] << " ";
			}
		}
		cout << endl;
	}
	return 0;
}



*/




























































































































































/*


#include <vector>
#include <math.h>
#include <algorithm>
#include <random>
#include <chrono>

#include "myTweet.hpp"
#include "myLexicon.hpp"
#include "readFileInput.hpp"
#include "readCmdRec.hpp"
#include "readLexicon.hpp"
#include "LSH.hpp"
#include "kmeans.hpp"
#include "readFileCsv.hpp"

static int clusterCount = 50;

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

void setUsers(vector<myVector> *trainSet, int sizei, int sizej){
// int k = 0;
//	for(vector<myUser>::iterator u_it = users->begin(); u_it != users->end(); ++u_it, ++k){
	for(int i = 0; i < sizei; ++i)
		functionVec(trainSet, sizej, i);
}

bool searchInLexiconA(string word, vector<myLexiconAMember> *lexiconA, double *score){
	for(vector<myLexiconAMember>::iterator l_it = lexiconA->begin(); l_it != lexiconA->end(); ++l_it ){
		if(word == (*l_it).word){
//			cout << ">" << l_it->word << ":" << l_it->sentscore << endl;
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
//				cout << word << "-'" << i << "'" << ' ';
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

struct myKD{
	int loc;
	double distance;
};

int main(int argc, char* argv[]){
	srand(time(nullptr));
	string inputfile2 = "twitter_dataset_small_v2.csv", inputfile, lexiconAfile = "vader_lexicon.csv", lexiconKfile = "coins_queries.csv";
	string outputfile;
	vector<myUser> users;
	vector<myTweet> tweets;
	vector<myLexiconAMember> lexiconA;
	vector<myLexiconKMember> lexiconK;
	vector<myVector> trainSet, trainSet2, trainSetPrj2;
	int P = 5, a = 15;
	bool val;

	if( readCmdRec(argc, argv, &inputfile, &outputfile, &val) ) return -1;

	if( readInputFile(inputfile, &users, &tweets) ) return -1;

	if( readLexiconA(lexiconAfile, &lexiconA) ) return -1;

	if( readLexiconK(lexiconKfile, &lexiconK) ) return -1;

	if( readFileCsv(inputfile2, &trainSetPrj2) ) return -1;

	setVectors(&trainSet, users.size(), lexiconK.size());

	setVectors(&trainSet2, clusterCount, lexiconK.size());

//	// Tweet printer
//	for(vector<myTweet>::iterator id_it = tweets.begin(); id_it != tweets.end(); ++id_it){
//		cout << id_it->userid << ' ' << id_it->id << ' ';
//		for(vector<string>::iterator str_it = (*id_it).tweet.begin(); str_it != (*id_it).tweet.end(); ++str_it ){
//			cout << (*str_it) << ' ';
//		}
//		cout << endl;
//	}
//	return 0;

//	// Trainset parser
//	for(vector<myVector>::iterator tS_it = trainSet2.begin(); tS_it != trainSet2.end(); ++tS_it){
//			for(vector<double>::iterator d = (*tS_it).coordinates.begin(); d != (*tS_it).coordinates.end(); ++d){
//			}
//	}
//	cout << "start" <<endl;


	myKmeans kMeansForPrj2( &trainSetPrj2, "euclidean", clusterCount, trainSetPrj2[0].coordinates.size() );
	kMeansForPrj2.runKmeans(1, 1, 1);
	vector<int> *clAssignPrj2 = kMeansForPrj2.getClustAssigned();
//	cout << "finish" << endl;
//	getchar();
//	int iii = 0;
//	for(vector<int>::iterator c_it = clAssignPrj2->begin(); c_it != clAssignPrj2->end(); c_it++, iii++){
//		cout << iii << ": " << *c_it <<endl;
//	}

	// Fix Locations!




	/////////////////////////////////
	// Sentiment Score calculation //
	/////////////////////////////////
	vector<myTweet>::iterator id_it = tweets.begin();
	for(vector<myUser>::iterator u_it = users.begin(); u_it != users.end(); ++u_it){
		for(vector<myTweet>::iterator tw_it = (*u_it).tweets.begin(); tw_it != (*u_it).tweets.end(); ++tw_it, ++id_it){
			double totalscore = 0.0;
			for(vector<string>::iterator wo_it = (*tw_it).tweet.begin(); wo_it != (*tw_it).tweet.end(); ++wo_it){
				double score = 0.0;
				searchInLexiconA(*wo_it, &lexiconA, &score);
				totalscore += score;
			}
			totalscore = totalscore / sqrt(totalscore*totalscore + a);
			(*tw_it).sentscore = totalscore;
			(*id_it).sentscore = totalscore;
		}
	}



	{

	////////////////////////////////////////////
	// Cluster-Cryptocurrency Vector creation //
	////////////////////////////////////////////
	vector<vector<bool>> cba( clusterCount, vector<bool>(lexiconK.size()) );
	for(int j = 0; j < clusterCount; ++j) for(int k = 0; k < lexiconK.size(); ++k) cba[j][k] = 0;


	int i = 0, pos = -1;
	for(vector<myVector>::iterator tS_it = trainSet2.begin(); tS_it != trainSet2.end(); ++tS_it, ++i){ //for every user
		vector<int> clusterPositions;
		int ii = 0;
		for(vector<int>::iterator c_it = clAssignPrj2->begin(); c_it != clAssignPrj2->end(); c_it++, ii++){ //find tweets
			if( *c_it== i ){
				clusterPositions.push_back(findInTweets(&tweets, ii+1));
			}
		}

		ii = 0;
		for(vector<int>::iterator cl_it = clusterPositions.begin(); cl_it != clusterPositions.end(); cl_it++, ii++){ //for every tweet
			bool tempbool[lexiconK.size()] = {0};
			for(vector<string>::iterator wo_it = tweets[*cl_it].tweet.begin(); wo_it != tweets[*cl_it].tweet.end(); ++wo_it){ //for every word
				if(searchInLexiconK((*wo_it), &lexiconK, &pos)){ //search in lexicon K
					if(!tempbool[pos]){
						(*tS_it).coordinates[pos] += tweets[*cl_it].sentscore;
						cba[i][pos]   = 1;
						tempbool[pos] = 1;
					}
				}
			}
		}
	}


	///////////////////////////////////////
	// Remove pseudousers with 0 vectors //
	///////////////////////////////////////
	vector<vector<bool>>::iterator bool_it = cba.begin();
	for(vector<myVector>::iterator tS_it = trainSet2.begin(); tS_it != trainSet2.end();){
		int flag = 1;
		for(vector<double>::iterator d_it = (*tS_it).coordinates.begin(); d_it != (*tS_it).coordinates.end(); ++d_it){
			if((*d_it) != 0.0){
				flag = 0;
				break;
			}
		}

		if(flag){
			tS_it	= trainSet2.erase(tS_it);
			bool_it = cba.erase(bool_it);
		}else{
			++tS_it;
			++bool_it;
		}
	}

	///////////////////////
	// Change pseudoID's //
	///////////////////////
	int l = 1;
	for(vector<myVector>::iterator tS_it = trainSet2.begin(); tS_it != trainSet2.end(); ++tS_it, ++l)
		tS_it->id = l;


	/////////////////////////////////////////
	// Calculate rating mean for each user //
	/////////////////////////////////////////
	double arrayy[trainSet2.size()] = {0.0};
	int b = 0;
	for(vector<myVector>::iterator tS_it = trainSet2.begin(); tS_it != trainSet2.end(); ++tS_it, ++b){ //Parse Users
		double sum = 0.0; int counter = 0, c = 0;
		for(vector<double>::iterator d_it = (*tS_it).coordinates.begin(); d_it != (*tS_it).coordinates.end(); ++d_it, ++c){ //Parse User-Crypto Vector
			if(cba[(*tS_it).id-1][c]){
				sum += (*d_it);
				counter++;
			}
		}
		arrayy[b] = sum/counter;
	}


	//////////////////////
	// pseudoCLUSTERING //
	//////////////////////
	cout << "Clustering" << endl;
	myKmeans kMeans (&trainSet2, "euclidean", 5, trainSet2[0].coordinates.size());
	kMeans.runKmeans(2,1,1);
	vector<int> *clAssign = kMeans.getClustAssigned();

	i = 0 ;
	vector<myUser>::iterator u1_it = users.begin();
	for(vector<myVector>::iterator tS_it = trainSet2.begin(); tS_it != trainSet2.end(); ++tS_it, ++u1_it, ++i){
		vector<myVector *> rsTemp;

		//Find Neighbors, no sort necessary
		for(vector<myVector>::iterator tS2_it = trainSet2.begin(); tS2_it != trainSet2.end(); ++tS2_it)
			if( (*clAssign)[tS_it->id-1] == (*clAssign)[tS2_it->id-1])
				if(tS_it->id!=tS2_it->id)
					rsTemp.push_back( &(*tS2_it) );

		//Find distance
		for(vector<myVector *>::iterator tt_it = rsTemp.begin(); tt_it != rsTemp.end(); ++tt_it)
			(*tt_it)->similarity =   euclideanDistOfTwoVectors(&((*tS_it).coordinates) ,&((*tt_it)->coordinates));


//		//PrintDistance
//		for(vector<myVector *>::iterator tt_it = rsTemp.begin(); tt_it != rsTemp.end(); ++tt_it)
//			cout << (*tt_it)->id << " " << (*tt_it)->similarity << endl;


//		//Print Vector
//		cout << tS_it->id << ": ";
//		int cc = 0;
//		for(vector<double>::iterator d_it = (*tS_it).coordinates.begin(); d_it != (*tS_it).coordinates.end(); ++d_it, ++cc){
//			if(!abc[(*tS_it).id-1][cc]){
//				cout << (*d_it) << " ";
//			}else{
//				cout << "|" << (*d_it) << "|" << " ";
//			}
//		}
//		cout << endl << endl;

		//Fill in vectors
		int c = 0;
		for(vector<double>::iterator d_it = (*tS_it).coordinates.begin(); d_it != (*tS_it).coordinates.end(); ++d_it, ++c){
			double result = 0.0;
			if(!cba[i][c]){
				//P Neighbors
				double suma = 0.0, sumb = 0.0;W
				for(vector<myVector *>::iterator tt_it = rsTemp.begin(); tt_it != rsTemp.end(); ++tt_it){
					if((*tt_it)->similarity < 0)
						suma += (-1)*(*tt_it)->similarity;
					else
						suma += (*tt_it)->similarity;
					sumb += (*tt_it)->similarity*( (*tt_it)->coordinates[c] - users[(*tt_it)->id ].meanRating);
				}
				result += (1/suma)*sumb;
			}else{
				result = (*d_it) - (*u1_it).meanRating;
			}
			(*d_it) = result;
		}

		//Recommend
		cout << tS_it->id << ' ' << u1_it->id << " ";
		for(int i = 0; i < 5; i++){
			int pos = -1, c = 0;
			double max = numeric_limits<double>::lowest();
			for(vector<double>::iterator d_it = (*tS_it).coordinates.begin(); d_it != (*tS_it).coordinates.end(); ++d_it, ++c){
				if(!cba[i][c]){
					//find max
					if ((*d_it) > max){
						max = (*d_it);
						pos = c;
					}
				}else{} //skip
			}
			if(pos==-1){
				break;
			}else{
				cba[i][pos] = 1;
				if(lexiconK[pos].words.size()>4)
					cout << lexiconK[pos].words[4] << " ";
				else
					cout << lexiconK[pos].words[0] << " ";
			}
		}
		cout << endl;
	}
	return 0;


	/////////////////////////////////////
	// pseudo Cosine LSHRecommendation //
	/////////////////////////////////////
	cout << "Cosine LSH" << endl;
	LSH lsh(&trainSet2, "cosine", 4, 4);
	i = 0;
	for(vector<myVector>::iterator tS_it = trainSet2.begin(); tS_it != trainSet2.end(); ++tS_it, ++i){
		P = 5;
		vector<myVector *> rsTemp;
		//Find Neighbors
		lsh.rangeSearch2( &(*tS_it), &rsTemp);
		//Sort
		sort(rsTemp.begin(), rsTemp.end(), compareByDist);
		//Fill in vectors
		int c = 0;
		for(vector<double>::iterator d_it = (*tS_it).coordinates.begin(); d_it != (*tS_it).coordinates.end(); ++d_it, ++c){
			double result = 0.0;
			if(rsTemp.size() < P)
				P = rsTemp.size();
			if(!cba[i][c]){
				//P Neighbors
				double suma = 0.0, sumb = 0.0;
				vector<myVector *>::iterator tt_it = rsTemp.begin();
				for(int jj = 0; jj < P; jj++, tt_it++){
					if((*tt_it)->similarity < 0)
						suma += (-1)*(*tt_it)->similarity;
					else
						suma += (*tt_it)->similarity;
					sumb += (*tt_it)->similarity*( (*tt_it)->coordinates[c] - users[(*tt_it)->id ].meanRating);
				}
				result += (1/suma)*sumb;
			}else{
				result = (*d_it) - arrayy[i];
			}
			(*d_it) = result;
		}

		//Recommend
		cout << i << ' ' << " ";
		for(int ii = 0; ii < 5; ii++){
			int pos = -1, c = 0;
			double max = numeric_limits<double>::lowest();
			for(vector<double>::iterator d_it = (*tS_it).coordinates.begin(); d_it != (*tS_it).coordinates.end(); ++d_it, ++c){
				if(!cba[i][c]){
					//find max
					if ((*d_it) > max){
						max = (*d_it);
						pos = c;
					}
				}else{} //skip
			}
			if(pos==-1){
				cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
				break;
			}else{
				cba[i][pos] = 1;
				if(lexiconK[pos].words.size()>4)
					cout << lexiconK[pos].words[4] << " ";
				else
					cout << lexiconK[pos].words[0] << " ";
			}
		}
		cout << endl << endl;
	}

	}


	return 0;
























	/////////////////////////////////////////
	// User-Cryptocurrency Vector creation //
	/////////////////////////////////////////
	vector<vector<bool>> abc(users.size(),vector<bool>(lexiconK.size()));
	for(int j = 0; j < users.size(); ++j) for(int k = 0; k < lexiconK.size(); ++k) abc[j][k] = 0;

//	vector<myVector>::iterator tS_it = trainSet.begin();
//	for(vector<myUser>::iterator u_it = users.begin(); u_it != users.end(); ++u_it, ++tS_it, ++i){


  	int i = 0, pos = -1;
	vector<myUser>::iterator u_it = users.begin();
	for(vector<myVector>::iterator tS_it = trainSet.begin(); tS_it != trainSet.end(); ++u_it, ++tS_it, ++i){
		for(vector<myTweet>::iterator tw_it = (*u_it).tweets.begin(); tw_it != (*u_it).tweets.end(); ++tw_it){
			//TODO me advance wste na einai sumvato kai me ta duo,

			bool tempbool[lexiconK.size()] = {0};
			for(vector<string>::iterator wo_it = (*tw_it).tweet.begin(); wo_it != (*tw_it).tweet.end(); ++wo_it){
				if(searchInLexiconK((*wo_it), &lexiconK, &pos)){
					if(!tempbool[pos]){
						(*tS_it).coordinates[pos] += (*tw_it).sentscore;
						abc[i][pos]   = 1;
						tempbool[pos] = 1;
					}
				}
			}
		}
	}



	/////////////////////////////////
	// Remove users with 0 vectors //
	/////////////////////////////////
//	tS_it = trainSet.begin();
//	for(vector<myUser>::iterator u_it = users.begin(); u_it != users.end(); )

	u_it = users.begin();
	vector<vector<bool>>::iterator bool_it = abc.begin();
	for(vector<myVector>::iterator tS_it = trainSet.begin(); tS_it != trainSet.end(); ){
		int flag = 1;
		for(vector<double>::iterator d_it = (*tS_it).coordinates.begin(); d_it != (*tS_it).coordinates.end(); ++d_it){
			if((*d_it) != 0.0){
				flag = 0;
				break;
			}
		}
		if(flag){
			tS_it	= trainSet.erase(tS_it);
			u_it	= users.erase(u_it);
			bool_it = abc.erase(bool_it);
		}else{
			++u_it;
			++tS_it;
			++bool_it;
		}
	}

	/////////////////
	// Change ID's //
	/////////////////
	int l = 1;
	for(vector<myVector>::iterator tS_it = trainSet.begin(); tS_it != trainSet.end(); ++tS_it, ++l)
		tS_it->id = l;


	/////////////////////////////////////////
	// Calculate rating mean for each user //
	/////////////////////////////////////////
	vector<myUser>::iterator u_it = users.begin();
	for( vector<myVector>::iterator tS_it = trainSet.begin(); tS_it != trainSet.end(); ++u_it, ++tS_it){
		//Parse Users
		double sum = 0.0; int counter = 0, c = 0;
		for(vector<double>::iterator d_it = (*tS_it).coordinates.begin(); d_it != (*tS_it).coordinates.end(); ++d_it, ++c){ //Parse User-Crypto Vector
			if(abc[(*tS_it).id-1][c]){
				sum += (*d_it);
				counter++;
			}
		}
		(*u_it).meanRating = sum/counter;
	}


	////////////////
	// CLUSTERING //
	////////////////
	cout << "Clustering" << endl;
	myKmeans kMeans (&trainSet, "euclidean", clusterCount, trainSet[0].coordinates.size());
	kMeans.runKmeans(2,1,1);
	vector<int> *clAssign = kMeans.getClustAssigned();

	i = 0;
	vector<myUser>::iterator u1_it = users.begin();
	for(vector<myVector>::iterator tS_it = trainSet.begin(); tS_it != trainSet.end(); ++tS_it, ++u1_it, ++i){
		vector<myVector *> rsTemp;

		//Find Neighbors, no sort necessary
		for(vector<myVector>::iterator tS2_it = trainSet.begin(); tS2_it != trainSet.end(); ++tS2_it)
			if( (*clAssign)[tS_it->id-1] == (*clAssign)[tS2_it->id-1] )
				if( tS_it->id != tS2_it->id )
					rsTemp.push_back( &(*tS2_it) );

		//Find distance
		for(vector<myVector *>::iterator tt_it = rsTemp.begin(); tt_it != rsTemp.end(); ++tt_it)
			(*tt_it)->similarity =   euclideanDistOfTwoVectors(&((*tS_it).coordinates) ,&((*tt_it)->coordinates));

		//Fill in vectors
		int c = 0;
		for(vector<double>::iterator d_it = (*tS_it).coordinates.begin(); d_it != (*tS_it).coordinates.end(); ++d_it, ++c){
			double result = 0.0;
			if(!abc[(*tS_it).id-1][c]){
				//P Neighbors
				double suma = 0.0, sumb = 0.0;
				vector<myVector *>::iterator tt_it = rsTemp.begin();
				for(vector<myVector *>::iterator tt_it = rsTemp.begin(); tt_it != rsTemp.end(); ++tt_it){
					if((*tt_it)->similarity < 0)
						suma += (-1)*(*tt_it)->similarity;
					else
						suma += (*tt_it)->similarity;
					sumb += (*tt_it)->similarity*( (*tt_it)->coordinates[c] - users[(*tt_it)->id ].meanRating);
				}
				result += (1/suma)*sumb;
			}else{
				result = (*d_it) - (*u1_it).meanRating;
			}
			(*d_it) = result;
		}

		//Recommend
		cout << tS_it->id << ' ';
		for(int i = 0; i < 5; i++){
			int pos = -1, c = 0;
			double max = numeric_limits<double>::lowest();
			for(vector<double>::iterator d_it = (*tS_it).coordinates.begin(); d_it != (*tS_it).coordinates.end(); ++d_it, ++c){
				if(!abc[(*tS_it).id-1][c]){
					//find max
					if ((*d_it) > max){
						max = (*d_it);
						pos = c;
					}
				}else{} //skip
			}
			if(pos==-1){
				break;
			}else{
				abc[(*tS_it).id-1][pos] = 1;
				if(lexiconK[pos].words.size()>4)
					cout << lexiconK[pos].words[4] << " ";
				else
					cout << lexiconK[pos].words[0] << " ";
			}
		}
		cout << endl;
	}


	///////////////////////////////
	// Cosine LSH Recommendation //
	///////////////////////////////
	cout << "Cosine LSH" << endl;
	LSH lsh(&trainSet, "cosine", 4, 4);

	i = 0;
	vector<myUser>::iterator u_it = users.begin();
	for(vector<myVector>::iterator tS_it = trainSet.begin(); tS_it != trainSet.end(); ++tS_it, ++u_it, ++i){
		vector<myVector *> rsTemp;

		//Find Neighbors
		lsh.rangeSearch2( &(*tS_it), &rsTemp);

		//Sort
		sort(rsTemp.begin(), rsTemp.end(), compareByDist);

		//Fill in vectors
		int c = 0;
		for(vector<double>::iterator d_it = (*tS_it).coordinates.begin(); d_it != (*tS_it).coordinates.end(); ++d_it, ++c){
			double result = 0.0;
			if(!abc[(*tS_it).id-1][c]){
				//P Neighbors
				double suma = 0.0, sumb = 0.0;
				vector<myVector *>::iterator tt_it = rsTemp.begin();
				for(int i = 0; i < P; i++, tt_it++){
					if((*tt_it)->similarity < 0)
						suma += (-1)*(*tt_it)->similarity;
					else
						suma += (*tt_it)->similarity;
					sumb += (*tt_it)->similarity*( (*tt_it)->coordinates[c] - users[(*tt_it)->id ].meanRating);
				}
				result += (1/suma)*sumb;
			}else{
				result = (*d_it) - (*u_it).meanRating;
			}
			(*d_it) = result;
		}


		//Recommend
		cout << tS_it->id << ' ' << u_it->id << " ";
		for(int i = 0; i < 5; i++){
			int pos = -1, c = 0;
			double max = numeric_limits<double>::lowest();
			for(vector<double>::iterator d_it = (*tS_it).coordinates.begin(); d_it != (*tS_it).coordinates.end(); ++d_it, ++c){
				if(!abc[(*tS_it).id][c]){
					//find max
					if ((*d_it) > max){
						max = (*d_it);
						pos = c;
					}
				}else{} //skip
			}
			if(pos==-1){
				break;
			}else{
				abc[(*tS_it).id][pos] = 1;
				if(lexiconK[pos].words.size()>4)
					cout << lexiconK[pos].words[4] << " ";
				else
					cout << lexiconK[pos].words[0] << " ";
			}
		}
		cout << endl;
	}

	return 0;
}

*/
