#pragma once

#include<vector>
#include <glm/glm.hpp>

class Lights
{


public:
	enum light_type
	{
		point,
		directional,
		spot
	};

	Lights(glm::vec3 diffuse, glm::vec3 specular, glm::vec3 ambient, glm::vec3 emmissive, float rotation_offset);
	Lights();

	int light_type;
	glm::vec3 light_position;
	glm::vec3 dlight_direction;
	float specular_exponent;

	glm::vec3 light_diffuse;
	glm::vec3 light_specular;
	glm::vec3 light_emmissive;
	glm::vec3 ambient_color;

	float rotation;
	float inner_angle;
	float outer_angle;
	float falloff_value;




};

