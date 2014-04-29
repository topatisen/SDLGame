#include "utilities.h"
#include <SDL2/SDL_ttf.h>

/*{{{ log an error to stream */
void logSDLError(std::ostream &os, const std::string &msg) {
	os << msg << " error:" << SDL_GetError() << std::endl;
}
/*}}}*/

/*{{{ loadTexture: Loads file, returns a texture*/
SDL_Texture *loadTexture(const std::string &file, SDL_Renderer *ren) {
	SDL_Texture *texture = nullptr;
	SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());

	if(loadedImage != nullptr) {
	
		//transparency ffs! (Foer fasen!)
		SDL_SetColorKey(loadedImage,true,SDL_MapRGB(loadedImage->format,255,0,255));
		
		texture = SDL_CreateTextureFromSurface(ren, loadedImage);
		SDL_FreeSurface(loadedImage);
		if(texture == nullptr)
			logSDLError(std::cerr, "CreateTextureFromSurface");
	} else 
		logSDLError(std::cerr, "LoadBMP");

	return texture;
}
/*}}}*/
/*{{{ Load font text test, returns texture*/
SDL_Texture *loadFont(std::string &file, int fontSize,SDL_Renderer *ren)
{
	TTF_Font *font = nullptr;
	
	SDL_Rect Message_rect;
	Message_rect.x = 0;
	Message_rect.y = 0;
	Message_rect.w = 512;//Not a good size, but will fix later
	Message_rect.h = 48;
	
	SDL_Rect dst;
	dst.x = 0;
	dst.y = 0;
	dst.w = 512;////Not a good size, but will fix later
	dst.h = 48;
	
	TTF_OpenFont(file.c_str(),fontSize);
	SDL_Color black = {0, 0, 0}; //Font color
	
	SDL_Surface* surfaceMessage = NULL;
	surfaceMessage = TTF_RenderText_Solid(font, "Testing testing", black);
	SDL_Texture* Message = NULL;
	Message = SDL_CreateTextureFromSurface(ren, surfaceMessage);
	SDL_FreeSurface(surfaceMessage);
	
	return Message;
};
	/* }}} */
	//{Text loading
	
	

	//}////

/*}}}*/

/*{{{ renderTexture: Renders a texture to renderer */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y) {
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;

	// Query the texture to get its width and heigth to use
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(ren, tex, NULL, &dst);
}
/*}}}*/
