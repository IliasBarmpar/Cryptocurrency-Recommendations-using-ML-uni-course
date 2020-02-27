#include "readFileCsv.hpp"

bool readFileCsv(string filename, vector<myVector> *points){
	string line;

	// Read entry points
 	ifstream myfile(filename);
	if (myfile.is_open()){
		while( getline (myfile, line) ){ //Parse lines
			function(points, line);
		}
		if(points->size()==0){ cout << "Not enough vectors on the file." << endl; return 1; }

		myfile.close();
	}
	else{  cout << "Unable to open a file" << endl; return 1; }

	return 0;
}

void function(vector<myVector> *points, string line){
	string::size_type sz;
	myVector temp;
	istringstream buf(line);
	string word; getline(buf,word,',');
	temp.id = stoi(word,&sz);
	while(getline(buf,word,',')){			//Parse words in each line
		double tempint = stod(word,&sz);
		(temp.coordinates).push_back(tempint);
	}
	points->push_back(temp);
}
