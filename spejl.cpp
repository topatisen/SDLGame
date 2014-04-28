/*////////////////////////
Mining-strategy-tycoon-digging-game
Development started on 2014.04.27
Ended on --

Made by;
topatisen & dunz0r
////////////////////////*/

#include <SDL2/SDL.h>
#include <iostream>
using namespace std;

//adding timer.h
#include "timer.h"
#include "utilities.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

int main(int argc, char *argv[]) {
	
	//quit-flag
	bool quit;

	/* {{{ FPS-limit tjofrees */
	//The frames per second timer
	LTimer fpsTimer;
	//The frames per second cap timer
	LTimer capTimer;
	int countedFrames = 0;
	fpsTimer.start();
	/* }}} */
	
	//Init SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		logSDLError(std::cerr, "SDL_Init");
		return 1;
	}

	/* {{{ SDL window, renderer'n'shizzle to ma dizzle */
	SDL_Window *window = SDL_CreateWindow("Jag har en skallerorm i min penis", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr){
		logSDLError(std::cerr, "CreateWindow");
		return 2;
	}
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr){
		logSDLError(std::cerr, "CreateRenderer");
		return 3;
	}
	/* }}} */
	
	/* {{{ SDL event */
	SDL_Event e;
	/* }}} */
	
	/* {{{ Textures */
	SDL_Texture *background = loadTexture("background.bmp", renderer);
	SDL_Texture *image = loadTexture("image.bmp", renderer);
	/* }}} */
	
	//temp variables
	int imageY = 0;
	
	//while running forever. Forever ever? Forever ever? FOREVER AND EVER AND EVER OSV.
	while(!quit) {
	
		// Empty event queueueueueu
		SDL_PumpEvents();
		
		/* {{{ Keyboard presses*/
		const Uint8 *state = SDL_GetKeyboardState(NULL);
		
		//Key up
		if (state[SDL_SCANCODE_DOWN]) {
			imageY +=4;
		}
		
		//key up
		if (state[SDL_SCANCODE_UP]) {
			imageY -=4;
		} 
		
		//quit
		if (state[SDL_SCANCODE_ESCAPE]){
			quit = true;
		}
		/*}}}*/
		
		//fps
		float avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );
		if( avgFPS > 2000000 )
		{
			avgFPS = 0;
		}
		
		//render background texture
		int bW, bH;
		SDL_QueryTexture(background, NULL, NULL, &bW, &bH);
		renderTexture(background, renderer, 0, 0);
		renderTexture(background, renderer, bW, 0);
		renderTexture(background, renderer, 0, bH);
		renderTexture(background, renderer, bW, bH);

		//set x & y
		int iW, iH;
		SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
		int x = SCREEN_WIDTH / 2 - iW / 2;
		int y = SCREEN_HEIGHT / 2 - iH / 2;

		++countedFrames;

		//If frame finished early
		int frameTicks = capTimer.getTicks();
		
		/* {{{ DRAW */
		if( frameTicks < SCREEN_TICK_PER_FRAME )
		{
			//Wait remaining time
			SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
			//draw texture
			renderTexture(image, renderer, x, imageY);
			
			//render texture
			SDL_RenderPresent(renderer);
		}
		/* }}} */
	}
	
	//destroy everyone of them! Utah! Come in Utah! There's a target in your immediate proximity!
	SDL_DestroyTexture(background);
	SDL_DestroyTexture(image);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
