#ifndef inp_hpp
#define inp_hpp

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <list>

#include "myTweet.hpp"

using namespace std;

void addUser(vector<myUser> *users, int id){
	myUser tempuser;
	tempuser.id = id;
	users->push_back(tempuser);
}

void function(vector<myUser> *users, vector<myTweet> *tweets, string line, int *tempint, int i){
	string word;
	string::size_type sz;
	myTweet temptweet;
	int id;
	istringstream buff(line);

	//Get User Id
	buff >> id;
	if(id != *tempint){
		addUser(users, id);
		*tempint = id;
	}

	temptweet.userid = id;
	//Get Tweet Id
	buff >> temptweet.id;

	//Parse words in each line
	while(buff >> word)
		temptweet.tweet.push_back(word);

	vector<myUser>::iterator us_it = (users->end()-1);
	(us_it->tweetIDs).push_back(i);
	tweets->push_back(temptweet);

}









bool readInputFile(string filename, vector<myUser> *users, vector<myTweet> *tweets){
	string::size_type sz;
	string line,word;

	// Read entry points
	ifstream myfile(filename);
	if (myfile.is_open()){
		// Get vectors
		int tempint = -1;
		int i = 0;
		while( getline (myfile, line) ){ //Parse lines
			function(users, tweets, line, &tempint, i);
			i++;
		}
		if(users->size()==0){ cout << "Not enough vectors on the file." << endl; return 1; }

		myfile.close();
	}
	else{  cout << "Unable to open a file" << endl; return 1; }

	return 0;
}

#endif

/*
void function(vector<myTweet> *input, string line){
	string word;
	string::size_type sz;
	myTweet temp;
	istringstream buff(line);
	int tempint;

	//Get User Id
	buff >> temp.userid;

	//Get Tweet Id
	buff >> temp.tweetid;

	//Parse words in each line
	while(buff >> word)
		temp.tweet.push_back(word);

	input->push_back(temp);
}

bool readInputFile(string filename, vector<myTweet> *input, int *P){
	string::size_type sz;
	string line,word;

	// Read entry points
	ifstream myfile(filename);
	if (myfile.is_open()){
		// Get P
		getline (myfile, line);{
			istringstream buff(line);
			getline(buff,word,':');
			buff >> *P;
		}

		// Get vectors
		while( getline (myfile, line) ){ //Parse lines
			function(input, line);
		}
		if(input->size()==0){ cout << "Not enough vectors on the file." << endl; return 1; }

		myfile.close();
	}
	else{  cout << "Unable to open a file" << endl; return 1; }

	return 0;
}
*/
