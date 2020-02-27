#include "readCmdRec.hpp"
#include <stdio.h>
#include <string.h>

bool readCmdRec(int argc, char **argv, string *inputfile, string *outputfile, bool *val){
	/* Command-line argument parser */
	if(argc==5){
		if( !strcmp(argv[1],"-d") && !strcmp(argv[3],"-o")){
			*inputfile  =  argv[2];
			*outputfile =  argv[4];
			*val = 0;
		}else{
			cout << "Incorrect command line arguments" << endl; return -1;
		}
	}else if(argc==6){
		if( !strcmp(argv[1],"-d") && !strcmp(argv[3],"-o") && !strcmp(argv[5],"-validate")){
			*inputfile  =  argv[2];
			*outputfile =  argv[4];
			*val = 1;
		}else{
			cout << "Incorrect command line arguments" << endl; return -1;
		}
	}else{
		cout << "Incorrect command line arguments" << endl; return -1;
	}


	/* Command-line argument parser using getopt */
/*
	int opt;
	bool inputflag = 1, outputflag = 1, valflag = 1;
	while( (opt = getopt(argc, argv, "d:o:v")) != -1){
		switch (opt){
			case 'd':
				inputflag = 0;
				*inputfile = optarg;
				break;
			case 'o':
				outputflag = 0;
				*outputfile = optarg;
				break;
			case 'v':
				valflag = 0;
				*val = 1;
				break;
			case '?':
				if (optopt == 'c')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
				return 1;
			default:
				cout << "Abort." << endl;
				abort ();
		}
	}
	if(inputflag) { cout << "Please type in the location of the dataset." << endl;            getline(cin, *inputfile);}
	if(outputflag){ cout << "Please give an output file name." << endl;                       getline(cin, *outputfile); }
	if(valflag){ *val = 0; }
*/
	return 0;
}
