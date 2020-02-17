#ifndef myV_H
#define myV_H

#include <iostream>
#include <vector>

using namespace std;

struct myVector{
	vector<double> coordinates;
	int id;

	~myVector(){
		coordinates.clear();
		vector<double>().swap(coordinates);
	}
};

#endif
/*
class myKmeans{
	vector<myVector> *vectors;
	vector<myVector *> centroids;
	vector<int> clustAssigned;
	int kClusters;
	int vDimensions;
	string metric;
	bool comments;
  public:
	myKmeans(vector<myVector> *, string , int , int );
	~myKmeans();
	void runKmeans();
	//Initializations
	void initialize_RandomClusters();
	void initialize_Kmeanspp();
		double findDistanceOfClosestCentroid(myVector *vec, int range);
	//Assignments
	void assignment_Lloyds();
	void assignment_HC(HyperCube *hc, int *pA, double *radius);
	void assignment_LSH(LSH *lsh, int *pA, double *radius);
		double lsh_or_hc_FindRadius();
		void complementaryAssignmentLloyds();
	int findLocationOfClosestCentroid(myVector *vec);
	//Updates
	void update_kmeans();
		void setupKmeans();
	void update_PAM();
	//Silhouette
	void silhouette();
	//General-Use
	void destroyCentroids();
	void comment_PAM_1();
	void comment_PAM_2();
	void comment_done();
};
*/
