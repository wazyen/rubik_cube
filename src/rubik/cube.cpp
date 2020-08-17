#include "cube.h"

#include <iostream>

Cube::Cube()
{
	// Center cubelets
	cubelets.push_back( new Cubelet( WHITE ) );
	cubelets.push_back( new Cubelet( RED ) );
	cubelets.push_back( new Cubelet( BLUE ) );
	cubelets.push_back( new Cubelet( YELLOW ) );
	cubelets.push_back( new Cubelet( ORANGE ) );
	cubelets.push_back( new Cubelet( GREEN ) );
	// Edge cubelets
	cubelets.push_back( new Cubelet( WHITE | RED ) );
	cubelets.push_back( new Cubelet( WHITE | BLUE ) );
	cubelets.push_back( new Cubelet( WHITE | ORANGE ) );
	cubelets.push_back( new Cubelet( WHITE | GREEN ) );
	cubelets.push_back( new Cubelet( YELLOW | RED ) );
	cubelets.push_back( new Cubelet( YELLOW | BLUE ) );
	cubelets.push_back( new Cubelet( YELLOW | ORANGE ) );
	cubelets.push_back( new Cubelet( YELLOW | GREEN ) );
	cubelets.push_back( new Cubelet( RED | BLUE ) );
	cubelets.push_back( new Cubelet( RED | GREEN ) );
	cubelets.push_back( new Cubelet( ORANGE | BLUE ) );
	cubelets.push_back( new Cubelet( ORANGE | GREEN ) );
	// Corner cubelets
	cubelets.push_back( new Cubelet( WHITE | RED | BLUE ) );
	cubelets.push_back( new Cubelet( WHITE | RED | GREEN) );
	cubelets.push_back( new Cubelet( WHITE | ORANGE | BLUE ) );
	cubelets.push_back( new Cubelet( WHITE | ORANGE | GREEN) );
	cubelets.push_back( new Cubelet( YELLOW | RED | BLUE ) );
	cubelets.push_back( new Cubelet( YELLOW | RED | GREEN) );
	cubelets.push_back( new Cubelet( YELLOW | ORANGE | BLUE ) );
	cubelets.push_back( new Cubelet( YELLOW | ORANGE | GREEN) );
};

void Cube::addMovement(Face_Color face, bool counter_clockwise)
{
	Movement movement;
	movement.face = face;
	movement.counter_clockwise = counter_clockwise;
	if ( movements_queue.size() < 10 )
		this->movements_queue.push(movement);
};

void Cube::render(Camera* camera)
{
	for (int i = 0; i < cubelets.size(); i++)
	{
		Cubelet* cubelet = cubelets[i];
		cubelet->render(camera);
	}
};

void Cube::update(double seconds_elapsed)
{
	bool isMoving = false;
	for (int i = 0; i < cubelets.size(); i++)
	{
		Cubelet* cubelet = cubelets[i];
		cubelet->update(seconds_elapsed);
		if ( cubelet->goal_rotation.length() > 0.f )
			isMoving = true;
	}

	if ( !movements_queue.empty() && !isMoving )
	{
		Movement movement = movements_queue.front();
		movements_queue.pop();
		for (int i = 0; i < cubelets.size(); i++)
		{
			Cubelet* cubelet = cubelets[i];
			if (movement.face == BLUE && cubelet->model.getTranslation().x > 100.f ||
				movement.face == GREEN && cubelet->model.getTranslation().x < -100.f ||
				movement.face == WHITE && cubelet->model.getTranslation().y > 100.f ||
				movement.face == YELLOW && cubelet->model.getTranslation().y < -100.f ||
				movement.face == RED && cubelet->model.getTranslation().z > 100.f ||
				movement.face == ORANGE && cubelet->model.getTranslation().z < -100.f)
			{
				cubelet->goal_rotation = (movement.counter_clockwise ? -1 : 1) * (PI / 2.f) * Cubelet::face_vectors[Cubelet::getFaceIndex(movement.face)];
			}
		}
	}
};