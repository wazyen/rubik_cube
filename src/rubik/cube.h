#ifndef CUBE_H
#define CUBE_H

/*
#include "../utils.h"
#include "../camera.h"
#include <queue>
*/

#include "cubelet.h"

class Cube
{
public:
	std::vector<Cubelet*> cubelets;
	std::queue<Movement> movements_queue;

	Cube();

	void addMovement(Face_Color face, bool counter_clockwise);
	void render(Camera* camera);
	void update(double seconds_elapsed);
};

#endif