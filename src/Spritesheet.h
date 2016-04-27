#pragma once
#include "includes.h"

class Spritesheet
{
public:
	Spritesheet(SDL_Renderer *ren, std::string image, int spriteNumber, int speed);
	~Spritesheet();
	Spritesheet(std::string image, int spriteNumber, int speed);
private:
	int spriteCount;
	int widthOfSprite;
	std::string imagePath;
	int spriteSpeed;
	SDL_Rect location;
public:
	int getSpriteCount();
private:
	SDL_Texture *sprites;
	void CleanExit(int);
	SDL_Renderer *renderer;
	int direction;
	int prevDirection;
	bool dead = false;
	Uint32 deathTime;
public:
	SDL_Texture * getSpriteSheet();
	void renderSprite(SDL_Rect destinationRect);
	void renderSprite();
	SDL_Rect updateAnimation();
	SDL_Rect updateAnimation(int num);
	void newRenderer(SDL_Renderer *ren);
	void updateLocation();
	void changeDirection(int newDirection);
	int getDirection();
	void reverseMove();
	SDL_Rect getLocation();
	void setLocation(int x, int y);
	void die(SDL_Renderer *ren);
};

