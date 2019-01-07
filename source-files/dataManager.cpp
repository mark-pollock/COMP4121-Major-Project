#include "dataManager.h"
#include "snow.h"
#include "rating.h"
#include "wax.h"
#include "waxCombination.h"

#include <string>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <math.h>
#include <queue>

/*
 * Global Variables ******************************************************************
 */
using namespace std;

/*
 * This global variable holds all possible wax combinations
 */
std::vector<waxCombination> allCombinations;

/*
 * This global variable holds all waxes that have been read in to the
 * program.
 */
std::vector<wax> allWaxes;

/*
 * This global variable holds all the ratings. i.e. the test data information
 * that is used to generate predictions.
 */
std::vector<rating> allRatings;

/*
 * This global variable holds all the snows that have been read into the system.
 */
std::vector<snow> allSnow;

/*
 * This global variable R is the matrix where R[i][j] is the rating snow i
 * gave wax j (from the rating data).
 */
std::vector<std::vector<double>> R;

/*
 * This global variable holds the final recommendations to be made.
 */
vector<recommendation> finalRecs;

vector<double> simSnow;	// similarity between snow s
							// and all other snows indexed by
							// allSnow


vector<vector<double>> simWaxT; // weighted combination of simWax1 and simWax2 where:
								// simWax1 = similarities between all pairs
								// of waxes indexed by allCombinations.
								// based on ratings by snow conditions.
								// and
								// simWax2 = similarities between all pairs of
								// waxes indexed by allCombinations.
								// based on whether any layers are the same.


#define DEFAULT_RATING 2.49999998 	// not exactly 2.5 so I can check whether it was
									// set to the default rating or not.

/*************************************************************************************/




/*
 * Forward declarations **************************************************************
 */
vector<recommendation> *getkNNRecommendations(snow *s, int numberToRecommend);
double kNNRating_wax(int snowIndex, int waxIndex, int k);
double kNNRating_snow(int snowIndex, int waxIndex, int k);
void getSnowSims(snow *s);
void getWaxSims();
double getCosineSim(unsigned int i, unsigned int j);
bool hasRated(unsigned int s, unsigned int i, unsigned int j);
double abs(double d);
void loadsimWaxT();
void savesimWaxT();

/*
 * Comparison which simply compares std:pair<int, double> types by their
 * second item.
 */
class mycomparison{
	bool reverse;
public:
	mycomparison(const bool& revparam=false) {reverse=revparam;}
	bool operator() (std::pair<int, double> lhs, std::pair<int, double> rhs) const{
		if (reverse)
			return (lhs.second > rhs.second);
		else
			return (lhs.second < rhs.second);
	}
};
/*************************************************************************************/




/*
 * This function takes in the information of the snow that needs to have
 * a wax recommended for and the number of recommendations required.
 *
 * It is a nearest neighbour method which obtains the recommendation
 * via implementing k-Nearest Neighbour recommendation algorithms for
 * both similar snows (to s) and similar waxes (to waxes s has already
 * rated - if any).
 */
vector<recommendation> *getkNNRecommendations(snow *s, int numberToRecommend){
#define SIMILAR_SNOW_WEIGHT 0.5
#define SIMILAR_WAX_WEIGHT 0.5

	if(numberToRecommend > (int)allCombinations.size()){
		cout << "Error: you have asked for more recommendations"
				<< " then there are total wax combinations." << endl;
		assert(false);
	}
	getSnowSims(s);
	loadsimWaxT();
	int k1 = 3; // three most similar waxes to ones snow s has rated.
	int k2 = 3; // three most similar snows who have rated the wax in question.

	typedef priority_queue<recommendation> p_queue;
	p_queue P;

	bool addedSnow = false;
	int snowIndex = findSnowIndex(s);
	if(snowIndex == -1){
		allSnow.push_back(*s);
		snowIndex = findSnowIndex(s);
		vector<double> zeroForR;
		for(auto i = 0; i < (int)allCombinations.size(); ++i){
			zeroForR.push_back(0);
		}
		R.push_back(zeroForR);
		addedSnow = true;
	}

	if(snowIndex != -1){
		// R[index] is the row of R for snow s
		double average_wax_weight = 0;
		double average_snow_weight = 0;
		int count = 0;
		for(int waxIndex = 0; waxIndex < (int)R[snowIndex].size(); ++waxIndex){
			if(R[snowIndex][waxIndex] == 0){
				count++;
				double Psj_1, Psj_2;
				double weight1 = SIMILAR_WAX_WEIGHT;
				double weight2 = SIMILAR_SNOW_WEIGHT;
				if(addedSnow == false)
					Psj_1 = kNNRating_wax(snowIndex, waxIndex, k1);
				else{
					Psj_1 = DEFAULT_RATING;
					weight1 -= 0.45;
					weight2 += 0.45;
				}


				Psj_2 = kNNRating_snow(snowIndex, waxIndex, k2);
				if(Psj_2 == DEFAULT_RATING){
					weight2 -= 0.45;
					weight1 += 0.45;
				}

				double currP = weight1*Psj_1 + weight2*Psj_2;

				average_wax_weight += weight1;
				average_snow_weight += weight2;

				if(currP > 5)
					currP = 5;

				recommendation currPrediction(&allCombinations[waxIndex], currP, weight1, weight2);

				P.push(currPrediction);

			}else{
				recommendation currRating(&allCombinations[waxIndex], min(5.0, R[snowIndex][waxIndex]), 0, 0);
				P.push(currRating);
			}

		}

		average_wax_weight /= (double)count;
		average_snow_weight /= (double)count;
		for(int i = 0; i < numberToRecommend; ++i){
			finalRecs.push_back(P.top());
			P.pop();
		}

		return &finalRecs;


	}else{
		cout << "error: something went wrong with inserting the snow into allSnow vector" << endl;
		return NULL;
	}
}


