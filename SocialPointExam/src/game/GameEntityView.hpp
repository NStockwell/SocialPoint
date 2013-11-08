
#ifndef __GameEntityView_H__
#define __GameEntityView_H__

#include "game/GameEntityView.fwd.hpp"
#include "game/GameEntity.fwd.hpp"
#include <cocos2d.h>
 
/**
 * Generic game entity view
 * Implements touch detection and basic view stuff
 */
class GameEntityView : public cocos2d::CCNode, public cocos2d::CCTouchDelegate
{
private:
    GameEntity* _entity;
    bool _touched;
    cocos2d::CCDrawNode* _debugDraw;
    cocos2d::CCNode* _content;
    cocos2d::CCNode* _base;
    bool _debugDrawEnabled;
    cocos2d::CCGLProgram* _normalShader;
    cocos2d::CCGLProgram* _selectedShader;
    static const char* kSelectedShaderCacheKey;

    /**
     * The priority on wich all entity touches are set
     * Lower number means higher priority
     * This priority should be higher than the GameMapView::kTouchPriority
     * so that the entities receive the touches before the map
     *
     * @see cocos2d::CCTouchDispatcher::addStandardDelegate
     * @see GameMapView::kTouchPriority
     */
    static const int kTouchPriority;
    
    /**
     * The scale to which each content node added to this
     * view will be scaled
     */
    static const float kContentScale;
    
    /**
     * The height in pixels where the content nodes have the
     * Origin
     */
    static const float kContentHeightOffset;
    
    /**
     * The height in pixels where the view should have the origin
     */
    static const float kViewHeightOffset;
protected:
    
    /**
     * Updated the debug draw to match the anchor point and size of the view
     */
    void updateDebugDraw();
    
    /**
     * Adds a base view to the view, the view will be resized to the
     * size of the base
     */
    void setEntityViewBase(cocos2d::CCNode* base, int zorder=0);
    
    /**
     * Adds a view to the view the view will be placed so that
     * the width*0.5 and the height-kContentHeightOffset are in the
     * anchor point of this view which will be set to width*0.5
     * height-kViewHeightOffset
     */
    void setEntityView(cocos2d::CCNode* layer, int zorder=0);
    
    /**
     * Creates an animation searching for spriteframes that start with a given name
     * With the format of name_01, name_02, name_03, etc
     */
    static cocos2d::CCAnimation* createAnimation(const std::string& name, float frameRate);
public:

	/**
	 * Constructor
	 */
	GameEntityView();

	/**
	 * Destructor
	 */
    virtual ~GameEntityView();
    
    /**
     * Standard cocos initializers
     * @see http://www.cocos2d-x.org/wiki/Reference_Count_and_AutoReleasePool_in_Cocos2d-x
     */
    bool init(GameEntity& entity);
    static GameEntityView* create(GameEntity& entity);

    /**
     * Called by the cocos touch dispatcher
     */
    bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    
    /**
     * Shows or hides the debug draw
     */
    void setDebugDraw(bool enabled);
    
    /**
     * Start and stop receiving touch notifications
     */
    void start();
    void stop();
    
    /**
     * Called when the entity is selected
     */
    virtual void setSelected(bool selected);
};

#endif /* defined(__GameEntityView_H__) */
