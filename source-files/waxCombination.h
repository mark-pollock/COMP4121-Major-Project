/*
 * waxCombination is a class that holds all the information for
 * a combination of single waxes. i.e. what each layer is and the
 * attributes for the combination.
 */

#ifndef WAXCOMBINATION_H
#define WAXCOMBINATION_H

class waxCombination;

#include "wax.h"
#include <string>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>



class waxCombination{
public:
	waxCombination(wax*, wax*); // otherContstructor
	waxCombination(waxCombination*, wax*);
	waxCombination(wax*, wax*, wax*);
	void printSpecs();
	void printSpecsShort();
	bool same(waxCombination *);

	bool possibleOnTop(wax*);
	bool possibleOnTop(waxCombination*);
	wax *layer1;
	wax *layer2;
	wax *layer3;

	double minAirTemp;
	double maxAirTemp;
	double minSnowTemp;
	double maxSnowTemp;
	double minHumidity;
	double maxHumidity;

	int numLayers;
	int dirty;
	int newSnow;
	int old;
	int abrasive;
	int artificial;
	int fine;
	int transformed;
	int all;
	int wet;
};

#endif

