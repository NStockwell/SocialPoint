
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
	unsigned short _range;
	unsigned short _power;
	unsigned short _hitPoints;

public:

	/**
	 * Constructor
	 */
	GameUnit(const std::string& name, unsigned short hitPoints, unsigned short power = 1, unsigned short range = 1);

	/**
	 * Destructor
	 */
    virtual ~GameUnit();

    /**
     * Move the unit to a point
     */
    void moveTo(const Tile& tile);
    

	/**
	 * Move the unit to a point and attacks the point tileToAttack
	 */
	void moveToAndAttack(const Tile& tile, const Tile& tileToAttack);


    /**
     * @see GameEntity::onTouch
     */
    bool onTouch();
    
    void setOrientation(Orientation orientation);
};

#endif /* defined(__GameUnit_H__) */
