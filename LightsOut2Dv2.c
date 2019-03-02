/* 
 * @author: Cecilia La Place
 * Date: 3/14/16
 * @version: 2.0
 * Program: Lights Out
 * Description: The user will choose a location in a 5x5 grid to "click" and 
 * 				turn "on" or "off" the lights. The goal of the game is to 
 *				turn all the lights off. The second goal is to turn off all 
 *				the lights as efficiently as possible. (Keep in mind not all
 *				games are solvable.)
 */

#include <SDL_image.h>
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

//Define constants.
#define SCREEN_WIDTH 420
#define SCREEN_HEIGHT 420
static int GRID_SIZE;	//Width and height of each grid space

//Prototypes

//Loads media
bool init();
bool loadMedia();
void closeImage();
//Frees media and shuts down SDL
void close();

//Loads individual image
SDL_Surface* loadSurface( char *path );
SDL_Window *window = NULL;
//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//Current displayed PNG image
SDL_Surface* gPNGSurface = NULL;

//void hoverMenu(int x, int y);
void lightChange(bool board [9][9], int locX, int locY);
bool isOff(bool board [9][9], int boardSize);
void reset(bool board[9][9], int boardSize);
void drawGame(int boardSize, SDL_Renderer *renderer, bool board[9][9], SDL_Rect squares[49], int count);

int main(int argv, char **argc) {
	//Allow for truly random filled in grid spaces
	srand(time(NULL));

	//Initialize game start
	int boardSize = 1;
	determineGridSize(boardSize);
	bool board [9][9];
	reset(board, boardSize);

	//Initialize everything
	// Initialize SDL
	int err = SDL_Init(SDL_INIT_EVERYTHING);
	if (err != 0) {
		// If SDL couldnt initialize
		printf("failed to init SDL: %s", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	// Create the window object.
	/*window = SDL_CreateWindow("Lights Out", // Window title
			//Where the window should be placed on-screen. UNDEFINED means
			//that the window manager gets to decide.
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			//The size of the window in pixels
			SCREEN_WIDTH, SCREEN_HEIGHT,
			//Miscellaneous other flags.
			SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	//If it fails
	if (window == NULL) {
		printf("failed to open window: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
*/
	//Initialize menu screen
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
		exit(EXIT_FAILURE);
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
			exit(EXIT_FAILURE);
		}
	}

	//Create the renderer. The renderer controls drawing. SDL_RENDERER_ACCELERATED
	//forces the use of the graphics card
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		printf("failed to create renderer: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	//Allow user to quit at any time
	bool quit = false;
	//If the game is over quit (for now) 
	//TODO
	bool gameOver = false;
	bool menu = true;
	//bool help = false;

	SDL_Event event;
	// Start main loop
	while (!gameOver && !quit) {
		//Initializes grid array to be drawn and keeps count of how many are drawn
		SDL_Rect squares[49];
		int count = 0;

		//Polling for events in the game
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				// If the event is a quit request (like if the user presses the
				// X button), break out of main loop.
				quit = true;
			} 
			/*
			//If the user is in the menu
			else if (event.type == SDL_MOUSEMOTION && (menu || help) || gameOver){
				//I have no idea if this even works since I can't test it til I do the image stuff
				//TODO
				int x, y;
				SDL_GetMouseState(&x,&y);
				hoverMenu(x, y);
			}

			int x, y;
			SDL_GetMouseState(&x,&y);
			//If user clicks play			
			if (event.type == SDL_MOUSEBUTTONDOWN && y >= 234 && y <= 279 && x >= 152 && x <= 266 && !help && menu){
				if (event.button.state == SDL_PRESSED && event.button.button == SDL_BUTTON_LEFT) {
					menu = false;
					help = false;
				}
			}
			//If user clicks help
			else if(event.type == SDL_MOUSEBUTTONDOWN && y >= 302 && y <= 347 && x >= 147 && x <= 271 && !help && menu){
				if (event.button.state == SDL_PRESSED && event.button.button == SDL_BUTTON_LEFT) {
					menu = true;
					help = true;
				}
			}
			//If user clicks back from help menu
			else if(event.type == SDL_MOUSEBUTTONDOWN && y >= 307 && y <= 351 && x >= 141 && x <= 276 && help && menu){
				if (event.button.state == SDL_PRESSED && event.button.button == SDL_BUTTON_LEFT) {
					menu = true;
					help = false;
				}
			}
			*/
			if (event.type == SDL_MOUSEBUTTONDOWN && menu /*&& !help &&*/) {
				// If the user presses a mouse button down. This gives us all
				// mouse-button-down events, so we have to make sure that this
				// event was triggered because the user has the mouse button
				// down and that it is the left mouse button.
				if (event.button.state == SDL_PRESSED && event.button.button == SDL_BUTTON_LEFT) {
					//Get rid of old loaded surface
					closeImage();
					menu = false;
				}
			}
			/*
			 *If user clicks replay from game over screen
			 */
			//In game commands
			if (event.type == SDL_MOUSEBUTTONDOWN && !menu && /*!help &&*/ !gameOver) {
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
		}//End polling loop


		if (!menu /*&& !help*/){
			//Clear the previous frameto draw a new frame. 
			//To make the background black, use SDL_SetRenderDrawColor 
			//with the color black
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);

			//Draw boxes
			drawGame(boardSize, renderer, board, squares, count);
			//Test if game is won or not and if so increment board size until it is 7x7
			if (isOff(board, boardSize)){
				if(boardSize == 7){
					gameOver = true;
				}
				else{
					boardSize+=2;
					determineGridSize(boardSize);
					reset(board, boardSize);
				}

				SDL_Delay(1000);
			}//End if
		}//End main if
		else{
			//Apply the PNG image
				SDL_BlitSurface( gPNGSurface, NULL, gScreenSurface, NULL );
			
				//Update the surface
				SDL_UpdateWindowSurface( window );
		}

	}//End main while loop
	close();
	return 0;
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		FILE *errFile = fopen("./error.txt", "w");
		fprintf(errFile, SDL_GetError());
		fclose(errFile);
		success = false;
	}
	else
	{
		//Create window
		window = SDL_CreateWindow( "Lights Out", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( window == NULL )
		{
			FILE *errFile = fopen("./error.txt", "w");
			fprintf(errFile, SDL_GetError());
			fclose(errFile);
			success = false;
		}
		else
		{
			//Initialize PNG loading
			int imgFlags = IMG_INIT_PNG;
			if( !( IMG_Init( imgFlags ) & imgFlags ) )
			{
				FILE *errFile = fopen("./error.txt", "w");
				fprintf(errFile, IMG_GetError());
				fclose(errFile);
				success = false;
			}
			else
			{
				//Get window surface
				gScreenSurface = SDL_GetWindowSurface( window );
			}
		}
	}

	return success;
}

