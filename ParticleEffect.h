#pragma once
#include <glew.h>

#include <String.h>
#include <vector>
#include "ShaderLoader.h"
#include "AlphaBlenderTool.h"
#include "ParticleTexture.h"
#include "SphereEmitter.h"
#include "ConeEmitter.h"
#include "Particle.h"
#include "Orientation.h"
#include "InstancedObject.h"
#include "info.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Random.h"

extern const GLint MAX_INSTANCES;
extern const GLint INSTANCE_DATA_LENGTH;
class ParticleEffect : InstancedObject
{
private:
	
	GLint pps, count, emitter;;
	Shader* shaderProgram;
	GLfloat* positionBlendData;
	GLfloat* textureData;

	GLuint vertexVBO, particleVBO, textureVBO;
	
	glm::vec3 change, systemCentre, direction;
	GLfloat directionDeviation, newParticles, partialParticle;
	GLfloat gravityEffect, duration;
	GLboolean infinite;

	AlphaBlenderTool blendTool;
	ParticleTexture particleTexture;

	GLint lastUsedParticle, instanceDataLength, pointer, particleCount, stageCount;;
	GLfloat lifeFactor, texProgression;
	
	glm::vec3 gravity;
	GLint texIndex1, texIndex2, column, row;
	GLuint vbo, vao;

	GLuint cameraRight_worldspace_ID;
	GLuint cameraUp_worldspace_ID;
	GLuint ViewProjMatrixID;
	GLuint noOfRows_ID;
	GLuint viewMatrix_ID;
	GLuint projectionMatrix_ID;
	GLuint textureID;

	GLint findUnusedParticle();
	GLvoid sortParticles();
	GLvoid setUp(GLchar* texture);
	GLvoid fillBuffersForRender();
	GLvoid postRender();
	GLvoid getUniformLocations();
	GLvoid createNewParticles();

public:
	ParticleEffect(GLint pps, glm::vec3 systemCentre, GLchar* texture, GLuint numberOfTextureRows, GLint emitter, GLboolean infinite, GLfloat duration, GLfloat gravityEffect);
	ParticleEffect(GLint pps, glm::vec3 systemCentre);
	~ParticleEffect();

	ConeEmitter* coneEmitter;
	SphereEmitter* sphereEmitter;

	GLvoid update(GLfloat deltaTime, glm::vec3 cameraPosition, glm::mat4 view, glm::vec3 systemCentre);
	GLvoid update(GLfloat deltaTime, glm::vec3 cameraPosition, glm::mat4 view);
	GLvoid draw(glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPosition);
	GLboolean isFinished();
	

};