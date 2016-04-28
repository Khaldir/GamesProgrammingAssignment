#include "includes.h"
#include "Spritesheet.h"
#include "Collectable.h"

void sources()
{
	std::cout << "References" << std::endl;
	std::cout << "-------" << std::endl;
	std::cout << "The following references were used to aid the programming of this game" << std::endl;
	std::cout << "No code was taken directly from any of these sites" << std::endl;
	std::cout << "==============================================================================" << std::endl;

	std::cout << "Collisions: " << std::endl; 
	std::cout << "http://headerphile.blogspot.co.uk/2014/04/part-5-game-programming-in-sdl2.html" << std::endl;
	std::cout << "Sprites: " << std::endl; 
	std::cout << "http://kafumble.blogspot.co.uk/2011/05/pacman.html" << std::endl;
	std::cout << "Sounds: " << std::endl;
	std::cout << "http://lazyfoo.net/SDL_tutorials/lesson11/" << std::endl;

	std::cout << "==============================================================================" << std::endl;

}

std::string exeName;
SDL_Window *win; //pointer to the SDL_Window
SDL_Renderer *ren; //pointer to the SDL_Renderer
SDL_Surface *surface; //pointer to the SDL_Surface
SDL_Texture *tex; //pointer to the SDL_Texture
SDL_Surface *messageSurface; //pointer to the SDL_Surface for message
SDL_Texture *messageTexture; //pointer to the SDL_Texture for message
SDL_Rect message_rect; //SDL_rect for the message
Mix_Music *music;
Mix_Chunk *eatPac;
Mix_Chunk *death;

bool Debug = false;
bool paused = true;

Spritesheet pacman("pacman", 2, 150);
Spritesheet ghostRed("ghostred", 2, 250);
Spritesheet ghostPink("ghostpink", 2, 250);
Spritesheet ghostOrange("ghostorange", 2, 250);
Spritesheet ghostBlue("ghostblue", 2, 250);

bool done = false;
bool dead = false;
int pacRemaining = 244;
int score = 0;
int pacCount = 0;
bool firstStart = true;
double wMod = 1;
double hMod = 1;

SDL_Rect voidzone[42];
Collectable *pac[244];



void cleanExit(int returnValue)
{
	if (messageTexture != nullptr) SDL_DestroyTexture(messageTexture);
	if (tex != nullptr) SDL_DestroyTexture(tex);
	if (ren != nullptr) SDL_DestroyRenderer(ren);
	if (win != nullptr) SDL_DestroyWindow(win);
	if (music != nullptr) Mix_FreeMusic(music);
	if (eatPac != nullptr) Mix_FreeChunk(eatPac);
	Mix_CloseAudio();
	SDL_Quit();
	exit(returnValue);
}

void updateScore()
{
	message_rect.w = 115;
	TTF_Font* sans = TTF_OpenFont("./assets/joystix.ttf", 96);
	if (sans == nullptr)
	{
		std::cout << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
		cleanExit(1);
	}
	SDL_Color White = { 255, 255, 255 };
	std::string scoreDisplay = "Score: " + std::to_string(score);
	messageSurface = TTF_RenderText_Solid(sans, scoreDisplay.c_str(), White);
	messageTexture = SDL_CreateTextureFromSurface(ren, messageSurface);
}

SDL_Rect fillRect(int x, int y, int w, int h)
{
	SDL_Rect tempRect;
	tempRect.x = x*wMod;
	tempRect.y = y*hMod;
	tempRect.w = w*wMod;
	tempRect.h = h*hMod;
	return tempRect;
}

void mirrorFill(int count, int x, int y, int w, int h)
{
	voidzone[count] = fillRect(x, y, w*wMod, h*hMod);
	voidzone[count + 1] = fillRect(463 - (x + w), y, w*wMod, h*hMod);
}

void mirrorPac(int x, int y)
{
	if (firstStart)
	{
		pac[pacCount] = new Collectable();
		pac[pacCount]->fillVariables("pac", 2, 0);
		pac[pacCount]->newRenderer(ren);
	}
	pac[pacCount]->setLocation(x*wMod, y*hMod);
	pacCount++;
	if (firstStart)
	{
		pac[pacCount] = new Collectable();
		pac[pacCount]->fillVariables("pac", 2, 0);
		pac[pacCount]->newRenderer(ren);
	}
	pac[pacCount]->setLocation((463 - (x + 13))*wMod, y*hMod);
	pacCount++;
}

