#include "waxCombination.h"

using namespace std;
waxCombination::waxCombination(wax *w1, wax *w2){
	assert(w1->layer != w2->layer);
	assert(w1->layer == w2->layer - 1);
	if(w1->layer == 1)
		layer1 = w1;
	else if(w1->layer == 2)
		layer2 = w1;
	else if(w1->layer == 3)
		layer3 = w1;
	else{
		//TODO error layer 4
	}

	if(w2->layer == 1)
		layer1 = w2;
	else if(w2->layer == 2)
		layer2 = w2;
	else if(w2->layer == 3)
		layer3 = w2;
	else{
		//TODO error layer 4
	}
	numLayers = 2;
	minAirTemp = min(w1->minAirTemp, w2->minAirTemp);
	maxAirTemp = max(w1->maxAirTemp, w2->maxAirTemp);
	minSnowTemp = min(w1->minSnowTemp, w2->minSnowTemp);
	maxSnowTemp = max(w1->maxSnowTemp, w2->maxSnowTemp);
	fine = max(w1->fine, w2->fine);
	dirty = max(w1->dirty, w2->dirty);
	artificial = max(w1->artificial, w2->artificial);
	all = max(w1->all, w2->all);
	abrasive = max(w1->abrasive, w2->abrasive);
	minHumidity = min(w1->minHumidity, w2->minHumidity);
	maxHumidity = max(w1->maxHumidity, w2->maxHumidity);
	newSnow = max(w1->newSnow, w2->newSnow);
	old = max(w1->old, w2->old);
	transformed = max(w1->transformed, w2->transformed);
	wet = max(w1->wet, w2->wet);
}

waxCombination::waxCombination(waxCombination *wC, wax *w){
	//assert(wC->layer1 == NULL);
	assert(w->layer == 1);
	layer1 = w;
	layer2 = wC->layer2;
	layer3 = wC->layer3;
	numLayers = 3;
	minAirTemp = min(wC->minAirTemp, w->minAirTemp);
	maxAirTemp = max(wC->maxAirTemp, w->maxAirTemp);
	minSnowTemp = min(wC->minSnowTemp, w->minSnowTemp);
	maxSnowTemp = max(wC->maxSnowTemp, w->maxSnowTemp);
	fine = max(wC->fine, w->fine);
	dirty = max(wC->dirty, w->dirty);
	artificial = max(wC->artificial, w->artificial);
	all = max(wC->all, w->all);
	abrasive = max(wC->abrasive, w->abrasive);
	minHumidity = min(wC->minHumidity, w->minHumidity);
	maxHumidity = max(wC->maxHumidity, w->maxHumidity);
	newSnow = max(wC->newSnow, w->newSnow);
	old = max(wC->old, w->old);
	transformed = max(wC->transformed, w->transformed);
	wet = max(wC->wet, w->wet);
}

waxCombination::waxCombination(wax *w1, wax *w2, wax *w3){
	layer1 = w1;
	layer2 = w2;
	layer3 = w3;
	numLayers = 3;
	minAirTemp = min(w1->minAirTemp, min(w2->minAirTemp, w3-> minAirTemp));
	maxAirTemp = max(w1->maxAirTemp, max(w2->maxAirTemp, w3-> maxAirTemp));
	minSnowTemp = min(w1->minSnowTemp, min(w2->minSnowTemp, w3-> minSnowTemp));
	maxSnowTemp = max(w1->maxSnowTemp, max(w2->maxSnowTemp, w3-> maxSnowTemp));
	minHumidity = min(w1->minHumidity, min(w2->minHumidity, w3-> minHumidity));
	maxHumidity = max(w1->maxHumidity, max(w2->maxHumidity, w3-> maxHumidity));

	fine = max(w1->fine, max(w2->fine, w3->fine));
	dirty = max(w1->dirty, max(w2->dirty, w3->dirty));
	artificial = max(w1->artificial, max(w2->artificial, w3->artificial));
	all = max(w1->all, max(w2->all, w3->all));
	abrasive = max(w1->abrasive, max(w2->abrasive, w3->abrasive));

	newSnow = max(w1->newSnow, max(w2->newSnow, w3->newSnow));
	old = max(w1->old, max(w2->old, w3->old));
	transformed = max(w1->transformed, max(w2->transformed, w3->transformed));
	wet = max(w1->wet, max(w2->wet, w3->wet));
}
void waxCombination::printSpecs(){
	cout << "----------------------------------------" << endl;
	cout << "Wax Combination Specs:" << endl;
	cout << "layer 1 = " << layer1->name << endl;
	cout << "layer 2 = " << layer2->name << endl;
	if(numLayers == 3)
		cout << "layer 3 = " << layer3->name << endl;
	cout << "Air Temp: " << minAirTemp << " - " << maxAirTemp << endl;
	cout << "Snow Temp: " << minSnowTemp << " - " << maxSnowTemp << endl;
	cout << "Humidity: " << minHumidity << " - " << maxHumidity << endl;
	cout << "dirty = " << dirty << endl;
	cout << "newSnow = " << newSnow << endl;
	cout << "old = " << old << endl;
	cout << "abrasive = " << abrasive << endl;
	cout << "artificial = " << artificial << endl;
	cout << "fine = " << fine << endl;
	cout << "transformed = " << transformed << endl;
	cout << "all = " << all << endl;
	cout << "wet = " << wet << endl;
	cout << "----------------------------------------" << endl;
}

void waxCombination::printSpecsShort(){
	cout << "\t Layer 1 = " << layer1->name << endl;
	cout << "\t Layer 2 = " << layer2->name << endl;
	if(numLayers == 3)
		cout << "\t Layer 3 = " << layer3->name << endl;
}

bool waxCombination::same(waxCombination *w){
	if(w->layer1->name.compare(layer1->name) != 0
			|| w->layer2->name.compare(layer2->name) != 0)
		return false;

	if(numLayers == 2 && w->numLayers == 2)
		return true;

	if(numLayers != w->numLayers)
		return false;

	if(w->layer3->name.compare(layer3->name) != 0)
		return false;

	return true;
}
