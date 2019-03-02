/* 
 * @author: Cecilia La Place
 * Date: 3/10/16
 * @version: 2.0
 * Program: Lights Out
 * Description: The user will choose a location in a 5x5 grid to "click" and 
 * 				turn "on" or "off" the lights. The goal of the game is to 
 *				turn all the lights off. The second goal is to turn off all 
 *				the lights as efficiently as possible. (Keep in mind not all
 *				games are solvable.)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include <SDL.h>

// Define our constants.
#define SCREEN_WIDTH 420
#define SCREEN_HEIGHT 420
static int GRID_SIZE;	// This will be the width and height of each grid space

void lightChange(bool board [9][9], int locX, int locY);
bool isOff(bool board [9][9], int boardSize);
void reset(bool board[9][9], int boardSize);

int main(int argv, char **argc) {
	srand(time(NULL));

	int boardSize = 1;
	determineGridSize(boardSize);
	bool board [9][9];
	reset(board, boardSize);

	// Initialize SDL. You can choose to only initialize parts of the library,
	// but I don't know why you would do that.
	int err = SDL_Init(SDL_INIT_EVERYTHING);
	if (err != 0) {
		// We couldn't initialize SDL for some reason.
		printf("failed to init SDL: %s", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	// Create the window object.
	SDL_Window *window = SDL_CreateWindow("Lights Out (Turn Off All the Lights)", // Window title
			// Where the window should be placed on-screen. UNDEFINED means
			// that the window manager gets to decide.
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			// The size of the window we're making in pixels
			SCREEN_WIDTH, SCREEN_HEIGHT,
			// Miscellaneous other flags. SDL_WINDOW_SHOWN makes the window
			// appear as soon as it's made. SDL_WINDOW_OPENGL means we want
			// SDL to make an OpenGL context for us. OpenGL contexts are where
			// we can draw stuff using the graphics card.
			SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	// Pretty much everything in SDL returns NULL if it fails
	if (window == NULL) {
		printf("failed to open window: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	// Create the renderer. The renderer controls the actual drawing. We give
	// it the window so it can use that OpenGL context we had the window make.
	// SDL_RENDERER_ACCELERATED forces the renderer to use the graphics card.
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		printf("failed to create renderer: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	//Allow user to quit or if went through the whole sequence
	bool quit = false;
	bool gameOver = false;
	SDL_Event event;
	// Start our main loop
	while (!gameOver && !quit) {
		SDL_Rect squares[49];
		int count = 0;

		// SDL queues up events for us while we're doing other things. This
		// while loop runs for every event it's queued up for us until SDL is
		// out of events. Events are, for our purposes, pretty much always user
		// input.
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				// If the event is a quit request (like if the user presses the
				// X button), break out of our main loop.
				quit = true;
			} else if (event.type == SDL_MOUSEBUTTONDOWN) {
				// If the user presses a mouse button down. This gives us all
				// mouse-button-down events, so we have to make sure that this
				// event was triggered because the user has the mouse button
				// down and that it is the left mouse button.
				if (event.button.state == SDL_PRESSED && event.button.button == SDL_BUTTON_LEFT) {
					// Change grid space the user clicked in.
					lightChange(board, event.button.y/GRID_SIZE, event.button.x/GRID_SIZE);
					//printf("You clicked on box %i, %i\n", event.button.x / GRID_SIZE, event.button.y / GRID_SIZE);
				}
			}

			// P.S. If you're curious about how the event object works for
			// multiple events in a language like C, look up Unions. They're
			// dumb.
		}//End polling loop

		// Clear the previous frame's stuff so we can draw a new frame. We want
		// the background to be black, so we use SDL_SetRenderDrawColor to make
		// all operations after it draw with the color black.
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Now, we want all drawing operations after this to use the color depending
		//on the grids size
		if (boardSize == 1)
			SDL_SetRenderDrawColor(renderer, 3, 169, 244, 0);
		else if (boardSize == 3)
			SDL_SetRenderDrawColor(renderer, 76, 175, 80, 0);
		else if (boardSize == 5)
			SDL_SetRenderDrawColor(renderer, 255, 235, 59, 0);
		else
			SDL_SetRenderDrawColor(renderer, 103, 58, 183, 0);

		//Add to array of rectangles based on what square should be highlighted
		//depending on if it's true in the board array or not
		int i;
		for(i = 1; i < 8; i++){
			int j;
			for(j = 1; j < 8; j++){
				if (board[i][j]){
					squares[count].x = i*GRID_SIZE-GRID_SIZE;
					squares[count].y = j*GRID_SIZE-GRID_SIZE;
					squares[count].h = GRID_SIZE;
					squares[count].w = GRID_SIZE;
					count++;
				}
			}
		}

		//Fill in the rectangles
		for(i = 0; i < count; i++)
			SDL_RenderFillRect(renderer, &squares[i]);

		//Make the lines a different color
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

		// Draw a line every GRID_SIZE pixels horizontally and vertically
		for (i=0; i<SCREEN_WIDTH / GRID_SIZE; i++) {
			// SDL_RenderDrawLine takes the renderer, x1, y1, x2, and y2
			SDL_RenderDrawLine(renderer, i*GRID_SIZE, 0, i*GRID_SIZE, SCREEN_HEIGHT);
			SDL_RenderDrawLine(renderer, 0, i*GRID_SIZE, SCREEN_WIDTH, i*GRID_SIZE);
		}

		// Present what we have drawn on screen.
		SDL_RenderPresent(renderer);

		//Test if game is won or not and if so increment board size until it is 5x5
		if (isOff(board, boardSize)){
			if(boardSize == 7)
				gameOver = true;
			else{
				boardSize+=2;
				determineGridSize(boardSize);
				reset(board, boardSize);
			}

			SDL_Delay(1000);
		}//End if

	}//End main while loop
	// Deinitialize
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

/* Function:	determineGridSize
 * --------------------------
 * @param: boardSize - board size affects grid size
 *
 * Change the sizes of the visible grid
 */
