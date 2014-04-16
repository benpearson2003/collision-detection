#include <iostream>
#include <fstream>
#include <string>
#include "GUI.h"
#include "constants.h"
#include "Engine.h"
#include "Body.h"

using namespace std;

GUI::GUI(){

	//Images surfaces used in GUI
	bodySheet = NULL;

	//Start-up the GUI!
	initGUI();

	//Exit criteria
	quit=false;

}

GUI::~GUI(){
	cleanUp();
}

int GUI::observeEvent(){
	return(SDL_PollEvent( &event ));
}

bool GUI::quitGame(){
	return(event.type == SDL_QUIT );
}


//Initialize SDL Components
void GUI::initGUI(){

	//Initialize all SDL subsystems
    SDL_Init( SDL_INIT_EVERYTHING );	//Initialize SDL subsystems

	//Construct Basic GUI components
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

	//Open Block Sprite Sheet
	bodySheet = loadImage("./images/ballNoBackground.gif", 255);

	//Set the window caption
    SDL_WM_SetCaption( "Capstone Physics Simulation", NULL );

}

void GUI::cleanUp(){

	//Free the graphics
	SDL_FreeSurface( bodySheet );
	SDL_FreeSurface( screen );
    
    //Quit SDL
    SDL_Quit();

}


SDL_Surface* GUI::loadImage(string filename, int mapkey ) 
{

	//The image that's loaded
    SDL_Surface* loadedImage = NULL;
    
    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;
    
    //Load the image
    loadedImage = IMG_Load( filename.c_str() );
    
    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized image
        optimizedImage = SDL_DisplayFormat( loadedImage );
        
        //Free the old image
        SDL_FreeSurface( loadedImage );


		//If the image was optimized just fine
        if( optimizedImage != NULL )
        {
            //Map the color key
            Uint32 colorkey = SDL_MapRGB( optimizedImage->format, mapkey, mapkey, mapkey );
 
			//Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );
        }
	}
	
	//Return the optimized image
    return optimizedImage;

}


void GUI::applySurface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}


void GUI::displayGameState(Engine & engine){

	//Fill the screen
    SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB( screen->format, 255, 255, 255) );

	//Blit all the Body instances to the Screen
	for(int i=4;i<engine.getNumObjects();i++){
		Body* body = engine.getObject(i);
		applySurface(body->getPosX(), body->getPosY(), bodySheet, screen, NULL);
	}

	//Update the visible screen
	SDL_Flip( screen );
	
}