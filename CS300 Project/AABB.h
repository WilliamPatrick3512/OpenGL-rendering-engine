#pragma once
#include "Bounding_Volume.h"
class AABB :
	public Bounding_Volume
{
	
	glm::vec3 aabb_min;
	glm::vec3 aabb_max;

public:

	AABB();
	AABB(glm::vec3 center, float extent);

	void calculate_AABB(std::vector<glm::vec3> vertices);
	Bounding_Volume* compute_bv(std::vector<glm::vec3> vertices);
	void calculate_AABB(AABB AABB_A, AABB AABB_B);
	void calculate_AABB(glm::vec3 Point_A, glm::vec3 Point_B);
	void calculate_AABB(AABB* objects[], int numObjects);

	glm::vec3 find_intercept(glm::vec3 inside, glm::vec3 outside);

	bool check_point(glm::vec3 point);

	glm::vec3 aabb_getmin();
	glm::vec3 aabb_getmax();



};

