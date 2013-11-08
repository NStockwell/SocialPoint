
#include "game/GameBuildingView.hpp"
#include "game/GameBuilding.hpp"
#include <iomanip>

USING_NS_CC;

/**
 * A building is composed of multiple layers, each one used for 
 * something different. This configuration tells the class where to find
 * the spriteFrames for each layer
 * The different layers are:
 * - Base: the basic building, can have multiple spriteFrames (@see setBaseIndex)
 * - Orientation: the building oriented in different directions, used in cannons or buildings that target
 * - Hit: added when the building is hit
 * - Overlay: an animation that is played over the base building
 */
GameBuildingView::LayerConfigMap GameBuildingView::createLayerConfigMap()
{
	LayerConfigMap config;
	config[Layer::Base] =			LayerConfig("_01", 1);
	config[Layer::Orientation] =	LayerConfig("_02", 2);
	config[Layer::Hit] =			LayerConfig("_03", 3);
	config[Layer::Overlay] =		LayerConfig("_04", 4);
	return config;
}

GameBuildingView::OrientationConfigMap GameBuildingView::createOrientationConfigMap()
{
	OrientationConfigMap config;
	config[Orientation::South] =		OrientationConfig("_01", false);
	config[Orientation::SouthWest] =	OrientationConfig("_05", false);
	config[Orientation::West] =			OrientationConfig("_09", false);
	config[Orientation::NorthWest] =	OrientationConfig("_13", false);
	config[Orientation::North] =		OrientationConfig("_17", false);
	config[Orientation::NorthEast] =	OrientationConfig("_13", true);
	config[Orientation::East] =			OrientationConfig("_09", true);
	config[Orientation::SouthEast] =	OrientationConfig("_05", true);
	return config;
}

const GameBuildingView::LayerConfigMap GameBuildingView::kLayerConfig = GameBuildingView::createLayerConfigMap();
const GameBuildingView::OrientationConfigMap GameBuildingView::kOrientationConfig = GameBuildingView::createOrientationConfigMap();
const float GameBuildingView::kSpriteFramerate = 1.0f/30.0f;

GameBuildingView::GameBuildingView() :
_baseSprite(nullptr),
_orientationSprite(nullptr),
_hitSprite(nullptr),
_overlaySprite(nullptr)
{
}

GameBuildingView::~GameBuildingView()
{
    CC_SAFE_RELEASE(_baseSprite);
    CC_SAFE_RELEASE(_orientationSprite);
    CC_SAFE_RELEASE(_hitSprite);
    CC_SAFE_RELEASE(_overlaySprite);
}

