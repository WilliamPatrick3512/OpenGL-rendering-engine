#include "Bounding_Sphere.h"

void Bounding_Sphere::calculate_Bsphere_Ritter(std::vector<glm::vec3> vertices)
{
	radius = 0.0f;
	center = glm::vec3(0.0f);
	//find the extremes
	glm::vec3 max_x(std::numeric_limits<float>::lowest());
	glm::vec3 max_y(std::numeric_limits<float>::lowest());
	glm::vec3 max_z(std::numeric_limits<float>::lowest());

	glm::vec3 min_x(std::numeric_limits<float>::max());
	glm::vec3 min_y(std::numeric_limits<float>::max());
	glm::vec3 min_z(std::numeric_limits<float>::max());


	for (int i = 0; i < vertices.size(); i++)
	{
		//check the x values
		if (min_x.x > vertices[i].x)
			min_x = vertices[i];
		if (max_x.x < vertices[i].x)
			max_x = vertices[i];

		//check the y values
		if (min_y.y > vertices[i].y)
			min_y = vertices[i];
		if (max_y.y < vertices[i].y)
			max_y = vertices[i];

		//check the z values
		if (min_z.z > vertices[i].z)
			min_z = vertices[i];
		if (max_z.z < vertices[i].z)
			max_z = vertices[i];
	}

	//add all mins and maxes to a vector
	std::vector<glm::vec3> points;
	points.push_back(min_x);
	points.push_back(min_y);
	points.push_back(min_z);
	points.push_back(max_x);
	points.push_back(max_y);
	points.push_back(max_z);

	float max_distance = 0.0f;
	glm::vec3 extreme_point_a;
	glm::vec3 extreme_point_b;

	//find the two most extreme points
	for (int i = 0; i < points.size(); i++)
	{
		for (int j = i + 1; j < points.size(); j++)
		{
			float distance = glm::length(points[i] - points[j]);
			if (distance > max_distance)
			{
				extreme_point_a = points[i];
				extreme_point_b = points[j];
				max_distance = distance;
			}
		}
	}

	//initialize the bounding sphere
	float temp_radius = max_distance/2.0f;
	glm::vec3 temp_center = (extreme_point_a + extreme_point_b)/2.0f;

	grow_radius(temp_center, temp_radius, vertices);
}

void Bounding_Sphere::calculate_Bsphere_Larsson(std::vector<glm::vec3> vertices, int EPOS_DEPTH)
{
	radius = 0.0f;
	center = glm::vec3(0.0f);
	glm::vec3 Larson_Mins[49];
	glm::vec3 Larson_Maxes[49];

	//get initial extreme points for the sphere
	for (int i = 0; i < EPOS_DEPTH/2; i++)
	{
		for (int j = 0; j < vertices.size(); j++)
		{
			if (glm::dot(vertices[j], Larson_Normals[i]) < glm::dot(Larson_Mins[i], Larson_Normals[i]))
			{
				Larson_Mins[i] = vertices[j];
			}

			if (glm::dot(vertices[j], Larson_Normals[i]) > glm::dot(Larson_Maxes[i], Larson_Normals[i]))
			{
				Larson_Maxes[i] = vertices[j];
			}
		}
	}

	std::vector<glm::vec3> point_list;

	
	point_list.insert(point_list.end(), Larson_Mins, Larson_Mins + 48);
	point_list.insert(point_list.end(), Larson_Maxes, Larson_Maxes + 48);

	float max_distance = 0.0f;
	glm::vec3 extreme_point_a;
	glm::vec3 extreme_point_b;
	for (int i = 0; i < EPOS_DEPTH; i++)
	{
		for (int j = i + 1; j < EPOS_DEPTH; j++)
		{
			float distance = glm::length(point_list[i] - point_list[j]);
			if (distance > max_distance)
			{
				extreme_point_a = point_list[i];
				extreme_point_b = point_list[j];
				max_distance = distance;
			}
		}
	}

	float temp_radius = glm::length(extreme_point_a - extreme_point_b) / 2.0f;
	glm::vec3 temp_center = (extreme_point_a + extreme_point_b) / 2.0f;

	grow_radius(temp_center, temp_radius, vertices);

}