/*
 * This finds the predicted rating for wax allCombinations[waxIndex] for
 * snow allSnow[snowIndex] using the k nearest wax neighbours - i.e.
 * waxes which are similar to those that snow allSnow[snowIndex] has
 * already rated.
 */
double kNNRating_wax(int snowIndex, int waxIndex, int k){

	typedef priority_queue<pair<int, double>, vector<pair<int, double>>, mycomparison> p_queue;

	p_queue allSimilar;

	for(int n = 0; n < (int)simWaxT[waxIndex].size(); ++n){\
		if(R[snowIndex][n] == 0)
			continue; 	// snow has not rated wax n therefore
						// there is no use finding similar waxes

		pair<int, double> tempSim(n, simWaxT[waxIndex][n]);
		allSimilar.push(tempSim);
	}


	vector<pair<int, double>> mostSimilar;
	for(int i = 0; i < k; ++i){
		pair<int, double> tempSim(allSimilar.top().first, allSimilar.top().second);
		mostSimilar.push_back(tempSim);
		allSimilar.pop();
	}

	double P1_numerator = 0;
	double P1_denominator = 0;

	for(auto it = mostSimilar.begin(); it != mostSimilar.end(); ++it){
		P1_numerator += (it->second * R[snowIndex][waxIndex]);
		P1_denominator += abs(it->second);
	}
	double P1;
	if(P1_denominator != 0)
		P1 = P1_numerator / P1_denominator;
	else
		P1 = DEFAULT_RATING;

	if(P1 > 5)
		P1 = 5;
	return P1;
}



/*
 * This finds the predicted rating for wax allCombinations[waxIndex] for
 * snow allSnow[snowIndex] using the k nearest snow neighbours - i.e.
 * the waxes that snows which are similar to allSnow[snowIndex] have
 * rated highly.
 */
double kNNRating_snow(int snowIndex, int waxIndex, int k){
	typedef priority_queue<pair<int, double>, vector<pair<int, double>>, mycomparison> p_queue;
	p_queue allSimilar;

	for(int n = 0; n < (int)simSnow.size(); ++n){
		if(R[n][waxIndex] == 0)
			continue; 	// snow n has not rated wax waxIndex therefore
						// there is no use finding similar snows

		pair<int, double> tempSim(n, simSnow[n]);
		allSimilar.push(tempSim);
	}

	if(allSimilar.size() == 0)
		return DEFAULT_RATING;

	vector<pair<int, double>> mostSimilar;
	for(int i = 0; i < k; ++i){
		pair<int, double> tempSim(allSimilar.top().first, allSimilar.top().second);
		mostSimilar.push_back(tempSim);
		allSimilar.pop();
	}

	double P1_numerator = 0;
	double P1_denominator = 0;

	for(auto it = mostSimilar.begin(); it != mostSimilar.end(); ++it){
		P1_numerator += it->second * R[it->first][waxIndex];
		P1_denominator += abs(it->second);
	}
	double P1;
	if(P1_denominator != 0)
		P1 = P1_numerator / P1_denominator;
	else
		P1 = DEFAULT_RATING;

	P1 += -0.25;
	if(P1 > 5)
		P1 = 5;

	return P1;
}


