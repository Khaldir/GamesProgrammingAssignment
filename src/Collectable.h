#pragma once
#include "includes.h"

class Collectable
{
public:
	~Collectable();
	Collectable(std::string image, int spriteTotal, int spriteNumber);
private:
	int spriteNumber;
	int spriteCount;
	int widthOfSprite;
	std::string imagePath;
	SDL_Rect location;
	SDL_Texture *sprite;
	void CleanExit(int);
	SDL_Renderer *renderer;
public:
	SDL_Texture * getCollectableTexture();
	void render();
	void newRenderer(SDL_Renderer *ren);
	SDL_Rect getLocation();
	void setLocation(int x, int y);
	void collect();
};

