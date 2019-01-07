/*
 * The main function resides in this file.
 * This file deals with the initial input and calls the appropriate
 * functions in dataManager.cpp
 */

#include "dataManager.h"
#include "wax.h"
#include "waxCombination.h"
#include "rating.h"
#include "snow.h"
#include "recommendation.h"

#define DEFAULT_NUM_RECOMMENDATIONS 3

using namespace std;

void giveRecommendation(vector<recommendation> *recs);
void waxRecommendation(char *fileName, int numRecommendations);

/*
 * Main function which takes in parameters in order to determine whether
 * wax data is being read in, test data is being read in, or
 * a recommendation is requested.
 * Usage:
 * read-in wax data: ./recommender -w fileName
 * read-in test data: ./recommender -t fileName
 * get recommendation: ./recommender -s fileName [number of waxes to recommend]
 */
int main(int argc, char* argv[]){
	if(argc < 2){
		cout << "error need atleast 1 argument" << endl;
		return 1;
	}else if(argv[1][0] != '-'){
		// error flag is wrong
		cout << "error, flag is wrong" << endl;
		return 2;
	}else if(argv[1][2] != '\0'){
		// error flag is wrong
		cout << "error, flag is wrong" << endl;
		return 3;
	}

	if(argc == 3){
		if(argv[1][1] == 's'){
			waxRecommendation(argv[2], DEFAULT_NUM_RECOMMENDATIONS);
		}else if(argv[1][1] == 'w'){
			// need to input new waxes
			readInWaxData(argv[2]);
		}else if(argv[1][1] == 't'){
			readInNewTestData(argv[2]);
		}else{
			cout << "unrecognised flag" << endl;
			return 4;
		}
	}else if(argc == 4){
		if(argv[1][1] == 's'){
			// wax recommendation
			waxRecommendation(argv[2], stoi(argv[3]));
		}else{
			cout << "error, usage is wrong" << endl;
		}
	}else{
		cout << "error, usage is wrong" << endl;
		return 1;
	}
	return EXIT_SUCCESS;
}

/*
 * Obtains the wax Recommendations for the snow data defined
 * in file 'fileName'
 */
void waxRecommendation(char *fileName, int numRecommendations){

	loadWaxData();
	loadSnowData();
	loadTestData();
	loadR();

	vector<string> snowData;
	getSnowDataVector(fileName, &snowData);
	snow s(snowData);

	vector<recommendation> *recommendation = getkNNRecommendations(&s, numRecommendations);

	giveRecommendation(&(*recommendation));

}

/*
 * Prints the recommendations
 */
void giveRecommendation(vector<recommendation> *recs){
	cout << "### Top " << recs->size() << " Wax Recommendations ###" << endl << endl;
	int num = 1;
	for(auto it = recs->begin(); it != recs->end(); ++it){
		cout << "Recommendation Number " << num << ":" << endl;
		cout << "----------------------------------------" << endl;
		it->print();
		cout << "----------------------------------------" << endl << endl;
		num++;
	}
}
