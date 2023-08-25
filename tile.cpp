#include "tile.h"

Tile::Tile() {
	for (char i = '1'; i <= '9'; i++)
		posibleValues.push_back(i);
	actualValue = 'X';
	assumptionLevel = 0;
	actualValueAssumption = actualValue;
	posibleValuesAssumption = posibleValues;
}

std::vector<char> Tile::getPosibleValues() {
	if(assumptionLevel==0)
		return posibleValues;
	return posibleValuesAssumption;
}

char Tile::getActualValue() {
	if(assumptionLevel==0)
		return actualValue;
	return actualValueAssumption;
}

int Tile::getSize() {
	if(assumptionLevel==0)
		return posibleValues.size();
	return posibleValuesAssumption.size();
}

bool Tile::exists(char f, int assumpLvl) {
	if (assumpLvl == 0) {
		auto pos = std::find(posibleValues.begin(), posibleValues.end(), f);
		if (pos != posibleValues.end()) {
			return true;
		}
		return false;
	}
	auto pos = std::find(posibleValuesAssumption.begin(), posibleValuesAssumption.end(), f);
	if (pos != posibleValuesAssumption.end()) {
		return true;
	}
	return false;
}

int Tile::getAssumptionLevel() {
	return assumptionLevel;
}

void Tile::removeAssumption() {
	assumptionLevel = 0;
	actualValueAssumption = actualValue;
	posibleValuesAssumption = posibleValues;
}

void Tile::setActualValue(char val, int assumpLvl) {
	if (assumptionLevel != assumpLvl)
		assumptionLevel = assumpLvl;
	if (assumptionLevel == 0)
		actualValue = val;
	actualValueAssumption = val;
}
int Tile::removePosibleValue(char val, int assumpLvl) {
	if (assumpLvl == 0) {
		auto pos = std::find(posibleValues.begin(), posibleValues.end(), val);
		auto pos2 = std::find(posibleValuesAssumption.begin(), posibleValuesAssumption.end(), val);
		if (pos != posibleValues.end()) {
			posibleValues.erase(pos);
			if(pos2 != posibleValuesAssumption.end())
				posibleValuesAssumption.erase(pos2);
			return 1;
		}
		return 0;
	}
	if (assumptionLevel != assumpLvl)
		assumptionLevel = assumpLvl;
	auto pos = std::find(posibleValuesAssumption.begin(), posibleValuesAssumption.end(), val);
	if (pos != posibleValuesAssumption.end()) {
		posibleValuesAssumption.erase(pos);
		return 1;
	}
	return 0;
}
