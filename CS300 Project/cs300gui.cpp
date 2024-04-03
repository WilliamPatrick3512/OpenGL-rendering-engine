/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: cs300gui.cpp
Purpose: controls the GUI for this program
Language: C++
Platform: visual studio 2019, windows 10
Project: william.patrick_CS300_1
Author: William Patrick, william.patrick, 280002517
Creation date: 9/17
End Header --------------------------------------------------------*/
#include "cs300gui.h"



static bool draw_fragment_normals = false;
static bool draw_vertex_normals = false;
static bool draw_uv_grid = false;
static bool create_light_flag = false;
static bool delete_light_flag = false;
static int current_item;
static int current_item_previous;

static int current_shader;
static int current_shader_previous;

static float dt;
static float camera_rotation;


static glm::vec3 surface_ambient;
static glm::vec3 surface_diffuse;
static glm::vec3 surface_specular;
static glm::vec3 surface_emmissive;

static int render_type = 0;

static int light_count;
static int light_types[16];
static float spotlight_inner_angles[16];
static float spotlight_outer_angles[16];
static float falloff_values[16];
static float light_rotation[16];
static glm::vec3 light_ambient[16];
static glm::vec3 light_diffuse[16];
static glm::vec3 light_specular[16];
static glm::vec3 light_emmissive[16];


static float attenuation_constants[3];
static float fog_distances[2];
static glm::vec3 fog_color;
static glm::vec3 global_ambient;

static int UV_CalculationType;
static int prev_UV_CalculationType;
static bool previous_showgrid;
static bool calculateuv_CPU;
static bool stop_orbit;

//bounding volumes and trees
static bool draw_BVs;
static bool draw_Octree;
static bool draw_BV_Sphere;
static bool previous_draw_BV_Sphere;
static bool draw_BV_Trees;

static int Bounding_sphere_type;
static int previous_Bounding_sphere_type;

static int Topdown_Split_type;
static int previous_Topdown_Split_type;

static int BottomUp_Merge_type;
static int previous_BottomUp_Merge_type;

static bool BV_Tree_Bottom_up;
static bool previous_BV_Tree_Bottom_up;

static bool Show_Section4;
static bool prev_Show_Section4;

static bool Show_Section5;
static bool prev_Show_Section5;

static bool Show_Section6;
static bool prev_Show_Section6;

static bool view_power_plant_full;
static bool view_power_plant_wire;
static bool previous_view_power_plant_full;
static bool previous_view_power_plant_wire;

static int BVH_DrawDepth;

const char* Sphere_types[] = { "Ritter","Larsson","PCA" };
const char* Topdown_Split_types[] = { "Median Centers","Median Extents","K even" };
const char* BottomUp_Merge_types[] = { "nearest neighbor","minimum combined volume","minimum relative increase" };


const char* lights[] = {"point light","directional light","spotlight"};
const char* items[] = { "sphere","cube", "cube2", "4sphere","sphere_modified", "bunny"};
const char* shaders[] = { "phong shading"};
const char* uv_types[] = { "planar","sphere","cylinder"};

float scenario_angles[] = { 0.0f, 45.0f, 90.0f, 135.0f, 180.0f, 225.0f, 270.0f, 315.0f};
float scenario3_angles[] = { 0.0f, 30.0f, 60.0f, 90.0f,
							120.0f, 150.0f, 180.0f, 210.0f,
							240.0f, 270.0f, 300.0f, 330.0f};

glm::vec3 scenario2_colors[] = {glm::vec3(1.0f,0.0f,0.0f),
								glm::vec3(1.0f,1.0f,0.0f),
								glm::vec3(0.5f,1.0f,0.0f),
								glm::vec3(0.0f,1.0f,0.0f),
								glm::vec3(0.0f,1.0f,1.0f),
								glm::vec3(0.0f,0.0f,1.0f),
								glm::vec3(0.5f,0.0f,1.0f),
								glm::vec3(1.0f,0.0f,1.0f),
};

