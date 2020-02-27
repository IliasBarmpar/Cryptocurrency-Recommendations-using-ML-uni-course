
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <random>
#include <chrono>
#include <string.h>
#include "HyperCube.hpp"
#include "ExactNN.hpp"
#include "readFile.hpp"

using namespace std;


int main(int argc, char* argv[]){
	cout << "HC" << endl;
	srand(time(nullptr));

/*			     Read Input 				*/
	string inputfile1 = "input/input_small_fixed"; //TODO User input
	list< myVector > entryPoints;
	readFile(inputfile1, &entryPoints);

/* 			    Read Queries 				*/
	string inputfile2 = "input/query_small_fixed"; //TODO User input
	list< myVector > queries;
	readFile(inputfile2, &queries);



/*            Create a HyperCube            */
	string metric = "euclidean";
	HyperCube hcube(metric, entryPoints.size(), 3);
	for(list< myVector >::iterator l_it = entryPoints.begin(); l_it != entryPoints.end(); ++l_it){
		hcube.putVector(&(*l_it));
	}

/*			 HyperCube Functions 			*/
	// Range Search HC
	list<list<myVector *>> rangeSearchResults;
	list<myVector *> rsTemp;
	double _R = 200; //TODO User input
	if(_R!=0){
		for(list< myVector >::iterator l_it = queries.begin(); l_it != queries.end(); ++l_it){
			hcube.rangeSearch(&(*l_it), &_R, &rsTemp);
			rangeSearchResults.push_back(rsTemp);
			rsTemp.clear();
		}
	}


	// Approx NNsearch HC
	list<myVector *> approxNNSearchResults;
	list<double> mindistList;
	myVector *nnTemp;
	for(list< myVector >::iterator l_it = queries.begin(); l_it != queries.end(); ++l_it){
		double mindist = numeric_limits<double>::max();
		hcube.approxNNSearch(&(*l_it), &nnTemp, &mindist);
		approxNNSearchResults.push_back(nnTemp);
		mindistList.push_back(mindist);
		nnTemp = NULL;
	}



	// Exact NN
	list<double> exactNNResults = NNSearch ( &queries, &entryPoints, metric);

	//Process that iterates from every single list we've made 
	//( R neighbors, Nearest neighbor, distance <metric>, distance <true>, time <metric>, time <true> )
	//cout << "Please give an output file name." << endl;
	//string ofname;
	//getline(cin, ofname);
	ofstream outputFile("HCresults");
	list<double>::iterator mindistLSH_it = mindistList.begin();
	list<double>::iterator mindistENN_it = exactNNResults.begin();
	list<list<myVector *>>::iterator rS_it = rangeSearchResults.begin();
	list<myVector *>::iterator aNN_it = approxNNSearchResults.begin();
	for(list< myVector >::iterator l_it = queries.begin(); l_it != queries.end(); ++l_it, ++rS_it, ++aNN_it, ++mindistLSH_it, ++mindistENN_it){
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
		outputFile << "distanceHC: " << (*mindistLSH_it) << endl;
		outputFile << "distanceTrue: " << (*mindistENN_it) << endl;
		outputFile << "tLSH: " << endl;
		outputFile << "tTrue: " << endl << endl;
	}
	return 0;
}














































// Printer RS
/*int jj = 1;
for(list<list<myVector *>>::iterator li_it = rangeSearchResults.begin(); li_it != rangeSearchResults.end(); ++li_it){
	cout << jj << "- ";
	for(list<myVector *>::iterator list_it = (*li_it).begin(); list_it != (*li_it).end(); ++list_it){
		cout << (*list_it)->id <<" ";
	}
	cout << endl;
	jj++;
}*/
// Printer NN
/*for(list<myVector *>::iterator z_it = approxNNSearchResults.begin(); z_it != approxNNSearchResults.end(); ++z_it){
	cout << (*z_it)->id << " - ";
	for(vector<int>::iterator zz_it = ((*z_it)->coordinates).begin(); zz_it != ((*z_it)->coordinates).end(); ++zz_it){
		cout << " " << (*zz_it);
	}
	cout << endl;
}*/

//Entry/Query printer
/*
	for(list< myVector >::iterator it_it = entryPoints.begin(); it_it != entryPoints.end(); ++it_it ){
		cout << (*it_it).id;
		for(vector< int >::iterator itt_it = (*it_it).coordinates.begin(); itt_it != ((*it_it).coordinates).end(); ++itt_it ){
			cout << " " << (*itt_it);
		}
		cout << endl;
	}
}*/

























/*
	ifstream myfile2 ("query_small_fixed");
	// Read entry points
	if (myfile2.is_open()){
		string line;
		string::size_type sz;
		myVector *temp;
		while( getline (myfile2, line)){
			temp = new myVector();
			istringstream buf(line);
			string word; buf >> word;
			temp->id = stoi(word,&sz);
			while(buf >> word){
				int tempint = stoi(word, &sz);
				(temp->coordinates).push_back(tempint);
			}
			queries.push_back(*temp);
			temp = NULL;
		}
		myfile.close();
	}
	else cout << "Unable to open a file" << endl;
*/

