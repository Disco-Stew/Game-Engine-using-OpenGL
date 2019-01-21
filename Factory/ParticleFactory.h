#pragma once
#include <String.h>
#include <vector>
#include "ShaderLoader.h"
#include "AlphaBlenderTool.h"
#include "ParticleTexture.h"
#include "ParticleEffect.h"
#include "Particle.h"

class ParticleFactory
{
private:

	ParticleEffect* particleEffect;
	GLboolean finished;
	GLint numStaticEffects, numMovingEffects;
	std::vector<ParticleEffect*> staticParticleEffects;
	std::vector<ParticleEffect*> movingParticleEffects;

public:
	ParticleFactory::ParticleFactory();
	ParticleFactory::~ParticleFactory();
	GLvoid update(GLfloat deltaTime, glm::vec3 cameraPosition, glm::mat4 view, glm::vec3 systemCentre);
	GLvoid draw(glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPosition);
	GLvoid addCollisionEffects(std::vector<glm::vec3> collisions);
	GLvoid addCollisionEffect(glm::vec3 location);
	GLvoid addMovingEffect(glm::vec3 location);
	GLvoid addFireEffect(glm::vec3 location);
	GLvoid addCustomEffect(GLint pps, glm::vec3 location, GLchar* texture, GLuint textureRows, GLint emitter, GLboolean infinite, GLfloat duration, GLfloat gravityEffect);
	
};