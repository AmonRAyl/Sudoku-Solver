#include <iostream>
#include "tile.h"

Tile** createGrid() {
	Tile** g;

	//Create the Basic Empty Grid

	g = new Tile* [9];
	for (int i = 0; i < 9; i++)
		g[i] = new Tile[9];

	//Add game parameters
	{

	}
	return g;
}
void printGrid(Tile** grid) {
	std::cout << "-------------------------" << std::endl;
	for (int i = 0; i < 9; i++) {
		std::cout << "| ";
		for (int j = 0; j < 9; j++) {
			std::cout << grid[i][j].getActualValue() << " ";
			if ((j + 1) % 3 == 0)
				std::cout << "| ";
		}
		std::cout << std::endl;
		if((i+1)%3==0)
			std::cout << "-------------------------" << std::endl;
	}
}
bool checkComplete(Tile** grid) {
	for (int i = 0; i < 9; i++)	{
		for (int j = 0; j < 9; j++)	{
			if (grid[i][j].getActualValue() == 'X')
				return false;
		}
	}
	return true;
}

int main() {

	Tile** grid = createGrid();

	printGrid(grid);

	//Lets start removing numbers
	for (int repeat = 0; repeat < 1000; repeat++) {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				if (grid[i][j].getActualValue() == 'X') {
					//Check column
					for (int c = 0; c < 9; c++) {
						if (grid[c][j].getActualValue() != 'X') {
							grid[i][j].removePosibleValue(grid[c][j].getActualValue());
							if (grid[i][j].getSize() == 1)
								grid[i][j].setActualValue(grid[i][j].getPosibleValues()[0]);
						}
					}
					//Check row
					for (int r = 0; r < 9; r++) {
						if (grid[i][r].getActualValue() != 'X') {
							grid[i][j].removePosibleValue(grid[i][r].getActualValue());
							if (grid[i][j].getSize() == 1)
								grid[i][j].setActualValue(grid[i][j].getPosibleValues()[0]);
						}
					}
					//Check quadrant
					int ii = (i / 3) * 3;
					int jj = (j / 3) * 3;
					for (int q = ii; q < 3; q++) {
						for (int qu = jj; qu < 3; qu++) {
							if (grid[q][qu].getActualValue() != 'X') {
								grid[i][j].removePosibleValue(grid[q][qu].getActualValue());
								if (grid[i][j].getSize() == 1)
									grid[i][j].setActualValue(grid[i][j].getPosibleValues()[0]);
							}
						}
					}
				}
			}
		}

		int counter;
		int foundk;
		int foundl;
		//Analyze values in each quadrant + Advanced analysis, check if the only posible positions for a number
		//are on the same column or row in a quadrant, if so remove that number from the rest of the row/column.
		//50/50 analysis missing if two numbers just appear twice and on the same positions then the rest of numbers 
		//from those position can be removed.
		std::vector<std::vector<std::vector<int>>> positions;
		//Analyze whether there is a 50/50 within the same quadrant
		for (int i = 0; i < 9; i += 3) {
			for (int j = 0; j < 9; j += 3) {
				//Count the number of tiles that each number has, if any of those is 1 then we have an answer.
				positions = std::vector<std::vector<std::vector<int>>>(10);
				for (char num = '1'; num <= '9'; num++) {
					int numi = num - '0';
					counter = 0;
					foundk = -1;
					foundl = -1;
					for (int k = i; k < i+3; k++) {
						for (int l = j; l < j+3; l++) {
							if (grid[k][l].getActualValue() =='X' && grid[k][l].exists(num)) {
								counter++;
								positions[numi].push_back({k,l});
								if (foundk == -1) {
									foundk = k;
									foundl = l;
								}
								else { //This code looks if the number is contained within the same row or column
									if (k == foundk || l == foundl) {
										if (k == foundk)
											foundl = -6;
										else
											foundk = -6;
									}
									else {
										counter = 6;
									}
								}
							}
							if (counter > 3 || grid[k][l].getActualValue() == num) {
								counter = -1;
								break;
							}
						}
						if (counter == -1)
							break;
					}
					if (counter != 2)
						positions[numi].clear();
					if(counter==1)
						grid[foundk][foundl].setActualValue(num);
					else {
						if (counter == 2 || counter == 3) {
							if (foundk != -6) {//remove the value for the rest of the row
								for (int t = 0; t < 9; t++)	{
									if (t != j) {
										grid[foundk][t].removePosibleValue(num);
										if (grid[foundk][t].getSize() == 1)
											grid[foundk][t].setActualValue(grid[foundk][t].getPosibleValues()[0]);
									}
									else
										t += 2;
								}
							}
							else {//remove the value for the rest of the column
								for (int t = 0; t < 9; t++) {
									if (t != i) {
										grid[t][foundl].removePosibleValue(num);
										if (grid[t][foundl].getSize() == 1)
											grid[t][foundl].setActualValue(grid[t][foundl].getPosibleValues()[0]);
									}
									else
										t += 2;
								}
							}
						}
					}
				}
				//Analyze 50 50
				for (int it = 1; it <= 9; it++) {
					if (positions[it].size() == 2) {
						for (int jt = it+1; jt <= 9; jt++) {
							if (positions[jt].size() == 2) {
								if ((positions[it][0] == positions[jt][0] && positions[it][1] == positions[jt][1])
									|| (positions[it][0] == positions[jt][1] && positions[it][1] == positions[jt][0])) {
									//Found a 50 50 , remove the other numbers
									for (char numm = '1'; numm <= '9'; numm++) {
										int nummi = numm - '0';
										if (nummi != it && nummi != jt) {
											grid[positions[it][0][0]][positions[it][0][1]].removePosibleValue(numm);
											grid[positions[jt][0][0]][positions[jt][0][1]].removePosibleValue(numm);
										}
									}
								}
							}
						}
					}
				}
			}
		}
		//Analyze if it is the last possible value of the row
		for (int k = 0; k < 9; k++) {
			for (char num = '1'; num <= '9'; num++) {
				counter = 0;
				for (int l = 0; l < 9; l++)	{
					if (grid[k][l].getActualValue() == 'X' && grid[k][l].exists(num)) {
						counter++;
						foundk = k;
						foundl = l;
					}
					if (counter > 1 || grid[k][l].getActualValue() == num) {
						counter = -1;
						break;
					}
				}
				if (counter==1)	
					grid[foundk][foundl].setActualValue(num);
			}
		}
		//Analyze if it is the last possible value of the column
		for (int k = 0; k < 9; k++) {
			for (char num = '1'; num <= '9'; num++) {
				counter = 0;
				for (int l = 0; l < 9; l++) {
					if (grid[l][k].getActualValue() == 'X' && grid[l][k].exists(num)) {
						counter++;
						foundk = k;
						foundl = l;
					}
					if (counter > 1 || grid[l][k].getActualValue() == num) {
						counter = -1;
						break;
					}
				}
				if (counter == 1)
					grid[foundl][foundk].setActualValue(num);
			}
		}
		system("cls");
		printGrid(grid);	
		if (checkComplete(grid)) {
			std::cout << "Sudoku completed in " << repeat << " iterations" << std::endl;
			break;
		}
	}
	system("pause");
	return 0;
}
