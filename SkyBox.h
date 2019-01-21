#pragma once
#include <glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <SOIL.h>
#include "stb_image.h"
#include "Shader.h"
#include "info.h"

class SkyBox
{
private:
	GLuint vertexArray;
	GLuint vertexBuffer;
	GLuint textureBuffer;

	Shader shader;

	GLuint numOfVertices;
	GLsizei sizeOfVertices;


	std::vector<const GLchar*> faces;


public:
	SkyBox::SkyBox();
	~SkyBox();

	GLuint texture;

	GLvoid setTexture(const GLchar* right, const GLchar* left, const GLchar* top, 
		const GLchar* bottom, const GLchar* back, const GLchar* front);
	GLuint getTexture();

	GLvoid draw(glm::mat4 view, glm::mat4 projection);
};