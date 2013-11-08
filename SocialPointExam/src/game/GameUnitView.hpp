
#ifndef __GameUnitView_H__
#define __GameUnitView_H__

#include "game/GameUnitView.fwd.hpp"
#include "game/GameUnit.fwd.hpp"
#include "game/GameEntityView.hpp"

/**
 * Represents a combination of an orientation
 * and an animation of a game unit
 */
struct GameUnitViewAnimationOrientation
{
	typedef GameUnitOrientation::Type Orientation;
    typedef GameUnitViewAnimation::Type Animation;

    Animation animation;
    Orientation orientation;

    bool operator==(const GameUnitViewAnimationOrientation& other) const;
    bool operator<(const GameUnitViewAnimationOrientation& other) const;
    GameUnitViewAnimationOrientation(Animation animation, Orientation orientation);
};

/**
 * Represents the configuration for an orientation and an animation of a game unit
 */
struct GameUnitViewAnimationConfig
{
    std::string suffix;
    bool flipped;

	GameUnitViewAnimationConfig(const std::string& suffix="", bool flipped=false);
};

/**
 * The view of a game unit
 */
class GameUnitView : public GameEntityView
{
public:
    typedef GameUnitOrientation::Type Orientation;
    typedef GameUnitViewAnimation::Type Animation;
    typedef GameUnitViewAnimationConfig AnimationConfig;
    typedef GameUnitViewAnimationOrientation AnimationOrientation;
    typedef std::map<AnimationOrientation, AnimationConfig> AnimationConfigMap;
private:
    
    /**
     * The configuration for the animations of the unit
     * Each orientation and animation has a suffix that is added to the
     * name to find the spriteFrames of the orientation
     * @see GameUnitViewAnimationOrientation
     * @see GameUnitViewAnimationConfig
     */
    static const AnimationConfigMap kAnimationsConfig;
    
    /**
     * The framerate of the animations
     */
    static const float kSpriteFramerate;
    
    
    Orientation _orientation;
    std::string _name;
    Animation _animation;
    float _moveSpeed;
    GameUnit* _unit;
    cocos2d::CCAction* _moveAction;
    cocos2d::CCSprite* _sprite;
    
    void loadSprite();
    void onMovedTo();
	static AnimationConfigMap setupAnimationConfigMap();
public:
    GameUnitView();
    virtual ~GameUnitView();
    
    /**
     * Standard cocos initializers
     * @see http://www.cocos2d-x.org/wiki/Reference_Count_and_AutoReleasePool_in_Cocos2d-x
     */
    static GameUnitView* create(GameUnit& unit, const std::string& name);
    bool init(GameUnit& unit, const std::string& name);
    
    /**
     * Set the orientation of the unit
     */
    void setOrientation(Orientation orientation);
    
    /**
     * Set the animation of the unit
     */
    void setAnimation(Animation animation);
    
    /**
     * Set the speed in pixels per second of the unit
     * @param speed 
     */
    void setMoveSpeed(float speed);
    
    /**
     * Moves the unit to the given point and sets the Idle animation at the end
     */
    void moveTo(const cocos2d::CCPoint& point);
    
    /**
     * Returns a unit orientation for a given angle
     */
    static Orientation orientationForAngle(float radians);
};

#endif /* defined(__GameUnitView_H__) */
