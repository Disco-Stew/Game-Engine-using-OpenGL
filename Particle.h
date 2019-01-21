#pragma once
#include <glm/glm.hpp>

struct Particle {

	Particle()
		: pos(0)
		, velocity(0)
		, age(0)
	{}
	glm::vec3 pos, velocity;
	GLfloat size, angle, weight;
	GLfloat lifeLength, age; // Remaining life of the particle. if <0 : dead and unused.
	GLfloat cameradistance; // *Squared* distance to the camera. if dead : -1.0f
	glm::vec2 textureOffset1, textureOffset2; // Offsets on the texture atlas
	GLfloat blend; //how much to blend current and next texture
	bool operator<(const Particle& that) const {
		// Sort in reverse order : far particles drawn first.
		return this->cameradistance > that.cameradistance;
	}
};


