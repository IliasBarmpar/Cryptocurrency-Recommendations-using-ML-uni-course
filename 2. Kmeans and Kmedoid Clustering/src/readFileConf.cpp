#include "readFileConf.hpp"

bool readFileConf(string filename, int *kClusters, int *kNum, int *lNum, int *mNum, int *pNum){
	ifstream myfile(filename);
	bool flagkc = 1, flaghf = 1, flaght = 1, flagmn = 1, flagpn = 1;
	string::size_type sz;
	string line, str_kClusters , str_hashFunctions, str_hashTables, str_points, str_probes;

	if (myfile.is_open()){
		while(getline(myfile, line)){
			line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
			if(line[0] == '#' || line.empty())
				continue;
			auto delimiterPos = line.find(":");
			auto name = line.substr(0, delimiterPos);
			// Catcher
			if(name=="number_of_clusters"){
				str_kClusters = line.substr(delimiterPos + 1);
				*kClusters = stoi(str_kClusters,&sz);
				flagkc = 0;
			}else if(name=="number_of_hash_functions"){
				str_hashFunctions = line.substr(delimiterPos + 1);
				*kNum = stoi(str_hashFunctions,&sz);
				flaghf = 0;
			}else if(name=="number_of_hash_tables"){
				str_hashTables = line.substr(delimiterPos + 1);
				*lNum = stoi(str_hashTables,&sz);
				flaght = 0;
			}else if(name=="number_of_points_to_check"){
				str_points = line.substr(delimiterPos + 1);
				*mNum = stoi(str_points,&sz);
				flagmn = 0;
			}else if(name=="number_of_probes"){
				str_probes = line.substr(delimiterPos + 1);
				*pNum = stoi(str_probes,&sz);
				flagpn = 0;
			}else{
				continue;
			}
		}
	}
	else { cerr << "Couldn't open config file for reading.\n"; return 1; }

	if(flagkc) { cout << "There must be a number_of_clusters addition in your configuration file." << endl; return 1;}
	if(kClusters <= 0){ cout << "Error" << endl; return 1; }
	if(flaghf) *kNum = 4;
	if(flaght) *lNum = 5;
	if(flagmn) *mNum = 10;
	if(flagpn) *pNum = 2;
	return 0;
}
