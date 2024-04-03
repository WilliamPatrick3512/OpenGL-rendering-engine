#include "BV_Heirarchy.h"
#include <algorithm>

bool bvsort_x(Bounding_Volume*&objA, Bounding_Volume*& objB)
{
	return objA->get_center().x < objB->get_center().x;
}

bool bvsort_y(Bounding_Volume*& objA, Bounding_Volume*& objB)
{
	return objA->get_center().y < objB->get_center().y;
}
bool bvsort_z(Bounding_Volume*& objA, Bounding_Volume*& objB)
{
	return objA->get_center().z < objB->get_center().z;
}


void BV_Heirarchy::construct_topdown(BV_Node*& node, Bounding_Volume* objects[], int numObjects, int depth)
{
	int k = -1;
	node->depth = depth;
	Bounding_Sphere* node_data_sphere = new Bounding_Sphere();
	AABB* node_data_AABB = new AABB();
	node_data_AABB->calculate_AABB((AABB**)objects, numObjects);
	node_data_sphere->calculate_Bsphere_Spheres((Bounding_Sphere**)objects, numObjects);

	if (sphere_tree)
	{
		node->volume_data = node_data_sphere;
	}
	else
	{
		node->volume_data = node_data_AABB;
		delete node_data_sphere;
	}

	if (numObjects <= MIN_OBJECTS_AT_LEAF || depth >= 7)
	{
		node->type = bv_treetype::leaf;
		node->object_count = numObjects;
	}
	else
	{
		glm::vec3 aabbExtents = (node_data_AABB->aabb_getmax() - node_data_AABB->aabb_getmin());

		if (aabbExtents.x > aabbExtents.y&& aabbExtents.x > aabbExtents.z)
		{
			node->partition_axis = bv_partition_axis::x_axis;
			std::sort(objects, objects + numObjects, bvsort_x);

		}
		else if (aabbExtents.y > aabbExtents.x&& aabbExtents.y > aabbExtents.z)
		{
			node->partition_axis = bv_partition_axis::y_axis;
			std::sort(objects, objects + numObjects, bvsort_y);
		}
		else
		{
			node->partition_axis = bv_partition_axis::z_axis;
			std::sort(objects, objects + numObjects, bvsort_z);
		}

		//delete un-needed data once it is no longer useful
		if (sphere_tree)
		{
			delete node_data_AABB;
		}

		node->type = bv_treetype::internal;
		node->object_count = numObjects;
		k = PartitionObjects(node, objects, numObjects);

		// Allocate lChild & rChild pointers
		node->left_child = new BV_Node();
		node->right_child = new BV_Node();
		// Recurse down the subtrees
		construct_topdown(node->left_child, &objects[0], k, depth + 1);
		construct_topdown(node->right_child, &objects[k], numObjects - k, depth + 1);
	}
}

BV_Heirarchy::BV_Node* BV_Heirarchy::construct_bottomup(std::vector<BV_Node*>& nodes)
{
	while (nodes.size() > 1)
	{
		int first = -1, second = -1;
		FindNodesToMerge(nodes, first, second, sphere_tree);
		BV_Node* parent = new BV_Node();

		// Remove first & second from the vector
		parent->left_child = nodes[first];
		parent->right_child = nodes[second];
		
		Bounding_Sphere* node_data_sphere = new Bounding_Sphere();
		AABB* node_data_AABB = new AABB();
		
		if (sphere_tree)
		{
			node_data_sphere->calculate_Bsphere_Spheres(*(Bounding_Sphere*)nodes[first]->volume_data, *(Bounding_Sphere*)nodes[second]->volume_data);
			parent->volume_data = node_data_sphere;
			delete node_data_AABB;
		}
		else
		{
			node_data_AABB->calculate_AABB(*(AABB*)nodes[first]->volume_data, *(AABB*)nodes[second]->volume_data);
			parent->volume_data = node_data_AABB;
			delete node_data_sphere;
		}
		parent->depth = std::max(std::min(nodes[first]->depth, nodes[second]->depth) - 1, 0);
		parent->object_count = nodes[first]->object_count + nodes[second]->object_count;

		nodes.erase(nodes.begin() + second);
		nodes.erase(nodes.begin() + first);

		// Add the parent node to the vector
		nodes.push_back(parent);
	}
	return nodes[0]; // return the root node
}

std::vector<BV_Heirarchy::BV_Node*> BV_Heirarchy::construct_nodelist(std::vector<fileObject> object_list)
{
	std::vector<BV_Node*> nodelist;
	for (int i = 0; i < object_list.size(); i++)
	{
		BV_Node* parent = new BV_Node();

		//parent->volume_data = &object_list[i].bounding_volume;

	}
	return nodelist;
}

void BV_Heirarchy::tree_set_parameters(enum class bv_merge_type merge, enum class bv_split_type split, bool sphere)
{
	merge_type = merge;
	split_type = split;
	sphere_tree = sphere;
}

