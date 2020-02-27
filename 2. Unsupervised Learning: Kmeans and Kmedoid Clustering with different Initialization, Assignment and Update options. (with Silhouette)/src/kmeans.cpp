#include "kmeans.hpp"

extern int runKmeansXtimes;

bool checkarray(int *array, int range, int value){
//Checks whether an int array in the given range already contains a value
	for(int i = 0; i < range; ++i)
		if(array[i]==value)
			return 1;
	return 0;
}


myKmeans::myKmeans(vector<myVector> *vec, string m, int kCl, int vD){
	finalSi = 0;
	comments = 1;
	metric = m;
	vectors = vec;
	kClusters = kCl;
	vDimensions = vD;
	duration = 0;
	vCountPerCluster = new int[kClusters]();
	clustSi = new double[kClusters]();
	clustAssigned.insert(clustAssigned.end(), vec->size(), -1);
}

myKmeans::~myKmeans(){}

//Running Kmeans
bool myKmeans::runKmeans(int inopt, int asopt, int upopt){
	auto start = std::chrono::steady_clock::now();
	//Initialization
	if(inopt==1)
		initialize_RandomClusters();
	else if(inopt==2)
		initialize_Kmeanspp();
	else
		return 1;
	cout << "Initialization completed." << endl;
	if(asopt==1){
		for(int i = 0; i < runKmeansXtimes; ++i){
			assignment_Lloyds();
			if(i==0 && upopt==1) setupKmeans();
			runUpdate(upopt);
		}
	}else if(asopt == 2){
		LSH lsh(vectors, metric, 4, 4); //TODO to initialization ksexwrista apo ton constructor...!
		for(int i = 0; i < runKmeansXtimes; ++i){
			runLSH(&lsh);
			if(i==0 && upopt==1) setupKmeans();
			runUpdate(upopt);
			//printClusters(); getchar();
		}
	}else if(asopt == 3){
		HyperCube hcube(metric,vectors, 10, 2 );
		for(int i = 0; i < runKmeansXtimes; ++i){
			runHC(&hcube);
			if(i==0 && upopt==1) setupKmeans();
			runUpdate(upopt);
		}

	}
	cout << "Assign + Update complited." << endl;
	auto end = std::chrono::steady_clock::now();
	duration = std::chrono::duration<double>(end-start).count(); //Computes in seconds.

	silhouette();
//		destroyCentroids();
}

bool myKmeans::runLSH(LSH *lsh){
	int vectorCount = vectors->size();
	bool flagAssigns[vectorCount] = {0};
	int flagWhile = 1, sumOfAssignedTotal = 0, pointsAssignedThisRun = 0, minimumRuns = 5, pointsLeft = vectorCount;
	double limit = vectorCount * 0.95, percentageAssigned = 0.0;
	double radius = hclsh_FindRadius();

	//Conditions to terminate Range Search
		//1) We've done at least 5 runs
		//2) 95% of points have been assigned
		//3) Percentage of points assigned is too low
	do{
		//Calculations (the names of the parameters speak for themselves)
		pointsAssignedThisRun = 0;
		assignment_LSH(lsh, &pointsAssignedThisRun, flagAssigns, &radius);
		radius = radius*2;
		pointsLeft -= pointsAssignedThisRun;
		sumOfAssignedTotal += pointsAssignedThisRun;

		// Check if we've met the condition set to terminate
		if((--minimumRuns)==0)
			if( (vectorCount - pointsLeft) < limit)
				flagWhile = 0;
	}while(flagWhile);
	cout << "sum of points assigned=" << sumOfAssignedTotal << endl;
	// For any vector that wasn't assigned to a cluster, run Lloyd's Assignment
	complementaryLloydsAssignment(flagAssigns);
}

bool myKmeans::runHC(HyperCube *hc){
	int vectorCount = vectors->size();
	bool flagAssigns[vectorCount] = {0};
	int flagWhile = 1, sumOfAssignedTotal = 0, pointsAssignedThisRun = 0, minimumRuns = 5, pointsLeft = vectorCount;
	double limit = vectorCount * 0.95, percentageAssigned = 0.0;
	double radius = hclsh_FindRadius();

	//Conditions to terminate Range Search
		//1) We've done at least 5 runs
		//2) 95% of points have been assigned
		//3) Percentage of points assigned is too low
	do{
		//Calculations (the names of the parameters speak for themselves)
		pointsAssignedThisRun = 0;
		assignment_HC(hc, &pointsAssignedThisRun, flagAssigns, &radius);
		radius = radius*2;
		pointsLeft -= pointsAssignedThisRun;
		sumOfAssignedTotal += pointsAssignedThisRun;

		// Check if we've met the condition set to terminate
		if((--minimumRuns)==0)
			if( (vectorCount - pointsLeft) < limit)
				flagWhile = 0;
	}while(flagWhile);
	cout << "sum of points assigned=" << sumOfAssignedTotal << endl;
	// For any vector that wasn't assigned to a cluster, run Lloyd's Assignment
	complementaryLloydsAssignment(flagAssigns);
}

