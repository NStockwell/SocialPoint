
#ifndef __GameController_H__
#define __GameController_H__

#include "game/GameHudLayer.fwd.hpp"
#include "game/GameMap.hpp"
#include <cocos2d.h>

/**
 * the scene of the game
 * @see cocos2d::CCScene
 */
class GameScene : public cocos2d::CCScene
{
public:
    typedef GameMapTile Tile;
    typedef GameMapTileSize TileSize;
private:
    GameMap _map;
    GameHudLayer* _hud;

public:

    /**
     * Constructor
     */
    GameScene();

    /**
     * Destructor
     */
    virtual ~GameScene();

    /**
     * Standard cocos initializers
     * @see http://www.cocos2d-x.org/wiki/Reference_Count_and_AutoReleasePool_in_Cocos2d-x
     */
    static GameScene* create();
    bool init();
};

#endif /* defined(__GameController_H__) */
