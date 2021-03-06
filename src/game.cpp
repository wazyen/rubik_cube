#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"

#include "rubik/includes.h"

#include <cmath>

//some globals
float mouse_speed = 100.0f;
Cube* cube = new Cube();

Game* Game::instance = NULL;

Game::Game(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;
	mouse_locked = false;

	//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	//create our camera
	camera = new Camera();
	camera->lookAt(Vector3(200.f, 300.f, 300.f),Vector3(0.f,0.f,0.f), Vector3(0.f,1.f,0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f,window_width/(float)window_height,0.1f,10000.f); //set the projection, we want to be perspective

	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse

	cube = new Cube();
}

//what to do when the image has to be draw
void Game::render(void)
{
	//set the clear color (the background color)
	glClearColor(0.7, 0.7, 1.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera as default
	camera->enable();

	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	//render skybox
	glDisable(GL_DEPTH_TEST);

	Mesh* mesh = Mesh::Get("data/sphere.obj");
	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	Texture* texture = Texture::Get("data/skysphere.tga");

	shader->enable();

	Matrix44 model;
	model.translate( camera->eye.x, camera->eye.y, camera->eye.z );
	
	shader->setUniform("u_model", model);
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_color", Vector4(1.f,1.f,1.f,1.f));
	shader->setUniform("u_texture", texture, 0);

	mesh->render(GL_TRIANGLES);

	shader->disable();

	glEnable(GL_DEPTH_TEST);

	//render light source
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
	shader->enable();

	model.setIdentity();
	model.translate(-300,300,-300);
	model.scale(10,10,10);

	shader->setUniform("u_model", model);
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_color", Vector4(1.f, 1.f, 1.f, 1.f));

	mesh->render(GL_TRIANGLES);

	shader->disable();

	//render our rubik cube
	cube->render( camera );

	//Draw the floor grid
	//drawGrid();

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{
	//mouse input to rotate the cam
	if ((Input::mouse_state & SDL_BUTTON_LEFT) || mouse_locked ) //is left button pressed?
	{
		camera->rotate(Input::mouse_delta.x * 0.005f, camera->getLocalVector( Vector3(0.0f,-1.0f,0.0f)));
		camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector( Vector3(-1.0f,0.0f,0.0f)));

		camera->eye = camera->eye - camera->center;
		camera->center = Vector3(0.f,0.f,0.f);
	}

	cube->update( seconds_elapsed );

	//to navigate with the mouse fixed in the middle
	if (mouse_locked)
		Input::centerMouse();
}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	if (event.repeat > 0)
		return;

	bool is_counterclockwise = Input::isKeyPressed(SDL_SCANCODE_LSHIFT);
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
		case SDLK_F1: Shader::ReloadAll(); break;
		case SDLK_b: cube->addMovement(BLUE, is_counterclockwise); break;
		case SDLK_g: cube->addMovement(GREEN, is_counterclockwise); break;
		case SDLK_w: cube->addMovement(WHITE, is_counterclockwise); break;
		case SDLK_y: cube->addMovement(YELLOW, is_counterclockwise); break;
		case SDLK_r: cube->addMovement(RED, is_counterclockwise); break;
		case SDLK_o: cube->addMovement(ORANGE, is_counterclockwise); break;
	}
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{
}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
	float speed = 30.f;
	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10.f; //move faster with left shift
	camera->move(Vector3(0.0f, 0.0f, speed*event.y));
	camera->center = Vector3(0.f,0.f,0.f);
}

void Game::onResize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport( 0,0, width, height );
	camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}

