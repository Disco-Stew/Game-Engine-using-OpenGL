#pragma once
#include "ParticleEmitter.h"

class ConeEmitter
{
private:
	GLfloat cosAngle, theta, x, y, z, rootOneMinusZSquared, rotateAngle;
	glm::vec4 tempdirection;
	glm::vec3 rotateAxis, direction;

public:

	ConeEmitter();
	~ConeEmitter();
	glm::vec3 generateVelocity();
	GLvoid setDirection(glm::vec3 direction);
};