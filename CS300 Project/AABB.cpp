#include "AABB.h"

AABB::AABB()
{
	center = glm::vec3(0.0f);
	aabb_max = glm::vec3(0.0f);
	aabb_min = glm::vec3(0.0f);
	volume = 0.0f;
}

AABB::AABB(glm::vec3 center, float extent)
{
	center = center;
	aabb_max = center + glm::vec3(extent);
	aabb_min = center - glm::vec3(extent);
	volume = (aabb_max - aabb_min).x * (aabb_max - aabb_min).y * (aabb_max - aabb_min).z;
}

void AABB::calculate_AABB(std::vector<glm::vec3> vertices)
{
	glm::vec3 maximum(std::numeric_limits<float>::lowest());
	glm::vec3 minimum(std::numeric_limits<float>::max());


	for (int i = 0; i < vertices.size(); i++)
	{
		//check the x values

		if (minimum.x > vertices[i].x)
			minimum.x = vertices[i].x;
		if (maximum.x < vertices[i].x)
			maximum.x = vertices[i].x;

		//check the y values
		if (minimum.y > vertices[i].y)
			minimum.y = vertices[i].y;
		if (maximum.y < vertices[i].y)
			maximum.y = vertices[i].y;

		//check the z values
		if (minimum.z > vertices[i].z)
			minimum.z = vertices[i].z;
		if (maximum.z < vertices[i].z)
			maximum.z = vertices[i].z;
	}
	aabb_min = minimum;
	aabb_max = maximum;
	center = (minimum + maximum) / 2.0f;

	volume = (aabb_max - aabb_min).x * (aabb_max - aabb_min).y * (aabb_max - aabb_min).z;

	
}

glm::vec3 AABB::aabb_getmin()
{
	return aabb_min;
}
glm::vec3 AABB::aabb_getmax()
{
	return aabb_max;
}

Bounding_Volume* AABB::compute_bv(std::vector<glm::vec3> vertices)
{
	calculate_AABB(vertices);
	return this;
}

void AABB::calculate_AABB(glm::vec3 Point_A, glm::vec3 Point_B)
{
	glm::vec3 result_min = Point_A;
	glm::vec3 result_max = Point_A;

	//compare mins
	if (result_min.x > Point_B.x)
		result_min.x = Point_B.x;

	if (result_min.y > Point_B.y)
		result_min.y = Point_B.y;

	if (result_min.z > Point_B.z)
		result_min.z = Point_B.z;

	//compare maxes

	if (result_max.x < Point_B.x)
		result_max.x = Point_B.x;

	if (result_max.y < Point_B.y)
		result_max.y = Point_B.y;

	if (result_max.z < Point_B.z)
		result_max.z = Point_B.z;

	aabb_min = result_min;
	aabb_max = result_max;
	center = (result_min + result_max) / 2.0f;
	volume = (aabb_max - aabb_min).x * (aabb_max - aabb_min).y * (aabb_max - aabb_min).z;
}

void AABB::calculate_AABB(AABB AABB_A, AABB AABB_B)
{
	glm::vec3 result_min = AABB_A.aabb_min;
	glm::vec3 result_max = AABB_A.aabb_max;

	//compare mins
	if (result_min.x > AABB_B.aabb_min.x)
		result_min.x = AABB_B.aabb_min.x;

	if (result_min.y > AABB_B.aabb_min.y)
		result_min.y = AABB_B.aabb_min.y;

	if (result_min.z > AABB_B.aabb_min.z)
		result_min.z = AABB_B.aabb_min.z;

	//compare maxes

	if (result_max.x < AABB_B.aabb_max.x)
		result_max.x = AABB_B.aabb_max.x;
								   
	if (result_max.y < AABB_B.aabb_max.y)
		result_max.y = AABB_B.aabb_max.y;
								   
	if (result_max.z < AABB_B.aabb_max.z)
		result_max.z = AABB_B.aabb_max.z;

	aabb_min = result_min;
	aabb_max = result_max;
	center = (result_min + result_max) / 2.0f;
	volume = (aabb_max - aabb_min).x * (aabb_max - aabb_min).y * (aabb_max - aabb_min).z;

}

void AABB::calculate_AABB(AABB* objects[], int numObjects)
{
	glm::vec3 maximum(std::numeric_limits<float>::lowest());
	glm::vec3 minimum(std::numeric_limits<float>::max());


	for (int i = 0; i < numObjects; i++)
	{
		//check the x values
		if (minimum.x > objects[i]->aabb_getmin().x)
			minimum.x = objects[i]->aabb_getmin().x;
		if (maximum.x < objects[i]->aabb_getmax().x)
			maximum.x = objects[i]->aabb_getmax().x;
								  
		//check the y values	  
		if (minimum.y > objects[i]->aabb_getmin().y)
			minimum.y = objects[i]->aabb_getmin().y;
		if (maximum.y < objects[i]->aabb_getmax().y)
			maximum.y = objects[i]->aabb_getmax().y;
								  
		//check the z values	  
		if (minimum.z > objects[i]->aabb_getmin().z)
			minimum.z = objects[i]->aabb_getmin().z;
		if (maximum.z < objects[i]->aabb_getmax().z)
			maximum.z = objects[i]->aabb_getmax().z;
	}
	aabb_min = minimum;
	aabb_max = maximum;
	center = (minimum + maximum) / 2.0f;
	volume = (aabb_max - aabb_min).x * (aabb_max - aabb_min).y * (aabb_max - aabb_min).z;

}

glm::vec3 AABB::find_intercept(glm::vec3 inside, glm::vec3 outside)
{
	glm::vec3 result = outside;
	glm::vec3 direction = (outside - inside);

	if ((outside.x > aabb_max.x) || (outside.x < aabb_min.x))
	{
		if (direction.x > 0.0f)
		{
			float distance = (inside.x - aabb_max.x) / (direction.x);
			result = inside + (direction * -distance);
		}
		else
		{
			float distance = (inside.x - aabb_min.x) / (direction.x);
			result = inside + (direction * -distance);
		}
	}
	else if ((outside.y > aabb_max.y) || (outside.y < aabb_min.y))
	{
		if (direction.y > 0.0f)
		{
			float distance = (inside.y - aabb_max.y) / (direction.y);
			result = inside + (direction * -distance);
		}
		else
		{
			float distance = (inside.y - aabb_min.y) / (direction.y);
			result = inside + (direction * -distance);
		}
	}
	else if ((outside.z > aabb_max.z) || (outside.z < aabb_min.z))
	{
		if (direction.z > 0.0f)
		{
			float distance = (inside.z - aabb_max.z) / (direction.z);
			result = inside + (direction * -distance);
		}
		else
		{
			float distance = (inside.z - aabb_min.z) / (direction.z);
			result = inside + (direction * -distance);
		}
	}
	
	return result;
}

bool AABB::check_point(glm::vec3 point)
{
	if (point.x < aabb_max.x && point.y < aabb_max.y && point.z < aabb_max.z
	  && point.x > aabb_min.x && point.y > aabb_min.y && point.z > aabb_min.z)
	{
		return true;
	}
	else
	{
		return false;
	}
}