#include "SkyBox.h"


GLuint loadCubeMap(std::vector<const GLchar*> faces);

SkyBox::SkyBox()
	:shader{ "Shader/skyBoxVertex.txt", "Shader/skyBoxFragment.txt" }
{
	GLfloat radius = 1.0f;

	GLfloat vertices[] =
	{
		// back face
		-radius, -radius, -radius,
		-radius,  radius, -radius,
		 radius, -radius, -radius,

		-radius,  radius, -radius,
		 radius,  radius, -radius,
		 radius, -radius, -radius,

		// front face
		-radius, -radius,  radius,
		-radius,  radius,  radius,
		 radius, -radius,  radius,

		-radius,  radius,  radius,
		 radius,  radius,  radius,
		 radius, -radius,  radius,

		// left face
		-radius, -radius,  radius,
		-radius,  radius,  radius,
		-radius,  radius, -radius,

		-radius,  radius, -radius,
		-radius, -radius, -radius,
		-radius, -radius,  radius,

		// right face
		 radius, -radius,  radius,
		 radius,  radius,  radius,
		 radius,  radius, -radius,

		 radius,  radius, -radius,
		 radius, -radius, -radius,
		 radius, -radius,  radius,

		// bottom face
		-radius, -radius,  radius,
		-radius, -radius, -radius,
		 radius, -radius, -radius,

		 radius, -radius, -radius,
		 radius, -radius,  radius,
		-radius, -radius,  radius,

		// top face
		-radius,  radius,  radius,
		-radius,  radius, -radius,
		 radius,  radius, -radius,

		 radius,  radius, -radius,
		 radius,  radius,  radius,
		-radius,  radius,  radius,
	};


	// Generate Vertex Array object and vertex buffer
	helper.createVertexArray(vertexArray);
	helper.fillVertexArrayWithVertices(vertexArray, vertexBuffer, vertices, sizeof(vertices), 0);
	
	glGenBuffers(1, &textureBuffer);

	// Cubemap (Skybox)
	faces.push_back("skybox/right.bmp");
	faces.push_back("skybox/left.bmp");
	faces.push_back("skybox/top.bmp");
	faces.push_back("skybox/bottom.bmp");
	faces.push_back("skybox/back.bmp");
	faces.push_back("skybox/front.bmp");
	texture = loadCubeMap(faces);
	

	//GLint  width, height;
	//unsigned char* image = SOIL_load_image("picture.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	//glGenerateMipmap(GL_TEXTURE_2D);
	//SOIL_free_image_data(image);
	//glBindTexture(GL_TEXTURE_2D, 0);

}

SkyBox::~SkyBox()
{
}

/*Stuart*/
GLvoid SkyBox::setTexture(const GLchar* right, const GLchar* left, const GLchar* top, const GLchar* bottom, const GLchar* back, const GLchar* front) {
	glGenBuffers(1, &textureBuffer);

	// Cubemap (Skybox)
	faces.clear();
	faces.push_back(right);
	faces.push_back(left);
	faces.push_back(top);
	faces.push_back(bottom);
	faces.push_back(back);
	faces.push_back(front);
	texture = loadCubeMap(faces);
}
GLvoid SkyBox::draw(glm::mat4 viewMatrix, glm::mat4 projection)
{
	// skybox cube
	shader.useProgram();

	// Remember to turn depth writing off
	glDepthMask(GL_FALSE);

	// Remove any translation component of the view matrix
	glm::mat4 view = glm::mat4(glm::mat3(viewMatrix));	

	GLuint program = shader.getProgram();
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	// skybox cube
	glBindVertexArray(vertexArray);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(program, "skybox"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	// Turn depth writing back on
	glDepthMask(GL_TRUE);
}

GLuint loadCubeMap(std::vector<const GLchar*> faces)
{
	stbi_set_flip_vertically_on_load(false);
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height,components;
	unsigned char* image;
	GLenum format;
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = stbi_load(faces[i], &width, &height,&components, 0);

		if (components == 1)
			format = GL_RED;
		else if (components == 3)
			format = GL_RGB;
		else
			format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
		stbi_image_free(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
	stbi_set_flip_vertically_on_load(true);
}

GLuint SkyBox::getTexture()
{
	return texture;
}