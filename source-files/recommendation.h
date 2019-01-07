/*
 * recommendation is a class which holds all the information of a
 * recommendation. i.e. the waxCombination, the stars and the
 * weights used for the wax and snow versions of the k-NN method.
 */

#ifndef RECOMMENDATION_H
#define RECOMMENDATION_H


class recommendation;
#include "waxCombination.h"



class recommendation{
public:
	recommendation(waxCombination *, double, double, double);
	void print();
	bool operator<(recommendation other) const {
		return stars < other.stars;
	}
	waxCombination *w;
	double stars;
	double wax_weight;
	double snow_weight;
};


#endif
