
#ifndef __GameMapView_H__
#define __GameMapView_H__

#include "game/GameMapView.fwd.hpp"
#include <cocos2d.h>
#include <functional>

/**
 * The game map view
 * @see cocos2d::CCNode
 */
class GameMapView : public cocos2d::CCNode, public cocos2d::CCTouchDelegate
{
public:

    typedef GameMapTile Tile;
    
    /**
     * The amount of tiles in each direction
     */
    static const unsigned kTileAmount;
    
    /**
     * The size of a tile
     */
    static const cocos2d::CCSize kTileSize;
    
private:
    
    static const int kTouchPriority;
    
    cocos2d::CCDrawNode* _debugDraw;
    cocos2d::CCNode* _background;
    GameMap* _map;
    bool _touched;
    bool _debugDrawEnabled;
    Tile _touchTile;
    
    /**
     * The point where the top tile is
     */
    static const cocos2d::CCPoint kTileOffset;
    
    void updateDebugDraw();

public:

    /**
     * Constructor
     */
    GameMapView();

    /**
     * Destructor
     */
    virtual ~GameMapView();

    /**
     * Initialize the view
     * @see http://www.cocos2d-x.org/wiki/Reference_Count_and_AutoReleasePool_in_Cocos2d-x
     */
    bool init(GameMap& map);

    /**
     * Create a new autoreleased view
     * @see cocos2d::CCNode::create
     * @see http://www.cocos2d-x.org/wiki/Reference_Count_and_AutoReleasePool_in_Cocos2d-x
     */
    static GameMapView* create(GameMap& map);

    
    /**
     * Get the coordinates of a tile inside the map
     *
     *                 (0,0)
     *                   /\
     *                y /  \ x
     *                 /    \
     *                 \    /
     *                  \  /
     *                   \/
     */
    cocos2d::CCPoint getTilePosition(const Tile& tile) const;
    
    /**
     * @param p the position
     * @param tile where the tile will be set
     * @return true if the position is inside a tile
     */
    bool getPositionTile(const cocos2d::CCPoint& p, Tile& tile) const;
    
    /**
     * Adds a node to a tile
     */
    void addTileChild(const Tile& tile, cocos2d::CCNode* child);
    
    /**
     * Receive touches
     */
    bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    
    void start();
    void stop();
    
    void setDebugDraw(bool enabled);

};

#endif /* defined(__GameMapView_H__) */
