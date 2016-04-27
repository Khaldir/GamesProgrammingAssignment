#include "includes.h"
#include "Spritesheet.h"

void sources()
{
	std::cout << "Sources" << std::endl;
	std::cout << "-------" << std::endl;
	std::cout << "The following sources were used to aid the programming of this game" << std::endl;
	std::cout << "No code was taken directly from any of these sites" << std::endl;
	std::cout << "==============================================================================" << std::endl;

	std::cout << "Collisions: " << std::endl; 
	std::cout << "http://headerphile.blogspot.co.uk/2014/04/part-5-game-programming-in-sdl2.html" << std::endl;
	std::cout << "Sprites: " << std::endl; 
	std::cout << "http://kafumble.blogspot.co.uk/2011/05/pacman.html" << std::endl;

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

bool Debug = false;

Spritesheet pacman("pacman", 2, 150);
Spritesheet ghostRed("ghostred", 2, 250);

bool done = false;

SDL_Rect voidzone[42];

SDL_Rect fillRect(int x, int y, int w, int h)
{
	SDL_Rect tempRect;
	tempRect.x = x;
	tempRect.y = y;
	tempRect.w = w;
	tempRect.h = h;
	return tempRect;
}

void mirrorFill(int count, int x, int y, int w, int h)
{
	voidzone[count] = fillRect(x, y, w, h);
	voidzone[count + 1] = fillRect(463 - (x + w), y, w, h);
}

void initialiseVoidZones()
{
	mirrorFill(0, 45, 45, 56, 56);
	mirrorFill(2, 135, 45, 57, 56);
	mirrorFill(4, 45, 135, 56, 34);
	mirrorFill(6, 135, 430, 54, 11);
	voidzone[8] = fillRect(226, 11, 11, 90);
	voidzone[9] = fillRect(0, 0, 464, 11);
	mirrorFill(10, 0, 11, 11, 272);
	mirrorFill(12, 11, 203, 90, 80);
	mirrorFill(14, 135, 135, 12, 148);
	mirrorFill(16, 147, 203, 42, 12);
	mirrorFill(18, 135, 316, 12, 80);
	voidzone[20] = fillRect(181, 135, 101, 34);
	voidzone[21] = fillRect(226, 169, 11, 46);
	voidzone[22] = fillRect(0, 588, 464, 12);
	voidzone[23] = fillRect(226, 509, 11, 45);
	voidzone[24] = fillRect(181, 475, 101, 34);
	voidzone[25] = fillRect(226, 396, 11, 45);
	voidzone[26] = fillRect(181, 362, 101, 34);
	mirrorFill(27, 0, 316, 11, 272);
	mirrorFill(29, 11, 316, 90, 80);
	mirrorFill(31, 11, 475, 45, 34);
	mirrorFill(33, 48, 430, 53, 11);
	mirrorFill(35, 90, 441, 11, 68);
	mirrorFill(37, 135, 475, 12, 68);
	mirrorFill(39, 48, 543, 141, 11);
	voidzone[41] = fillRect(181, 249, 101, 79);

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

		if (eLeft<vRight && eRight>vLeft && eBottom>vTop && eTop<vBottom)	//Inside on x
		{
			return true;
		}
	}
	if (pacman)
	{
		overlaps = 0;
		// CoOrdinates of entity corners
		vLeft = voidzone[41].x;
		vRight = voidzone[41].x + voidzone[41].w;
		vTop = voidzone[41].y;
		vBottom = voidzone[41].y + voidzone[41].h;

		if (eLeft<vRight && eRight>vLeft && eBottom>vTop && eTop<vBottom)	//Inside on x
		{
			return true;
		}
	}
	//Checked all void zones, no collision
	return false;
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

	//NOTE: there may be multiple events per frame
	while (SDL_PollEvent(&event)) //loop until SDL_PollEvent returns 0 (meaning no more events)
	{
		switch (event.type)
		{
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
			if (!event.key.repeat)
				switch (event.key.keysym.sym)
				{
					//hit escape to exit
					case SDLK_ESCAPE: done = true;
					case SDLK_UP: 
					{
						pacman.changeDirection(2);
						break;
					}
					case SDLK_DOWN: 
					{
						pacman.changeDirection(3);
						break;
					}
					case SDLK_RIGHT: 
					{
						pacman.changeDirection(0);
						break;
					}
					case SDLK_LEFT: 
					{
						pacman.changeDirection(1);
						break;
					}
				}
			break;
		}
	}
}
// end::handleInput[]

// tag::updateSimulation[]
void updateSimulation(double simLength = 0.02) //update simulation with an amount of time to simulate for (in seconds)
{
	//move pacman
	pacman.updateLocation();
	bool Collided = CheckCollisions(pacman, true);
	if (Collided)
	{
		pacman.reverseMove();
	}
}

void render()
{
		//First clear the renderer
		SDL_RenderClear(ren);

		//Draw the texture
		SDL_RenderCopy(ren, tex, NULL, NULL);

		//Draw the sprites
		pacman.renderSprite();
		ghostRed.renderSprite();

		//Only Execute if Debugging
		if (Debug)
		{
			//Debug - show Out of Bounds Area

			for (int i = 0; i < 42; i++)
			{
				SDL_Rect plainGhostColour = fillRect(5, 0, 1, 1);
				SDL_RenderCopy(ren, ghostRed.getSpriteSheet(), &plainGhostColour, &voidzone[i]);
			}
			done = true;
		}

		//Update the screen
		SDL_RenderPresent(ren);
		
}

void cleanExit(int returnValue)
{
	if (messageTexture != nullptr) SDL_DestroyTexture(messageTexture);
	if (tex != nullptr) SDL_DestroyTexture(tex);
	if (ren != nullptr) SDL_DestroyRenderer(ren);
	if (win != nullptr) SDL_DestroyWindow(win);
	SDL_Quit();
	exit(returnValue);
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

	TTF_Font* sans = TTF_OpenFont("./assets/Hack-Regular.ttf", 96);
	if (sans == nullptr)
	{
		std::cout << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
		cleanExit(1);
	}
	SDL_Color White = {255, 255, 255};
	messageSurface = TTF_RenderText_Solid(sans, "Hello World!", White);
	messageTexture = SDL_CreateTextureFromSurface(ren, messageSurface);
	message_rect.x = 0;
	message_rect.y = 0;
	message_rect.w = 300;
	message_rect.h = 100;

	//Now the renderer is made, add it to the sprites
	pacman.newRenderer(ren);
	ghostRed.newRenderer(ren);

	while (!done) //loop until done flag is set)
	{
		handleInput(); // this should ONLY SET VARIABLES

		updateSimulation(); // this should ONLY SET VARIABLES according to simulation

		render(); // this should render the world state according to VARIABLES

		SDL_Delay(20); // unless vsync is on??
	}

	cleanExit(0);
	return 0;
}


