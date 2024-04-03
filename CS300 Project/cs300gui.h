/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: cs300gui.h
Purpose: controls the GUI for this program
Language: C++
Platform: visual studio 2019, windows 10
Project: william.patrick_CS300_1
Author: William Patrick, william.patrick, 280002517
Creation date: 9/17
End Header --------------------------------------------------------*/
#pragma once
#include <GLFW/glfw3.h>
#include "../ThirdParty/imgui/imgui.h"
#include "../ThirdParty/imgui/imgui_impl_glfw.h"
#include "../ThirdParty/imgui/imgui_impl_opengl3.h"
#include <glm/vec3.hpp>

#include <vector>

void initializeGUI(GLFWwindow* window);
void updateGUI();
void GUIsettings();
void renderGUI();
void shutdownGUI();

bool drawVertNormals();
bool drawFragNormals();
bool drawUVGrid();
bool hasModelChanged();
bool hasShaderChanged();


int giveCurrentItem();
int giveCurrentShader();


int* getLightCount();
float* getLightRotations();
float* getLightAngles_inner();
float* getLightAngles_outer();
float* getLightFalloff();

float* getCameraRotation();
int* getLightTypes();
glm::vec3* getLightDiffuse();
glm::vec3* getLightAmbient();
glm::vec3* getLightSpecular();

glm::vec3* getFogColor();
glm::vec3* getGlobalAmbientColor();
float* getAttenuationConstants();
float* getFogDistances();
int* getUVCalculationType();
bool hasUVChanged();

bool hasTextureChanged();
bool stopOrbit();
int* calculateUVCPU();
float gui_getdt();

int get_rendertype();
int getBVSphereType();
bool hasBVsphereChanged();
int getTopdownSplitType();
int getBottomUpMergeType();

bool getBVdraw();
bool getOctreedraw();
bool getBVTreedraw();
bool getBVSphere();
bool getBVTreeType();
bool BVTreeChanged();

bool display_section4();
bool display_section5();
bool display_section6();
bool powerplantChanged();
int getdrawdepth();

bool get_camera_setting();
bool has_camera_changed();



//surface material attributes
glm::vec3* getSurfaceAmbient();
glm::vec3* getSurfaceDiffuse();
glm::vec3* getSurfaceSpecular();
glm::vec3* getSurfaceEmmissive();
