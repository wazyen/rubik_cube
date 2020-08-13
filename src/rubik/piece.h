#ifndef PIECE_H
#define PIECE_H

#include "../utils.h"
#include "../camera.h"
#include <queue>

enum Face_Color { WHITE = 1, RED = 2, BLUE = 4, YELLOW = 8, ORANGE = 16, GREEN = 32 };

class Piece
{
public:
	Vector3 position;
	Vector3 rotation;
	Vector3 goal_rotation;
	unsigned short int faces;
	std::queue<Face_Color> movements_queue;

	Piece(const unsigned short int _faces);

	void reduce_rotation();
	void rotate(Vector3 rotation_angle);
	void move(Face_Color face);
	void addMovement(Face_Color face);
	void render(Camera* camera);
	void update(double seconds_elapsed);
};

#endif