void mirrorPac(int x, int y, int num)
{
	if (firstStart)
	{
		pac[pacCount] = new Collectable();
		pac[pacCount]->fillVariables("pac", 2, 1);
		pac[pacCount]->newRenderer(ren);
	}
	pac[pacCount]->setLocation((double)x*(double)wMod, (double)y*(double)hMod);
	pacCount++;
	if (firstStart)
	{
		pac[pacCount] = new Collectable();
		pac[pacCount]->fillVariables("pac", 2, 1);
		pac[pacCount]->newRenderer(ren);
	}
	pac[pacCount]->setLocation((463 - (x + 13))*wMod, y*hMod);
	pacCount++;
}

void initialisePacDots()
{
	pacCount = 0;
	mirrorPac(18, 23);		//0,1
	mirrorPac(35, 23);
	mirrorPac(51, 23);
	mirrorPac(68, 23);
	mirrorPac(84, 23);
	mirrorPac(101, 21);		//10,11
	mirrorPac(118, 23);
	mirrorPac(134, 23);
	mirrorPac(151, 23);
	mirrorPac(167, 23);
	mirrorPac(184, 23);		//20,21
	mirrorPac(200, 23);
	mirrorPac(18, 41);
	mirrorPac(101, 41);
	mirrorPac(200, 42);
	mirrorPac(101, 60);		//30,31
	mirrorPac(200, 62);
	mirrorPac(18, 81);
	mirrorPac(101, 79);
	mirrorPac(200, 81);
	mirrorPac(18, 100);		//40,41
	mirrorPac(35, 100);
	mirrorPac(51, 100);
	mirrorPac(68, 100);
	mirrorPac(84, 100);
	mirrorPac(101, 99);		//50,51
	mirrorPac(118, 100);
	mirrorPac(134, 100);
	mirrorPac(151, 100);
	mirrorPac(167, 100);
	mirrorPac(184, 100);	//60,61
	mirrorPac(200, 100);
	mirrorPac(217, 100);
	mirrorPac(18, 120);
	mirrorPac(101, 118);
	mirrorPac(151, 120);	//70,71
	mirrorPac(18, 139);
	mirrorPac(101, 138);
	mirrorPac(151, 139);
	mirrorPac(18, 158);
	mirrorPac(35, 158);		//80,81
	mirrorPac(51, 158);
	mirrorPac(68, 158);
	mirrorPac(84, 158);
	mirrorPac(101, 157);
	mirrorPac(151, 158);	//90,91
	mirrorPac(167, 158);
	mirrorPac(184, 158);
	mirrorPac(200, 158);
	mirrorPac(101, 177);
	mirrorPac(101, 197);	//100,101
	mirrorPac(101, 216);
	mirrorPac(101, 236);
	mirrorPac(101, 255);
	mirrorPac(101, 274);
	mirrorPac(101, 294);	//110,111
	mirrorPac(101, 313);
	mirrorPac(101, 333);
	mirrorPac(101, 352);
	mirrorPac(101, 371);
	mirrorPac(18, 391);		//120, 121
	mirrorPac(35, 391);
	mirrorPac(51, 391);
	mirrorPac(68, 391);
	mirrorPac(85, 391);
	mirrorPac(101, 391);	//130,131
	mirrorPac(118, 391);
	mirrorPac(134, 391);
	mirrorPac(151, 391);
	mirrorPac(167, 391);
	mirrorPac(184, 391);	//140,141
	mirrorPac(200, 391);
	mirrorPac(18, 410);
	mirrorPac(101, 410);
	mirrorPac(200, 410);
	mirrorPac(18, 429);		//150,151
	mirrorPac(101, 429);
	mirrorPac(200, 429);
	mirrorPac(35, 449);
	mirrorPac(51, 449);
	mirrorPac(101, 448);	//160,161
	mirrorPac(118, 448);
	mirrorPac(135, 448);
	mirrorPac(151, 448);
	mirrorPac(168, 448);
	mirrorPac(184, 448);	//170,171
	mirrorPac(200, 448);
	mirrorPac(51, 468);
	mirrorPac(101, 468);
	mirrorPac(151, 468);
	mirrorPac(51, 487);		//180,181
	mirrorPac(101, 487);
	mirrorPac(151, 487);
	mirrorPac(18, 507);
	mirrorPac(35, 507);
	mirrorPac(51, 507);		//190,191
	mirrorPac(68, 507);
	mirrorPac(84, 507);
	mirrorPac(101, 507);
	mirrorPac(151, 507);
	mirrorPac(167, 507);	//200,201
	mirrorPac(184, 507);
	mirrorPac(200, 507);
	mirrorPac(18, 526);
	mirrorPac(200, 526);
	mirrorPac(18, 545);		//210,211
	mirrorPac(200, 545);
	mirrorPac(18, 565);
	mirrorPac(35, 565);
	mirrorPac(51, 565);
	mirrorPac(68, 565);		//220,221
	mirrorPac(84, 565);
	mirrorPac(101, 565);
	mirrorPac(118, 565);
	mirrorPac(134, 565);
	mirrorPac(151, 565);	//230,231
	mirrorPac(167, 565);
	mirrorPac(184, 565);
	mirrorPac(200, 565);
	mirrorPac(217, 565);

	mirrorPac(15, 59);	//240,241
	mirrorPac(15, 446);	
}