/*
 * Function which fills in the simSnow matrix.
 * That is, the similarities between the snow pointed to by s and all
 * other snows are calculated and stored in the simSnow vector.
 *
 * This is based on the proximity of each snow's attributes to snow s.
 */
void getSnowSims(snow *s){

#define AIR_TEMP_RANGE 8.0
#define HUMIDITY_RANGE 20.0
#define SNOW_TEMP_RANGE 8.0
#define DIRTY_RANGE 2.0
#define NEW_SNOW_RANGE 1.0
#define ARTIFICIAL_RANGE 1.0
#define ABRASIVE_RANGE 1.0
#define CRYSTAL_SIZE_RANGE 1.0
#define MOISTURE_LEVEL_RANGE 2.0

	int i = 0;
	for(auto it = allSnow.begin(); it != allSnow.end(); ++it){
		double sim = 0;

		sim += max(-1.0, (AIR_TEMP_RANGE - abs(s->airTemp - it->airTemp)) / AIR_TEMP_RANGE);
		sim += max(-1.0, (HUMIDITY_RANGE - abs(s->humidity - it->humidity)) / HUMIDITY_RANGE);
		sim += max(-1.0, (SNOW_TEMP_RANGE - abs(s->snowTemp - it->snowTemp)) / SNOW_TEMP_RANGE);
		sim += max(-1.0, (DIRTY_RANGE - abs(s->dirty - it->dirty)) / DIRTY_RANGE);
		sim += max(-1.0, (NEW_SNOW_RANGE - abs(s->newSnow - it->newSnow)) / NEW_SNOW_RANGE);
		sim += max(-1.0, (ARTIFICIAL_RANGE - abs(s->artificial - it->artificial)) / ARTIFICIAL_RANGE);
		sim += max(-1.0, (ABRASIVE_RANGE - abs(s->abrasive - it->abrasive)) / ABRASIVE_RANGE);
		sim += max(-1.0, (CRYSTAL_SIZE_RANGE - abs(s->crystalSize - it->crystalSize)) / CRYSTAL_SIZE_RANGE);
		sim += max(-1.0, (MOISTURE_LEVEL_RANGE - abs(s->moistureLevel - it->moistureLevel)) / MOISTURE_LEVEL_RANGE);


		sim = sim / (double)NUM_SNOW_PARAMETERS;
		assert(sim <= 1);
		assert(sim >= -1);


		simSnow.push_back(sim);

		assert(simSnow[i] == sim);
		i++;

	}
}


/*
 * Function which fills in simWax matrices.
 *
 * That is, this function finds the similarities between each wax pair.
 * This is based on a weighted average (by default evenly weighted)
 * of the similarities based on common layers and the similarities based
 * on the cosine similarity function.
 *
 */
void getWaxSims(){
	vector<vector<double>> simWax1;	// similarities between all pairs
									// of waxes indexed by allCombinations.
									// based on ratings by snow conditions.

	vector<vector<double>> simWax2; // similarities between all pairs of
									// waxes indexed by allCombinations.
									// based on whether any layers are the same.

	for(int i = 0; i < (int)allCombinations.size(); ++i){
		vector<double> zero;
		for(int j = 0; j < (int)allCombinations.size(); ++j){
			zero.push_back(0);
		}
		simWax1.push_back(zero);
		simWax2.push_back(zero);
	}
	int update_percentage = allCombinations.size() / 10;
	int count = 1;
	for(unsigned int i = 0; i < allCombinations.size(); ++i){
		if((int)i == count*update_percentage){
			if(count != 10){
				cout << count << "0 percent complete calculating..." << endl;
				count++;
			}
		}
		for(unsigned int j = 0; j < allCombinations.size(); ++j){
			if(i == j){
				simWax1[i][j] = 1;
				simWax2[i][j] = 1;
			}else{
				int numLayersCommon = 0;
				if(allCombinations[i].layer1->name.compare(allCombinations[j].layer1->name) == 0)
					numLayersCommon++;
				if(allCombinations[i].layer2->name.compare(allCombinations[j].layer2->name) == 0)
					numLayersCommon++;
				if(allCombinations[i].numLayers == 3 && allCombinations[j].numLayers == 3
						&& allCombinations[i].layer3->name.compare(allCombinations[j].layer3->name) == 0)
					numLayersCommon++;

				simWax2[i][j] = (double)numLayersCommon / 3.0;


				simWax1[i][j] = getCosineSim(i, j);

			}
		}
	}
	assert(simWax1.size() == simWax2.size());
	assert(simWax1[0].size() == simWax1.size());
	assert(simWax2[0].size() == simWax2.size());

	double weight1 = 0.5;
	double weight2 = 0.5;
	for(int i = 0; i < (int)simWax1.size(); ++i){
		vector<double> tempSimWax;
		for(int j = 0; j < (int)simWax1[0].size(); ++j){
			tempSimWax.push_back(weight1*simWax1[i][j] + weight2*simWax2[i][j]);
		}
		simWaxT.push_back(tempSimWax);
	}
	cout << "100 percent complete calculating." << endl;
}

