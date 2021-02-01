#pragma once
#include "NavigationMap.h"
#include "../../Common/Plane.h"
#include <string>
#include <vector>
#include <set>

//This article was handy for figuring out Simple Stupid Funnel Algorithm: http://jceipek.com/Olin-Coding-Tutorials/pathing.html

namespace NCL {
	namespace CSC8508 {

		struct NavTri {
			Plane   triPlane;
			Vector3 centroid;
			float	area;
			NavTri* neighbours[3];

			int indices[3];

			NavTri() {
				area = 0.0f;
				neighbours[0] = nullptr;
				neighbours[1] = nullptr;
				neighbours[2] = nullptr;

				indices[0] = -1;
				indices[1] = -1;
				indices[2] = -1;
			}
		};

		struct PathNode {
			const NavTri* tri;
			float g;
			float h;
			float f;
			PathNode* parent;
			int left;
			int right;
		};

		struct PathNodeComparison {
			//Return true if a's total cost is less that b's total cost.
			bool operator()(const PathNode* a, const PathNode* b) const {

				//This distinction is important as sets believe objects equal if comparator
				//returns false no matter the order in which the elements are passed in as arguments.
				//Meaning objects with equal heuristics could not co-exist.
				if (a->f == b->f)
					return a < b;
				else
					return a->f < b->f;
			}
		};

		typedef std::set<PathNode*, PathNodeComparison> NodeSet;

		class NavigationMesh : public NavigationMap {
		public:
			NavigationMesh();
			NavigationMesh(const std::string& filename);
			~NavigationMesh();

			bool FindPath(const Vector3& from, const Vector3& to, NavigationPath& outPath, float maximumCost = 0.0f) override;

		protected:
			

			const NavTri*	GetTriForPosition(const Vector3& pos) const;
			PathNode*		NodeInSet(PathNode* n, NodeSet& set) const;

			std::vector<NavTri>		allTris;
			std::vector<Vector3>	allVerts;
		};
	}
}