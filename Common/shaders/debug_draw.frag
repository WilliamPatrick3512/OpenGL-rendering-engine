/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: debug_draw.frag
Purpose: fragment shader for forward rendering objects
Language: C++
Platform: visual studio 2019, windows 10
Project: william.patrick_CS300_1
Author: William Patrick, william.patrick, 280002517
Creation date: 3/14
End Header --------------------------------------------------------*/
#version 410 core

// Input from the rasterizer
// Conceptually equivalent to
// "WritePixel( x, y, rasterColor )"

uniform vec3 debugcolor;

// Output data
out vec3 color;

void main()
{
  color = debugcolor;
  
}