/*
 * Returns the similarity between wax i and j based on
 * the cosine similarity function.
 */
double getCosineSim(unsigned int i, unsigned int j){
	double numerator = 0;
	double denominator1 = 0;
	double denominator2 = 0;

	for(unsigned int s = 0; s < R.size(); ++s){
		if(!hasRated(s, i, j))
			continue;

		numerator += (R[s][i])*(R[s][j]);

		denominator1 += pow((R[s][i]), 2);
		denominator2 += pow((R[s][j]), 2);

	}

	if(denominator1 == 0 || denominator2 == 0)
		return 0;

	return numerator / sqrt(denominator1*denominator2);
}

/*
 * Simply returns if snow s has rated both wax i and wax j
 * (where s, i, and j are indexes to appropriate global vectors.
 * i.e. allSnow and allCombinations)
 */
bool hasRated(unsigned int s, unsigned int i, unsigned int j){
	return (R[s][i] != 0 && R[s][j] != 0);
}



/*
 * Printing functions
 */
void printSnowByIndex(int i){
	allSnow[i].printSpecs();
}
void printCombinationByIndex(int i){
	allCombinations[i].printSpecs();
}
void printAllCombinations(){
	for(auto it = allCombinations.begin(); it != allCombinations.end(); ++it){
		it->printSpecs();
	}
}
void printAllSnow(){
	for(auto it = allSnow.begin(); it != allSnow.end(); ++it){
		it->printSpecs();
	}
}
void printAllRatings(){
	for(auto it = allRatings.begin(); it != allRatings.end(); ++it){
		it->printSpecs();
	}
}
void printAllWaxes(){
	for(auto it = allWaxes.begin(); it != allWaxes.end(); ++it){
		it->printSpecs();
	}
}



/*
 * Find functions - these do what they sound like they do
 */
wax *findWax(string *name){
	for (auto it = allWaxes.begin(); it != allWaxes.end(); ++it){
		if(*name == it->name)
			return &(*it);
	}
	return NULL;
}

waxCombination *findCombination(wax *w1, wax *w2, wax *w3, int numLayers){
	if(numLayers == 3){
		for (auto it = allCombinations.begin(); it != allCombinations.end(); ++it){
			if(w1->name == it->layer1->name && w2->name == it->layer2->name
					&& w3->name == it->layer3->name)
				return &(*it);
		}
		return NULL;
	}else if(numLayers == 2){
		for (auto it = allCombinations.begin(); it != allCombinations.end(); ++it){
			if(it->numLayers == 2 && w1->name == it->layer1->name
					&& w2->name == it->layer2->name)
				return &(*it);
		}
		return NULL;
	}else{
		return NULL;
	}
}

int findCombinationIndex(wax *w1, wax *w2, wax *w3, int numLayers){
	assert(numLayers == 2 || numLayers == 3);
	if(numLayers == 3){
		for (int i = 0; i < (int)allCombinations.size(); ++i){
			if(allCombinations[i].numLayers == 3
					&& w1->name.compare(allCombinations[i].layer1->name) == 0
					&& w2->name.compare(allCombinations[i].layer2->name) == 0
					&& w3->name.compare(allCombinations[i].layer3->name) == 0)
				return i;
		}
		return -1;
	}else{
		for (int i = 0; i < (int)allCombinations.size(); ++i){
			if(allCombinations[i].numLayers == 2
					&& w1->name.compare(allCombinations[i].layer1->name) == 0
					&& w2->name.compare(allCombinations[i].layer2->name) == 0)
				return i;
		}
		return -1;
	}
}

int findCombinationIndex(wax *w1, wax *w2){
	return findCombinationIndex(w1, w2, w2, 2);
}

