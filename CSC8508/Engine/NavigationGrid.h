#pragma once
#include "NavigationMap.h"
#include "QuadTree.h"

#include "../../Common/Vector2.h"

#include <string>
#include <set>

namespace NCL {
	namespace CSC8508 {

		struct GridNode {
			GridNode* parent;

			GridNode* connected[8];
			float		  costs[8];

			Vector3		position;

			float f;
			float g;

			int type;

			GridNode() {
				for (int i = 0; i < 8; ++i) {
					connected[i] = nullptr;
					costs[i] = 0;
				}
				f = 0;
				g = 0;
				type = 0;
				parent = nullptr;
			}
			~GridNode() {	}
		};

		inline bool operator< (const GridNode& lhs, const GridNode& rhs) {
			return lhs.f < rhs.f;
		}
		struct GridPtrComparison {
			//Return true if a's total cost is less that b's total cost.
			bool operator()(const GridNode* a, const GridNode* b) const {

				//This distinction is important as sets believe objects equal if comparator
				//returns false no matter the order in which the elements are passed in as arguments.
				if (a->f == b->f)
					return a < b;
				else
					return a->f < b->f;
			}
		};

		typedef std::set<GridNode*, GridPtrComparison> NodeSet;

		class NavigationGrid : public NavigationMap	{

		public:
			NavigationGrid();
			NavigationGrid(const std::string&filename, Vector3 offSet = Vector3(0,0,0));
			NavigationGrid(QuadTree<GameObject*>* objectTree, Vector3 offset, float maxHeight, Vector2 gridDims, float nodeSize);
			~NavigationGrid();

			bool FindPath(const Vector3& from, const Vector3& to, NavigationPath& outPath, float maximumCost = INFINITY) override;
				
		protected:
			void		BuildConnections();
			bool		NodeInSet(GridNode* n, NodeSet& set) const;
			float		Heuristic(GridNode* hNode, GridNode* endNode) const;
			float nodeSize;
			int gridWidth;
			int gridHeight;

			GridNode* allNodes;
			
			//This vector represents where the top left corner of the grid is in the game world.
			Vector3 offset;
		};
	}
}

