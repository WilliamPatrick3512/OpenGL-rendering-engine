//
// Created by pushpak on 6/1/18.
//


#include "SimpleScene_Quad.h"
#include <shader.hpp>
#include <glm/vec3.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <../CS300 Project/cs300gui.h>

const glm::vec3 O(0, 0, 0),
EX(1, 0, 0),
EY(0, 1, 0),
EZ(0, 0, 1);
glm::vec3 DK_GRAY(0.1f, 0.1f, 0.1f);

const glm::vec3 LIGHT_COLOR(1, 1, 1),
AMBIENT_COLOR(0.2f, 0.2f, 0.2f);

float camera_pan_speed;
float camera_rotate_speed = 0.05f;

float specularExponent = 10.5;

float* guiCameraRotation;

int width = 512;
int height = 512;
int layers = 1;
float quadVertices[] = {
	// positions        // texture Coords
	-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
	 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
};

float Bounding_cubeVertices[] = {
	// positions        
	 0.5f,  0.5f, 0.5f,
	 0.5f,  0.5f,-0.5f,

	 0.5f,  0.5f, 0.5f,
	 0.5f, -0.5f, 0.5f,

	 0.5f,  0.5f, 0.5f,
	-0.5f,  0.5f, 0.5f,

	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,

	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,

	 0.5f,  -0.5f,  0.5f,
	 0.5f,  -0.5f, -0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,

	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,

};

glm::vec3 debugcolors[] = {		glm::vec3(1.0f,0.0f,0.0f),
								glm::vec3(1.0f,0.5f,0.0f),
								glm::vec3(1.0f,1.0f,0.0f),
								glm::vec3(0.0f,1.0f,0.0f),
								glm::vec3(0.0f,1.0f,1.0f),
								glm::vec3(0.0f,0.0f,1.0f),
								glm::vec3(0.5f,0.0f,1.0f),
								glm::vec3(1.0f,0.0f,1.0f),
};

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
void SimpleScene_Quad::SetupNanoGUI(GLFWwindow *pWindow)
{
    pWindow = nullptr;
}


