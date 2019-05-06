#ifndef readcmd_H
#define readcmd_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>
#include <list>

#include "myTweet.hpp"

using namespace std;

bool readCmdRec(int , char **, string *, string *, bool *);

#endif
