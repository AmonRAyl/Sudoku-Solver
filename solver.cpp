/*
	Ideas for solving sudoku:
	1) First, select possible numbers for each position by checking horizontally, vertically, and within the quadrant. If only one number is 
	found for a certain position, then place it and reevaluate all the affected positions, including the same column, row, and quadrant.

	2) Once this is completed, if the game becomes stuck, look for a number with a limited number of possibilities, then make an 
	assumption and repeat step 1. Continue this process until a cell has no available numbers. If the game remains stuck, repeat step 2. If an
	empty cell with no available numbers is found, restore the position before the last assumption was made and make a different assumption.

	3)
*/

int main() {







	return 0;
}