glm::vec3 scenario3_colors[] = { glm::vec3(1.0f,0.0f,0.0f),
								glm::vec3(1.0f,0.5f,0.0f),
								glm::vec3(1.0f,1.0f,0.0f),
								glm::vec3(0.5f,1.0f,0.0f),
								glm::vec3(0.0f,1.0f,0.0f),
								glm::vec3(0.0f,1.0f,0.5f),
								glm::vec3(0.0f,1.0f,1.0f),
								glm::vec3(0.0f,0.5f,1.0f),
								glm::vec3(0.0f,0.0f,1.0f),
								glm::vec3(0.5f,0.0f,1.0f),
								glm::vec3(1.0f,0.0f,1.0f),
								glm::vec3(1.0f,0.0f,.5f),
};


void initializeGUI(GLFWwindow* window)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 400");

	glm::vec3 defaultlight(1.0f, 1.0f, 1.0f);
	for (int i = 0; i < 16; i++)
	{
		light_diffuse[i] = defaultlight;
		light_ambient[i] = defaultlight;
		light_specular[i] = defaultlight;

		falloff_values[i] = 1.0f;

		spotlight_inner_angles[i] = 90.f/4.0f;
		spotlight_outer_angles[i] = 90.f / 2.0f;
	}

	light_count = 1;
	surface_diffuse = defaultlight;
	surface_specular = defaultlight;
	surface_ambient = glm::vec3(0.0f,0.0f,0.1f);
	attenuation_constants[0] = 1.0f;
	attenuation_constants[1] = 0.22f;
	attenuation_constants[2] = 0.2f;

	surface_emmissive = glm::vec3(1.0f, 0.0f, 0.0f);

	fog_distances[0] = 0.1f;
	fog_distances[1] = 20.0f;

	fog_color = glm::vec3(0.2f,0.4f,0.55f);
	global_ambient = glm::vec3(0.0f,0.0f,0.11f);

	Show_Section4 = true;
	Show_Section5 = true;
	Show_Section6 = true;

}

void updateGUI()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	current_item_previous = current_item;
	current_shader_previous = current_shader;
	prev_UV_CalculationType = UV_CalculationType;
	previous_showgrid = draw_uv_grid;
	previous_Bounding_sphere_type = Bounding_sphere_type;
	prev_Show_Section4 = Show_Section4;
	prev_Show_Section5 = Show_Section5;
	prev_Show_Section6 = Show_Section6;
	previous_BV_Tree_Bottom_up = BV_Tree_Bottom_up;
	previous_BottomUp_Merge_type = BottomUp_Merge_type;
	previous_Topdown_Split_type = Topdown_Split_type;
	previous_draw_BV_Sphere = draw_BV_Sphere;
	previous_view_power_plant_full = view_power_plant_full;
	ImGui::NewFrame();
}

