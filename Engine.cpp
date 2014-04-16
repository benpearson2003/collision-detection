#include <fstream>
#include <string>
#include <iostream>
#include "Engine.h"
#include "constants.h"

using namespace std;

Engine::Engine(int numBodies){
	float impulseX = 0.0f,impulseY = 0.0f;
	objects.push_back(new Body(true, 1.0, 0.0));					//top barrier
	objects.push_back(new Body(true, 1.0, (float)SCREEN_HEIGHT));	//bottom
	objects.push_back(new Body(true, 0.0, 1.0));					//left
	objects.push_back(new Body(true, (float)SCREEN_WIDTH, 1.0));	//right
	for(int i=4;i<numBodies+4;i++){									//starts at i=4 because first four objects are walls.
		objects.push_back(new Body());
		for(unsigned int a = 4; a < objects.size(); a++)
			if(a != i)
				if(collision(*objects[a],*objects[i])){				//This check is used to prevent objects from
					objects.pop_back();								//starting in a state of collision.
					i--;
				}
	}
}

Engine::~Engine(){

	for(unsigned int i=0;i<objects.size();i++){
		delete objects[i];
	}
}

void Engine::update(float t, float dt){
	Body* A = objects[0];	//Placeholder to avoid hanging pointers.
	Body* B = A;
	float collisionTime = dt;
	float min = 0.0, max = dt, mid = 0.0, difference = 0.0; //Used for binary search.

	for(unsigned int i=0;i<objects.size();i++){
		objects[i]->update(t,dt);
	}

	/*while(dt >= 0){*/
	for(unsigned int a = 0;a < objects.size();a++)
	{
		for(unsigned int b = a+1;b < objects.size();b++)
		{
			if(collision(*objects[a],*objects[b]))
			{
				for(unsigned int i=0;i<objects.size();i++){
					objects[i]->update(t,-dt);
				}
				resolveCollision(*objects[a],*objects[b]);
				for(unsigned int i=0;i<objects.size();i++){
					objects[i]->update(t,dt);
				}
				/*while(max>min){
				mid = (max+min)/2;
				for(unsigned int i=0;i<objects.size();i++){
				objects[i]->update(t,mid);
				}
				difference = diff(*objects[a],*objects[b]);
				if(difference < 0)
				min = mid;
				else if(difference > 0)
				max = mid;
				if(abs(difference) <= 1)
				max = min - 1;

				for(unsigned int i=0;i<objects.size();i++){
				objects[i]->update(t,-mid);
				}
				}
				for(unsigned int i=0;i<objects.size();i++){
				objects[i]->update(t,mid);
				}
				if(mid < collisionTime){
				collisionTime = mid;
				A = objects[a];
				B = objects[b];
				}*/
			}
		}
	}
	/*if(A != B){
	resolveCollision(*A,*B);
	}
	if(collisionTime !=0)
	dt -= collisionTime;
	else
	dt = -1;
	}*/

	for(unsigned int a = 0;a < objects.size();a++)				//Simply checks if any collisions were missed.
		for(unsigned int b = a+1;b < objects.size();b++)		//If missed collisions, then algorithm is insufficient.
			if(collision(*objects[a],*objects[b]) && a != b)
			{
				cout << "undetected collision" << endl;
				system("PAUSE");
			}
}