//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
SimpleScene_Quad::~SimpleScene_Quad()
{
    initMembers();
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
SimpleScene_Quad::SimpleScene_Quad(int windowWidth, int windowHeight) : Scene(windowWidth, windowHeight),
                                                                        programID(0), vertexbuffer(0),
                                                                        angleOfRotation(0.0f), spherebuffer(0), spheres(10)
{
    initMembers();
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-unused-return-value"
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
void SimpleScene_Quad::initMembers()
{
    programID = 0;
    vertexbuffer = 0;
	spherebuffer = 0; 

	light_count = 1;


	Floor.empty();
    file_object.geometry_buffer.empty();
    file_object2.geometry_buffer.empty();
    angleOfRotation = 0.0f;

	Scene_camera = Camera();
	Scene_camera.forward(-1.0f);
	Scene_camera.lift(-0.125f);

	
}
#pragma clang diagnostic pop

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
void SimpleScene_Quad::CleanUp()
{
    // Cleanup VBO
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteBuffers(1, &spherebuffer);
    glDeleteBuffers(1, &floorbuffer);
    glDeleteBuffers(1, &normal_buffer);
    glDeleteBuffers(1, &sphere_normal_buffer);
    glDeleteBuffers(1, &floor_normal_buffer);

	glDeleteTextures(1, &texture_handle);
	glDeleteTextures(1, &gPosition);
	glDeleteTextures(1, &gNormal);
	glDeleteTextures(1, &gAlbedoSpec);

	glDeleteRenderbuffers(1, &rboDepth);

    glDeleteProgram(programID);
    glDeleteProgram(Shader_GeometryPass);
    glDeleteProgram(Shader_LightingPass);
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
void SimpleScene_Quad::SetupBuffers()
{

	//geometry buffer for deffered shading
	glGenFramebuffers(1, &gbuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gbuffer);

	//buffer generation, labeled below when bound

	glGenVertexArrays(1, &fileobjectVAO);
	glGenVertexArrays(1, &floorVAO);
	glGenVertexArrays(1, &light_sphereVAO);
	glGenVertexArrays(1, &Bounding_CubeVAO);
	glGenVertexArrays(1, &octreeVAO);
	



	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &normal_buffer);
    glGenBuffers(1, &uvbuffer);

    glGenBuffers(1, &spherebuffer);
	glGenBuffers(1, &sphere_normal_buffer);



	glGenBuffers(1, &floorbuffer);
	glGenBuffers(1, &floor_normal_buffer);

	glGenBuffers(1, &Bounding_CubeVBO);
	glGenBuffers(1, &octreeVBO);


	glBindVertexArray(fileobjectVAO);
	//buffer for vertices of file object
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, file_object.geometry_buffer.size() * sizeof(glm::vec3),
                 file_object.geometry_buffer.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	//buffer to store file object normal vectors
	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * file_object.geometry_normals.size(),
		file_object.geometry_normals.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(1);

	//buffer to store the UV info for each vertex in the file object
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, file_object.UV_buffer.size() * sizeof(glm::vec2),
		file_object.UV_buffer.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(2);



	glBindVertexArray(floorVAO);
	//buffer for the floor object
	glBindBuffer(GL_ARRAY_BUFFER, floorbuffer);
	glBufferData(GL_ARRAY_BUFFER, Floor.size() * sizeof(GLfloat),
		Floor.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	//buffer for floor normals
	glBindBuffer(GL_ARRAY_BUFFER, floor_normal_buffer);
	glBufferData(GL_ARRAY_BUFFER, floor_normals.size() * sizeof(GLfloat),
		floor_normals.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(1);

	// setup plane VAO
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//setup light VAO
	glBindVertexArray(light_sphereVAO);
	//buffer for the generated debug spheres/light sources
	glBindBuffer(GL_ARRAY_BUFFER, spherebuffer);
	glBufferData(GL_ARRAY_BUFFER, spheres.geometry_buffer.size() * sizeof(glm::vec3),
		spheres.geometry_buffer.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);


	//setup bounding cube vao
	glBindVertexArray(Bounding_CubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, Bounding_CubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Bounding_cubeVertices), &Bounding_cubeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);


	glBindVertexArray(octreeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, octreeVBO);
	glBufferData(GL_ARRAY_BUFFER, spheres.geometry_buffer.size() * sizeof(glm::vec3),
		spheres.geometry_buffer.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);


	//create texture object for the file object
	glGenTextures(1, &texture_handle);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	texture.loadTextureFromFile("../Common/textures/metal_roof_diff_512x512.png", &texture_handle, &width, &height, &layers);
	


	// position color buffer
	glGenTextures(1, &gPosition);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, _windowWidth, _windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	// normal color buffer
	glGenTextures(1, &gNormal);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, _windowWidth, _windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	// color + specular color buffer
	glGenTextures(1, &gAlbedoSpec);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _windowWidth, _windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);


	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	attachments[0] = GL_COLOR_ATTACHMENT0;
	attachments[1] = GL_COLOR_ATTACHMENT1;
	attachments[2] = GL_COLOR_ATTACHMENT2;
	glDrawBuffers(3, attachments);
	
	//set up depth buffer
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _windowWidth, _windowHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("buffer not complete");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
int SimpleScene_Quad::Init()
{
	
	LoadShader(0);
	Floor = { -1.0f, -1.0f, 0.0f,
						1.0f, -1.0f, 0.0f,
						-1.0f, 1.0f, 0.0f,
						1.0f, -1.0f, 0.0f,
						1.0f, 1.0f, 0.0f,
						-1.0f, 1.0f, 0.0f
	};

	floor_normals = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f

	};
	powerplant_objects.clear();

	
	file_object.readfile("../Common/models/CS350_Models/sphere.obj");
	file_object.calculateScale();
	file_object.calculateUV_CPU(0);
	file_object.setNormalScale(1.0f );
	file_object.calculateNormals();
	file_object.calculateBVs((Bounding_Sphere::BSphere_type) getBVSphereType());

	powerplant_objects.push_back(file_object);

	std::ifstream input_files("../Common/models/section4.txt");

	if (input_files)
	{
		char buffer[128];
		while (input_files.getline(buffer, 128))
		{
			fileObject object;
			object.readpptfile(buffer);
			object.calculateScale();
			object.calculateUV_CPU(0);
			object.setNormalScale(1.0f);
			object.calculateNormals();
			object.calculateBVs((Bounding_Sphere::BSphere_type) getBVSphereType());

			section4.push_back(object);
		}
	}
	
	std::ifstream input_files2("../Common/models/section5.txt");

	if (input_files2)
	{
		char buffer[128];
		while (input_files2.getline(buffer, 128))
		{
			fileObject object;
			object.readpptfile(buffer);
			object.calculateScale();
			object.calculateUV_CPU(0);
			object.setNormalScale(1.0f);
			object.calculateNormals();
			object.calculateBVs((Bounding_Sphere::BSphere_type) getBVSphereType());

			section5.push_back(object);
		}
	}

	std::ifstream input_files3("../Common/models/section6.txt");

	if (input_files3)
	{
		char buffer[128];
		while (input_files3.getline(buffer, 128))
		{
			fileObject object;
			object.readpptfile(buffer);
			object.calculateScale();
			object.calculateUV_CPU(0);
			object.setNormalScale(1.0f);
			object.calculateNormals();
			object.calculateBVs((Bounding_Sphere::BSphere_type) getBVSphereType());

			section6.push_back(object);
		}
	}
	

	construct_display_list();
	create_octree();
	create_bvtree();
	

	//floor properties
	properties[1].ambient_color = glm::vec3(0.05f, 0.05f, 0.05f);
	properties[1].diffuse_color = glm::vec3(0.25f, 0.25f, 0.25f);
	properties[1].specular_color = glm::vec3(0.0f, 0.0f, 0.0f);
	properties[1].emmissive_color = glm::vec3(0.0f, 0.0f, 0.0f);


    SetupBuffers();
	for (int i = 0; i < 16; i++)
	{
		Lights newlight(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f);
		scene_lights[i] = (newlight);
	}
	light_count = 1;

    return Scene::Init();
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
int SimpleScene_Quad::Render()
{
    glClearColor( 0.f, 0.f, 0.f, 1.0f );
	glEnable(GL_DEPTH_TEST);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


    glEnableVertexAttribArray(0);

	Render_Deffered();

	if(get_rendertype() == 0)
		Render_Forward();

	if (hasModelChanged())
	{
		SwapFile(giveCurrentItem());
	}

	if (hasShaderChanged())
	{
		LoadShader(giveCurrentShader());
	}


    glDisableVertexAttribArray(0);

    return 0;
}

int SimpleScene_Quad::Render_Forward()
{
	// Uniform matrix
	// Uniform transformation (vertex shader)
	glUseProgram(Shader_Debug);
	GLint vTransformLoc = glGetUniformLocation(Shader_Debug, "vertexTransform");

	glm::mat4 persp = glm::perspective(glm::radians(80.f), 1024.f / 768.f, 0.1f, 1000000.f);
	glm::mat4 view = model(Scene_camera);
	GLint vPerspLoc = glGetUniformLocation(Shader_Debug, "persp");
	GLint vViewLoc = glGetUniformLocation(Shader_Debug, "view");

	glUniformMatrix4fv(vPerspLoc, 1, GL_FALSE, glm::value_ptr(persp));
	glUniformMatrix4fv(vViewLoc, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 modelMat = glm::mat4(1.0f);
	
	glm::vec3 debug_color = *getSurfaceEmmissive();

	glUniform3fv(glGetUniformLocation(Shader_Debug, "debugcolor"), 1, &debug_color[0]);
	glUniformMatrix4fv(vTransformLoc, 1, GL_FALSE, &modelMat[0][0]);


	//draw file object normals
	if (drawVertNormals())
	{
		glLineWidth(1.0f);
		glBindVertexArray(fileobjectVAO);
		glDrawArrays(GL_LINES, file_object.getFaceNormalEnd(), file_object.geometry_buffer.size());
		glBindVertexArray(0);

	}
	if (drawFragNormals())
	{
		glLineWidth(1.0f);
		glBindVertexArray(fileobjectVAO);
		glDrawArrays(GL_LINES, file_object.getGeometryEnd(), file_object.face_normals.size());
		glBindVertexArray(0);

	}

	glm::vec3 scaleVector;
	glm::vec3 centroid;
	modelMat = glm::mat4(1.0f);

	glm::mat4 objectmodelMat = glm::mat4(1.0f);
	glm::vec3 objectscaleVector = glm::vec3(1.0f / (2.5 * file_object.scale));
	glm::vec3 objectcentroid = glm::vec3(-file_object.center);
	objectmodelMat = glm::scale(objectscaleVector) *
		glm::translate(objectcentroid);

	glUniform3fv(glGetUniformLocation(Shader_Debug, "debugcolor"), 1, &(debugcolors[0])[0]);
	//gui_display_depth
		if (!getBVSphere())
		{

			scaleVector = glm::vec3((file_object.bounding_AABB.aabb_getmax() - file_object.bounding_AABB.aabb_getmin()));
			centroid = file_object.bounding_AABB.get_center();
			modelMat = objectmodelMat * glm::translate(centroid) * glm::scale(scaleVector);


			glUniformMatrix4fv(glGetUniformLocation(Shader_Debug, "vertexTransform"), 1, GL_FALSE, &modelMat[0][0]);

			glLineWidth(1.0f);
			glBindVertexArray(Bounding_CubeVAO);
			glBindBuffer(GL_ARRAY_BUFFER, Bounding_CubeVBO);
			glVertexAttribPointer(aposition, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glEnableVertexAttribArray(aposition);
			glDrawArrays(GL_LINES, 0, 24);
			glBindVertexArray(0);
		}
		//draw sphere
		else
		{
			scaleVector = glm::vec3(file_object.bounding_sphere.radius);
			centroid = file_object.bounding_sphere.get_center();
			modelMat = objectmodelMat * glm::translate(centroid) * glm::scale(scaleVector);

			glUniformMatrix4fv(glGetUniformLocation(Shader_Debug, "vertexTransform"), 1, GL_FALSE, &modelMat[0][0]);

			glBindVertexArray(light_sphereVAO);
			glDrawArrays(GL_LINES, 0, spheres.orbit_index);
			glBindVertexArray(0);
		}



	if (getBVdraw() && powerplant_objects.size() != 0)
	{
		renderBVTree(&bvTree_root);
	}
	
	if (getOctreedraw() && powerplant_objects.size() != 0)
	{
		render_octree(scene_octree.root);
	}
		
	glBindVertexArray(light_sphereVAO);
	glBindBuffer(GL_ARRAY_BUFFER, spherebuffer);
	glVertexAttribPointer(aposition, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(aposition);

	// T * R * S * Vertex
	glm::mat4 sphereMat = glm::mat4(1.0f);
	glm::vec3 spherescaleVector = glm::vec3(1.0f / (15.0f * spheres.scale));
	glm::vec3 spherecentroid = glm::vec3(0.0f, 20.0f, 0.0f);

	for (int i = 0; i < light_count; i++)
	{
		if (stopOrbit())
		{
			sphereMat = glm::rotate(scene_lights[i].rotation, glm::vec3(0.0f, 1.0f, 0.0f)) *
				glm::rotate(1.57f, glm::vec3(1.0f, 0.0f, 0.0f)) *
				glm::scale(spherescaleVector) *
				glm::translate(spherecentroid);
		}
		else
		{
			sphereMat = glm::rotate(angleOfRotation + scene_lights[i].rotation, glm::vec3(0.0f, 1.0f, 0.0f)) *
				glm::rotate(1.57f, glm::vec3(1.0f, 0.0f, 0.0f)) *
				glm::scale(spherescaleVector) *
				glm::translate(spherecentroid);

		}

		glUniformMatrix4fv(vTransformLoc, 1, GL_FALSE, &sphereMat[0][0]);

		glUniform3fv(glGetUniformLocation(Shader_Debug, "debugcolor"), 1, &(scene_lights[i].light_diffuse[0]));

		glBindVertexArray(light_sphereVAO);
		glDrawArrays(GL_LINES, 0, spheres.orbit_index);
		glBindVertexArray(0);
	}
	sphereMat = glm::mat4(1.0f) * glm::rotate(1.57f, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::scale(spherescaleVector);

	debug_color = glm::vec3(1.0f);
	glUniform3fv(glGetUniformLocation(Shader_Debug, "debugcolor"), 1, &debug_color[0]);

	glUniformMatrix4fv(vTransformLoc, 1, GL_FALSE, &sphereMat[0][0]);

	glBindVertexArray(light_sphereVAO);
	glDrawArrays(GL_LINES, spheres.orbit_index + 2, spheres.geometry_buffer.size());
	glBindVertexArray(0);
	return 0;
	
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
int SimpleScene_Quad::Render_Deffered()
{

	glBindFramebuffer(GL_FRAMEBUFFER, gbuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use first shader to get info 
	glUseProgram(Shader_GeometryPass);
	guiCameraRotation = getCameraRotation();

	aposition = glGetAttribLocation(Shader_GeometryPass, "aPosition");
	anormal = glGetAttribLocation(Shader_GeometryPass, "aNormal");
	aCPUuv = glGetAttribLocation(Shader_GeometryPass, "aCPU_uv");


	glm::mat4 persp = glm::perspective(glm::radians(80.f), 1024.f / 768.f, 0.1f, 1000000.f);
	glm::mat4 view = model(Scene_camera);
	GLint vPerspLoc = glGetUniformLocation(Shader_GeometryPass, "persp");
	GLint vViewLoc = glGetUniformLocation(Shader_GeometryPass, "view");

	glBindBuffer(GL_ARRAY_BUFFER, floorbuffer);
	glVertexAttribPointer(aposition, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(aposition);

	glBindBuffer(GL_ARRAY_BUFFER, floor_normal_buffer);
	glVertexAttribPointer(anormal, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(anormal);

	// Uniform sampler for brick texture for object
	//GLint texSamplerLoc = glGetUniformLocation(Shader_GeometryPass, "texture_diffuse");
	//glUniform1i(texSamplerLoc, 0);

	// Uniform matrix
	// Uniform transformation (vertex shader)
	GLint vfloorTransformLoc = glGetUniformLocation(Shader_GeometryPass, "vertexTransform");
	GLint vfloorNormalLoc = glGetUniformLocation(Shader_GeometryPass, "normalTransform");

	// Draw the triangle !
	// T * R * S * Vertex
	glm::mat4 floorMat = glm::mat4(1.0f);
	glm::vec3 floorscaleVector = glm::vec3(4.0f);
	glm::vec3 floorcentroid = glm::vec3(0.f, 0.f, -.1f);
	floorMat = glm::rotate(-3.14f / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::scale(floorscaleVector) *
		glm::translate(floorcentroid);

	glm::mat4 floornormalMat = glm::inverseTranspose(floorMat);

	glUniformMatrix4fv(vfloorTransformLoc, 1, GL_FALSE, &floorMat[0][0]);
	glUniformMatrix4fv(vfloorNormalLoc, 1, GL_FALSE, &floornormalMat[0][0]);
	glUniformMatrix4fv(vPerspLoc, 1, GL_FALSE, glm::value_ptr(persp));
	glUniformMatrix4fv(vViewLoc, 1, GL_FALSE, glm::value_ptr(view));




	//draw the floor
	glBindVertexArray(floorVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	
	/**************************************************************/
	// BEGIN DRAWING FILE OBJECT
	/**************************************************************/
	



	//bind the vertex array
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(aposition, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(aposition);

	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
	glVertexAttribPointer(anormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(anormal);

	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(aCPUuv, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(aCPUuv);


	// Uniform matrix
	// Uniform transformation (buffer vertex shader)
	GLint vTransformLoc = glGetUniformLocation(Shader_GeometryPass, "vertexTransform");
	GLint vNormalLoc = glGetUniformLocation(Shader_GeometryPass, "normalTransform");


	// Draw the triangle 
	// T * R * S * Vertex
	glm::mat4 modelMat = glm::mat4(1.0f);
	glm::vec3 scaleVector = glm::vec3(1.0f / (2.5 * file_object.scale));
	glm::vec3 centroid = glm::vec3(-file_object.center);
	modelMat = glm::scale(scaleVector) *
		glm::translate(centroid);
	glm::mat4 normalMat = glm::inverseTranspose(modelMat);


	glUniformMatrix4fv(vTransformLoc, 1, GL_FALSE, &modelMat[0][0]);
	glUniformMatrix4fv(vNormalLoc, 1, GL_FALSE, &normalMat[0][0]);


	// brick texture
	glActiveTexture(GL_TEXTURE1);
	glUniform1i(glGetUniformLocation(Shader_GeometryPass, "texture_diffuse"), 1);
	glBindTexture(GL_TEXTURE_2D, texture_handle);


	glBindVertexArray(fileobjectVAO);
	//buffer for vertices of file object
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, file_object.geometry_buffer.size() * sizeof(glm::vec3),
		file_object.geometry_buffer.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	//buffer to store file object normal vectors
	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* file_object.geometry_normals.size(),
		file_object.geometry_normals.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(1);

	//buffer to store the UV info for each vertex in the file object
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, file_object.UV_buffer.size() * sizeof(glm::vec2),
		file_object.UV_buffer.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(2);


	glDrawArrays(GL_TRIANGLES, 0, file_object.getGeometryEnd());
	glBindVertexArray(0);


	modelMat = glm::mat4(1.0f);
	normalMat = glm::inverseTranspose(modelMat);
	glUniformMatrix4fv(vTransformLoc, 1, GL_FALSE, &modelMat[0][0]);
	glUniformMatrix4fv(vNormalLoc, 1, GL_FALSE, &normalMat[0][0]);
	
	if (!get_camera_setting())
	{
		for (int i = 0; i < powerplant_objects.size(); i++)
		{
			glBindVertexArray(fileobjectVAO);
			//buffer for vertices of file object
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glBufferData(GL_ARRAY_BUFFER, powerplant_objects[i].geometry_buffer.size() * sizeof(glm::vec3),
				powerplant_objects[i].geometry_buffer.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
			glEnableVertexAttribArray(0);

			//buffer to store file object normal vectors
			glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * powerplant_objects[i].geometry_normals.size(),
				powerplant_objects[i].geometry_normals.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
			glEnableVertexAttribArray(1);

			//buffer to store the UV info for each vertex in the file object
			glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
			glBufferData(GL_ARRAY_BUFFER, powerplant_objects[i].UV_buffer.size() * sizeof(glm::vec2),
				powerplant_objects[i].UV_buffer.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
			glEnableVertexAttribArray(2);


			glDrawArrays(GL_TRIANGLES, 0, powerplant_objects[i].getGeometryEnd());

			glBindVertexArray(0);
		}
	}

	// 2. lighting pass: calculate lighting by iterating over a screen filled quad pixel-by-pixel using the gbuffer's content.
	// -----------------------------------------------------------------------------------------------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, 0);




	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(Shader_LightingPass);

	glUniform1i(glGetUniformLocation(Shader_LightingPass, "gPosition"), 2);
	glUniform1i(glGetUniformLocation(Shader_LightingPass, "gNormal"), 3);
	glUniform1i(glGetUniformLocation(Shader_LightingPass, "gAlbedoSpec"), 4);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);

	

	//send light data
	ulight_count = glGetUniformLocation(Shader_LightingPass, "shader_light_count");
	glUniform1i(ulight_count, light_count);

	glm::vec3 positions[16];
	glm::vec3 directions[16];
	glm::vec3 diffuse[16];
	glm::vec3 ambients[16];
	glm::vec3 emmissive[16];
	glm::vec3 specular[16];
	int types[16];
	float spotlight_angles_inner[16];
	float spotlight_angles_outer[16];
	float falloff[16];

	properties[0].ambient_color = *getSurfaceAmbient();
	properties[0].diffuse_color = *getSurfaceDiffuse();
	properties[0].specular_color = *getSurfaceSpecular();
	properties[0].emmissive_color = *getSurfaceEmmissive();

	//set light information for lights
	for (int i = 0; i < light_count; i++) {
		positions[i] = scene_lights[i].light_position;
		directions[i] = scene_lights[i].dlight_direction;
		types[i] = scene_lights[i].light_type;
		spotlight_angles_inner[i] = scene_lights[i].inner_angle;
		spotlight_angles_outer[i] = scene_lights[i].outer_angle;
		diffuse[i] = scene_lights[i].light_diffuse;
		ambients[i] = scene_lights[i].ambient_color;
		emmissive[i] = scene_lights[i].light_emmissive;
		specular[i] = scene_lights[i].light_specular;
		falloff[i] = scene_lights[i].falloff_value;
	}

	
	//get locations for light properties
	uattenuation_constants = glGetUniformLocation(Shader_LightingPass, "attenuation_constants");
	uglobal_ambient = glGetUniformLocation(Shader_LightingPass, "global_ambient");
	ufog_color = glGetUniformLocation(Shader_LightingPass, "fog_color");
	ufog_distances = glGetUniformLocation(Shader_LightingPass, "fog_distances");
	ulight_position = glGetUniformLocation(Shader_LightingPass, "light_pos");
	uligt_direction = glGetUniformLocation(Shader_LightingPass, "dlight_directions");
	ulight_types = glGetUniformLocation(Shader_LightingPass, "light_types");
	ulight_angles_inner = glGetUniformLocation(Shader_LightingPass, "spotlight_angles_inner");
	ulight_angles_outer = glGetUniformLocation(Shader_LightingPass, "spotlight_angles_outer");
	ulight_falloff = glGetUniformLocation(Shader_LightingPass, "falloff_value");


	//send light properties
	glUniform3fv(ulight_position, light_count, &positions[0][0]);
	glUniform3fv(uligt_direction, light_count, &directions[0][0]);
	glUniform1iv(ulight_types, light_count, &types[0]);
	glUniform1fv(ulight_angles_inner, light_count, &spotlight_angles_inner[0]);
	glUniform1fv(ulight_angles_outer, light_count, &spotlight_angles_outer[0]);
	glUniform3fv(udiffuse_color, light_count, &diffuse[0][0]);
	glUniform3fv(uambient_color, light_count, &ambients[0][0]);
	glUniform3fv(uspecular_color, light_count, &specular[0][0]);
	glUniform3fv(uemmissive_color, light_count, &emmissive[0][0]);
	glUniform1fv(ulight_falloff, light_count, &falloff[0]);

	glUniform3fv(uattenuation_constants, 1, &AttenuationConstants[0]);
	glUniform3fv(uglobal_ambient, 1, &global_ambient[0]);
	glUniform3fv(ufog_color, 1, &fogColor[0]);
	glUniform2fv(ufog_distances, 1, &fogDistances[0]);

	glUniform1i(glGetUniformLocation(Shader_LightingPass, "render_type"), get_rendertype());

	glm::vec4 eyeposition = Scene_camera.eye();
	glm::vec3 camera_position = glm::vec3(eyeposition.x,eyeposition.y,eyeposition.z);
	glUniform3fv(glGetUniformLocation(Shader_LightingPass, "camera_pos"), 1, &camera_position[0]);

	renderQuad();

	glBindFramebuffer(GL_READ_FRAMEBUFFER, gbuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
	// blit to default framebuffer.
	glBlitFramebuffer(0, 0, _windowWidth, _windowHeight, 0, 0, _windowWidth, _windowHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	return 0;
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
int SimpleScene_Quad::postRender()
{
	light_count = *getLightCount();

	//calculate the light positions for the next frame
	glm::mat4 sphereMat = glm::mat4(1.0f);
	glm::vec3 spherescaleVector = glm::vec3(1.0f / (15.0f * spheres.scale));
	glm::vec3 spherecentroid = glm::vec3(0.0f, 20.0f, 0.0f);

	for (int i = 0; i < light_count; i++)
	{
		if (stopOrbit())
		{
			sphereMat = glm::rotate(scene_lights[i].rotation, glm::vec3(0.0f, 1.0f, 0.0f)) *
				glm::rotate(1.57f, glm::vec3(1.0f, 0.0f, 0.0f)) *
				glm::scale(spherescaleVector) *
				glm::translate(spherecentroid);
		}
		else
		{
			sphereMat = glm::rotate( angleOfRotation + scene_lights[i].rotation, glm::vec3(0.0f, 1.0f, 0.0f)) *
				glm::rotate(1.57f, glm::vec3(1.0f, 0.0f, 0.0f)) *
				glm::scale(spherescaleVector) *
				glm::translate(spherecentroid);

		}


		glm::vec4 position = sphereMat * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		scene_lights[i].light_position.x = position.x;
		scene_lights[i].light_position.y = position.y;
		scene_lights[i].light_position.z = position.z;

	}
	
	float* guiLightRotations = getLightRotations();
	float* guiLightInnerAngles = getLightAngles_inner();
	float* guiLightOuterAngles = getLightAngles_outer();
	
	int* guiLightTypes = getLightTypes();
	glm::vec3* guiLightDiffuse = getLightDiffuse();
	glm::vec3* guiLightAmbient = getLightAmbient();
	glm::vec3* guiLightSpecular = getLightSpecular();
	float* guiFalloff = getLightFalloff();

	float* guiAttenuation(getAttenuationConstants());
	glm::vec3* guiGlobalAmbient = getGlobalAmbientColor();
	glm::vec3* guiFogColor = getFogColor();
	float* guiFogDistances(getFogDistances());

	fogColor = *guiFogColor;
	global_ambient = *guiGlobalAmbient;
	fogDistances = glm::vec2(guiFogDistances[0], guiFogDistances[1]);
	AttenuationConstants = glm::vec3(guiAttenuation[0], guiAttenuation[1], guiAttenuation[2]);


	//set all imgui stuff for lights
	for (int i = 0; i < light_count; i++)
	{
		scene_lights[i].rotation = glm::radians(guiLightRotations[i]);
		scene_lights[i].light_type = guiLightTypes[i];
		scene_lights[i].light_diffuse = guiLightDiffuse[i];
		scene_lights[i].ambient_color = guiLightAmbient[i];
		scene_lights[i].light_specular = guiLightSpecular[i];
		scene_lights[i].inner_angle = glm::radians(guiLightInnerAngles[i]) - 3.14f / 2.0f;
		scene_lights[i].outer_angle = glm::radians(guiLightOuterAngles[i]) - 3.14f / 2.0f;
		scene_lights[i].dlight_direction = (scene_lights[i].light_position - glm::vec3(0.0f));
		scene_lights[i].falloff_value = guiFalloff[i];
	}

	if (hasUVChanged())
	{
		file_object.calculateUV_CPU(*getUVCalculationType());
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, file_object.UV_buffer.size() * sizeof(glm::vec2),
			file_object.UV_buffer.data(), GL_STATIC_DRAW);
	}

	angleOfRotation += gui_getdt() / 1000.0f;


	if (hasTextureChanged())
	{
		if(drawUVGrid())
			texture.loadTextureFromFile("../Common/textures/grid_512.png", &texture_handle, &width, &height, &layers);
		else
			texture.loadTextureFromFile("../Common/textures/metal_roof_diff_512x512.png", &texture_handle, &width, &height, &layers);
	}

	
	if (has_camera_changed())
	{
		if (!get_camera_setting())
		{
			camera_pan_speed = 15.f;
			Scene_camera = Camera();
			Scene_camera.forward(-100050.0f);
			Scene_camera.pan(70000.0f);
			Scene_camera.lift(-50000.f);
			Scene_camera.yaw(45.0f);
		}
		else
		{
			camera_pan_speed = 0.01f;
			Scene_camera = Camera();
			Scene_camera.forward(-1.0f);
			Scene_camera.lift(-0.125f);
		}

	}
	
	
	if (powerplantChanged() || BVTreeChanged() || hasBVsphereChanged())
	{
		
		construct_display_list();

		create_octree();
		create_bvtree();
	}
	
	return 0;
}

void SimpleScene_Quad::SwapFile(int index)
{
	switch (index) {
	case 1:
	{
		file_object.readfile("../Common/models/CS350_Models/cube.obj"); 
		break;
	}
	case 2:
	{
		file_object.readfile("../Common/models/CS350_Models/cube2.obj");
		break; 
	}
	case 3:
	{
		file_object.readfile("../Common/models/CS350_Models/4sphere.obj");
		break;
	}
	case 4:
	{
		file_object.readfile("../Common/models/CS350_Models/sphere_modified.obj");
		break;
	}
	case 5:
	{
		file_object.readfile("../Common/models/CS350_Models/bunny.obj");
		break;
	}
	default:
	{
		file_object.readfile("../Common/models/CS350_Models/sphere.obj");
		break;
	}
	}
	file_object.calculateScale();
	file_object.calculateUV_CPU(0);
	file_object.setNormalScale(1.0f);
	file_object.calculateNormals();
	file_object.calculateBVs((Bounding_Sphere::BSphere_type) getBVSphereType());
	

	if (hasTextureChanged())
	{
		if (drawUVGrid())
			texture.loadTextureFromFile("../Common/textures/grid_512.png", &texture_handle, &width, &height, &layers);
		else
			texture.loadTextureFromFile("../Common/textures/metal_roof_diff_512x512.png", &texture_handle, &width, &height, &layers);
	}

	SetupBuffers();
}

void SimpleScene_Quad::LoadShader(int index)
{
	
	 //first pass shaders
	Shader_GeometryPass = LoadShaders("../Common/shaders/deffered_buffer.vert",
		"../Common/shaders/deffered_buffer.frag");
	
	//second pass shaders
	Shader_LightingPass = LoadShaders("../Common/shaders/deffered_lighting.vert",
		"../Common/shaders/deffered_lighting.frag");

	Shader_Debug = LoadShaders("../Common/shaders/debug_draw.vert",
		"../Common/shaders/debug_draw.frag");


	//get the information to send from the first to second pass
	glUseProgram(Shader_LightingPass);


	aposition = glGetAttribLocation(Shader_GeometryPass, "aPosition");
	anormal = glGetAttribLocation(Shader_GeometryPass, "aNormal");
	aCPUuv = glGetAttribLocation(Shader_GeometryPass, "aCPU_uv");

	ueye_position = glGetUniformLocation(Shader_LightingPass, "eye_position");
	udiffuse_color = glGetUniformLocation(Shader_LightingPass, "diffuse_color");
	uspecular_color = glGetUniformLocation(Shader_LightingPass, "specular_color");
	uspecular_exponent = glGetUniformLocation(Shader_LightingPass, "specular_exponent");
	uambient_color = glGetUniformLocation(Shader_LightingPass, "ambient_color");
	uemmissive_color = glGetUniformLocation(Shader_LightingPass, "emmissive_color");
	
}

void SimpleScene_Quad::CheckInputs(GLFWwindow* window)
{
	int state = glfwGetKey(window, GLFW_KEY_W);
	if (state == GLFW_PRESS)
	{
		Scene_camera.forward(camera_pan_speed * gui_getdt());
	}

	state = glfwGetKey(window, GLFW_KEY_A);
	if (state == GLFW_PRESS)
	{
		Scene_camera.pan(camera_pan_speed * gui_getdt());
	}

	state = glfwGetKey(window, GLFW_KEY_S);
	if (state == GLFW_PRESS)
	{
		Scene_camera.forward(-camera_pan_speed * gui_getdt());
	}

	state = glfwGetKey(window, GLFW_KEY_D);
	if (state == GLFW_PRESS)
	{
		Scene_camera.pan(-camera_pan_speed * gui_getdt());
	}

	state = glfwGetKey(window, GLFW_KEY_SPACE);
	if (state == GLFW_PRESS)
	{
		Scene_camera.lift(-camera_pan_speed * gui_getdt());
	}

	state = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL);
	if (state == GLFW_PRESS)
	{
		Scene_camera.lift(camera_pan_speed * gui_getdt());
	}

	state = glfwGetKey(window, GLFW_KEY_KP_0);
	if (state == GLFW_PRESS)
	{
		Scene_camera.roll(camera_rotate_speed * gui_getdt());
	}

	state = glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL);
	if (state == GLFW_PRESS)
	{
		Scene_camera.roll(-camera_rotate_speed * gui_getdt());
	}

	state = glfwGetKey(window, GLFW_KEY_UP);
	if (state == GLFW_PRESS)
	{
		Scene_camera.pitch(camera_rotate_speed * gui_getdt());
	}

	state = glfwGetKey(window, GLFW_KEY_DOWN);
	if (state == GLFW_PRESS)
	{
		Scene_camera.pitch(-camera_rotate_speed * gui_getdt());
	}

	state = glfwGetKey(window, GLFW_KEY_LEFT);
	if (state == GLFW_PRESS)
	{
		Scene_camera.yaw(-camera_rotate_speed * gui_getdt());
	}

	state = glfwGetKey(window, GLFW_KEY_RIGHT);
	if (state == GLFW_PRESS)
	{
		Scene_camera.yaw(camera_rotate_speed * gui_getdt());
	}

}


// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------

void SimpleScene_Quad::renderQuad()
{

	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void SimpleScene_Quad::renderBVTree(BV_Heirarchy::BV_Node* node)
{
	if (node)
	{
		glm::vec3 scaleVector;
		glm::vec3 centroid;
		glm::mat4 modelMat = glm::mat4(1.0f);

		glUniform3fv(glGetUniformLocation(Shader_Debug, "debugcolor"), 1, &(debugcolors[node->depth])[0]);
		//gui_display_depth
		if (node->depth >= getdrawdepth() && !BVTreeChanged())
		{
			if (!getBVSphere())
			{

				scaleVector = glm::vec3(((AABB*)node->volume_data)->aabb_getmax() - ((AABB*)node->volume_data)->aabb_getmin());
				centroid = ((AABB*)node->volume_data)->get_center();
				modelMat = glm::translate(centroid) * glm::scale(scaleVector);


				glUniformMatrix4fv(glGetUniformLocation(Shader_Debug, "vertexTransform"), 1, GL_FALSE, &modelMat[0][0]);

				glLineWidth(1.0f);
				glBindVertexArray(Bounding_CubeVAO);
				glBindBuffer(GL_ARRAY_BUFFER, Bounding_CubeVBO);
				glVertexAttribPointer(aposition, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
				glEnableVertexAttribArray(aposition);
				glDrawArrays(GL_LINES, 0, 24);
				glBindVertexArray(0);
			}
			//draw sphere
			else
			{
				scaleVector = glm::vec3(((Bounding_Sphere*)node->volume_data)->radius);
				centroid = ((Bounding_Sphere*)node->volume_data)->get_center();
				//modelMat = glm::scale(scaleVector);
				modelMat = glm::translate(centroid) * glm::scale(scaleVector);

				glUniformMatrix4fv(glGetUniformLocation(Shader_Debug, "vertexTransform"), 1, GL_FALSE, &modelMat[0][0]);

				glBindVertexArray(light_sphereVAO);
				glDrawArrays(GL_LINES, 0, spheres.orbit_index);
				glBindVertexArray(0);
			}
		}
		if (node->left_child)
		{
			renderBVTree(node->left_child);
		}

		if (node->right_child)
		{
			renderBVTree(node->right_child);
		}
	}
}

void SimpleScene_Quad::create_bvtree()
{
	for (int i = 0; i < bottom_up_construction_objects.size(); i++)
	{
		delete bottom_up_construction_objects[i];
	}

	top_down_construction_objects.clear();
	bottom_up_construction_objects.clear();
	bv_heirarchy.tree_set_parameters((BV_Heirarchy::bv_merge_type)getBottomUpMergeType() , (BV_Heirarchy::bv_split_type) getTopdownSplitType(), getBVSphere());

	//returns true if bottom up
	if (getBVTreeType())
	{
		//construct list of bv nodes
		for (int i = 0; i < powerplant_objects.size(); i++)
		{
			BV_Heirarchy::BV_Node* construction_node = new BV_Heirarchy::BV_Node;
			if (getBVSphere())
				construction_node->volume_data = &powerplant_objects[i].bounding_sphere;
			else
				construction_node->volume_data = &powerplant_objects[i].bounding_AABB;

			construction_node->type = BV_Heirarchy::bv_treetype::leaf;
			construction_node->left_child = nullptr;
			construction_node->right_child = nullptr;
			construction_node->object_count = 1;
			construction_node->depth = 7;

			bottom_up_construction_objects.push_back(construction_node);
		}

		//construct tree
		bvTree_root = *bv_heirarchy.construct_bottomup(bottom_up_construction_objects);
	}
	else
	{
		//construct list of bvs
		for (int i = 0; i < powerplant_objects.size(); i++)
		{
			Bounding_Volume* construction_node = &powerplant_objects[i].bounding_sphere;
			if (getBVSphere())
				construction_node = &powerplant_objects[i].bounding_sphere;
			else
				construction_node = &powerplant_objects[i].bounding_AABB;

			top_down_construction_objects.push_back(construction_node);
		}
		BV_Heirarchy::BV_Node* temp_root = &bvTree_root;
		bv_heirarchy.construct_topdown(temp_root, &top_down_construction_objects[0], top_down_construction_objects.size(), 0);
	}
}


void SimpleScene_Quad::construct_display_list()
{
	powerplant_objects.clear();

	if (display_section4())
	{
		for (int i = 0; i < section4.size(); i++)
		{
			powerplant_objects.push_back(section4[i]);
		}
	}
	if (display_section5())
	{
		for (int i = 0; i < section5.size(); i++)
		{
			powerplant_objects.push_back(section5[i]);
		}
	}
	if (display_section6())
	{
		for (int i = 0; i < section6.size(); i++)
		{
			powerplant_objects.push_back(section6[i]);
		}
	}

	if (hasBVsphereChanged())
	{
		for (int i = 0; i < powerplant_objects.size(); i++)
		{
			file_object.calculateBVs((Bounding_Sphere::BSphere_type) getBVSphereType());
			powerplant_objects[i].calculateBVs((Bounding_Sphere::BSphere_type) getBVSphereType());
		}
	}

}

void SimpleScene_Quad::destroy_bvtree(BV_Heirarchy::BV_Node* node)
{
	
	if (node && node != &bvTree_root)
	{
		if (node->left_child)
		{
			destroy_bvtree(node->left_child);

		}

		if (node->right_child)
		{
			destroy_bvtree(node->right_child);

		}
		if(node->volume_data)
			delete node->volume_data;

		delete node;
	}
}

void SimpleScene_Quad::create_octree()
{
	//delete the old tree
	if (scene_octree.root)
	{
		destroy_octree(scene_octree.root);
		delete scene_octree.root;
	}

	//create a new tree
	scene_octree.root = new Octree::Octree_Node;
	scene_octree.root->volume_data.clear();
	int total_tri_count = 0;
	for (int i = 0; i < powerplant_objects.size(); i++)
	{
		total_tri_count += powerplant_objects[i].fragment_buffer.size();
		for (int j = 0; j < powerplant_objects[i].getGeometryEnd(); j++)
		{
			scene_octree.root->volume_data.push_back(powerplant_objects[i].geometry_buffer[j]);
		}
	}

	scene_octree.root->depth = 0;
	scene_octree.root->tri_count = total_tri_count;
	scene_octree.root->extents = new AABB;
	scene_octree.root->extents->calculate_AABB(scene_octree.root->volume_data);
	scene_octree.calculate_octree(scene_octree.root, scene_octree.root->volume_data);
	
}

void SimpleScene_Quad::destroy_octree(Octree::Octree_Node* node)
{
	if (node)
	{
		node->volume_data.clear();
		node->wireframe.clear();

		for (int i = 0; i < OCTREE_CHILDREN_COUNT; i++)
		{
			if (node->Children[i])
			{
				destroy_octree(node->Children[i]);
				delete node->Children[i];
			}
		}
	}
}

void SimpleScene_Quad::render_octree(Octree::Octree_Node* node)
{
	if (node != nullptr)
	{
		glm::vec3 scaleVector;
		glm::vec3 centroid;
		glm::mat4 modelMat = glm::mat4(1.0f);

		glUniform3fv(glGetUniformLocation(Shader_Debug, "debugcolor"), 1, &(debugcolors[0])[0]);
		aposition = glGetAttribLocation(Shader_GeometryPass, "aPosition");

		scaleVector = glm::vec3((node->extents->aabb_getmax()) - (node->extents->aabb_getmin()));
		centroid = (node->extents->get_center());
		modelMat = glm::translate(centroid) * glm::scale(scaleVector);


		glUniformMatrix4fv(glGetUniformLocation(Shader_Debug, "vertexTransform"), 1, GL_FALSE, &modelMat[0][0]);

		glLineWidth(1.0f);
		glBindVertexArray(Bounding_CubeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, Bounding_CubeVBO);

		glDrawArrays(GL_LINES, 0, 24);
		glBindVertexArray(0);

		if (node->type == Octree::octree_node_type::leaf)
		{
			modelMat = glm::mat4(1.0f);
			glUniformMatrix4fv(glGetUniformLocation(Shader_Debug, "vertexTransform"), 1, GL_FALSE, &modelMat[0][0]);
			glUniform3fv(glGetUniformLocation(Shader_Debug, "debugcolor"), 1, &node->color[0]);

			glLineWidth(1.0f);
			glBindVertexArray(octreeVAO);

			glBindBuffer(GL_ARRAY_BUFFER, octreeVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * node->wireframe.size(), node->wireframe.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
			glEnableVertexAttribArray(0);


			glDrawArrays(GL_LINES, 0, node->wireframe.size());
			glBindVertexArray(0);
		}
		

		
		for (int i = 0; i < OCTREE_CHILDREN_COUNT; i++)
		{
			if(node->Children[i] != nullptr)
				render_octree(node->Children[i]);
		}
	}
}