void initialiseVoidZones()
{
	mirrorFill(0, 41, 48, 50, 40);
	mirrorFill(2, 123, 48, 68, 40);
	mirrorFill(4, 41, 126, 50, 20);
	mirrorFill(6, 123, 416, 68, 20);
	voidzone[8] = fillRect(222, 10, 18, 79);
	voidzone[9] = fillRect(0, 0, 464, 10);
	mirrorFill(10, 0, 10, 9, 252);
	mirrorFill(12, 9, 184, 82, 78);
	mirrorFill(14, 123, 126, 18, 136);
	mirrorFill(16, 141, 184, 50, 20);
	mirrorFill(18, 123, 300, 18, 78);
	voidzone[20] = fillRect(173, 126, 117, 20);
	voidzone[21] = fillRect(223, 146, 17, 58);		
	voidzone[22] = fillRect(0, 591, 464, 9);		
	voidzone[23] = fillRect(223, 494, 17, 58);		
	voidzone[24] = fillRect(173, 474, 117, 20);		
	voidzone[25] = fillRect(223, 378, 17, 58);		
	voidzone[26] = fillRect(173, 358, 117, 20);		
	mirrorFill(27, 0, 300, 9, 291);					
	mirrorFill(29, 9, 300, 82, 78);					
	mirrorFill(31, 9, 474, 33, 20);					
	mirrorFill(33, 41, 416, 50, 20);
	mirrorFill(35, 74, 436, 17, 58);
	mirrorFill(37, 123, 474, 18, 58);				
	mirrorFill(39, 41, 532, 150, 20);				
	voidzone[41] = fillRect(173, 242, 117, 78);

}

bool CheckCollisions(Spritesheet entity, bool pacman)
{
	SDL_Rect eRect = entity.getLocation();
	int eLeft = eRect.x;
	int eRight = eRect.x + eRect.w;
	int eTop = eRect.y;
	int eBottom = eRect.y + eRect.h;
	int overlaps = 0;

	int vLeft;
	int vRight;
	int vTop;
	int vBottom;

	for (int i = 0; i < 41; i++)
	{
		overlaps = 0;
		// CoOrdinates of entity corners
		vLeft = voidzone[i].x;
		vRight = voidzone[i].x + voidzone[i].w;
		vTop = voidzone[i].y;
		vBottom = voidzone[i].y + voidzone[i].h;

		if (eLeft<vRight && eRight>vLeft && eBottom>vTop && eTop<vBottom)	
		{
			return true;
		}
	}
	if (pacman)
	{
		overlaps = 0;
		// CoOrdinates of entity corners
		vLeft = voidzone[41].x ;
		vRight = voidzone[41].x + voidzone[41].w;
		vTop = voidzone[41].y;
		vBottom = voidzone[41].y + voidzone[41].h;

		if (eLeft<vRight && eRight>vLeft && eBottom>vTop && eTop<vBottom)	
		{
			return true;
		}
	}
	//Checked all void zones, no collision
	return false;
}

