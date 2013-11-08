
#ifndef __GameHudLayer_H__
#define __GameHudLayer_H__

#include "game/GameHudLayer.fwd.hpp"
#include "game/GameMap.fwd.hpp"
#include <cocos2d.h>
#include <cocos-ext.h>

/**
 * This view shows the gui over the game map
 * @see cocos2d::CCNode
 */
class GameHudLayer : public cocos2d::CCNode
{
private:
    
    GameMap* _map;

    /**
     * Exit selector called on ccb event
     * @see cocos2d::extension::CCBSelectorResolver
     */
    void exitSelector(cocos2d::CCObject* obj, cocos2d::extension::CCControlEvent ev);
    
    /**
     * Debug selector called on ccb event
     * @see cocos2d::extension::CCBSelectorResolver
     */
    void debugSelector(cocos2d::CCObject* obj, cocos2d::extension::CCControlEvent ev);

public:

    /**
     * Constructor
     */
    GameHudLayer();

    /**
     * Destructor
     */
    virtual ~GameHudLayer();

    /**
     * Initialize the view
     */
    bool init(GameMap& map);

    /**
     * Create a new autoreleased view
     * @see cocos2d::CCNode::create
     */
    static GameHudLayer* create(GameMap& map);

};

#endif /* defined(__GameHudLayer_H__) */
