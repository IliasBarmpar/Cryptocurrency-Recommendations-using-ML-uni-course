#ifndef rfcsvv2_H
#define rfcsvv2_H

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <list>

#include "myTweet.hpp"

using namespace std;

bool readFileCsvV2(string , vector<myVector> *);

//This function was made in order to get optimal memory management
void functionV2(vector<myVector> *, string );

bool checkTwoVectors(myVector *, myVector *);

bool checkarrayV2(vector<myVector> *);

#endif
