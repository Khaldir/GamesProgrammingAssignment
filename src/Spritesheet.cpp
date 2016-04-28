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
	SDL_Rect output;
	Uint32 ticks;
	if (!dead)
	{
		ticks = SDL_GetTicks();
	}
	else
	{
		ticks = SDL_GetTicks() - deathTime;
		if (SDL_TICKS_PASSED(ticks,2000))
		{
			CleanExit(0);
		}
	}
	Uint32 seconds = ticks / spriteSpeed;
	Uint32 sprite = seconds % spriteCount;
	output = { (int)sprite * widthOfSprite, direction * widthOfSprite, widthOfSprite, widthOfSprite };
	return output;
	
}

SDL_Rect Spritesheet::updateAnimation(int num)
{
	SDL_Rect output;
	Uint32 ticks = SDL_GetTicks();
	Uint32 seconds = ticks / spriteSpeed;
	Uint32 sprite = seconds % spriteCount;
	int covered;

	if (location.x < 0)
		covered = -location.x;
	else
		covered = location.x - 464;
	
	if (num == 0)
	{
		output = {covered + ((int)sprite * widthOfSprite) , direction * widthOfSprite , widthOfSprite - covered, widthOfSprite};
	}
	if (num == 1)
	{
		output = { ((int)sprite * widthOfSprite) , direction * widthOfSprite , widthOfSprite - covered, widthOfSprite };
	}
	/*
	if (location.x < 0)
	{
		int covered = -location.x;
		if (num == 0)
		{
			output = { covered + ((int)sprite * widthOfSprite), direction * widthOfSprite, widthOfSprite - covered, widthOfSprite };
		}
		else
		{
			output = { ((int)sprite * widthOfSprite), direction * widthOfSprite, widthOfSprite - covered, widthOfSprite };
		}
	}
	else
	{
		int covered = location.x - 464;
		if (num == 0)
		{
			output = { (int)sprite * widthOfSprite, direction * widthOfSprite, widthOfSprite - covered, widthOfSprite };
		}
		else
		{
			output = { covered + ((int)sprite * widthOfSprite), direction * widthOfSprite, widthOfSprite - covered, widthOfSprite };
		}
	}
	*/
	return output;

}

void Spritesheet::renderSprite(SDL_Rect destinationRect)
{
	SDL_Rect srcRect = updateAnimation();
	SDL_RenderCopy(renderer, sprites, &srcRect, &destinationRect);
}

void Spritesheet::renderSprite()
{
	if (location.x < 0) 
	{
		SDL_Rect srcRect = updateAnimation();
		SDL_RenderCopy(renderer, sprites, &srcRect, &location);
		SDL_Rect oppositeLocation = location;
		oppositeLocation.x = 464 - widthOfSprite;
		srcRect = updateAnimation(1);
		SDL_RenderCopy(renderer, sprites, &srcRect, &oppositeLocation);
		if (location.x < -(widthOfSprite / 2))
		{
			location.x += 464;
		}
		if (location.x > 464 + (widthOfSprite / 2))
		{
			location.x = 0 - (widthOfSprite / 2);
		}
	}
	else
	{
		if (location.x > (464 - widthOfSprite))
		{
			SDL_Rect srcRect = updateAnimation();
			SDL_RenderCopy(renderer, sprites, &srcRect, &location);
			SDL_Rect oppositeLocation = location;
			oppositeLocation.x = 0;
			srcRect = updateAnimation(0);
			SDL_RenderCopy(renderer, sprites, &srcRect, &oppositeLocation);
			if (location.x < -(widthOfSprite / 2))
			{
				location.x += 464;
			}
			if (location.x > 464 + (widthOfSprite / 2))
			{
				location.x = 0 - (widthOfSprite / 2);
			}
		}
		SDL_Rect srcRect = updateAnimation();
		SDL_RenderCopy(renderer, sprites, &srcRect, &location);
	}
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
	location.x = 0;
	location.y = 0;

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
	if (!dead)
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

void Spritesheet::setLocation(int x, int y)
{
	location.x = x;
	location.y = y;
}

void Spritesheet::setLocation(SDL_Rect newLocation)
{
	location = newLocation;
}

void Spritesheet::die(SDL_Renderer *ren)
{
	
	dead = true;
	int pathLength = imagePath.length() - 4;
	std::string newPath = imagePath.substr(0, pathLength);
	newPath = newPath + "death.png";

	SDL_Surface *surface = IMG_Load(newPath.c_str());
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

	renderer = ren;
	SDL_QueryTexture(sprites, NULL, NULL, &widthOfSprite, NULL);
	spriteCount = widthOfSprite / 16;
	widthOfSprite = 16;
	direction = 0;
	spriteSpeed = 200;

	deathTime = SDL_GetTicks();
}



bool Spritesheet::directionPicked()
{
	return(directionChosen);
}

void Spritesheet::toggleDirectionPicked()
{
	directionChosen = !directionChosen;
}