bool CheckCollisions(SDL_Rect eRect)
{
	int eLeft = eRect.x;
	int eRight = eRect.x + eRect.w;
	int eTop = eRect.y;
	int eBottom = eRect.y + eRect.h;
	int overlaps = 0;

	int vLeft;
	int vRight;
	int vTop;
	int vBottom;

	for (int i = 0; i < 42; i++)
	{
		overlaps = 0;
		// CoOrdinates of entity corners
		vLeft = voidzone[i].x;
		vRight = voidzone[i].x + voidzone[i].w;
		vTop = voidzone[i].y;
		vBottom = voidzone[i].y + voidzone[i].h;

		if (eLeft<vRight && eRight>vLeft && eBottom>vTop && eTop<vBottom)
		{
			return true;
		}
	}
	//Checked all void zones, no collision
	return false;
}

bool CheckCollisions(Spritesheet firstEntity, Spritesheet secondEntity)
{
	SDL_Rect rect = firstEntity.getLocation();
	int eLeft = rect.x;
	int eRight = rect.x + rect.w;
	int eTop = rect.y;
	int eBottom = rect.y + rect.h;
	int overlaps = 0;

	rect = secondEntity.getLocation();

	int vLeft = rect.x;
	int vRight = rect.x + rect.w;
	int vTop = rect.y;
	int vBottom = rect.y + rect.h;

	if (eLeft<vRight && eRight>vLeft && eBottom>vTop && eTop<vBottom)	
	{
		return true;
	}
	
	
	//Checked all void zones, no collision
	return false;
}

bool CheckCollisions(Collectable * firstEntity, Spritesheet secondEntity)
{
	SDL_Rect rect = firstEntity->getLocation();
	int eLeft = rect.x;
	int eRight = rect.x + rect.w;
	int eTop = rect.y;
	int eBottom = rect.y + rect.h;
	int overlaps = 0;

	rect = secondEntity.getLocation();

	int vLeft = rect.x + (rect.w / 2);
	int vRight = rect.x + rect.w - (rect.w / 2);
	int vTop = rect.y + (rect.h / 2);
	int vBottom = rect.y + rect.h - (rect.h / 2);

	if (eLeft<vRight && eRight>vLeft && eBottom>vTop && eTop<vBottom)
	{
		return true;
	}


	//Checked all void zones, no collision
	return false;
}

SDL_Rect modifyRect(SDL_Rect locations)
{
	locations.x = locations.x * wMod;
	locations.w = locations.w * wMod;
	locations.y = locations.y * hMod;
	locations.h = locations.h * hMod;
	return locations;
}

void windowResized()
{
	initialisePacDots();
	initialiseVoidZones();
	SDL_Rect locations = pacman.getLocation();
	locations = modifyRect(locations);
	pacman.setLocation(locations);

	locations = ghostRed.getLocation();
	locations = modifyRect(locations);
	ghostRed.setLocation(locations);

	locations = ghostPink.getLocation();
	locations = modifyRect(locations);
	ghostPink.setLocation(locations);

	locations = ghostOrange.getLocation();
	locations = modifyRect(locations);
	ghostOrange.setLocation(locations);

	locations = ghostBlue.getLocation();
	locations = modifyRect(locations);
	ghostBlue.setLocation(locations);
}

