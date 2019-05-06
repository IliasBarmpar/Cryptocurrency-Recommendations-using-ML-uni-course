#include "recommendationFunctions.hpp"

void sentimentScoresCalculation(vector<myTweet> *tweets, vector<myLexiconAMember> *lexiconA){
	// Sentiment Score calculation
	for( vector<myTweet>::iterator tw_it = tweets->begin(); tw_it != tweets->end(); ++tw_it){
		double totalscore = 0.0;
		for(vector<string>::iterator wo_it = (*tw_it).tweet.begin(); wo_it != (*tw_it).tweet.end(); ++wo_it){
			double score = 0.0;
			searchInLexiconA(*wo_it, lexiconA, &score);
			totalscore += score;
		}
		totalscore = totalscore / sqrt(totalscore*totalscore + 15);
		(*tw_it).sentscore = totalscore;
	}
}

void userCryptoVectorCreation(vector<myVector> *trainSet, vector<vector<bool>> *userCryptoReferences, vector<myTweet> *tweets, vector<myUser> *users, vector<myLexiconKMember> *lexiconK){
	int i = 0, pos = -1;
	for(vector<myUser>::iterator u_it = users->begin(); u_it != users->end(); ++u_it, ++i){
		for(vector<int>::iterator tw_it = (*u_it).tweetIDs.begin(); tw_it != (*u_it).tweetIDs.end(); ++tw_it){
			bool tempbool[lexiconK->size()] = {0};
			for(vector<string>::iterator wo_it = (*tweets)[*tw_it].tweet.begin(); wo_it != (*tweets)[*tw_it].tweet.end(); ++wo_it){
				if(searchInLexiconK((*wo_it), lexiconK, &pos)){
					if(!tempbool[pos]){
						(*trainSet)[i].coordinates[pos] += (*tweets)[*tw_it].sentscore;
						(*userCryptoReferences)[i][pos]   = 1;
						tempbool[pos] = 1;
					}
				}
			}
		}
	}
}

void removeZeroVectors( vector<myVector> *trainSet, vector<vector<bool>> *userCryptoReferences, vector<myUser> *users ){
	vector<myUser>::iterator u_it = users->begin();
	vector<vector<bool>>::iterator bool_it = userCryptoReferences->begin();
	for(vector<myVector>::iterator tS_it = trainSet->begin(); tS_it != trainSet->end(); ){
		int flag = 1;
		for(vector<double>::iterator d_it = (*tS_it).coordinates.begin(); d_it != (*tS_it).coordinates.end(); ++d_it)
			if((*d_it) != 0.0){
				flag = 0;
				break;
			}
		if(flag){
			tS_it	= trainSet->erase(tS_it);
			u_it	= users->erase(u_it);
			bool_it = userCryptoReferences->erase(bool_it);
		}else{
			u_it++;
			tS_it++;
			bool_it++;
		}
	}

	int l = 1;
	for(vector<myVector>::iterator tS_it = trainSet->begin(); tS_it != trainSet->end(); ++tS_it, ++l)
		tS_it->id = l;
}

void calculateRatingMean(vector<myVector> *trainSet, vector<vector<bool>> *userCryptoReferences, vector<myUser> *users ){
	int i = 0;
	for( vector<myVector>::iterator tS_it = trainSet->begin(); tS_it != trainSet->end(); ++i, ++tS_it){
		double sum = 0.0; int counter = 0, c = 0;
		for(vector<double>::iterator d_it = (*tS_it).coordinates.begin(); d_it != (*tS_it).coordinates.end(); ++d_it, ++c){ //Parse User-Crypto Vector
			if( (*userCryptoReferences)[(*tS_it).id-1][c]){
				sum += (*d_it);
				counter++;
			}
		}
		if(counter!=0)
			(*users)[i].meanRating = sum/counter;
		else
			(*users)[i].meanRating = 0;
	}

	// Fill Cryptocurrencies that were not mentioned
	i = 0;
	for( vector<myVector>::iterator tS_it = trainSet->begin(); tS_it != trainSet->end(); ++i, ++tS_it){
		int c = 0;
		for(vector<double>::iterator d_it = (*tS_it).coordinates.begin(); d_it != (*tS_it).coordinates.end(); ++d_it, ++c){ //Parse User-Crypto Vector
			if(!(*userCryptoReferences)[i][c]){
//				(*d_it)  = 0;
				(*d_it)  = (*users)[i].meanRating;
			}
		}
	}
}

