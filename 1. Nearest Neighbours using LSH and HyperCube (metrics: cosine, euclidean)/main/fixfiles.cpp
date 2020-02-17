#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int main(){
	string inname, ofname;
	cout << "Type in the file you want to fix." << endl;
	getline(cin, inname);
	cout << "Type in the name of the output file." << endl;
	getline(cin, ofname);


	ifstream  fin (inname, ios::in);
	ofstream fout (ofname, ios::out);
	// Read entry points
	int i = 0;
	string line;
	if (fin.is_open()){
		while( getline (fin, line)){
			fout << i << " ";
			fout << line;
			fout << '\n';
			i++;
		}
		fin.close();
		fout.close();

	}
	else cout << "Unable to open a file" << endl;
}
