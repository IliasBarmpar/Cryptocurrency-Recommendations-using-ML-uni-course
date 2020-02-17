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
#include "readFileCsv.hpp"

#include "LSH.hpp"
#include "kmeans.hpp"
#include "simplefunctions.hpp"
#include "recommendationFunctions.hpp"

static int virtualOpt = 0;
static int virtualusers = 100;

int main(int argc, char* argv[]){
	srand(time(nullptr));
	bool val;
	string inputfile, lexiconAfile = "vader_lexicon.csv", lexiconKfile = "coins_queries.csv";
	string outputfile;
	vector<myUser> users;
	vector<myTweet> tweets;
	vector<myLexiconAMember> lexiconA;
	vector<myLexiconKMember> lexiconK;
	vector<myVector> trainSet;
	int NP = 120, P = 20, recommendCount = 5;

	// Reads and initializations
	if( readCmdRec(argc, argv, &inputfile, &outputfile, &val) )	return -1;

	if( readInputFile(inputfile, &users, &tweets) )				return -1;

	if( readLexiconA(lexiconAfile, &lexiconA) )					return -1;

	if( readLexiconK(lexiconKfile, &lexiconK) )					return -1;


	// Sentiment Score calculation
	sentimentScoresCalculation( &tweets, &lexiconA);

	if(!virtualOpt){	/* Normal Users */
		// Set User-Crypto Vectors
		setVectors(&trainSet, users.size(), lexiconK.size());

	}else{				/* Virtual Users */

		vector<myVector> trainSetPrj2;
		if( readFileCsv( "twitter_dataset_small_v2.csv", &trainSetPrj2) ) return -1;

		// Set VirtualUser-Crypto Vectors
		setVectors(&trainSet, virtualusers, lexiconK.size());

		// Clear Old Users
		for(vector<myUser>::iterator u_it = users.begin(); u_it != users.end(); ++u_it)
			(*u_it).~myUser();
		users.clear();

		// Create new Virtual User-Crypto vectors
//		cout << "vrc...";
		createVirtualUsers(&users, &trainSetPrj2, &tweets, virtualusers);
//		cout << " done!" << endl;

		NP = 5;
		recommendCount = 2;
	}

	// User-Cryptocurrency Vector creation
	vector<vector<bool>> userCryptoReferences( users.size(), vector<bool>(lexiconK.size()));
		for(int j = 0; j < users.size(); ++j) for(int k = 0; k < lexiconK.size(); ++k) userCryptoReferences[j][k] = 0;
	userCryptoVectorCreation( &trainSet, &userCryptoReferences, &tweets, &users, &lexiconK);


	// Remove users with 0 vectors and reset their IDs
	removeZeroVectors( &trainSet, &userCryptoReferences, &users);


	// Calculate rating mean for each vector
	calculateRatingMean( &trainSet, &userCryptoReferences, &users );

	if(val){
		// Copy Users
		vector<myUser> usersReset = users;
		vector<vector<bool>> userCryptoReferencesReset = userCryptoReferences;
		vector<myVector> trainSetReset = trainSet;

		vector<myUser> usersCltOut = users;
		vector<vector<bool>> userCryptoReferencesCltOut  = userCryptoReferences;
		vector<myVector> trainSetCltOut  = trainSet;

		vector<myUser> usersLshIn  = users;
		vector<myUser> usersCltIn  = users;
		vector<vector<bool>> userCryptoReferencesLshIn  = userCryptoReferences;
		vector<vector<bool>> userCryptoReferencesCltIn  = userCryptoReferences;
		vector<myVector> trainSetLshIn  = trainSet;
		vector<myVector> trainSetCltIn  = trainSet;


		// Cosine LSH Recommendation
//		cout << "Lsh Out" << endl;
		prerecCosineFunc( &trainSet, users, P, recommendCount, &userCryptoReferences, lexiconK, outputfile, 0);


		// Clustering
//		cout << "Clustering Out" << endl;
		prerecClusteringFunc( &trainSetCltOut, usersCltOut, NP, recommendCount, &userCryptoReferencesCltOut, lexiconK, outputfile, 0);


		// Create an one dimention array of users and cryptocurrencies mentioned
		vector<uc> ucloc;
		setUserCryptLoc( &ucloc, &userCryptoReferencesReset);
		int *uclocfolds = kfold( ucloc.size(), 10 );

		// K-Fold Cross Validation
		double MAElsh = 0.0;
		double MAEclt = 0.0;
		for(int kf = 0; kf < 10; ++kf){
			// Set Validation set
			for(int i = 0; i < ucloc.size(); ++i)
				if(uclocfolds[i]==kf){
					userCryptoReferencesLshIn[ucloc[i].user][ucloc[i].crypto] = 0;
					userCryptoReferencesCltIn[ucloc[i].user][ucloc[i].crypto] = 0;
				}

			// Calculate Rating Mean and Fill in Vectors
			calculateRatingMean( &trainSetLshIn, &userCryptoReferencesLshIn, &usersLshIn );

			// Calculate Rating Mean and Fill in Vectors
			calculateRatingMean( &trainSetCltIn, &userCryptoReferencesCltIn, &usersCltIn );


			// Cosine LSH Recommendation
//			cout << "Lsh In" << endl;
			prerecCosineFunc( &trainSetLshIn, usersLshIn, P, recommendCount, &userCryptoReferencesLshIn, lexiconK, outputfile, 0);


			// Clustering
//			cout << "Clustering In" << endl;
			prerecClusteringFunc( &trainSetCltIn, usersCltIn, NP, recommendCount, &userCryptoReferencesCltIn, lexiconK, outputfile, 0);


			// Calculate MAE
//			cout << "MAE" << endl;
			int Jlsh = 0, Jclt = 0;
			double resultlsh = 0.0, resultclt = 0.0;
			for(int i = 0; i < ucloc.size(); ++i){
				if(uclocfolds[i]==kf){
					double templsh = trainSet[ucloc[i].user].coordinates[ucloc[i].crypto] - trainSetLshIn[ucloc[i].user].coordinates[ucloc[i].crypto];
					if(templsh < 0) templsh = templsh*(-1);
					resultlsh += templsh;
					Jlsh++;

					double tempclt = trainSetCltOut[ucloc[i].user].coordinates[ucloc[i].crypto] - trainSetCltIn[ucloc[i].user].coordinates[ucloc[i].crypto];
					if(tempclt < 0) tempclt = tempclt*(-1);
					resultclt += tempclt;
					Jclt++;
				}
			}

			if(Jlsh!=0)
				resultlsh = resultlsh/Jlsh;
			else
				resultlsh = 0;

			if(Jclt!=0)
				resultclt = resultclt/Jclt;
			else
				resultclt = 0;

			MAElsh += resultlsh;
			MAEclt += resultclt;

			//Reset
			userCryptoReferencesLshIn = userCryptoReferencesReset;
			trainSetLshIn  = trainSetReset;
			usersLshIn  = usersReset;

			userCryptoReferencesCltIn = userCryptoReferencesReset;
			trainSetCltIn = trainSetReset;
			usersCltIn = usersReset;
		}
		MAElsh = MAElsh/10;
		cout << "Cosine LSH Recommendation MAE: " << MAElsh << endl;

		MAEclt = MAEclt/10;
		cout << "Clustering Recommendation MAE: " << MAEclt << endl;

	}else{
		ofstream outputFile(outputfile);
		outputFile.close();

		// Cosine LSH Recommendation
		prerecCosineFunc    ( &trainSet, users, P, recommendCount, &userCryptoReferences, lexiconK, outputfile, 1);

		// Clustering
		prerecClusteringFunc(&trainSet, users, NP, recommendCount, &userCryptoReferences, lexiconK, outputfile, 1);
	}
}

