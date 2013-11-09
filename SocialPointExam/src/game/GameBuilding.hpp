

#ifndef __GameBuilding_H__
#define __GameBuilding_H__

#include "game/GameBuilding.fwd.hpp"
#include "game/GameBuildingView.fwd.hpp"
#include "game/GameEntity.hpp"
#include "game/GameMap.hpp"
#include <string>
 
/**
 * Represents a game entity of type building
 * Can be subclassed to implement each building behaviour
 */
class GameBuilding : public GameEntity
{
public:
    typedef GameBuildingOrientation::Type Orientation;
private:
    GameBuildingView* _view;
    TileSize _size;
    
protected:
    
    GameBuildingView* getBuildingView();
public:

	/**
	 * Constructor
	 */
	GameBuilding(const std::string& name, const TileSize& size=TileSize(1, 1));

	/**
	 * Destructor
	 */
    virtual ~GameBuilding();
    
	/**
     * @see GameEntity::onTouch
     */
    bool onTouch();

    /**
     * @return the size in tiles of the building
     */
    virtual const TileSize& getTileSize();
};

#endif /* defined(__GameBuilding_H__) */