//assume objects are pre-sorted along each axis
int BV_Heirarchy::PartitionObjects(BV_Node*& node, Bounding_Volume* objects[], int numObjects)
{
	int result_index = 0;

	glm::vec3 mean;
	float median;

	if (split_type == bv_split_type::mean_centers)
	{
		
		glm::vec3 median_center = objects[numObjects / 2]->get_center();
		for (int i = 0; i < numObjects; i++)
		{
			//printf("%f - %f - %f\n", objects[i]->get_center().x, objects[i]->get_center().y, objects[i]->get_center().z);
			bool check_x = ((node->partition_axis == bv_partition_axis::x_axis) && (objects[i]->get_center().x >= median_center.x));
			bool check_y = ((node->partition_axis == bv_partition_axis::y_axis) && (objects[i]->get_center().y >= median_center.y));
			bool check_z = ((node->partition_axis == bv_partition_axis::z_axis) && (objects[i]->get_center().z >= median_center.z));
			
			if (check_x || check_y || check_z)
			{

				return i;
			}
		}
	}
	else if (split_type == bv_split_type::median_extents)
	{
		std::vector<float> extents;

		for (int i = 0; i < numObjects; i++)
		{
			if (sphere_tree)
			{
				if (node->partition_axis == bv_partition_axis::x_axis)
				{
					extents.push_back((objects[i]->center - glm::vec3(((Bounding_Sphere*)objects[i])->radius)).x);
					extents.push_back((objects[i]->center + glm::vec3(((Bounding_Sphere*)objects[i])->radius)).x);
				}
				else if (node->partition_axis == bv_partition_axis::y_axis)
				{
					extents.push_back((objects[i]->center - glm::vec3(((Bounding_Sphere*)objects[i])->radius)).y);
					extents.push_back((objects[i]->center + glm::vec3(((Bounding_Sphere*)objects[i])->radius)).y);
				}
				else
				{
					extents.push_back((objects[i]->center - glm::vec3(((Bounding_Sphere*)objects[i])->radius)).z);
					extents.push_back((objects[i]->center + glm::vec3(((Bounding_Sphere*)objects[i])->radius)).z);
				}
			}
			else
			{
				if (node->partition_axis == bv_partition_axis::x_axis)
				{
					extents.push_back((((AABB*)objects[i])->aabb_getmax()).x);
					extents.push_back((((AABB*)objects[i])->aabb_getmin()).x);
				}													  
				else if (node->partition_axis == bv_partition_axis::y_axis)
				{													  
					extents.push_back((((AABB*)objects[i])->aabb_getmax()).y);
					extents.push_back((((AABB*)objects[i])->aabb_getmin()).y);
				}													   
				else												   
				{													   
					extents.push_back((((AABB*)objects[i])->aabb_getmax()).z);
					extents.push_back((((AABB*)objects[i])->aabb_getmin()).z);
				}
			}
		}
		std::sort(extents.begin(), extents.end());

		median = extents[extents.size() / 2];

		for (int i = 0; i < numObjects; i++)
		{
			bool check_x = node->partition_axis == bv_partition_axis::x_axis && objects[i]->get_center().x > median;
			bool check_y = node->partition_axis == bv_partition_axis::y_axis && objects[i]->get_center().y > median;
			bool check_z = node->partition_axis == bv_partition_axis::z_axis && objects[i]->get_center().z > median;

			if (check_x || check_y || check_z)
			{
				return i;
			}
		}

	}
	else
	{
		//grab the index with equal numbers on each side
		return numObjects / 2;
	}
}

void BV_Heirarchy::FindNodesToMerge(std::vector<BV_Node*>& nodes, int& first, int& second, bool sphere_tree)
{
	if (merge_type == bv_merge_type::nearest_neighbor)
	{
		float min_distance = std::numeric_limits<float>::max();
		float min_volume = std::numeric_limits<float>::max();
		for (int i = 0; i < nodes.size() - 1; i++)
		{
			for (int j = i + 1; j < nodes.size(); j++)
			{
				Bounding_Volume* BV_A = nodes[j]->volume_data;
				Bounding_Volume* BV_B = nodes[i]->volume_data;
				float volume = 0.0f;
				if (!sphere_tree)
				{
					AABB* BV_Volume = new AABB;

					BV_Volume->calculate_AABB(*(AABB*)BV_A, *(AABB*)BV_B);
					volume = BV_Volume->get_volume();
					delete BV_Volume;
				}
				else
				{
					Bounding_Sphere* BV_Volume = new Bounding_Sphere;

					BV_Volume->calculate_Bsphere_Spheres(*(Bounding_Sphere*)BV_A, *(Bounding_Sphere*)BV_B);
					volume = BV_Volume->get_volume();
					delete BV_Volume;

				}

				if (glm::length(BV_A->get_center() - BV_B->get_center()) < min_distance || volume < min_volume)
				{


					first = i;
					second = j;
					min_distance = glm::length(BV_A->get_center() - BV_B->get_center());
					min_volume = volume;
				}

			}
		}
	}

	
}