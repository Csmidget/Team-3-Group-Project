#include "NavigationGrid.h"
#include "TraversableObject.h"

#include "../../Common/Assets.h"

#include <fstream>
#include <set>

using namespace NCL;
using namespace CSC8508;

const int LEFT_NODE		= 0;
const int RIGHT_NODE	= 1;
const int TOP_NODE		= 2;
const int BOTTOM_NODE	= 3;

const char WALL_NODE	= 'x';
const char FLOOR_NODE	= '.';
const char SWAMP_NODE	= 'o';

NavigationGrid::NavigationGrid()	{
	nodeSize	= 0;
	gridWidth	= 0;
	gridHeight	= 0;
	allNodes	= nullptr;
}

//Builds a grid based on provided quadtree of objects. (In use, this will be the static quadtree of the game world)
NavigationGrid::NavigationGrid(QuadTree<GameObject*>* objectTree, Vector3 offset, float maxHeight, Vector2 gridHalfDims, float nodeSize) {
	gridWidth =  (int)gridHalfDims.x * 2;
	gridHeight = (int)gridHalfDims.y * 2;
	this->nodeSize = nodeSize;

	this->offset = offset;

	//add grid dims to centre the grid on offset point.
	this->offset.x += gridHalfDims.x;
	this->offset.z += gridHalfDims.y;

	allNodes = new GridNode[gridWidth * gridHeight];

	for (int x = 0; x < gridWidth; x++) {
		for (int y = 0; y < gridHeight; y++) {
			Ray r = Ray(Vector3(x * nodeSize - this->offset.x, maxHeight, y * nodeSize - this->offset.z), Vector3(0, -1, 0));

			RayCollision closestCollision;
			auto objects = objectTree->GetPossibleRayCollisions(r);

			for (auto o : objects) {
				RayCollision col;
				if (CollisionDetection::RayIntersection(r, *o, col)) {
					if (col.rayDistance < closestCollision.rayDistance) {
						closestCollision = col;
						closestCollision.node = o;
					}
				}
			}

			GridNode& n = allNodes[(gridWidth * y) + x];
			n.position = Vector3((float)(x * nodeSize), closestCollision.collidedAt.y, (float)(y * nodeSize));


			if (closestCollision.node) {
				GameObject* go = (GameObject*)closestCollision.node;
				go->HasTag("traversable") ? n.type = ((TraversableObject*)go)->TraversalType() : n.type = 'x';
			} 
			else {
				n.type = 'x';
			}
		}
	}

	BuildConnections();
}

NavigationGrid::NavigationGrid(const std::string&filename, Vector3 offset) : NavigationGrid() {
	std::ifstream infile(Assets::DATADIR + filename);
	this->offset = offset;
	infile >> nodeSize;
	infile >> gridWidth;
	infile >> gridHeight;

	allNodes = new GridNode[gridWidth * gridHeight];

	for (int y = 0; y < gridHeight; ++y) {
		for (int x = 0; x < gridWidth; ++x) {
			GridNode&n = allNodes[(gridWidth * y) + x];
			char type = 0;
			infile >> type;
			n.type = type;
			n.position = Vector3((float)(x * nodeSize), 0, (float)(y * nodeSize));
		}
	}
	
	BuildConnections();
}

NavigationGrid::~NavigationGrid()	{
	delete[] allNodes;
}

