#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <random>
#include <chrono>
#include <string.h>
#include <algorithm>

#include "kmeans.hpp"
#include "readCmdRec.hpp"
#include "readFileCsv.hpp"
#include "readFileCsvV2.hpp"
#include "readFileConf.hpp"

using namespace std;

int main(int argc, char* argv[]){
	srand(time(nullptr));
	string inputfile, conffile, outputfile, metric; //Read Cmd
	int inopt, asopt, upopt;						//Read Cmd
	vector<myVector> trainSet;						//Read Csv
	int kClusters, kNum, lNum, mNum, pNum; 		//Read Conf

	/* Read Cmd	*/
//	if( readCmd(argc, argv, &inputfile, &conffile, &outputfile, &metric, &inopt, &asopt, &upopt) ) return -1;

	/* Read Input */
	if( readFileCsv(inputfile, &trainSet) ) return -1;

	/* Read Conf */
	if( readFileConf(conffile, &kClusters, &kNum, &lNum, &mNum, &pNum) ) return -1;



	/* Kmeans */							 	   //Trust that all vectors are made equally and correctly
	myKmeans kMeans( &trainSet, metric, kClusters, trainSet[0].coordinates.size() );
	kMeans.runKmeans(inopt, asopt, upopt);


	/* Start printing */
	kMeans.makeOutputFile(outputfile, inopt, asopt, upopt);


	/* Free Memory  */
	for(vector<myVector>::iterator tS_it = trainSet.begin(); tS_it != trainSet.end(); ++tS_it)
		(*tS_it).~myVector();
	return 0;
}













































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

//cout << "size=" << trainSet.size() << endl;
//for(vector<myVector>::iterator v = trainSet.begin(); v != trainSet.end(); ++v ){
//	cout << v->id <<endl;
//}
//return 0;
