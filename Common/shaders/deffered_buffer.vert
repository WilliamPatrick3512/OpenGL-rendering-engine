/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: deffered_buffer.vert
Purpose: generates textures for deffered rendering
Language: C++
Platform: visual studio 2019, windows 10
Project: william.patrick_CS300_1
Author: William Patrick, william.patrick, 280002517
Creation date: 2/5
End Header --------------------------------------------------------*/

#version 450 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aCPU_uv;

//in vec3 aPosition;
//in vec3 aNormal;
//in vec2 aCPU_uv;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;

uniform mat4 vertexTransform;
uniform mat4 view;
uniform mat4 persp;

void main()
{
    vec4 worldPos = vertexTransform * vec4(aPosition, 1.0);
    FragPos = worldPos.xyz; 
    TexCoords = aCPU_uv;
    
    mat3 normalMatrix = transpose(inverse(mat3(vertexTransform)));
    Normal = normalMatrix * aNormal;

    gl_Position = persp * view * worldPos;
}

