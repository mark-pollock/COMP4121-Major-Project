#include "rating.h"
#include "snow.h"
#include "dataManager.h"

using namespace std;
rating::rating(int s, int w, double stars){
	this->s = s;
	this->w = w;
	this->stars = stars;
}

rating::rating(vector<string> v){
	vector<string> snowInfo;
	auto it = v.begin();
	for(int i = 0; i < NUM_SNOW_PARAMETERS; i++){
		snowInfo.push_back(*it);
		++it;
	}
	snow snowTemp(snowInfo);
	s = findSnowIndex(&snowTemp);

	wax *l1 = findWax(&(*it));
	++it;
	wax *l2 = findWax(&(*it));
	++it;
	wax *l3 = NULL;
	if ((*it).compare("-") != 0)
		l3 = findWax(&(*it));

	w = findCombinationIndex(l1, l2, l3);
	++it;
	stars = stod(*it);
}


void rating::printSpecs(){
	cout << "---------- Rating Spec -------------" << endl;
	cout << "---- Snow:" << endl;
	printSnowByIndex(s);
	cout << "---- Wax Combination:" << endl;
	printCombinationByIndex(w);
	cout << "---- Stars:" << endl;
	cout << stars << endl;
	cout << "---------- End Printing Rating Spec ----------" << endl;
}
