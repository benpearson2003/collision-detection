/*
* Author:	Benjamin Pearson
*			UALR
* 
* Last Updated:		03 July 2013	
*
* File:		main.cpp
*
* Purpose:	Collision Detection utilizing RK4 and a binary search in regards to time.
* This project has been adapted from a shell provided by Dr. Keith Bush at UALR.
* The RK4 method was adapted from Glen Fiedler's version
*	at http://gafferongames.com/game-physics/integration-basics/ 
*
*This version: This focuses on combining the evaluation of wall-ball collisions and ball-ball
*collisions. The binary search is not used because it has not yet been perfected, and this was
*just for combining those collisions and ensuring functionality in that form.*/

#include <fstream>
#include <cstdlib> 
#include <ctime> 
#include <string>
#include <iostream>

#include "GUI.h"
#include "engine.h"
#include "constants.h"
#include "timer.h"

using namespace std;

int main(int argc, char *argv[]){

	//Seed random number generator
	srand((unsigned)time(0)); 

	//Initialize GUI->Engine communication*/
	string command = "NA";

	//Construct Timer
	Timer fps;
	
	//Construct GUI
	GUI gui;

	//Construct Engine (number of bodies is argument)
	Engine engine(4);

	//Initialize Termination Criteria
	bool quit = false;
	
	//While the user hasn't quit
	while(quit == false){

		//Start the frame timer
		fps.start();
		
		//GUI waits for mouse events
		while(gui.observeEvent()){

			//GUI transmits quit event
			if(gui.quitGame()){
				quit=true;
			}
		}

		//Task 3: Update Game state
		fps.pause();
		engine.update(fps.get_ticks(),1.0f);
		fps.unpause();

		//Render Game Data
		gui.displayGameState(engine); 

		//Adjust Frame Rate
		if(fps.get_ticks() < FRAME_DELAY){
			SDL_Delay( FRAME_DELAY - fps.get_ticks() );
		}
	}

	//Return
	return 0;

}