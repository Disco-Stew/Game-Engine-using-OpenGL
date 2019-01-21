#pragma once
#include <ctime>

#include "GameWorld.h"
#include "Billboard.h"
#include "Cube.h"
#include "ParticleEffect.h"
#include "ParticleTexture.h"
#include "SphereEmitter.h"
#include "ParticleFactory.h"

class ParticleEffectWorld : public GameWorld
{
private:
	Camera camera;
	SkyBox skyBox;
	ParticleEffect* particleEffect;
	ParticleFactory* particleFactory;
	ParticleTexture texture;
	GLboolean lines = false;

	GLuint size = 500;
	GLuint vertexCount = 4;

public:

	ParticleEffectWorld();
	~ParticleEffectWorld();


	GLvoid update(GLfloat deltaTime);
	GLvoid draw();
	GLvoid input(GLFWwindow* window, int key, int scancode, int action, int mode);
	GLvoid mouseButton(GLFWwindow* window, int button, int action, int mods);
	GLvoid mouseInput(GLFWwindow* window, double xPosition, double yPosition);
	GLvoid scrollInput(GLFWwindow* window, double xScroll, double yScroll);
};
