#include "Collectable.h"


Collectable::~Collectable()
{

}
Collectable::Collectable(std::string image, int spriteTotal, int spriteNum)
{
	spriteCount = spriteTotal;
	spriteNumber = spriteNum;
	imagePath = "./assets/" + image + ".png";
}

void Collectable::CleanExit(int returnValue)
{
	if (sprite != nullptr) SDL_DestroyTexture(sprite);
	if (renderer != nullptr) SDL_DestroyRenderer(renderer);
	SDL_Quit();
	exit(returnValue);
}

SDL_Texture * Collectable::getCollectableTexture() 
{
	return sprite;
}
void Collectable::render()
{
	SDL_Rect srcRect = {spriteNumber*widthOfSprite, 0, widthOfSprite, widthOfSprite};
	SDL_RenderCopy(renderer, sprite, &srcRect, &location);
}
void Collectable::newRenderer(SDL_Renderer *ren) 
{
	SDL_Surface *surface = IMG_Load(imagePath.c_str());
	if (surface == nullptr) {
		std::cout << "SDL IMG_Load Error: " << SDL_GetError() << std::endl;
		CleanExit(1);
	}
	sprite = SDL_CreateTextureFromSurface(ren, surface);
	SDL_FreeSurface(surface);
	if (sprite == nullptr) {
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		CleanExit(1);
	}

	SDL_QueryTexture(sprite, NULL, NULL, &widthOfSprite, NULL);
	widthOfSprite = widthOfSprite / spriteCount;

	location.h = widthOfSprite * 2;
	location.w = widthOfSprite * 2;
	location.x = 0;
	location.y = 0;

	renderer = ren;
}


SDL_Rect Collectable::getLocation()
{
	return(location);
}

void Collectable::setLocation(int x, int y)
{
	location.x = x;
	location.y = y;
}

void Collectable::collect()
{

}

