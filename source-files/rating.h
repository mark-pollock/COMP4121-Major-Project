/*
 * rating is a class which holds the information of a rating.
 * i.e. the snow, the waxCombination and the actual rating (stars)
 */

#ifndef RATING_H
#define RATING_H

class rating;

#include "wax.h"
#include "waxCombination.h"
#include "snow.h"

#include <string>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>

class rating{
public:
	rating(int, int, double);
	rating(std::vector<std::string>);
	void printSpecs();

	int s;			// snow index
	int w;			// wax index
	double stars; 	// actual rating - based on a 5 stars system.
};

#endif
