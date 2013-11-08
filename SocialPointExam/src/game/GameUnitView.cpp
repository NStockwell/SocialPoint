

#include "game/GameUnitView.hpp"
#include "game/GameUnit.hpp"

USING_NS_CC;

GameUnitView::AnimationConfigMap GameUnitView::setupAnimationConfigMap()
{
	AnimationConfigMap config;
	config[AnimationOrientation(Animation::Idle, Orientation::SouthEast)]	= AnimationConfig("_01", false);
	config[AnimationOrientation(Animation::Idle, Orientation::SouthWest)]	= AnimationConfig("_01", true);
	config[AnimationOrientation(Animation::Idle, Orientation::South)]		= AnimationConfig("_02", false);
	config[AnimationOrientation(Animation::Idle, Orientation::East)]		= AnimationConfig("_03", false);
	config[AnimationOrientation(Animation::Idle, Orientation::West)]		= AnimationConfig("_03", true);
	config[AnimationOrientation(Animation::Idle, Orientation::North)]		= AnimationConfig("_04", false);
	config[AnimationOrientation(Animation::Idle, Orientation::NorthWest)]	= AnimationConfig("_05", true);
	config[AnimationOrientation(Animation::Idle, Orientation::NorthEast)]	= AnimationConfig("_06", false);
	
	config[AnimationOrientation(Animation::Move, Orientation::SouthEast)]	= AnimationConfig("_11", false);
	config[AnimationOrientation(Animation::Move, Orientation::SouthWest)]	= AnimationConfig("_11", true);
	config[AnimationOrientation(Animation::Move, Orientation::South)]		= AnimationConfig("_11", false);
	config[AnimationOrientation(Animation::Move, Orientation::East)]		= AnimationConfig("_08", false);
	config[AnimationOrientation(Animation::Move, Orientation::West)]		= AnimationConfig("_08", true);
	config[AnimationOrientation(Animation::Move, Orientation::North)]		= AnimationConfig("_07", false);
	config[AnimationOrientation(Animation::Move, Orientation::NorthWest)]	= AnimationConfig("_07", true);
	config[AnimationOrientation(Animation::Move, Orientation::NorthEast)]	= AnimationConfig("_07", false);

	config[AnimationOrientation(Animation::Attack, Orientation::SouthEast)]	= AnimationConfig("_14", false);
	config[AnimationOrientation(Animation::Attack, Orientation::SouthWest)]	= AnimationConfig("_14", true);
	config[AnimationOrientation(Animation::Attack, Orientation::South)]		= AnimationConfig("_14", false);
	config[AnimationOrientation(Animation::Attack, Orientation::East)]		= AnimationConfig("_13", false);
	config[AnimationOrientation(Animation::Attack, Orientation::West)]		= AnimationConfig("_13", true);
	config[AnimationOrientation(Animation::Attack, Orientation::North)]		= AnimationConfig("_12", false);
	config[AnimationOrientation(Animation::Attack, Orientation::NorthWest)]	= AnimationConfig("_12", true);
	config[AnimationOrientation(Animation::Attack, Orientation::NorthEast)]	= AnimationConfig("_12", false);

	return config;
}

const GameUnitView::AnimationConfigMap GameUnitView::kAnimationsConfig = GameUnitView::setupAnimationConfigMap();
const float GameUnitView::kSpriteFramerate = 1.0f/30.0f;

GameUnitView::GameUnitView() :
_moveAction(nullptr), _sprite(nullptr)
{
}

GameUnitView::~GameUnitView()
{
    CC_SAFE_RELEASE(_moveAction);
    CC_SAFE_RELEASE(_sprite);
}

