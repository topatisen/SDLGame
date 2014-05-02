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