void handleInput()
{
	//Event-based input handling
	//The underlying OS is event-based, so **each** key-up or key-down (for example)
	//generates an event.
	//  - https://wiki.libsdl.org/SDL_PollEvent
	//In some scenarios we want to catch **ALL** the events, not just to present state
	//  - for instance, if taking keyboard input the user might key-down two keys during a frame
	//    - we want to catch based, and know the order
	//  - or the user might key-down and key-up the same within a frame, and we still want something to happen (e.g. jump)
	//  - the alternative is to Poll the current state with SDL_GetKeyboardState

	SDL_Event event; //somewhere to store an event
	SDL_Rect playerLocation;

	//NOTE: there may be multiple events per frame
	while (SDL_PollEvent(&event)) //loop until SDL_PollEvent returns 0 (meaning no more events)
	{
		switch (event.type)
		{
		case SDL_WINDOWEVENT:
		{
			if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
			{
				windowResized();
			}
			break;
		}
		case SDL_QUIT:
			done = true; //set donecreate remote branch flag if SDL wants to quit (i.e. if the OS has triggered a close event,
							//  - such as window close, or SIGINT
			break;

			//keydown handling - we should to the opposite on key-up for direction controls (generally)
		case SDL_KEYDOWN:
			//Keydown can fire repeatable if key-repeat is on.
			//  - the repeat flag is set on the keyboard event, if this is a repeat event
			//  - in our case, we're going to ignore repeat events
			//  - https://wiki.libsdl.org/SDL_KeyboardEvent
			switch (event.key.keysym.sym)
			{
				//hit escape to exit
				
				case SDLK_ESCAPE: 
				{
					Mix_HaltMusic();
					paused = !paused;
					if (paused)
						Mix_HaltChannel(-1);
					break;
				}
				case SDLK_UP: 
				{
					
					
					if (paused)
					{
						if (Mix_Volume(-1, -1) > 117)
						{
							if (Mix_Volume(-1, 128) == -1)
							{
								std::cout << "SDL_Mixer Error: " << Mix_GetError() << std::endl;
								cleanExit(1);
							}
						}
						else
						{
							if (Mix_Volume(-1, Mix_Volume(-1, -1) + 10) == -1)
							{
								std::cout << "SDL_Mixer Error: " << Mix_GetError() << std::endl;
								cleanExit(1);
							}
						}
					}
					else
					{
						playerLocation = pacman.getLocation();
						playerLocation.y -= 5;
						bool collide = CheckCollisions(playerLocation);
						if (!collide)
							pacman.changeDirection(2);
					}
					break;
				}
				case SDLK_DOWN: 
				{
					if (paused)
					{
						if (Mix_Volume(-1, -1) < 11)
						{
							if (Mix_Volume(-1, 0) == -1)
							{
								std::cout << "SDL_Mixer Error: " << Mix_GetError() << std::endl;
								cleanExit(1);
							}
						}
						else
						{
							if (Mix_Volume(-1, Mix_Volume(-1, -1) - 10) == -1)
							{
								std::cout << "SDL_Mixer Error: " << Mix_GetError() << std::endl;
								cleanExit(1);
							}
						}
					}
					else
					{
						playerLocation = pacman.getLocation();
						playerLocation.y += 5;
						bool collide = CheckCollisions(playerLocation);
						if (!collide)
							pacman.changeDirection(3);
					}
					break;
				}
				case SDLK_RIGHT: 
				{
					playerLocation = pacman.getLocation();
					playerLocation.x += 5;
					bool collide = CheckCollisions(playerLocation);
					if (!collide)
						pacman.changeDirection(0);
					break;
				}
				case SDLK_LEFT: 
				{
					playerLocation = pacman.getLocation();
					playerLocation.x -= 5;
					bool collide = CheckCollisions(playerLocation);
					if (!collide)
						pacman.changeDirection(1);
					break;
				}
			}
			break;
		}
	}
}
// end::handleInput[]

void moveGhost(Spritesheet * ghost)
{
	ghost->updateLocation();
	bool Collided = CheckCollisions(*ghost, false);
	if (Collided)
	{
		ghost->reverseMove();
		int ghostDirection = ghost->getDirection();
		switch (ghostDirection)
		{
		case 0:
		{
			ghost->changeDirection(3);
			ghost->updateLocation();
			bool Collided = CheckCollisions(*ghost, false);
			if (Collided)
			{
				ghost->reverseMove();
				ghost->changeDirection(2);
			}
			break;
		}
		case 1:
		{
			ghost->changeDirection(3);
			ghost->updateLocation();
			bool Collided = CheckCollisions(*ghost, false);
			if (Collided)
			{
				ghost->reverseMove();
				ghost->changeDirection(2);
			}
			break;
		}
		case 2:
		{
			ghost->changeDirection(1);
			ghost->updateLocation();
			bool Collided = CheckCollisions(*ghost, false);
			if (Collided)
			{
				ghost->reverseMove();
				ghost->changeDirection(0);
			}
			break;
		}
		case 3:
		{
			ghost->changeDirection(1);
			ghost->updateLocation();
			bool Collided = CheckCollisions(*ghost, false);
			if (Collided)
			{
				ghost->reverseMove();
				ghost->changeDirection(0);
			}
			break;
		}
		}
		moveGhost(ghost);
	}
	else
	{
		int junctiondots[14]{ 10,40,50,56,62,88,108,130,136,160,166,172,190,236 };
		for (int i = 0; i < 14; i++)
		{
			bool firstCheck = CheckCollisions(pac[junctiondots[i]], *ghost);
			bool secondCheck = CheckCollisions(pac[junctiondots[i] + 1], *ghost);
			Collided = (firstCheck || secondCheck);
			if (Collided)
				break;
		}
		bool directionChosen = ghost->directionPicked();
		if (Collided && !directionChosen)
		{
			SDL_Rect ghostPosition = ghost->getLocation();
			SDL_Rect pacmanPosition = pacman.getLocation();
			int xDiff = ghostPosition.x - pacmanPosition.x;
			int yDiff = ghostPosition.y - pacmanPosition.y;
			
			if (abs(xDiff) > abs(yDiff))	//larger difference in x
			{
				if (xDiff < 0)
					ghost->changeDirection(0);
				else
					ghost->changeDirection(1);
			}
			else
			{
				if (yDiff < 0)
					ghost->changeDirection(3);
				else
					ghost->changeDirection(2);
			}
			ghost->toggleDirectionPicked();
		}
		if (!Collided && directionChosen)
		{
			ghost->toggleDirectionPicked();
		}
	}

}

