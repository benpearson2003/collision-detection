#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include "Body.h"

using namespace std;

class Engine{

public:
	Engine(int);
	~Engine();
	void update(float,float);
	int getNumObjects();
	Body* getObject(int);

private:
	vector<Body*> objects;

};

float dist(Body& A, Body& B);
float diff(Body& A, Body& B);

bool collision(Body& A, Body& B);
bool collision(Body& A, float& normalX, float& normalY);
void resolveCollision(Body& A, float normalX, float normalY);
void resolveCollision(Body& A, Body& B);

#endif