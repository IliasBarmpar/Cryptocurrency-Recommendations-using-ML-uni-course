//============================================================================
// Name        : Prj1.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <random>
#include <chrono>
#include <string.h>
#include "LSH.hpp"
#include "ExactNN.hpp"
#include "readFile.hpp"
#include <fstream>
#include <time.h>
using namespace std;

static int L = 5;

int main(int argc, char* argv[]){
	cout << "LSH" << endl;
	int K = 4; 										//TODO possibly cin
	srand(time(nullptr));

/*			     Read Input 				*/
	string inputfile1 = "input/input_small_fixed";	//TODO User input
	list< myVector > entryPoints;
	readFile(inputfile1, &entryPoints);



/* 			    Read Queries 				*/
	string inputfile2 = "input/query_small_fixed";	//TODO User input
	list< myVector > queries;
	readFile(inputfile2, &queries);


/*			Create LSH hashtables 			*/
	string metric = "euclidean"; //TODO cin
	LSH lsh;
	HashTable *htable;
	for(int j = 0; j < L; ++j){  // for each hashtable
		htable = new HashTable(metric, entryPoints.size(), K);
		for(list< myVector >::iterator l_it = entryPoints.begin(); l_it != entryPoints.end(); ++l_it)
			htable->putVector(&(*l_it));
		lsh.addTable(htable);
		htable = NULL;
	}

//////////////////////////////////////////////

/* 		 		LSH functions 				*/
	// Range Search
	list<list<myVector *>> rangeSearchResults;
	list<myVector *> rsTemp;
	double _Rr = 400;
	if(_Rr!=0){
		for(list< myVector >::iterator l_it = queries.begin(); l_it != queries.end(); ++l_it){
			lsh.rangeSearch(&(*l_it), &_Rr, &rsTemp); //TODO1 under here
			rangeSearchResults.push_back(rsTemp);
			rsTemp.clear();
		}
	}
	//do{ //TODO1
	//	if(rsTemp.size()==0)
	//		_R = _R + _R/4;
	//}while(rsTemp.size()==0);



	// Approximate NN
	list<myVector *> approxNNSearchResults;
	myVector *nnTemp;
	list<double> mindistList;
	for(list< myVector >::iterator l_it = queries.begin(); l_it != queries.end(); ++l_it){
		double mindist = numeric_limits<double>::max();
		lsh.approxNNSearch(&(*l_it), &nnTemp, &mindist);
		approxNNSearchResults.push_back(nnTemp);
		mindistList.push_back(mindist);
		nnTemp = NULL;
	}

	// Exact NN
	list<double> exactNNResults = NNSearch ( &queries, &entryPoints, metric);

	//Process that iterates from every single list we've made ( R neighbors, Nearest neighbor, distance <metric>, distance <true>, time <metric>, time <true> )
	//cout << "Please give an output file name." << endl;
	//string ofname;
	//getline(cin, ofname);
	ofstream outputFile("output/LSHresults");
	list<double>::iterator mindistLSH_it	= mindistList.begin();
	list<double>::iterator mindistENN_it	= exactNNResults.begin();
	list<list<myVector *>>::iterator rS_it	= rangeSearchResults.begin();
	list<myVector *>::iterator aNN_it		= approxNNSearchResults.begin();
	for(list< myVector >::iterator l_it		= queries.begin(); l_it != queries.end(); ++l_it, ++rS_it, ++aNN_it, ++mindistLSH_it, ++mindistENN_it){
		outputFile << "Query: " << (*l_it).id << endl;
		if(rangeSearchResults.size() > 0){
			outputFile << "R-near neighbors: " << endl;
			if(rS_it->size()==0){
				outputFile << "empty" << endl;
			}else{
				for(list<myVector *>::iterator list_it = (*rS_it).begin(); list_it != (*rS_it).end(); ++list_it){
					outputFile << (*list_it)->id << endl;
				}
			}
		}
		outputFile << "Nearest neighbor: " << (*aNN_it)->id << endl;
		outputFile << "distanceLSH: " << (*mindistLSH_it) << endl;
		outputFile << "distanceTrue: " << (*mindistENN_it) << endl;
		outputFile << "tLSH: " << endl;
		outputFile << "tTrue: " << endl << endl;
	}
	return 0;
}

// Printer RS
/*
	for(list<list<myVector *>>::iterator li_it = rangeSearchResults.begin(); li_it != rangeSearchResults.end(); ++li_it){
		for(list<myVector *>::iterator list_it = (*li_it).begin(); list_it != (*li_it).end(); ++list_it){
			cout << (*list_it)->id <<" ";
		}
		cout << endl;
	}
}
*/

// Printer NN
/*
	for(list<myVector *>::iterator z_it = approxNNSearchResults.begin(); z_it != approxNNSearchResults.end(); ++z_it){
		cout << (*z_it)->id << " -";
		for(vector<int>::iterator zz_it = ((*z_it)->coordinates).begin(); zz_it != ((*z_it)->coordinates).end(); ++zz_it){
			cout << " " << (*zz_it);
		}
		cout << endl;
	}
*/

//Entry/Query printer
/*
	for(list< myVector >::iterator it_it = entryPoints.begin(); it_it != entryPoints.end(); ++it_it ){
		cout << (*it_it).id;
		for(vector< int >::iterator itt_it = (*it_it).coordinates.begin(); itt_it != ((*it_it).coordinates).end(); ++itt_it ){
			cout << " " << (*itt_it);
		}
		cout << endl;
	}
*/