// tag::updateSimulation[]
void updateSimulation(int ghosts, double simLength = 0.02) //update simulation with an amount of time to simulate for (in seconds)
{
	//move pacman
	pacman.updateLocation();
	bool Collided = CheckCollisions(pacman, true);
	if (Collided)
	{
		pacman.reverseMove();
	}
	Collided = CheckCollisions(pacman, ghostRed) || CheckCollisions(pacman, ghostPink) || CheckCollisions(pacman, ghostOrange) || CheckCollisions(pacman, ghostBlue);
	if (Collided && !dead)
	{
		dead = true;
		pacman.die(ren);
		if (Mix_PlayChannel(-1, death, 0) == -1)
		{
			std::cout << "SDL_Mixer Error: " << Mix_GetError() << std::endl;
			cleanExit(1);
		}
	}
	for (int i = 0; i < pacRemaining; i++)
	{
		if (pac[i]->isCollected())
		{

		}
		else 
		{
			Collided = CheckCollisions(pac[i], pacman);
			if (Collided)
			{
				if (Mix_PlayChannel(1, eatPac, 0) == -1)
				{
					std::cout << "SDL_Mixer Error: " << Mix_GetError() << std::endl;
					cleanExit(1);
				}
				pac[i]->collect();
				score++;
				updateScore();
				
			}
		}
		
	}
	moveGhost(&ghostRed);
	if (ghosts > 1)
		moveGhost(&ghostPink);
	if (ghosts > 2)
		moveGhost(&ghostOrange);
	if (ghosts > 3)
		moveGhost(&ghostBlue);
}

void render()
{
		//First clear the renderer
		SDL_RenderClear(ren);
		int wPointer;
		int hPointer;
		SDL_GetWindowSize(win, &wPointer, &hPointer);
		wMod = (double)wPointer / (double)464;
		hMod = (double)hPointer / (double)600;
		
		//Draw the texture
		SDL_Rect background;
		background.x = 0;
		background.y = 0;
		background.h = 600 * hMod;
		background.w = 464 * wMod;
		SDL_RenderCopy(ren, tex, NULL, &background);

		//Draw the sprites
		pacman.renderSprite();
		ghostRed.renderSprite();
		ghostPink.renderSprite();
		ghostOrange.renderSprite();
		ghostBlue.renderSprite();

		//Draw the pac dots
		for (int i = 0; i < pacRemaining; i++)
		{
			pac[i]->render();
		}

		SDL_RenderCopy(ren, messageTexture, NULL, &message_rect);

		//Only Execute if Debugging
		if (Debug)
		{
			//Debug - show Out of Bounds Area

			for (int i = 0; i < 42; i++)
			{
				SDL_Rect plainGhostColour = fillRect(5, 0, 1, 1);
				SDL_RenderCopy(ren, ghostRed.getSpriteSheet(), &plainGhostColour, &voidzone[i]);
			}
		}

		//Update the screen
		SDL_RenderPresent(ren);
		
}




