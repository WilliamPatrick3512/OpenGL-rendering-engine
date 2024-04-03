#pragma once
#include "Bounding_Volume.h"
#include "Bounding_Sphere.h"
#include "AABB.h"
#include "fileObject.h"

#define MAX_TRIS_AT_LEAF 50000
#define POS_XYZ 0 
#define POS_XY_NEG_Z 1
#define POS_XZ_NEG_Y 2
#define POS_YZ_NEG_X 3
#define POS_X_NEG_YZ 4
#define POS_Y_NEG_XZ 5
#define POS_Z_NEG_XY 6
#define NEG_XYZ 7
#define OCTREE_CHILDREN_COUNT 8
class Octree
{

public:


	enum class octree_node_type
	{
		internal,
		leaf
	};

	struct Octree_Node
	{
		int tri_count;
		//IMPORTANT
		//ORDER OF CHILDREN DEFINED ABOVE
		Octree_Node* Children[8];
		octree_node_type type;
		AABB* extents;
		
		glm::vec3 color;
		
		int depth;

		std::vector<glm::vec3> volume_data;
		std::vector<glm::vec3> wireframe;
		void Add_Tri(glm::vec3 point_A, glm::vec3 point_B, glm::vec3 point_C);
	};

	Octree_Node* root;

	Octree();

	void calculate_octree(Octree_Node* node, std::vector<glm::vec3> input_geometry);

};

