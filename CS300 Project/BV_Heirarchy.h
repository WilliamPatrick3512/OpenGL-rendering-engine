#pragma once
#include "Bounding_Volume.h"
#include "Bounding_Sphere.h"
#include "AABB.h"
#include "fileObject.h"

#define BVH_MAXDEPTH 7
#define MIN_OBJECTS_AT_LEAF 1


enum class bv_partition_axis
{
	x_axis,
	y_axis,
	z_axis
};
class BV_Heirarchy
{




public:


	enum class bv_treetype
	{
		internal,
		leaf
	};
	enum class bv_split_type
	{

		mean_centers,
		median_extents,
		K_evensplit
	};
	struct BV_Node
	{
		BV_Node* left_child;
		BV_Node* right_child;
		bv_partition_axis partition_axis;

		int object_count;
		int depth;

		bv_treetype type;

		Bounding_Volume* volume_data;
	};
	enum class bv_merge_type
	{
		nearest_neighbor,
		minimum_combined_volume,
		minimum_relative_increase
	};
	bv_split_type split_type;

	bv_merge_type merge_type;
	bool sphere_tree;

	void tree_set_parameters(enum class bv_merge_type merge, enum class bv_split_type split, bool sphere);

	void construct_topdown(BV_Node*& node, Bounding_Volume* objects[], int numObjects, int depth);
	BV_Node* construct_bottomup(std::vector<BV_Node*> &nodes);

	std::vector<BV_Node*> construct_nodelist(std::vector<fileObject> object_list);
	
	int PartitionObjects(BV_Node*& node, Bounding_Volume* objects[], int numObjects);
	void FindNodesToMerge(std::vector<BV_Node*>& nodes, int& first, int& second, bool sphere_tree);
};