// based on http://www.willusher.io/sdl2%20tutorials/2013/08/17/lesson-1-hello-world/
int main( int argc, char* args[] )
{
	initialiseVoidZones();
	sources();
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		cleanExit(1);
	}
	std::cout << "SDL initialised OK!\n";

	//Initialize SDL_mixer
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		std::cout << "SDL_Mixer Error: " << Mix_GetError() << std::endl;
		cleanExit(1);
	}

	//create window
	win = SDL_CreateWindow("SDL Hello World!", 100, 100, 464, 600, SDL_WINDOW_SHOWN);

	//error handling
	if (win == nullptr)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		cleanExit(1);
	}
	std::cout << "SDL CreatedWindow OK!\n";

	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr)
	{
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		cleanExit(1);
	}

	std::string imagePath = "./assets/background.png";
	surface = IMG_Load(imagePath.c_str());
	if (surface == nullptr){
		std::cout << "SDL IMG_Load Error: " << SDL_GetError() << std::endl;
		cleanExit(1);
	}

	tex = SDL_CreateTextureFromSurface(ren, surface);
	SDL_FreeSurface(surface);
	if (tex == nullptr){
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		cleanExit(1);
	}


	if( TTF_Init() == -1 )
	{
		std::cout << "TTF_Init Failed: " << TTF_GetError() << std::endl;
		cleanExit(1);
	}

	TTF_Font* sans = TTF_OpenFont("./assets/joystix.ttf", 96);
	if (sans == nullptr)
	{
		std::cout << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
		cleanExit(1);
	}
	SDL_Color White = {255, 255, 255};
	
	

	music = Mix_LoadMUS("./assets/music.wav");
	if (music == nullptr)
	{
		std::cout << "Mix_Music Error: " << Mix_GetError() << std::endl;
		cleanExit(1);
	}

	eatPac = Mix_LoadWAV("./assets/eatPac.wav");
	if (eatPac == nullptr)
	{
		std::cout << "Mix_Music Error: " << Mix_GetError() << std::endl;
		cleanExit(1);
	}

	death = Mix_LoadWAV("./assets/death.wav");
	if (death == nullptr)
	{
		std::cout << "Mix_Music Error: " << Mix_GetError() << std::endl;
		cleanExit(1);
	}

	//Play Music
	if (Mix_PlayMusic(music, 1) == -1)
	{
		std::cout << "Mix_Music Error: " << Mix_GetError() << std::endl;
		cleanExit(1);
	}

	std::string scoreDisplay = "Press ESC to pause, up/down while paused changes volume" + score;
	messageSurface = TTF_RenderText_Solid(sans, scoreDisplay.c_str(), White);
	messageTexture = SDL_CreateTextureFromSurface(ren, messageSurface);
	message_rect.x = 10;
	message_rect.y = 0;
	message_rect.w = 444;
	message_rect.h = 25;

	//Now the renderer is made, add it to the sprites
	pacman.newRenderer(ren);
	ghostRed.newRenderer(ren);
	ghostPink.newRenderer(ren);
	ghostOrange.newRenderer(ren);
	ghostBlue.newRenderer(ren);

	initialisePacDots();
	
	for (int i = 0; i < pacRemaining; i++)
	{
		pac[i]->newRenderer(ren);
	}

	//Set the starting locations for the sprites
	pacman.setLocation(219, 325);
	ghostRed.setLocation(219, 210);
	ghostPink.setLocation(219, 267);
	ghostPink.changeDirection(2);
	ghostOrange.setLocation(219, 267);
	ghostOrange.changeDirection(2);
	ghostBlue.setLocation(219, 267);
	ghostBlue.changeDirection(2);

	Uint32 startTime = SDL_GetTicks() + 4217;
	Uint32 ghost2 = startTime + 3000;
	Uint32 ghost3 = ghost2 + 3000;
	Uint32 ghost4 = ghost3 + 3000;
	Uint32 currentTicks;
	while (!done) //loop until done flag is set)
	{
		
		
		handleInput(); // this should ONLY SET VARIABLES

		currentTicks = SDL_GetTicks();
		int ghosts = 1;
		if (SDL_TICKS_PASSED(currentTicks, ghost2))
			ghosts = 2;
		if (SDL_TICKS_PASSED(currentTicks, ghost3))
			ghosts = 3;
		if (SDL_TICKS_PASSED(currentTicks, ghost4))
			ghosts = 4;
		if (SDL_TICKS_PASSED(currentTicks, startTime) && firstStart)
		{
			paused = false;
			firstStart = false;
		}
		if (!paused)
			updateSimulation(ghosts); // this should ONLY SET VARIABLES according to simulation

		

		render(); // this should render the world state according to VARIABLES

		SDL_Delay(20); // unless vsync is on??
	}

	cleanExit(0);
	return 0;
}


