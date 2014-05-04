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

//globals
//view-moving, add those to every draw-function(later)
int viewx = 0;
int viewy = 0;

//tool, just an integer
//0 = nothing, selecting osv.
//1 = goalmakerthinga'ma'jing
int tool = 0;

using namespace std;

//adding timer.h
#include "timer.h"
#include "utilities.h"

#include "ground.h"
#include "miner.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768


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
	SDL_Window *window = SDL_CreateWindow("Jag har en skalle i min skalle", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
	
	/* {{{ Make a "string", to make more, just do 1 new surface and  new texture, then free it */
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(fFont, "Testing testing", black);
	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	SDL_FreeSurface(surfaceMessage);
	
	SDL_Surface* debugSurface = TTF_RenderText_Solid(fFont, "Testing testing", black);
	SDL_Texture* debugMessage = SDL_CreateTextureFromSurface(renderer, debugSurface);
	SDL_FreeSurface(debugSurface);
	/* }}} */
	
	//{/* {{{ SDL event */
	SDL_Event e;
	//}/* }}} */
	
	//{/* {{{ Textures */
	SDL_Texture *sBackground = loadTexture("sBackground.bmp", renderer);
	SDL_Texture *sGrid = loadTexture("sGrid.bmp", renderer);
	//}/* }}} */
	
	//Create some miners!
	cMiner oMiner[5];
	int minerx = 64;
	for(int i = 0; i<5;i++)
	{
		oMiner[i].create(renderer);
		oMiner[i].x = minerx;
		minerx += 64;
	}
	
	//ground test
	cCreateGround oCreateGround;
	oCreateGround.create(renderer);
	
	//temp variables
	int imageY = 0;
	char text[40];
	int textInt = 0;
	/////////
	
	//mouse
	int mouse_x = 0;
	int mouse_y = 0;
	
	
	//while running forever. Forever ever? Forever ever? FOREVER AND EVER AND EVER OSV.
	while(!quit) {
	
		// Empty event queueueueueu
		SDL_PumpEvents();
		textInt ++;
		//run text
		sprintf(text,"0 = selection, 1 = place goals, 2 = place ladders, when selected press enter to start path");
		surfaceMessage = TTF_RenderText_Solid(fFont,text, black);
		Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
		SDL_FreeSurface(surfaceMessage);
		
		sprintf(text,"Current tool: %d , groundnum: %d", tool, oCreateGround.groundnum);
		debugSurface = TTF_RenderText_Solid(fFont,text, black);
		debugMessage = SDL_CreateTextureFromSurface(renderer, debugSurface);
		SDL_FreeSurface(debugSurface);
		//
		
		//{/* {{{ Keyboard presses, mouse events osv.*/
		const Uint8 *state = SDL_GetKeyboardState(NULL);
		//quit
		if (state[SDL_SCANCODE_ESCAPE]){
			quit = true;
		}
		
		//make miner follow the trail!
		if (state[SDL_SCANCODE_RETURN])
		{
			for(int i = 0; i<5;i++)
			{
				if(oMiner[i].selected == true)
				{
					oMiner[i].moveToGoal = true;
				}
			}
		}
		
		//Select tools (will be a class later)
		if (state[SDL_SCANCODE_0]){
			tool = 0;
		}
		if (state[SDL_SCANCODE_1]){
			tool = 1;
		}
		if (state[SDL_SCANCODE_2]){
			tool = 2;
		}
		//}/*}}}*/
		
		//miner run
		for(int i = 0; i<5;i++)
		{
			oMiner[i].run(e,renderer);
		}
		//check collision for miner
		for(int i = 0;i < oCreateGround.groundnum;i++)
		{
			for(int p = 0; p<5;p++)
			{
				oMiner[p].checkCollision(oCreateGround.oGround[i].x,oCreateGround.oGround[i].y);
			}
		}
		//check ladders for miner
		for(int i = 0;i < oCreateGround.laddernum;i++)
		{
			for(int p = 0; p<5;p++)
			{
				oMiner[p].checkLadder(oCreateGround.oStepladder[i].x,oCreateGround.oStepladder[i].y);
			}
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
			
			//{/* {{{ ALL DRAW GOES HERE, FATTAR DU?! */
			// Draw background
			renderTexture(sBackground, renderer, 0, 0);
			
			//draw miner
			for(int i = 0; i<5;i++)
			{
				oMiner[i].draw(renderer);
			}
			
			//draw ground
			oCreateGround.draw(renderer);
			
			//draw grid on top of everything, good for debugging
			renderTexture(sGrid, renderer, 0, 0);
			
			//draw text
			renderTexture(Message, renderer, 0, 0);
			renderTexture(debugMessage, renderer, 0, 32);
			//}/* }}} */
			
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
