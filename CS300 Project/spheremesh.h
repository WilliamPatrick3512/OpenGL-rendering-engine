/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: spheremesh.h
Purpose: creates a sphere mesh
Language: C++
Platform: visual studio 2019, windows 10
Project: william.patrick_CS300_1
Author: William Patrick, william.patrick, 280002517
Creation date: 10/14
End Header --------------------------------------------------------*/
#pragma once
#include<vector>
#include<string>
#include <glm/glm.hpp>
class spheremesh
{
	std::vector<glm::vec3> vertex_buffer;


public:
	std::vector<glm::vec3> geometry_buffer;
	std::vector<glm::vec3> geometry_normals;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> fragment_buffer;

	glm::vec3 center;
	float scale;
	int orbit_index;
	spheremesh(int size);

};

