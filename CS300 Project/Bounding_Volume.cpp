#include "Bounding_Volume.h"

glm::vec3 Bounding_Volume::get_center()
{
	return center;
}

float Bounding_Volume::get_volume()
{
	return volume;
}