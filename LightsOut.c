/* 
 * @author: Cecilia La Place
 * Date: 1/21/16
 * @version: 1.0
 * Program: Lights Out
 * Description: The user will choose a location in a 5x5 grid to "click" and 
 * 				turn "on" or "off" the lights. The goal of the game is to 
 *				turn all the lights on contrary to the title. The second goal
 *				is to turn on all the lights as efficiently as possible.
 */


#include <stdio.h>
#include <stdbool.h>

int checkIntInput(int loc);
void lightChange(bool board [7][7], int locX, int locY);
void printBoard(bool board[7][7]);
bool isLit(bool board [7][7]);
void reset(bool board[7][7]);

int main(){
	//define variables
	bool board [7][7];
	char input[6];
	int xCor;
	int yCor;
	bool correct = false; //used as a boolean
	bool gameOver = false; //used as a boolean

	reset(board);

	//Introductory statements
	printf("Welcome to Lights Out. This game will be text based.\n");
	printf("Coordinate input will be taken separately and prompted for.\n");
	printf("For example: \n");
	while (!correct){
		printf("Please enter an x coordinate between 1 and 5 (including): ");
		scanf("%s", input);
		xCor = atoi(input);
		correct = checkIntInput(xCor);
		if (!correct)
			printf("Please enter a correct input.");
	}
	correct = false;
	while (!correct){
		printf("Please enter an y coordinate between 1 and 5 (including): ");
		scanf("%s", input);
		yCor = atoi(input);
		correct = checkIntInput(yCor);
		if (!correct)
			printf("Please enter a correct input.");
	}
	lightChange(board, xCor, yCor);
	printBoard(board);
	printf("You'll notice that the x coordinate %d and the y coordinate ", xCor);
	printf("%d\nis in the relative center of the all the x's. ", yCor);
	printf("Your goal is to \nmake the whole board x's.\n");
	printf("\nIf you would like to reset the game at any point, it \n");
	printf("will be asked prior to the next prompts for x and y coordinates.\n");
	printf("Simply enter y or n when it does\n");

	reset(board);
	while (!gameOver){
		//Get coordinate input
		correct = false;
		while (!correct){
			printf("Please enter an x coordinate between 1 and 5 (including): ");
			scanf("%s", input);
			xCor = atoi(input);
			correct = checkIntInput(xCor);
			if (!correct)
				printf("Please enter a correct input.");
		}
		correct = false;
		while (!correct){
			printf("Please enter an y coordinate between 1 and 5 (including): ");
			scanf("%s", input);
			yCor = atoi(input);
			correct = checkIntInput(yCor);
			if (!correct)
				printf("Please enter a correct input.");
		}
		//Update then print board
		lightChange(board, xCor, yCor);
		printBoard(board);

		//Ask user if they would like to reset the board
		printf("Reset? ");
		scanf("%s", input);
		if (input[0] == 'y' || input[0] == 'Y')
			reset(board);
		
		if (isLit(board))
			gameOver = true;
	}
	printf("You won!");
	reset(board);
}

/* Function:	checkIntInput
 * --------------------------
 * @param: loc - coordinate input
 *
 * Determines if the user has input a number between 1 and 5 inclusive
 * 
 * @return: 1 (true) if input is okay, 0 (false) if input is not okay
 */
int checkIntInput(int loc){

	if (loc > 5 || loc < 1){
		return 0;
	}
	return 1; 
}

/* Function:	lightChange
 * --------------------------
 * @param: board - passes direct reference to the integer board 2D array
 * @param: locX - passes copy of user inputted x coordinate
 * @param: locY - passes copy of user inputted y coordinate
 *
 * Change the lights adjacent to and at the location chosen
 */
void lightChange(bool board [7][7], int locX, int locY){
	board [locY][locX] = !board [locY][locX]; /*If this location is true, make it false*/
	board [locY+1][locX] = !board [locY+1][locX];
	board [locY-1][locX] = !board [locY-1][locX];
	board [locY][locX+1] = !board [locY][locX+1];
	board [locY][locX-1] = !board [locY][locX-1];
}

/* Function:	printBoard
 * --------------------------
 * @param: board - passes direct reference to the integer board 2D array
 * 
 * Prints the board. If a light is on an 'x' is used instead of an 'o' 
 */
void printBoard(bool board [7][7]){
	int y;
	int x;
	for (y = 1; y <= 5; y++){
		for (x = 1; x <= 5; x++){
			/*If 1 print 'x ' else print 'o ', space for formatting*/
			(board [x][y]) ? printf("x ") : printf("o "); 
		}
		printf("\n");
	}
}

/* Function:	isLit
* -------------------------
* 
* @param: board - passes direct reference to the integer board 2D array
* 
* @return: boolean value if board is completely "lit up" or not
*/
bool isLit(bool board [7][7]){
	int x;
	int y;
	for (x = 1; x <= 5; x++){
		for (y = 1; y <= 5; y++){
			if (!board[x][y])
				return false;
		}
	}
	return true;
}

/* Function:	reset
 * --------------------------
 * @param: board - passes direct reference to the integer board 2D array
 *
 * Resets the board when game complete or string input of reset
 */
void reset(bool board [7][7]){
	int y;
 	int x;
 	for (y = 0; y < 7; y++){
 		for (x = 0; x < 7; x++){
 			board [x][y] = false; 
 		}
 	}
 }
