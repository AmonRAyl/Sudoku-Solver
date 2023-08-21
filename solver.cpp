/*
	Ideas for solving sudoku:
	1) First, select possible numbers for each position by checking horizontally, vertically, and within the quadrant. If only one number is 
	found for a certain position, then place it and reevaluate all the affected positions, including the same column, row, and quadrant.

	2) Once this is completed, if the game becomes stuck, look for a number with a limited number of possibilities, then make an 
	assumption and repeat step 1. Continue this process until a cell has no available numbers. If the game remains stuck, repeat step 2. If an
	empty cell with no available numbers is found, restore the position before the last assumption was made and make a different assumption.

	3)It should also be checked if that position is the last one of the quadrant/row/column that can have a specific value.

	4)Some basic understanding is missing, example:
		| 1 2 3 |
		| 4 a 5 | If the letters beneath 2 have the following possibilities:
		| b c d | a=89 b=789 c=89 d=6789 e=6 f=X
		---------- this means that in b the only real option is 7 as 89 are in a and c.
		| X X X |
		| e f X |
		| X X X |
		Also if for example we have the following options for a=b=c=d=6789 and e=X f=9
		in b or d has to be the other 9 which can be used in horizontal quadrants.
	5) A paramaeter to check if the game is stuck should be added , as if the game remains stuck for an iteration an assumption may be made
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
		g[0][3].setActualValue('3');
		g[0][4].setActualValue('8');
		g[0][5].setActualValue('4');
		g[0][7].setActualValue('5');
		g[1][3].setActualValue('1');
		g[1][4].setActualValue('2');
		g[1][5].setActualValue('5');
		g[1][6].setActualValue('8');
		g[2][0].setActualValue('4');
		g[2][1].setActualValue('8');
		g[2][4].setActualValue('9');
		g[2][6].setActualValue('1');
		g[3][0].setActualValue('5');
		g[3][7].setActualValue('9');
		g[4][0].setActualValue('6');
		g[4][1].setActualValue('3');
		g[4][4].setActualValue('5');
		g[4][5].setActualValue('1');
		g[5][2].setActualValue('4');
		g[5][5].setActualValue('9');
		g[5][6].setActualValue('3');
		g[5][7].setActualValue('7');
		g[6][0].setActualValue('7');
		g[6][3].setActualValue('5');
		g[6][5].setActualValue('6');
		g[6][7].setActualValue('2');
		g[7][2].setActualValue('2');
		g[7][5].setActualValue('3');
		g[7][8].setActualValue('7');
		g[8][3].setActualValue('2');
		g[8][5].setActualValue('8');
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
	for (int repeat = 0; repeat < 1000; repeat++)
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

		//Analyze values in each quadrant + Advanced analysis, check if the only posible positions for a number
		//are on the same column or row in a quadrant, if so remove that number from the rest of the row/column.
		//50/50 analysis missing if two numbers just appear twice and on the same positions then the rest of numbers 
		//from those position can be removed.****************************
		int counter;
		int foundk;
		int foundl;
		for (int i = 0; i < 9; i += 3) {
			for (int j = 0; j < 9; j += 3) {
				//Count the number of tiles that each number has, if any of those is 1 then we have an answer.
				for (char num = '1'; num <= '9'; num++) {
					counter = 0;
					foundk = -1;
					foundl = -1;
					for (int k = i; k < i+3; k++) {
						for (int l = j; l < j+3; l++) {
							bool y = grid[k][l].exists(num);
							if (grid[k][l].getActualValue() =='X' && grid[k][l].exists(num)) {
								counter++;
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
					if(counter==1)
						grid[foundk][foundl].setActualValue(num);
					else {
						if (counter == 2 || counter == 3) {
							printGrid(grid);
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
			}
		}
		//Analyze if it is the last possible value of the row
		for (int k = 0; k < 9; k++) {
			for (char num = 1; num <= 9; num++) {
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
			for (char num = 1; num <= 9; num++) {
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
