
#include "game/GameBuildingDestroyable.hpp"
#include "game/GameBuilding.hpp"
#include "game/GameBuildingView.hpp"

GameBuildingDestroyable::GameBuildingDestroyable(const std::string& name, int hitPoints, const TileSize& size):GameBuilding(name,size)
{
	_hitPoints = hitPoints;
	_destroyed = false;
	
	_hitPointsLabel = CCLabelTTF::create(std::to_string(_hitPoints).c_str(), "Helvetica", 12, 
                                      CCSizeMake(245, 32), kCCTextAlignmentCenter);

	GameBuildingView* gbv = getBuildingView();
	_hitPointsLabel->setPosition(CCPointMake( gbv->getPositionX() + gbv->getContentSize().width/2, gbv->getPositionY() + gbv->getContentSize().height)),
	getBuildingView()->addChild(_hitPointsLabel);
}

GameBuildingDestroyable::~GameBuildingDestroyable()
{
}


bool GameBuildingDestroyable::onTouch()
{
	if(_destroyed)
		return true;
    //if(GameEntity::onTouch())
    {
      //  return true;
    }
    getMap().selectBuilding(*this);
    return true;
}


bool GameBuildingDestroyable::isDestroyed()
{
	return _destroyed;
}

	
bool GameBuildingDestroyable::attacked(int damage)
{
	if(_destroyed)
		return false;


	_hitPoints -= damage;
	_hitPointsLabel->setString(std::to_string(_hitPoints).c_str());
	if(_hitPoints <=0)
	{
		_destroyed = true;
		getBuildingView()->setVisible(false);
		_hitPoints = 0;
		cocos2d::CCNotificationCenter::sharedNotificationCenter()->postNotification("DESTROYED",(cocos2d::CCObject*)this);
	}
	return true;
}