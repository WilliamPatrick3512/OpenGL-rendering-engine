#include "Lights.h"
Lights::Lights(glm::vec3 diffuse, glm::vec3 specular, glm::vec3 ambient, glm::vec3 emmissive, float rotation_offset)
{

	light_diffuse = diffuse;
	light_specular = specular;
	ambient_color = ambient;
	light_emmissive = emmissive;
	rotation = rotation_offset;
	dlight_direction = glm::vec3(0.0f, 0.0f, 1.0f);
	inner_angle = 30.0f;
	specular_exponent = 10.5f;
	falloff_value = 1.0f;

}Lights::Lights()
{
	light_type = spot;
	specular_exponent = 10.5f;

	glm::vec3 light_color(1.0f, 1.0f, 1.0f);

	light_diffuse = light_color;
	light_specular = light_color;
	light_emmissive = light_color;
	ambient_color = light_color;
	dlight_direction = glm::vec3(0.0f, 0.0f, 1.0f);

	rotation = 0.0f;
	inner_angle = 30.0f;
	falloff_value = 1.0f;

	
}