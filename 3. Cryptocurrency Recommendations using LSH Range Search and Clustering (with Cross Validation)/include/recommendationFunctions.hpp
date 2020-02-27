#ifndef rec_H
#define rec_H

#include <vector>
#include <math.h>
#include <algorithm>
#include "myVector.hpp"
#include "myTweet.hpp"
#include "myLexicon.hpp"
#include "simplefunctions.hpp"
#include "LSH.hpp"
#include "kmeans.hpp"


void sentimentScoresCalculation(vector<myTweet> *, vector<myLexiconAMember> *);

void userCryptoVectorCreation(vector<myVector> *, vector<vector<bool>> *, vector<myTweet> *, vector<myUser> *, vector<myLexiconKMember> *);

void removeZeroVectors( vector<myVector> *, vector<vector<bool>> *, vector<myUser> *);

void calculateRatingMean(vector<myVector> *, vector<vector<bool>> *, vector<myUser> *);

void createVirtualUsers(vector<myUser> *, vector<myVector> *, vector<myTweet> *, int );

void prerecClusteringFunc(vector<myVector> *, vector<myUser> , int , int , vector<vector<bool>> *, vector<myLexiconKMember> , string , int );

void prerecCosineFunc(vector<myVector> *, vector<myUser> , int , int , vector<vector<bool>> *, vector<myLexiconKMember> , string , int );

void recommend(vector<myVector> *, vector<myUser> users, int , vector<vector<bool>> *, vector<myLexiconKMember> , double , string , int );

#endif