bool collision(Body& A, Body& B){
	float aX = A.getPosX(); float aY = A.getPosY();
	float bX = B.getPosX(); float bY = B.getPosY();
	bool aWall = A.isWall(); bool bWall = B.isWall();

	if(aWall && bWall){			//if A and B are both walls, they are obviously not colliding.
		return false;
	}

	else if(aWall){				//if A is a wall, then find collision for B.
		if(aX == 1.0)			//aX == 1.0 indicates a barrier at either the TOP or BOTTOM of the screen.
		{
			if(aY == 0.0)
			{
				if(bY - BODY_SPRITE_WIDTH/2 < aY)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else if(aY == SCREEN_HEIGHT)
			{
				if(bY + BODY_SPRITE_HEIGHT/2 > SCREEN_HEIGHT)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
		else if(aY == 1.0)			//aY == 1.0 indicates a barrier at either the LEFT or RIGHT of the screen.
		{
			if(aX == 0.0)
			{
				if(bX - BODY_SPRITE_WIDTH/2 < aX)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else if(aX == SCREEN_WIDTH)
			{
				if(bX + BODY_SPRITE_WIDTH/2 > aX)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
		else
		{
			cout << "Neither X or Y of the wall position returned '1.0'." << endl;
			cout << "This indicates either 1) a moving object has been declared a wall" << endl;
			cout << "or 2) (x,y) of the wall was incorrectly declared or changed." << endl;
			system("PAUSE");
			return false;
		}
	}						
	
	//if A and B are mobile objects, determine if there is collision between them.
	else if(((BODY_SPRITE_WIDTH/2 + BODY_SPRITE_WIDTH/2)*(BODY_SPRITE_WIDTH/2 + BODY_SPRITE_WIDTH/2)) 
		> ((aX-bX)*(aX-bX) + (aY-bY)*(aY-bY)))
	{
		return true;
	}

	return false;
}


int Engine::getNumObjects(){
	return(objects.size());
}

Body* Engine::getObject(int i){
	return(objects[i]);
}

float diff(Body& A, Body& B){
	if(A.isWall() || B.isWall())
		return BODY_SPRITE_WIDTH/2 - dist(A,B);
	else
		return BODY_SPRITE_WIDTH - dist(A,B);
}

float dist(Body& A, Body& B){
	bool aWall = A.isWall(); bool bWall = B.isWall();
	float aX = A.getPosX(); float aY = A.getPosY();	//positions of objects
	float bX = B.getPosX(); float bY = B.getPosY();

	if(aWall && bWall){
		cout << "Attempted to find distance between two walls. Check use of dist() function." << endl;
		system("PAUSE");
		return 0;
	}

	else if(aWall){
		if(aX == 1.0)			//A value of 1.0 for X or Y indicates that, respectively, Y or X are to be used to
		{						//find collision with a wall.
			return abs(aY - bY);
		}
		else if(aY == 1.0)
		{
			return abs(aX - bX);
		}
		else
		{
			cout << "Neither X or Y of the wall position returned '1.0'." << endl;
			cout << "This indicates either 1) a moving object has been declared a wall" << endl;
			cout << "or 2) (x,y) of the wall was incorrectly declared or changed." << endl;
			system("PAUSE");
			return 0;
		}
	}

	else if(bWall){
		if(bX == 1.0)			//A value of 1.0 for X or Y indicates that, respectively, Y or X are to be used to
		{						//find collision with a wall.
			return abs(bY - aY);
		}
		else if(bY == 1.0)
		{
			return abs(bX - aX);
		}
		else
		{
			cout << "Neither X or Y of the wall position returned '1.0'." << endl;
			cout << "This indicates either 1) a moving object has been declared a wall" << endl;
			cout << "or 2) (x,y) of the wall was incorrectly declared or changed." << endl;
			system("PAUSE");
			return 0;
		}
	}

	else{
		return sqrt((aX-bX)*(aX-bX)+(aY-bY)*(aY-bY));
	}
}

void resolveCollision(Body& A, float impulseX, float impulseY){		//resolves collision with wall

	float aV_X = A.getVelX(); float aV_Y = A.getVelY();	//components of velocity vector

	int aMass = A.getMass();

	A.setVelX(aV_X + impulseX / aMass);	//via conservation of momentum
	A.setVelY(aV_Y + impulseY / aMass);
}

void resolveCollision(Body& A,Body& B){		//resolves collision with wall
	float impulseX = 0.0, impulseY = 0.0;
	float aX = A.getPosX(); float aY = A.getPosY();
	float bX = B.getPosX(); float bY = B.getPosY();

	float aV_X = A.getVelX(); float aV_Y = A.getVelY();	//components of velocity vector
	float bV_X = B.getVelX(); float bV_Y = B.getVelY();

	int aMass = A.getMass(); int bMass = B.getMass();
	float e = min(A.getRestitution(),B.getRestitution());

	float velAlongNormal = 0.0;
	float massProduct_e = 0.0;

	if(A.isWall()){
		if(aX == 1.0)			
		{
			if(aY == 0.0)
			{
				velAlongNormal = (bV_X) * 0 + (bV_Y) * 1;
				massProduct_e = bMass * (1 + e);

				impulseX = -(massProduct_e * velAlongNormal)/(bMass) * 0;
				impulseY = -(massProduct_e * velAlongNormal)/(bMass) * 1;
			}

			if(aY == SCREEN_HEIGHT)
			{
				velAlongNormal = (bV_X) * 0 + (bV_Y) * -1;
				massProduct_e = bMass * (1 + e);

				impulseX = -(massProduct_e * velAlongNormal)/(bMass) * 0;
				impulseY = -(massProduct_e * velAlongNormal)/(bMass) * -1;
			}
		}
		else if(aY == 1.0)
		{
			if(aX == 0.0)
			{
				velAlongNormal = (bV_X) * 1 + (bV_Y) * 0;
				massProduct_e = bMass * (1 + e);

				impulseX = -(massProduct_e * velAlongNormal)/(bMass) * 1;
				impulseY = -(massProduct_e * velAlongNormal)/(bMass) * 0;
			}

			if(aX == SCREEN_WIDTH)
			{
				velAlongNormal = (bV_X) * -1 + (bV_Y) * 0;
				massProduct_e = bMass * (1 + e);

				impulseX = -(massProduct_e * velAlongNormal)/(bMass) * -1;
				impulseY = -(massProduct_e * velAlongNormal)/(bMass) * 0;
			}
		}
		resolveCollision(B, impulseX, impulseY);
	}

	else{
		float distance = dist(A,B);
		float normalX = (aX-bX)/distance; 
		float normalY = (aY-bY)/distance;

		velAlongNormal = (aV_X - bV_X) * normalX + (aV_Y - bV_Y) * normalY;

		float impulseScalar =  -(aMass*bMass * (1 + e) * velAlongNormal)/(aMass + bMass);

		impulseX = impulseScalar * normalX;
		impulseY = impulseScalar * normalY;

		A.setVelX(aV_X + impulseX / aMass);	//via conservation of momentum
		A.setVelY(aV_Y + impulseY / aMass);
		B.setVelX(bV_X - impulseX / bMass);
		B.setVelY(bV_Y - impulseY / bMass);
	}
}