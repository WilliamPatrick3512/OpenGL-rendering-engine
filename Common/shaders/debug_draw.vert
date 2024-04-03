/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: debug_draw.vert
Purpose: vertex shader for forward rendering objects
Language: C++
Platform: visual studio 2019, windows 10
Project: william.patrick_CS300_1
Author: William Patrick, william.patrick, 280002517
Creation date: 3/14
End Header --------------------------------------------------------*/
#version 410 core

layout (location = 0) in vec3 aposition;

uniform mat4  vertexTransform;
uniform mat4 view;
uniform mat4 persp;

void main()
{
    vec4 vPos = vec4( aposition.x, aposition.y, aposition.z, 1.0f );
	
	gl_Position = persp * view * vertexTransform * vPos;
}
