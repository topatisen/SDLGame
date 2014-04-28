#include <SDL2/SDL.h>
#include <iostream>
using namespace std;

//adding timer.h
#include "timer.h"

//tjipp
//tjopp
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768



const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;


/*
 * Log an SDL error
 * @param os Which strean to log to
 * @param msg The error message to write, format will be msg error: SDL_GetError()
 */
void logSDLError(std::ostream &os, const std::string &msg) {
	os << msg << " error:" << SDL_GetError() << std::endl;
}

/*
 * Loads a BMP image into a texture rendering device
 * @param file The BMP image to load
 * @param ren The rendered to use
 * @return the loaded texture or nullptr if something went wrong
 */

SDL_Texture *loadTexture(const std::string &file, SDL_Renderer *ren) {
	SDL_Texture *texture = nullptr;
	SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());

	if(loadedImage != nullptr) {
		texture = SDL_CreateTextureFromSurface(ren, loadedImage);
		SDL_FreeSurface(loadedImage);
		if(texture == nullptr)
			logSDLError(std::cerr, "CreateTextureFromSurface");
	} else 
		logSDLError(std::cerr, "LoadBMP");

	return texture;
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y) {
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;

	// Query the texture to get its width and heigth to use
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

int main(int argc, char *argv[]) {
	SDL_Event e;
	bool quit;

	//The frames per second timer
	LTimer fpsTimer;

	//The frames per second cap timer
	LTimer capTimer;

	int countedFrames = 0;
	fpsTimer.start();

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		logSDLError(std::cerr, "SDL_Init");
		return 1;
	}

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

	SDL_Texture *background = loadTexture("background.bmp", renderer);
	SDL_Texture *image = loadTexture("image.bmp", renderer);
	if (background == nullptr || image == nullptr){
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		logSDLError(std::cerr, "LoadBMPs");
		return 127;
	}

	int imageY = 0;
	while(!quit) {
		// Empty event queueueueueu
		SDL_PumpEvents();
		const Uint8 *state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_DOWN]) {
			imageY +=4;
		}
		if (state[SDL_SCANCODE_UP]) {
			imageY -=4;
		} 
		if (state[SDL_SCANCODE_ESCAPE]){
			quit = true;
		} 
		float avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );
		if( avgFPS > 2000000 )
		{
			avgFPS = 0;
		}

		int bW, bH;
		SDL_QueryTexture(background, NULL, NULL, &bW, &bH);
		renderTexture(background, renderer, 0, 0);
		renderTexture(background, renderer, bW, 0);
		renderTexture(background, renderer, 0, bH);
		renderTexture(background, renderer, bW, bH);

		int iW, iH;
		SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
		int x = SCREEN_WIDTH / 2 - iW / 2;
		int y = SCREEN_HEIGHT / 2 - iH / 2;


		++countedFrames;

		//If frame finished early
		int frameTicks = capTimer.getTicks();
		if( frameTicks < SCREEN_TICK_PER_FRAME )
		{
			//Wait remaining time
			SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
			renderTexture(image, renderer, x, imageY);
			SDL_RenderPresent(renderer);
		}
	}
	SDL_DestroyTexture(background);
	SDL_DestroyTexture(image);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