void createVirtualUsers(vector<myUser> *virtUsers, vector<myVector> *trainSetPrj2, vector<myTweet> *tweets, int virtualusers){
	myKmeans kMeansForPrj2( trainSetPrj2, "euclidean", virtualusers, (*trainSetPrj2)[0].coordinates.size() );
	kMeansForPrj2.runKmeans(1, 1, 1);
	vector<int> *clAssignPrj2 = kMeansForPrj2.getClustAssigned();

	//Let's create us some virtual users!
	for(int i = 0; i < virtualusers; i++){
		myUser tempUser;
		tempUser.id = i+1;
		int j = 1;
		for(vector<int>::iterator c_it = clAssignPrj2->begin(); c_it != clAssignPrj2->end(); c_it++, j++){
			if( (*c_it) == i )
				tempUser.tweetIDs.push_back( findInTweets(tweets, j));
		}
		virtUsers->push_back(tempUser);
	}
}


void recommend(vector<myVector> *trainSet, vector<myUser> users, int cryptC, vector<vector<bool>> *abc, vector<myLexiconKMember> lexiconK, double duration, string outputfile, int opt){
	ofstream outputFile;//(outputfile);
	outputFile.open(outputfile, std::ios::app);

	if(opt){
		outputFile << "Cosine LSH" << endl;
	}else{
		outputFile << "Clustering" << endl;
	}

	int i = 0;
	for(vector<myVector>::iterator tS_it = trainSet->begin(); tS_it != trainSet->end(); ++tS_it, ++i){
		//Recommend
		outputFile << users[i].id << ' ';
		for(int m = 0; m < cryptC; m++){
			int pos = -1, c = 0;
			double max = numeric_limits<double>::lowest();
			for(vector<double>::iterator d_it = (*tS_it).coordinates.begin(); d_it != (*tS_it).coordinates.end(); ++d_it, ++c){
				if(!(*abc)[i][c]){
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
				(*abc)[i][pos] = 1;
				if(lexiconK[pos].words.size()>4){
					outputFile << lexiconK[pos].words[4] << " ";
				}else{
					outputFile << lexiconK[pos].words[0] << " ";
				}
			}
		}
		outputFile << endl;
	}
	outputFile << "Execution Time: " << duration << endl << endl;

	outputFile.close();
}


void prerecClusteringFunc(vector<myVector> *trainSet, vector<myUser> users, int clusterCount, int cryptC, vector<vector<bool>> *abc, vector<myLexiconKMember> lexiconK, string outputfile, int flagRec){
	auto start = std::chrono::steady_clock::now();

	myKmeans kMeans (trainSet, "euclidean", clusterCount, (*trainSet)[0].coordinates.size());
	kMeans.runKmeans(2,1,1);
	vector<int> *clAssign = kMeans.getClustAssigned();

	int i = 0;
	for(vector<myVector>::iterator tS_it = trainSet->begin(); tS_it != trainSet->end(); ++tS_it, ++i){
		vector<myVector *> rsTemp;

		//Find Neighbors, no sort necessary
		for(vector<myVector>::iterator tS2_it = trainSet->begin(); tS2_it != trainSet->end(); ++tS2_it)
			if( (*clAssign)[tS_it->id-1] == (*clAssign)[tS2_it->id-1] )
				if( tS_it->id != tS2_it->id )
					rsTemp.push_back( &(*tS2_it) );

		//Find distance
		for(vector<myVector *>::iterator tt_it = rsTemp.begin(); tt_it != rsTemp.end(); ++tt_it)
			(*tt_it)->similarity = euclideanDistOfTwoVectors(&((*tS_it).coordinates) ,&((*tt_it)->coordinates));


		//Fill in vectors
		int c = 0;
		for(vector<double>::iterator d_it = (*tS_it).coordinates.begin(); d_it != (*tS_it).coordinates.end(); ++d_it, ++c){
			double result = 0.0;
			if(!(*abc)[i][c]){
				//P Neighbors
				double suma = 0.0, sumb = 0.0;
				if(rsTemp.size() != 0){
					for(vector<myVector *>::iterator tt_it = rsTemp.begin(); tt_it != rsTemp.end(); ++tt_it){
						if((*tt_it)->similarity < 0)
							suma += (-1)*(*tt_it)->similarity;
						else
							suma += (*tt_it)->similarity;
						sumb += (*tt_it)->similarity*( (*tt_it)->coordinates[c] - users[(*tt_it)->id - 1].meanRating);
					}
					if(suma!=0){
//						result = (sumb/suma);
						result = (sumb/suma) - users[i].meanRating;
					}else{
//						result = 0;
						result = -users[i].meanRating;
					}
				}
			}else{
				result = (*d_it) - users[i].meanRating;
			}
			(*d_it) = result ;
		}
	}
	auto end = std::chrono::steady_clock::now();
	double duration = std::chrono::duration<double>(end-start).count(); //Computes in seconds.

	if(flagRec)
		recommend(trainSet, users, cryptC, abc, lexiconK, duration, outputfile, 0);
}

void prerecCosineFunc(vector<myVector> *trainSet, vector<myUser> users, int P, int cryptC, vector<vector<bool>> *abc, vector<myLexiconKMember> lexiconK, string outputfile, int flagRec){
	auto start = std::chrono::steady_clock::now();

	LSH lsh(trainSet, "cosine", 4, 10);

	int i = 0, Pnum;
	for(vector<myVector>::iterator tS_it = trainSet->begin(); tS_it != trainSet->end(); ++tS_it, ++i){
		vector<myVector *> rsTemp;
		Pnum = P;

		//Find Neighbors
		lsh.rangeSearch2( &(*tS_it), &rsTemp);
		if(rsTemp.size() < Pnum)
			Pnum = rsTemp.size();

		//Sort
		sort(rsTemp.begin(), rsTemp.end(), compareByDist);

		//Fill in vectors
		int c = 0;
		for(vector<double>::iterator d_it = (*tS_it).coordinates.begin(); d_it != (*tS_it).coordinates.end(); ++d_it, ++c){
			double result = 0.0;
			if(!(*abc)[i][c]){
				//P Neighbors
				double suma = 0.0, sumb = 0.0;
				vector<myVector *>::iterator tt_it = rsTemp.begin();
				if(rsTemp.size() != 0){
					for(int i = 0; i < Pnum; i++, tt_it++){
						if((*tt_it)->similarity < 0)
							suma += (-1)*(*tt_it)->similarity;
						else
							suma += (*tt_it)->similarity;
						sumb += (*tt_it)->similarity*( (*tt_it)->coordinates[c] - users[ (*tt_it)->id-1 ].meanRating); //users[(*tt_it)->id - 1].meanRating);
					}

					if(suma!=0){
//						result = (sumb/suma);
						result = (sumb/suma) - users[i].meanRating;
					}else{
//						result = 0;
						result = -users[i].meanRating;
					}
				}
			}else{
				result = (*d_it) - users[i].meanRating;
			}
			(*d_it) = result;
		}
	}
	auto end = std::chrono::steady_clock::now();
	double duration = std::chrono::duration<double>(end-start).count(); //Computes in seconds.

	if(flagRec)
		recommend(trainSet, users, cryptC, abc, lexiconK, duration, outputfile, 1);
}
