#include "cubelet.h"

#include "../mesh.h"
#include "../shader.h"
#include "../game.h"

#include <iostream>

const Vector3 Cubelet::face_vectors[6] = { Vector3(0.f, 1.f, 0.f), Vector3(0.f, 0.f, 1.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, -1.f, 0.f), Vector3(0.f, 0.f, -1.f), Vector3(-1.f, 0.f, 0.f) };

Cubelet::Cubelet(const unsigned short int _faces) : faces(_faces), goal_rotation(Vector3(0.f, 0.f, 0.f))
{
	position = Vector3(0.f, 0.f, 0.f);
	unsigned short int tmp_faces = _faces;
	for ( int i = 0; i < 6; i++ )
	{
		if (has_faces[i] = (tmp_faces % 2 == 1) )
			position = position + 110.f * face_vectors[i];
		tmp_faces /= 2;
	}
	model.translate( position.x, position.y, position.z);
};

// Function that returns an index from 0 to 5 for the face colors white, red, blue, yellow, orange, green; in that order
int Cubelet::getFaceIndex(Face_Color face)
{
	return (int)log2( (double)face );
};

void Cubelet::render(Camera* camera)
{
	Mesh* mesh = Mesh::Get("data/box.ASE");
	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");

	if (shader)
	{
		//render wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glLineWidth(3);

		shader->enable();

		shader->setUniform("u_color", Vector4(0.f, 0.f, 0.f, 1.f));
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		shader->setUniform("u_model", model);

		mesh->render(GL_POLYGON);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0, 1.0);
		glLineWidth(1);

		//disable shader
		shader->disable();

		//fill cubelets
		shader = Shader::Get("data/shaders/basic.vs", "data/shaders/rubik.fs");

		//enable shader
		shader->enable();

		//upload uniforms
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		shader->setUniform("u_model", model);
		shader->setUniform("u_eye", camera->eye);

		std::string uniform_tags[6] = {"u_white", "u_red", "u_blue", "u_yellow", "u_orange", "u_green"};
		unsigned short int tmp_faces = faces;
		for (int i = 0; i < 6; i++)
		{
			shader->setUniform(uniform_tags[i].c_str(), has_faces[i]);
		};

		//do the draw call
		mesh->render(GL_TRIANGLES);

		shader->disable();
	}
};

void Cubelet::update(double seconds_elapsed)
{
	if (goal_rotation.length() > 0.f)
	{
		Vector3 step = goal_rotation.length() < 0.1f ? goal_rotation : goal_rotation * 0.2f;
		goal_rotation = goal_rotation - step;

		Matrix44 rotation;
		if (step.x != 0.f )
			rotation.rotate(step.x, Vector3(1, 0, 0));
		else if (step.y != 0.f)
			rotation.rotate(step.y, Vector3(0, 1, 0));
		else if (step.z != 0.f)
			rotation.rotate(step.z, Vector3(0, 0, 1));

		model = model * rotation;
	}
};