bool myKmeans::runUpdate(int upopt){
	if(upopt==1){
		update_kmeans();
	}else if(upopt==2){
		update_PAM();
		cout << "PAM completed." << endl;
	}else{
		return 1;
	}
}

//Initialization//
void myKmeans::initialize_RandomClusters(){
	//Initialize distinct random numbers
	int chosenrands[kClusters];
	for(int i = 0; i < kClusters; ++i){
		int tempint;
		do{
			tempint = rand()%(vectors->size());
		}while( checkarray(chosenrands, i, tempint) );
		chosenrands[i] = tempint;
	}

	//Set Clusters to those distinct, randomly chosen vectors
	for(int l = 0; l < kClusters; ++l){
		vIter vc = vectors->begin();
		advance(vc, chosenrands[l]);
		centroids.push_back( &(*vc) );
	}
}

void myKmeans::initialize_Kmeanspp(){
	double P[vectors->size()] = {0.0};
	int centroidsLeft = kClusters; //Centroids to be assigned

	//Choose a vector at random to be a centroid
	int tempint = rand()%(vectors->size()-1);
	vIter vc = vectors->begin();
	advance(vc, tempint);
	centroids.push_back( &(*vc) );
	centroidsLeft--;

	//For the rest of the centroids
	while(centroidsLeft){
		//Set P: 'P holds the partial sums', we make sure to set P[0] before going into the loop to avoid problems with P[i-1].
		vc = vectors->begin();
		P[0] = findDistanceOfClosestCentroid(&(*vc), kClusters-centroidsLeft);
		P[0] = P[0]*P[0];
		++vc;
		for(int i = 1; vc != vectors->end(); ++vc, ++i){
			double dist = findDistanceOfClosestCentroid(&(*vc), (kClusters-centroidsLeft) );
			P[i] = P[i-1] + dist*dist;
		}

		//Choose C: 'Pick new centroid with weighted probability'
		unsigned seed = chrono::system_clock::now().time_since_epoch().count();
		default_random_engine generator(seed);
		uniform_real_distribution<> dis(0.0,P[vectors->size()-1]);
		double pick = dis(generator);
		int i = 0;
		for(; i < vectors->size(); i++){
			if(pick < P[i]){
				break;
			}
		}

		//Finally: i holds the location of the new chosen centroid
		vc = vectors->begin();
		advance(vc, i);
		centroids.push_back( &(*vc) );
		centroidsLeft--;
	}
}

double myKmeans::findDistanceOfClosestCentroid(myVector *vec, int range){
	//Calculates distance to closest centroid in the range provided
	double min = numeric_limits<double>::max();
	cIter cl_it = centroids.begin();

	for(int i = 0; i < range; ++i, ++cl_it){
		double dist = getDistance( vec,  (*cl_it) );
		if(dist < min)
			min = dist;
	}
	return min;
}

//Assignment//
void myKmeans::assignment_Lloyds(){
	//For every vector find the closest centeroid and keep it's location (clustAssigned[])
	for(vIter vc = vectors->begin(); vc != vectors->end(); ++vc )
		clustAssigned[vc->id-1] = findLocationOfClosestCentroid( &(*vc));
}

void myKmeans::assignment_HC(HyperCube *hc, int *pA,bool *flagAssigns, double *radius){
	int centroidloc = 0;
	vector<myVector *> rsTemp;

	//For every centroid get a list of vectors from range search and then assign them to the centroid
	for(cIter ct_it = centroids.begin(); ct_it != centroids.end(); ++ct_it, ++centroidloc){
		hc->rangeSearch( (*ct_it), radius, &rsTemp);
		for(cIter list_it = rsTemp.begin(); list_it != rsTemp.end(); ++list_it){
			if(!flagAssigns[(*list_it)->id-1]){
				flagAssigns[(*list_it)->id-1] = 1;
				clustAssigned[(*list_it)->id-1] = centroidloc;
				(*pA)++;
			}else{
				//TODO: compare the two
			}
		}
		vector<myVector *>().swap(rsTemp); //Emptry vector and free it's space
	}
}