void NavigationGrid::BuildConnections() {
	//Build the connectivity between the nodes
	for (int y = 0; y < gridHeight; ++y) {
		for (int x = 0; x < gridWidth; ++x) {
			GridNode& n = allNodes[(gridWidth * y) + x];

			if (y > 0) { //get the above node

				n.connected[0] = &allNodes[(gridWidth * (y - 1)) + x];

				if (x > 0) {
					n.connected[4] = &allNodes[(gridWidth * (y - 1)) + x - 1];
				}

				if (x < gridWidth - 1) {
					n.connected[5] = &allNodes[(gridWidth * (y - 1)) + x + 1];
				}

			}
			if (y < gridHeight - 1) { //get the below node

				n.connected[1] = &allNodes[(gridWidth * (y + 1)) + x];

				if (x > 0) {
					n.connected[6] = &allNodes[(gridWidth * (y + 1)) + x - 1];
				}

				if (x < gridWidth - 1) {
					n.connected[7] = &allNodes[(gridWidth * (y + 1)) + x + 1];
				}
			}

			if (x > 0) { //get left node
				n.connected[2] = &allNodes[(gridWidth * (y)) + (x - 1)];
			}
			if (x < gridWidth - 1) { //get right node
				n.connected[3] = &allNodes[(gridWidth * (y)) + (x + 1)];
			}

			//diagonals connected to each of the cardinal directions. e.g. left is connected to up-left and down-left.
			std::pair<int, int> diagonalNeighbours[4]{ {4,5},{6,7},{4,6},{5,7} };

			for (int i = 0; i < 4; ++i) {
				if (n.connected[i]) {

					switch (n.connected[i]->type) {
					case '.':
						n.costs[i] = (n.position - n.connected[i]->position).Length();
						break;
					case 'o':
						n.costs[i] = (n.position - n.connected[i]->position).Length() * 5.0f;
						break;
					case 'x':
						n.connected[i] = nullptr;

						//Also disconnect diagonals connected to this neighbour, to avoid ai ramming themselves into walls
						n.connected[diagonalNeighbours[i].first] = nullptr;
						n.connected[diagonalNeighbours[i].second] = nullptr;
						break;
					}					
				}
			}

			for (int i = 4; i < 8; ++i) {
				if (n.connected[i]) {
					switch (n.connected[i]->type) {
						case '.':
							n.costs[i] = (n.position - n.connected[i]->position).Length();
							break;
						case 'o':
							n.costs[i] = (n.position - n.connected[i]->position).Length() * 10.0f;
							break;
						case 'x':
							n.connected[i] = nullptr;
							break;
					}
				}
			}
		}
	}
}

bool NavigationGrid::FindPath(const Vector3& rawFrom, const Vector3& rawTo, NavigationPath& outPath, float maximumCost) {

	Vector3 from = rawFrom + offset;
	Vector3 to   = rawTo + offset;

	//need to work out which node 'from' sits in, and 'to' sits in
	int fromX = ((int)round(from.x / nodeSize));
	int fromZ = ((int)round(from.z / nodeSize));

	//Round before casting to ensure closest node (cast to int will floor the value)
	int toX = ((int)round(to.x / nodeSize));
	int toZ = ((int)round(to.z / nodeSize));

	if (fromX < 0 || fromX > gridWidth - 1 ||
		fromZ < 0 || fromZ > gridHeight - 1) {
		return false; //outside of map region!
	}

	if (toX < 0 || toX > gridWidth - 1 ||
		toZ < 0 || toZ > gridHeight - 1) {
		return false; //outside of map region!
	}

	GridNode* startNode = &allNodes[(fromZ * gridWidth) + fromX];
	GridNode* endNode	= &allNodes[(toZ * gridWidth) + toX];
	
	NodeSet openSet;
	NodeSet closedSet;

	openSet.emplace(startNode);

	startNode->f = 0;
	startNode->g = 0;
	startNode->parent = nullptr;

	GridNode* currentBestNode = nullptr;

	while (!openSet.empty()) {
		currentBestNode = *openSet.begin();
		openSet.erase(openSet.begin());

		if (currentBestNode == endNode) { //we've found the path!
			GridNode* node = endNode;
			outPath.PushWaypoint(rawTo);
			while (node != nullptr) {
				outPath.PushWaypoint(node->position - offset);
				node = node->parent;
			}
			return true;
		}
		else {
			for (int i = 0; i < 8; ++i) {
				GridNode* neighbour = currentBestNode->connected[i];
				if (!neighbour) { //might not be connected...
					continue;
				}	
				bool inClosed	= NodeInSet(neighbour, closedSet);
				if (inClosed) {
					continue; //already discarded this neighbour...
				}

				float h = Heuristic(neighbour, endNode);				
				float g = currentBestNode->g + currentBestNode->costs[i];
				float f = h + g;

				bool inOpen = NodeInSet(neighbour, openSet);

				if (inOpen && f < neighbour->f) {
					openSet.erase(neighbour);
					inOpen = false;
				}

				if (!inOpen) {//might be a better route to this neighbour
					neighbour->parent = currentBestNode;
					neighbour->f = f;
					neighbour->g = g;
				}

				if (!inOpen && neighbour->g < maximumCost) {
					openSet.emplace(neighbour);
				}
			}
			closedSet.emplace(currentBestNode);
		}
	}
	return false; //open list emptied out with no path!
}

bool NavigationGrid::NodeInSet(GridNode* n, NodeSet& set) const {
	NodeSet::iterator i = set.find(n);
	return i == set.end() ? false : true;
}

float NavigationGrid::Heuristic(GridNode* hNode, GridNode* endNode) const {
	return (hNode->position - endNode->position).Length();
}