int findCombinationIndex(wax *w1, wax *w2, wax *w3){
	if(w3 == NULL)
		return findCombinationIndex(w1, w2, w2, 2);
	else
		return findCombinationIndex(w1, w2, w3, 3);
}

waxCombination *findCombination(wax *w1, wax *w2){
	return findCombination(w1, w2, w2, 2);
}

waxCombination *findCombination(wax *w1, wax *w2, wax *w3){
	if(w3 == NULL)
		return findCombination(w1, w2);
	return findCombination(w1, w2, w3, 3);
}

snow *findSnow(vector<string> v){
	for(auto it = allSnow.begin(); it != allSnow.end(); ++it){
		if(it->same(v))
			return &(*it);
	}
	return NULL;
}
int findWaxIndex(waxCombination *w){
	for(int i = 0; i < (int)allCombinations.size(); ++i){
		if(allCombinations[i].same(w))
			return i;
	}

	return -1;
}
snow *findSnow(snow *s){
	for(auto it = allSnow.begin(); it != allSnow.end(); ++it){
		if(it->same(s))
			return &(*it);
	}
	assert(1 == 2);
	return NULL;
}
int findSnowIndex(snow *s){
	for(int i = 0; i < (int)allSnow.size(); ++i){
		if(allSnow[i].same(s)){
			return i;
		}

	}
	return -1; // not found
}

/*
 * This function loads data from wax.dat and combo.dat into global vectors
 * allWaxes and allCombinations.
 */
void loadWaxData(){
	ifstream waxData;
	waxData.open("data_files/wax.dat", ios::in);
	if(waxData.is_open()){
		while(waxData){
			string line;
			if(!getline(waxData, line))
				break;

			istringstream values(line);
			vector<string> waxSpecs;

			while(values){
				string value;
				if(!getline(values, value, ','))
					break;
				waxSpecs.push_back(value);
			}
			wax curr(waxSpecs);
			allWaxes.push_back(curr);
		}
	}else{
		cout << "error: data_files/wax.dat could not be opened! Terminating..." << endl;
		assert(false);
	}

	ifstream comboData;
	comboData.open("data_files/combo.dat", ios::in);
	if(comboData.is_open()){
		while(comboData){
			string line;
			if(!getline(comboData, line)){
				break;
			}
			istringstream fullLine(line);

			string layers[3];
			for(int i = 0; i < 3; ++i){
				if(!getline(fullLine, layers[i], ','))
					break;
			}

			wax *l1, *l2, *l3;
			l1 = findWax(&layers[0]);
			l2 = findWax(&layers[1]);
			if(layers[2].compare("-") != 0){
				l3 = findWax(&layers[2]);
				waxCombination new_Combo(l1, l2, l3);
				allCombinations.push_back(new_Combo);
			}else{
				waxCombination new_Combo(l1, l2);
				allCombinations.push_back(new_Combo);
			}

		}
	}else{
		cout << "error: data_files/combo.dat could not be opened! Terminating..." << endl;
		assert(false);
	}
}

/*
 * This function reads in data from the file ratings.dat into
 * the global vector ratings.
 */
void loadTestData(){
	ifstream testData;
	testData.open("data_files/ratings.dat", ios::in);
	if(testData.is_open()){
		while(testData){
			string line;
			if(!getline(testData, line))
				break;
			istringstream values(line);
			vector<string> testSpecs;
			while(values){
				string value;
				if(!getline(values, value, ','))
					break;
				testSpecs.push_back(value);
			}

			rating ratingCurr(testSpecs);
			allRatings.push_back(ratingCurr);
		}
	}else{
		cout << "error: data_files/ratings.dat could not be opened! Terminating..." << endl;
		assert(false);
	}


}


/*
 * This function reads in data from snow.dat into the global vector
 * allSnow
 */
void loadSnowData(){
	ifstream snowData;
	snowData.open("data_files/snow.dat", ios::in);
	if(snowData.is_open()){
		while(snowData){
			string line;
			if(!getline(snowData, line))
				break;
			istringstream values(line);
			vector<string> snowSpecs;
				while(values){
				string value;
				if(!getline(values, value, ','))
					break;
				snowSpecs.push_back(value);
			}
			snow curr(snowSpecs);
			allSnow.push_back(curr);
		}
	}else{
		cout << "error: data_files/snow.dat could not be opened! Terminating..." << endl;
		assert(false);
	}
}

/*
 * This function loads waxSims1, waxSims2 and waxSimsT.
 */
