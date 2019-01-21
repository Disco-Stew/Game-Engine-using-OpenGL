#include "ParticleFactory.h"

ParticleFactory::ParticleFactory() 
{
	numStaticEffects = 0;
	numMovingEffects = 0;
}

ParticleFactory::~ParticleFactory() 
{
	delete(&staticParticleEffects);
	delete(&movingParticleEffects);

}
GLvoid ParticleFactory::addFireEffect(glm::vec3 location) {
	particleEffect = new ParticleEffect(1000, location, "Texture/particleAtlas.png", 4, 0, true, 0, 0.0);
	staticParticleEffects.push_back(particleEffect);
	numStaticEffects++;

}

GLvoid ParticleFactory::addCollisionEffects(std::vector<glm::vec3> collisions) {
	for (GLuint i = 0; i < collisions.size(); i++) {
		particleEffect = new ParticleEffect(50, collisions.at(i), "Texture/dustAtlas.png", 4, 0, false, 1, 0.3);
		staticParticleEffects.push_back(particleEffect);
		numStaticEffects++;
	}
}

GLvoid ParticleFactory::addCollisionEffect(glm::vec3 location)
{
	particleEffect = new ParticleEffect(50, location, "Texture/dustAtlas.png", 4, 0, false, 3, 0.3);
	staticParticleEffects.push_back(particleEffect);
	numStaticEffects++;
}

GLvoid ParticleFactory::addMovingEffect(glm::vec3 location) {
	particleEffect = new ParticleEffect(400, location, "Texture/blueParticle.png", 4, 0, true, 0, 1);
	movingParticleEffects.push_back(particleEffect);
	numMovingEffects++;
}

GLvoid ParticleFactory::addCustomEffect(GLint pps, glm::vec3 location, GLchar* texture, GLuint textureRows, GLint emitter, GLboolean infinite, GLfloat duration, GLfloat gravityEffect)
{
	particleEffect = new ParticleEffect(pps, location, texture, textureRows, emitter, infinite, duration, gravityEffect);
	staticParticleEffects.push_back(particleEffect);
	numStaticEffects++;
}

GLvoid ParticleFactory::update(GLfloat deltaTime, glm::vec3 cameraPosition, glm::mat4 view, glm::vec3 systemCentre)
{
	for (GLuint i = 0; i < numStaticEffects; i++)
	{
		staticParticleEffects.at(i)->update(deltaTime, cameraPosition, view);
		if (staticParticleEffects.at(i)->isFinished()) {
			staticParticleEffects.erase(staticParticleEffects.begin() + i);
			numStaticEffects--;
		}
	}
	for (GLuint i = 0; i < numMovingEffects; i++)
	{ 
		movingParticleEffects.at(i)->update(deltaTime, cameraPosition, view, systemCentre);
		if (movingParticleEffects.at(i)->isFinished()) {
			movingParticleEffects.erase(movingParticleEffects.begin() + i);
			numMovingEffects--;
		}
	}
}

GLvoid ParticleFactory::draw(glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPosition) 
{
	for (GLuint i = 0; i < numStaticEffects; i++)
	{	
		staticParticleEffects.at(i)->draw(view, projection, cameraPosition);
	}
	for (GLuint i = 0; i < numMovingEffects; i++)
	{	
		movingParticleEffects.at(i)->draw(view, projection, cameraPosition);
	}
}