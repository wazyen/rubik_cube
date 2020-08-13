#include "piece.h"

#include "../mesh.h"
#include "../shader.h"
#include "../game.h"

#include <iostream>

Piece::Piece(const unsigned short int _faces) : faces(_faces), position(Vector3(0.f, 0.f, 0.f)), rotation(Vector3(0.f, 0.f, 0.f)), goal_rotation(Vector3(0.f, 0.f, 0.f))
{
};

void Piece::reduce_rotation()
{
	while (rotation.x < 0)
		rotation.x += 2 * PI;
	while (rotation.x >= 2 * PI)
		rotation.x -= 2 * PI;

	while (rotation.y < 0)
		rotation.y += 2 * PI;
	while (rotation.y >= 2 * PI)
		rotation.y -= 2 * PI;

	while (rotation.z < 0)
		rotation.z += 2 * PI;
	while (rotation.z >= 2 * PI)
		rotation.z -= 2 * PI;
};

void Piece::rotate(Vector3 rotation_angle)
{
	goal_rotation = goal_rotation + rotation_angle;
};

void Piece::move(Face_Color face)
{
	Vector3 rotation_angles[6] = { Vector3(0,PI/2,0), Vector3(0,0,PI/2), Vector3(PI/2,0,0), Vector3(0,-PI/2,0), Vector3(0,0,-PI/2), Vector3(-PI/2,0,0) };
	this->rotate(rotation_angles[(int)log2((double)face)]);
};

void Piece::addMovement(Face_Color face)
{
	if ( movements_queue.size() < 10 )
		this->movements_queue.push(face);
};

void Piece::render(Camera* camera)
{

	Mesh* mesh = Mesh::Get("data/box.ASE");
	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/rubik.fs");

	Matrix44 model;
	//rotation.x = Game::instance->time;
	model.translate(position.x, position.y, position.z);
	model.rotate(rotation.x, Vector3(1, 0, 0));
	model.rotate(rotation.y, Vector3(0, 1, 0));
	model.rotate(rotation.z, Vector3(0, 0, 1));

	if (shader)
	{
		//enable shader
		shader->enable();

		//upload uniforms
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		shader->setUniform("u_model", model);
		shader->setUniform("u_rotation", rotation);

		std::string uniform_tags[6] = {"u_white", "u_red", "u_blue", "u_yellow", "u_orange", "u_green"};
		unsigned short int tmp_faces = faces;
		for (int i = 0; i < 6; i++)
		{
			shader->setUniform(uniform_tags[i].c_str(), tmp_faces % 2 == 1);
			tmp_faces /= 2;
		};

		//do the draw call
		mesh->render(GL_TRIANGLES);

		//disable shader
		shader->disable();
	}
};

void Piece::update(double seconds_elapsed)
{
	if (goal_rotation.length() > 0.f)
	{
		Vector3 step = goal_rotation.length() < 0.1f ? goal_rotation : goal_rotation * 0.2f;
		rotation = rotation + step;
		goal_rotation = goal_rotation - step;
	}
	else if (!movements_queue.empty())
	{
		Face_Color face = movements_queue.front();
		movements_queue.pop();
		this->move(face);
	}
};