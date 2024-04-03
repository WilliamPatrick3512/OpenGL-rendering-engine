#include "Octree.h"

Octree::Octree(): root(nullptr)
{
	
}

void Octree::calculate_octree(Octree_Node* node, std::vector<glm::vec3> input_geometry)
{
	//subdivide if there is too much geometry
	if (node->tri_count > MAX_TRIS_AT_LEAF)
	{

		node->type = octree_node_type::internal;
		//create each cell and the AABB associated
		node->Children[POS_XYZ] = new Octree_Node;
		node->Children[POS_XYZ]->extents = new AABB;
		node->Children[POS_XYZ]->extents->calculate_AABB(node->extents->center, node->extents->aabb_getmax());
			  
		//for following children the aabb will be calculated by the center, and a combination of maxes and mins
		node->Children[POS_XY_NEG_Z] = new Octree_Node;
		node->Children[POS_XY_NEG_Z]->extents = new AABB;
		node->Children[POS_XY_NEG_Z]->extents->calculate_AABB(node->extents->get_center(),
										 glm::vec3(node->extents->aabb_getmax().x,
												   node->extents->aabb_getmax().y,
												   node->extents->aabb_getmin().z));
			
		node->Children[POS_XZ_NEG_Y] = new Octree_Node;
		node->Children[POS_XZ_NEG_Y]->extents = new AABB;
		node->Children[POS_XZ_NEG_Y]->extents->calculate_AABB(node->extents->get_center(),
										 glm::vec3(node->extents->aabb_getmax().x,
												   node->extents->aabb_getmin().y,
												   node->extents->aabb_getmax().z));
		
		
		node->Children[POS_YZ_NEG_X] = new Octree_Node;
		node->Children[POS_YZ_NEG_X]->extents = new AABB;
		node->Children[POS_YZ_NEG_X]->extents->calculate_AABB(node->extents->get_center(),
										 glm::vec3(node->extents->aabb_getmin().x,
										 		   node->extents->aabb_getmax().y,
										 		   node->extents->aabb_getmax().z));
			  
		node->Children[POS_X_NEG_YZ] = new Octree_Node;
		node->Children[POS_X_NEG_YZ]->extents = new AABB;
		node->Children[POS_X_NEG_YZ]->extents->calculate_AABB(node->extents->get_center(),
										 glm::vec3(node->extents->aabb_getmax().x,
										 		   node->extents->aabb_getmin().y,
										 		   node->extents->aabb_getmin().z));
			  
		node->Children[POS_Y_NEG_XZ] = new Octree_Node;
		node->Children[POS_Y_NEG_XZ]->extents = new AABB;
		node->Children[POS_Y_NEG_XZ]->extents->calculate_AABB(node->extents->get_center(),
										 glm::vec3(node->extents->aabb_getmin().x,
										 		   node->extents->aabb_getmax().y,
										 		   node->extents->aabb_getmin().z));
			  
		node->Children[POS_Z_NEG_XY] = new Octree_Node;
		node->Children[POS_Z_NEG_XY]->extents = new AABB;
		node->Children[POS_Z_NEG_XY]->extents->calculate_AABB(node->extents->get_center(),
										 glm::vec3(node->extents->aabb_getmin().x,
											       node->extents->aabb_getmin().y,
												   node->extents->aabb_getmax().z));
			  
		node->Children[NEG_XYZ] = new Octree_Node;
		node->Children[NEG_XYZ]->extents = new AABB;
		node->Children[NEG_XYZ]->extents->calculate_AABB(node->extents->center, node->extents->aabb_getmin());
		//end calculating AABBs for each child

		for (int i = 0; i < OCTREE_CHILDREN_COUNT; i++)
		{
			node->Children[i]->tri_count = 0;
			node->Children[i]->depth = node->depth + 1;
		}

		//for each object, search through the geometry
		for (int i = 0; i < input_geometry.size(); i += 3)
		{

			glm::vec3 Point_A = input_geometry[i];
			glm::vec3 Point_B = input_geometry[i + 1];
			glm::vec3 Point_C = input_geometry[i + 2];


			//for each of the potential children
			for (int j = 0; j < OCTREE_CHILDREN_COUNT; j++)
			{
				bool check_A = node->Children[j]->extents->check_point(Point_A);
				bool check_B = node->Children[j]->extents->check_point(Point_B);
				bool check_C = node->Children[j]->extents->check_point(Point_C);

				//Best case scenario
				if(check_A && check_B && check_C)
				{
					node->Children[j]->Add_Tri(Point_A, Point_B, Point_C);
					node->Children[j]->tri_count += 1;
					break;
				}
				
				//check if none are present
				else if (!check_A && !check_B && !check_C)
				{
					
				}

				//if only one point is present in the child
				else if (check_A && !check_B && !check_C)
				{
					glm::vec3 intercept_1 = node->Children[j]->extents->find_intercept(Point_A, Point_B);
					glm::vec3 intercept_2 = node->Children[j]->extents->find_intercept(Point_A, Point_C);
					node->Children[j]->Add_Tri(Point_A, intercept_1, intercept_2);
					node->Children[j]->tri_count += 1;
				}
				else if (!check_A && check_B && !check_C)
				{
					glm::vec3 intercept_1 = node->Children[j]->extents->find_intercept(Point_B, Point_A);
					glm::vec3 intercept_2 = node->Children[j]->extents->find_intercept(Point_B, Point_C);
					node->Children[j]->Add_Tri(Point_B, intercept_1, intercept_2);
					node->Children[j]->tri_count += 1;
				}
				else if (!check_A && !check_B && check_C)
				{
					glm::vec3 intercept_1 = node->Children[j]->extents->find_intercept(Point_C, Point_A);
					glm::vec3 intercept_2 = node->Children[j]->extents->find_intercept(Point_C, Point_B);
					node->Children[j]->Add_Tri(Point_C, intercept_1, intercept_2);
					node->Children[j]->tri_count += 1;
				}
				//checks for if two points are in the current child
				else if (check_A && check_B && !check_C)
				{
					glm::vec3 intercept_1 = node->Children[j]->extents->find_intercept(Point_A, Point_C);
					glm::vec3 intercept_2 = node->Children[j]->extents->find_intercept(Point_B, Point_C);
					node->Children[j]->Add_Tri(Point_A, intercept_1, intercept_2);
					node->Children[j]->Add_Tri(Point_A, Point_B, intercept_2);
					node->Children[j]->tri_count += 2;
				}
				else if (check_A && !check_B && check_C)
				{
					glm::vec3 intercept_1 = node->Children[j]->extents->find_intercept(Point_A, Point_B);
					glm::vec3 intercept_2 = node->Children[j]->extents->find_intercept(Point_C, Point_B);
					node->Children[j]->Add_Tri(Point_A, intercept_1, intercept_2);
					node->Children[j]->Add_Tri(Point_A, Point_C, intercept_2);
					node->Children[j]->tri_count += 2;
				}
				else if (!check_A && check_B && check_C)
				{
					glm::vec3 intercept_1 = node->Children[j]->extents->find_intercept(Point_B, Point_A);
					glm::vec3 intercept_2 = node->Children[j]->extents->find_intercept(Point_C, Point_A);
					node->Children[j]->Add_Tri(Point_B, intercept_1, intercept_2);
					node->Children[j]->Add_Tri(Point_B, Point_C, intercept_2);
					node->Children[j]->tri_count += 2;
				}
				
			}
		}
		//recurse down each valid child
		for (int i = 0; i < OCTREE_CHILDREN_COUNT; i++)
		{
			if (node->Children[i]->tri_count != 0)
			{
				calculate_octree(node->Children[i], node->Children[i]->volume_data);
			}
			else
			{
				delete node->Children[i];
				node->Children[i] = nullptr;
			}
		}

	}
	//continued off of "if" at start of the function, divides geometry into a wireframe
	else
	{
		node->type = octree_node_type::leaf;
		node->wireframe.clear();

		float r = rand() / static_cast<float>(RAND_MAX);
		float g = rand() / static_cast<float>(RAND_MAX);
		float b = rand() / static_cast<float>(RAND_MAX);

		glm::vec3 color = glm::vec3(r, g, b);

		node->color = color;

		for (int i = 0; i < node->volume_data.size(); i += 3)
		{
			node->wireframe.push_back(node->volume_data[i]);
			node->wireframe.push_back(node->volume_data[i + 1]);

			node->wireframe.push_back(node->volume_data[i]);
			node->wireframe.push_back(node->volume_data[i + 2]);

			node->wireframe.push_back(node->volume_data[i + 2]);
			node->wireframe.push_back(node->volume_data[i + 1]);
		}
		for (int i = 0; i < OCTREE_CHILDREN_COUNT; i++)
		{
			if (node->Children[i])
			{
				node->Children[i] = nullptr;
			}
		}

	}
}

void Octree::Octree_Node::Add_Tri(glm::vec3 point_A, glm::vec3 point_B, glm::vec3 point_C)
{
	volume_data.push_back(point_A);
	volume_data.push_back(point_B);
	volume_data.push_back(point_C);
}