#ifndef smpf_H
#define smpf_H

#include "myVector.hpp"
#include "myTweet.hpp"
#include "myLexicon.hpp"

struct uc{
	int user;
	int crypto;
};

void setVectors(vector<myVector> *, int , int );

void functionVec( vector<myVector> *, int , int );

bool searchInLexiconA(string , vector<myLexiconAMember> *, double *);

bool searchInLexiconK(string , vector<myLexiconKMember> *, int *);

bool checkIfZeroVector(double *, int );

bool compareByDist(const myVector * , const myVector * );

int findInTweets(vector<myTweet> *, int );

void setUserCryptLoc(vector<uc> *, vector<vector<bool>> *);

void shuffleArray(int* , int );

int* kfold( int , int );

void printUserCrypto(vector<myVector> *, int );

#endif