void GUIsettings()
{
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	static float f = 0.0f;
	static int counter = 0;
	create_light_flag = false;
	delete_light_flag = false;
	dt = 1000.0f / ImGui::GetIO().Framerate;
	ImGui::Begin("CS300P1_settings");// Create a window called "Hello, world!" and append into it.
	if (ImGui::CollapsingHeader("object"))
	{
		ImGui::Combo("file loaded", &current_item, items, 6);

		//ImGui::Checkbox("show fragment normals", &draw_fragment_normals);
		//ImGui::Checkbox("show vertex normals", &draw_vertex_normals);

		ImGui::Checkbox("Power Plant camera", &view_power_plant_full);
		ImGui::Checkbox("Show Only Wireframe", &view_power_plant_wire);


		ImGui::Checkbox("show section 4", &Show_Section4);
		ImGui::Checkbox("show section 5", &Show_Section5);
		ImGui::Checkbox("show section 6", &Show_Section6);



		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",dt , ImGui::GetIO().Framerate);
	}

	if (ImGui::CollapsingHeader("Shader"))
	{
		if (ImGui::Button("Default"))
		{
			render_type = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button("Position"))
		{
			render_type = 1;
		}
		ImGui::SameLine();
		if (ImGui::Button("Normals"))
		{
			render_type = 2;
		}
		ImGui::SameLine();
		if (ImGui::Button("Albedo"))
		{
			render_type = 3;
		}
		ImGui::SameLine();
		if (ImGui::Button("Depth"))
		{
			render_type = 4;
		}


		ImGui::Combo("shader loaded", &current_shader, shaders,1);
		if(ImGui::Button("Reload Shader"))
		{
			current_shader_previous = -1;
		}
	}

	if (ImGui::CollapsingHeader("material"))
	{
		ImGui::BulletText("Surface Color tints");
		ImGui::ColorEdit3("debug color", (float*)&surface_emmissive);


		ImGui::Checkbox("visualize UV", &draw_uv_grid);

		ImGui::Combo("UV Projection", &UV_CalculationType, uv_types, 3);
	}

	if (ImGui::CollapsingHeader("Bounding Volumes"))
	{
		ImGui::Combo("TopDown split type", &Topdown_Split_type, Topdown_Split_types, 3);
		ImGui::Combo("Bounding sphere type", &Bounding_sphere_type, Sphere_types, 3);

		ImGui::Text("setting this to 0 will show all BVs in the BVH");
		ImGui::SliderInt("BVH depth", &BVH_DrawDepth, 0,7);

		ImGui::Checkbox("Show object BVs", &draw_BVs);
		ImGui::Checkbox("Show Octree", &draw_Octree);
		ImGui::Checkbox("Sphere_BV", &draw_BV_Sphere);
		ImGui::Checkbox("Bottom Up tree", &BV_Tree_Bottom_up);

	}


	if (ImGui::CollapsingHeader("Global Constants"))
	{

		ImGui::SliderFloat3("Attenuation", attenuation_constants, 0.0f, 2.0f, "%.3f", 0);
		ImGui::ColorEdit3("Global Ambient", (float*)&global_ambient);
		ImGui::ColorEdit3("Fog color", (float*)&fog_color);

		ImGui::SliderFloat2("Fog Distances", fog_distances, 0.0f, 100.0f, "%.3f", 0);
	}

	if (ImGui::CollapsingHeader("lights"))
	{
		ImGui::Checkbox("Stop Orbit", &stop_orbit);
		if(ImGui::Button("CreateLight"))
		{
			if (light_count < 16)
			{
				light_count++;
			}
		}
		ImGui::SameLine();
		
		if (ImGui::Button("DestroyLight"))
		{
			if (light_count > 0)
			{
				light_count--;
			}
		}

		if (ImGui::Button("Scenario 1"))
		{
			light_count = 8;

			for (int i = 0; i < 8; i++)
			{
				light_rotation[i] = scenario_angles[i];
				light_diffuse[i] = glm::vec3(1.0f);
				light_ambient[i] = glm::vec3(1.0f);
				light_specular[i] = glm::vec3(1.0f);
				light_emmissive[i] = glm::vec3(1.0f);
				light_types[i] = 0;

			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Scenario 2"))
		{
			light_count = 8;
			for (int i = 0; i < 8; i++)
			{
				light_rotation[i] = scenario_angles[i];
				light_diffuse[i] = scenario2_colors[i];
				light_ambient[i] = scenario2_colors[i];
				light_specular[i] = scenario2_colors[i];
				light_emmissive[i] = scenario2_colors[i];
				light_types[i] = 2;

			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Scenario 3"))
		{
			light_count = 12;
			for (int i = 0; i < 12; i++)
			{
				light_rotation[i] = scenario3_angles[i];
				light_diffuse[i] = scenario3_colors[i];
				light_ambient[i] = scenario3_colors[i];
				light_specular[i] = scenario3_colors[i];
				light_emmissive[i] = scenario3_colors[i];
				light_types[i] = i % 3;
			}
		}

		//I know this is literally the worst way to do this, but I couldn't figure out another way, so I am very sorry
		switch (light_count)
		{
		case 16:
		{
			if (ImGui::CollapsingHeader("light 16"))
			{
				ImGui::Combo("light type", &light_types[15], lights, 3);
				ImGui::SliderFloat("angular rotation", &light_rotation[15], 0.0f, 360.0f);
				if (light_types[15] == 2)
				{
					ImGui::SliderFloat("spotlight inner angle", &spotlight_inner_angles[15], 0.0f, 90.f);
					ImGui::SliderFloat("spotlight outer angle", &spotlight_outer_angles[15], 0.0f, 90.f);
					ImGui::SliderFloat("falloff value", &falloff_values[15], 0.0f, 10.0f);



				}
				ImGui::ColorEdit3("light ambient", (float*)&light_ambient[15]);
				ImGui::ColorEdit3("light diffuse", (float*)&light_diffuse[15]);
				ImGui::ColorEdit3("light specular", (float*)&light_specular[15]);
			}
		}
		case 15:
		{
			if (ImGui::CollapsingHeader("light 15"))
			{
				ImGui::Combo("light type", &light_types[14], lights, 3);
				ImGui::SliderFloat("angular rotation", &light_rotation[14], 0.0f, 360.0f);
				if (light_types[14] == 2)
				{
					ImGui::SliderFloat("spotlight inner angle", &spotlight_inner_angles[14], 0.0f, 90.f);
					ImGui::SliderFloat("spotlight outer angle", &spotlight_outer_angles[14], 0.0f, 90.f);
					ImGui::SliderFloat("falloff value", &falloff_values[14], 0.0f, 10.0f);



				}
				ImGui::ColorEdit3("light ambient", (float*)&light_ambient[14]);
				ImGui::ColorEdit3("light diffuse", (float*)&light_diffuse[14]);
				ImGui::ColorEdit3("light specular", (float*)&light_specular[14]);
			}
		}
		case 14:
		{
			if (ImGui::CollapsingHeader("light 14"))
			{
				ImGui::Combo("light type", &light_types[13], lights, 3);
				ImGui::SliderFloat("angular rotation", &light_rotation[13], 0.0f, 360.0f);
				if (light_types[13] == 2)
				{
					ImGui::SliderFloat("spotlight inner angle", &spotlight_inner_angles[13], 0.0f, 90.f);
					ImGui::SliderFloat("spotlight outer angle", &spotlight_outer_angles[13], 0.0f, 90.f);
					ImGui::SliderFloat("falloff value", &falloff_values[13], 0.0f, 10.0f);



				}
				ImGui::ColorEdit3("light ambient", (float*)&light_ambient[13]);
				ImGui::ColorEdit3("light diffuse", (float*)&light_diffuse[13]);
				ImGui::ColorEdit3("light specular", (float*)&light_specular[13]);
			}
		}
		case 13:
		{
			if (ImGui::CollapsingHeader("light 13"))
			{
				ImGui::Combo("light type", &light_types[12], lights, 3);
				ImGui::SliderFloat("angular rotation", &light_rotation[12], 0.0f, 360.0f);
				if (light_types[12] == 2)
				{
					ImGui::SliderFloat("spotlight inner angle", &spotlight_inner_angles[12], 0.0f, 90.f);
					ImGui::SliderFloat("spotlight outer angle", &spotlight_outer_angles[12], 0.0f, 90.f);
					ImGui::SliderFloat("falloff value", &falloff_values[12], 0.0f, 10.0f);



				}
				ImGui::ColorEdit3("light ambient", (float*)&light_ambient[12]);
				ImGui::ColorEdit3("light diffuse", (float*)&light_diffuse[12]);
				ImGui::ColorEdit3("light specular", (float*)&light_specular[12]);
			}
		}
		case 12:
		{
			if (ImGui::CollapsingHeader("light 12"))
			{
				ImGui::Combo("light type", &light_types[11], lights, 3);
				ImGui::SliderFloat("angular rotation", &light_rotation[11], 0.0f, 360.0f);
				if (light_types[11] == 2)
				{
					ImGui::SliderFloat("spotlight inner angle", &spotlight_inner_angles[11], 0.0f, 90.f);
					ImGui::SliderFloat("spotlight outer angle", &spotlight_outer_angles[11], 0.0f, 90.f);
					ImGui::SliderFloat("falloff value", &falloff_values[11], 0.0f, 10.0f);



				}
				ImGui::ColorEdit3("light ambient", (float*)&light_ambient[11]);
				ImGui::ColorEdit3("light diffuse", (float*)&light_diffuse[11]);
				ImGui::ColorEdit3("light specular", (float*)&light_specular[11]);
			}
		}
		case 11:
		{
			if (ImGui::CollapsingHeader("light 11"))
			{
				ImGui::Combo("light type", &light_types[10], lights, 3);
				ImGui::SliderFloat("angular rotation", &light_rotation[10], 0.0f, 360.0f);
				if (light_types[10] == 2)
				{
					ImGui::SliderFloat("spotlight inner angle", &spotlight_inner_angles[10], 0.0f, 90.f);
					ImGui::SliderFloat("spotlight outer angle", &spotlight_outer_angles[10], 0.0f, 90.f);
					ImGui::SliderFloat("falloff value", &falloff_values[10], 0.0f, 10.0f);



				}
				ImGui::ColorEdit3("light ambient", (float*)&light_ambient[10]);
				ImGui::ColorEdit3("light diffuse", (float*)&light_diffuse[10]);
				ImGui::ColorEdit3("light specular", (float*)&light_specular[10]);
			}
		}
		case 10:
		{
			if (ImGui::CollapsingHeader("light 10"))
			{
				ImGui::Combo("light type", &light_types[9], lights, 3);
				ImGui::SliderFloat("angular rotation", &light_rotation[9], 0.0f, 360.0f);
				if (light_types[9] == 2)
				{
					ImGui::SliderFloat("spotlight inner angle", &spotlight_inner_angles[9], 0.0f, 90.f);
					ImGui::SliderFloat("spotlight outer angle", &spotlight_outer_angles[9], 0.0f, 90.f);
					ImGui::SliderFloat("falloff value", &falloff_values[9], 0.0f, 10.0f);



				}
				ImGui::ColorEdit3("light ambient", (float*)&light_ambient[9]);
				ImGui::ColorEdit3("light diffuse", (float*)&light_diffuse[9]);
				ImGui::ColorEdit3("light specular", (float*)&light_specular[9]);
			}
		}
		case 9:
		{
			if (ImGui::CollapsingHeader("light 9"))
			{
				ImGui::Combo("light type", &light_types[8], lights, 3);
				ImGui::SliderFloat("angular rotation", &light_rotation[8], 0.0f, 360.0f);
				if (light_types[8] == 2)
				{
					ImGui::SliderFloat("spotlight inner angle", &spotlight_inner_angles[8], 0.0f, 90.f);
					ImGui::SliderFloat("spotlight outer angle", &spotlight_outer_angles[8], 0.0f, 90.f);
					ImGui::SliderFloat("falloff value", &falloff_values[8], 0.0f, 10.0f);



				}
				ImGui::ColorEdit3("light ambient", (float*)&light_ambient[8]);
				ImGui::ColorEdit3("light diffuse", (float*)&light_diffuse[8]);
				ImGui::ColorEdit3("light specular", (float*)&light_specular[8]);
			}
		}
		case 8:
		{
			if (ImGui::CollapsingHeader("light 8"))
			{
				ImGui::Combo("light type", &light_types[7], lights, 3);
				ImGui::SliderFloat("angular rotation", &light_rotation[7], 0.0f, 360.0f);
				if (light_types[7] == 2)
				{
					ImGui::SliderFloat("spotlight inner angle", &spotlight_inner_angles[7], 0.0f, 90.f);
					ImGui::SliderFloat("spotlight outer angle", &spotlight_outer_angles[7], 0.0f, 90.f);
					ImGui::SliderFloat("falloff value", &falloff_values[7], 0.0f, 10.0f);



				}
				ImGui::ColorEdit3("light ambient", (float*)&light_ambient[7]);
				ImGui::ColorEdit3("light diffuse", (float*)&light_diffuse[7]);
				ImGui::ColorEdit3("light specular", (float*)&light_specular[7]);
			}
		}
		case 7:
		{
			if (ImGui::CollapsingHeader("light 7"))
			{
				ImGui::Combo("light type", &light_types[6], lights, 3);
				ImGui::SliderFloat("angular rotation", &light_rotation[6], 0.0f, 360.0f);
				if (light_types[6] == 2)
				{
					ImGui::SliderFloat("spotlight inner angle", &spotlight_inner_angles[6], 0.0f, 90.f);
					ImGui::SliderFloat("spotlight outer angle", &spotlight_outer_angles[6], 0.0f, 90.f);
					ImGui::SliderFloat("falloff value", &falloff_values[6], 0.0f, 10.0f);



				}
				ImGui::ColorEdit3("light ambient", (float*)&light_ambient[6]);
				ImGui::ColorEdit3("light diffuse", (float*)&light_diffuse[6]);
				ImGui::ColorEdit3("light specular", (float*)&light_specular[6]);
			}
		}
		case 6:
		{
			if (ImGui::CollapsingHeader("light 6"))
			{
				ImGui::Combo("light type", &light_types[5], lights, 3);
				ImGui::SliderFloat("angular rotation", &light_rotation[5], 0.0f, 360.0f);
				if (light_types[5] == 2)
				{
					ImGui::SliderFloat("spotlight inner angle", &spotlight_inner_angles[5], 0.0f, 90.f);
					ImGui::SliderFloat("spotlight outer angle", &spotlight_outer_angles[5], 0.0f, 90.f);
					ImGui::SliderFloat("falloff value", &falloff_values[5], 0.0f, 10.0f);



				}
				ImGui::ColorEdit3("light ambient", (float*)&light_ambient[5]);
				ImGui::ColorEdit3("light diffuse", (float*)&light_diffuse[5]);
				ImGui::ColorEdit3("light specular", (float*)&light_specular[5]);
			}
		}
		case 5:
		{
			if (ImGui::CollapsingHeader("light 5"))
			{
				ImGui::Combo("light type", &light_types[4], lights, 3);
				ImGui::SliderFloat("angular rotation", &light_rotation[4], 0.0f, 360.0f);
				if (light_types[4] == 2)
				{
					ImGui::SliderFloat("spotlight inner angle", &spotlight_inner_angles[4], 0.0f, 90.f);
					ImGui::SliderFloat("spotlight outer angle", &spotlight_outer_angles[4], 0.0f, 90.f);
					ImGui::SliderFloat("falloff value", &falloff_values[4], 0.0f, 10.0f);



				}
				ImGui::ColorEdit3("light ambient", (float*)&light_ambient[4]);
				ImGui::ColorEdit3("light diffuse", (float*)&light_diffuse[4]);
				ImGui::ColorEdit3("light specular", (float*)&light_specular[4]);
			}
		}
		case 4:
		{
			if (ImGui::CollapsingHeader("light 4"))
			{
				ImGui::Combo("light type", &light_types[3], lights, 3);
				ImGui::SliderFloat("angular rotation", &light_rotation[3], 0.0f, 360.0f);
				if (light_types[3] == 2)
				{
					ImGui::SliderFloat("spotlight inner angle", &spotlight_inner_angles[3], 0.0f, 90.f);
					ImGui::SliderFloat("spotlight outer angle", &spotlight_outer_angles[3], 0.0f, 90.f);
					ImGui::SliderFloat("falloff value", &falloff_values[3], 0.0f, 10.0f);


					
				}
				ImGui::ColorEdit3("light ambient", (float*)&light_ambient[3]);
				ImGui::ColorEdit3("light diffuse", (float*)&light_diffuse[3]);
				ImGui::ColorEdit3("light specular", (float*)&light_specular[3]);
			}
		}
		case 3:
		{
			if (ImGui::CollapsingHeader("light 3"))
			{
				ImGui::Combo("light type", &light_types[2], lights, 3);
				ImGui::SliderFloat("angular rotation", &light_rotation[2], 0.0f, 360.0f);
				if (light_types[2] == 2)
				{
					ImGui::SliderFloat("spotlight inner angle", &spotlight_inner_angles[2], 0.0f, 90.f);
					ImGui::SliderFloat("spotlight outer angle", &spotlight_outer_angles[2], 0.0f, 90.f);
					ImGui::SliderFloat("falloff value", &falloff_values[2], 0.0f, 10.0f);

				}
				ImGui::ColorEdit3("light ambient", (float*)&light_ambient[2]);
				ImGui::ColorEdit3("light diffuse", (float*)&light_diffuse[2]);
				ImGui::ColorEdit3("light specular", (float*)&light_specular[2]);
			}
		}
		case 2:
		{
			if (ImGui::CollapsingHeader("light 2"))
			{
				ImGui::Combo("light type",&light_types[1], lights, 3);
				ImGui::SliderFloat("angular rotation", &light_rotation[1], 0.0f, 360.0f);
				if (light_types[1] == 2)
				{
					ImGui::SliderFloat("spotlight inner angle", &spotlight_inner_angles[1], 0.0f, 90.f);
					ImGui::SliderFloat("spotlight outer angle", &spotlight_outer_angles[1], 0.0f, 90.f);
					ImGui::SliderFloat("falloff value", &falloff_values[1], 0.0f, 10.0f);

				}

				ImGui::ColorEdit3("light ambient", (float*)&light_ambient[1]);
				ImGui::ColorEdit3("light diffuse", (float*)&light_diffuse[1]);
				ImGui::ColorEdit3("light specular", (float*)&light_specular[1]);
			}
		}
		case 1:
		{
			if (ImGui::CollapsingHeader("light 1"))
			{
				ImGui::Combo("light type", &light_types[0], lights, 3);
				ImGui::SliderFloat("angular rotation", &light_rotation[0], 0.0f, 360.0f);
				if (light_types[0] == 2)
				{
					ImGui::SliderFloat("spotlight inner angle", &spotlight_inner_angles[0], 0.0f, 90.f);
					ImGui::SliderFloat("spotlight outer angle", &spotlight_outer_angles[0], 0.0f, 90.f);
					ImGui::SliderFloat("falloff value", &falloff_values[0], 0.0f, 10.0f);
				}
				ImGui::ColorEdit3("light ambient", (float*)&light_ambient[0]);
				ImGui::ColorEdit3("light diffuse", (float*)&light_diffuse[0]);
				ImGui::ColorEdit3("light specular", (float*)&light_specular[0]);
			}
		}
		}



	}
		ImGui::End();
}

void renderGUI()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void shutdownGUI()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

bool drawVertNormals()
{
	return draw_vertex_normals;
}
bool drawFragNormals()
{
	return draw_fragment_normals;
}
bool drawUVGrid()
{
	return draw_uv_grid;
}
bool hasModelChanged()
{
	return (current_item != current_item_previous);
}

int giveCurrentItem()
{
	return current_item;
}

int* getLightCount()
{
	return &light_count;
}
float* getLightRotations()
{
	return &light_rotation[0];
}

int* getLightTypes()
{
	return &light_types[0];
}

glm::vec3* getLightDiffuse()
{
	return &light_diffuse[0];
}

glm::vec3* getLightAmbient()
{
	return &light_ambient[0];
}

glm::vec3* getLightSpecular()
{
	return &light_specular[0];
}

float* getLightAngles_inner()
{
	return &spotlight_inner_angles[0];
}

float* getLightAngles_outer()
{
	return &spotlight_outer_angles[0];
}

float* getLightFalloff()
{
	return &falloff_values[0];
}

float* getCameraRotation()
{
	return &camera_rotation;
}


// getter functions for surface offset attributes
glm::vec3* getSurfaceAmbient()
{
	return &surface_ambient;
}

glm::vec3* getSurfaceDiffuse()
{
	return &surface_diffuse;
}

glm::vec3* getSurfaceSpecular()
{
	return &surface_specular;
}

glm::vec3* getSurfaceEmmissive()
{
	return &surface_emmissive;
}

bool hasShaderChanged()
{
	return (current_shader != current_shader_previous);

}

bool hasUVChanged()
{
	return (prev_UV_CalculationType != UV_CalculationType);
}

int* calculateUVCPU()
{
	return (int*) &calculateuv_CPU;
}

int giveCurrentShader()
{
	return current_shader;
}

glm::vec3* getFogColor()
{
	return &fog_color;
}
glm::vec3* getGlobalAmbientColor()
{
	return &global_ambient;
}
float* getAttenuationConstants()
{
	return attenuation_constants;
}
float* getFogDistances()
{
	return fog_distances;
}

int* getUVCalculationType()
{
	return &UV_CalculationType;
}

bool hasTextureChanged()
{
	return (previous_showgrid != draw_uv_grid);
}

bool stopOrbit()
{
	return stop_orbit;
}
float gui_getdt()
{
	return dt;
}

int get_rendertype()
{
	return render_type;
}

int getBVSphereType()
{
	return Bounding_sphere_type;
}

bool hasBVsphereChanged()
{
	return Bounding_sphere_type != previous_Bounding_sphere_type;
}

int getTopdownSplitType()
{
	return Topdown_Split_type;
}
int getBottomUpMergeType()
{
	return BottomUp_Merge_type;
}

bool getBVdraw()
{
	return draw_BVs;
}

bool getOctreedraw()
{
	return draw_Octree;
}

bool getBVTreedraw()
{
	return draw_BV_Trees;
}
bool getBVSphere()
{
	return draw_BV_Sphere;
}

bool getBVTreeType()
{
	return BV_Tree_Bottom_up;
}

bool BVTreeChanged()
{
	return (BV_Tree_Bottom_up != previous_BV_Tree_Bottom_up)
		|| (previous_BottomUp_Merge_type != BottomUp_Merge_type)
		|| (previous_Topdown_Split_type != Topdown_Split_type)
		|| (previous_draw_BV_Sphere != draw_BV_Sphere);
}

bool display_section4()
{
	return Show_Section4;

}
bool display_section5()
{
	return Show_Section5;

}
bool display_section6()
{
	return Show_Section6;
}


bool powerplantChanged()
{
	return (prev_Show_Section4 != Show_Section4)
		|| (prev_Show_Section5 != Show_Section5)
		|| (prev_Show_Section6 != Show_Section6);

}

int getdrawdepth()
{
	return BVH_DrawDepth;
}

bool get_camera_setting()
{
	return !view_power_plant_full;
}

bool has_camera_changed()
{
	return view_power_plant_full != previous_view_power_plant_full;

}

bool get_camera_wireframe()
{
	return view_power_plant_wire;
}

bool has_wireframe_changed()
{
	return view_power_plant_wire != previous_view_power_plant_wire;

}