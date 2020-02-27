#ifndef kmean_H
#define kmean_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "myVector.hpp"
#include "HyperCube.hpp"
#include "LSH.hpp"

using namespace std;

typedef vector<myVector>::iterator vIter;
typedef vector<myVector*>::iterator cIter;

static int runKmeansXtimes = 20;

bool checkarray(int *, int, int);

class myKmeans{
	vector<myVector> *vectors;
	vector<myVector *> centroids;
	vector<int> clustAssigned;
	double *clustSi;
	int *vCountPerCluster;
	int kClusters;
	int vDimensions;
	string metric;
	double finalSi;
	bool comments;
	double duration;
  public:
	myKmeans(vector<myVector> *, string, int, int);
	~myKmeans();

//Running Kmeans
	bool runKmeans(int, int, int);
	bool runLSH(LSH *);
	bool runHC(HyperCube *);
	bool runUpdate(int );

//Initialization//
	void initialize_RandomClusters();
	void initialize_Kmeanspp();
	double findDistanceOfClosestCentroid(myVector *, int);

//Assignment//
	void assignment_Lloyds();
	void assignment_HC(HyperCube *, int *, bool *, double *);
	void assignment_LSH(LSH *, int *, bool *, double *);
	double hclsh_FindRadius();
	void complementaryLloydsAssignment(bool *);
	int findLocationOfClosestCentroid(myVector *);

//Updates//
	void update_kmeans();
	void setupKmeans();
	void update_PAM();

//Silhouette//
	void silhouette();

//General-Use//
	double getDistance(myVector *, myVector *);
	void destroyCentroids();
	void makeOutputFile(string , int , int , int );
	void printLoc();
	int printClusters();
	int printVectors();

//Comments//
	void comment_PAM_1();
	void comment_PAM_2();
	void comment_done();
	void comment_hclsh(int ,int );
};

#endif
