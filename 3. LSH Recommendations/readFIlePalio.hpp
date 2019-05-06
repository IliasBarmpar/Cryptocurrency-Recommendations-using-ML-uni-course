#ifndef rf_H
#define rf_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <list>

#include "myTweet.hpp"

using namespace std;

void readFile(string filename, list<myVector> *points){
 	ifstream myfile (filename);
	// Read entry points
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
			points->push_back((*temp));
			temp = NULL;
		}
		myfile.close();
	}
	else cout << "Unable to open a file" << endl;
}

#endif
