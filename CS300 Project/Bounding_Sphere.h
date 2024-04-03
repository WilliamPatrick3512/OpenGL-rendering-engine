#pragma once
#include "Bounding_Volume.h"
#include "AABB.h"
#define Decomposition_limit 50

class Bounding_Sphere :
	public Bounding_Volume
{
	glm::vec3 Larson_Normals[49] = {
		//EPOS 6
		glm::vec3(1.0f  ,  0.0f  ,  0.0f),
		glm::vec3(0.0f  ,  1.0f  ,  0.0f),
		glm::vec3(0.0f  ,  0.0f  ,  1.0f),
		//EPOS 14	    	  	    
		glm::vec3(1.0f  ,  1.0f  ,  1.0f),
		glm::vec3(1.0f  ,  1.0f  , -1.0f),
		glm::vec3(1.0f  , -1.0f  ,  1.0f),
		glm::vec3(1.0f  , -1.0f  , -1.0f),
		//EPOS 26		  		   
		glm::vec3(1.0f  ,  1.0f  ,  0.0f),
		glm::vec3(1.0f  , -1.0f  ,  0.0f),
		glm::vec3(1.0f  ,  0.0f  ,  1.0f),
		glm::vec3(1.0f  ,  0.0f  , -1.0f),
		glm::vec3(0.0f  ,  1.0f  ,  1.0f),
		glm::vec3(0.0f  ,  1.0f  , -1.0f),
		//EPOS 50
		glm::vec3(0.0f  ,  1.0f  ,  2.0f),
		glm::vec3(0.0f  ,  2.0f  ,  1.0f),
		glm::vec3(1.0f  ,  0.0f  ,  2.0f),
		glm::vec3(2.0f  ,  0.0f  ,  1.0f),
		glm::vec3(1.0f  ,  2.0f  ,  0.0f),
		glm::vec3(2.0f  ,  1.0f  ,  0.0f),
		glm::vec3(0.0f  ,  1.0f  ,  -2.0f),
		glm::vec3(0.0f  ,  2.0f  ,  -1.0f),
		glm::vec3(1.0f  ,  0.0f  ,  -2.0f),
		glm::vec3(2.0f  ,  0.0f  ,  -1.0f),
		glm::vec3(1.0f  , -2.0f  ,   0.0f),
		glm::vec3(2.0f  , -1.0f  ,   0.0f),
		//EPOS 74
		glm::vec3(1.0f  ,  1.0f  ,   2.0f),
		glm::vec3(2.0f  ,  1.0f  ,   1.0f),
		glm::vec3(1.0f  ,  2.0f  ,   1.0f),
		glm::vec3(1.0f  , -1.0f  ,   2.0f),
		glm::vec3(1.0f  ,  1.0f  ,  -2.0f),
		glm::vec3(1.0f  , -1.0f  ,  -2.0f),
		glm::vec3(2.0f  , -1.0f  ,   1.0f),
		glm::vec3(2.0f  ,  1.0f  ,  -1.0f),
		glm::vec3(2.0f  , -1.0f  ,  -1.0f),
		glm::vec3(1.0f  , -2.0f  ,   1.0f),
		glm::vec3(1.0f  ,  2.0f  ,  -1.0f),
		glm::vec3(1.0f  , -2.0f  ,  -1.0f),
		//EPOS 98
		glm::vec3(2.0f  ,  2.0f  ,  1.0f),
		glm::vec3(1.0f  ,  2.0f  ,  2.0f),
		glm::vec3(2.0f  ,  1.0f  ,  2.0f),
		glm::vec3(2.0f  , -2.0f  ,  1.0f),
		glm::vec3(2.0f  ,  2.0f  , -1.0f),
		glm::vec3(2.0f  , -2.0f  , -1.0f),
		glm::vec3(1.0f  , -2.0f  ,  2.0f),
		glm::vec3(1.0f  ,  2.0f  , -2.0f),
		glm::vec3(1.0f  , -2.0f  , -2.0f),
		glm::vec3(2.0f  , -1.0f  ,  2.0f),
		glm::vec3(2.0f  ,  1.0f  , -2.0f),
		glm::vec3(2.0f  , -1.0f  , -2.0f)
	};
	

public:
	enum class BSphere_type
	{
		Ritter,
		Larsson,
		PCA,
		AABB_sphere
	};
	float radius;

	void calculate_Bsphere_Ritter(std::vector<glm::vec3> vertices);
	void calculate_Bsphere_Larsson(std::vector<glm::vec3> vertices, int EPOS_DEPTH);
	void calculate_Bsphere_PCA(std::vector<glm::vec3> vertices);
	void calculate_Bsphere_AABB(AABB bounding_box);
	void calculate_Bsphere_Spheres(Bounding_Sphere sphere_A, Bounding_Sphere sphere_B);
	void calculate_Bsphere_Spheres(Bounding_Sphere* objects[], int numObjects);
	void grow_radius(glm::vec3 initial_center, float initial_radius, std::vector<glm::vec3> vertices);

	Bounding_Volume* compute_bv(std::vector<glm::vec3> vertices, BSphere_type calculation_type);
	Bounding_Volume* compute_bv(AABB bounding_box);

};

