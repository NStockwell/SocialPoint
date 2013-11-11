#include "PathFinder.hpp"
#include <cocos2d.h>
#include <vector>

using namespace std;
USING_NS_CC;

PathFinder::PathFinder(GameMap* map)
{
	_map = map;
}


PathFinder::~PathFinder(void)
{
}

void PathFinder::findPath(CCPoint origin, CCPoint dest)
{
	aStar(origin, dest);
}

int PathFinder::manhattanDistance(CCPoint origin, CCPoint dest)
{
	int xDiff = dest.x - origin.x;
	int yDiff = dest.y - origin.y;

	return xDiff*xDiff + yDiff*yDiff;
}


vector<CCPoint*> PathFinder::aStar(CCPoint origin, CCPoint dest)
{
    vector<CCPoint*> path;

    // Define points to work with
	GraphNode *start = new GraphNode(origin.x, origin.y);
    GraphNode *end = new GraphNode(dest.x, dest.y);
    GraphNode *current;
    GraphNode *child;

    // Define the open and the close list
    list<GraphNode*> openList;
    list<GraphNode*> closedList;
    list<GraphNode*>::iterator i;

    unsigned int n = 0;

    // Add the start point to the openList
    openList.push_back(start);
    start->opened = true;

    while (n == 0 || (current != end && n < 50))
    {
        // Look for the smallest value in the openList and make it the current point
        for (i = openList.begin(); i != openList.end(); ++ i)
        {
			if (i == openList.begin() || manhattanDistance((*i)->point, end->point) <= manhattanDistance(current->point, end->point))
            {
                current = (*i);
            }
        }

        // Stop if we reached the end
        if (current == end)
        {
            break;
        }

        // Remove the current point from the openList
        openList.remove(current);
        current->opened = false;

        // Add the current point to the closedList
        closedList.push_back(current);
        current->closed = true;

        // Get all current's adjacent walkable points
        for (int x = -1; x < 2; x ++)
        {
            for (int y = -1; y < 2; y ++)
            {
                // If it's current point then pass
                if (x == 0 && y == 0)
                {
                    continue;
                }

				// Get this point
				CCPoint childPoint = CCPointMake(current->getX() + x, current->getY() + y);
				child = getPoint(childPoint,openList, closedList);
					
                
                // If it's closed or not walkable then pass
                if (child->closed || !pointIsWalkable(child))
                {
                    continue;
                }

                // If we are at a corner
                if (x != 0 && y != 0)
                {
					GraphNode* neighbour = getPoint(CCPointMake(current->getX(), current->getY() + y), openList, closedList);
                    // if the next horizontal point is not walkable or in the closed list then pass
                    if (!pointIsWalkable(current->getX(), current->getY() + y) || neighbour->closed)
                    {
                        continue;
                    }
					neighbour = getPoint(CCPointMake(current->getX() +x, current->getY()), openList, closedList);
                   
                    // if the next vertical point is not walkable or in the closed list then pass
                    if (!pointIsWalkable(current->getX() + x, current->getY()) || neighbour->closed)
                    {
                        continue;
                    }
                }

                // If it's already in the openList
                if (child->opened)
                {
                    // If it has a wroste g score than the one that pass through the current point
                    // then its path is improved when it's parent is the current point
					if (manhattanDistance( child->point, end->point)/*getGScore()*/ > manhattanDistance(current->point, end->point))// child->getGScore(current))
                    {
                        // Change its parent and g score
                        child->setParent(current);
                        //child->computeScores(end);
                    }
                }
                else
                {
                    // Add it to the openList with current point as parent
                    openList.push_back(child);
                    child->opened = true;

                    // Compute it's g, h and f score
                    child->setParent(current);
                    //child->computeScores(end);
                }
            }
        }

        n ++;
    }

    // Reset
    for (i = openList.begin(); i != openList.end(); ++ i)
    {
        (*i)->opened = false;
    }
    for (i = closedList.begin(); i != closedList.end(); ++ i)
    {
        (*i)->closed = false;
    }

    // Resolve the path starting from the end point
    while (current->hasParent() && current != start)
    {
        path.push_back(&(current->point));
        current = current->getParent();
        n ++;
    }

    return path;
}


bool PathFinder::pointIsWalkable(GraphNode* current)
{
	return pointIsWalkable(current->getX(), current->getY());
}

bool PathFinder::pointIsWalkable(float x, float y)
{
	GameMapTile *t = new GameMapTile(0,0);
	bool walkable = false;
	if(_map->getPositionTile(CCPointMake(x,y), *t))
	{
		GameMapTile *free = new GameMapTile(0,0);
		_map->getFreeTile(*t,*free);
		if(free->x == t->x && free->y == t->y)
			walkable = true;
	}
	return walkable;
}

GraphNode* PathFinder::getPoint(CCPoint point, list<GraphNode*> openList, list<GraphNode*> closedList)
{
	GraphNode* node;
	for (list<GraphNode*>::iterator iter = openList.begin(); iter != openList.end(); ++ iter)
	{
		if((*iter)->point.equals(point))
		{
			node = *iter;
			return node;
		}
	}

	for (list<GraphNode*>::iterator iter = closedList.begin(); iter != closedList.end(); ++ iter)
	{
		if((*iter)->point.equals(point))
		{
			node = *iter;
			return node;
		}
	}

	return new GraphNode(point.x, point.y);

}