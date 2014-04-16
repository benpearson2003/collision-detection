#ifndef GUI_H
#define GUI_H

#include <string>

//GUI definitions
#include "SDL.h"
#include "SDL_image.h"
#include "constants.h"
#include "Engine.h"
#include "Body.h"

//class ActiveTerritory;

using namespace std;

class GUI{

public:
	GUI();
	~GUI();
	int observeEvent();
	bool quitGame();
	void displayGameState(Engine&);
private:

	//PRIVATE METHODS
	void initGUI();
	SDL_Surface* loadImage(string, int);
	void cleanUp();
	void applySurface(int, int, SDL_Surface*, SDL_Surface*, SDL_Rect* = NULL);

	//PRIVATE MEMBERS
	bool quit;

	//The surfaces that will be used for images
	SDL_Surface* bodySheet;
	SDL_Surface* screen;

	//The event structure that will be used
	SDL_Event event;

};

#endif
