void loadsimWaxT(){
	cout << "Loading similarities between waxes. This may take a while." << endl;
	ifstream simWaxTFile;
	simWaxTFile.open("data_files/simWaxT.dat");
	int count = 1;
	if(simWaxTFile.is_open()){
		while(simWaxTFile){
			if(count == (int)(allCombinations.size() / 4))
				cout << "25% complete loading." << endl;
			else if(count == (int)(allCombinations.size() / 2))
				cout << "50% complete loading." << endl;
			else if(count == 3*(int)(allCombinations.size() / 4))
				cout << "75% complete loading." << endl;

			count++;
			string line;
			if(!getline(simWaxTFile, line))
				break;
			istringstream values(line);
			vector<double> simWaxTRow;
			while(values){
				string value;
				if(!getline(values, value, ','))
					break;
				simWaxTRow.push_back(stod(value));
			}
			simWaxT.push_back(simWaxTRow);
		}
	}else{
		cout << "error: data_files/simWaxT.dat could not be opened! Terminating..." << endl;
		assert(false);
	}
	cout << "100% complete loading." << endl << endl;
}

/*
 * This returns a pointer to the allCombinations vector as newly read from
 * the combo.dat file. Used for printing purposes by other cpp files.
 */
vector<waxCombination> *getAllCombos(){
	loadWaxData();
	return &allCombinations;
}


/*
 * This function copies the data that occurs in file fileName into
 * the vector v - which is simply a vector of strings which represents
 * snow data.
 */
void getSnowDataVector(char *fileName, std::vector<std::string> *v){
	ifstream snowData;
	snowData.open(fileName, ios::in);
	if(snowData.is_open()){
		string line;
		if(!getline(snowData, line)){
			cout << "no data in file \'" << fileName << "\'. Terminating..." << endl;
			assert(false);
		}
		istringstream values(line);
			while(values){
			string value;
			if(!getline(values, value, ','))
				break;
			v->push_back(value);
		}

	}else{
		cout << "error: \'" << fileName << "\' could not be opened! Terminating..." << endl;
		assert(false);
	}
}

/*
 * This function saves allSnow to snow.dat
 */
void saveSnowData(){
	assert(allSnow.size() > 0);
	ofstream snowFile;
	snowFile.open("data_files/snow.dat");
	for (auto it = allSnow.begin(); it != allSnow.end(); ++it){
		snowFile << it->airTemp << "," << it->humidity << "," <<
				it->snowTemp << "," << it->dirty << "," << it->newSnow
				<< "," << it->artificial << "," << it->abrasive
				<< "," << it->crystalSize << "," << it->moistureLevel << endl;
	}
	snowFile.close();
}

/*
 * This function saves allRatings to ratings.dat
 */
void saveTestData(){
	assert(allRatings.size() > 0);
	ofstream ratingFile;
	ratingFile.open("data_files/ratings.dat");
	for (auto it = allRatings.begin(); it != allRatings.end(); ++it){
		ratingFile << allSnow[it->s].airTemp << "," << allSnow[it->s].humidity << "," <<
				allSnow[it->s].snowTemp << "," << allSnow[it->s].dirty << "," << allSnow[it->s].newSnow
				<< "," << allSnow[it->s].artificial << "," << allSnow[it->s].abrasive
				<< "," << allSnow[it->s].crystalSize << "," << allSnow[it->s].moistureLevel <<
				"," << allCombinations[it->w].layer1->name << "," << allCombinations[it->w].layer2->name << ",";
				if(allCombinations[it->w].numLayers == 3)
					ratingFile << allCombinations[it->w].layer3->name;
				else
					ratingFile << "-";

				ratingFile << "," << it->stars << endl;
	}
	ratingFile.close();
}

/*
 * This function saves simWaxT to simWaxT.dat
 */
void savesimWaxT(){
	assert(simWaxT.size() > 0);
	ofstream simWaxTFile;
	simWaxTFile.open("data_files/simWaxT.dat");
	for(auto it = simWaxT.begin(); it != simWaxT.end(); ++it){
		auto it2 = it->begin();
		for(; it2 != it->end(); ++it2){
			if(it2 != it->begin())
				simWaxTFile << ",";
			simWaxTFile << *it2;
		}
		simWaxTFile << endl;

	}
	simWaxTFile.close();
}

/*
 * This function computes the matrix R where R[i][j] holds
 * the rating snow i has given to snow j.
 */