void myKmeans::assignment_LSH(LSH *lsh, int *pA, bool *flagAssigns, double *radius){
	int centroidloc = 0;
	vector<myVector *> rsTemp;

	//For every centroid get a list of vectors from range search and then assign them to the centroid
	for(cIter ct_it = centroids.begin(); ct_it != centroids.end(); ++ct_it, ++centroidloc ){
		lsh->rangeSearch( (*ct_it), radius, &rsTemp);
		for(cIter list_it = rsTemp.begin(); list_it != rsTemp.end(); ++list_it){
			if(!flagAssigns[(*list_it)->id-1]){
				flagAssigns[(*list_it)->id-1] = 1;
				clustAssigned[(*list_it)->id-1] = centroidloc;
				(*pA)++;
			}else{
				//TODO: compare the two
			}
		}
		vector<myVector *>().swap(rsTemp); //Emptry vector and free it's space
	}
}

double myKmeans::hclsh_FindRadius(){
	//Parses centroids in a factorial fashion and keeps the minimum distance
	double min = numeric_limits<double>::max();
	for(cIter ct1_it = centroids.begin(); ct1_it != centroids.end(); ++ct1_it ){
		for(cIter ct2_it = (ct1_it+1); ct2_it != centroids.end(); ++ct2_it ){
			double dist = getDistance( (*ct1_it),  (*ct2_it) );
			if(dist < min){
				min = dist;
			}
		}
	}
	return min;
}

void myKmeans::complementaryLloydsAssignment(bool *flagAssigns){
	//For every vector, if it wasn't assigned by the Range Search, assign the closest center using Lloyd's algorithm
	int j = 0;
	for(vIter vec_it = vectors->begin(); vec_it != vectors->end(); ++vec_it, ++j ){
		if(!flagAssigns[j])
			clustAssigned[j] = findLocationOfClosestCentroid( &(*vec_it));
	}
}

int myKmeans::findLocationOfClosestCentroid(myVector *vec){
	//For a given vector find the location of the closests centroid and return it
	double min = numeric_limits<double>::max();
	int temploc = -1; int centroidloc = 0;

	for(cIter ct_it = centroids.begin(); ct_it != centroids.end(); ++ct_it, ++centroidloc){
		double dist = getDistance( vec, (*ct_it) );
		if(dist < min){
			min = dist;
			temploc = centroidloc;
		}
	}
	return temploc;
}

//Updates//
void myKmeans::update_kmeans(){
	for(int l = 0; l < kClusters; ++l)
		vCountPerCluster[l] = 0;

	//Set clusters to 0
	for(cIter cl_it = centroids.begin(); cl_it != centroids.end(); ++cl_it )
		for(int i = 0; i < vDimensions; ++i)
			(*cl_it)->coordinates[i] = 0;

	//Increment sum of vectors per cluster and then add the vector to that cluster
	for(vIter vc = vectors->begin(); vc != vectors->end(); ++vc){
		vCountPerCluster[clustAssigned[vc->id-1]]++; //This (clustAssigned[vc->id]) cluster has vCount[] vectors

		int j = 0;
		for(vector<double>::iterator p_it = (vc->coordinates).begin(); p_it != (vc->coordinates).end(); ++j, ++p_it)
			centroids[clustAssigned[vc->id-1]]->coordinates[j] = centroids[clustAssigned[vc->id-1]]->coordinates[j] + (*p_it);
	}

	//Divide with sum of vectors in each cluster
	for(cIter ce = centroids.begin(); ce != centroids.end(); ++ce )
		for(int k = 0; k < vDimensions; ++k)
			(*ce)->coordinates[k] = (*ce)->coordinates[k] / vCountPerCluster[(*ce)->id];
}

void myKmeans::setupKmeans(){ //TODO change it so it doesnt have to be done after the first assignment
	//Makes sure we have allocated enough memory for the new vectors that are going to be
	//made by update_kmeans, this should only be done once and only after the first assignment
	myVector *temp;
	for(int i = 0; i < kClusters; ++i){
		temp = new myVector();

		for(int i = 0; i < vDimensions; ++i)
			temp->coordinates.push_back(0.0);
		temp->id = i;

		centroids[i] = temp;
		temp = NULL;
	}
}

