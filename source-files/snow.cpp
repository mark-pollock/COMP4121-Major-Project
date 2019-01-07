#include "snow.h"

using namespace std;

snow::snow(vector<string> v){
	auto it = v.begin();
	if((*it)[0] != '\0')
		airTemp = stod(*it);
	else{
		//error need air temp
	}
	++it;
	if((*it)[0] != '\0')
		humidity = stod(*it);
	else{
		//todo check
		humidity = -1;
	}
	++it;

	if((*it)[0] != '\0')
		snowTemp = stod(*it);
	else{
		//todo check
		snowTemp = -1;
	}
	++it;

	if((*it)[0] != '\0')
		dirty = stoi(*it);
	else{
		dirty = (CLEAN + DIRTY) / 2;
	}
	++it;

	if((*it)[0] != '\0')
		newSnow = stoi(*it);
	else{
		newSnow = (NEW + OLD) / 2;
	}
	++it;

	if((*it)[0] != '\0')
		artificial = stoi(*it);
	else
		artificial = (ARTIFICIAL + NATURAL) / 2;
	++it;

	if((*it)[0] != '\0')
		abrasive = stoi(*it);
	else
		abrasive = (ABRASIVE + SOFT) / 2;
	++it;

	if((*it)[0] != '\0')
		crystalSize = stoi(*it);
	else
		crystalSize = (FINE + TRANSFORMED) / 2;
	++it;

	if((*it)[0] != '\0')
		moistureLevel = stoi(*it);
	else
		moistureLevel = (WET + DRY) / 2;
	assert(++it == v.end()); // making sure there is no more values.
}


bool snow::same(vector<string> v){
	snow tempSnow(v);
	if(tempSnow.airTemp == airTemp && tempSnow.humidity == humidity &&
			tempSnow.snowTemp == snowTemp && tempSnow.dirty == dirty &&
			tempSnow.newSnow == newSnow && tempSnow.artificial == artificial &&
			tempSnow.abrasive == abrasive && tempSnow.crystalSize == crystalSize &&
			tempSnow.moistureLevel == moistureLevel)
		return true;
	else
		return false;
}

bool snow::same(snow *s){
	if(s->airTemp == airTemp && s->humidity == humidity &&
			s->snowTemp == snowTemp && s->dirty == dirty &&
			s->newSnow == newSnow && s->artificial == artificial &&
			s->abrasive == abrasive && s->crystalSize == crystalSize &&
			s->moistureLevel == moistureLevel)
		return true;
	else
		return false;
}

void snow::printSpecs(){
	cout << "----------------------------------------" << endl;
	cout << "Snow Specs:" << endl;
	cout << "Air Temp: " << airTemp << endl;
	cout << "Humidity: " << humidity << endl;
	cout << "Snow Temp: " << snowTemp << endl;
	cout << "Dirtiness (Dirty = " << DIRTY << " .. Clean = " << CLEAN
			<< "): " << dirty << endl;
	cout << "Newness (New = " << NEW << " .. Old = " << OLD << "): "
			<< newSnow << endl;
	cout << "Naturalness (Artificial = " << ARTIFICIAL << " .. Natural = " << NATURAL
			<< "): " << artificial << endl;
	cout << "Abrasiveness (Abrasive = " << ABRASIVE << " .. Soft = " << SOFT
			<< "): " << abrasive << endl;
	cout << "Crystal Size (Fine = " << FINE << " .. Transformed = " << TRANSFORMED
			<< "): " << crystalSize << endl;
	cout << "Moisture Level (Wet = " << WET << " .. Dry = " << DRY
			<< "): " << moistureLevel << endl;
	cout << "----------------------------------------" << endl;
}
