#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <random>
#include <chrono>
#include <string.h>
#include "ExactNN.hpp"
#include "readFile.hpp"

using namespace std;

int main(int argc, char* argv[]){
	srand(time(nullptr));

/*			     Read Input 				*/
	string inputfile1 = "input/input_small_fixed";
	list< myVector > entryPoints;
	readFile(inputfile1, &entryPoints);

/* 			    Read Queries 				*/
	string inputfile2 = "input/query_small_fixed"; //TODO User input
	list< myVector > queries;
	readFile(inputfile2, &queries);


	// EXACT NN
	string metric = "euclidean"; //TODO cin
	list<double> exactNNResults = NNSearch ( &queries, &entryPoints, metric);
	for(list<double>::iterator b_it = exactNNResults.begin(); b_it != exactNNResults.end(); ++b_it)
		cout << (*b_it) << endl;

	return 0;
}