/*string input, query, output;
int K, L;
if(argc<2){
	cout << "You need to provide command line arguments for the program to work." << endl;
}
if(argc<11){
	cout << "Not enough arguments. Please use the following syntax: ''" << endl;
}
if(argv[1]=="-d"){
	int size = strlen(argv[2]);
	input.assign(argv[2],size);
}
if(argv[3]=="-q"){
	query.assign(argv[4]);
}
if(argv[5]=="-k"){
	K = stoi(argv[6]);
}
if(argv[7]=="-L"){
	L = stoi(argv[9]);
}
if(argv[9]=="-o"){
	output.assign(argv[10]);
}
for(int i = 0; i <argc; ++i){
	cout << "argv[" << i << "]: " << "`" << argv[i] << "`" << endl;
}
cout << "|" << input << "|" << query << "|" << K << "|" << L << "|" << output << "|" << endl;
return 0;
*/















































/*
 	ifstream myfile ("a");
	// Read entry points
	list< myVector > entryPoints;
	if (myfile.is_open()){
		string line;
		string::size_type sz;
		myVector *temp;
		while( getline (myfile, line)){
			temp = new myVector();
			istringstream buf(line);
			string word; buf >> word;
			temp->id = stoi(word,&sz);
			while(buf >> word){
				int tempint = stoi(word,&sz);
				(temp->coordinates).push_back(tempint);
			}
			entryPoints.push_back((*temp));
			temp = NULL;
		}
		myfile.close();
	}
	else cout << "Unable to open a file" << endl;*/



















/*			Create LSH hashtables
	//TODO change it to lsh.createYoSelf(points);
	//string metric = "euclidean";
	LSH lsh;
	HashTable *htable;
	for(int j = 0; j < L; ++j){  // for each hashtable
		htable    = new HashTable(metric, entryPoints.size(), K);
		for(list< myVector >::iterator l_it = entryPoints.begin(); l_it != entryPoints.end(); ++l_it)
			htable->putVector(&(*l_it));
		//htable->printNstuff();
		lsh.addTable(htable);
	}
*/
//////////////////////////////////////////////

/* 		 		LSH functions 				*/
	// Range Search
	//Todo myVector -> myVector*
/*	list<list<myVector *>> rsList;
	list<myVector *> rsTemp;
	double _R = 0.05;
	if(_R!=0){
		for(list< myVector >::iterator l_it = queries.begin(); l_it != queries.end(); ++l_it){
			//do{
			lsh.rangeSearch(&(*l_it), &_R, &rsTemp);
			//	if(rsTemp.size()==0)
			//		_R = _R + _R/4;
			//}while(rsTemp.size()==0);
			rsList.push_back(rsTemp);
			rsTemp.clear();
		}
	}
*/
	// Printer
	/*for(list<list<myVector *>>::iterator li_it = rsList.begin(); li_it != rsList.end(); ++li_it){
		for(list<myVector *>::iterator list_it = (*li_it).begin(); list_it != (*li_it).end(); ++list_it){
			cout << (*list_it)->id <<" ";
		}
		cout << endl;
	}
	*/
	// Approximate NN
/*
	list<myVector *> nnList;
	myVector *nnTemp;
	list<double> mindistList;
	list< myVector >::iterator l_it = queries.begin();
	for(list< myVector >::iterator l_it = queries.begin(); l_it != queries.end(); ++l_it){
		double mindist = numeric_limits<double>::max();
		lsh.approxNNSearch(&(*l_it), &nnTemp, &mindist);
		nnList.push_back(nnTemp);
		mindistList.push_back(mindist);
		nnTemp = NULL;
	}
*/
	// Printer
	/*for(list<myVector *>::iterator z_it = nnList.begin(); z_it != nnList.end(); ++z_it){
		cout << (*z_it)->id << " -";
	 	for(vector<int>::iterator zz_it = ((*z_it)->coordinates).begin(); zz_it != ((*z_it)->coordinates).end(); ++zz_it){
			cout << " " << (*zz_it);
		}
		cout << endl;
	}*/

/*	list<long double> b = NNSearch ( &queries, &entryPoints, metric);

	list<double>::iterator a_it = mindistList.begin();
	for(list<long double>::iterator b_it = b.begin(); b_it != b.end(); ++b_it, ++a_it){
		cout << (*b_it) << " - " << (*a_it) << endl;
	}

	return 0;
*/
//////////////////////////////////////////////
/*  //DOULEVEI
	ifstream myfile ("input_small_fixed");
	// Read entry points
	vector<int> entryPointIDs;
	list< vector<int> > entryPoints;
	if (myfile.is_open()){
		string line;
		string::size_type sz;
		vector<int> temp;
		while( getline (myfile, line)){
			istringstream buf(line);
			string word; buf >> word;
			int tempp = stoi(word,&sz);
			entryPointIDs.push_back(tempp);
			for(; buf >> word; ){
				int tempint = stoi(word,&sz);
				temp.push_back(tempint);
			}
			entryPoints.push_back(temp);
			temp.clear();
		}
		myfile.close();
	}
	else cout << "Unable to open a file" << endl;
*/
