

#ifndef __GameBuildingDestroyable_H__
#define __GameBuildingDestroyable_H__

//#include "game/GameBuildingDestroyable.fwd.hpp"
#include "game/GameBuilding.hpp"
#include "game/GameEntity.hpp"
#include "game/GameBuildingView.hpp"
#include <string>

#include <cocos2d.h>
 
using namespace cocos2d;

/**
 * Represents a game entity of type building destroyable
 */
class GameBuildingDestroyable : public GameBuilding
{
public:
   
private:
    int _hitPoints;
	bool _destroyed;    
	cocos2d::CCLabelTTF* _hitPointsLabel;
protected:
    
public:

	/**
	 * Constructor
	 */
	GameBuildingDestroyable(const std::string& name, int hitPoints, const TileSize& size=TileSize(1, 1));

	/**
	 * Destructor
	 */
    virtual ~GameBuildingDestroyable();
    
	bool onTouch();

	/**
	 * Returns true if the building has been destroyed
	 */
	bool isDestroyed();

	/**
	 * Removes damage to the hit points of the building. if hitpoints reaches 0, building is destroyed. Returns true if the building is still standing
	 */
    bool attacked(int damage);

};

#endif /* defined(__GameBuildingDestroyable_H__) */
