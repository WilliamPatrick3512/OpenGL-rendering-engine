#pragma once
#include <glm/glm.hpp>
class normal
{
public:
	normal(glm::vec3 point, glm::vec3 vector);
	glm::vec3 origin;
	glm::vec3 direction;
};