void myKmeans::update_PAM(){
	//Define and Initialize Arrays
	int minmedloc[kClusters];
	double minmedoid[kClusters];
	for(int j = 0; j < kClusters; ++j){
		minmedloc[j] = -1;
		minmedoid[j] = numeric_limits<double>::max();
		vCountPerCluster[j] = 0;
	}

	//Find Medoids for each Cluster
	if(comments) comment_PAM_1();
	for(vIter vc = vectors->begin(); vc != vectors->end(); ++vc){
		double temp = 0.0;
		for(vIter vc2 = vectors->begin(); vc2 != vectors->end(); ++vc2){
			if(clustAssigned[vc->id-1] == clustAssigned[vc2->id-1])
				if(vc->id != vc2->id)
					temp += getDistance( &(*vc), &(*vc2) );
		}

		int x = clustAssigned[vc->id-1]; //Assigned cluster
		vCountPerCluster[x]++;			 //Increase vector counter
		if( temp < minmedoid[x] ){
			minmedoid[x] = temp;
			minmedloc[x] = vc->id;
		}
	}
	if(comments) comment_done();

	//Set new cluster centers
	if(comments) comment_PAM_2();
	int i = 0;
	for(cIter cent = centroids.begin(); cent != centroids.end(); ++cent, ++i){
		vIter vc = vectors->begin(); advance(vc, (minmedloc[i]-1));
		(*cent) = &(*vc);
	}
	if(comments) comment_done();
}

//Silhouette//
void myKmeans::silhouette(){
	int temploc, templocsec, arraySize = vectors->size();
	double min, secmin, centroidPos;
	int secondlocation[arraySize] = {0};
	//double clustSi[kClusters] = {0.0};
	double silSi[arraySize]   = {0.0};
	double silAi[arraySize]   = {0.0};
	double silBi[arraySize]   = {0.0};
	vIter vc, vc2;
	for(int i = 0; i < kClusters; ++i)
		vCountPerCluster[i] = 0;

	//For each vector find it's two closest centroids
	for(vc = vectors->begin(); vc != vectors->end(); ++vc){
		//Initializations
		min = numeric_limits<double>::max(), secmin = numeric_limits<double>::max(); temploc = -1, templocsec = -1, centroidPos = 0;
		//Calculate distance to each centroid and keep the two minimum distances
		for(cIter cn = centroids.begin(); cn != centroids.end(); ++cn, ++centroidPos ){
			double dist = getDistance( &(*vc), (*cn) );
			if(dist < min){
				secmin = min;
				min = dist;
				templocsec = temploc;
				temploc = centroidPos;
			}else if( (dist < secmin) && dist != min){
				secmin = dist;
				templocsec = centroidPos;
			}
		}
		clustAssigned[vc->id-1] = temploc;
		secondlocation[vc->id-1] = templocsec;
	}
//		cout << "1" << endl; getchar();
//		//TempPrint
//		for(int i = 0; i < arraySize; ++i){
//			cout <<  "clustAssigned[" << i << "]=" << clustAssigned[i] << " - ";
//			cout <<  "secondlocation[" << i << "]=" << secondlocation[i] << endl;
//		}

	//For each vector calculate their a(i) == silAi[] and their b(i) == silBi[]
	for(vc = vectors->begin(); vc != vectors->end(); ++vc){
		int n1=0,n2=0;
		for(vc2 = vectors->begin(); vc2 != vectors->end(); ++vc2){
			if(clustAssigned[vc->id-1]==clustAssigned[vc2->id-1]) 	//Anhkei sto prwto kalutero cluster
				if(vc->id != vc2->id){
					silAi[vc->id-1] += getDistance( &(*vc) , &(*vc2));	//a(i)
					n1++;
				}

			if(secondlocation[vc->id-1]==clustAssigned[vc2->id-1]){	//Anhkei sto deutero kalutero cluster
				silBi[vc->id-1] += getDistance( &(*vc) , &(*vc2));	//b(i)
				n2++;
			}
		}
		silAi[vc->id-1] = silAi[vc->id-1] / n1;
		silBi[vc->id-1] = silBi[vc->id-1] / n2;
	}
//		cout << "2" << endl; getchar();
//		//TempPrint
//		for(int i = 0; i < arraySize; ++i){
//			cout <<  "silAi[" << i << "]=" << silAi[i] << " - ";
//			cout <<  "silBi[" << i << "]=" << silBi[i] << endl;
//		}



	//For each vector calculate their s(i) == silSi[] and add it to the assigned cluster while keeping track of how many points are per cluster
	for(int n = 0; n != arraySize; ++n){
		if(silAi[n] <= silBi[n])
			silSi[n] = (silBi[n] - silAi[n])/silBi[n];
		else //This shouldn't happen with our dataset
			silSi[n] = (silBi[n] - silAi[n])/silAi[n];

		clustSi[clustAssigned[n]] += silSi[n];
		vCountPerCluster[clustAssigned[n]]++;
	}
//		cout << "3" << endl; getchar();
//		//TempPrint
//		for(int i = 0; i < arraySize; ++i)
//			cout <<  "silSi[" << i << "]=" << silSi[i] << endl;
//
//		cout << "4" << endl; getchar();
//		//TempPrint
//		for(int i = 0; i < kClusters; ++i){
//			cout <<  "clustSi[" << i << "]=" << clustSi[i] << " - ";
//			cout <<  "vCounterPerCluster[" << i << "]=" << vCounterPerCluster[i] << endl;
//		}


	//Finally for each cluster divide by the sum of vectors that exist in said cluster in order to get the mean and add that to the final calculation.
	for(int i = 0; i < kClusters; i++){
		clustSi[i] = clustSi[i]/vCountPerCluster[i];
		finalSi += clustSi[i];
	}
//		cout << "5" << endl; getchar();
//		cout << finalSi << endl;
}

