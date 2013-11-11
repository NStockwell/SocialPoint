#pragma once


#include "game/GameMap.hpp"
#include <cocos2d.h>

USING_NS_CC;

using namespace std;

struct GraphNode
{
	CCPoint point;
	GraphNode* parent;
	bool opened;
	bool closed;
    
	GraphNode(unsigned x, unsigned y){point = CCPointMake(x,y); parent = nullptr; opened = true; closed = false;}
	float getX() {return point.x;}
	float getY() {return point.y;}
	bool operator==(const GraphNode& node) const {return (point.x == node.point.x && point.y == node.point.y);}
	void setParent(GraphNode* p) {parent = p;}
	GraphNode* getParent(){return parent;}
	bool hasParent(){return parent != nullptr;}
};


class PathFinder
{
private :
	GameMap* _map;
	
	int manhattanDistance(CCPoint origin, CCPoint dest);
	
	std::vector<CCPoint*> aStar(CCPoint origin, CCPoint dest);
	bool pointIsWalkable(GraphNode* current);
	bool pointIsWalkable(float x, float y);		
	GraphNode* getPoint(CCPoint point, list<GraphNode*> openList, list<GraphNode*> closedList);

public:
	PathFinder(GameMap* map);
	~PathFinder(void);

	void findPath(CCPoint origin, CCPoint dest);
};

