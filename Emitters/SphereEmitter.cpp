#include "SphereEmitter.h"

//code modelled off https://www.3dgep.com/simulating-particle-effects-using-opengl/

//set defaults for the emitter for ease of use, these can be changed at any time using the set methods
SphereEmitter::SphereEmitter()
	: minRadius(0)
	, maxRadius(1)
	, minInclination(0)
	, maxInclination(180)
	, minAzimuth(0)
	, maxAzimuth(360)
	, minSpeed(10)
	, maxSpeed(20)
	, minLifetime(1)
	, maxLifetime(3)
{}

SphereEmitter::~SphereEmitter()
{

}

glm::vec3 SphereEmitter::generateVelocity()
{
	inclination = glm::radians(randomRange(minInclination, maxInclination));
	azimuth = glm::radians(randomRange(minAzimuth, maxAzimuth));

	radius = randomRange(minRadius, maxRadius);
	speed = randomRange(minSpeed, maxSpeed);

	sInclination = sinf(inclination);

	x = sInclination * cosf(azimuth);
	y = sInclination * sinf(azimuth);
	z = cosf(inclination);

	return glm::vec3(x, y, z) * speed;
}

GLvoid SphereEmitter::setMinRadius(GLfloat minRadius) { this->minRadius = minRadius; }
GLvoid SphereEmitter::setMaxRadius(GLfloat newMaxRadius) { this->maxRadius = maxRadius; }

GLvoid SphereEmitter::setMinInclination(GLfloat minInclination) { this->minInclination = minInclination; }
GLvoid SphereEmitter::setMaxInclination(GLfloat maxInclination) { this->maxInclination = maxInclination; }

GLvoid SphereEmitter::setMinAzimuth(GLfloat minAzimuth) { this->minAzimuth = minAzimuth; }
GLvoid SphereEmitter::setMaxAzimuth(GLfloat maxAzimuth) { this->maxAzimuth = maxAzimuth; }

GLvoid SphereEmitter::setMinSpeed(GLfloat minSpeed) { this->minSpeed = minSpeed; }
GLvoid SphereEmitter::setMaxSpeed(GLfloat maxSpeed) { this->maxSpeed = maxSpeed; }