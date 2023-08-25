#pragma once
#include <vector>

class Tile {
public:
	Tile();

	std::vector<char> getPosibleValues();
	char getActualValue();
	int getSize();
	bool exists(char f, int assmpLvl);
	int getAssumptionLevel();

	void removeAssumption();
	void setActualValue(char val,int assumpLvl);
	int removePosibleValue(char val,int assmpLvl);

private:
	std::vector<char> posibleValues;
	char actualValue;
	int assumptionLevel;
	char actualValueAssumption;
	std::vector<char> posibleValuesAssumption;
};
