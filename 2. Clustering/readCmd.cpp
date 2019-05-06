#include "readCmd.hpp"

bool readCmd(int argc, char **argv, string *inputfile, string *conffile, string *outputfile,
			 string *metric, int *inopt, int *asopt, int *upopt){
	int opt;
	string::size_type sz;
	bool inputflag = 1, confflag = 1, Mflag = 1, outputflag = 1, inflag = 1, asflag = 1, upflag = 1;

	/* Command-line argument parser using getopt */
	while( (opt = getopt(argc, argv, "i:c:o:d:n:a:u:")) != -1){
		switch (opt){
			case 'i':
				inputflag = 0;
				*inputfile = optarg;
				break;
			case 'c':
				confflag = 0;
				*conffile = optarg;
				break;
			case 'o':
				outputflag = 0;
				*outputfile = optarg;
				break;
			case 'd':
				Mflag = 0;
				*metric = optarg;
				break;
			case 'n':
				inflag = 0;
				*inopt = stoi(optarg, &sz);
				if(*inopt >= 3 || *inopt <= 0)
					*inopt = 1;
				break;
			case 'a':
				asflag = 0;
				*asopt = stoi(optarg, &sz);
				if(*asopt >= 4 || *asopt <= 0)
					*asopt = 1;
				break;
			case 'u':
				upflag = 0;
				*upopt = stoi(optarg, &sz);
				if(*upopt >= 3 || *upopt <= 0)
					*upopt = 1;
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
//TODO:	if(checktypes) *metric = "euclidean";
	if(Mflag) *metric = "euclidean";
	if(inflag) *inopt = 1;
	if(asflag) *asopt = 1;
	if(upflag) *upopt = 1;
	if(inputflag) { cout << "Please type in the location of the dataset." << endl;            getline(cin, *inputfile);}
	if(confflag)  { cout << "Please type in the location of the configuration file." << endl; getline(cin, *conffile); }
	if(outputflag){ cout << "Please give an output file name." << endl;                       getline(cin, *outputfile); }
	return 0;
}
