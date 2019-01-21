#include "ParticleEffect.h"
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <glm/glm.hpp>
#include "Particle.h"
#include "InstancedModel.h"

//Max number of particles *per effect*
const GLint MAX_INSTANCES = 10000;
const GLint INSTANCE_DATA_LENGTH = 8;
InstancedObject instancingMethods = InstancedObject();

Particle ParticlesContainer[MAX_INSTANCES];
Particle& p = ParticlesContainer[0];
ParticleEffect::ParticleEffect(GLint pps, glm::vec3 systemCentre, GLchar* texture, GLuint numberOfTextureRows, GLint emitter, GLboolean infinite, GLfloat duration, GLfloat gravityEffect) 
{ 	
	//particles to generate every second
	this->pps = pps;
	//choice of emitter; sphere = 0, cone = 1;
	this->emitter = emitter;
	this->systemCentre = systemCentre;
	this->infinite = infinite;
	this->duration = duration;
	this->gravityEffect = gravityEffect;
	this->sphereEmitter = new SphereEmitter();
	this->coneEmitter = new ConeEmitter();
	particleTexture.numberOfRows = numberOfTextureRows;
	setUp(texture);

}
ParticleEffect::ParticleEffect(GLint pps, glm::vec3 systemCentre) 
{
	this->pps = pps;

	//choice of emitter; sphere = 0, cone = 1;
	this->emitter = 0;

	this->systemCentre = systemCentre;
	this->infinite = true;
	this->duration = 0;
	this->gravityEffect = 1.0;
	particleTexture.numberOfRows = 4;
	setUp("Texture/blueParticle.png");
}

ParticleEffect::~ParticleEffect()
{
	delete[] positionBlendData;
	delete[] textureData;
	delete[] ParticlesContainer;
}

