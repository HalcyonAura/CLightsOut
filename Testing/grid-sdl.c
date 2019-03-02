#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL.h>

// Define our constants.
#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 400
#define GRID_SIZE 40	// This will be the width and height of each grid space

int main(int argv, char **argc) {
	// Initialize SDL. You can choose to only initialize parts of the library,
	// but I don't know why you would do that.
	int err = SDL_Init(SDL_INIT_EVERYTHING);
	if (err != 0) {
		// We couldn't initialize SDL for some reason.
		printf("failed to init SDL: %s", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	// Create the window object.
	SDL_Window *window = SDL_CreateWindow("Gridworld", // Window title
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

	bool quit = false;
	SDL_Event event;
	// Start our main loop
	while (!quit) {
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
					// Print out what grid space the user clicked in.
					printf("You clicked on box %i, %i\n", event.button.x / GRID_SIZE, event.button.y / GRID_SIZE);
				}
			}

			// P.S. If you're curious about how the event object works for
			// multiple events in a language like C, look up Unions. They're
			// dumb.
		}

		// Clear the previous frame's stuff so we can draw a new frame. We want
		// the background to be black, so we use SDL_SetRenderDrawColor to make
		// all operations after it draw with the color black.
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Now, we want all drawing operations after this to use the color red.
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

		// Draw a line every GRID_SIZE pixels horizontally and vertically
		int i;
		for (i=0; i<SCREEN_WIDTH / GRID_SIZE; i++) {
			// SDL_RenderDrawLine takes the renderer, x1, y1, x2, and y2
			SDL_RenderDrawLine(renderer, i*GRID_SIZE, 0, i*GRID_SIZE, SCREEN_HEIGHT);
			SDL_RenderDrawLine(renderer, 0, i*GRID_SIZE, SCREEN_WIDTH, i*GRID_SIZE);
		}

		// Present what we have drawn on screen.
		SDL_RenderPresent(renderer);

	}

	// Deinitialize
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
