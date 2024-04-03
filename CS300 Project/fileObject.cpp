/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: fileObject.cpp
Purpose: Opens obj files and generates a mesh and normals for the object
Language: C++
Platform: visual studio 2019, windows 10
Project: william.patrick_CS300_1
Author: William Patrick, william.patrick, 280002517
Creation date: 9/17
End Header --------------------------------------------------------*/
#include "fileObject.h"
#include <fstream>
#include <iostream>
void fileObject::readfile(std::string file)
{

	std::ifstream input_file(file);

	if (input_file)
	{
		char buffer[128];

		vertex_buffer.clear();
		geometry_buffer.clear();
		face_normals.clear();
		vert_normals.clear();
		face_normals_vectors.clear();
		vert_normals_vectors.clear();
		fragment_buffer.clear();
		geometry_normals.clear();

		center = glm::vec3(0.0f, 0.0f, 0.0f);
		while (input_file.getline(buffer, 128))
		{
			if (buffer[0] == 'v')
			{

				float vx = 0.0f;
				float vy = 0.0f;
				float vz = 0.0f;

				sscanf_s(buffer, "v %f %f %f", &vx, &vy, &vz);
				vertex_buffer.push_back(glm::vec3(vx, vy, vz));
				center += glm::vec3(vx, vy, vz);
			}
			else if (buffer[0] == 'f')
			{
				int fx = 0;
				int fy = 0;
				int fz = 0;
				int fw = INT_MIN;

				sscanf_s(buffer, "f %d %d %d %d", &fx, &fy, &fz, &fw);
				//sscanf_s(buffer, "f %d//%d %d//%d %d//%d %d//%d", &fx, &fx, &fy, &fy, &fz, &fz, &fw, &fw);
				fragment_buffer.push_back(glm::vec3(fx,fy,fz));
				geometry_buffer.push_back(glm::vec3(vertex_buffer[(fx - 1)]));
				geometry_buffer.push_back(glm::vec3(vertex_buffer[(fy - 1)]));
				geometry_buffer.push_back(glm::vec3(vertex_buffer[(fz - 1)]));

				if (fw != INT_MIN)
				{
					fragment_buffer.push_back(glm::vec3(fx, fz, fw));
					geometry_buffer.push_back(glm::vec3(vertex_buffer[(fx - 1)]));
					geometry_buffer.push_back(glm::vec3(vertex_buffer[(fz - 1)]));
					geometry_buffer.push_back(glm::vec3(vertex_buffer[(fw - 1)]));

				}

			}
		}

		center /= vertex_buffer.size();


		geometry_border = geometry_buffer.size();

		

	}
}

//reads in a list of files from a folder, used for power plant files
void fileObject::readpptfile(std::string file)
{

	std::ifstream input_file(file);

	if (input_file)
	{
		char buffer[128];

		vertex_buffer.clear();
		geometry_buffer.clear();
		face_normals.clear();
		vert_normals.clear();
		face_normals_vectors.clear();
		vert_normals_vectors.clear();
		fragment_buffer.clear();
		geometry_normals.clear();

		center = glm::vec3(0.0f, 0.0f, 0.0f);
		while (input_file.getline(buffer, 128))
		{
			if (buffer[0] == 'v' && buffer[1] != 'n')
			{

				float vx = 0.0f;
				float vy = 0.0f;
				float vz = 0.0f;

				sscanf_s(buffer, "v %f %f %f", &vx, &vy, &vz);
				vertex_buffer.push_back(glm::vec3(vx, vy, vz));
				center += glm::vec3(vx, vy, vz);
			}
			else if (buffer[0] == 'f')
			{
				int fx = 0;
				int fy = 0;
				int fz = 0;
				int fw = INT_MIN;

				
				sscanf_s(buffer, "f %d//%d %d//%d %d//%d %d//%d", &fx, &fx, &fy, &fy, &fz, &fz, &fw, &fw);
				fragment_buffer.push_back(glm::vec3(fx, fy, fz));
				geometry_buffer.push_back(glm::vec3(vertex_buffer[(fx - 1)]));
				geometry_buffer.push_back(glm::vec3(vertex_buffer[(fy - 1)]));
				geometry_buffer.push_back(glm::vec3(vertex_buffer[(fz - 1)]));

			}
		}

		center /= vertex_buffer.size();


		geometry_border = geometry_buffer.size();



	}
}

