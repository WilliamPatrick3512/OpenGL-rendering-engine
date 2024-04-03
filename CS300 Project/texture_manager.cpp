#include "texture_manager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
void texture_manager::loadTextureFromFile(std::string filename, GLuint* texture_handle, int* width, int* height, int* layers)
{

	unsigned char* data = stbi_load(filename.c_str(), width, height, layers, 0);
	glBindTexture(GL_TEXTURE_2D, *texture_handle);
	if (data)
	{
		if(strcmp(filename.c_str(), "../Common/textures/grid_512.png"))
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *width, *height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, *width, *height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	}
	stbi_image_free(data);
}


void texture_manager::bindTexture(GLuint* texture_handle)
{

}