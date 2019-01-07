/*
 * snow is a class that holds all the information required for
 * a unique snow condition.
 */

#ifndef SNOW_H
#define SNOW_H
class snow;

#include <string>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>

/*
 * The following describes the values of the attributes that correspond
 * to the physical attributes.
 *
 * dirty: clean = 0 .. dirty = 4
 * newSnow: old = 0 .. new = 2
 * artificial: artificial = 0 .. natural = 2
 * abrasive: abrasive = 0 .. soft = 2
 * crystalSize: fine = 0 .. transformed = 2
 * moistureLevel: wet = 0 .. dry = 4
 */


#define FINE 0
#define TRANSFORMED 2
#define WET 0
#define DRY 4
#define CLEAN 0
#define DIRTY 4
#define OLD 0
#define NEW 2
#define ABRASIVE 0
#define SOFT 2
#define ARTIFICIAL 0
#define NATURAL 2

#define NUM_SNOW_PARAMETERS 9

class snow{
public:
	snow(std::vector<std::string>); //constructor
	void printSpecs();
	bool same(std::vector<std::string>);

	bool same(snow *s);

	double airTemp;
	double humidity;

	double snowTemp;

	int dirty;
	int newSnow;
	int artificial;
	int abrasive;
	int crystalSize;
	int moistureLevel;
};




#endif
