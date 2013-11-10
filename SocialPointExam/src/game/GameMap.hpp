
#ifndef __GameMap_H__
#define __GameMap_H__

#include "game/GameMap.fwd.hpp"
#include "game/GameEntity.fwd.hpp"
#include "game/GameUnit.fwd.hpp"
#include "game/GameBuilding.fwd.hpp"
#include "GameBuildingDestroyable.fwd.hpp"
#include "game/GameMapView.fwd.hpp"
#include <map>
#include <set>

namespace cocos2d {
    class CCNode;
    class CCPoint;
	class CCObject;
}

struct GameMapTile
{
    unsigned x;
    unsigned y;
    
	GameMapTile(unsigned x, unsigned y);
    bool operator<(const GameMapTile& tile) const;
};

struct GameMapTileSize
{
    unsigned width;
    unsigned height;

	GameMapTileSize(unsigned width, unsigned height);
	bool operator<(const GameMapTileSize& size) const;
};

/**
 * Represents the isometric tile map
 */
class GameMap
{
public:
    typedef GameMapTile Tile;
    typedef GameMapTileSize TileSize;

private:
    typedef std::map<Tile, GameEntity*> TileMap;
    typedef std::set<GameEntity*> EntitySet;
    typedef std::set<GameUnit*> UnitSet;
	typedef std::set<GameBuildingDestroyable*> BuildingSet;
    GameMapView* _view;
    EntitySet _entities;
    TileMap _tiles;
	BuildingSet _buildingList;
    UnitSet _selectedUnits;
	GameBuilding* _selectedBuilding;
    bool _ignoreNextTouch;
    bool _debug;
    
    void setEntityTiles(const Tile& tile, GameEntity* entity);
    
public:

	/**
	 * Constructor
	 */
	GameMap();

	/**
	 * Destructor
	 */
    virtual ~GameMap();
    
    /**
     * @return the view of the map
     */
    cocos2d::CCNode* getView();
    
    /**
     * @return the position of a tile
     */
    cocos2d::CCPoint getTilePosition(const Tile& tile) const;
    
    /**
     * @return true if an entity is on the tile
     */
    bool isTileOccupied(const Tile& tile) const;
    
    /**
     * This will fail if there is no entity in the given tile
     * Always check isTileOccupied first
     * @see GameMap::isTileOccupied
     * @return the entity for a given tile
     */
    GameEntity& getTileEntity(const Tile& tile);
    
    /**
     * @param point the point to look for
     * @param tile where the tile is set
     * @return if the position is inside a tile
     */
    bool getPositionTile(const cocos2d::CCPoint& point, Tile& tile) const;
    
    /**
     * @param tile the tile to start from
     * @param free where the free tile is set
     * @return true if free tile found
     */
    bool getFreeTile(const Tile& tile, Tile& free) const;
    
    /**
     * Put the entity inside this map
     */
    void putEntity(const Tile& tile, GameEntity* entity);
    
    /**
     * Called when the map is touched
     * @return true if the touch was handled
     */
    bool onTouch(const Tile& tile);
    
    /**
     * Called when an entity is touched
     */
    bool onEntityTouched(GameEntity& entity);

    /**
     * Add a unit to the selected units list
     */
    void addSelectedUnit(GameUnit& unit);
    
	/**
	 * Selects a building
	 */
	void selectBuilding(GameBuilding& building);

	
	void onBuildingDestroyed(cocos2d::CCObject* obj);

	
	void watchForDestruction(GameEntity* entity);

    /**
     * Enable and disable debug mode
     */
    void toggleDebug();
    
    /**
     * Return the z order for a tile
     */
    int getZOrderForTile(const Tile &tile);

};

#endif /* defined(__GameMap_H__) */