void determineGridSize(int boardSize){
	if (boardSize == 1){
		GRID_SIZE = 420;
	}
	else if (boardSize == 3){
		GRID_SIZE = 140;
	}
	else if (boardSize == 5){
		GRID_SIZE = 84;
	}
	else{
		GRID_SIZE = 60;
	}
}

/* Function:	lightChange
 * --------------------------
 * @param: board - passes direct reference to the integer board 2D array
 * @param: locX - passes copy of user inputted x coordinate
 * @param: locY - passes copy of user inputted y coordinate
 *
 * Change the lights adjacent to and at the location chosen
 */
void lightChange(bool board [9][9], int locX, int locY){
	locX++;
	locY++;
	board [locY][locX] = !board [locY][locX]; /*If this location is true, make it false*/
	board [locY+1][locX] = !board [locY+1][locX];
	board [locY-1][locX] = !board [locY-1][locX];
	board [locY][locX+1] = !board [locY][locX+1];
	board [locY][locX-1] = !board [locY][locX-1];
}

/* Function:	isOff
* -------------------------
* 
* @param: board - passes direct reference to the integer board 2D array
* @param boardSize - size of current board user is on
*
* @return: boolean value if board is completely "lit up" or not
*/
bool isOff(bool board [9][9], int boardSize){
	int x;
	int y;
	for (x = 1; x <= boardSize; x++){
		for (y = 1; y <= boardSize; y++){
			if (board[x][y])
				return false;
		}
	}
	return true;
}

/* Function:	reset
 * --------------------------
 * @param: board - passes direct reference to the integer board 2D array
 * @param boardSize - size of current board user is on
 *
 * Resets the board when game complete or string input of reset
 */
void reset(bool board [9][9], int boardSize){
	int y;
 	int x;
 	if (boardSize!=1){
	 	for (y = 1; y < 8; y++){
	 		for (x = 1; x < 8; x++){
	 			int random = rand()%20;
	 			if(random%2 == 0)
	 				board[x][y] = true;
	 			else 
	 				board[x][y] = false;
	 		}
	 	}
	 }
	else{
		for (y = 1; y < 8; y++){
	 		for (x = 1; x < 8; x++){
	 			int random = rand()%20;
	 				board[x][y] = true;
	 		}
	 	}
	}
 }
 /*
 gcc LightsOut2D.c -IC:\mingw_dev_lib\include\SDL2 -LC:\mingw_dev_lib\lib -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -o LightsOut2D
 */