#include "recommendation.h"

using namespace std;

recommendation::recommendation(waxCombination *w, double stars, double wax_weight, double snow_weight){
	this->w = w;
	this->stars = stars;
	this->wax_weight = wax_weight;
	this->snow_weight = snow_weight;
}

void recommendation::print(){
	cout << "Wax:" << endl;
	w->printSpecsShort();
	cout << "Rating:" << endl;
	cout << "\t Predicted rating = ";
	printf("%1.1f \n", stars);//<< fixed << setprecision(3) << stars << endl;
	cout << "\t based on:" << endl;
	if(wax_weight == 0 && snow_weight == 0){
		cout << "\t \t previous rating for this wax by this snow" << endl;
	}else{
		cout << "\t \t similar waxes with a weight of " << wax_weight << endl;
		cout << "\t \t similar snows with a weight of " << snow_weight << endl;
	}
}