//General-Use//
double myKmeans::getDistance(myVector *p, myVector *q){
	if(metric=="euclidean"){
		return euclideanDistOfTwoVectors( &(p->coordinates), &(q->coordinates));
	}else if(metric=="cosine"){
		return cosineDistOfTwoVectors( &(p->coordinates), &(q->coordinates));
	}else{
		cout << "Something went wrong on getDistance, unknown metric." << endl;
		exit(1);
	}
}

void myKmeans::destroyCentroids(){
	//destroying centroids for 3 mins straight
	for(cIter c_it = centroids.begin(); c_it != centroids.end(); ++c_it){
		(*c_it)->~myVector();
		delete (*c_it);
	}
}

void myKmeans::makeOutputFile(string outputfile, int inopt, int asopt, int upopt){
	ofstream outputFile(outputfile);
	outputFile << "Algorithm: I" << inopt << "A" << asopt << "U" << upopt << endl;
	outputFile << "Metric: " << metric << endl;
	int i = 1;
	for(cIter cn = centroids.begin(); cn != centroids.end(); ++cn, ++i){
		outputFile << "CLUSTER-: " << i << "  ";
		outputFile << "{size:  " << vCountPerCluster[i-1] << ", centroid: "; //TODO:size
		if(upopt==1){ 	//Kmeans
			vector<double>::iterator d_it = ((*cn)->coordinates).begin();
			outputFile << "[" << (*d_it);
			for(; d_it != ((*cn)->coordinates).end(); ++d_it )
				outputFile << ", " << (*d_it);
			outputFile << "]" << endl;
		} else {		//PAM
			outputFile << (*cn)->id << "}" << endl;
		}
	}
	outputFile << endl << "clustering_time: " << duration << endl;
	outputFile << "Silhouette: [";
	for(int i = 0; i < kClusters; ++i)
		outputFile << clustSi[i] << ",";
	outputFile << finalSi << "]" << endl;

	outputFile.close();
}

void myKmeans::printLoc(){
	for(vector<int>::iterator cl_it = clustAssigned.begin(); cl_it != clustAssigned.end(); ++cl_it )
		cout << (*cl_it) <<endl;
}

int myKmeans::printClusters(){
	if(centroids.empty()){
		cout << "Error: There are no clusters." << endl;
		return 1;
	}

	for(cIter it_it = centroids.begin(); it_it != centroids.end(); ++it_it ){
		cout << endl << (*it_it)->id;
		for(vector<double>::iterator itt_it = ((*it_it)->coordinates).begin(); itt_it != ((*it_it)->coordinates).end(); ++itt_it ){
			cout << " " << (*itt_it);
		}
		cout << endl;
	}
	return 0;
}

int myKmeans::printVectors(){
	if(vectors==NULL)
		return 1;
	for(vIter it_it = vectors->begin(); it_it != vectors->end(); ++it_it ){
		cout << (*it_it).id;
		for(vector< double >::iterator itt_it = (it_it->coordinates).begin(); itt_it != (it_it->coordinates).end(); ++itt_it ){
			cout << " " << (*itt_it);
		}
		cout << endl;
	}
	return 0;
}

//Comments//
void myKmeans::comment_PAM_1(){
	cout << "Finding medoids...";
}

void myKmeans::comment_PAM_2(){
	cout << "Setting new centroids";
}

void myKmeans::comment_done(){
	cout << " finished!"<< endl;
}

void myKmeans::comment_hclsh(int pointsAssignedThisRun,int percentageAssigned){
	cout << "pointsAssignedThisRun=" << pointsAssignedThisRun << endl;
	cout << "percentage="  << percentageAssigned <<"%" << endl << endl;
}