void addRatingsToR(){
	for(auto it = allSnow.begin(); it != allSnow.end(); ++it){
		vector<double> currSnowRatings;
		for(auto it2 = allCombinations.begin(); it2 != allCombinations.end(); ++it2){
			currSnowRatings.push_back(0);
		}
		R.push_back(currSnowRatings);
	}

	for(auto it = allRatings.begin(); it != allRatings.end(); ++it){
		assert(it->s >= 0);
		assert(it->w >= 0);

		R[it->s][it->w] = it->stars;
	}
}

/*
 * This function saves the matrix R to R.dat
 */
void saveR(){
	assert(R.size() > 0);
	ofstream RFile;
	RFile.open("data_files/R.dat");

	for(int i = 0; i < (int)R.size(); ++i){
		for(int j = 0; j < (int)R[0].size(); ++j){
			if(R[i][j] != 0){
				RFile << i << "," << j << "," << R[i][j] << endl;
			}
		}
	}
	RFile.close();
}


/*
 * This function loads data from R.dat to the global matrix R.
 */
void loadR(){

	// Initializing to be all 0's
	for(auto it = allSnow.begin(); it != allSnow.end(); ++it){
		vector<double> currSnowRatings;
		for(auto it2 = allCombinations.begin(); it2 != allCombinations.end(); ++it2){
			currSnowRatings.push_back(0);
		}
		R.push_back(currSnowRatings);
	}


	ifstream RData;
	RData.open("data_files/R.dat", ios::in);
	if(RData.is_open()){
		while(RData){
			string line;
			if(!getline(RData, line))
				break;
			istringstream values(line);
			vector<string> RSpecs;
			while(values){
				string value;
				if(!getline(values, value, ','))
					break;
				RSpecs.push_back(value);
			}

			int i = stoi(RSpecs[0]);
			int j = stoi(RSpecs[1]);

			double r = stod(RSpecs[2]);

			R[i][j] = r;
		}
	}else{
		cout << "error: R.dat could not be opened! Terminating..." << endl;
		assert(false);
	}
}


/*
 * This function reads in test data from file fileName into global variable:
 * ratings (and allSnow and allCombinations if applicable).
 */
void readInNewTestData(char* fileName){
	loadWaxData();
	ifstream testData;
	testData.open(fileName, ios::in);
	int numNewCombos = 0;

	if(testData.is_open()){
		while(testData){
			string line;
			if(!getline(testData,line))
				break;
			istringstream snowvalues(line);
			vector<string> snowSpecs;

			while(snowvalues){
				string value;
				if(!getline(snowvalues, value, ','))
					break;

				snowSpecs.push_back(value);
			}
			snow snowTemp(snowSpecs);
			int sCurr = findSnowIndex(&snowTemp);
			if(sCurr == -1){
				allSnow.push_back(snowTemp);
				sCurr = findSnowIndex(&snowTemp);
			}

			assert(sCurr != -1);


			if(!getline(testData,line))
				break;
			istringstream waxlayers(line);

			string layer[3];
			int i = 0;
			while(waxlayers){
				if(!getline(waxlayers, layer[i], ','))
					break;
				i++;
			}
			if(layer[2].compare("-") == 0){
				i--;
			}
			wax *l[3];
			int numlayers = i;
			while(i > 0){
				i--;
				l[i] = findWax(&layer[i]);
				if(l[i] == NULL){
					cout << "error - wax not in system. Terminating..." << endl;
					assert(false);
				}
			}

			int waxComboCurr = findCombinationIndex(l[0], l[1], l[2], numlayers);
			if(waxComboCurr == -1){
				cout << "did not find" << endl;
				numNewCombos++;
				if(numlayers == 2){
					waxCombination newCombo(l[0], l[1]);
					allCombinations.push_back(newCombo);
				}else{
					assert(numlayers == 3);
					waxCombination newCombo(l[0], l[1], l[2]);
					allCombinations.push_back(newCombo);
				}
				waxComboCurr = findCombinationIndex(l[0], l[1], l[2], numlayers);
			}

			if(!getline(testData,line))
				break;
			double starsCurr = stod(line);

			rating ratingCurr(sCurr, waxComboCurr, starsCurr);

			allRatings.push_back(ratingCurr);

		}

	}
	cout << "Test data read in and overwritten: " << endl <<
			"\t" << allRatings.size() << " data points read in;" <<  endl <<
			"\t" << numNewCombos << " new combination(s) added; and" << endl <<
			"\t" << allSnow.size() << " snow conditions added." << endl << endl;

	saveTestData();
	if(numNewCombos > 0){
		saveWaxData();
	}
	saveSnowData();

	addRatingsToR();
	saveR();
	cout << "Calculating and saving similarities between waxes."
			<< " This may take a while..." << endl;
	cout << "Calculating similarities..." << endl;
	getWaxSims();
	cout << "Saving similarities..." << endl;
	savesimWaxT();
	cout << "Similarities between waxes calculated and saved!" << endl;
}


