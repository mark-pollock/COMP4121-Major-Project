#include "wax.h"

using namespace std;
wax::wax(vector<string> v){
	auto it = v.begin();
	if((*it)[0] != '\0')
		name = *it;
	else
		name = "noName";
	++it;
	if((*it)[0] != '\0')
		minAirTemp = stod(*it);
	else
		minAirTemp = MIN_AIR_TEMP;
	++it;

	if((*it)[0] != '\0')
		maxAirTemp = stod(*it);
	else
		maxAirTemp = MAX_AIR_TEMP;
	++it;

	if((*it)[0] != '\0')
		minSnowTemp = stod(*it);
	else
		minSnowTemp = MIN_SNOW_TEMP;
	++it;

	if((*it)[0] != '\0')
		maxSnowTemp = stod(*it);
	else
		maxSnowTemp = MAX_SNOW_TEMP;
	++it;

	if((*it)[0] != '\0')
		minHumidity = stod(*it);
	else
		minHumidity = MIN_HUMIDITY;
	++it;

	if((*it)[0] != '\0')
		maxHumidity = stod(*it);
	else
		maxHumidity = MAX_HUMIDITY;
	++it;

	if((*it)[0] != '\0')
		dirty = stoi(*it);
	else
		dirty = 0;
	++it;

	if((*it)[0] != '\0')
		newSnow = stoi(*it);
	else
		newSnow = 0;
	++it;

	if((*it)[0] != '\0')
		old = stoi(*it);
	else
		old = 0;
	++it;

	if((*it)[0] != '\0')
		abrasive = stoi(*it);
	else
		abrasive = 0;
	++it;

	if((*it)[0] != '\0')
		artificial = stoi(*it);
	else
		artificial = 0;
	++it;

	if((*it)[0] != '\0')
		fine = stoi(*it);
	else
		fine = 0;
	++it;

	if((*it)[0] != '\0')
		transformed = stoi(*it);
	else
		transformed = 0;
	++it;

	if((*it)[0] != '\0')
		all = stoi(*it);
	else
		all = 0;
	++it;

	if((*it)[0] != '\0')
		wet = stoi(*it);
	else
		wet = 0;
	++it;

	if((*it)[0] != '\0')
		layer = stoi(*it);
	else{
		// TODO error message
	}
	assert(++it == v.end()); // making sure there is no more values.
}

void wax::printSpecs(){
	cout << name << endl;
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
	cout << "layer = " << layer << endl;
	cout << "----------------------------------------" << endl;
}


bool wax::canApplyOnTop(wax *w){
	//cout << "this layer = " << layer << endl;
	//cout << "other layer = " << w->layer << endl;
	//cout << "this name = " << name << endl;
	//cout << "other name = " << w->name << endl;
	assert(layer == w->layer - 1);

	if(((minAirTemp > w->minAirTemp) && (maxAirTemp > w->maxAirTemp + 2))
		|| ((minAirTemp < w->minAirTemp) && (maxAirTemp > w->maxAirTemp + 3)))
		return false;

	else if((maxAirTemp < w->minAirTemp) || (minAirTemp > w->maxAirTemp))
		return false;

	else if((maxHumidity < w->minHumidity) || (minHumidity > w->maxHumidity))
		return false;

	else
		return true;
}


bool wax::canApplyOnTop(waxCombination *wC){
	assert(layer == 1);

	if(((minAirTemp > wC->minAirTemp) && (maxAirTemp > wC->maxAirTemp + 2))
		|| ((minAirTemp < wC->minAirTemp) && (maxAirTemp > wC->maxAirTemp + 3)))
		return false;

	else if((maxAirTemp < wC->minAirTemp) || (minAirTemp > wC->maxAirTemp))
		return false;

	else if((maxHumidity < wC->minHumidity) || (minHumidity > wC->maxHumidity))
		return false;

	else
		return true;
}
