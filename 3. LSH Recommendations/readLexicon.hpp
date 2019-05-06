#ifndef rdlx_hpp
#define rdlx_hpp

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <list>
#include "myLexicon.hpp"

void functionA(vector<myLexiconAMember> *input, string line){
	myLexiconAMember temp;
	istringstream buff(line);

	buff >> temp.word;
	buff >> temp.sentscore;

	input->push_back(temp);
}

bool readLexiconA(string filename, vector<myLexiconAMember> *input){
	string::size_type sz;
	string line,word;

	// Read entry points
	ifstream myfile(filename);
	if (myfile.is_open()){
		while( getline (myfile, line) ){ //Parse lines
//			cout << line << endl;
//			getchar();
			functionA(input, line);
		}
		if(input->size()==0){ cout << "Not enough vectors on the file." << endl; return 1; }

		myfile.close();
	}
	else{   cout << "Unable to open a file" << endl; return 1;}

	return 0;
}

void functionK(vector<myLexiconKMember> *input, string line){
	string word;
	myLexiconKMember temp;
	istringstream buff(line);

	while(getline(buff,word,'\t')){
		temp.words.push_back(word);
	}
//	while(buff >> word)
//		temp.words.push_back(word);

//	cout << "|in|" <<endl;
//	for(vector<string>::iterator it = temp.words.begin(); it != temp.words.end(); ++it){
//		cout << *it << endl;
//	}
//	cout  << endl;
//	getchar();

	input->push_back(temp);
}

bool readLexiconK(string filename, vector<myLexiconKMember> *input){
	string::size_type sz;
	string line,word;

	// Read entry points
	ifstream myfile(filename);
	if (myfile.is_open()){
		while( getline (myfile, line) ){ //Parse lines
			functionK(input, line);
		}
		if(input->size()==0){ cout << "Not enough vectors on the file." << endl; return 1; }

		myfile.close();
	}
	else{ cout << "Unable to open a file" << endl; return 1;}

	return 0;
}
#endif
