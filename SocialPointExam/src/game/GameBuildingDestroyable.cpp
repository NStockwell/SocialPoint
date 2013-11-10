
#include "game/GameBuildingDestroyable.hpp"
#include "game/GameBuilding.hpp"
#include "game/GameBuildingView.hpp"

GameBuildingDestroyable::GameBuildingDestroyable(const std::string& name, int hitPoints, const TileSize& size):GameBuilding(name,size)
{
	_hitPoints = hitPoints;
	_destroyed = false;
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
	if(_hitPoints <=0)
	{
		_destroyed = true;
		getBuildingView()->setVisible(false);
		_hitPoints = 0;
		cocos2d::CCNotificationCenter::sharedNotificationCenter()->postNotification("DESTROYED",(cocos2d::CCObject*)this);
	}
	return true;
}