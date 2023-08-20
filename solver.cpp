/*
	Ideas for solving sudoku:
	1) First, select possible numbers for each position by checking horizontally, vertically, and within the quadrant. If only one number is 
	found for a certain position, then place it and reevaluate all the affected positions, including the same column, row, and quadrant.

	2) Once this is completed, if the game becomes stuck, look for a number with a limited number of possibilities, then make an 
	assumption and repeat step 1. Continue this process until a cell has no available numbers. If the game remains stuck, repeat step 2. If an
	empty cell with no available numbers is found, restore the position before the last assumption was made and make a different assumption.

	3)It should also be checked if that position is the last one of the quadrant/row/column that can have a specific value.
*/
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
		g[0][1].setActualValue('3');
		g[0][2].setActualValue('4');
		g[0][3].setActualValue('9');
		g[0][4].setActualValue('1');
		g[0][5].setActualValue('7');
		g[1][0].setActualValue('5');
		g[1][4].setActualValue('6');
		g[1][8].setActualValue('9');
		g[2][2].setActualValue('1');
		g[3][0].setActualValue('6');
		g[3][3].setActualValue('1');
		g[3][5].setActualValue('8');
		g[3][8].setActualValue('5');
		g[4][0].setActualValue('2');
		g[4][1].setActualValue('4');
		g[4][7].setActualValue('1');
		g[4][8].setActualValue('7');
		g[5][0].setActualValue('1');
		g[5][3].setActualValue('3');
		g[5][5].setActualValue('2');
		g[5][8].setActualValue('6');
		g[6][6].setActualValue('5');
		g[7][0].setActualValue('7');
		g[7][4].setActualValue('9');
		g[7][8].setActualValue('8');
		g[8][3].setActualValue('5');
		g[8][4].setActualValue('3');
		g[8][5].setActualValue('6');
		g[8][6].setActualValue('7');
		g[8][7].setActualValue('9');
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
	for (int repeat = 0; repeat < 100; repeat++)
	{
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

		//Analyze values in each quadrant
		int counter = 0;
		int foundk, foundl;
		for (int i = 0; i < 9; i += 3) {
			for (int j = 0; j < 9; j += 3) {
				//Count the number of tiles that each number has, if any of those is 1 then we have an answer.
				for (char num = '1'; num <= '9'; num++) {
					counter = 0;
					for (int k = i; k < i+3; k++) {
						for (int l = j; l < j+3; l++) {
							bool y = grid[k][l].exists(num);
							if (grid[k][l].getActualValue() =='X' && grid[k][l].exists(num)) {
								counter++;
								foundk = k;
								foundl = l;
							}
							if (counter > 1 || grid[k][l].getActualValue() == num) {
								counter = -1;
								break;
							}
						}
						if (counter == -1)
							break;
					}
					if(counter==1)
						grid[foundk][foundl].setActualValue(num);
				}
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