/*
 * This function saves the allWaxes and allCombination vectors to
 * wax.dat and combos.dat respectively.
 */
void saveWaxData(){
	assert(allCombinations.size() > 0);
	assert(allWaxes.size() > 0);
	ofstream waxFile;
	waxFile.open("data_files/wax.dat");
	for (auto it = allWaxes.begin(); it != allWaxes.end(); ++it){
		waxFile << it->name << "," << it->minAirTemp << "," << it->maxAirTemp << ","
				<< it->minSnowTemp << "," << it->maxSnowTemp << ","
				<< it->minHumidity << "," << it->maxHumidity << ","
				<< it->dirty << "," << it->newSnow << ","
				<< it->old << "," << it->abrasive << ","
				<< it->artificial << "," << it->fine << ","
				<< it->transformed << "," << it->all << ","
				<< it->wet << "," << it->layer << endl;
	}

	waxFile.close();

	ofstream comboFile;
	comboFile.open("data_files/combo.dat");
	for(auto it = allCombinations.begin(); it != allCombinations.end(); ++it){
		comboFile << it->layer1->name << "," << it->layer2->name << ",";
		if(it->numLayers == 3)
			comboFile << it->layer3->name;
		else
			comboFile << "-";
		comboFile << endl;
	}

	comboFile.close();

}

/*
 * This function reads in the list of waxes and their attributes to
 * allWaxes from file fileName. It also computes all possible combinations
 * based on a very conservative pairing approach. i.e. any combination that
 * would be made in reality is definitely made in this function (and many more).
 */
void readInWaxData(char* fileName){
	vector<wax> layer1Wax;
	vector<wax> layer2Wax;
	vector<wax> layer3Wax;

	ifstream waxData;
	waxData.open(fileName, ios::in);

	if(waxData.is_open()){
		while(waxData){
			string line;
			if(!getline(waxData, line))
				break;
			istringstream values(line);
			vector<string> waxSpecs;

			while(values){
				string value;
				if(!getline(values, value, ','))
					break;
				waxSpecs.push_back(value);
			}
			wax curr(waxSpecs);
			allWaxes.push_back(curr);
		}
	}

	for(auto it = allWaxes.begin(); it != allWaxes.end(); ++it){
		if((*it).layer == 1)
			layer1Wax.push_back(*it);
		else if((*it).layer == 2)
			layer2Wax.push_back(*it);
		else if((*it).layer == 3)
			layer3Wax.push_back(*it);
		else{
			cout << "Error: unacceptable layer number. Terminating..." << endl;
			assert(false);
		}
	}

	vector<waxCombination> l2_l3Combos;
	for(auto it = layer2Wax.begin(); it != layer2Wax.end(); ++it){
		for(auto it2 = layer3Wax.begin(); it2 != layer3Wax.end(); ++it2){
			if((*it).canApplyOnTop(&(*it2))){
				waxCombination combo_temp(&(*it), &(*it2));
				l2_l3Combos.push_back(combo_temp);
			}
		}
	}

	for(auto it = layer1Wax.begin(); it != layer1Wax.end(); ++it){
		for(auto it2 = l2_l3Combos.begin(); it2 != l2_l3Combos.end(); ++it2){
			if((*it).canApplyOnTop(&(*it2))){
				waxCombination combo_temp(&(*it2), &(*it));
				allCombinations.push_back(combo_temp);
			}
		}
	}


	/*
	 * Finds all the 2 layer combinations (1st and 2nd layer)
	 */
	for(auto it = layer1Wax.begin(); it != layer1Wax.end(); ++it){
		for(auto it2 = layer2Wax.begin(); it2 != layer2Wax.end(); ++it2){
			if(it->canApplyOnTop(&(*it2))){
				waxCombination combo_temp(&(*it), &(*it2));
				allCombinations.push_back(combo_temp);
			}
		}
	}

	cout << "Wax read in complete, there are " << allCombinations.size() << " combinations of waxes!" << endl;

	saveWaxData();
}

/*
 * Small helper function that returns the absolute value of d.
 */
double abs(double d){
	if(d > 0)
		return d;
	else
		return -d;
}