GameUnitView* GameUnitView::create(GameUnit& unit, const std::string& name)
{
    GameUnitView* ret = new GameUnitView();
    if(ret && ret->init(unit, name))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool GameUnitView::init(GameUnit& unit, const std::string& name)
{
    if(!GameEntityView::init(unit))
    {
        return false;
    }
    
    setAnchorPoint(CCPoint(0.5f, 0.0f));
    _orientation = Orientation::South;
    _animation = Animation::Idle;
    _name = name;
    _moveSpeed = 100.0f;
    _unit = &unit;
    
    std::string path = std::string("units/")+_name+".plist";
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(path.c_str());

    loadSprite();
    
    return true;
}

void GameUnitView::loadSprite()
{
    if(_sprite)
    {
        _sprite->removeFromParent();
        CC_SAFE_RELEASE(_sprite);
    }
    
    const AnimationConfig& config = kAnimationsConfig.at(AnimationOrientation(_animation, _orientation));
    CCAnimation* animation = createAnimation(_name+"_"+_name+config.suffix, kSpriteFramerate);
    assert(animation);
    CCSpriteFrame* spriteFrame = static_cast<CCAnimationFrame*>(animation->getFrames()->objectAtIndex(0))->getSpriteFrame();
    _sprite = CCSprite::createWithSpriteFrame(spriteFrame);
    CC_SAFE_RETAIN(_sprite);
    _sprite->setFlipX(config.flipped);
    _sprite->runAction(CCRepeatForever::create(CCAnimate::create(animation)));
    setEntityViewBase(_sprite);
    updateDebugDraw();
}

void GameUnitView::setOrientation(Orientation orientation)
{
    if(_orientation != orientation)
    {
        _orientation = orientation;
        loadSprite();
    }
}

void GameUnitView::setAnimation(Animation animation)
{
    if(_animation != animation)
    {
        _animation = animation;
        loadSprite();
    }
}

void GameUnitView::setMoveSpeed(float speed)
{
    _moveSpeed = speed;
}

void GameUnitView::moveTo(const cocos2d::CCPoint& point)
{
    float duration = ccpDistance(getPosition(), point)/_moveSpeed;
    stopAllActions();
    if(_moveAction)
    {
        _moveAction->stop();
        CC_SAFE_RELEASE(_moveAction);
    }
    _moveAction = CCSequence::create(
        CCMoveTo::create(duration, point),
        CCCallFunc::create(this, callfunc_selector(GameUnitView::onMovedTo)),
        NULL
    );
    CC_SAFE_RETAIN(_moveAction);
    runAction(_moveAction);
}

void GameUnitView::onMovedTo()
{
    setAnimation(Animation::Idle);
}

GameUnitView::Orientation GameUnitView::orientationForAngle(float radians)
{
    int a = radians / (M_PI/8.0f);
    if(a >= -1 && a < 1)
    {
        return Orientation::East;
    }
    else if(a >= 1 && a < 4)
    {
        return Orientation::NorthEast;
    }
    else if(a >= 4 && a < 7)
    {
        return Orientation::NorthWest;
    }
    else if((a >= 7 && a <= 8) || ( a >= -8 && a < -7))
    {
        return Orientation::West;
    }
    else if(a >= -7 && a < -4)
    {
        return Orientation::SouthWest;
    }
    else if(a >= -4 && a < -1)
    {
        return Orientation::SouthEast;
    }
    else
    {
        assert(false);
    }
}

bool GameUnitViewAnimationOrientation::operator==(const GameUnitViewAnimationOrientation& other) const
{
    return animation==other.animation && orientation==other.orientation;
}

bool GameUnitViewAnimationOrientation::operator<(const GameUnitViewAnimationOrientation& other) const
{
    if(animation != other.animation)
    {
        return animation < other.animation;
    }
    else
    {
        return orientation < other.orientation;
    }
}

GameUnitViewAnimationOrientation::GameUnitViewAnimationOrientation(Animation animation_, Orientation orientation_) :
animation(animation_), orientation(orientation_)
{
}

GameUnitViewAnimationConfig::GameUnitViewAnimationConfig(const std::string& suffix_, bool flipped_) :
suffix(suffix_), flipped(flipped_)
{
}
