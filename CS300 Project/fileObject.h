/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: fileObject.h
Purpose: Opens obj files and generates a mesh and normals for the object
Language: C++
Platform: visual studio 2019, windows 10
Project: william.patrick_CS300_1
Author: William Patrick, william.patrick, 280002517
Creation date: 9/17
End Header --------------------------------------------------------*/
#pragma once
#include<vector>
#include<string>
#include <glm/glm.hpp>
#include "normal.h"
#include "Bounding_Volume.h"
#include "AABB.h"
#include "Bounding_Sphere.h"

class fileObject
{

	
	float normal_scale;
	int geometry_border;
	int facenormal_border;
	
  public:
	  std::vector<glm::vec3> geometry_buffer;
	  std::vector<glm::vec3> geometry_normals;
	  std::vector<glm::vec3> vertex_buffer;
	  std::vector<glm::vec3> face_normals;
	  std::vector<glm::vec3> face_normals_vectors;
	  std::vector<glm::vec3> vert_normals;
	  std::vector<glm::vec3> vert_normals_vectors;
	  std::vector<glm::vec3> fragment_buffer;

	  std::vector<glm::vec2> UV_buffer;

	  Bounding_Sphere bounding_sphere;
	  AABB bounding_AABB;

	  glm::vec3 center;
	  float scale;

	  glm::vec3 minimum;
	  glm::vec3 maximum;

	  void setNormalScale(float scale);
	  void readfile(std::string file);
	  void readpptfile(std::string file);
	  void calculateScale();
	  void calculateNormals();
	  int getGeometryEnd();
	  int getFaceNormalEnd();
	  void calculateUV_CPU(int type);
	  void calculateBVs(enum Bounding_Sphere::BSphere_type type);

};

