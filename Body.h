#ifndef BODY_H
#define BODY_H

using namespace std;

class Body{

public:
	Body();
	Body(Body* A);
	Body(bool,float,float);
	void update(float,float);
	float getPosX();
	float getPosY();
	//Other getters/setters here

	float getVelX();
	float getVelY();
	int getMass();
	float getRestitution();
	void setPosX(float x);
	void setPosY(float y);
	void setVelX(float x);
	void setVelY(float y);
	void setMass(int m);
	void setRestitution(float r);

	void setIsWall(bool w);
	bool isWall();

private:
	float posX, posY;
	//Other members here

	float velX, velY;
	int mass;
	float restitution;		//elasticity.

	bool wall;				//body should be treated as a barrier, not a free object.
};

#endif	
