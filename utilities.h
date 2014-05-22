#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
using namespace std;

#ifndef UTIL_H
#define UTIL_H
void logSDLError(std::ostream &os, const std::string &msg);
SDL_Texture *loadTexture(const std::string &file, SDL_Renderer *ren);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);
SDL_Texture *createTextMessage(TTF_Font *font, SDL_Color color, const char *message, SDL_Renderer *ren);
#endif
