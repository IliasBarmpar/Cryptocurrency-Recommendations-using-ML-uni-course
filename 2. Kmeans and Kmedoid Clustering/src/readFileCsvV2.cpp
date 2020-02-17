#include "readFileCsvV2.hpp"

bool readFileCsvV2(string filename, vector<myVector> *points){
	string line;

	// Read entry points
 	ifstream myfile(filename);
	if (myfile.is_open()){
		while( getline (myfile, line) ){ //Parse lines
			functionV2(points, line);
		}
		if(points->size()==0){ cout << "Not enough vectors on the file." << endl; return 1; }

		myfile.close();
	}
	else{  cout << "Unable to open a file" << endl; return 1; }

	return 0;
}

void functionV2(vector<myVector> *points, string line){
	string::size_type sz;
	myVector temp;
	istringstream buf(line);
	string word; getline(buf,word,',');
	temp.id = stoi(word,&sz);
	while(getline(buf,word,',')){			//Parse words in each line
		double tempint = stod(word,&sz);
		(temp.coordinates).push_back(tempint);
	}
	if(!checkarrayV2(points))
		points->push_back(temp);
}

bool checkarrayV2(vector<myVector> *trainSet){
	if(trainSet->size()<2)
		return 0;

	for(vector<myVector>::iterator v = trainSet->begin(); v != trainSet->end(); ++v ){
		for(vector<myVector>::iterator v2 = (v+1); v2 != trainSet->end(); ++v2 ){
			if( checkTwoVectors( &(*v), &(*v2)) ){
				return 1;
			}
		}
	}
	return 0;
}

bool checkTwoVectors(myVector *a, myVector *b){
	vector<double>::iterator b_it = b->coordinates.begin();
	for(vector<double>::iterator a_it = a->coordinates.begin(); a_it != a->coordinates.end(); ++b_it, ++a_it){
		if((*a_it)!=(*b_it))
			return 0;
	}
	return 1;
}
