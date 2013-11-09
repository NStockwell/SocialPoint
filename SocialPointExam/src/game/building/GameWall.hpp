

#ifndef __GameWall_H__
#define __GameWall_H__

#include "game/building/GameWall.fwd.hpp"
#include "game/GameBuildingDestroyable.hpp"
 
/**
 * A wall type building
 */
class GameWall : public GameBuildingDestroyable
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
