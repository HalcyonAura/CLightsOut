/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdbool.h>
#include <stdio.h>

//Screen dimension constants
const int SCREEN_WIDTH = 420;
const int SCREEN_HEIGHT = 420;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image
SDL_Surface* loadSurface( char *path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
	
//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//Current displayed PNG image
SDL_Surface* gPNGSurface = NULL;

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
		gWindow = SDL_CreateWindow( "Lights Out Menu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
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
				gScreenSurface = SDL_GetWindowSurface( gWindow );
			}
		}
	}

	return success;
}

bool loadMedia()	//int imageNum
{
	/*
	 * 0 Menu unhighlighted
	 * 1 Menu highlighted play
	 * 2 Menu highlighted help
	 * 3 Help unhighlighted
	 * 4 Help highlighted back
	 * 5 Reset unhighlighted
	 * 6 Reset highlighted
	 */
	//Loading success flag
	bool success = true;

	//Load PNG surface
	/*
	switch(imageNum){
		case 0:
			SDL_FreeSurface(gPNGSurface);
			gPNGSurface = loadSurface("Images\LightsOutTitle.png");
			break;
		case 1:
			SDL_FreeSurface(gPNGSurface);
			gPNGSurface = loadSurface("Images\LightsOutPlay.png");
			break;
		case 2:
			SDL_FreeSurface(gPNGSurface);
			gPNGSurface = loadSurface("Images\LightsOutHelp.png");
			break;
		case 3:
			SDL_FreeSurface(gPNGSurface);
			gPNGSurface = loadSurface("Images\LightsOutHelpScreen.png");
			break;
		case 4: 
			SDL_FreeSurface(gPNGSurface);
			gPNGSurface = loadSurface("Images\LightsOutHelpBack.png");
			break;
		case 5: 
			SDL_FreeSurface(gPNGSurface);
			gPNGSurface = loadSurface("Images\LightsOutGameOver.png");	
			break;
		case 6:
			SDL_FreeSurface(gPNGSurface);
			gPNGSurface = loadSurface("Images\LightsOutGameOverReplay.png");
			break;
		default:
			SDL_FreeSurface(gPNGSurface);
			gPNGSurface = NULL;
		}
	}
	 */
	gPNGSurface = loadSurface( "./Images\\LightsOutIntroHelp.png" );
	if( gPNGSurface == NULL )
	{
		printf( "Failed to load PNG image!\n" );
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded image
	SDL_FreeSurface( gPNGSurface );
	gPNGSurface = NULL;

	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

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

/*
int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
				}

				//Apply the PNG image
				SDL_BlitSurface( gPNGSurface, NULL, gScreenSurface, NULL );
			
				//Update the surface
				SDL_UpdateWindowSurface( gWindow );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
*/
/*
gcc LightsOutMenus.c -IC:\mingw_dev_lib\include\SDL2 -LC:\mingw_dev_lib\lib -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -o LightsOutMenus
*/