/*
 * DataManager.cpp is the main file that holds and deals with all global
 * variables; manipulates data; reads in, loads and saves data and etc.
 */

#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "wax.h"
#include "waxCombination.h"
#include "snow.h"
#include "rating.h"
#include "recommendation.h"

std::vector<recommendation> *getkNNRecommendations(snow *, int);

void getSnowDataVector(char *, std::vector<std::string> *);

std::vector<waxCombination> *getAllCombos();

/*
 * Loading functions
 */
void loadWaxData();
void loadTestData();
void loadSnowData();
void loadR();

/*
 * Saving functions
 */
void saveSnowData();
void saveTestData();
void saveWaxData();

/*
 * Reading in functions
 */
void readInNewTestData(char *);
void readInWaxData(char *);


/*
 * Find functions
 */
int findCombinationIndex(wax *, wax *, wax *);
int findSnowIndex(snow *);
wax *findWax(std::string *);


/*
 * Printing functions
 */
void printSnowByIndex(int i);
void printCombinationByIndex(int i);
void printAllCombinations();
void printAllSnow();
void printAllRatings();
void printAllWaxes();

#endif
