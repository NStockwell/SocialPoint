
#ifndef __GameUnit_H__
#define __GameUnit_H__

#include "game/GameUnit.fwd.hpp"
#include "game/GameUnitView.fwd.hpp"
#include "game/GameEntity.hpp"
#include "game/GameMap.fwd.hpp"
#include <string>

namespace cocos2d {
    class CCPoint;
}
 
/**
 * This class repesents a game entity that moves and can attack
 */
class GameUnit : public GameEntity
{
public:
    typedef GameMapTile Tile;
    typedef GameUnitOrientation::Type Orientation;
private:
    GameUnitView* _view;
public:

	/**
	 * Constructor
	 */
	GameUnit(const std::string& name);

	/**
	 * Destructor
	 */
    virtual ~GameUnit();

    /**
     * Move the unit to a point
     */
    void moveTo(const Tile& tile);
    
    /**
     * @see GameEntity::onTouch
     */
    bool onTouch();
    
    void setOrientation(Orientation orientation);
};

#endif /* defined(__GameUnit_H__) */
