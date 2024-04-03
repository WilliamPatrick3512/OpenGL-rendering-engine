#include "spheremesh.h"
/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: spheremesh.cpp
Purpose: creates a sphere mesh
Language: C++
Platform: visual studio 2019, windows 10
Project: william.patrick_CS300_1
Author: William Patrick, william.patrick, 280002517
Creation date: 10/14
End Header --------------------------------------------------------*/

#include <cmath>
using namespace std;


const float PI = 4.0f * atan(1.0f);


#define M (2*size)
#define N (size)
#define NORTH (M*(N-1))
#define SOUTH (M*(N-1)+1)
spheremesh::spheremesh(int size){
	for (int i = 1; i < N; ++i) {
		float theta = PI * i / N;
		for (int j = 0; j < M; ++j) {
			int index = M * (i - 1) + j;
			float phi = 2 * PI * j / M;
			normals.push_back(glm::vec3(0.0f,0.0f,0.0f));
			normals[index].x = sin(theta) * cos(phi);
			normals[index].y = sin(theta) * sin(phi);
			normals[index].z = cos(theta);
		}
	}
	normals.push_back( glm::vec3(0.0f, 0.0f, 1.0f));
	normals.push_back( glm::vec3(0.0f, 0.0f, -1.0f));

	for (unsigned n = 0; n < normals.size(); ++n) {
		vertex_buffer.push_back(normals[n]);
	}

	for (int i = 2; i < N; ++i) {
		for (int j = 0; j < M; ++j) {
			glm::vec3 face;
			int jp1 = (j + 1) % M;
			face.x = M * (i - 2) + j;
			face.y = M * (i - 1) + jp1;
			face.z = M * (i - 2) + jp1;
			fragment_buffer.push_back(face);
			face.y = M * (i - 1) + j;
			face.z = M * (i - 1) + jp1;
			fragment_buffer.push_back(face);
		}
	}
	for (int j = 0; j < M; ++j) {
		glm::vec3 face;
		int jp1 = (j + 1) % M;
		face.x = j;
		face.y = jp1;
		face.z = NORTH;
		fragment_buffer.push_back(face);
		face.x = M * (N - 2) + j;
		face.y = SOUTH;
		face.z = M * (N - 2) + jp1;
		fragment_buffer.push_back(face);
	}

	for (int i = 0; i < fragment_buffer.size(); i++)
	{
		geometry_buffer.push_back(glm::vec3(vertex_buffer[(fragment_buffer[i].x)]));
		geometry_buffer.push_back(glm::vec3(vertex_buffer[(fragment_buffer[i].y)]));

		geometry_buffer.push_back(glm::vec3(vertex_buffer[(fragment_buffer[i].x)]));
		geometry_buffer.push_back(glm::vec3(vertex_buffer[(fragment_buffer[i].z)]));

		geometry_buffer.push_back(glm::vec3(vertex_buffer[(fragment_buffer[i].z)]));
		geometry_buffer.push_back(glm::vec3(vertex_buffer[(fragment_buffer[i].y)]));

		geometry_normals.push_back(glm::vec3(normals[(fragment_buffer[i].x)]));
		geometry_normals.push_back(glm::vec3(normals[(fragment_buffer[i].y)]));
		geometry_normals.push_back(glm::vec3(normals[(fragment_buffer[i].z)]));
	}
	scale = 2.0f;
	orbit_index = geometry_buffer.size();
	for (int i = 0; i <= 360; i++)
	{
		float x1 = 20.0f * cos(((float)i) * 0.0174f);
		float y1 = 20.0f * sin(((float) i) * 0.0174f);

		float x2 = 20.0f * cos(((float)(i + 2)) * 0.0174f);
		float y2 = 20.0f * sin(((float)(i + 2)) * 0.0174f);

		geometry_buffer.push_back(glm::vec3(x1, y1, 0.0f));
		geometry_buffer.push_back(glm::vec3(x2, y2, 0.0f));
	
	}
	

}
#undef SOUTH
#undef NORTH
#undef N
#undef M

