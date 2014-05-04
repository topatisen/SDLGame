/*////////////////////////
Mining-strategy-tycoon-digging-game
Development started on 2014.04.27
Ended on --

Made by;
topatisen & dunz0r
////////////////////////*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <cstdio>

using namespace std;

//adding timer.h
#include "timer.h"
#include "utilities.h"

#include "ground.h"
#include "miner.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

//mouse_x & mouse_y, is global, a lot of classes'n'shitz will be using those
int mouse_x = 0;
int mouse_y = 0;

const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

int main(int argc, char *argv[]) {
	
	//quit-flag
	bool quit;

	//{/* {{{ FPS-limit tjofrees */
	//The frames per second timer
	LTimer fpsTimer;
	//The frames per second cap timer
	LTimer capTimer;
	int countedFrames = 0;
	fpsTimer.start();
	//}/* }}} */
	
	//Init SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		logSDLError(std::cerr, "SDL_Init");
		return 1;
	}
	TTF_Init();
	
	//{/* {{{ SDL window, renderer'n'shizzle to ma dizzle */
	SDL_Window *window = SDL_CreateWindow("Jag har en penis i min skallerorm", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr){
		logSDLError(std::cerr, "CreateWindow");
		return 2;
	}
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr){
		logSDLError(std::cerr, "CreateRenderer");
		return 3;
	}
	//}/* }}} */
	//{/* {{{ Load a font */
	TTF_Font *fFont = NULL;
	fFont = TTF_OpenFont("font.ttf", 24);
	SDL_Color black = {0, 0, 0};
	//}/* }}} */
	
	/* {{{ Make a "string", to make more, just do 1 new surface and  new texture */
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(fFont, "Testing testing", black);
	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	SDL_FreeSurface(surfaceMessage);
	/* }}} */
	
	//{/* {{{ SDL event */
	SDL_Event e;
	//}/* }}} */
	
	//{/* {{{ Textures */
	SDL_Texture *sBackground = loadTexture("sBackground.bmp", renderer);
	SDL_Texture *sGrid = loadTexture("sGrid.bmp", renderer);
	//}/* }}} */
	
	//Create a miner!
	cMiner oMiner;
	oMiner.create(renderer);
	
	//ground test
	cCreateGround oCreateGround;
	oCreateGround.create(renderer);
	
	//temp variables
	int imageY = 0;
	char text[40];
	int textInt = 0;
	/////////
	
	
	//while running forever. Forever ever? Forever ever? FOREVER AND EVER AND EVER OSV.
	while(!quit) {
	
		// Empty event queueueueueu
		SDL_PumpEvents();
		textInt ++;
		//run text
		sprintf(text,"groundnum;%d ",oCreateGround.groundnum);
		surfaceMessage = TTF_RenderText_Solid(fFont,text, black);
		Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
		SDL_FreeSurface(surfaceMessage);
		//
		
		//{/* {{{ Keyboard presses*/
		const Uint8 *state = SDL_GetKeyboardState(NULL);
		//quit
		if (state[SDL_SCANCODE_ESCAPE]){
			quit = true;
		}
		//}/*}}}*/
		
		//miner run
		oMiner.run(e,renderer);
		//checkCollision for miner
		for(int i = 0;i < oCreateGround.groundnum;i++)
		{
			oMiner.checkCollision(oCreateGround.oGround[i].x,oCreateGround.oGround[i].y);
		}
		
		//make ground, loop
		oCreateGround.makeGround(renderer);
		
		
		//fps
		float avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );
		if( avgFPS > 2000000 )
		{
			avgFPS = 0;
		}
		
		++countedFrames;

		//If frame finished early
		int frameTicks = capTimer.getTicks();
		
		//{/* {{{ DRAW */
		if( frameTicks < SCREEN_TICK_PER_FRAME )
		{
			//Wait remaining time
			SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
			
			// Draw background
			renderTexture(sBackground, renderer, 0, 0);
			
			//draw text
			renderTexture(Message, renderer, 0, 0);
			
			oMiner.draw(renderer);
			
			oCreateGround.draw(renderer);
			
			//draw grid on top of everything, good for debugging
			renderTexture(sGrid, renderer, 0, 0);
			
			//render texture
			SDL_RenderPresent(renderer);
		}
		//}/* }}} */
	}
	
	//destroy everyone of them! Utah! Come in Utah! There's a target in your immediate proximity!
	SDL_DestroyTexture(sBackground);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
