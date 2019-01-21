#include "ConeEmitter.h"

ConeEmitter::ConeEmitter()
	//default direction UP
	:direction(glm::vec3(0.0f, 1.0f, 0.0))
{}

ConeEmitter::~ConeEmitter()
{}

glm::vec3 ConeEmitter::generateVelocity() 
{
	cosAngle = (GLfloat)glm::cos(0.4f * M_PI);
	theta = (GLfloat)(randomFloat() * 2.0f * M_PI);
	z = cosAngle + (randomFloat() * (1 - cosAngle));
	rootOneMinusZSquared = (GLfloat)glm::sqrt(1 - z * z);
	x = (GLfloat)(rootOneMinusZSquared * glm::cos(theta));
	y = (GLfloat)(rootOneMinusZSquared * glm::sin(theta));
	y *= 2;
	tempdirection = glm::vec4(x, y, z, 1);
	if (direction.x != 0 || direction.y != 0 || (direction.z != 1 && direction.z != -1)) {
		rotateAxis = glm::cross(direction, glm::vec3(0, 0, 1));
		glm::normalize(rotateAxis);
		rotateAngle = (GLfloat)glm::acos(glm::dot(direction, glm::vec3(0, 0, 1)));
		//create new matrix every time to reset
		glm::mat4 rotationMatrix;
		glm::rotate(rotationMatrix, -rotateAngle, rotateAxis);
		tempdirection = rotationMatrix * tempdirection;
	}
	else if (direction.z == -1) {
		tempdirection.z *= -1;
	}
	return glm::vec3(tempdirection);
}

GLvoid ConeEmitter::setDirection(glm::vec3 direction) { this->direction = direction; }