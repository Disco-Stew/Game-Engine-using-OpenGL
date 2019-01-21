#pragma once
#include "ParticleEmitter.h"

class SphereEmitter
{
private:

	GLfloat minRadius;
	GLfloat maxRadius;

	GLfloat minInclination;
	GLfloat maxInclination;

	GLfloat minAzimuth;
	GLfloat maxAzimuth;

	GLfloat minSpeed;
	GLfloat maxSpeed;

	GLfloat minLifetime;
	GLfloat maxLifetime;

	GLfloat inclination;
	GLfloat azimuth;
	GLfloat radius;
	GLfloat speed;
	GLfloat sInclination;
	GLfloat x;
	GLfloat y;
	GLfloat z;

public:

	SphereEmitter();
	~SphereEmitter();
	glm::vec3 generateVelocity();

	GLvoid setMinRadius(GLfloat minRadius);
	GLvoid setMaxRadius(GLfloat maxRadius);

	GLvoid setMinInclination(GLfloat minInclination);
	GLvoid setMaxInclination(GLfloat maxInclination);

	GLvoid setMinAzimuth(GLfloat minAzimuth);
	GLvoid setMaxAzimuth(GLfloat maxAzimuth);

	GLvoid setMinSpeed(GLfloat minSpeed);
	GLvoid setMaxSpeed(GLfloat maxSpeed);
};