#include "ExactNN.hpp"


list<double> NNSearch (list< myVector > *queries, list< myVector > *entryPoints, string metric){
	list<double> NNdistances;
	for(list<myVector>::iterator q_it = queries->begin(); q_it != queries->end(); ++q_it){
		long double mintemp = numeric_limits<long double>::max();
		for(list<myVector>::iterator p_it = entryPoints->begin(); p_it != entryPoints->end(); ++p_it){
			long double tempdist = 0;
			if(metric == "euclidean"){
				vector<int>::iterator pi_it = (*p_it).coordinates.begin();
				for( vector<int>::iterator qi_it = (*q_it).coordinates.begin(); qi_it != (*q_it).coordinates.end(); ++qi_it, ++pi_it)
					tempdist += ( (*qi_it) - (*pi_it) )*( (*qi_it) - (*pi_it) );

				tempdist = sqrt( tempdist );

				if( tempdist < mintemp){
					mintemp = tempdist;
					//minid = id;
				}
				//exw thn apostash tou prwtou q me tou prwtou p twra?
			}else if(metric == "cosine"){
				long double temp1 = 0, temp2 = 0, temp3 =0;
				vector<int>::iterator pi_it = (*p_it).coordinates.begin();
				for( vector<int>::iterator qi_it = (*q_it).coordinates.begin(); qi_it != (*q_it).coordinates.end(); ++qi_it, ++pi_it){
					temp1 += (*qi_it)*(*pi_it);
					temp2 += (*qi_it)*(*qi_it);
					temp3 += (*pi_it)*(*pi_it);
				}
				temp2 = sqrt ( temp2 );
				temp3 = sqrt ( temp3 );
				temp2 = temp2*temp3;
				temp1 = temp1 / temp2;
				tempdist = 1 - temp1;

				if( tempdist < mintemp){
					mintemp = tempdist;
					//minid = id;
				}
				//exw thn apostash tou prwtou q me tou prwtou p twra?
			}
		}
		//edw upotithete exoume vrei gia to sugkekrimeno q thn mikroterh apostash ara to vazoume
		NNdistances.push_back((double)mintemp);
	}
	return NNdistances;
}




