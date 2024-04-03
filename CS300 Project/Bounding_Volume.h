#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <limits>
class Bounding_Volume
{

public:
	float volume;
	glm::vec3 center;
	static Bounding_Volume* compute_bv();
	glm::vec3 get_center();
	float get_volume();
	bool check_collision();
};

