#include <iostream>
#include "tile.h"

Tile** createGrid() {
	Tile** g;

	//Create the Basic Empty Grid

	g = new Tile* [9];
	for (int i = 0; i < 9; i++)
		g[i] = new Tile[9];

	//Add game parameters
	{ //Extreme difficulty #1536 from https://www.sudoku-online.org/extremo.php, 107 iterations to beat
		g[0][3].setActualValue('4', 0);
		g[0][4].setActualValue('8', 0);
		g[0][5].setActualValue('7', 0);
		g[1][1].setActualValue('5', 0);
		g[1][3].setActualValue('9', 0);
		g[2][2].setActualValue('7', 0);
		g[2][6].setActualValue('2', 0);
		g[3][0].setActualValue('2', 0);
		g[3][2].setActualValue('1', 0);
		g[3][5].setActualValue('4', 0);
		g[3][7].setActualValue('9', 0);
		g[4][2].setActualValue('4', 0);
		g[4][6].setActualValue('6', 0);
		g[4][8].setActualValue('3', 0);
		g[5][1].setActualValue('9', 0);
		g[5][5].setActualValue('5', 0);
		g[6][0].setActualValue('3', 0);
		g[6][7].setActualValue('2', 0);
		g[7][2].setActualValue('8', 0);
		g[7][3].setActualValue('7', 0);
		g[7][7].setActualValue('3', 0);
		g[8][1].setActualValue('6', 0);
		g[8][4].setActualValue('1', 0);
		g[8][6].setActualValue('8', 0);
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
bool checkCorrect(Tile** grid,int x, int y, char num, int assumpLvl) {
	//Check row, column
	for (int i = 0; i < 9; i++){
		if (grid[x][i].getActualValue() == num || grid[i][y].getActualValue() == num)
			return false;
	}
	//Check quadrant
	int ii = (x / 3) * 3;
	int jj = (y / 3) * 3;
	for (int q = ii; q < 3; q++) {
		for (int qu = jj; qu < 3; qu++) {
			if (grid[q][qu].getActualValue() == num)
				return false;
		}
	}
	return true;
}
int main() {

	Tile** grid = createGrid();

	printGrid(grid);

	int gameStuck = 0;
	int assumptionLevel = 0;
	//std::vector<int[3]> assumptionIterator;//It will save k,l, iteration
	std::vector<std::vector<int>> assumptionIterator;
	//Lets start removing numbers
	for (int repeat = 0; repeat < 1000; repeat++) {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				if (grid[i][j].getActualValue() == 'X') {
					//Check column
					for (int c = 0; c < 9; c++) {
						if (grid[c][j].getActualValue() != 'X') {
							gameStuck+=grid[i][j].removePosibleValue(grid[c][j].getActualValue(),assumptionLevel);
							if (grid[i][j].getSize() == 1) {
								if(assumptionLevel==0)
									grid[i][j].setActualValue(grid[i][j].getPosibleValues()[0], assumptionLevel);
								else{
									if (checkCorrect(grid, i, j, grid[i][j].getPosibleValues()[0], assumptionLevel))
										grid[i][j].setActualValue(grid[i][j].getPosibleValues()[0], assumptionLevel);
									else
										gameStuck += grid[i][j].removePosibleValue(grid[i][j].getPosibleValues()[0], assumptionLevel);
								}
							}
						}
					}
					//Check row
					for (int r = 0; r < 9; r++) {
						if (grid[i][r].getActualValue() != 'X') {
							gameStuck += grid[i][j].removePosibleValue(grid[i][r].getActualValue(), assumptionLevel);
							if (grid[i][j].getSize() == 1) {
								if (assumptionLevel == 0)
									grid[i][j].setActualValue(grid[i][j].getPosibleValues()[0], assumptionLevel);
								else {
									if (checkCorrect(grid, i, j, grid[i][j].getPosibleValues()[0], assumptionLevel))
										grid[i][j].setActualValue(grid[i][j].getPosibleValues()[0], assumptionLevel);
									else
										gameStuck += grid[i][j].removePosibleValue(grid[i][j].getPosibleValues()[0], assumptionLevel);
								}
							}
						}
					}
					//Check quadrant
					int ii = (i / 3) * 3;
					int jj = (j / 3) * 3;
					for (int q = ii; q < 3; q++) {
						for (int qu = jj; qu < 3; qu++) {
							if (grid[q][qu].getActualValue() != 'X') {
								gameStuck += grid[i][j].removePosibleValue(grid[q][qu].getActualValue(), assumptionLevel);
								if (grid[i][j].getSize() == 1) {
									if (assumptionLevel == 0)
										grid[i][j].setActualValue(grid[i][j].getPosibleValues()[0], assumptionLevel);
									else {
										if (checkCorrect(grid, i, j, grid[i][j].getPosibleValues()[0], assumptionLevel))
											grid[i][j].setActualValue(grid[i][j].getPosibleValues()[0], assumptionLevel);
										else
											gameStuck += grid[i][j].removePosibleValue(grid[i][j].getPosibleValues()[0], assumptionLevel);
									}
								}
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
		//50/50 analysis, if two numbers just appear twice and on the same positions then the rest of numbers 
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
							if (grid[k][l].getActualValue() =='X' && grid[k][l].exists(num, assumptionLevel)) {
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
					if (counter == 1) {
						gameStuck++;
						if(assumptionLevel==0)
							grid[foundk][foundl].setActualValue(num, assumptionLevel);
						else {
							if(checkCorrect(grid,foundk,foundl,num,assumptionLevel))
								grid[foundk][foundl].setActualValue(num, assumptionLevel);
							else
								gameStuck += grid[foundk][foundl].removePosibleValue(num, assumptionLevel);
						}
					}
					else {
						if (counter == 2 || counter == 3) {
							if (foundk != -6) {//remove the value for the rest of the row
								for (int t = 0; t < 9; t++)	{
									if (t != j) {
										gameStuck += grid[foundk][t].removePosibleValue(num, assumptionLevel);
										if (grid[foundk][t].getSize() == 1) {
											if (assumptionLevel == 0)
												grid[foundk][t].setActualValue(grid[foundk][t].getPosibleValues()[0], assumptionLevel);
											else {
												if (checkCorrect(grid, foundk, t, grid[foundk][t].getPosibleValues()[0], assumptionLevel))
													grid[foundk][t].setActualValue(grid[foundk][t].getPosibleValues()[0], assumptionLevel);
												else
													gameStuck += grid[foundk][t].removePosibleValue(grid[foundk][t].getPosibleValues()[0], assumptionLevel);
											}
										}
									}
									else
										t += 2;
								}
							}
							else {//remove the value for the rest of the column
								for (int t = 0; t < 9; t++) {
									if (t != i) {
										gameStuck += grid[t][foundl].removePosibleValue(num, assumptionLevel);
										if (grid[t][foundl].getSize() == 1) {
											if (assumptionLevel == 0)
												grid[t][foundl].setActualValue(grid[t][foundl].getPosibleValues()[0], assumptionLevel);
											else {
												if (checkCorrect(grid, t, foundl, grid[t][foundl].getPosibleValues()[0], assumptionLevel))
													grid[t][foundl].setActualValue(grid[t][foundl].getPosibleValues()[0], assumptionLevel);
												else
													gameStuck += grid[t][foundl].removePosibleValue(grid[t][foundl].getPosibleValues()[0], assumptionLevel);
											}
										}
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
											gameStuck += grid[positions[it][0][0]][positions[it][0][1]].removePosibleValue(numm, assumptionLevel);
											gameStuck += grid[positions[jt][0][0]][positions[jt][0][1]].removePosibleValue(numm, assumptionLevel);
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
					if (grid[k][l].getActualValue() == 'X' && grid[k][l].exists(num, assumptionLevel)) {
						counter++;
						foundk = k;
						foundl = l;
					}
					if (counter > 1 || grid[k][l].getActualValue() == num) {
						counter = -1;
						break;
					}
				}
				if (counter == 1) {
					gameStuck++;
					if (assumptionLevel == 0)
						grid[foundk][foundl].setActualValue(num, assumptionLevel);
					else {
						if (checkCorrect(grid, foundk, foundl, num, assumptionLevel))
							grid[foundk][foundl].setActualValue(num, assumptionLevel);
						else
							gameStuck += grid[foundk][foundl].removePosibleValue(num, assumptionLevel);
					}
				}
			}
		}
		//Analyze if it is the last possible value of the column
		for (int k = 0; k < 9; k++) {
			for (char num = '1'; num <= '9'; num++) {
				counter = 0;
				for (int l = 0; l < 9; l++) {
					if (grid[l][k].getActualValue() == 'X' && grid[l][k].exists(num, assumptionLevel)) {
						counter++;
						foundk = k;
						foundl = l;
					}
					if (counter > 1 || grid[l][k].getActualValue() == num) {
						counter = -1;
						break;
					}
				}
				if (counter == 1) {
					gameStuck++;
					if (assumptionLevel == 0)
						grid[foundl][foundk].setActualValue(num, assumptionLevel);
					else {
						if (checkCorrect(grid, foundl, foundk, num, assumptionLevel))
							grid[foundl][foundk].setActualValue(num, assumptionLevel);
						else
							gameStuck += grid[foundl][foundk].removePosibleValue(num, assumptionLevel);
					}
				}
			}
		}
		int lowestSize = 5;
		int saveK, saveL;
		bool errorAssumption = true;
		int found = -1;
		bool remove = false;
		//Make a guess/assumption in case a stuck position is reached
		
		if (gameStuck == 0) {
			assumptionLevel++;
			restartassumption:
			// Look for the position with the lowest number of posibilities and choose one
			for (int k = 0; k < 9; k++) {
				for (int l = 0; l < 9; l++) {
					for (int i = 0; i < assumptionIterator.size(); i++) {
						if (assumptionIterator[i][0] == k && assumptionIterator[i][1] == l) {
							found = i;
							break;
						}
					}
					if (grid[k][l].getSize() < lowestSize && grid[k][l].getActualValue()=='X'&&(found == -1 || (found != -1 && assumptionIterator[found][2]<grid[k][l].getSize()))) {//I have to implement a way to not re
						saveK = k; //peat assumptions
						saveL = l;
						lowestSize = grid[k][l].getSize();
						errorAssumption = false;
					}
					found = -1;
				}
			}
			found = -1;
			if (errorAssumption){
				remove = true;
			}
			else {
					//This is an iterator where i save the actual possition being assumed and in case it ends
					//in an unwinable position we can select the next number in the list
					for (int i = 0; i < assumptionIterator.size(); i++) {
						if (assumptionIterator[i][0] == saveK && assumptionIterator[i][1] == saveL) {
							assumptionIterator[i][2]++;
							found = i;
							break;
						}
					}
					if (found == -1) {
						assumptionIterator.push_back({ saveK, saveL, 0 });
						found = assumptionIterator.size() - 1;
					}
					if (assumptionIterator[found][2] >= grid[saveK][saveL].getSize())
						remove = true;
					else {
						if(checkCorrect(grid,saveK,saveL, grid[saveK][saveL].getPosibleValues()[assumptionIterator[found][2]],assumptionLevel))
							grid[saveK][saveL].setActualValue(grid[saveK][saveL].getPosibleValues()[assumptionIterator[found][2]], assumptionLevel);
						else {
							gameStuck += grid[saveK][saveL].removePosibleValue(grid[saveK][saveL].getPosibleValues()[assumptionIterator[found][2]], assumptionLevel);
						}
					}
				}
			}
		gameStuck = 0;
		//look if imposible position is reached, if a certain tile has no numbers available
		// then remove the last assumption from all the board
		for (int k = 0; k < 9; k++) {
			for (int l = 0; l < 9; l++) {
				if (assumptionLevel!=0 && (remove || (grid[k][l].getSize() == 0 && grid[k][l].getActualValue()=='X'))) {
					if (remove)
						assumptionLevel--;
					for (int k = 0; k < 9; k++)	{
						for (int l = 0; l < 9; l++)	{
							if (grid[k][l].getAssumptionLevel()==assumptionLevel) {
								grid[k][l].removeAssumption();
							}
						}
					}
					assumptionLevel--;
					goto end;//Now the next assumption should be made
				}
			}
		}
		end:
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
