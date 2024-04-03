//
// Created by pushpak on 6/1/18.
//

#ifndef SIMPLE_SCENE_SIMPLESCENE_QUAD_H
#define SIMPLE_SCENE_SIMPLESCENE_QUAD_H

#include "../Common/Scene.h"
#include <fstream>
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "../CS300 Project/fileObject.h"
#include "../CS300 Project/spheremesh.h"
#include "../CS300 Project/BV_Heirarchy.h"
#include <vector>
#include"../CS300 Project/Lights.h"
#include"../CS300 Project/texture_manager.h"
#include "../CS300 Project/Camera.h"
#include "../CS300 Project/Octree.h"




class SimpleScene_Quad : public Scene
{

public:
    SimpleScene_Quad() = default;
    SimpleScene_Quad( int windowWidth, int windowHeight );
    virtual ~SimpleScene_Quad();


public:
    int Init() override;
    void CleanUp() override;

    int Render() override;
    int Render_Forward();
    int Render_Deffered();
    int postRender() override;


private:




    // member functions
    void initMembers();

    // This is the non-software engineered portion of the code
    // Go ahead and modularize the VAO and VBO setup into
    // BufferManagers and ShaderManagers
    void SetupBuffers();

    void SetupNanoGUI(GLFWwindow *pWwindow) override;

	void SwapFile(int index);

	void LoadShader(int index);


	void DeleteLight(int i);
	void create_bvtree();


	void CheckInputs(GLFWwindow* window);

	Camera  Scene_camera;
	
	//shader ID handles
	GLuint  programID;
	GLuint  Shader_GeometryPass;
	GLuint  Shader_LightingPass;
	GLuint  Shader_Debug;

	//deffered geometry buffer handle
	GLuint gbuffer;

	//file object buffers
	GLuint  fileobjectVAO;
	GLuint  floorVAO;
	GLuint  light_sphereVAO;
	GLuint  Bounding_CubeVAO;

	GLuint  Bounding_CubeVBO;

	GLuint  octreeVAO;
	GLuint  octreeVBO;

    GLuint  vertexbuffer;
    
	GLuint  normal_buffer;
	GLuint  uvbuffer;
	GLuint  texture_handle;


    GLuint  spherebuffer;
	GLuint  sphere_normal_buffer;

    GLuint  floorbuffer;
    GLuint  floor_normal_buffer;


    GLuint  quaduvbuffer;



	GLuint rboDepth;
	GLuint attachments[3];
	
	fileObject file_object;
	fileObject file_object2;

	std::vector<fileObject> powerplant_objects;
	std::vector<fileObject> section4;
	std::vector<fileObject> section5;
	std::vector<fileObject> section6;
	std::vector<BV_Heirarchy::BV_Node*> bottom_up_construction_objects;
	std::vector<Bounding_Volume*> top_down_construction_objects;

	std::vector<glm::vec3> Octree_construction_object;

	Octree scene_octree;

	spheremesh spheres;
	texture_manager texture;

	std::vector<GLfloat>    Floor;
	std::vector<GLfloat>    floor_normals;
	
	GLuint program,
		aposition,
		anormal,
		aCPUuv,
		upersp_matrix,
		uview_matrix,
		umodel_matrix,
		unormal_matrix,
		ulight_position,
		ueye_position,
		udiffuse_color,
		uspecular_color,
		uemmissive_color,
		uspecular_exponent,
		uambient_color,

		gPosition,
		gNormal,
		gAlbedoSpec,

		usurface_ambient,
		usurface_diffuse,
		usurface_specular,
		usurface_emmissive,

		uattenuation_constants,
		uglobal_ambient,
		ufog_color,
		ufog_distances,

		utex_minimum,
		utex_maximum,
		utex_uvCalculateCPU,
		utex_uvCalculateType,

		ulight_count,
		uligt_direction,
		ulight_types,
		ulight_angles_inner,
		ulight_angles_outer,
		ulight_falloff;

	//angle the camera is rotated by around the y axis, old code
    GLfloat   angleOfRotation;
	
	//current light count and an array to store all light data
	int light_count;
	Lights scene_lights[16];

	BV_Heirarchy::BV_Node bvTree_root;
	BV_Heirarchy bv_heirarchy;
	struct Material {
		glm::vec3 diffuse_color,
			specular_color,
			emmissive_color;
		float specular_exponent; //ns in notes
		glm::vec3 light_color,   // not really material properies, but..
			ambient_color;		 // needed for rendering light source
	};
	
	//array for floor and file object
	Material properties[2];
	
	unsigned int quadVAO = 0;
	unsigned int quadVBO;
	unsigned int quadUV;

	glm::vec3 fogColor;
	glm::vec3 global_ambient;
	glm::vec2 fogDistances;
	glm::vec3 AttenuationConstants;
	void renderQuad();
	void renderBVTree(BV_Heirarchy::BV_Node* node);
	void construct_display_list();
	void destroy_bvtree(BV_Heirarchy::BV_Node* node);

	void create_octree();
	void destroy_octree(Octree::Octree_Node* node);
	void render_octree(Octree::Octree_Node* node);

};


#endif //SIMPLE_SCENE_SIMPLESCENE_QUAD_H
