#include <cstdlib>
#include "constants.h"
#include "Body.h"	

using namespace std;

Body::Body(){
	posX = rand()%(SCREEN_WIDTH-BODY_SPRITE_WIDTH)+BODY_SPRITE_WIDTH/2;
	posY = rand()%(SCREEN_HEIGHT-BODY_SPRITE_HEIGHT)+BODY_SPRITE_HEIGHT/2;
	velX = (rand()%2 * 2 - 1) * (rand()%5 + 1);
	velY = (rand()%2 * 2 - 1) * (rand()%5 + 1);
	mass = 1;
	restitution = 1.0f;
	wall = false;
}

Body::Body(Body* A){
	posX = A->getPosX();
	posY = A->getPosY();
	velX = A->getVelX();
	velY = A->getVelY();
	mass = A->getMass();
	restitution = A->getRestitution();
}

Body::Body(bool w, float x, float y){
	posX = x;
	posY = y;
	velX = 0;
	velY = 0;
	mass = 0;
	restitution = 1;
	wall = w;
}

int Body::getMass(){
	return(mass);
}

void Body::setMass(int m){
	mass = m;
}

float Body::getRestitution(){
	return(restitution);
}

void Body::setRestitution(float r){
	restitution = r;
}

float Body::getPosX(){
	return(posX);
}

float Body::getPosY(){
	return(posY);
}

float Body::getVelX(){
	return(velX);
}

float Body::getVelY(){
	return(velY);
}

void Body::setPosX(float x){
	posX = x;
}

void Body::setPosY(float y){
	posY = y;
}

void Body::setVelX(float x){
	velX = x;
}

void Body::setVelY(float y){
	velY = y;
}

void Body::setIsWall(bool w){
	wall = w;
}

bool Body::isWall(){
	return wall;
}

struct State{
	float x;
	float y;
	float v_x;
	float v_y;
};

struct Derivative{
	float dx;
	float dy;
	float dv_x;
	float dv_y;
};

void acceleration(State &state, float t, float &dv_x, float &dv_y){		//If acceleration is added, it can fit here.
	dv_x = 0;	
	dv_y = 0;	
}

Derivative evaluate(const State &initial, float t, float dt, const Derivative &d){
	State state;

	state.x = initial.x + d.dx * dt;			//d/dt s(t) = (s(t)-s(t-1))/dt
	state.y = initial.y + d.dy * dt;
	state.v_x = initial.v_x + d.dv_x * dt;
	state.v_y = initial.v_y + d.dv_y * dt;

	Derivative output;
	output.dx = state.v_x;
	output.dy = state.v_y;
	acceleration(state,t+dt,output.dv_x,output.dv_y);	//Though acceleration isn't actually used, the program can support it.

	return output;
}

void integrate(State &state, float t, float dt){
	Derivative a = evaluate(state,t,0.0f,Derivative());					//k1 = v(t)
	Derivative b = evaluate(state,t,dt*0.5f,a);						//k2 = v@s(t) + (dt/2)*k1
	Derivative c = evaluate(state,t,dt*0.5f,b);						//k3 = v@s(t) + (dt/2)*k2
	Derivative d = evaluate(state,t,dt,c);							//k4 = v@s(t) + (dt)*k3

	const float dxdt = 1.0f/6.0f * (a.dx + 2.0f * (b.dx + c.dx) + d.dx);			//d/dt s(t) = (k1 + 2*k2 + 2*k3 + k4)/6
	const float dydt = 1.0f/6.0f * (a.dy + 2.0f * (b.dy + c.dy) + d.dy);

	const float dv_xdt = 1.0f/6.0f * (a.dv_x + 2.0f * (b.dv_x + c.dv_x) + d.dv_x);	//RK4 is simultaneously being applied to velocity;
	const float dv_ydt = 1.0f/6.0f * (a.dv_y + 2.0f * (b.dv_y + c.dv_y) + d.dv_y);	//though unnecessary a.t.m., it seems useful in later applications.

	state.x = state.x + dxdt * dt;
	state.y = state.y + dydt * dt;

	state.v_x = state.v_x + dv_xdt * dt;	//The velocity is not likely to change here due to acceleration being absent,
	state.v_y = state.v_y + dv_ydt * dt;	//but this will be available if acceleration is added.
}

void Body::update(float t, float dt){
	if(!wall)
	{
		State state = {posX,posY,velX,velY};		//structs are being used to keep the code readable
		integrate(state,t,dt);				//RK4 begins here.
		posX=state.x;
		posY=state.y;
		velX=state.v_x;
		velY=state.v_y;
	}
}