#include "readFile.hpp"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <algorithm>
#include <cstdio>

void readFile(string filename, list<myVector> *points){
	/*
	cout << "fn: " << filename << endl;
	char szTmp[32];
	char pBuf[500];
	sprintf(szTmp, "/proc/%d/exe", getpid());
	int bytes = min(readlink(szTmp, pBuf, 500), (ssize_t)499);
	if(bytes >= 0)
		pBuf[bytes] = '\0';
	cout << "pBuf[500]: " <<  pBuf << endl;
	*/

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