GameBuildingView* GameBuildingView::create(GameBuilding& building, const std::string& name)
{
    GameBuildingView* ret = new GameBuildingView();
    if(ret && ret->init(building, name))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool GameBuildingView::init(GameBuilding& building, const std::string& name)
{
    if(!GameEntityView::init(building))
    {
        return false;
    }
    
    setAnchorPoint(CCPoint(0.5f, 0.0f));
    
    _name = name;
    _spriteFramePrefix = name+"_"+name;
    _orientation = Orientation::South;
    _baseIndex = 1;
    
    std::string path = std::string("buildings/") + name + ".plist";
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(path.c_str());

    loadSprites();
    updateDebugDraw();
    return true;
}

void GameBuildingView::setOrientation(Orientation orientation)
{
    if(_orientation != orientation)
    {
        _orientation = orientation;
        loadOrientationSprite();
        updateSpriteFlip();
    }
}

void GameBuildingView::setBaseIndex(unsigned int i)
{
    if(_baseIndex != i)
    {
        _baseIndex = i;
        loadBaseSprite();
        updateSpriteFlip();
    }
}

bool GameBuildingView::loadBaseSprite()
{
    if(_baseSprite)
    {
        _baseSprite->removeFromParent();
    }
    CC_SAFE_RELEASE(_baseSprite);
    const LayerConfig& config = kLayerConfig.at(Layer::Base);
    std::stringstream spriteFrameName;
    spriteFrameName << _spriteFramePrefix << config.suffix << "_" << std::setfill('0') << std::setw(2) << _baseIndex;
    CCSpriteFrame* spriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(spriteFrameName.str().c_str());
    if(spriteFrame)
    {
        _baseSprite = CCSprite::createWithSpriteFrame(spriteFrame);
        CC_SAFE_RETAIN(_baseSprite);
        setEntityViewBase(_baseSprite, config.zorder);
        return true;
    }
    return false;
}

bool GameBuildingView::loadOrientationSprite()
{
    if(_orientationSprite)
    {
        _orientationSprite->removeFromParent();
    }
    CC_SAFE_RELEASE(_orientationSprite);
    const LayerConfig& config = kLayerConfig.at(Layer::Orientation);
    const OrientationConfig& oconfig = kOrientationConfig.at(_orientation);
    
    std::string spriteFrameName = _spriteFramePrefix+config.suffix+oconfig.suffix;
    CCSpriteFrame* spriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(spriteFrameName.c_str());
    if(spriteFrame)
    {
        _orientationSprite = CCSprite::createWithSpriteFrame(spriteFrame);
        CC_SAFE_RETAIN(_orientationSprite);
        setEntityView(_orientationSprite, config.zorder);
        return true;
    }
    return false;
}

bool GameBuildingView::loadHitSprite()
{
    if(_hitSprite)
    {
        _hitSprite->removeFromParent();
    }
    CC_SAFE_RELEASE(_hitSprite);
    const LayerConfig& config = kLayerConfig.at(Layer::Hit);
    CCAnimation* animation = createAnimation(_spriteFramePrefix+config.suffix, kSpriteFramerate);
    if(animation)
    {
        CCSpriteFrame* spriteFrame = static_cast<CCAnimationFrame*>(animation->getFrames()->objectAtIndex(0))->getSpriteFrame();
        _hitSprite = CCSprite::createWithSpriteFrame(spriteFrame);
        CC_SAFE_RETAIN(_hitSprite);
        _hitSprite->runAction(CCRepeatForever::create(CCAnimate::create(animation)));
        setEntityView(_hitSprite, config.zorder);
        return true;
    }
    return false;
}

bool GameBuildingView::loadOverlaySprite()
{
    if(_overlaySprite)
    {
        _overlaySprite->removeFromParent();
    }
    CC_SAFE_RELEASE(_overlaySprite);
    const LayerConfig& config = kLayerConfig.at(Layer::Overlay);
    CCAnimation* animation = createAnimation(_spriteFramePrefix+config.suffix, kSpriteFramerate);
    if(animation)
    {
        CCSpriteFrame* spriteFrame = static_cast<CCAnimationFrame*>(animation->getFrames()->objectAtIndex(0))->getSpriteFrame();
        _overlaySprite = CCSprite::createWithSpriteFrame(spriteFrame);
        CC_SAFE_RETAIN(_overlaySprite);
        _overlaySprite->runAction(CCRepeatForever::create(CCAnimate::create(animation)));
        setEntityView(_overlaySprite, config.zorder);
        return true;
    }
    return false;
}

void GameBuildingView::updateSpriteFlip()
{
    const OrientationConfig& oconfig = kOrientationConfig.at(_orientation);
    
    if(_baseSprite)
    {
        _baseSprite->setFlipX(oconfig.flipped);
    }
    if(_orientationSprite)
    {
        _orientationSprite->setFlipX(oconfig.flipped);
    }
    if(_hitSprite)
    {
        _hitSprite->setFlipX(oconfig.flipped);
    }
    if(_overlaySprite)
    {
        _overlaySprite->setFlipX(oconfig.flipped);
    }
}

void GameBuildingView::loadSprites()
{
    loadBaseSprite();
    loadOrientationSprite();
    loadHitSprite();
    loadOverlaySprite();
}

GameBuildingViewOrientationConfig::GameBuildingViewOrientationConfig(const std::string& suffix_, bool flipped_) :
suffix(suffix_), flipped(flipped_)
{
};


GameBuildingViewLayerConfig::GameBuildingViewLayerConfig(const std::string& suffix_, int zorder_) :
suffix(suffix_), zorder(zorder_)
{
}
