

#ifndef __GameBuildingView_H__
#define __GameBuildingView_H__

#include "game/GameBuildingView.fwd.hpp"
#include "game/GameBuilding.fwd.hpp"
#include "game/GameEntityView.hpp"

/**
 * This is the configuration for a building orientation
 */
struct GameBuildingViewOrientationConfig
{
    std::string suffix;
    bool flipped;

	GameBuildingViewOrientationConfig(const std::string& suffix="", bool flipped=false);
};

/**
 * This is the configuration for a building layer
 */
struct GameBuildingViewLayerConfig
{
    std::string suffix;
    int zorder;

	GameBuildingViewLayerConfig(const std::string& suffix="", int zorder=0);
};

/**
 * The view for any entity of type building
 * @see GameBuilding
 */
class GameBuildingView : public GameEntityView
{
public:
    typedef GameBuildingViewLayer::Type Layer;
    typedef GameBuildingOrientation::Type Orientation;
    typedef GameBuildingViewLayerConfig LayerConfig;
    typedef GameBuildingViewOrientationConfig OrientationConfig;
    typedef std::map<Layer, LayerConfig> LayerConfigMap;
    typedef std::map<Orientation, OrientationConfig> OrientationConfigMap;
private:
    static const LayerConfigMap kLayerConfig;
    static const OrientationConfigMap kOrientationConfig;
    static const float kSpriteFramerate;

	static LayerConfigMap createLayerConfigMap();
	static OrientationConfigMap createOrientationConfigMap();
    
    std::string _name;
    std::string _spriteFramePrefix;
    Orientation _orientation;
    GameBuilding* _building;
    cocos2d::CCSprite* _baseSprite;
    cocos2d::CCSprite* _orientationSprite;
    cocos2d::CCSprite* _hitSprite;
    cocos2d::CCSprite* _overlaySprite;
    float _heightOffset;
    unsigned _baseIndex;
    
    void loadSprites();
    bool loadBaseSprite();
    bool loadOrientationSprite();
    bool loadHitSprite();
    bool loadOverlaySprite();
    void updateSpriteFlip();
public:
    GameBuildingView();
    virtual ~GameBuildingView();
    
    /**
     * Standard cocos initializers
     * @see http://www.cocos2d-x.org/wiki/Reference_Count_and_AutoReleasePool_in_Cocos2d-x
     */
    static GameBuildingView* create(GameBuilding& building, const std::string& name);
    bool init(GameBuilding& building, const std::string& name);
    
    /**
     * Change ther building orientation
     */
    void setOrientation(Orientation orientation);
    
    /**
     * Change the building base index
     * This will change the base sprite, it is used in buildings that
     * Can have different appearances (like a gold storage that gets filled up)
     * @see GameWall
     */
    void setBaseIndex(unsigned i);
};

#endif /* defined(__GameBuildingView_H__) */
