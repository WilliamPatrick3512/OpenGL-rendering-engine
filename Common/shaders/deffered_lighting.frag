/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: deffered_lighting.frag
Purpose: fragment shader for lighting pass, performs phong shading
Language: C++
Platform: visual studio 2019, windows 10
Project: william.patrick_CS300_1
Author: William Patrick, william.patrick, 280002517
Creation date: 2/5
End Header --------------------------------------------------------*/
#version 410 core

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

uniform vec3 camera_pos;

//light components
uniform vec3 ambient_color[16];
uniform vec3 diffuse_color[16];
uniform vec3 specular_color[16];
uniform vec3 emmissive_color[16];
uniform int shader_light_count;

uniform float specular_exponent;
uniform float falloff_value[16];

uniform vec3 light_pos[16];
uniform vec3 dlight_directions[16];
uniform int light_types[16];
uniform float spotlight_angles_inner[16];
uniform float spotlight_angles_outer[16];

uniform vec3 attenuation_constants;
uniform vec3 global_ambient;
uniform vec3 fog_color;
uniform vec2 fog_distances;

uniform int render_type;


// Output data
out vec3 color;

void main()
{
  color = vec3(0.0f,0.0f,0.0f);
  
  vec3 normal = texture(gNormal, TexCoords).rgb;
  vec3 fragment_pos = texture(gPosition, TexCoords).rgb;
  vec3 surface_texcolor = texture(gAlbedoSpec, TexCoords).rgb;
  
  vec3 viewing_vector = normalize(camera_pos - fragment_pos);
  float viewing_distance = length(-camera_pos - fragment_pos);
  
  if(length(normal) != 0.0f && length(fragment_pos) != 0.0f && length(surface_texcolor) != 0.0f)
  {
  
  for(int i = 0; i<shader_light_count; i++)
  {
	vec3 norm = normalize(normal);
	vec3 lightDir = vec3(0.0f,0.0f,0.0f);
	vec3 direction = normalize(dlight_directions[i]);
	
	float spotlight_effect = 1.0f;
	
	//L vector
	lightDir = normalize(light_pos[i] - fragment_pos);
	
	float distance = length(light_pos[i] - fragment_pos);
	
	float attenuation = min( 1/(attenuation_constants[0] + (distance * attenuation_constants[2]) + pow((distance * attenuation_constants[2]),2.0f)) ,1.0f);
	
	

	vec3 invlightdir = -lightDir;
	float diff = max(dot(norm, lightDir), 0.0);
	float initialdiff = max(dot(norm, lightDir), 0.0);
	
	if(diff > 0.0f)
	{
	vec3 reflection_vector = normalize((2.0f * dot(norm,lightDir) * norm) - lightDir);
	float spec = max(pow(max(dot(reflection_vector,-viewing_vector),0.0f),10.5f),0.0f);
	float alpha = dot(light_pos[i] - fragment_pos, dlight_directions[i]) / (length(light_pos[i] - fragment_pos) * length(dlight_directions[i]));

   //directional light
    if(light_types[i] == 1)
    {
    	if(diff != 0.0f)
    		diff = dot(norm, direction);
    }
    
    // spot light
    else if(light_types[i] == 2)
    {
    	if(initialdiff > 0.0f)
    	{
    		float LightDot = dot(lightDir,direction);
    		if(LightDot < spotlight_angles_outer[i] && LightDot < 0.0f)
    		{
    			spotlight_effect = 0.0f;
    		}
    		else
    		{
    
    			
    			//p = falloff value
    			
    			float inner = spotlight_angles_inner[i];
    			float outer = spotlight_angles_outer[i];
    			
    			if(inner > outer)
    			{
    				inner = outer;
    			}
    			
    			spotlight_effect = pow((cos(alpha) - cos(outer)) / (cos(inner) - cos(outer)), falloff_value[i]/ 20.0f);
    		}
    
    	}
    	else
    	{
    	spotlight_effect = 0.0f;
    	}
    }
	vec3 specular = specular_color[i] * spec;
	
	//set diffuse to just the texture (for testing)
	//vec3 diffuse = texture( texture_diffuse, CPU_uv ).rgb;
	//vec3 diffuse = texture( texture_diffuse, GPU_UV ).rgb;
	
	//show texture and diffuse
	vec3 diffuse;
	
	
	diffuse = diff * (diffuse_color[i] * surface_texcolor);

	color += attenuation * spotlight_effect * (specular + diffuse);
	
	}
	}
  
  

  color += global_ambient;
  
  float Fog_s = ((fog_distances[1] - viewing_distance)/(fog_distances[1] - fog_distances[0]));
  
  color = (Fog_s * color) + ((1-Fog_s) * fog_color);
  
  
  switch (render_type)
  {
  	case 4:
	{
	  color = vec3(viewing_distance/4.0f);
	  break;
	}
	case 3:
	{
	  color = surface_texcolor;
	  break;
	}
	case 2:
	{
	  color = normal;
	  break;
	}
	case 1:
	{
	  color = fragment_pos;
	  break;
	}
	default:
	{
	  break;
	}
	}
}
}