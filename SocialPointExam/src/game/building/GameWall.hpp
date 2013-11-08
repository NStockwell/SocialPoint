

#ifndef __GameWall_H__
#define __GameWall_H__

#include "game/building/GameWall.fwd.hpp"
#include "game/GameBuilding.hpp"
 
/**
 * A wall type building
 */
class GameWall : public GameBuilding
{
public:

	/**
	 * Constructor
	 */
	GameWall();

	/**
	 * Destructor
	 */
    virtual ~GameWall();
    
    /**
     * @see GameEntity::onTile
     */
    virtual void onTile(const Tile& tile);
};

#endif /* defined(__GameWall_H__) */
