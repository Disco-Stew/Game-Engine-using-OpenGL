#include "ParticleEffectWorld.h"


ParticleEffectWorld::ParticleEffectWorld()
{
	skyBox.setTexture("skybox/right.png", "skybox/left.png", "skybox/top.png", "skybox/bottom.png", "skybox/back.png", "skybox/front.png");
	glm::vec3 cameraPos = camera.getCameraPosition();
	glm::mat4 m[2];
	particleFactory = new ParticleFactory();
	particleFactory->addFireEffect(glm::vec3(0, 0, -200));

	srand(static_cast <unsigned> (time(0)));
}

ParticleEffectWorld::~ParticleEffectWorld()
{
	delete(particleEffect);
}

void ParticleEffectWorld::update(GLfloat deltaTime)
{
	camera.move(deltaTime);
	glm::mat4 view = camera.getView();
	glm::vec3 cameraPos = camera.getCameraPosition();
	particleFactory->update(deltaTime, cameraPos, view, glm::vec3(0,0,0));
	
}

void ParticleEffectWorld::draw()
{
	glm::mat4 view = camera.getView();
	glm::mat4 projection = camera.getProjection();
	glm::vec3 cameraPos = camera.getCameraPosition();

	glDepthFunc(GL_LEQUAL);
	skyBox.draw(view, projection);

	glDepthFunc(GL_LESS);
	particleFactory->draw(view, projection, cameraPos);
}

GLvoid ParticleEffectWorld::input(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_L && action == GLFW_RELEASE)
	{
		lines = !lines;
		if (lines)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (action == GLFW_PRESS)
	{
		camera.setKey(key, true);
	}

	if (action == GLFW_RELEASE)
	{
		camera.setKey(key, false);
	}

	if (key == GLFW_KEY_UP)
	{
		camera.addCameraSpeed(100);
	}

	if (key == GLFW_KEY_DOWN)
	{
		camera.addCameraSpeed(-100);
	}
}

GLvoid ParticleEffectWorld::mouseInput(GLFWwindow* window, double xPosition, double yPosition)
{
	camera.updateFront(xPosition, yPosition);
}

GLvoid ParticleEffectWorld::scrollInput(GLFWwindow* window, double xPosition, double yPosition)
{
}

GLvoid ParticleEffectWorld::mouseButton(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		camera.setKey(button, true);
	}

	if (action == GLFW_RELEASE)
	{
		camera.setKey(button, false);
	}
}