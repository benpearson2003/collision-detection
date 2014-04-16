The major change between the third and fourth draft is that the four walls
have been added to the list of objects in the simulation and
the collision resolution for walls & body and body & body are accomplished with the same function.

The Capstone project assignment has four general requirements:
1. Body class that has all the physical properties expected from a body class.
2. A method Body::update() that performs numerical integration using RK4.
3. A collision detection and resolution system within the main simulation loop that
	a. generates all collisions within dt
	b. finds collision time using a binary search
	c. resolves collision using conservation of momentum
	d. accounts for screen boundaries.
4. Engine that constructs all the bodies and updates them.




In this version, requirements 1, 2, and 4 have been fulfilled.
Requirement 3 is missing these crucial components:
	1. Collision detection does not occur strictly within the main simulation loop, i.e. main calls engine.update()
	2. Exact time of collision is in development. The results are disastrous. 