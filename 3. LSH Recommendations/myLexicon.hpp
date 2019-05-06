#ifndef myL_hpp
#define myL_hpp


#include <iostream>
#include <vector>

using namespace std;

struct myLexiconAMember{
	string word;
	double sentscore;

	~myLexiconAMember(){}
};

struct myLexiconKMember{
	vector<string> words;

	~myLexiconKMember(){
		words.clear();
		vector<string>().swap(words);
	}
};

#endif
