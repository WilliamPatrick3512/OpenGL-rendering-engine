#pragma once
#include <glm/glm.hpp>
#include<string>
#include <GL/glew.h>

class texture_manager
{
public:
	void loadTextureFromFile(std::string filename, GLuint* texture_handle, int* width, int* height, int* layers);
	void bindTexture(GLuint* texture_handle);

	bool calculateCPU;

	unsigned char* image_buffer;

	unsigned int size;

	int width;
	int height;

};