void fileObject::calculateScale()
{
	float result = 0.0f;
	glm::vec3 scales = glm::vec3(0.0f, 0.0f, 0.0f);

	minimum = center;
	maximum = center;

	for (int i = 0; i < vertex_buffer.size(); i++)
	{
		if (minimum.x > vertex_buffer[i].x)
		{
			minimum.x = vertex_buffer[i].x;
		}
		else if (maximum.x < vertex_buffer[i].x)
		{
			maximum.x = vertex_buffer[i].x;
		}

		if (minimum.y > vertex_buffer[i].y)
		{
			minimum.y = vertex_buffer[i].y;
		}
		else if (maximum.y < vertex_buffer[i].y)
		{
			maximum.y = vertex_buffer[i].y;
		}

		if (minimum.z > vertex_buffer[i].z)
		{
			minimum.z = vertex_buffer[i].z;
		}
		else if (maximum.z < vertex_buffer[i].z)
		{
			maximum.z = vertex_buffer[i].z;
		}

	}

	scales = maximum - minimum;

	if (scales.x >= scales.y&& scales.x >= scales.z)
	{
		result = scales.x;
	}
	else if (scales.y >= scales.x && scales.y >= scales.z)
	{
		result = scales.y;
	}
	else if (scales.z >= scales.y && scales.z >= scales.x)
	{
		result = scales.z;
	}

	scale = result;

}
void fileObject::calculateNormals()
{
	
	for (int i = 0; i < fragment_buffer.size(); i++)
	{
		glm::vec3 position = (vertex_buffer[(int)fragment_buffer[i].x - 1] + vertex_buffer[(int)fragment_buffer[i].y - 1] + vertex_buffer[(int)fragment_buffer[i].z - 1]) / 3.0f;

		glm::vec3 A = (vertex_buffer[(int)fragment_buffer[i].y - 1] - vertex_buffer[(int)fragment_buffer[i].x - 1]);
		glm::vec3 B = (vertex_buffer[(int)fragment_buffer[i].y - 1] - vertex_buffer[(int)fragment_buffer[i].z - 1]);


		glm::vec3 direction = glm::normalize(glm::cross(B,A));


		if (abs(direction.x) > 0.0f && abs(direction.x) < 0.0001f)
		{
			direction.x = 0.0f;
		}
		if (abs(direction.y) > 0.0f && abs(direction.y) < 0.0001f)
		{
			direction.y = 0.0f;
		}
		if (abs(direction.z) > 0.0f && abs(direction.z) < 0.0001f)
		{
			direction.z = 0.0f;
		}

		face_normals_vectors.push_back(direction);
		face_normals.push_back(position);
		face_normals.push_back(position + (direction * normal_scale));
	}

	facenormal_border = geometry_buffer.size() + face_normals.size();

	for (int i = 0; i < face_normals.size(); i++)
	{
		geometry_buffer.push_back(face_normals[i]);
	}



	for (int i = 0; i < vertex_buffer.size(); i++)
	{
		glm::vec3 position = vertex_buffer[i];
		glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);
	
		std::vector<glm::vec3> duplicate_check;

		for (int j = 0; j < fragment_buffer.size(); j++)
		{
			if (fragment_buffer[j].x - 1 == i ||
				fragment_buffer[j].y - 1 == i ||
				fragment_buffer[j].z - 1 == i)
			{
				bool is_duplicate = false;
				for (int k = 0; k < duplicate_check.size(); k++)
				{
					if (face_normals_vectors[j] == duplicate_check[k])
					{
						is_duplicate = true;
					}
				}
				if (!is_duplicate)
				{
					duplicate_check.push_back(face_normals_vectors[j]);
					direction += face_normals_vectors[j];
				}
			}

		}
		direction = glm::normalize(direction);
		vert_normals_vectors.push_back(direction);
		vert_normals.push_back(position);
		vert_normals.push_back(position + (direction * normal_scale));
	}

	for (int i = 0; i < vert_normals.size(); i++)
	{
		geometry_buffer.push_back(vert_normals[i]);
	}
	for (int i = 0; i < fragment_buffer.size(); i++)
	{
		geometry_normals.push_back(vert_normals_vectors[fragment_buffer[i].x - 1]);
		geometry_normals.push_back(vert_normals_vectors[fragment_buffer[i].y - 1]);
		geometry_normals.push_back(vert_normals_vectors[fragment_buffer[i].z - 1]);
	}
	
}

void fileObject::setNormalScale(float scale)
{
	normal_scale = scale;
}

int fileObject::getGeometryEnd()
{
	return geometry_border;
} 

int fileObject::getFaceNormalEnd()
{
	return facenormal_border;
}

void fileObject::calculateUV_CPU(int type)
{
	float u = 0.0f;
	float v = 0.0f;
	glm::vec3 bb_minimum = minimum - center;
	glm::vec3 bb_maximum = maximum - center;

	UV_buffer.clear();
	for (int i = 0; i < getGeometryEnd(); i++)
	{
		glm::vec3 point = geometry_buffer[i] - center;
		
		//planar for testing
		if (type == 0)
		{

			u = (point.x + 1.0f)/2.0f;
			v = (point.y + 1.0f)/2.0f;
		}

		// sphere
		else if (type == 1)
		{

			float radius = sqrtf(pow((bb_maximum.x - bb_minimum.x), 2.0f) + pow((bb_maximum.y - bb_minimum.y), 2.0f) + pow((bb_maximum.z - bb_minimum.z), 2.0f));
			float theta = glm::degrees(atanf(point.z / point.x));
			float phi = glm::degrees(acosf(point.y / radius));
			//float phi = acosf(point.y / point.length());

			u = (theta + 180.0f) / 360.0f;
			v = 0.5f - (phi/45.0f);
		}
		//cylinder
		else if (type == 2)
		{

			//split into the 6 faces
			//calculate uv on that face
		//float theta = atanf(point.z / point.x);
		float theta = glm::degrees(atanf(point.z / point.x));
		
			u = (theta + 180.0f) / 360.0f;
			v = (point.y + 1.0f) / 2.0f;
		}

		glm::vec2 result = glm::vec2(u, v);

		UV_buffer.push_back(result);
	}
}

void fileObject::calculateBVs(enum Bounding_Sphere::BSphere_type type)
{
	bounding_AABB.compute_bv(vertex_buffer);
	bounding_sphere.compute_bv(vertex_buffer, type);
}