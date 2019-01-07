/*
 * wax is a class that holds all the information of a given wax layer.
 */


#ifndef WAX_H
#define WAX_H

class wax;
#include "waxCombination.h"

#define MIN_AIR_TEMP -40
#define MAX_AIR_TEMP 20
#define MIN_SNOW_TEMP -40
#define MAX_SNOW_TEMP 15
#define MIN_HUMIDITY 0
#define MAX_HUMIDITY 100

#include <string>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>

class wax{
public:
	wax(std::vector<std::string>); // constructor
	void printSpecs();
	bool canApplyOnTop(wax*);
	bool canApplyOnTop(waxCombination*);
	std::string name;
	double minAirTemp;
	double maxAirTemp;
	double minSnowTemp;
	double maxSnowTemp;
	double minHumidity;
	double maxHumidity;

	int dirty;
	int newSnow;
	int old;
	int abrasive;
	int artificial;
	int fine;
	int transformed;
	int all;
	int wet;
	int layer;
};


#endif