bool loadMedia()	//int imageNum
{
	bool success = true;
	gPNGSurface = loadSurface( "./Images\\LightsOutIntroHelp.png" );
	if( gPNGSurface == NULL )
	{
		printf( "Failed to load PNG image!\n" );
		success = false;
	}

	return success;
}

void closeImage(){
	//Free loaded image
	SDL_FreeSurface( gPNGSurface );
	gPNGSurface = NULL;
}

void close()
{
	//Free loaded image
	SDL_FreeSurface( gPNGSurface );
	gPNGSurface = NULL;

	//Destroy window
	SDL_DestroyWindow( window );
	window = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Surface* loadSurface( char *path )
{
	//The final optimized image
	SDL_Surface* optimizedSurface = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path );
	if( loadedSurface == NULL )
	{
		FILE *errFile = fopen("./error.txt", "w");
		fprintf(errFile, IMG_GetError());
		fclose(errFile);
		//printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Convert surface to screen format
		optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, NULL );
		if( optimizedSurface == NULL )
		{
			FILE *errFile = fopen("./error.txt", "w");
		fprintf(errFile, SDL_GetError());
		fclose(errFile);
			//printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return optimizedSurface;
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

/*void hoverMenu(int x, int y){
	//play.y 234-279
	//play.x 152-266
	//If hovering over the play button
	if (y >= 234 && y <= 279 && x >= 152 && x <= 266 && !help){
		//change screen to highlighted play
	}
	//help.y 302-347
	//help.x 147-271
	//If hovering over the help button
	else if(y >= 302 && y <= 347 && x >= 147 && x <= 271 && !help){
		//change screen to highlighted help

	}
	//help.back.y 307-351
	//help.back.x 141-276
	//If in the help menu and hovering over back
	else if(y >= 307 && y <= 351 && x >= 141 && x <= 276 && help){
		//change screen to highlighted back
	}
	//Else if in help menu unhighlight back
	else if (menu && help){
		//change help screen to nothing highlighted
	}
	//If game over show game over screen
	else if (gameOver && !menu && !help){
		
	}

	//Else main is not highlighted
	else{
		//change main screen to nothing highlighted
	}
}
*/

void drawGame(int boardSize, SDL_Renderer *renderer, bool board[9][9], SDL_Rect squares[49], int count){
	// Now, we want all drawing operations after this to use the color depending
	//on the grids size
	if (boardSize == 1)
		SDL_SetRenderDrawColor(renderer, 3, 169, 244, 0);	//Blue
	else if (boardSize == 3)
		SDL_SetRenderDrawColor(renderer, 76, 175, 80, 0);	//Green
	else if (boardSize == 5)
		SDL_SetRenderDrawColor(renderer, 255, 235, 59, 0);	//Yellow
	else
		SDL_SetRenderDrawColor(renderer, 103, 58, 183, 0);	//Purple

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

	//Fill in rectangles
	for(i = 0; i < count; i++)
		SDL_RenderFillRect(renderer, &squares[i]);

	//Make the lines white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

	// Draw a line every GRID_SIZE pixels horizontally and vertically
	for (i=0; i<SCREEN_WIDTH / GRID_SIZE; i++) {
		// SDL_RenderDrawLine takes the renderer, x1, y1, x2, and y2
		SDL_RenderDrawLine(renderer, i*GRID_SIZE, 0, i*GRID_SIZE, SCREEN_HEIGHT);
		SDL_RenderDrawLine(renderer, 0, i*GRID_SIZE, SCREEN_WIDTH, i*GRID_SIZE);
	}

	// Draw screen.
	SDL_RenderPresent(renderer);

 }

 /*
 gcc LightsOut2Dv2.c -IC:\mingw_dev_lib\include\SDL2 -LC:\mingw_dev_lib\lib -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -o LightsOut2Dv2
 */

 //Notes
 /*
 Make 2 versions
 1 version with incrementing board size
 one w/ 3x3 w/ solvable ones (time and moes taken data collection to test file?)
 */