void Bounding_Sphere::calculate_Bsphere_PCA(std::vector<glm::vec3> vertices)
{
	radius = 0.0f;
	center = glm::vec3(0.0f);
	glm::vec3 mean;
	for (int i = 0; i < vertices.size(); i++)
	{
		mean += (vertices[i] / (float)vertices.size());
	}

	glm::mat3 covariance_matrix;

	for (int i = 0; i < vertices.size(); i++)
	{
		//covariance values for x*x
		covariance_matrix[0][0] += (pow(vertices[i].x - mean.x, 2.0f) / (float)vertices.size());

		//covariance values for x*y
		covariance_matrix[0][1] += ((vertices[i].x - mean.x) * (vertices[i].y - mean.y) / (float)vertices.size());
		covariance_matrix[1][0] += ((vertices[i].x - mean.x) * (vertices[i].y - mean.y) / (float)vertices.size());

		//covariance values for x*z
		covariance_matrix[0][2] += ((vertices[i].x - mean.x) * (vertices[i].z - mean.z) / (float)vertices.size());
		covariance_matrix[2][0] += ((vertices[i].x - mean.x) * (vertices[i].z - mean.z) / (float)vertices.size());


		//covariance values for y*y
		covariance_matrix[1][1] += (pow(vertices[i].y - mean.y, 2.0f) / (float)vertices.size());

		//covariance values for y*z
		covariance_matrix[2][1] += ((vertices[i].z - mean.z) * (vertices[i].y - mean.y) / (float)vertices.size());
		covariance_matrix[1][2] += ((vertices[i].z - mean.z) * (vertices[i].y - mean.y) / (float)vertices.size());


		//covariance values for z*z
		covariance_matrix[2][2] += (pow(vertices[i].z - mean.z, 2.0f) / (float)vertices.size());
	}

	//define A and V from slides
	glm::mat3 eigenvalues = covariance_matrix;
	glm::mat3 eigenvectors = glm::mat3();

	//iterate until "converged"
	for (int i = 0; i < Decomposition_limit; i++)
	{
		//find nondiagonal max element
		float max_value = std::numeric_limits<float>::lowest();
		int max_p, max_q;
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				if (eigenvalues[j][k] > max_value&& j != k)
				{
					max_value = eigenvalues[j][k];
					max_p = j;
					max_q = k;
				}
			}
		}

		//calculate c & s
		float beta_value = (eigenvalues[max_p][max_p] - eigenvalues[max_q][max_q]) / 2.0f * eigenvalues[max_p][max_q];

		float t_value = 1.0f;

		if (beta_value > 0.0f)
		{
			t_value = 1.0f / (abs(beta_value) + sqrt(pow(beta_value, 2.0f) + 1.0f));

		}
		else
		{
			t_value = -1.0f / (abs(beta_value) + sqrt(pow(beta_value, 2.0f) + 1.0f));
		}

		float C = 1.0f / sqrt(pow(t_value, 2.0f) + 1.0f);
		float S = C * t_value;

		//construct J and J_inverse matrices
		glm::mat3 J = glm::mat3();
		glm::mat3 J_inverse = glm::mat3();

		J[max_p][max_p] = C;
		J[max_q][max_q] = C;
		J[max_q][max_p] = S;
		J[max_p][max_q] = -1.0f * S;


		J_inverse[max_p][max_p] = C;
		J_inverse[max_q][max_q] = C;
		J[max_q][max_p] = -1.0f * S;
		J[max_p][max_q] = S;

		//modify the A matrix
		eigenvalues = J_inverse * eigenvalues * J;

		//modify the V matrix
		eigenvectors = eigenvectors * J;

	}
	//interpret values to get the sphere
	float max_eigenvalue = eigenvalues[0][0];
	glm::vec3 max_vector = eigenvectors[0];

	if (max_eigenvalue < eigenvalues[1][1])
	{
		max_eigenvalue = eigenvalues[1][1];
		max_vector = eigenvectors[1];
	}

	if (max_eigenvalue < eigenvalues[2][2])
	{
		max_eigenvalue = eigenvalues[2][2];
		max_vector = eigenvectors[2];
	}
	glm::vec3 min_extreme(0.0f);
	glm::vec3 max_extreme(0.0f);

	//get initial extreme points for the sphere
	for (int i = 0; i < vertices.size(); i++)
	{
		if (glm::dot(vertices[i], max_vector) < glm::dot(min_extreme, max_vector))
		{
			min_extreme = vertices[i];
		}

		if (glm::dot(vertices[i], max_vector) > glm::dot(max_extreme, max_vector))
		{
			max_extreme = vertices[i];
		}
	}

	float temp_radius = glm::length(min_extreme - max_extreme) / 2.0f;
	glm::vec3 temp_center = (min_extreme + max_extreme) / 2.0f;

	grow_radius(temp_center, temp_radius, vertices);
}

