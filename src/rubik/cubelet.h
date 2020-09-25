#ifndef CUBELET_H
#define CUBELET_H

#include "../utils.h"
#include "../camera.h"
#include <queue>

enum Face_Color { WHITE = 1, RED = 2, BLUE = 4, YELLOW = 8, ORANGE = 16, GREEN = 32 };

typedef struct sMovement
{
	Face_Color face;
	bool is_counterclockwise;
} Movement;

class Cubelet
{
public:
	Matrix44 model;
	Vector3 position;
	Vector3 goal_rotation;
	unsigned short int faces;
	bool has_faces[6];

	static const Vector3 face_vectors[6];

	Cubelet(const unsigned short int _faces);

	static int getFaceIndex(Face_Color face);
	void render(Camera* camera);
	void update(double seconds_elapsed);
};

#endif