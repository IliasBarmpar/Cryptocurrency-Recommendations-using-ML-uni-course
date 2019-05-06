#ifndef rfcsv_H
#define rfcsv_H

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <list>
#include "myVector.hpp"

using namespace std;

bool readFileCsv(string , vector<myVector> *);

//This function was made in order to get optimal memory management
void function(vector<myVector> *, string );

#endif