void Bounding_Sphere::calculate_Bsphere_AABB(AABB bounding_box)
{
	glm::vec3 extent = (bounding_box.aabb_getmax() - bounding_box.get_center());
	center = bounding_box.get_center();
	radius = glm::length(extent);
}

Bounding_Volume* Bounding_Sphere::compute_bv(std::vector<glm::vec3> vertices, BSphere_type calculation_type)
{
	if (calculation_type == BSphere_type::Ritter)
	{
		calculate_Bsphere_Ritter(vertices);
	}
	else if (calculation_type == BSphere_type::Larsson)
	{
		calculate_Bsphere_Larsson(vertices, 26);
	}
	// PCA sphere
	else if (calculation_type == BSphere_type::PCA)
	{
		calculate_Bsphere_PCA(vertices);
	}
	return this;
}

void Bounding_Sphere::calculate_Bsphere_Spheres(Bounding_Sphere sphere_A, Bounding_Sphere sphere_B)
{
	center = (sphere_A.get_center() + sphere_B.get_center()) / 2.0f;

	radius = (glm::length(sphere_A.get_center() - sphere_B.get_center()) + sphere_A.radius + sphere_B.radius)/2.0f;
}

void Bounding_Sphere::calculate_Bsphere_Spheres(Bounding_Sphere* objects[], int numObjects)
{
	std::vector<AABB*> temp_AABB_List;

	for (int i = 0; i < numObjects; i++)
	{
		AABB* tempBV = new AABB(objects[i]->center, objects[i]->radius);
		temp_AABB_List.push_back(tempBV);
	}

	AABB* complete_box = new AABB;

	complete_box->calculate_AABB(&temp_AABB_List[0], numObjects);

	center = complete_box->get_center();

	radius = glm::length(complete_box->get_center() - complete_box->aabb_getmax());

	for (int i = 0; i < temp_AABB_List.size(); i++)
	{
		delete temp_AABB_List[i];
	}
	delete complete_box;
}

void Bounding_Sphere::grow_radius(glm::vec3 initial_center, float initial_radius, std::vector<glm::vec3> vertices)
{
	glm::vec3 temp_center = initial_center;
	float temp_radius = initial_radius;
	for (int i = 0; i < vertices.size(); i++)
	{
		if (glm::length(temp_center - vertices[i]) >= temp_radius)
		{
			//adjust the bounding sphere
			glm::vec3 direction = (vertices[i] - temp_center);

			float new_radius = (temp_radius + glm::length(direction)) / 2.0f;
			

			temp_center = temp_center + ((new_radius - temp_radius) * glm::normalize(direction));
			temp_radius = new_radius;

			//i = 0;
		}
	}
	radius = temp_radius;
	center = temp_center;
}

Bounding_Volume* Bounding_Sphere::compute_bv(AABB bounding_box)
{
	calculate_Bsphere_AABB(bounding_box);
	volume = 3.1415f * radius * radius;
	return this;
}