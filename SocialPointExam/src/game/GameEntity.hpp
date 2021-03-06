
#ifndef __GameEntity_H__
#define __GameEntity_H__

#include "game/GameEntity.fwd.hpp"
#include "game/GameEntityView.fwd.hpp"
#include "game/GameMap.fwd.hpp"

namespace cocos2d {
    class CCNode;
}
 
/**
 * Represents any object inside the game map
 */
class GameEntity
{
public:
    typedef GameMapTileSize TileSize;
    typedef GameMapTile Tile;
private:
    GameEntityView* _view;
    GameMap* _map;
	int _hitPoints;
	bool _destroyed; 
    
protected:
    void setView(GameEntityView* view);
    
    GameMap& getMap();
    const GameMap& getMap() const;
    
    void setZOrderForTile(const Tile& tile);
    
public:

	/**
	 * Constructor
	 */
	GameEntity();

	/**
	 * Destructor
	 */
    virtual ~GameEntity();

    /**
     * @return the node that will be added to the game view
     */
    GameEntityView* getView();
    
    /**
     * The entity gets a reference to the map when it 
     * is put inside it
     */
    void setMap(GameMap& map);

	/**
	 * Sets if current entoty is destroyed
	*/ 	 
	void setDestroyed(bool destroyed);

	/**
	* The hitpoints of this entity
	*/
	void setHitPoints(int hitPoints);
    
	 /**
	 * Returns true if the building has been destroyed
	 */
	 virtual bool isDestroyed();

    /**
     * Called when the entity was touched
     * @return true if the touch was handled
     */
    virtual bool onTouch();

    /**
     * Called when the entity is set on a tile
     */
    virtual void onTile(const Tile& tile);
    
    /**
     * @return the size in tiles of the entity
     */
    virtual const TileSize& getTileSize();
    
    /**
     * Called when the entity is selected
     */
    virtual void setSelected(bool selected);

	/**
	 * Called when this entity is being attacked.
	 * @return true if this entity was attacked successfully (still had hitpoints to be removed)
	 */
	virtual bool attacked(int power);
};

#endif /* defined(__GameEntity_H__) */