GLvoid ParticleEffect::setUp(GLchar* texture) 
{
	gravity = glm::vec3(0.0f, -9.81f, 0.0f);
	this->lastUsedParticle = 0;
	shaderProgram = &ShaderLoader::particleShader;
	directionDeviation = 0.2f * M_PI;
	getUniformLocations();

	positionBlendData = new GLfloat[MAX_INSTANCES * 4];
	textureData = new GLfloat[MAX_INSTANCES * 4];

	for (GLint i = 0; i<MAX_INSTANCES; i++)
	{
		ParticlesContainer[i].age = 0.0f;
		ParticlesContainer[i].cameradistance = -1.0f;
		ParticlesContainer[i].lifeLength = 0.0f;
	}
	helper.loadTextureWithImage2D(particleTexture.textureID, texture, 0);
	
	GLfloat vertices[] = 
	{
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
		0.5f,  -0.5f, 0.0f,
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Initialize with empty buffers
	glGenBuffers(1, &particleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_INSTANCES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	glGenBuffers(1, &textureVBO);
	glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_INSTANCES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
}

GLvoid ParticleEffect::update(GLfloat deltaTime, glm::vec3 cameraPosition, glm::mat4 view, glm::vec3 systemCentre) 
{
	this->systemCentre = systemCentre;
	update(deltaTime, cameraPosition, view);
}

//texture atlas management modelled from https://www.youtube.com/watch?v=POEzdiqEESo
GLvoid ParticleEffect::update(GLfloat deltaTime, glm::vec3 cameraPosition, glm::mat4 view) 
{
	duration -=deltaTime;
	newParticles = pps*deltaTime;
	count = (GLint)(newParticles);
	partialParticle = fmod(newParticles, 1);
	if (randomFloat() < partialParticle) {
		count += 1;
	}
	
	createNewParticles();
	
	pointer = 0;
	particleCount = 0;
	stageCount = particleTexture.numberOfRows * particleTexture.numberOfRows;
	for (GLint i = 0; i<MAX_INSTANCES; i++)
	{

		Particle& p = ParticlesContainer[i];

		if (p.age < p.lifeLength) 
		{

			// Decrease life
			p.age += deltaTime;
			if (p.age < p.lifeLength) 
			{
	
				p.velocity += gravity * gravityEffect * (GLfloat)deltaTime;
				p.pos += p.velocity * (GLfloat)deltaTime;
				p.cameradistance = glm::length(p.pos - cameraPosition);

				lifeFactor = p.age / p.lifeLength;
				texProgression = lifeFactor * stageCount;
				texIndex1 = (GLint)glm::floor(texProgression);
				//don't want to go further than the last texture in atlas
				texIndex2 = texIndex1 < stageCount - 1 ? texIndex1 + 1: texIndex1;
				p.blend = fmod(texProgression, 1);

				column = texIndex1 % particleTexture.numberOfRows;
				row = texIndex1 / particleTexture.numberOfRows;
				p.textureOffset1.x = (GLfloat)column / particleTexture.numberOfRows;
				p.textureOffset1.y = (GLfloat)row / particleTexture.numberOfRows;

				column = texIndex2 % particleTexture.numberOfRows;
				row = texIndex2 / particleTexture.numberOfRows;
				p.textureOffset2.x = (GLfloat)column / particleTexture.numberOfRows;
				p.textureOffset2.y = (GLfloat)row / particleTexture.numberOfRows;

				//put positions and blend data into array for instancing
				positionBlendData[4 * particleCount + 0] = p.pos.x;
				positionBlendData[4 * particleCount + 1] = p.pos.y;
				positionBlendData[4 * particleCount + 2] = p.pos.z;
				positionBlendData[4 * particleCount + 3] = p.blend;

				//put texture data into array for instancing
				textureData[4 * particleCount + 0] = p.textureOffset1.x;
				textureData[4 * particleCount + 1] = p.textureOffset1.y;
				textureData[4 * particleCount + 2] = p.textureOffset2.x;
				textureData[4 * particleCount + 3] = p.textureOffset2.y;
			}
			else 
			{
				p.cameradistance = -1.0f;
			}
			particleCount++;
		}
	}

	sortParticles();
	
}


GLvoid ParticleEffect::draw(glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPosition) 
{
	shaderProgram->useProgram();
	blendTool.transclucentSetUp();
	glBindTexture(GL_TEXTURE_2D, particleTexture.textureID);

	// Bind our texture in Texture Unit 0
	
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(textureID, 0);
	glBindTexture(GL_TEXTURE_2D, particleTexture.textureID);

	glm::vec3 CameraRight_worldspace = { view[0][0], view[1][0], view[2][0] };
	glm::vec3 CameraUp_worldspace = { view[0][1], view[1][1], view[2][1] };

	glUniformMatrix4fv(viewMatrix_ID, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionMatrix_ID, 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3f(cameraRight_worldspace_ID, CameraRight_worldspace.x, CameraRight_worldspace.y, CameraRight_worldspace.z);
	glUniform3f(cameraUp_worldspace_ID, CameraUp_worldspace.x, CameraUp_worldspace.y, CameraUp_worldspace.z);
	glUniform1f(noOfRows_ID, particleTexture.numberOfRows);
	
	fillBuffersForRender();

	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, particleCount);

	postRender();
	
	blendTool.disableBlending();
}

GLvoid ParticleEffect::createNewParticles() 
{
	for (int i = 0; i<count; i++) {
		GLint particleIndex = findUnusedParticle();
		Particle& p = ParticlesContainer[particleIndex];
		
		p.pos = systemCentre;
		p.lifeLength = 2.0f;
		p.age = 0.0f;

		GLfloat spread = 2.0f;

		if (emitter == 0) p.velocity = sphereEmitter->generateVelocity();
		else if (emitter == 1) p.velocity = coneEmitter->generateVelocity() * spread;
		else p.velocity = randUnitVec(); 

		p.size = 0.5f;
	}
}

GLvoid ParticleEffect::getUniformLocations() 
{
	cameraRight_worldspace_ID = glGetUniformLocation(shaderProgram->getProgram(), "CameraRight_worldspace");
	cameraUp_worldspace_ID = glGetUniformLocation(shaderProgram->getProgram(), "CameraUp_worldspace");
	noOfRows_ID = glGetUniformLocation(shaderProgram->getProgram(), "noOfRows");
	projectionMatrix_ID = glGetUniformLocation(shaderProgram->getProgram(), "projectionMatrix");
	viewMatrix_ID = glGetUniformLocation(shaderProgram->getProgram(), "viewMatrix");
	textureID = glGetUniformLocation(shaderProgram->getProgram(), "textureID");
}

GLint ParticleEffect::findUnusedParticle() 
{
	for (int i = lastUsedParticle; i<MAX_INSTANCES; i++) 
	{
		if (ParticlesContainer[i].age >= ParticlesContainer[i].lifeLength) 
		{
			lastUsedParticle = i;
			return i;
		}
	}

	for (int i = 0; i<lastUsedParticle; i++) 
	{
		if (ParticlesContainer[i].age >= ParticlesContainer[i].lifeLength) 
		{
			lastUsedParticle = i;
			return i;
		}
	}

	return 0; // All particles are taken, override the first one
}

GLvoid ParticleEffect::fillBuffersForRender()
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_INSTANCES * sizeof(GLfloat) * 4, NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particleCount * sizeof(GLfloat) * 4, positionBlendData);

	glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_INSTANCES * sizeof(GLfloat) * 4, NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particleCount * sizeof(GLfloat) * 4, textureData);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);


	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
}

GLvoid ParticleEffect::postRender() 
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glActiveTexture(0);
}
GLvoid ParticleEffect::sortParticles() 
{
	std::sort(std::begin(ParticlesContainer), std::end(ParticlesContainer), [&](const Particle& first, const Particle& second) -> bool
	{
		return (first.cameradistance > second.cameradistance);
	});
}

GLboolean ParticleEffect::isFinished() 
{
	if (!infinite) {
		return duration <= 0;
	}
	else return false;
}