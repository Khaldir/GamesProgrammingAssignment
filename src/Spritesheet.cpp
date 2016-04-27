#include "Spritesheet.h"

Spritesheet::Spritesheet(std::string image, int spriteNumber, int speed)
{
	spriteCount = spriteNumber;
	imagePath = "./assets/" + image + ".png";
	spriteSpeed = speed;
	direction = 0;
}

Spritesheet::Spritesheet(SDL_Renderer *ren, std::string image, int spriteNumber, int speed)
{
	spriteSpeed = speed;
	imagePath = "./assets/" + image + ".png";
	spriteCount = spriteNumber;
	SDL_Surface *surface = IMG_Load(imagePath.c_str());
	if (surface == nullptr) {
		std::cout << "SDL IMG_Load Error: " << SDL_GetError() << std::endl;
		CleanExit(1);
	}

	sprites = SDL_CreateTextureFromSurface(ren, surface);
	SDL_FreeSurface(surface);
	if (sprites == nullptr) {
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		CleanExit(1);
	}
	SDL_QueryTexture(sprites, NULL, NULL, &widthOfSprite, NULL);
	widthOfSprite = widthOfSprite / spriteNumber;
	renderer = ren;
	direction = 0;
}


Spritesheet::~Spritesheet()
{
}


int Spritesheet::getSpriteCount()
{
	return spriteCount;
}


SDL_Texture * Spritesheet::getSpriteSheet()
{
	return sprites;
}

void Spritesheet::CleanExit(int returnValue)
{
	if (sprites != nullptr) SDL_DestroyTexture(sprites);
	if (renderer != nullptr) SDL_DestroyRenderer(renderer);
	SDL_Quit();
	exit(returnValue);
}

SDL_Rect Spritesheet::updateAnimation()
{
	Uint32 ticks = SDL_GetTicks();
	Uint32 seconds = ticks / spriteSpeed;
	Uint32 sprite = seconds % spriteCount;
	SDL_Rect output = { sprite * widthOfSprite, direction * widthOfSprite, widthOfSprite, widthOfSprite };
	return output;
}

void Spritesheet::renderSprite(SDL_Rect destinationRect)
{
	SDL_Rect srcRect = updateAnimation();
	SDL_RenderCopy(renderer, sprites, &srcRect, &destinationRect);
}

void Spritesheet::renderSprite()
{
	SDL_Rect srcRect = updateAnimation();
	SDL_RenderCopy(renderer, sprites, &srcRect, &location);
}

void Spritesheet::newRenderer(SDL_Renderer *ren)
{
	SDL_Surface *surface = IMG_Load(imagePath.c_str());
	if (surface == nullptr) {
		std::cout << "SDL IMG_Load Error: " << SDL_GetError() << std::endl;
		CleanExit(1);
	}
	sprites = SDL_CreateTextureFromSurface(ren, surface);
	SDL_FreeSurface(surface);
	if (sprites == nullptr) {
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		CleanExit(1);
	}
	
	SDL_QueryTexture(sprites, NULL, NULL, &widthOfSprite, NULL);
	widthOfSprite = widthOfSprite / spriteCount;

	location.h = widthOfSprite*2;
	location.w = widthOfSprite*2;
	location.x = 220;
	location.y = 333;

	renderer = ren;
}

void Spritesheet::changeDirection(int newDirection)
{
	if(newDirection<4 && newDirection>-1)
		direction = newDirection;	
}

int Spritesheet::getDirection()
{
	return direction;
}

SDL_Rect Spritesheet::getLocation()
{
	return(location);
}

void Spritesheet::updateLocation()
{
	switch (direction)
	{
		case 0:
		{
			location.x += 1;
			break;
		}
		case 1:
		{
			location.x -= 1;
			break;
		}
		case 2:
		{
			location.y -= 1;
			break;
		}
		case 3:
		{
			location.y += 1;
			break;
		}
	}
}

void Spritesheet::reverseMove()
{
	switch (direction)
	{
	case 0:
	{
		location.x -= 1;
		break;
	}
	case 1:
	{
		location.x += 1;
		break;
	}
	case 2:
	{
		location.y += 1;
		break;
	}
	case 3:
	{
		location.y -= 1;
		break;
	}
	}
}